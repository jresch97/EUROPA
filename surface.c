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

#include "surface.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

SURFACE *surfalloc(PXFMT pxfmt, int w, int h)
{
        return surfalloc0(pxfmt, w, h);
}

SURFACE *surfalloc0(PXFMT pxfmt, int w, int h)
{
        SURFACE *surf;
        assert(w > 0 && h > 0);
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->sys   = NULL;
        surf->pxfmt = pxfmt;
        surf->w     = w;
        surf->h     = h;
        surf->size  = w * h;
        surf->bytes = w * h * pxfmt.bypp;
        surf->ownpx = true;
        surf->px    = malloc(surf->bytes);
        surf->dat   = NULL;
        if (!surf->px) goto errfs;
        return surf;
errfs:  free(surf);
        return NULL;
}

SURFACE *surfalloc1(PXFMT *pxfmt, int w, int h)
{
        return surfalloc2(winsysd(), pxfmt, w, h);
}

SURFACE *surfalloc2(const WINSYS* sys, PXFMT *pxfmt, int w, int h)
{
        SURFACE* surf;
        assert(sys != NULL);
        assert(w > 0 && h > 0);
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->sys   = sys;
        surf->w     = w;
        surf->h     = h;
        surf->size  = w * h;
        surf->ownpx = true;
        surf->px    = surf->dat = NULL;
        if (!surf->sys->drv.surfalloc(surf, pxfmt)) goto errfs;
        return surf;
errfs:  free(surf);
        return NULL;
}

SURFACE *surfwrap(PXFMT pxfmt, int w, int h, void *px)
{
        SURFACE *surf;
        assert(w > 0 && h > 0);
        assert(px != NULL);
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->sys   = NULL;
        surf->pxfmt = pxfmt;
        surf->w     = w;
        surf->h     = h;
        surf->size  = w * h;
        surf->bytes = w * h * pxfmt.bypp;
        surf->ownpx = false;
        surf->dat   = NULL;
        surf->px    = px;
        return surf;
}

void surffree(SURFACE *surf)
{
        if (surf) {
                if (surf->sys)     surf->sys->drv.surffree(surf);
                else if (surf->ownpx && surf->px) free(surf->px);
                free(surf);
        }
}

void *surfpx(SURFACE *surf)
{
        assert(surf != NULL);
        return surf->px;
}

uint32_t *surfpx32(SURFACE *surf)
{
        assert(surf != NULL);
        return (uint32_t*)surf->px;
}

uint32_t surfpxr32(SURFACE *surf, int x, int y)
{
        assert(surf != NULL);
        assert(x >= 0 && y >= 0 && x < surf->w && y < surf->h);
        return ((uint32_t*)surf->px)[x + y * surf->w];
}

void surfpxw32(SURFACE *surf, int x, int y, uint32_t c)
{
        assert(surf != NULL);
        assert(x >= 0 && y >= 0 && x < surf->w && y < surf->h);
        ((uint32_t*)surf->px)[x + y * surf->w] = c;
}

/* TODO: What if PXFMT.bipp is not 2^N? */
void surfclr32(SURFACE *surf, uint32_t c)
{
        int i;
        assert(surf != NULL);
        for (i = 0; i < surf->size; i++) {
                ((uint32_t*)surf->px)[i] = c;
        }
}

/* TODO: What if PXFMT.bipp is not 32? */
void surfln32(SURFACE *surf, int x1, int y1, int x2, int y2, uint32_t c)
{
        /* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
        int       dx, dy, sx, sy, e, e2;
        uint32_t *px;
        assert(surf != NULL);
        assert(x1 >= 0 && x2 >= 0 && y1 >= 0 && y2 >= 0 &&
               x1 < surf->w && x2 < surf->w && y1 < surf->h && y2 < surf->h);
        dx =  abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        px = (uint32_t*)surf->px;
        if (x1 == x2) {
                e = y2 + sy;
                while (y1 != e) {
                        px[x1 + y1 * surf->w] = c;
                        y1 += sy;
                }
        }
        else if (y1 == y2) {
                e = x2 + sx;
                while (x1 != e) {
                        px[x1 + y1 * surf->w] = c;
                        x1 += sx;
                }
        }
        else {
                e = dx + dy;
                while (1) {
                        px[x1 + y1 * surf->w] = c;
                        if (x1 == x2 && y1 == y2) break;
                        e2 = e * 2;
                        if (e2 >= dy) e += dy, x1 += sx;
                        if (e2 <= dx) e += dx, y1 += sy;
                }
        }
}
