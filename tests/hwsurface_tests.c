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
#include <hwsurface.h>

int main(int argc, char *argv[])
{
        HWSURFACE *hwsurf;
        int       *px;

        /* Hardware surfaces need access to the operating/windowing system. */
        defwinsysinit();

        /* Hardware surfaces are managed by the operating/windowing system. */
        hwsurf = hwsurfalloc(RGBA32, 8, 8); /* RGBA 32-bit 8x8 surface. */
        printf("hwsurf=%p,hwsurf->px=%p\n", (void*)hwsurf, hwsurf->px);

        /* Memory read/write. */
        for (int y = 0; y < hwsurf->h; y++) {
                for (int x = 0; x < hwsurf->w; x++) {
                        px = (int*)hwsurf->px + x + (y * hwsurf->w);
                        *px = (x & y); 
                        printf("0x%08x%s", *px, (x + 1) < hwsurf->w ? " " : "");
                }
                printf("\n");
        }

        hwsurffree(hwsurf); /* Free surface memory. */

        defwinsysterm(); /* Terminate default windowing system. */

        return EXIT_SUCCESS;
}