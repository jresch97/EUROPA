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

#ifndef EUROPA_WIN32SYS_H
#define EUROPA_WIN32SYS_H

#include "platform.h"

#ifdef PLATFORM_WIN32

#include "winsys.h"

int  win32init    ();
void win32term    ();
void win32poll    ();
int  win32alloc(WINDOW *win, PXFMT *pxfmt, void **px);
void win32free (WINDOW *win);
void win32rettl(WINDOW* win, const char* title);
void win32mov  (WINDOW* win, int x, int y);
void win32resz (WINDOW* win, int w, int h);
void win32swap (WINDOW *win);

static const WINSYS WIN32SYS = {
        "win32",
        {
                &win32init,
                &win32term,
                &win32poll,
                &win32alloc,
                &win32free,
                &win32mov,
                &win32resz,
                &win32recap,
                &win32swap
        }
};

#endif

#endif