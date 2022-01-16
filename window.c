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
#include "surface.h"

#include <assert.h>
#include <stdlib.h>

WINDOW* winalloc (const char *cap, int x, int y, int w, int h, int d,
                  const WINOPTS *opts)
{
        WINDOW *win;
        win = malloc(sizeof(*win));
        if (!win) return NULL;
        win->sys   = opts ? opts->sys ? opts->sys : winsysd() : winsysd();
        if (!win->sys) goto errfw;
        win->cap  = cap;
        win->open = 1;
        win->x    = x;
        win->y    = y;
        win->w    = w;
        win->h    = h;
        win->d    = d;
        win->dat = NULL;
        if (!win->sys->drv.winalloc(win)) goto errfw;
        win->surf = surfallocs(win->sys, NULL, w, h);
        if (!win->surf) goto errwf;
        return win;
errwf:  win->sys->drv.winfree(win);
errfw:  free(win);
        return NULL;
}

void winfree(WINDOW *win)
{
        assert(win != NULL);
        if (win) {
                surffree(win->surf);
                win->sys->drv.winfree(win);
                free(win);
        }
}

int winopen(WINDOW* win)
{
        assert(win != NULL);
        return win->open;
}

const char* wincap(WINDOW *win)
{
        assert(win != NULL);
        return win->cap;
}

void winrecap(WINDOW *win, const char *cap)
{
        assert(win != NULL);
        win->sys->drv.winrecap(win, cap);
}

void winxy(WINDOW *win, int* x, int* y)
{
        assert(win != NULL);
        *x = win->x; *y = win->y;
}

void winsz(WINDOW *win, int *w, int *h)
{
        assert(win != NULL);
        *w = win->w; *h = win->h;
}

void winmov(WINDOW *win, int x, int y)
{
        assert(win != NULL);
        win->sys->drv.winmov(win, x, y);
}

void winresz(WINDOW *win, int w, int h)
{
        assert(win != NULL);
        win->sys->drv.winresz(win, w, h);
}

SURFACE* winsurf(WINDOW *win)
{
        assert(win != NULL);
        return win->surf;
}

void* winpx(WINDOW *win)
{
        assert(win != NULL);
        return win->surf->px;
}

void winswap(WINDOW *win)
{
        assert(win != NULL);
        win->sys->drv.winswap(win);
}

void winpush(WINDOW *win)
{
        assert(win != NULL);
}