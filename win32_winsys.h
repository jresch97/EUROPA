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

#ifndef EUROPA_WIN32_WINSYS_H
#define EUROPA_WIN32_WINSYS_H

#include "platform.h"

#ifdef PLATFORM_WIN32

#include "winsys.h"

int  win32_init    ();
void win32_term    ();
void win32_poll    ();
int  win32_winalloc(WINDOW *win, PXFMT *pxfmt, void **px);
void win32_winfree (WINDOW *win);
void win32_winrettl(WINDOW* win, const char* title);
void win32_winmov  (WINDOW* win, int x, int y);
void win32_winresz (WINDOW* win, int w, int h);
void win32_winswap (WINDOW *win);

static const WINSYS WIN32_WINSYS = {
        "win32",
        {
                &win32_init,
                &win32_term,
                &win32_poll,
                &win32_winalloc,
                &win32_winfree,
                &win32_winrettl,
                &win32_winmov,
                &win32_winresz,
                &win32_winswap
        }
};

#endif

#endif