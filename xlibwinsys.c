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

#include "xlibwinsys.h"

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

#define XLIBSHMPERM 0600
#define XLIBWINTAG  "EUROPA WINDOW"

typedef struct XLIBWINSYSDAT {
        Display *xdisp;
        int      xscr, xdepth;
        Window   xroot;
        GC       xgc;
} XLIBWINSYSDAT;

typedef struct XLIBWINDAT {
        Window xwin;
} XLIBWINDAT;

typedef struct XLIBHWSURFDAT {
        int              shm;
        Pixmap           xpxm;
        XImage          *xshmimg;
        XShmSegmentInfo  xshminf;
} XLIBHWSURFDAT;

static XLIBWINSYSDAT  dat;
static WINDOW        *gwin;

static void xlibwinpos (Window xwin, int *x, int *y)
{
        Window tmpwin;
        int    tmpx, tmpy;
        assert(dat.xdisp != NULL);
        assert(x != NULL);
        assert(y != NULL);
        XTranslateCoordinates(dat.xdisp, xwin, dat.xroot,
                              0, 0, &tmpx, &tmpy, &tmpwin);
        *x = tmpx;
        *y = tmpy;
}

int xlibinit ()
{
        if (dat.xdisp) return 1;
        dat.xdisp = XOpenDisplay(NULL);
        if (!dat.xdisp) return 0;
        dat.xscr   = DefaultScreen(dat.xdisp);
        dat.xdepth = DefaultDepth(dat.xdisp, dat.xscr);
        dat.xroot  = DefaultRootWindow(dat.xdisp);
        dat.xgc    = DefaultGC(dat.xdisp, dat.xscr);
        return 1;
}

void xlibterm ()
{
        assert(dat.xdisp != NULL);
        if (dat.xdisp) {
                XCloseDisplay(dat.xdisp);
                dat.xdisp = NULL;
        }
}

void xlibpoll ()
{
        HWSURFACE *nsurf;
        XEvent xe;
        assert(dat.xdisp != NULL);
        while (XPending(dat.xdisp)) {
                XNextEvent(dat.xdisp, &xe);
                switch (xe.type) {
                        case ConfigureNotify: {
                                xlibwinpos(xe.xconfigure.window,
                                           &gwin->x, &gwin->y);
                                if (gwin->w != xe.xconfigure.width ||
                                    gwin->h != xe.xconfigure.height) {
                                        gwin->w = xe.xconfigure.width;
                                        gwin->h = xe.xconfigure.height;
                                        nsurf = hwsurfalloc(RGBA32,
                                                            gwin->w, gwin->h);
                                        if (nsurf) {
                                                hwsurffree(gwin->surf);
                                                gwin->surf = nsurf;
                                        }
                                }
                                break;
                        }
                }
        }
}

int xlibwinalloc (WINDOW *win)
{
        XLIBWINDAT *wdat;
        assert(win != NULL);
        assert(dat.xdisp != NULL);
        wdat = malloc(sizeof(*wdat));
        if (!wdat) return 0;
        wdat->xwin = XCreateSimpleWindow(dat.xdisp, dat.xroot,
                                         win->x, win->y, win->w, win->h,
                                         0, 0, 0);
        if (!wdat->xwin) goto errfwdat;
        XSelectInput(dat.xdisp, wdat->xwin, StructureNotifyMask);
        XStoreName(dat.xdisp, wdat->xwin, win->title);
        XMapWindow(dat.xdisp, wdat->xwin);
        xlibwinpos(wdat->xwin, &win->x, &win->y);
        win->dat = wdat;
        gwin = win;
        return 1;
errfwdat:
        free(wdat);
        return 0;
}

void xlibwinfree (WINDOW *win)
{
        XLIBWINDAT *wdat;
        assert(win != NULL);
        assert(dat.xdisp != NULL);
        if (win) {
                wdat = (XLIBWINDAT*)win->dat;
                XDestroyWindow(dat.xdisp, wdat->xwin);
                free(wdat);
        }
}

