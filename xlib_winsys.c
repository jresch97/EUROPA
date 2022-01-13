/**
 *
 * Author: Jared B. Resch
 *
 * This file is part of EUROPA.
 *
 * EUROPA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EUROPA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EUROPA. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "platform.h"

#ifdef PLATFORM_LINUX

#include "xlib_winsys.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "window.h"

#define XLIB_XWINHTSZ  (257)
#define XLIB_SHMPERM   (0600)
#define XLIB_XWINCLASS (InputOutput)
#define XLIB_INPUTMASK (StructureNotifyMask)
#define XLIB_ATTRMASK  (CWBackPixmap)

#define WINDAT(win) ((XLIB_WINDAT*)win->dat)

typedef struct XLIB_XWINHT {
        struct XLIB_XWINHT *n;
        Window              xwin;
        WINDOW             *win;
} XLIB_XWINHT;

typedef struct XLIB_DAT {
        Display     *xdpy;
        Window       xroot;
        int          xscr, xd;
        GC           xgc;
        Visual      *xvis;
        XLIB_XWINHT *xwinht[XLIB_XWINHTSZ];
} XLIB_DAT;

typedef struct XLIB_WINDAT {
        Window           xwin;
        int              xshm;
        XShmSegmentInfo  xinf;
        XImage          *ximg;
        void            *xpx;
} XLIB_WINDAT;

static int     xlib_xwinhsh  (Window xwin);
static int     xlib_xwinhti  (Window xwin, WINDOW *win);
static WINDOW* xlib_xwinhtw  (Window xwin);
static void    xlib_xwinhtf  ();
static Window  xlib_xwinalloc(WINDOW *win);
static int     xlib_ximgalloc(WINDOW *win);
static void    xlib_ximgfree (WINDOW *win);
static void    xlib_xwinxy   (Window xwin, int *x, int *y);

static XLIB_DAT d;

int xlib_xwinhsh(Window xwin)
{
        return xwin % XLIB_XWINHTSZ;
}

int xlib_xwinhti(Window xwin, WINDOW *win)
{
        XLIB_XWINHT *ht;
        int          h;
        assert(d.xdpy != NULL);
        h = xlib_xwinhsh(xwin);
        ht = d.xwinht[h];
        if (!ht) {
                ht = malloc(sizeof(*ht));
                if (!ht) return 0;
                ht->xwin  = xwin;
                ht->win   = win;
                d.xwinht[h] = ht;
                return 1;
        }
        else {
                while (ht) {
                        if (ht->xwin == xwin) {
                                ht->win = win;
                                return 1;
                        }
                        ht = ht->n;
                }
        }
        return 0;
}

WINDOW* xlib_xwinhtw(Window xwin)
{
        XLIB_XWINHT *ht;
        assert(d.xdpy != NULL);
        ht = d.xwinht[xlib_xwinhsh(xwin)];
        while (ht) {
                if (ht->xwin == xwin) return ht->win;
                ht = ht->n;
        }
        return NULL;
}

void xlib_xwinhtf()
{
        /*
        XLIB_XWINHT *ht;
        int          i;
        assert(d.xdpy != NULL);
        for (i = 0; i < XLIB_XWINHTSZ; i++) {
                ht = d.xwinht[i];
        }
        */
}

Window xlib_xwinalloc(WINDOW *win)
{
        Window               xwin;
        XSetWindowAttributes xattr;
        xattr.background_pixmap = None;
        xwin = XCreateWindow(d.xdpy, d.xroot,
                             win->x, win->y, win->w, win->h,
                             0, d.xd, XLIB_XWINCLASS, d.xvis,
                             XLIB_ATTRMASK, &xattr);
        if (!xwin) return 0;
        XSelectInput(d.xdpy, xwin, XLIB_INPUTMASK);
        XStoreName  (d.xdpy, xwin, win->title);
        XMapWindow  (d.xdpy, xwin);
        xlib_xwinhti(xwin, win);
        return xwin;
}

int xlib_ximgalloc(WINDOW *win)
{
        XLIB_WINDAT *wd;
        XVisualInfo  xvinf;
        assert(win != NULL);
        assert(d.xdpy != NULL);
        wd = WINDAT(win);
        if (!XMatchVisualInfo(d.xdpy, d.xscr, d.xd, TrueColor, &xvinf))
                return 0;
        if (xlib_shmav()) {
                wd->xshm          = 1;
                wd->xinf.shmid    = shmget(IPC_PRIVATE,
                                           win->w * win->h * d.xd,
                                           IPC_CREAT | XLIB_SHMPERM);
                wd->xinf.shmaddr  = shmat (wd->xinf.shmid, NULL, 0);
                wd->xinf.readOnly = 0;
                if (!XShmAttach(d.xdpy, &wd->xinf)) goto errfsh;
                wd->ximg = XShmCreateImage(d.xdpy,
                                           xvinf.visual, xvinf.depth,
                                           ZPixmap,
                                           wd->xinf.shmaddr, &wd->xinf,
                                           win->w, win->h);
                if (!wd->ximg) goto errdsh;
                wd->xpx = wd->xinf.shmaddr;
        }
        else {
                wd->xshm = 0;
                return 0;
        }
        return 1;
errdsh:
        XShmDetach(d.xdpy, &wd->xinf);
errfsh:
        shmdt (wd->xinf.shmaddr);
        shmctl(wd->xinf.shmid, IPC_RMID, 0);
        return 0;
}

