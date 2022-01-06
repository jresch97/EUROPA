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
#include <platform.h>
#include <math.h>

#define WINTITLE   "EUROPA"
#define WINWIDTH   640
#define WINHEIGHT  480

int main(void)
{
        WINDOW *win;
        int     x, y, w, h, xx, yy, c, i, *px;

        /* Initialize default windowing system. */
        defwinsysinit();

        /* Allocates a software window. */
        win = winalloc(WINTITLE, WINPOSUND, WINPOSUND, WINWIDTH, WINHEIGHT);
        assert(win != NULL);
        //winallocsys(winsys, ...);

        printf("win=%p\n", (void*)win);

        i = 0;
        while (1) {
                winpos(win, &x, &y);   /* Retrieve window position. */
                winsz(win, &w, &h);    /* Retrieve window size. */
                px = (int*)winpx(win); /* Retrieve window pixels as (int*). */

                printf("x=%d,y=%d,w=%d,h=%d\n", x, y, w, h);

                /* Write to window back buffer memory. */
                for (yy = 0; yy < h; yy++) {
                        for (xx = 0; xx < w; xx++) {
                                /* INPUTS */
                                int xxx = xx + i;
                                int yyy = yy + i;
                                /* SEQUENCE */
                                c = (xxx ^ ((int)(sin(xxx + yyy))) ^ yyy);
                                /* ITERATOR & BLUE MASK */
                                c = (c + i) & 0xff;
                                /* MAP BLUE -> GRAYSCALE */
                                px[xx + yy * w] = c + c * 0x100 + c * 0x10000;
                        }
                }
                
                winswap(win);    /* Swap front/back buffers. */
                defwinsyspoll(); /* Poll events from default windowing system. */
                i++;
        }

        winfree(win); /* Free window. */

        /* Terminate default windowing system. */
        defwinsysterm();

        return EXIT_SUCCESS;
}