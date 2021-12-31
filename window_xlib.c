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

#include "window_xlib.h"

#include <stdlib.h>

#include <X11/Xlib.h>

static Display *xdisp;
static Window   xroot;

typedef struct XLIB_BUFFER {
        Pixmap xpixmap;
        XShmSegmentInfo xshm;
} XLIB_BAKBUF;

typedef struct XLIB_WINDAT {
        Window xwin;
        XLIB_BUFFER xbuf;
} XLIB_WINDAT;

static int xlibinit();

void winpoll()
{
        XEvent xe;
        while (XPending(xdisp)) {
                XNextEvent(xdisp, &xe);
        }
}

int xlibinit()
{
        static int init = 0;
        if (init) goto suc_ret;
        xdisp = XOpenDisplay(NULL);
        if (!xdisp) goto err_ret;
        xroot = DefaultRootWindow(xdisp);
        init = 1;
suc_ret:
        return 1;
err_ret:
        return 0;
}

int xlib_bakbufalloc(XLIB_BUFFER *buf, int w, int h, int d)
{
        // allocate shm
        buf->xshm->shmid = shmget(IPC_PRIVATE, w * h * d, IPC_CREAT | 600);

        if (buf->xshm->shmid < 0) {
                goto err_ret;
        }

        // attach shm to shmaddr
        buf->xshm->shmaddr = shmat(buf->xshm->shmid, NULL, 0);

        // read-write enable
        buf->xshm->readOnly = 0;

        buf->xpixmap = XShmCreateImage(xdisp,);

err_ret:
        return 0;
}

int xlib_winalloc(WINDOW *win)
{
        XLIB_WINDAT *dat;
        Window xwin;
        if (!xlibinit()) goto err_ret;
        dat = malloc(sizeof(*dat));
        if (!dat) goto err_ret;
        xwin = XCreateSimpleWindow(xdisp, xroot,
                                   win->x, win->y,
                                   win->w, win->h,
                                   0, 0, 0);
        if (!xwin) goto err_free;
        dat->xwin = xwin;
        dat->bakbuf->xshm = 
        XSelectInput(xdisp, xwin, ExposureMask);
        XStoreName(xdisp, xwin, win->title);
        XMapWindow(xdisp, xwin);
        win->dat = dat;
        return 1;
err_free:
        free(dat);
err_ret:
        return 0;
}

void xlib_winfree(WINDOW *win)
{

}

void xlib_winsize(WINDOW *win, int* w, int* h)
{

}

void xlib_winpos(WINDOW *win, int* x, int* y)
{

}

#endif