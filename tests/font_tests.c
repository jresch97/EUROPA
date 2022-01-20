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
#include <font.h>

#define PATH "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
#define MSG  "Hello World"

#define MAP(c, a, s) ((((int)(((c >> s) & 0xff) * (a / 255.0f)) & 0xff) << s))
#define COL(c, a)    (MAP(c, a, 0) + MAP(c, a, 8) + MAP(c, a, 16))

void txtdraw(SURFACE *surf, FONT *font, const char *s, int x, int y, int c)
{
        GLYPH    *g;
        uint32_t *px;
        uint8_t  *gpx;
        int       cc, xx, yy, gx, gy, i, l, xadv;
        px   = (uint32_t*)surf->px;
        xadv = 0;
        for (i = 0, l = strlen(s); i < l; i++) {
                g = fntglyph(font, s[i]);
                if (!g) continue;
                if (!g->surf) goto adv;
                gpx = (uint8_t*)g->surf->px;
                for (gy = 0; gy < g->surf->h; gy++) {
                        for (gx = 0; gx < g->surf->w; gx++) {
                                xx = x + gx + xadv;
                                yy = y + gy + (font->pt - g->surf->h);
                                cc = gpx[gx + gy * g->surf->w];
                                px[xx + yy * surf->w] = COL(c, cc);
                        }
                }
        adv:    xadv += g->xadv;
        }       
}

int main(int argc, char *argv[])
{
        WINDOW  *win;
        SURFACE *surf;
        FONT    *font;
        unsigned i, fc, fps, tfps;
        uint64_t t0, t1, dt, f, a;
        wininit();
        fntinit();
        win  = winalloc("EUROPA FONT TESTS", WINCTR, WINCTR, 640, 480, 32);
        surf = winsurf (win);
        font = fntload(PATH, 72);
        i    = fc = 0, a = 0, fps = UINT_MAX;
        tfps = argc > 1 ? atoi(argv[1]) : 60;
        printf("winsysd()->name=\"%s\"\n", winsysd()->name);
        printf("fntsysd()->name=\"%s\"\n", fntsysd()->name);
        printf("font->path=\"%s\"\n", font->path);
        printf("font->family=\"%s\"\n", font->family);
        printf("font->style=\"%s\"\n", font->style);
        printf("font->pt=%d\n", font->pt);
        while (winopen(win)) {
                f  = clkfreq();
                t0 = clkelap();
                surfclr32(surf, 0);
                txtdraw(surf, font, MSG, 16, 16, rand());
                winswap(win);
                winpoll();
                if (fps < UINT_MAX) {
                        printf("fps=%u\n", fps);
                        fps = UINT_MAX;
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
        fntfree(font);
        winfree(win);
        fntterm();
        winterm();
        return EXIT_SUCCESS;
}
