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

#include "surface.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

SURFACE* surfalloc(PXFMT pxfmt, int w, int h)
{
        SURFACE *surf;
        assert(w > 0 && h > 0);
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->sys   = NULL;
        surf->pxfmt = pxfmt;
        surf->w     = w;
        surf->h     = h;
        surf->pxsz  = w * h;
        surf->bysz  = w * h * pxfmt.bypp;
        surf->ownpx = 1;
        surf->px    = malloc(surf->bysz);
        surf->dat   = NULL;
        if (!surf->px) goto errfs;
        return surf;
errfs:  free(surf);
        return NULL;
}

SURFACE* surfwrap(PXFMT pxfmt, int w, int h, void *px)
{
        SURFACE *surf;
        assert(px != NULL);
        assert(w > 0 && h > 0);
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->sys   = NULL;
        surf->pxfmt = pxfmt;
        surf->w     = w;
        surf->h     = h;
        surf->pxsz  = w * h;
        surf->bysz  = w * h * pxfmt.bypp;
        surf->ownpx = 0;
        surf->dat   = NULL;
        surf->px    = px;
        return surf;
}

SURFACE* surfallocs(const WINSYS *sys, PXFMT *pxfmt, int w, int h)
{
        SURFACE *surf;
        assert(w > 0 && h > 0);
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->sys   = sys ? sys : winsysd();
        memset(&surf->pxfmt, 0, sizeof(PXFMT));
        surf->w     = w;
        surf->h     = h;
        surf->pxsz  = w * h;
        surf->bysz  = 0;
        surf->ownpx = 1;
        surf->dat   = surf->px = NULL;
        if (!sys->drv.surfalloc(surf, pxfmt)) goto errfs;
        return surf;
errfs:  free(surf);
        return NULL;
}

SURFACE* surfallocd(PXFMT *pxfmt, int w, int h)
{
        assert(w > 0 && h > 0);
        return surfallocs(NULL, pxfmt, w, h);
}

void surffree(SURFACE *surf)
{
        assert(surf != NULL);
        if (surf) {
                if (surf->sys) surf->sys->drv.surffree(surf);
                else if (surf->ownpx && surf->px) free(surf->px);
                free(surf);
        }
}

void* surfpx(SURFACE* surf)
{
        assert(surf != NULL);
        return surf->px;
}

int* surfipx(SURFACE* surf)
{
        assert(surf != NULL);
        return (int*)surf->px;
}

int surfipxr(SURFACE *surf, int x, int y)
{
        assert(surf != NULL);
        assert(x >= 0 && y >= 0 && x < surf->w && y < surf->h);
        return surfipx(surf)[x + y * surf->w];
}

void surfipxw(SURFACE* surf, int x, int y, int c)
{
        assert(surf != NULL);
        assert(x >= 0 && y >= 0 && x < surf->w && y < surf->h);
        surfipx(surf)[x + y * surf->w] = c;
}

/* TODO: What if PXFMT.bipp is not 2^N? */
void surfclr(SURFACE *surf, int c)
{
        int i;
        assert(surf != NULL);
        for (i = 0; i < surf->pxsz; i++) {
                ((int*)surf->px)[i] = c;
        }
}

/* TODO: What if PXFMT.bipp is not 32? */
void surfln(SURFACE *surf, int x1, int y1, int x2, int y2, int c)
{
        /* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
        int dx, dy, sx, sy, e, e2;
        assert(surf != NULL);
        assert(x1 >= 0 && x2 >= 0 && y1 >= 0 && y2 >= 0 &&
               x1 < surf->w && x2 < surf->w && y1 < surf->h && y2 < surf->h);
        dx =  abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        if (x1 == x2) {
                e = y2 + sy;
                while (y1 != e) {
                        surfipxw(surf, x1, y1, c);
                        y1 += sy;
                }
        }
        else if (y1 == y2) {
                e = x2 + sx;
                while (x1 != e) {
                        surfipxw(surf, x1, y1, c);
                        x1 += sx;
                }
        }
        else {
                e = dx + dy;
                while (1) {
                        surfipxw(surf, x1, y1, c);
                        if (x1 == x2 && y1 == y2) break;
                        e2 = e * 2;
                        if (e2 >= dy) e += dy, x1 += sx;
                        if (e2 <= dx) e += dx, y1 += sy;
                }
        }
}