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
#include <window.h>

#define WINTITLE  "EUROPA"
#define WINWIDTH  64
#define WINHEIGHT 48

int main(void)
{
        const WINSYS *sys;
        WINDOW       *win;
        WINOPTS       opts;
        int           c[3], i;
        c[0] = R24, c[1] = G24, c[2] = B24;
        sys = winsysd();
        wininit(sys);
        opts.sys   = sys;
        opts.scale = 10.0;
        win = winalloc(WINTITLE, WINDEF, WINDEF, WINWIDTH, WINHEIGHT, &opts);
        while (winopen(win)) {
                surfclr(winsurf(win), 0);
                for (i = 0; i < 10; i++) {
                        surfln(winsurf(win),
                               rand() % winsurf(win)->w,
                               rand() % winsurf(win)->h,
                               rand() % winsurf(win)->w,
                               rand() % winsurf(win)->h,
                               rand() + rand() * 0x100 + rand() * 0x10000);
                }
                surfln(winsurf(win),
                       0, 0,
                       winsurf(win)->w - 1,
                       winsurf(win)->h - 1,
                       0xffffffff);
                surfln(winsurf(win),
                       winsurf(win)->w - 1,
                       0, 0,
                       winsurf(win)->h - 1,
                       0xffffffff);
                winswap(win);
                winpoll(sys);
        }
        winfree(win);
        winterm(sys);
        return EXIT_SUCCESS;
}