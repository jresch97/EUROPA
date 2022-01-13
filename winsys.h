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

#ifndef EUROPA_WINSYS_H
#define EUROPA_WINSYS_H

#include "pxfmt.h"

typedef struct WINSYS   WINSYS;
typedef struct WINDOW   WINDOW;
typedef struct GLWINDOW GLWINDOW;
typedef struct VKWINDOW VKWINDOW;
typedef struct DXWINDOW DXWINDOW;

struct WINSYS {
        const char *name;
        const struct {
                int  (*init)    ();
                void (*term)    ();
                void (*poll)    ();
                int  (*winalloc)(WINDOW *win, PXFMT *pxfmt, void **px);
                void (*winfree) (WINDOW *win);
                void (*winrettl)(WINDOW *win, const char *title);
                void (*winmov)  (WINDOW *win, int x, int y);
                void (*winresz) (WINDOW *win, int w, int h);
                void (*winswap) (WINDOW *win);
        } drv;
};

const WINSYS* winsysd();
const WINSYS* winsysn(const char *name);
int           wininit(const WINSYS *sys);
void          winterm(const WINSYS *sys);
void          winpoll(const WINSYS *sys);

#endif