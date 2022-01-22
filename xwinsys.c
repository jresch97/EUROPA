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
#include "window.h"
#include "hashtabx.h"

#define XWINHTSZ   (257)
#define XSHMPERM   (0600)
#define XWC        (InputOutput)
#define XATTRMASK  (CWBitGravity | CWBackPixel | CWBackPixmap | CWColormap | \
                    CWEventMask)
#define XINPUTMASK (StructureNotifyMask)
#define XASSERT()  (assert(d.xdpy != NULL))
#define XWD(win)   ((XWINDAT*)win->dat)
#define XSD(surf)  ((XSURFDAT*)surf->dat)

typedef Window XWINDOW;

static int xwinhsh(XWINDOW xwin)
{
        return xwin;
}

HASHTABX(XWINHT, XWINNODE, xwinht, WINDOW*, XWINDOW, int, int, xwinhsh, 2)

typedef struct XDAT {
        Display   *xdpy;
        Window     xroot;
        int        xscr, xd;
        GC         xgc;
        Visual    *xvis;
        XWINHT    *xwht;
        Atom       xwmdm;
} XDAT;

typedef struct XWINDAT {
        XWINDOW xwin;
} XWINDAT;

typedef struct XSURFDAT {
        XImage         *ximg0, *ximg1, *curr;
        int             useshm;
        XShmSegmentInfo xshm0, xshm1;
} XSURFDAT;

static XDAT d;

static void    xwinpos    (XWINDOW xwin, int *x, int *y);
static XWINDOW xwincreate (WINDOW *win);
static int     ximgalloc  (SURFACE *surf);
static void    ximgfree   (SURFACE *surf);

void xwinpos(XWINDOW xwin, int *x, int *y)
{
        XWINDOW tw;
        int     tx, ty;
        XASSERT();
        XTranslateCoordinates(d.xdpy, xwin, d.xroot, 0, 0, &tx, &ty, &tw);
        XFlush(d.xdpy);
        if (x) *x = tx;
        if (y) *y = ty;
}

XWINDOW xwincreate(WINDOW *win)
{
        XWINDOW              xwin;
        XSetWindowAttributes xattr;
        XASSERT();
        xattr.bit_gravity = StaticGravity;
        xattr.background_pixel = 0;
        xattr.background_pixmap = None;
        xattr.colormap = XCreateColormap(d.xdpy, d.xroot, d.xvis, AllocNone);
        xattr.event_mask = XINPUTMASK;
        xwin = XCreateWindow(d.xdpy, d.xroot, win->x, win->y, win->w, win->h,
                             0, 24, XWC, d.xvis, XATTRMASK, &xattr);
        if (!xwin) return xwin;
        XSetWMProtocols(d.xdpy, xwin, &d.xwmdm, 1);
        XStoreName     (d.xdpy, xwin, win->cap);
        XMapWindow     (d.xdpy, xwin);
        XFlush         (d.xdpy);
        xwinhtins      (d.xwht, xwin, win);
        return xwin;
}

int ximgalloc(SURFACE *surf)
{
        XSURFDAT   *sd;
        XVisualInfo xvi;
        XASSERT();
        assert(surf != NULL);
        sd = XSD(surf);
        if (!XMatchVisualInfo(d.xdpy, d.xscr, d.xd, TrueColor, &xvi)) return 0;
        if (xshmav()) {
                sd->useshm        = 1;
                sd->xshm0.shmid    = shmget(IPC_PRIVATE,
                                            surf->w * surf->h * d.xd,
                                            IPC_CREAT | XSHMPERM);
                sd->xshm0.shmaddr  = shmat (sd->xshm0.shmid, NULL, 0);
                sd->xshm0.readOnly = 0;
                sd->xshm1.shmid    = shmget(IPC_PRIVATE,
                                            surf->w * surf->h * d.xd,
                                            IPC_CREAT | XSHMPERM);
                sd->xshm1.shmaddr  = shmat (sd->xshm1.shmid, NULL, 0);
                sd->xshm1.readOnly = 0;
                if (!XShmAttach(d.xdpy, &sd->xshm0)) goto errfsh;
                if (!XShmAttach(d.xdpy, &sd->xshm1)) goto errfsh;
                sd->ximg0 = XShmCreateImage(d.xdpy,
                                            xvi.visual, xvi.depth, ZPixmap,
                                            sd->xshm0.shmaddr, &sd->xshm0,
                                            surf->w, surf->h);
                if (!sd->ximg0) goto errdsh;
                sd->ximg1 = XShmCreateImage(d.xdpy,
                                            xvi.visual, xvi.depth, ZPixmap,
                                            sd->xshm1.shmaddr, &sd->xshm1,
                                            surf->w, surf->h);
                if (!sd->ximg1) goto errdsh;
                surf->px = sd->xshm0.shmaddr;
                sd->curr = sd->ximg0;
        }
        else {
                sd->useshm = 0;
                surf->px   = malloc(surf->w * surf->h * d.xd);
                if (!surf->px) goto err;
                sd->ximg0  = XCreateImage(d.xdpy,
                                          xvi.visual, xvi.depth, ZPixmap, 0,
                                          surf->px, surf->w, surf->h, 32,
                                          0);
                if (!sd->ximg0) {
                        free(surf->px);
                        goto err;
                }
        }
        return 1;
errdsh: XShmDetach(d.xdpy, &sd->xshm0);
        XShmDetach(d.xdpy, &sd->xshm1);
errfsh: shmdt (sd->xshm0.shmaddr);
        shmdt (sd->xshm1.shmaddr);
        shmctl(sd->xshm0.shmid, IPC_RMID, 0);
        shmctl(sd->xshm1.shmid, IPC_RMID, 0);
err:    return 0;
}

