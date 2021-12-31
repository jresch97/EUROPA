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

#include "window.h"

#define WINTITLE  "EUROPA"
#define WINWIDTH  640
#define WINHEIGHT 480

int main(void)
{
        int* px;
        WINDOW* win;
        const WINDRV* drv;
        int x, y, w, h;
        win = winalloc(WINTITLE, WINPOSUND, WINPOSUND, WINWIDTH, WINHEIGHT);
        assert(win != NULL);
        drv = windrv(win);
        assert(drv != NULL);
        while (1) {
                winpos(win, &x, &y);
                winsize(win, &w, &h);
                //px = (int*)winpx(win);
                for (int y = 0; y < h; y++) {
                        for (int x = 0; x < w; x++); //*(px + x + y * w) = x & y;
                }
                winpoll();
        }
        winfree(win);
        return 0;
}