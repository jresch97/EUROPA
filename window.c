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

#include "window.h"
#include "window_win32.h"

#include <assert.h>
#include <stdlib.h>

static const WINDRV* WINDRVS[] = {
#ifdef PLATFORM_WIN32
        &WIN32_WINDRV,
#endif
        NULL
};

static inline const WINDRV* windrvsel()
{
        return WINDRVS[0];
}

WINDOW* winalloc(const char* title, int x, int y, int w, int h)
{
        WINDOW* win;
        win = malloc(sizeof(*win));
        if (!win) goto err_ret;
        win->drv = windrvsel();
        if (!win->drv) goto err_free;
        win->title = title;
        win->x = x;
        win->y = y;
        win->w = w;
        win->h = h;
        win->surf = NULL;
        win->dat = NULL;
        assert(win->drv->winalloccb != NULL);
        if (!win->drv->winalloccb(win)) goto err_free;
        return win;
err_free:
        free(win);
err_ret:
        return NULL;
}

void winfree(WINDOW* win)
{
        assert(win != NULL);
        assert(win->drv != NULL);
        assert(win->drv->winfreecb != NULL);
        if (win) {
                if (win->drv && win->drv->winfreecb) {
                        win->drv->winfreecb(win);
                }
                free(win);
        }
}

const WINDRV* windrv(WINDOW* win)
{
        assert(win != NULL);
        return win->drv;
}

void winsize(WINDOW* win, int* w, int* h)
{
        assert(win != NULL);
        assert(win->drv != NULL);
        assert(win->drv->winsizecb != NULL);
        win->drv->winsizecb(win, w, h);
}

void winpos(WINDOW* win, int* x, int* y)
{
        assert(win != NULL);
        assert(win->drv != NULL);
        assert(win->drv->winposcb != NULL);
        win->drv->winposcb(win, x, y);
}

SURFACE* winsurf(WINDOW* win)
{
        assert(win != NULL);
        return win->surf;
}

int* winpx(WINDOW* win)
{
        assert(win != NULL);
        assert(win->surf != NULL);
        return win->surf->px;
}