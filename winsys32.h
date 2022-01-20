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

#ifndef EUROPA_WINSYS32_H
#define EUROPA_WINSYS32_H

#include <stddef.h>

#include "platform.h"

#ifdef PLATFORM_WIN32

#include "winsys.h"

int  wininit32   ();
void winterm32   ();
void winpoll32   ();
int  winalloc32  (WINDOW *win);
void winfree32   (WINDOW *win);
void winrecap32  (WINDOW *win, const char* title);
void winmove32   (WINDOW *win, int x, int y);
void winresize32 (WINDOW *win, int w, int h);
void winswap32   (WINDOW *win);
int  surfalloc32 (SURFACE *surf, PXFMT *pxfmt);
void surffree32  (SURFACE *surf);

static const WINSYS WINSYS32 = {
        "win32",
        {
                &wininit32,
                &winterm32,
                &winpoll32,
                &winalloc32,
                &winfree32,
                NULL, /* winshow */
                NULL, /* winhide */
                &winrecap32,
                &winmove32,
                &winresize32,
                &winswap32,
                NULL, /* winpush */
                &surfalloc32,
                &surffree32
        }
};

#endif

#endif
