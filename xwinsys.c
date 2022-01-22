/**
 *
 * Copyright (C) 2021 Jared B. Resch
 *
 * This file is part of EUROPA.
 * 
 * EUROPA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * EUROPA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with EUROPA. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "platform.h"

#ifdef PLATFORM_LINUX

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "xwinsys.h"
#include "hashtabx.h"
#include "window.h"

#define XWINHTSZ  (257)
#define XWD(win)  ((XWINDAT*)win->dat)
#define XSD(surf) ((XSURFDAT*)surf->dat)

static int xwinhsh(Window xwin)
{
        return xwin;
}

HASHTABX(XWINHT, XWINND, xwinht, WINDOW*, Window, int, int, xwinhsh, 2)

typedef struct XDAT {
        Display *xdpy;
        Window   xroot;
        int      xscr, xd;
        GC       xgc;
        Visual  *xvis;
        XWINHT  *xwht;
        Atom     xwmdm;
} XDAT;

typedef struct XWINDAT {
        Window xwin;
        GC     xgc;
} XWINDAT;

typedef struct XSURFDAT {
        XImage         *ximg;
        XShmSegmentInfo xshm;
        int             useshm;
} XSURFDAT;

static XDAT d;

static int ximgalloc(SURFACE *surf)
{
        XSURFDAT   *sd;
        XVisualInfo xvi;
        int         w, h, bytes;
        assert(surf != NULL);
        sd = XSD(surf);
        if (!XMatchVisualInfo(d.xdpy, d.xscr, 32, TrueColor, &xvi)) {
                return 0;
        }
        w = surf->w, h = surf->h;
        bytes = w * h * (xvi.depth / 8);
        surf->bytes = bytes;
        sd->useshm = xshmav();
        if (sd->useshm) {
                sd->xshm.shmid = shmget(IPC_PRIVATE, bytes, IPC_CREAT | 0600);
                if (sd->xshm.shmid < 0) return 0;
                sd->xshm.shmaddr = shmat(sd->xshm.shmid, NULL, 0);
                if (sd->xshm.shmaddr == NULL) goto errrs;
                sd->xshm.readOnly = 0;
                if (!XShmAttach(d.xdpy, &sd->xshm)) goto errfs;
                sd->ximg = XShmCreateImage(d.xdpy, xvi.visual, xvi.depth,
                                           ZPixmap, sd->xshm.shmaddr,
                                           &sd->xshm, w, h);
                if (!sd->ximg) goto errds;
                surf->px = sd->xshm.shmaddr;
        }
        else {
                surf->px = malloc(bytes);
                if (!surf->px) return 0;
                sd->ximg = XCreateImage(d.xdpy, xvi.visual, xvi.depth, ZPixmap,
                                        0, surf->px, w, h, xvi.depth, 0);
                if (!sd->ximg) {
                        if (surf->px) {
                                free(surf->px);
                                surf->px = NULL;
                        }
                        return 0;
                }
        }
        return 1;
errds:  XShmDetach(d.xdpy, &sd->xshm);
errfs:  shmdt (sd->xshm.shmaddr);
        sd->xshm.shmaddr = NULL;
errrs:  shmctl(sd->xshm.shmid, IPC_RMID, 0);
        return 0;
}

static void ximgfree(SURFACE *surf)
{
        XSURFDAT *sd;
        assert(surf != NULL);
        if (surf) {
                sd = XSD(surf);
                if (sd->useshm) {
                        XShmDetach(d.xdpy, &sd->xshm);
                        shmdt(sd->xshm.shmaddr);
                        sd->xshm.shmaddr = NULL;
                        shmctl(sd->xshm.shmid, IPC_RMID, 0);
                        if (sd->ximg) {
                                XDestroyImage(sd->ximg);
                                sd->ximg = NULL;
                        }
                }
                else {
                        if (sd->ximg) {
                                XDestroyImage(sd->ximg);
                                sd->ximg = NULL;
                        }
                        else if (surf->px) {
                                free(surf->px);
                                surf->px = NULL;
                        }
                }
                
        }
}

int xinit()
{
        if (d.xdpy)  return 1;
        d.xdpy  = XOpenDisplay(NULL);
        if (!d.xdpy) return 0;
        d.xscr  = DefaultScreen(d.xdpy);
        d.xroot = DefaultRootWindow(d.xdpy);
        d.xd    = DefaultDepth(d.xdpy, d.xscr);
        d.xvis  = DefaultVisual(d.xdpy, d.xscr);
        d.xgc   = DefaultGC(d.xdpy, d.xscr);
        d.xwmdm = XInternAtom(d.xdpy, "WM_DELETE_WINDOW", False);
        d.xwht  = xwinhtalloc(XWINHTSZ);
        if (!d.xwht) goto err;
        return 1;
err:    d.xdpy = NULL;
        return 0;
}

void xterm()
{
        if (d.xdpy) {
                XCloseDisplay(d.xdpy);
                d.xdpy = NULL;
        }
        if (d.xwht) {
                xwinhtfree(d.xwht);
                d.xwht = NULL;
        }
}

static void xclient(XEvent *xev)
{
        static WINDOW *win;
        if (!xwinhtget(d.xwht, xev->xclient.window, &win)) return;
        if (!win) return;
        if (xev->xclient.data.l[0] == d.xwmdm) win->open = 0;
}

static void xconfig(XEvent *xev)
{
        static WINDOW *win;
        static Window  xwin, twin;
        SURFACE       *surf;
        int            w, h;
        xwin = xev->xconfigure.window;
        if (!xwinhtget(d.xwht, xwin, &win)) return;
        if (!win) return;
        surf = win->surf;
        XTranslateCoordinates(d.xdpy, xwin, d.xroot, 0, 0,
                              &win->x, &win->y, &twin);
        w = xev->xconfigure.width;
        h = xev->xconfigure.height;
        if (win->w == w && win->h == h) return;
        win->w     = w;
        win->h     = h;
        surf->w    = w;
        surf->h    = h;
        surf->size = w * h;
        ximgfree(surf);
        ximgalloc(surf);
}

void xpoll()
{
        XEvent xev;
        assert(d.xdpy != NULL);
        while (XPending(d.xdpy)) {
                XNextEvent(d.xdpy, &xev);
                switch (xev.type) {
                        case ClientMessage:
                                xclient(&xev);
                                break;
                        case ConfigureNotify:
                                xconfig(&xev);
                                break;
                        default:
                                break;
                }
        }
}

int xwinalloc(WINDOW *win)
{
        XWINDAT             *wd;
        XSetWindowAttributes swa;
        XVisualInfo          vi;
        assert(d.xdpy != NULL);
        assert(win    != NULL);
        wd = malloc(sizeof(*wd));
        if (!wd) return 0;
        win->dat = wd;
        if (!XMatchVisualInfo(d.xdpy, d.xscr, win->d, TrueColor, &vi)) {
                goto errfwd;
        }
        swa.bit_gravity       = StaticGravity;
        swa.background_pixel  = XBlackPixel(d.xdpy, d.xscr);
        swa.border_pixel      = XBlackPixel(d.xdpy, d.xscr);
        swa.background_pixmap = None;
        swa.event_mask        = StructureNotifyMask;
        swa.colormap          = XCreateColormap(d.xdpy, d.xroot, vi.visual,
                                                AllocNone);
        wd->xwin = XCreateWindow(d.xdpy, d.xroot, win->x, win->y, win->w,
                                 win->h, 0, vi.depth, InputOutput, vi.visual,
                                 CWBitGravity  | CWBackPixel | CWBackPixmap |
                                 CWBorderPixel | CWEventMask | CWColormap,
                                 &swa);
        if (!wd->xwin) goto errfwd;
        wd->xgc = XCreateGC(d.xdpy, wd->xwin, 0, 0);
        if (!wd->xgc) goto errdxw;
        if (!xwinhtins(d.xwht, wd->xwin, win)) goto errdxw;
        XSetWMProtocols(d.xdpy, wd->xwin, &d.xwmdm, 1);
        XStoreName(d.xdpy, wd->xwin, win->cap);
        XMapWindow(d.xdpy, wd->xwin);
        XFlush(d.xdpy);
        return 1;
errdxw: XDestroyWindow(d.xdpy, wd->xwin);
        XFreeColormap(d.xdpy, swa.colormap);
errfwd: free(wd);
        return 0;
}

void xwinfree(WINDOW *win)
{
        XWINDAT *wd;
        assert(d.xdpy != NULL);
        assert(win    != NULL);
        if (win) {
                wd = XWD(win);
                if (wd) {
                        if (wd->xwin) {
                                XDestroyWindow(d.xdpy, wd->xwin);
                        }
                        free(wd);
                        win->dat = NULL;
                }
        }
}

void xwinswap(WINDOW *win)
{
        SURFACE  *surf;
        XWINDAT  *wd;
        XSURFDAT *sd;
        assert(d.xdpy != NULL);
        assert(win    != NULL);
        surf = win->surf;
        wd = XWD(win);
        sd = XSD(surf);
        if (sd->useshm) XShmPutImage(d.xdpy, wd->xwin, wd->xgc, sd->ximg,
                                     0, 0, 0, 0, surf->w, surf->h, 0);
        else XPutImage(d.xdpy, wd->xwin, wd->xgc, sd->ximg,
                       0, 0, 0, 0, surf->w, surf->h);
        XSync(d.xdpy, False);
}

int xsurfalloc(SURFACE *surf, PXFMT *pxfmt)
{
        XSURFDAT *sd;
        assert(d.xdpy != NULL);
        assert(surf   != NULL);
        sd = malloc(sizeof(*sd));
        if (!sd) return 0;
        surf->dat   = sd;
        /* TODO: Handle pixel format properly. */
        surf->pxfmt = RGBA32;
        if (!ximgalloc(surf)) goto errfsd;
        return 1;
