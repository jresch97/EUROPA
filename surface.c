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

SURFACE* surfalloc (PXFMT pxfmt, int w, int h)
{
        SURFACE* surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->pxfmt = pxfmt;
        surf->w     = w;
        surf->h     = h;
        surf->ownpx = 1;
        surf->px    = malloc(w * h * pxfmt.bypp);
        if (!surf->px) goto errfsurf;
        return surf;
errfsurf:
        free(surf);
        return NULL;
}

SURFACE* surfwrap (PXFMT pxfmt, int w, int h, void *px)
{
        SURFACE* surf;
        assert(px != NULL);
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->pxfmt = pxfmt;
        surf->w     = w;
        surf->h     = h;
        surf->ownpx = 0;
        surf->px    = px;
        return surf;
}

void surffree (SURFACE *surf)
{
        assert(surf != NULL);
        if (surf) {
                if (surf->ownpx && surf->px) free(surf->px);
                free(surf);
        }
}