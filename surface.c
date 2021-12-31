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

SURFACE* surfalloc(PXFMT pxfmt, int w, int h)
{
        SURFACE* surf = malloc(sizeof(*surf));
        if (!surf) goto err_ret;
        surf->pxfmt = pxfmt;
        surf->w = w;
        surf->h = h;
        surf->px = malloc(w * h * pxfmt.bypp);
        if (!surf->px) goto err_free;
        surf->ownpx = 1;
        return surf;
err_free:
        free(surf);
err_ret:
        return NULL;
}

SURFACE* surfwrap(PXFMT pxfmt, int w, int h, void *px)
{
        SURFACE* surf = malloc(sizeof(*surf));
        if (!surf) goto err_ret;
        surf->pxfmt = pxfmt;
        surf->w = w;
        surf->h = h;
        surf->px = px;
        surf->ownpx = 0;
        return surf;
err_ret:
        return NULL;
}

void surffree(SURFACE *surf)
{
        assert(surf != NULL);
        if (surf) {
                if (surf->ownpx) free(surf->px);
                free(surf);
        }
}