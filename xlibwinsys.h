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

#ifndef EUROPA_XLIBWINSYS_H
#define EUROPA_XLIBWINSYS_H

#include "platform.h"

#ifdef PLATFORM_LINUX

#include "winsys.h"

int  xlibinit        ();
void xlibterm        ();
void xlibpoll        ();
int  xlibshmav       ();
int  xlibshmvers     (int *maj, int *min, int *pxmav);
int  xlibshmpxmav    ();
int  xlibwinalloc    (WINDOW *win);
void xlibwinfree     (WINDOW *win);
void xlibwinpos      (WINDOW *win, int* x, int* y);
void xlibwinsize     (WINDOW *win, int* w, int* h);
int  xlibhwsurfalloc (HWSURFACE *surf);
void xlibhwsurffree  (HWSURFACE *surf);

static const WINSYS XLIBWINSYS = {
        "x11/xlib",
        {
                &xlibinit,
                &xlibterm,
                &xlibpoll,
                &xlibwinalloc,
                &xlibwinfree,
                &xlibwinpos,
                &xlibwinsize,
                &xlibhwsurfalloc,
                &xlibhwsurffree
        }
};

#endif

#endif