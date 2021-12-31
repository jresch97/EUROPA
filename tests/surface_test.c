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
#include <surface.h>

int main(int argc, char *argv[])
{
        SURFACE   *swsurf, wrapsurf;
        HWSURFACE *hwsurf;
        void      *px;
        int       *ipx;

        // Allocate arbitrary memory.
        void *px = malloc(640 * 480 * 4);

        // Software surfaces exist in RAM.
        swsurf = surfalloc(RGBA32, 640, 480); // Allocate 32-bit 640x480 surface in RAM.
        wrapsurf = surfwrap(RGBA32, 640, 480, px); // Wrap existing memory as a SW surface.

        // Hardware surfaces are managed by the OS.
        hwsurf = hwsurfalloc(RGBA32, 640, 480); // Allocate 32-bit 640x480 surface via OS.

        // Blit SW surface to SW surface.
        surfblit(swsurf, wrapsurf);

        // Blit HW surface to HW surface. Internally uses OS copying.
        hwsurfblit(hwsurf, hwsurf);

        // Blit SW surface to HW surface.
        hwsurfswblit(hwsurf, swsurf);

        // Retrieve SW surface memory (as void*) and reinterpret as int*.
        ipx = (int*)surfpx(swsurf);

        // Free SW surfaces.
        surffree(swsurf);
        surffree(wrapsurf);

        // Free HW surfaces.
        hwsurffree(hwsurf);

        // Wrapped surfaces do not own the memory so must be freed.
        free(px);

        return EXIT_SUCCESS;
}