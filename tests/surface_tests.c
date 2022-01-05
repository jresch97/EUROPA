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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <surface.h>

int main(int argc, char *argv[])
{
        SURFACE *surf, *wsurf;
        void    *px;
        //int     *ipx;

        /* Allocate arbitrary memory. */
        px = malloc(640 * 480 * RGBA32.bypp);
        printf("px=%p\n", px);

        /* Software surfaces exist in RAM. */
        surf = surfalloc(RGBA32, 640, 480); /* RGBA 32-bit 640x480. */
        printf("surf=%p,surf->px=%p\n", (void*)surf, surf->px);
        wsurf = surfwrap(RGBA32, 640, 480, px); /* Wrap existing memory as a SW surface. */
        printf("wsurf=%p,wsurf->px=%p\n", (void*)wsurf, wsurf->px);

        /* Blit surface to surface. */
        //surfblit(surf, wsurf, 0, 0, 640, 480);

        /* Retrieve surface memory as (void*) and reinterpret as (int*). */
        //ipx = (int*)surfpx(surf);

        /* Free surfaces. */
        surffree(surf);
        surffree(wsurf);

        /* Wrapped surfaces do not own the memory so it must be freed. */
        free(px);

        return EXIT_SUCCESS;
}