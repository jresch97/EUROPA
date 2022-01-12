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

#ifndef EUROPA_XLIB_WINSYS_H
#define EUROPA_XLIB_WINSYS_H

#include "platform.h"

#ifdef PLATFORM_LINUX

#include "winsys.h"

int  xlib_init    ();
void xlib_term    ();
void xlib_poll    ();
int  xlib_shmav   ();
int  xlib_shmvers (int *maj, int *min, int *pxmav);
int  xlib_shmpxmav();
int  xlib_winalloc(WINDOW *win);
void xlib_winfree (WINDOW *win);
void xlib_winswap (WINDOW *win);

static const WINSYS XLIB_WINSYS = {
        "x11/xlib",
        {
                &xlib_init,
                &xlib_term,
                &xlib_poll,
                &xlib_winalloc,
                &xlib_winfree,
                &xlib_winswap,
                /* ... */
        }
};

#endif

#endif