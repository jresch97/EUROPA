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
#include <unistd.h>

#define WINCAP "EUROPA"
#define WINW    640
#define WINH    480
#define WIND    32
#define TGTFPS  60
#define MSPERS  1000
#define USPERS  1000000
#define NSPERS  1000000000

typedef struct timespec TIMESPEC;

void deltatime(TIMESPEC *t1, TIMESPEC *t2, TIMESPEC *td)
{
        /* https://stackoverflow.com/a/53708448 */
        td->tv_nsec = t2->tv_nsec - t1->tv_nsec;
        td->tv_sec  = t2->tv_sec  - t1->tv_sec;
        if (td->tv_sec > 0 && td->tv_nsec < 0) {
                td->tv_nsec += NSPERS, td->tv_sec--;
        }
        else if (td->tv_sec < 0 && td->tv_nsec > 0) {
                td->tv_nsec -= NSPERS, td->tv_sec++;
        }
}

int main(void)
{
        const WINSYS *sys;
        WINDOW       *win;
        SURFACE      *surf;
        TIMESPEC      start, end, delta;
        int           x, y, c, i, s, ms, us, ns, fps, showfps;
        double        dt, accum;
        sys   = winsysd();
        wininit(sys);
        win  = winalloc(WINCAP, WINCTR, WINCTR, WINW, WINH, WIND, NULL);
        surf = winsurf(win);
        i = s = ms = us = ns = fps = 0;
        showfps = -1;
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
                if (showfps >= 0) {
                        printf("fps=%d\n", showfps);
                        showfps = -1;
                }
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                deltatime(&start, &end, &delta);
                dt = ((double)delta.tv_sec) * NSPERS + (double)delta.tv_nsec;
                usleep((USPERS / TGTFPS) - (int)(dt / 1000.0));
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                deltatime(&start, &end, &delta);
                dt = ((double)delta.tv_sec) * NSPERS + (double)delta.tv_nsec;
                accum += dt;
                if (accum >= (double)NSPERS) {
                        showfps = fps;
                        accum = 0.0;
                        fps = 0;
                }
                i++, fps++;
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}
