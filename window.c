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
#include "hwsurface.h"

#include <assert.h>
#include <stdlib.h>

WINDOW* winalloc (const char* title, int x, int y, int w, int h)
{
        WINDOW *win;
        win = malloc(sizeof(*win));
        if (!win) return NULL;
        win->sys = defwinsys();
        if (!win->sys) goto errfwin;
        win->title = title;
        win->x     = x;
        win->y     = y;
        win->w     = w;
        win->h     = h;
        win->surf  = hwsurfalloc(RGBA32, w, h);
        win->dat   = NULL;
        if (!win->sys->drv.winalloccb(win)) goto errfwin;
        return win;
errfwin:
        free(win);
        return NULL;
}

void winfree (WINDOW* win)
{
        assert(win != NULL);
        if (win) {
                win->sys->drv.winfreecb(win);
                free(win);
        }
}

void winpush (WINDOW *win)
{
        assert(win != NULL);
}

void winpull (WINDOW *win)
{
        assert(win != NULL);
}

void winupdt (WINDOW *win)
{
        assert(win != NULL);
}

void winsize (WINDOW* win, int* w, int* h)
{
        assert(win != NULL);
        win->sys->drv.winsizecb(win, w, h);
}

void winpos (WINDOW* win, int* x, int* y)
{
        assert(win != NULL);
        win->sys->drv.winposcb(win, x, y);
}

const HWSURFACE* winsurf (WINDOW* win)
{
        assert(win != NULL);
        return win->surf;
}

void* winpx (WINDOW* win)
{
        assert(win != NULL);
        return win->surf->px;
}

void winswap(WINDOW *win)
{
        assert(win != NULL);
}