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
#include <window.h>

#ifdef PLATFORM_LINUX
#include <time.h>
#include <unistd.h>
#endif

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#define WINCAP "EUROPA"
#define WINW    640
#define WINH    480
#define WIND    32
#define TGTFPS  60
#define MSPERS  1000
#define USPERS  1000000
#define NSPERS  1000000000

#ifdef PLATFORM_LINUX
typedef struct timespec TIMESPEC;

long long deltatime(TIMESPEC *t1, TIMESPEC *t2)
{
        /* https://stackoverflow.com/a/53708448 */
        TIMESPEC td;
        td.tv_nsec = t2->tv_nsec - t1->tv_nsec;
        td.tv_sec  = t2->tv_sec  - t1->tv_sec;
        if (td.tv_sec > 0 && td.tv_nsec < 0) {
                td.tv_nsec += NSPERS, td.tv_sec--;
        }
        else if (td.tv_sec < 0 && td.tv_nsec > 0) {
                td.tv_nsec -= NSPERS, td.tv_sec++;
        }
        return (td.tv_sec * NSPERS) + td.tv_nsec
}
#endif

#ifdef PLATFORM_WIN32
long long deltatime(LARGE_INTEGER *t1, LARGE_INTEGER *t2)
{
        return t2->QuadPart - t1->QuadPart;
}
#endif

int main(int argc, char *argv[])
{
        const WINSYS *sys;
        WINDOW       *win;
        SURFACE      *surf;
#ifdef PLATFORM_LINUX
        TIMESPEC      start, end;
#endif
#ifdef PLATFORM_WIN32
        LARGE_INTEGER start, end, freqq, sl1, sl2;
#endif
        int           x, y, c, i, fps, tgtfps, showfps;
        long long     a, b, dt, accum, freq;
        sys  = winsysd();
        wininit(sys);
        win  = winalloc(WINCAP, WINCTR, WINCTR, WINW, WINH, WIND, NULL);
        surf = winsurf(win);
        accum = 0;
        i = fps = 0, showfps = -1;
        tgtfps = (argc > 1) ? atoi(argv[1]) : TGTFPS;
        while (winopen(win)) {
#ifdef PLATFORM_LINUX
                freq = NSPERS;
                clock_gettime(CLOCK_MONOTONIC_RAW, &start);
#endif
#ifdef PLATFORM_WIN32
                QueryPerformanceFrequency(&freqq);
                QueryPerformanceCounter(&start);
                freq = freqq.QuadPart;
#endif
                for (y = 0; y < surf->h; y++) {
                        for (x = 0; x < surf->w; x++) {
                                c = ((x + i) ^ (y + i)) & 0xff;
                                ((int*)surf->px)[x + y * surf->w] = c;
                        }
                }
                winswap(win);
                winpoll(sys);
                fps++, i++;
                if (showfps >= 0) {
                        printf("fps=%d\n", showfps);
                        showfps = -1;
                }
#ifdef PLATFORM_LINUX
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
#endif
#ifdef PLATFORM_WIN32
                QueryPerformanceCounter(&end);
#endif
                dt = deltatime(&start, &end);
                if (tgtfps > 0) {
#ifdef PLATFORM_LINUX
                        a = (freq / tgtfps) - dt;
                        b = a / freq;
                        end.tv_sec  = b;
                        end.tv_nsec = a - (b * NSPERS);
                        if (end.tv_sec >= 0 && end.tv_nsec >= 0) {
                                nanosleep(&end, NULL);
                        }
#endif
#ifdef PLATFORM_WIN32
                        a = (freq / tgtfps) - dt;
                        b = 0;
                        QueryPerformanceCounter(&sl1);
                        do {
                                QueryPerformanceCounter(&sl2);
                                b += deltatime(&sl1, &sl2);
                                sl1 = sl2;
                        } while (b < a);
#endif
                }
#ifdef PLATFORM_LINUX
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
#endif
#ifdef PLATFORM_WIN32
                QueryPerformanceCounter(&end);
#endif
                dt = deltatime(&start, &end);
                accum += dt;
                if (accum >= freq) {
                        showfps = fps;
                        accum   = 0;
                        fps     = 0;
                }
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}
