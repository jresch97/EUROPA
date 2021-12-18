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

SURFACE* surfalloc(PXFMT pxfmt, int w, int h)
{
        SURFACE* surf = malloc(sizeof(*surf));
        if (!surf) goto err_ret;
        surf->pxfmt = pxfmt;
        surf->w = w;
        surf->h = h;
        surf->px = malloc(w * h * pxfmt.bypp);
        surf->ownpx = 1;
        return surf;
err_ret:
        return NULL;
}

SURFACE* surfwrap(PXFMT pxfmt, int w, int h, int* px)
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