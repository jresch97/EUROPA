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
#include <limits.h>
#include <math.h>
#include <platform.h>
#include <clock.h>
#include <window.h>

#define WINC "EUROPA"
#define WINX WINCTR
#define WINY WINCTR
#define WINW 640
#define WINH 480
#define WIND 32
#define TFPS 60

int main(int argc, char *argv[])
{
        WINDOW   *win;
        SURFACE  *surf;
        int      c;
        size_t   i, x, y, fps, fc, tfps;
        uint64_t s, e, f, d, a;
        wininit();
        win  = winalloc(WINC, WINX, WINY, WINW, WINH, WIND, NULL);
        surf = winsurf(win);
        i    = fc = 0, a = 0, fps = UINT_MAX;
        tfps = argc > 1 ? atoi(argv[1]) : TFPS;
        printf("winsysd()->name=\"%s\"\n", winsysd()->name);
        while (winopen(win)) {
                s = clkelapt();
                for (y = 0; y < surf->h; y++) {
                        for (x = 0; x < surf->w; x++) {
                                c = ((x + i) ^ (y + i)) & 0xff;
                                ((int*)surf->px)[x + y * surf->w] = c;
                        }
                }
                winswap(win);
                winpoll();
                if (fps < UINT_MAX) {
                        printf("fps=%lu\n", fps);
                        fps = UINT_MAX;
                }
                if (tfps > 0) {
                    e = clkelapt(), f = clkfreq() / tfps, d = e - s;
                    if (d < f) clkslept(f - d);
                }
                e = clkelapt(), f = clkfreq(), d = e - s;
                a += (e - s);
                if (a >= f) {
                        fps = fc;
                        a   = fc = 0;
                }
                fc++, i++;
        }
        winfree(win);
        winterm();
        return EXIT_SUCCESS;
}
