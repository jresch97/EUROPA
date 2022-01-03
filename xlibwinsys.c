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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "window.h"
#include "hwsurface.h"

#define XLIBSHMPERM 0600

typedef struct XLIBWINSYSDAT {
        Display *xdisp;
        int      xscr;
        Window   xroot;
} XLIBWINSYSDAT;

typedef struct XLIBWINDAT {
        Window xwin;
} XLIBWINDAT;

typedef struct XLIBHWSURFDAT {
        int             shm;
        Pixmap          xpxm;
        XShmSegmentInfo xshminf;
} XLIBHWSURFDAT;

static XLIBWINSYSDAT dat;

int xlibwinsysinit ()
{
        if (dat.xdisp) return 1;
        dat.xdisp = XOpenDisplay(NULL);
        if (!dat.xdisp) return 0;
        dat.xscr  = DefaultScreen(dat.xdisp);
        dat.xroot = DefaultRootWindow(dat.xdisp);
        return 1;
}

void xlibwinsysterm ()
{
        assert(dat.xdisp != NULL);
        if (dat.xdisp) {
                XCloseDisplay(dat.xdisp);
                dat.xdisp = NULL;
        }
}

void xlibwinsyspoll (WINSYS *winsys)
{
        XEvent xe;
        assert(dat.xdisp != NULL);
        while (XPending(dat.xdisp)) {
                XNextEvent(dat.xdisp, &xe);
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
        XSelectInput(dat.xdisp, wdat->xwin, ExposureMask);
        XStoreName(dat.xdisp, wdat->xwin, win->title);
        XMapWindow(dat.xdisp, wdat->xwin);
        win->dat = wdat;
        return 1;
errfwdat:
        free(wdat);
        return 0;
}

void xlibwinfree (WINDOW *win)
{
        assert(win != NULL);
        assert(dat.xdisp != NULL);
        // free window...
}

void xlibwinsize (WINDOW *win, int* w, int* h)
{
        assert(win != NULL);
        assert(dat.xdisp != NULL);
        // get window size...
}

void xlibwinpos (WINDOW *win, int* x, int* y)
{
        assert(win != NULL);
        assert(dat.xdisp != NULL);
        // get window pos...
}

int xlibhwsurfalloc (HWSURFACE *surf)
{
        XLIBHWSURFDAT *sdat;
        XVisualInfo    xvinf;
        assert(surf != NULL);
        assert(dat.xdisp != NULL);
        sdat = malloc(sizeof(*sdat));
        if (!sdat) return 0;
        if (!XMatchVisualInfo(dat.xdisp, dat.xscr, surf->pxfmt.bipp,
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
                sdat->xpxm = (Pixmap)XShmCreateImage(
                        dat.xdisp, xvinf.visual, xvinf.depth, ZPixmap,
                        sdat->xshminf.shmaddr, &sdat->xshminf,
                        surf->w, surf->h);
                if (!sdat->xpxm) goto errdshm;
                surf->px = sdat->xshminf.shmaddr;
        }
        else {
                sdat->shm = 0;
                // dat->xpxm = ...
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
                if (sdat->xpxm) XFreePixmap(dat.xdisp, sdat->xpxm);
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