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

#ifndef EUROPA_WINSYS_H
#define EUROPA_WINSYS_H

#include <stdbool.h>

#include "pxfmt.h"

typedef struct WINSYS   WINSYS;
typedef struct WINDOW   WINDOW;
typedef struct GLWINDOW GLWINDOW;
typedef struct VKWINDOW VKWINDOW;
typedef struct DXWINDOW DXWINDOW;
typedef struct SURFACE  SURFACE;

struct WINSYS {
        const char *name;
        const struct {
                bool (*init)();
                void (*term)();
                void (*poll)();
                bool (*winalloc)  (WINDOW  *win);
                void (*winfree)   (WINDOW  *win);
                void (*winshow)   (WINDOW  *win);
                void (*winhide)   (WINDOW  *win);
                void (*winrecap)  (WINDOW  *win, const char *cap);
                void (*winmove)   (WINDOW  *win, int x, int y);
                void (*winresize) (WINDOW  *win, int w, int h);
                void (*winswap)   (WINDOW  *win);
                void (*winpush)   (WINDOW  *win);
                bool (*surfalloc) (SURFACE *surf, PXFMT *pxfmt);
                void (*surffree)  (SURFACE *surf);
        } drv;
};

const WINSYS *winsysn  (const char *name);
const WINSYS *winsysd  ();
bool          wininit  ();
bool          wininit0 ();
bool          wininit1 (const WINSYS *sys);
void          winterm  ();
void          winterm0 ();
void          winterm1 (const WINSYS *sys);
void          winpoll  ();
void          winpoll0 ();
void          winpoll1 (const WINSYS *sys);

#endif
