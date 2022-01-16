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
#include <string.h>
#include <math.h>
#include <window.h>

#define WINTTL "EUROPA"
#define WINW    640
#define WINH    480
#define WIND    32

int main(void)
{
        const WINSYS *sys;
        WINDOW       *win;
        SURFACE      *surf;
        int           x, y, i;
        sys  = winsysd();
        wininit(sys);
        win  = winalloc(WINTTL, WINCTR, WINCTR, WINW, WINH, WIND, NULL);
        surf = winsurf(win);
        i    = 0;
        while (winopen(win)) {
                for (y = 0; y < surf->h; y++) {
                        for (x = 0; x < surf->w; x++) {
                                surfipxw(surf, x, y, ((x + i) ^ (y + i)) & 0xff);
                        }
                }
                winswap(win);
                winpoll(sys);
                i++;
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}
