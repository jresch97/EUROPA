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
        int *px;
        HWSURFACE *hwsurf;

        // Hardware surfaces need access to OS.
        defwinsysinit();

        // Hardware surfaces are managed by the OS.
        hwsurf = hwsurfalloc(RGBA32, 8, 8); // Allocate 32-bit 640x480 surface via OS.

        printf("hwsurf=%p,hwsurf->px=%p\n", (void*)hwsurf, hwsurf->px);

        for (int y = 0; y < hwsurf->h; y++) {
                for (int x = 0; x < hwsurf->w; x++) {
                        px = (int*)hwsurf->px + x + (y * hwsurf->w);
                        *px = (x & y); 
                        printf("0x%08x%s", *px, (x + 1) < hwsurf->w ? " " : "");
                }
                printf("\n");
        }

        // Free HW surface.
        hwsurffree(hwsurf);

        // Terminate default windowing system.
        defwinsysterm();

        return EXIT_SUCCESS;
}