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

#include "hwsurface.h"

#include <stdlib.h>

HWSURFACE* hwsurfalloc(PXFMT pxfmt, int w, int h)
{
        HWSURFACE *surf;
        surf = malloc(sizeof(*surf));
        if (!surf) return NULL;
        surf->sys = defwinsys();
        if (!surf->sys) goto errfsurf;
        surf->pxfmt = pxfmt;
        surf->w     = w;
        surf->h     = h;
        surf->px    = NULL;
        surf->dat   = NULL;
        if (!surf->sys->drv.hwsurfalloccb(surf)) goto errfsurf;
        return surf;
errfsurf:
        free(surf);
        return NULL;
}

void hwsurffree(HWSURFACE *surf)
{

}