errfsd: free(sd);
        return 0;
}

void xsurffree(SURFACE *surf)
{
        assert(d.xdpy != NULL);
        assert(surf   != NULL);
        if (surf) {
                ximgfree(surf);
                if (surf->dat) {
                        free(surf->dat);
                        surf->dat = NULL;
                }
        }
}

int xshmav()
{
        assert(d.xdpy != NULL);
        return XShmQueryExtension(d.xdpy) == 1;
}

int xshmvers(int *maj, int *min, int *pxmav)
{
        int tmaj, tmin, tpxmav;
        assert(d.xdpy != NULL);
        if (XShmQueryVersion(d.xdpy, &tmaj, &tmin, &tpxmav) == 1) {
                if (maj)   *maj   = tmaj;
                if (min)   *min   = tmin;
                if (pxmav) *pxmav = tpxmav;
                return 1;
        }
        else return false;
}

int xshmpxmav()
{
        int tpxmav;
        assert(d.xdpy != NULL);
        if (xshmvers(NULL, NULL, &tpxmav)) {
                return tpxmav == 1;
        }
        else return false;
}

unsigned long xblackpx()
{
        assert(d.xdpy != NULL);
        return BlackPixel(d.xdpy, d.xscr);
}

unsigned long xwhitepx()
{
        assert(d.xdpy != NULL);
        return WhitePixel(d.xdpy, d.xscr);
}

#endif
