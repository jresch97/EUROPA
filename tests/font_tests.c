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
        WINDOW   *win;
        SURFACE  *surf;
        FONT     *font;
        long long s, e, a;
        int       x, y, i, c, fps, fc, tfps;
        wininit();
        fontinit();
        win  = winalloc(WINC, WINX, WINY, WINW, WINH, WIND, NULL);
        surf = winsurf (win);
        font = fontload(FPTH, 12);
        i    = fc = 0, a = 0, fps = -1;
        tfps = argc > 1 ? atoi(argv[1]) : TFPS;
        printf("winsysd()->name=\"%s\"\n", winsysd()->name);
        printf("fontsysd()->name=\"%s\"\n", fontsysd()->name);
        printf("font->path=\"%s\"\n", font->path);
        printf("font->family=\"%s\"\n", font->family);
        printf("font->style=\"%s\"\n", font->style);
        printf("font->pt=%d\n", font->pt);
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
        fontfree(font);
        winfree(win);
        fontterm();
        winterm();
        return EXIT_SUCCESS;
}
