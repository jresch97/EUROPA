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

#ifndef EUROPA_WINDOW_WIN32_H
#define EUROPA_WINDOW_WIN32_H

#include "platform.h"

#ifdef PLATFORM_WIN32

#include "window.h"

int  win32_winalloc(WINDOW *win);
void win32_winfree(WINDOW *win);
void win32_winsize(WINDOW *win, int* w, int* h);
void win32_winpos(WINDOW *win, int* x, int* y);

static const WINDRV WIN32_WINDRV = {
        "win32",
        &win32_winalloc,
        &win32_winfree,
        &win32_winsize,
        &win32_winpos
};

#endif

#endif