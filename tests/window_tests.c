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
#include <math.h>
#include <window.h>

#define WINTITLE  "EUROPA"
#define WINWIDTH  640
#define WINHEIGHT 480

int main(void)
{
        const WINSYS *sys;
        WINDOW       *win;
        WINOPTS       opts;
        int           c, i = 0, x, y, w, h, xx, yy, xi, yi, *px;
        sys = winsysd();
        wininit(sys);
        opts.sys   = sys;
        opts.scale = 2.0;
        win = winalloc(WINTITLE, WINUDF, WINUDF, WINWIDTH, WINHEIGHT, &opts);
        while (winopen(win)) {
                winxy(win, &x, &y);
                winsz(win, &w, &h);
                px = (int*)winpx(win);
                printf("x=%d,y=%d,w=%d,h=%d,px=%p\n", x, y, w, h, (void*)px);
                for (yy = 0; yy < win->surf->h; yy++) {
                        for (xx = 0; xx < win->surf->w; xx++) {
                                /*xi = xx + i; yi = yy + i;
                                c = (xi ^ ((int)(sin(xi + yi))) ^ yi);
                                c = (c + i) & 0xff;*/
                                c = rand() + rand() * 0x100 + rand() * 0x10000;
                                px[xx + yy * win->surf->w] = c;
                        }
                }
                i++;
                winswap(win);
                winpoll(sys);
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}