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

#define WINTITLE  "EUROPA"
#define WINWIDTH  640
#define WINHEIGHT 480

int main(void)
{
        const WINSYS *sys;
        WINDOW       *win;
        SURFACE      *surf;
        int           i;
        sys  = winsysd();
        wininit(sys);
        win  = winalloc(WINTITLE, WINCTR, WINCTR, WINWIDTH, WINHEIGHT, NULL);
        surf = winsurf(win);
        while (winopen(win)) {
                surfclr(surf, 0);
                for (i = 0; i < 100; i++) {
                        surfln(winsurf(win),
                               rand() % surf->w, rand() % surf->h,
                               rand() % surf->w, rand() % surf->h,
                               rand() + (rand() << 8) + (rand() << 16));
                }
                surfln(winsurf(win), 0, 0, surf->w - 1, surf->h - 1,
                       0xffffffff);
                surfln(winsurf(win), surf->w - 1, 0, 0, surf->h - 1,
                       0xffffffff);
                winswap(win);
                winpoll(sys);
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}