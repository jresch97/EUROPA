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
#include <time.h>
#include <window.h>

#define WINTTL "EUROPA"
#define WINW    640
#define WINH    480
#define WIND    32

typedef struct timespec TIMESPEC;

void deltatime(TIMESPEC *t1, TIMESPEC *t2, TIMESPEC *td)
{
        /* https://stackoverflow.com/a/53708448 */
        td->tv_nsec = t2->tv_nsec - t1->tv_nsec;
        td->tv_sec  = t2->tv_sec  - t1->tv_sec;
        if (td->tv_sec > 0 && td->tv_nsec < 0) {
                td->tv_nsec += 1000000000, td->tv_sec--;
        }
        else if (td->tv_sec < 0 && td->tv_nsec > 0) {
                td->tv_nsec -= 1000000000, td->tv_sec++;
        }
}

int main(void)
{
        const WINSYS *sys;
        WINDOW       *win;
        SURFACE      *surf;
        TIMESPEC      start, end, delta;
        int           x, y, c, i, s, ms, us, ns, fps;
        double        dt, accum;
        sys   = winsysd();
        wininit(sys);
        win   = winalloc(WINTTL, WINCTR, WINCTR, WINW, WINH, WIND, NULL);
        surf  = winsurf(win);
        i     = fps = 0;
        accum = 0.0;
        while (winopen(win)) {
                clock_gettime(CLOCK_MONOTONIC_RAW, &start);
                for (y = 0; y < surf->h; y++) {
                        for (x = 0; x < surf->w; x++) {
                                c = ((x + i) ^ (y + i)) & 0xff;
                                ((int*)surf->px)[x + y * surf->w] = c;
                        }
                }
                winswap(win);
                winpoll(sys);
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                deltatime(&start, &end, &delta);
                dt = (double)delta.tv_sec + (double)delta.tv_nsec;
                s  = (int)(dt / 1000000000.0);
                ms = (int)(dt / 1000000.0);
                us = (int)(dt / 1000.0);
                ns = (int)(dt);
                accum += dt;
                if (accum >= 1000000000.0) {
                        printf("fps=%d,dt=%ds,%dms,%dus,%dns\n",
                               fps, s, ms, us, ns);
                        fps = 0;
                        accum = 0.0;
                }
                i++, fps++;
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}
