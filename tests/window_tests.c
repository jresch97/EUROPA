/**
 *
 * Copyright (C) 2021 Jared B. Resch
 *
 * This file is part of EUROPA.
 * 
 * EUROPA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * EUROPA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
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

int main(int argc, char *argv[])
{
        WINDOW  *win;
        SURFACE *surf;
        int      x, y;
        unsigned i, fc, fps, tfps;
        uint32_t c, *px;
        uint64_t t0, t1, dt, f, a;
        wininit();
        win  = winalloc("EUROPA", WINCTR, WINCTR, 640, 480, 32);
        surf = winsurf(win);
        i = fc = 0, a = 0, fps = 0xffffffff;
        tfps = argc > 1 ? atoi(argv[1]) : 60;
        printf("winsysd()->name=\"%s\"\n", winsysd()->name);
        while (winopen(win)) {
                f  = clkfreq();
                t0 = clkelap();
                px = (uint32_t*)winpx(win);
                for (y = 0; y < surf->h; y++) {
                        for (x = 0; x < surf->w; x++) {
                                c = (x * x + y * y + i) & 0xff;
                                px[x + y * surf->w] = c;
                        }
                }
                winswap(win);
                winpoll();
                if (fps < 0xffffffff) {
                        printf("fps=%u\n", fps);
                        fps = 0xffffffff;
                }
                if (tfps > 0) {
                        f  = clkfreq() / tfps;
                        t1 = clkelap();
                        dt = t1 - t0;
                        if (dt < f) clkslep(f - dt);
                }
                f  = clkfreq();
                t1 = clkelap();
                dt = t1 - t0;
                a += dt;
                if (a >= f) {
                        fps = fc;
                        a = fc = 0;
                }
                fc++, i++;
        }
        winfree(win);
        winterm();
        return EXIT_SUCCESS;
}
