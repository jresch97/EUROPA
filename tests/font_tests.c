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
#define MSG  "Hello, World! "

#define MAP(c, a, s) (((int)(((c >> s) & 0xff) * (a / 255.0f)) & 0xff) << s)
#define COL(c, a)    (MAP(c, a, 0) + MAP(c, a, 8) + MAP(c, a, 16))

void txtmeas(FONT *font, const char *s, int *w, int *h, int *xadv)
{
        GLYPH *g;
        int    i, l, tw, th, txadv;
        tw = th = txadv = 0;
        for (i = 0, l = strlen(s); i < l; i++) {
                g = fntglyph(font, s[i]);
                if (!g) continue;
                if (g->h > th) th = g->h;
                tw += g->w + (i < (l - 1)) ? g->xadv : 0;
                if (i == (l - 1)) txadv = g->xadv - g->w;
        }
        if (w)    *w = tw;
        if (h)    *h = th;
        if (xadv) *xadv = txadv;
}

void txtdraw(SURFACE *surf, FONT *font, const char *s, int x, int y, int c)
{
        GLYPH    *g;
        uint32_t *px;
        uint8_t  *gpx;
        int       cc, tx, ty, gx, gy, i, l, xadv, h;
        px   = (uint32_t*)surf->px;
        xadv = h = 0;
        txtmeas(font, s, NULL, &h, NULL);
        for (i = 0, l = strlen(s); i < l; i++) {
                g = fntglyph(font, s[i]);
                if (!g) continue;
                if (!g->surf) goto adv;
                gpx = (uint8_t*)g->surf->px;
                for (gy = 0; gy < g->surf->h; gy++) {
                        for (gx = 0; gx < g->surf->w; gx++) {
                                tx = x + gx + xadv;
                                ty = y + gy + (h - g->h);
                                cc = gpx[gx + gy * g->surf->w];
                                if (tx >= 0 && ty >= 0 && tx < surf->w && ty < surf->h) {
                                        px[tx + ty * surf->w] = COL(c, cc);
                                }
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
        int      j, k, l, m, w, h, xadv;
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
        txtmeas(font, MSG, &w, &h, &xadv);
        while (winopen(win)) {
                f  = clkfreq();
                t0 = clkelap();
                surfclr32(surf, 0);
                for (l = 16, m = 0; l < win->w; l += w, m++) {
                        for (j = 16, k = 0; j < win->h; j += 16 + h, k++) {
                                txtdraw(surf, font, MSG, l, j, rand());
                        }
                }
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
