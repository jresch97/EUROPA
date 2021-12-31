#include "platform.h"

#ifdef PLATFORM_LINUX

#include "window_xlib.h"

#include <stdlib.h>

#include <X11/Xlib.h>

static Display *xdisp;

typedef struct XLIB_WINDAT {
        Window xwin;
} XLIB_WINDAT;

static int xlibinit();

void winpoll()
{
        while (1) {
                break;
        }
}

int xlibinit()
{
        static int init = 0;
        if (init) goto suc_ret;
        xdisp = XOpenDisplay(NULL);
        if (!xdisp) goto err_ret;
        init = 1;
suc_ret:
        return 1;
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
        xwin = XCreateSimpleWindow(xdisp,
                                   DefaultRootWindow(xdisp),
                                   win->x, win->y,
                                   win->w, win->h,
                                   0, 0, 0);
        if (!xwin) goto err_free;
        dat->xwin = xwin;
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