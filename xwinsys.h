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

int           xinit      ();
void          xterm      ();
void          xpoll      ();
int           xwinalloc  (WINDOW *win);
void          xwinfree   (WINDOW *win);
void          xwinswap   (WINDOW *win);
int           xsurfalloc (SURFACE *surf, PXFMT *pxfmt);
void          xsurffree  (SURFACE *surf);
int           xshmav     ();
int           xshmvers   (int *maj, int *min, int *pxmav);
int           xshmpxmav  ();
unsigned long xblackpx   ();
unsigned long xwhitepx   ();

static const WINSYS XWINSYS = {
        "x11/xlib",
        {
                &xinit,
                &xterm,
                &xpoll,
                &xwinalloc,
                &xwinfree,
                NULL, /* xwinshow */
                NULL, /* xwinhide */
                NULL, /* xwinrecap */
                NULL, /* xwinmove */
                NULL, /* xwinresize */
                &xwinswap,
                NULL, /* xwinpush */
                &xsurfalloc,
                &xsurffree
        }
};

#endif

#endif
