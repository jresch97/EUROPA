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
#include "hwsurface.h"

#define XLIB_SHMPERM  (0600)
#define XLIB_XWINHTSZ (257)

typedef struct XLIB_XWINHT {
        XLIB_XWINHTENT *n;
        Window          xwin;
        WINDOW         *win;
} XLIB_XWINHT;

typedef struct XLIB_DAT {
        Display     *xdisp;
        int          xscr, xdepth;
        Window       xroot;
        GC           xgc;
        XLIB_XWINHT *xwinht[XLIB_XWINHTSZ];
} XLIB_DAT;

typedef struct XLIB_WINDAT {
        Window xwin;
} XLIB_WINDAT;

typedef struct XLIB_SURFDAT {
        int              shm;
        XImage          *ximg;
        XShmSegmentInfo  xshminf;
} XLIB_SURFDAT;

static int     xlib_xwinhsh(Window xwin);
static int     xlib_xwinhti(Window xwin, WINDOW* win);
static WINDOW* xlib_xwinhtw(Window xwin)
static void    xlib_xwinhtf()
static void    xlib_xwinxy (Window xwin, int* x, int* y)

static XLIB_DAT d;

int xlib_xwinhsh(Window xwin)
{
        return xwin % XLIB_XWINHTSZ;
}

int xlib_xwinhti(Window xwin, WINDOW* win)
{
        XLIB_XWINHT *xwinht;
        int          h;
        assert(d.xdisp != NULL);
        h = xlib_xwinhsh(xwin);
        xwinht = dat.xwinht[h];
        if (!xwinht) {
                xwinht = malloc(sizeof(*xwinht));
                if (!xwinht) return 0;
                xwinht->xwin     = xwin;
                xwinht->win      = win;
                dat.xwinht[h] = xwinht;
                return 1;
        }
        else {
                while (xwinht) {
                        if (xwinht->xwin == xwin) {
                                xwinht->win = win;
                                return 1;
                        }
                        xwinht = xwinht->n;
                }
        }
        return 0;
}

WINDOW* xlib_xwinhtw(Window xwin)
{
        XLIB_XWINHT *ht;
        assert(d.xdisp != NULL);
        ht = d.xwinht + xlib_xwinhsh(xwin);
        while (ht) {
                if (ht->xwin == xwin) return ht->win;
                ht = ht->n;
        }
        return NULL;
}

void xlib_xwinhtf()
{
        XLIB_XWINHT *ht;
        int          i;
        assert(d.xdisp != NULL);
        for (i = 0; i < XLIB_XWINHTSZ; i++) {
                ht = d.xwinht[i];
        }
}

void xlib_xwinxy(Window xwin, int *x, int *y)
{
        Window tw;
        int    tx, ty;
        assert(d.xdisp != NULL);
        XTranslateCoordinates(d.xdisp, xwin, d.xroot, 0, 0, &tx, &ty, &tw);
        if (x) *x = tx;
        if (y) *y = ty;
}

int xlib_init()
{
        if (d.xdisp)  return 1;
        d.xdisp  = XOpenDisplay(NULL);
        if (!d.xdisp) return 0;
        d.xscr   = DefaultScreen(d.xdisp);
        d.xdepth = DefaultDepth(d.xdisp, d.xscr);
        d.xroot  = DefaultRootWindow(d.xdisp);
        d.xgc    = DefaultGC(d.xdisp, d.xscr);
        return 1;
}

void xlib_term ()
{
        assert(d.xdisp != NULL);
        if (d.xdisp) {
                xlib_xwinhtf();
                XCloseDisplay(d.xdisp);
                d.xdisp = NULL;
        }
}

void xlib_poll ()
{
        WINDOW  *win;
        SURFACE *surf;
        XEvent   xe;
        assert(d.xdisp != NULL);
        while (XPending(d.xdisp)) {
                XNextEvent(d.xdisp, &xe);
                switch (xe.type) {
                        case ConfigureNotify: {
                                win = xlib_xwinhtw(xe.xconfigure.window);
                                if (!win) break;
                                xlib_xwinxy(xe.xconfigure.window,
                                            &win->x, &win->y);
                                win->w = xe.xconfigure.width;
                                win->h = xe.xconfigure.height;
                                surf = winsurfalloc(win, win->w, win->h);
                                if (surf) {
                                        winsurffree(win->surf);
                                        win->surf = surf;
                                }
                                break;
                        }
                }
        }
}