void ximgfree(SURFACE *surf)
{
        XSURFDAT *sd;
        XASSERT();
        assert(surf != NULL);
        if (surf && surf->dat) {
                sd = XSD(surf);
                if (sd->useshm) {
                        XShmDetach(d.xdpy, &sd->xshm0);
                        shmdt (sd->xshm0.shmaddr);
                        shmctl(sd->xshm0.shmid, IPC_RMID, 0);
                        XShmDetach(d.xdpy, &sd->xshm1);
                        shmdt (sd->xshm0.shmaddr);
                        shmctl(sd->xshm0.shmid, IPC_RMID, 0);
                }
                if (sd->ximg0) XDestroyImage(sd->ximg0);
                if (sd->ximg1) XDestroyImage(sd->ximg1);
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
        XFlush(d.xdpy);
        if (!d.xwht) goto err;
        return 1;
err:    d.xdpy = NULL;
        return 0;
}

void xterm()
{
        if (d.xdpy) {
                if (d.xwht) {
                        xwinhtfree(d.xwht);
                        d.xwht = NULL;
                }
                XCloseDisplay(d.xdpy);
                d.xdpy = NULL;
        }
}

void xpoll()
{
        static WINDOW  *win;
        static XWINDOW  xwin;
        XEvent          xe;
        int             w, h;
        XASSERT();
        while (XPending(d.xdpy)) {
                XNextEvent(d.xdpy, &xe);
                switch (xe.type) {
                        case ClientMessage: {
                                if (xwin != xe.xclient.window) {
                                        xwin = xe.xclient.window;
                                        xwinhtget(d.xwht, xwin, &win);
                                }
                                if (xe.xclient.data.l[0] == d.xwmdm) {
                                        win->open = 0;
                                }
                                break;
                        }   
                        case ConfigureNotify: {
                                if (xwin != xe.xconfigure.window) {
                                        xwin = xe.xconfigure.window;
                                        xwinhtget(d.xwht, xwin, &win);
                                }
                                xwinpos(xwin, &win->x, &win->y);
                                w = xe.xconfigure.width;
                                h = xe.xconfigure.height;
                                if (win->w == w && win->h == h) break;
                                win->w       = w, win->h       = h;
                                win->surf->w = w, win->surf->h = h;
                                /* TODO: What if ximgalloc fails? */
                                ximgfree(win->surf);
                                ximgalloc(win->surf);
                                break;
                        }
                        default:
                                break;
                }
        }
}

int xwinalloc(WINDOW *win)
{
        XWINDAT *wd;
        XASSERT();
        assert(win != NULL);
        wd = malloc(sizeof(*wd));
        if (!wd) return 0;
        win->dat = wd;
        wd->xwin = xwincreate(win);
        if (!wd->xwin) goto errfwd;
        return 1;
errfwd: free(wd);
        return 0;
}

void xwinfree(WINDOW *win)
{
        XWINDAT *wd;
        XASSERT();
        assert(win != NULL);
        if (win) {
                wd = XWD(win);
                if (wd->xwin) XDestroyWindow(d.xdpy, wd->xwin);
                free(wd);
        }
}

void xwinswap(WINDOW *win)
{
        SURFACE  *surf;
        XWINDAT  *wd;
        XSURFDAT *sd;
        assert(win != NULL);
        assert(d.xdpy != NULL);
        surf = win->surf;
        wd = XWD(win);
        sd = XSD(surf);
        if (sd->useshm) {
                if (sd->curr == sd->ximg0) {
                        XShmPutImage(d.xdpy, wd->xwin, d.xgc, sd->ximg0,
                                     0, 0, 0, 0, surf->w, surf->h, 0);
                        surf->px = sd->xshm1.shmaddr;
                        sd->curr = sd->ximg1;
                }
                else {
                        XShmPutImage(d.xdpy, wd->xwin, d.xgc, sd->ximg1,
                                     0, 0, 0, 0, surf->w, surf->h, 0);
                        surf->px = sd->xshm0.shmaddr;
                        sd->curr = sd->ximg0;
                }
                //XSync(d.xdpy, 0);
        }
        else {
                XPutImage(d.xdpy, wd->xwin, d.xgc, sd->ximg0,
                          0, 0, 0, 0, surf->w, surf->h);
        }
}

int xsurfalloc(SURFACE *surf, PXFMT *pxfmt)
{
        XSURFDAT *sd;
        XASSERT();
        assert(surf != NULL);
        sd = malloc(sizeof(*sd));
        if (!sd) return 0;
        surf->dat   = sd;
        /* TODO: Handle pixel format properly. */
        surf->bytes = surf->size * RGBA32.bypp;
        surf->pxfmt = RGBA32;
        if (!ximgalloc(surf)) goto errfsd;
        return 1;
errfsd: free(sd);
        return 0;
}

void xsurffree(SURFACE *surf)
{
        XASSERT();
        assert(surf != NULL);
        if (surf) {
                ximgfree(surf);
                if (surf->dat) free(surf->dat);
        }
}

int xshmav()
{
        XASSERT();
        return XShmQueryExtension(d.xdpy) == 1;
}

int xshmvers(int *maj, int *min, int *pxmav)
{
        int tmaj, tmin, tpxmav;
        XASSERT();
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
        XASSERT();
        if (xshmvers(NULL, NULL, &tpxmav)) {
                return tpxmav == 1;
        }
        else return false;
}

unsigned long xblackpx()
{
        XASSERT();
        return BlackPixel(d.xdpy, d.xscr);
}

unsigned long xwhitepx()
{
        XASSERT();
        return WhitePixel(d.xdpy, d.xscr);
}

#endif
