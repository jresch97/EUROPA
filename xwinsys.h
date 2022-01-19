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

#ifndef EUROPA_XWINSYS_H
#define EUROPA_XWINSYS_H

#include <stddef.h>

#include "platform.h"

#ifdef PLATFORM_LINUX

#include <X11/Xlib.h>

#include "winsys.h"

bool          xinit     ();
void          xterm     ();
void          xpoll     ();
bool          xwinalloc (WINDOW *win);
void          xwinfree  (WINDOW *win);
void          xwinswap  (WINDOW *win);
bool          xsurfalloc(SURFACE *surf, PXFMT *pxfmt);
void          xsurffree (SURFACE *surf);
bool          xshmav    ();
bool          xshmvers  (int *maj, int *min, int *pxmav);
bool          xshmpxmav ();
unsigned long xblackpx  ();
unsigned long xwhitepx  ();

static const WINSYS XWINSYS = {
        "x11/xlib",
        {
                &xinit,
                &xterm,
                &xpoll,
                &xwinalloc,
                &xwinfree,
                NULL, /* winshow */
                NULL, /* winhide */
                NULL, /* winrecap */
                NULL, /* winmove */
                NULL, /* winresize */
                &xwinswap,
                &xsurfalloc,
                &xsurffree
        }
};

#endif

#endif