int xlib_winalloc (WINDOW *win)
{
        XLIB_WINDAT *wd;
        assert(win != NULL);
        assert(d.xdisp != NULL);
        wd = malloc(sizeof(*wd));
        if (!wd) return 0;
        wd->xwin = XCreateSimpleWindow(d.xdisp, d.xroot,
                                       win->x, win->y, win->w, win->h,
                                       0, 0, 0);
        if (!wd->xwin)                    goto errfwd;
        XSelectInput(d.xdisp, wd->xwin, StructureNotifyMask);
        XSetWindowBackgroundPixmap(d.xdisp, wd->xwin, None);
        XStoreName(d.xdisp, wd->xwin, win->title);
        XMapWindow(d.xdisp, wd->xwin);
        if (!xlib_xwinhti(wd->xwin, win)) goto errxwd;
        win->dat = wd;
        return 1;
errxwd: XDestroyWindow(d.xdisp, wd->xwin);
errfwd: free(wdat);
        return 0;
}

void xlib_winfree (WINDOW *win)
{
        XLIB_WINDAT *wd;
        assert(win != NULL);
        assert(d.xdisp != NULL);
        if (win) {
                wd = (XLIB_WINDAT*)win->dat;
                XDestroyWindow(d.xdisp, wd->xwin);
                free(wd);
        }
}

void xlib_winswap (WINDOW *win)
{
        XLIB_WINDAT  *wd;
        XLIB_SURFDAT *sd;
        assert(win != NULL);
        assert(d.xdisp != NULL);
        wd = (XLIB_WINDAT*)win->dat;
        sd = (XLIB_SURFDAT*)win->surf->dat;
        if (sd->shm) {
                XShmPutImage(d.xdisp, wd->xwin, d.xgc, sd->ximg,
                             0, 0, 0, 0, win->w, win->h, 0);
        }
        else {
                /*XCopyArea(d.xdisp, sd->???, wd->xwin, d.xgc,
                            0, 0, win->w, win->h, 0, 0);*/
        }
}

int xlib_surfalloc (SURFACE *surf)
{
        XLIB_SURFDAT *sd;
        XVisualInfo   xvinf;
        assert(surf != NULL);
        assert(d.xdisp != NULL);
        sd = malloc(sizeof(*sd));
        if (!sd) return 0;
        if (!XMatchVisualInfo(d.xdisp, d.xscr, d.xdepth, TrueColor, &xvinf))
                goto errfsd;
        if (xlib_shmav()) {
                sd->shm = 1;
                sd->xshminf.shmid = shmget(IPC_PRIVATE,
                                           surf->w * surf->h * d.xdepth,
                                           IPC_CREAT | XLIB_SHMPERM);
                sd->xshminf.shmaddr = shmat(sd->xshminf.shmid, NULL, 0);
                sd->xshminf.readOnly = 0;
                if (!XShmAttach(d.xdisp, &sd->xshminf)) goto errfsh;
                sd->ximg = XShmCreateImage(
                        d.xdisp, xvinf.visual, xvinf.depth, ZPixmap,
                        sd->xshminf.shmaddr, &sd->xshminf,
                        surf->w, surf->h);
                if (!sdat->ximg) goto errdsh;
                surf->px = sdat->xshminf.shmaddr;
        }
        else {
                sd->shm = 0;
                goto errfsd;
        }
        surf->dat = sd;
        return 1;
errdsh:
        XShmDetach(dat.xdisp, &sd->xshminf);
errfsh:
        shmdt(sd->xshminf.shmaddr);
        shmctl(sd->xshminf.shmid, IPC_RMID, 0);
errfsd:
        free(sd);
        return 0;
}

void xlib_surffree (SURFACE *surf)
{
        XLIB_SURFDAT *sd;
        assert(surf != NULL);
        assert(d.xdisp != NULL);
        if (surf && surf->dat) {
                sd = (XLIB_SURFDAT*)surf->dat;
                if (sdat->shm) {
                        XShmDetach(d.xdisp, &sd->xshminf);
                        shmdt(sd->xshminf.shmaddr);
                        shmctl(sd->xshminf.shmid, IPC_RMID, 0);
                }
                if (sd->ximg) XDestroyImage(sd->ximg);
                free(sd);
        }
}

int xlib_shmav ()
{
        assert(d.xdisp != NULL);
        return XShmQueryExtension(d.xdisp) == 1;
}

int xlib_shmvers (int *maj, int *min, int *pxmav)
{
        int tmaj, tmin, tpxmav;
        assert(d.xdisp != NULL);
        if (XShmQueryVersion(d.xdisp, &tmaj, &tmin, &tpxmav) == 1) {
                if (maj)   *maj   = tmaj;
                if (min)   *min   = tmin;
                if (pxmav) *pxmav = tpxmav;
                return 1;
        }
        return 0;
}

int xlib_shmpxmav ()
{
        int tpxmav;
        assert(d.xdisp != NULL);
        if (xlib_shmvers(NULL, NULL, &tpxmav)) return tpxmav == 1;
        return 0;
}

#endif