void xlibwinswap (WINDOW *win)
{
        XLIBWINDAT    *wdat;
        XLIBHWSURFDAT *sdat;
        assert(win != NULL);
        assert(dat.xdisp != NULL);
        wdat = (XLIBWINDAT*)win->dat;
        sdat = (XLIBHWSURFDAT*)win->surf->dat;
        if (sdat->shm) {
                XShmPutImage(dat.xdisp, wdat->xwin, dat.xgc, sdat->xshmimg,
                             0, 0, 0, 0, win->w, win->h, 0);
        }
        else {
                XCopyArea(dat.xdisp, sdat->xpxm, wdat->xwin, dat.xgc,
                          0, 0, win->w, win->h, 0, 0);
        }
}

int xlibhwsurfalloc (HWSURFACE *surf)
{
        XLIBHWSURFDAT *sdat;
        XVisualInfo    xvinf;
        assert(surf != NULL);
        assert(dat.xdisp != NULL);
        sdat = malloc(sizeof(*sdat));
        if (!sdat) return 0;
        if (!XMatchVisualInfo(dat.xdisp, dat.xscr, dat.xdepth,
                              TrueColor, &xvinf)) goto errfsdat;
        if (xlibshmpxmav()) {
                sdat->shm = 1;
                sdat->xshminf.shmid = shmget(IPC_PRIVATE,
                                             surf->w * surf->h *
                                             surf->pxfmt.bypp,
                                             IPC_CREAT | XLIBSHMPERM);
                sdat->xshminf.shmaddr = shmat(sdat->xshminf.shmid, NULL, 0);
                sdat->xshminf.readOnly = 0;
                if (!XShmAttach(dat.xdisp, &sdat->xshminf)) goto errfshm;
                sdat->xshmimg = XShmCreateImage(
                        dat.xdisp, xvinf.visual, xvinf.depth, ZPixmap,
                        sdat->xshminf.shmaddr, &sdat->xshminf,
                        surf->w, surf->h);
                if (!sdat->xshmimg) goto errdshm;
                surf->px = sdat->xshminf.shmaddr;
        }
        else {
                sdat->shm = 0;
                // sdat->xpxm = ...
                goto errfsdat;
        }
        surf->dat = sdat;
        return 1;
errdshm:
        XShmDetach(dat.xdisp, &sdat->xshminf);
errfshm:
        shmdt(sdat->xshminf.shmaddr);
        shmctl(sdat->xshminf.shmid, IPC_RMID, 0);
errfsdat:
        free(sdat);
        return 0;
}

void xlibhwsurffree (HWSURFACE *surf)
{
        XLIBHWSURFDAT *sdat;
        assert(surf != NULL);
        assert(dat.xdisp != NULL);
        if (surf && surf->dat) {
                sdat = (XLIBHWSURFDAT*)surf->dat;
                if (sdat->shm) {
                        XShmDetach(dat.xdisp, &sdat->xshminf);
                        shmdt(sdat->xshminf.shmaddr);
                        shmctl(sdat->xshminf.shmid, IPC_RMID, 0);
                }
                if (sdat->xshmimg) XDestroyImage(sdat->xshmimg);
                //if (sdat->xpxm)    XFreePixmap(dat.xdisp, sdat->xpxm);
                free(sdat);
        }
}

int xlibshmav ()
{
        assert(dat.xdisp != NULL);
        return XShmQueryExtension(dat.xdisp) == 1;
}

int xlibshmvers (int *maj, int *min, int *pxmav)
{
        int tmaj, tmin, tpxmav;
        assert(dat.xdisp != NULL);
        if (XShmQueryVersion(dat.xdisp, &tmaj, &tmin, &tpxmav) == 1) {
                if (maj)   *maj   = tmaj;
                if (min)   *min   = tmin;
                if (pxmav) *pxmav = tpxmav;
                return 1;
        }
        return 0;
}

int xlibshmpxmav ()
{
        int tpxmav;
        assert(dat.xdisp != NULL);
        if (xlibshmvers(NULL, NULL, &tpxmav)) return tpxmav == 1;
        return 0;
}

#endif