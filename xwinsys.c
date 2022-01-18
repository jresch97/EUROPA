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

#include "xwinsys.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "window.h"
#include "hashtabx.h"

#define XWINHTSZ   (257)
#define XSHMPERM   (0600)
#define XWC        (InputOutput)
#define XATTRMASK  (CWBackPixmap)
#define XINPUTMASK (StructureNotifyMask)

#define XASSERT() (assert(d.xdpy != NULL))
#define XWD(win)  ((XWINDAT*)win->dat)
#define XSD(surf) ((XSURFDAT*)surf->dat)

static inline int xwinhash(Window xwin)
{
        return xwin;
}

HASHTABX(XWINHT, XWINNODE, xwinht, WINDOW*, Window, int, int, xwinhash, 2)

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
} XWINDAT;

typedef struct XSURFDAT {
        XImage         *ximg;
        int             useshm;
        XShmSegmentInfo xshm;
} XSURFDAT;

static void   xwinxy    (Window xwin, int *x, int *y);
static Window xwincreate(WINDOW *win);
static int    ximgalloc (SURFACE *surf);
static void   ximgfree  (SURFACE *surf);

static XDAT d;

void xwinxy(Window xwin, int *x, int *y)
{
        Window tw;
        int    tx, ty;
        XASSERT();
        XTranslateCoordinates(d.xdpy, xwin, d.xroot, 0, 0, &tx, &ty, &tw);
        if (x) *x = tx;
        if (y) *y = ty;
}

Window xwincreate(WINDOW *win)
{
        Window               xwin;
        XSetWindowAttributes xattr;
        XASSERT();
        xattr.background_pixmap = None;
        xwin = XCreateWindow(d.xdpy, d.xroot, win->x, win->y, win->w, win->h,
                             0, d.xd, XWC, d.xvis, XATTRMASK, &xattr);
        if (!xwin) return xwin;
        XSetWMProtocols(d.xdpy, xwin, &d.xwmdm, 1);
        XSelectInput   (d.xdpy, xwin, XINPUTMASK);
        XStoreName     (d.xdpy, xwin, win->cap);
        XMapWindow     (d.xdpy, xwin);
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
                sd->xshm.shmid    = shmget(IPC_PRIVATE,
                                           surf->pxsz * d.xd,
                                           IPC_CREAT | XSHMPERM);
                sd->xshm.shmaddr  = shmat (sd->xshm.shmid, NULL, 0);
                sd->xshm.readOnly = 0;
                if (!XShmAttach(d.xdpy, &sd->xshm)) goto errfsh;
                sd->ximg = XShmCreateImage(d.xdpy,
                                           xvi.visual, xvi.depth, ZPixmap,
                                           sd->xshm.shmaddr, &sd->xshm,
                                           surf->w, surf->h);
                if (!sd->ximg) goto errdsh;
                surf->px = sd->xshm.shmaddr;
                return 1;
        }
        else goto errret;
errdsh: XShmDetach(d.xdpy, &sd->xshm);
errfsh: shmdt (sd->xshm.shmaddr);
        shmctl(sd->xshm.shmid, IPC_RMID, 0);
errret: return 0;
}

void ximgfree(SURFACE *surf)
{
        XSURFDAT *sd;
        XASSERT();
        assert(surf != NULL);
        if (surf && surf->dat) {
                sd = XSD(surf);
                if (sd->useshm) {
                        XShmDetach(d.xdpy, &sd->xshm);
                        shmdt (sd->xshm.shmaddr);
                        shmctl(sd->xshm.shmid, IPC_RMID, 0);
                }
                if (sd->ximg) XDestroyImage(sd->ximg);
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
        static WINDOW *win;
        static Window  xwin;
        XEvent         xe;
        int            w, h;
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
                                xwinxy(xwin, &win->x, &win->y);
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
                XShmPutImage(d.xdpy, wd->xwin, d.xgc, sd->ximg,
                             0, 0, 0, 0, surf->w, surf->h, 0);
        }
        else {
                /* XCopyArea(...) */
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
        surf->bysz  = surf->pxsz * RGBA32.bypp;
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
        else return 0;
}

int xshmpxmav()
{
        int tpxmav;
        XASSERT();
        if (xshmvers(NULL, NULL, &tpxmav)) {
                return tpxmav == 1;
        }
        else return 0;
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
