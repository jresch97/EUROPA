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
#include <clock.h>
#include <window.h>
#include <font.h>

#define WINC "EUROPA FONT TESTS"
#define WINX WINCTR
#define WINY WINCTR
#define WINW 640
#define WINH 480
#define WIND 32
#define TFPS 60
#define FPTH "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"

int main(int argc, char *argv[])
{
        const WINSYS  *winsys;
        const FONTSYS *fontsys;
        WINDOW        *win;
        SURFACE       *surf;
        FONT          *font;
        int            x, y, i, c, fps, fc, tfps;
        long long      s, e, a;
        winsys  = winsysd();
        fontsys = fontsysd();
        wininit(winsys);
        fontinit(fontsys);
        win  = winalloc(WINC, WINX, WINY, WINW, WINH, WIND, NULL);
        surf = winsurf(win);
        font = fontload(FPTH, 12);
        i    = fc = 0, a = 0, fps = -1;
        tfps = argc > 1 ? atoi(argv[1]) : TFPS;
        printf("winsys->name=\"%s\"\n", winsys->name);
        printf("fontsys->name=\"%s\"\n", fontsys->name);
        printf("font->family=\"%s\"\nfont->style=\"%s\"\nfont->pt=%d\n",
               font->family, font->style, font->pt);
        while (winopen(win)) {
                s = clkelapt();
                for (y = 0; y < surf->h; y++) {
                        for (x = 0; x < surf->w; x++) {
                                c = ((x + i) ^ (y + i)) & 0xff;
                                ((int*)surf->px)[x + y * surf->w] = c;
                        }
                }
                winswap(win);
                winpoll(winsys);
                if (fps >= 0) {
                        printf("fps=%d\n", fps);
                        fps = -1;
                }
                e = clkelapt();
                if (tfps > 0) clkslept((clkfreq() / tfps) - (e - s));
                e = clkelapt();
                a += (e - s);
                if (a >= clkfreq()) {
                        fps = fc;
                        a   = fc = 0;
                }
                fc++, i++;
        }
        winfree(win);
        winterm(winsys);
        return EXIT_SUCCESS;
}
