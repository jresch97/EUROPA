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

#ifndef EUROPA_SURFACE_H
#define EUROPA_SURFACE_H

#include "pxfmt.h"

typedef struct SURFACE SURFACE;

typedef struct SURFDRV {
        const char *name;
        int  (*surfalloccb)(SURFACE* surf);
        void (*surffreecb) (SURFACE* surf);
} SURFDRV;

typedef struct SURFACE {
        const SURFDRV *drv;
        PXFMT pxfmt;
        int w, h, ownpx;
        void *px, *dat;
} SURFACE;

SURFACE* surfalloc(PXFMT pxfmt, int w, int h);
SURFACE* surfwrap (PXFMT pxfmt, int w, int h, void *px);
void     surffree (SURFACE *surf);

#endif