void xlib_ximgfree(WINDOW *win)
{
        XLIB_WINDAT *wd;
        assert(win != NULL);
        assert(d.xdpy != NULL);
        if (win && win->dat) {
                wd = WINDAT(win);
                if (wd->xshm) {
                        XShmDetach(d.xdpy, &wd->xinf);
                        shmdt (wd->xinf.shmaddr);
                        shmctl(wd->xinf.shmid, IPC_RMID, 0);
                }
                if (wd->ximg) XDestroyImage(wd->ximg);
        }
}

void xlib_xwinxy(Window xwin, int *x, int *y)
{
        Window tw;
        int    tx, ty;
        assert(d.xdpy != NULL);
        XTranslateCoordinates(d.xdpy, xwin, d.xroot, 0, 0, &tx, &ty, &tw);
        if (x) *x = tx;
        if (y) *y = ty;
}

int xlib_init()
{
        int i;
        if (d.xdpy)  return 1;
        d.xdpy  = XOpenDisplay(NULL);
        if (!d.xdpy) return 0;
        d.xscr  = DefaultScreen(d.xdpy);
        d.xroot = DefaultRootWindow(d.xdpy);
        d.xd    = DefaultDepth(d.xdpy, d.xscr);
        d.xvis  = DefaultVisual(d.xdpy, d.xscr);
        d.xgc   = DefaultGC(d.xdpy, d.xscr);
        for (i = 0; i < XLIB_XWINHTSZ; i++) d.xwinht[i] = NULL;
        return 1;
}

void xlib_term()
{
        assert(d.xdpy != NULL);
        if (d.xdpy) {
                xlib_xwinhtf();
                XCloseDisplay(d.xdpy);
                d.xdpy = NULL;
        }
}

void xlib_poll()
{
        static Window  xwin;
        static WINDOW *win;
        SURFACE       *surf;
        XLIB_WINDAT   *wd;
        XEvent         xe;
        int            w, h;
        assert(d.xdpy != NULL);
        while (XPending(d.xdpy)) {
                XNextEvent(d.xdpy, &xe);
                switch (xe.type) {
                        case ConfigureNotify: {
                                if (xwin != xe.xconfigure.window) {
                                        xwin = xe.xconfigure.window;
                                        win  = xlib_xwinhtw(xwin);
                                }
                                if (!win) break;
                                wd   = WINDAT(win);
                                surf = win->surf;
                                xlib_xwinxy(xwin, &win->x, &win->y);
                                w = xe.xconfigure.width;
                                h = xe.xconfigure.height;
                                if (win->w == w && win->h == h) break;
                                win->w = w;
                                win->h = h;
                                xlib_ximgfree(win);
                                if (xlib_ximgalloc(win)) {
                                        surf->w  = win->w;
                                        surf->h  = win->h;
                                        surf->px = wd->xpx;
                                }
                                else {
                                        surf->w  = surf->h = 0;
                                        surf->px = NULL;
                                }
                                break;
                        }
                }
        }
}

int xlib_winalloc(WINDOW *win, PXFMT *pxfmt, void **px)
{
        XLIB_WINDAT *wd;
        assert(win != NULL);
        assert(d.xdpy != NULL);
        wd = malloc(sizeof(*wd));
        if (!wd) return 0;
        wd->xwin = xlib_xwinalloc(win);
        if (!wd->xwin)            goto errfwd;
        win->dat = wd;
        if (!xlib_ximgalloc(win)) goto errxdw;
        *px    = wd->xpx;
        *pxfmt = RGBA32;
        return 1;
errxdw: XDestroyWindow(d.xdpy, wd->xwin);
errfwd: free(wd);
        return 0;
}

void xlib_winfree(WINDOW *win)
{
        XLIB_WINDAT *wd;
        assert(win != NULL);
        assert(d.xdpy != NULL);
        if (win) {
                wd = WINDAT(win);
                if (wd->xwin) XDestroyWindow(d.xdpy, wd->xwin);
                free(wd);
        }
}

void xlib_winswap(WINDOW *win)
{
        XLIB_WINDAT *wd;
        assert(win != NULL);
        assert(d.xdpy != NULL);
        wd = WINDAT(win);
        if (wd->xshm) {
                XShmPutImage(d.xdpy, wd->xwin, d.xgc, wd->ximg,
                             0, 0, 0, 0, win->w, win->h, 0);
        }
        else {
                /* XCopyArea(...) */
        }         
}

int xlib_shmav()
{
        assert(d.xdpy != NULL);
        return XShmQueryExtension(d.xdpy) == 1;
}

int xlib_shmvers(int *maj, int *min, int *pxmav)
{
        int tmaj, tmin, tpxmav;
        assert(d.xdpy != NULL);
        if (XShmQueryVersion(d.xdpy, &tmaj, &tmin, &tpxmav) == 1) {
                if (maj)   *maj   = tmaj;
                if (min)   *min   = tmin;
                if (pxmav) *pxmav = tpxmav;
                return 1;
        }
        return 0;
}

int xlib_shmpxmav()
{
        int tpxmav;
        assert(d.xdpy != NULL);
        if (xlib_shmvers(NULL, NULL, &tpxmav)) return tpxmav == 1;
        return 0;
}

#endif