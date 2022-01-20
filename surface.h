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

#ifndef EUROPA_SURFACE_H
#define EUROPA_SURFACE_H

/**
 * 
 * The SURFACE module is concerned with the representation and manipulation of
 * 2D surfaces. Surfaces are grids of arbitrarily sized and formatted bytes.
 * 
 * Provides:
 * - Allocation of surfaces in RAM, or on the display server or window manager
 *   for blitting acceleration.
 * - Software routines for drawing lines and simple shapes.
 * 
 */

#include "pxfmt.h"
#include "winsys.h"

#include <stdint.h>
#include <stdbool.h>

struct SURFACE {
        const WINSYS *sys;
        PXFMT         pxfmt;
        int           w, h, size, bytes;
        bool          ownpx;
        void         *px, *dat;
};

SURFACE  *surfalloc  (PXFMT pxfmt, int w, int h);
SURFACE  *surfalloc0 (PXFMT pxfmt, int w, int h);
SURFACE  *surfalloc1 (PXFMT *pxfmt, int w, int h);
SURFACE  *surfalloc2 (const WINSYS *sys, PXFMT *pxfmt, int w, int h);
SURFACE  *surfwrap   (PXFMT pxfmt, int w, int h, void* px);
void      surffree   (SURFACE *surf);
void     *surfpx     (SURFACE *surf);
uint8_t  *surfpx8    (SURFACE *surf);
uint8_t   surfpxr8   (SURFACE *surf, int x, int y, int ch);
void      surfpxw8   (SURFACE *surf, int x, int y, int ch, uint8_t c);
uint32_t *surfpx32   (SURFACE *surf);
uint32_t  surfpxr32  (SURFACE *surf, int x, int y);
void      surfpxw32  (SURFACE *surf, int x, int y, uint32_t c);
void      surfclr32  (SURFACE *surf, uint32_t c);
void      surfln32   (SURFACE *surf,
                      int x1, int y1, int x2, int y2, uint32_t c);

#endif
