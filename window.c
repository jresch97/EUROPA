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

#include <assert.h>
#include <stdlib.h>

#include "window.h"
#include "surface.h"

WINDOW *winalloc(const char *cap, int x, int y, int w, int h, int d)
{
        return winalloc0(cap, x, y, w, h, d);
}

WINDOW *winalloc0(const char* cap, int x, int y, int w, int h, int d)
{
        return winalloc1(winsysd(), cap, x, y, w, h, d);
}

WINDOW *winalloc1(const WINSYS *sys, const char *cap,
                  int x, int y, int w, int h, int d)
{
        WINDOW* win;
        assert(sys != NULL);
        win = malloc(sizeof(*win));
        if (!win) goto erret;
        win->sys  = sys;
        win->cap  = cap;
        win->open = true;
        win->x    = x;
        win->y    = y;
        win->w    = w;
        win->h    = h;
        win->d    = d;
        win->dat  = NULL;
        if (!win->sys) goto errfw;
        if (!win->sys->drv.winalloc(win)) goto errfw;
        win->surf = surfalloc2(win->sys, NULL, w, h);
        if (!win->surf) goto errwf;
        return win;
errwf:  win->sys->drv.winfree(win);
errfw:  free(win);
erret:  return NULL;
}

WINDOW *winalloc2(const WINOPT *opt)
{
        assert(opt != NULL);
        return NULL;
}

void winfree(WINDOW *win)
{
        if (win) {
                surffree(win->surf);
                win->sys->drv.winfree(win);
                free(win);
        }
}

int winopen(WINDOW *win)
{
        assert(win != NULL);
        return win->open;
}

void winshow(WINDOW *win)
{
        assert(win != NULL);
        win->sys->drv.winshow(win);
}

void winhide(WINDOW *win)
{
        assert(win != NULL);
        win->sys->drv.winhide(win);
}

const char *wincap(WINDOW *win)
{
        assert(win != NULL);
        return win->cap;
}

void winrecap(WINDOW *win, const char *cap)
{
        assert(win != NULL);
        win->sys->drv.winrecap(win, cap);
}

void winpos(WINDOW *win, int *x, int *y)
{
        assert(win != NULL);
        *x = win->x; *y = win->y;
}

void winmove(WINDOW *win, int x, int y)
{
        assert(win != NULL);
        win->sys->drv.winmove(win, x, y);
}

void winsize(WINDOW *win, int *w, int *h)
{
        assert(win != NULL);
        *w = win->w; *h = win->h;
}

void winresize(WINDOW *win, int w, int h)
{
        assert(win != NULL);
        win->sys->drv.winresize(win, w, h);
}

unsigned windepth(WINDOW *win)
{
        assert(win != NULL);
        return win->d;
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
        win->sys->drv.winpush(win);
}
