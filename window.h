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

#include "surface.h"

#define WINPOSUND -1
#define WINPOSCTR -2

typedef struct WINDOW WINDOW;

typedef struct WINDRV {
        const char *name;
        int  (*winalloccb)(WINDOW* win);
        void (*winfreecb)(WINDOW* win);
        void (*winsizecb)(WINDOW* win, int* w, int* h);
        void (*winposcb)(WINDOW* win, int* x, int* y);
} WINDRV;

struct WINDOW {
        const char *title;
        int x, y, w, h;
        SURFACE *surf;
        const WINDRV *drv;
        void* dat;
};

WINDOW*       winalloc(const char *title, int x, int y, int w, int h);
void          winfree(WINDOW *win);
const WINDRV* windrv(WINDOW *win);
void          winsize(WINDOW *win, int *w, int *h);
void          winpos(WINDOW *win, int *x, int *y);
SURFACE*      winsurf(WINDOW *win);
int*          winpx(WINDOW *win);
void          winpoll();

#endif