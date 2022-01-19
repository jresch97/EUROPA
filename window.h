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

#ifndef EUROPA_WINDOW_H
#define EUROPA_WINDOW_H

#include <stdbool.h>

#include "winsys.h"
#include "surface.h"

#define WINDEF -1
#define WINCTR -2

struct WINDOW {
        const WINSYS *sys;
        const char   *cap;
        int           x, y, w, h, d;
        bool          open;
        SURFACE      *surf;
        void         *dat;
};

typedef struct WINOPT {
        const WINSYS *sys;
        const char   *cap;
        int           x, y, w, h, d;
        bool          open;
} WINOPT;

WINDOW     *winalloc  (const char *cap, int x, int y, int w, int h, int d);
WINDOW     *winalloc0 (const char *cap, int x, int y, int w, int h, int d);
WINDOW     *winalloc1 (const WINSYS *sys, const char *cap,
                       int x, int y, int w, int h, int d);
WINDOW     *winalloc2 (const WINOPT* opt);
void        winfree   (WINDOW *win);
bool        winopen   (WINDOW *win);
void        winshow   (WINDOW *win);
void        winhide   (WINDOW *win);
const char *wincap    (WINDOW *win);
void        winrecap  (WINDOW *win, const char *cap);
void        winpos    (WINDOW *win, int *x, int *y);
void        winmove   (WINDOW *win, int  x, int  y);
void        winsize   (WINDOW *win, int *w, int *h);
void        winresize (WINDOW *win, int  w, int  h);
unsigned    windepth  (WINDOW *win);
SURFACE    *winsurf   (WINDOW *win);
void       *winpx     (WINDOW *win);
void        winswap   (WINDOW *win);
void        winpush   (WINDOW *win);

#endif
