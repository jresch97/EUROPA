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
#include <window.h>
#include <math.h>

#define WINTITLE   "EUROPA"
#define WINWIDTH   640
#define WINHEIGHT  480

int main(void)
{
        WINDOW *win;
        int     c, i, x, y, w, h, nx, ny, nw, nh, xx, yy, xxx, yyy, *px, *npx;
        i = x = y = w = h = 0; px = NULL;
        defwinsysinit();
        win = winalloc(WINTITLE, WINPOSUND, WINPOSUND, WINWIDTH, WINHEIGHT);
        while (1) {
                winpos(win, &nx, &ny);
                winsz (win, &nw, &nh);
                npx = (int*)winpx(win);
                if (x != nx || y != ny || w != nw || h != nh || px != npx) {
                        x = nx; y = ny; w = nw; h = nh; px = npx;
                        printf("x=%d,y=%d,w=%d,h=%d,px=%p\n",
                               x, y, w, h, (void*)npx);
                }
                for (yy = 0; yy < h; yy++) {
                        for (xx = 0; xx < w; xx++) {
                                xxx = xx + i; yyy = yy + i;
                                c = (xxx ^ ((int)(sin(xxx + yyy))) ^ yyy);
                                c = (c + i) & 0xff;
                                px[xx + yy * w] = c + c * 0x100 + c * 0x10000;
                        }
                }
                i++;
                winswap(win);
                defwinsyspoll();
        }
        winfree(win);
        defwinsysterm();
        return EXIT_SUCCESS;
}
