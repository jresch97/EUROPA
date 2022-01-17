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
#include <platform.h>
#include <clock.h>
#include <window.h>

#define WINCAP "EUROPA"
#define WINW    640
#define WINH    480
#define WIND    32
#define TGTFPS  60

#ifdef PLATFORM_LINUX

#include <time.h>
#include <unistd.h>

typedef struct timespec TIMESPEC;

#endif

int main(int argc, char *argv[])
{
        const WINSYS *sys;
        WINDOW       *win;
        SURFACE      *surf;
        int           x, y, i, c, fps, fpsc, tfps;
        long long     s, e, a;
        sys  = winsysd();
        wininit(sys);
        win  = winalloc(WINCAP, WINCTR, WINCTR, WINW, WINH, WIND, NULL);
        surf = winsurf(win);
        i    = fpsc = 0, a = 0, fps = -1;
        tfps = argc > 1 ? atoi(argv[1]) : TGTFPS;
        while (winopen(win)) {
                s = clkelapt();
                for (y = 0; y < surf->h; y++) {
                        for (x = 0; x < surf->w; x++) {
                                c = ((x + i) ^ (y + i)) & 0xff;
                                ((int*)surf->px)[x + y * surf->w] = c;
                        }
                }
                winswap(win);
                winpoll(sys);
                if (fps >= 0) {
                        printf("fps=%d\n", fps);
                        fps = -1;
                }
                e  = clkelapt();
                if (tfps > 0) {
                        clkslept((clkfreq() / tfps) - (e - s));
                }
                e = clkelapt();
                a += (e - s);
                if (a >= clkfreq()) {
                        fps = fpsc;
                        a   = fpsc = 0;
                }
                fpsc++, i++;
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}
