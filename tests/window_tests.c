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
        int           x, y, c, i, fps, tfps, dfps;
        long long     s, e, accum, slpt, slpc;
#ifdef PLATFORM_LINUX
        TIMESPEC      slp;
#elif  PLATFORM_WIN32
        LARGE_INTEGER slp1, slp2;
#endif
        sys  = winsysd();
        wininit(sys);
        win  = winalloc(WINCAP, WINCTR, WINCTR, WINW, WINH, WIND, NULL);
        surf = winsurf(win);
        i    = fps = 0, accum = 0, dfps = -1;
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
                fps++, i++;
                if (dfps >= 0) {
                        printf("fps=%d\n", dfps);
                        dfps = -1;
                }
                e  = clkelapt();
                if (tfps > 0) {
                        slpt = (clkfreq() / tfps) - (e - s);
#ifdef PLATFORM_LINUX
                        slp.tv_sec  = slpt / clkfreq();
                        slp.tv_nsec = slpt - (slp.tv_sec * NSPERS);
                        if (slp.tv_sec >= 0 && slp.tv_nsec >= 0) {
                                nanosleep(&slp, NULL);
                        }
#elif  PLATFORM_WIN32
                        slpc = 0;
                        QueryPerformanceCounter(&sl1);
                        do {
                                QueryPerformanceCounter(&sl2);
                                slpc += deltatime(&sl1, &sl2);
                                sl1 = sl2;
                        } while (slpc < slpt);
#endif
                }
                e  = clkelapt();
                accum += (e - s);
                if (accum >= clkfreq()) {
                        dfps  = fps;
                        accum = fps = 0;
                }
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}
