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

#ifndef EUROPA_WINDOW_H
#define EUROPA_WINDOW_H

#include "winsys.h"
#include "surface.h"

#define WINDEF -1
#define WINCTR -2

struct WINDOW {
        const WINSYS *sys;
        const char   *title;
        int           open;
        double        scale;
        int           x, y, w, h;
        SURFACE      *surf;
        void         *dat;
};

typedef struct WINOPTS {
        const WINSYS *sys;
        double        scale;
} WINOPTS;

WINDOW*     winalloc(const char *title, int x, int y, int w, int h,
                     const WINOPTS *opts);
void        winfree (WINDOW *win);
int         winopen (WINDOW* win);
const char* wintitle(WINDOW* win);
void        winrettl(WINDOW *win, const char *title);
void        winxy   (WINDOW *win, int *x, int *y);
void        winsz   (WINDOW *win, int *w, int *h);
double      winscale(WINDOW *win);
void        winmov  (WINDOW *win, int  x, int  y);
void        winresz (WINDOW *win, int  w, int  h);
void        winrescl(WINDOW *win, double scale);
SURFACE*    winsurf (WINDOW *win);
void*       winpx   (WINDOW *win);
void        winswap (WINDOW *win);
void        winpush (WINDOW *win);

#endif