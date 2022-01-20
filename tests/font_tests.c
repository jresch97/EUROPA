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

#define MAP(c, a, s) ((((int)((c >> s) * (a / 255.0))) & 0xff) << s)
#define COL(c, a)    (MAP(c, a, 0) + MAP(c, a, 8) + MAP(c, a, 16))

int txtmeas(FONT *font, const char *s, int *w, int *h, int *xadv)
{
        GLYPH *g;
        int    i, l, m, tw, th, gw, gh, gxadv, txadv;
        tw = th = txadv = 0;
        for (i = 0, l = strlen(s), m = l - 1; i < l; i++) {
                g = fntglyph(font, s[i]);
                if (!g) return 0;
                gw = g->w, gh = g->h, gxadv = g->xadv;
                tw += gw + (i < m) ? gxadv : 0;
                if (gh > th) th = gh;
                txadv = gxadv - gw;
        }
        if (w)    *w = tw;
        if (h)    *h = th;
        if (xadv) *xadv = txadv;
        return 1;
}

int txtdraw(SURFACE *surf, FONT *font, const char *s, int x, int y, int c)
{
        GLYPH      *g;
        SURFACE    *gs;
        uint32_t   *px;
        uint8_t    *gpx;
        const char *ss;
        int         ch, a, sw, sh;
        int         tx, ty, th, h, xadv;
        int         gw, gh, gx, gy, go;
        px   = (uint32_t*)surf->px;
        ss   = s;
        h    = th = 0;
        xadv = x;
        while ((ch = *ss++)) {
                g = fntglyph(font, ch);
                if (!g) return 0;
                h = g->h;
                if (h > th) th = h;
        }
        ss = s, sw = surf->w, sh = surf->h;
        while ((ch = *ss++)) {
                g = fntglyph(font, ch);
                if (!g) return 0;
                gs = g->surf;
                if (!gs) goto adv;
                gpx = (uint8_t*)gs->px;
                gw  = g->w, gh = g->h, go = th - gh;
                for (gy = 0; gy < gh; gy++) {
                        ty = y + gy + go;
                        if (ty < 0 || ty >= sh) continue;
                        for (gx = 0; gx < gw; gx++) {
                                tx = gx + xadv;
                                if (tx < 0 || tx >= sw) continue;
                                a = gpx[gx + gy * gw];
                                if (a == 255) px[tx + ty * sw] = COL(c, a);
                        }
                }
        adv:    xadv += g->xadv;
        }
        return 1;
}

int main(int argc, char *argv[])
{
        WINDOW   *win;
        SURFACE  *surf;
        FONT     *font;
        int       c, x, y, z, j, k, l, w, h, sw, sh;
        unsigned  i, fc, fps, tfps;
        uint32_t *px;
        uint64_t  t0, t1, dt, f, g, a;
        wininit();
        fntinit();
        win  = winalloc("EUROPA FONT TESTS", WINCTR, WINCTR, 640, 480, 32);
        surf = winsurf(win);
        font = fntload(PATH, 72);
        txtmeas(font, MSG, &w, &h, NULL);
        i    = z = fc = 0, a = 0, fps = UINT_MAX, l = 16 + h;
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
                px = (uint32_t*)surf->px;
                sw = surf->w, sh = surf->h;
                for (y = 0; y < sh; y++, z = y * sw) {
                        for (x = 0; x < sw; x++) {
                                c = ((x ^ y) + i) & 0xff;
                                px[x + z] = c + (c << 8) + (c << 16);
                        }
                }
                for (j = 16; j < sw; j += w) {
                        for (k = 16; k < sh; k += l) {
                                txtdraw(surf, font, MSG, j, k, rand());
                        }
                }
                winswap(win);
                winpoll();
                if (fps < UINT_MAX) {
                        printf("fps=%u\n", fps);
                        fps = UINT_MAX;
                }
                if (tfps > 0) {
                        g = f / tfps;
                        t1 = clkelap();
                        dt = t1 - t0;
                        if (dt < g) clkslep(g - dt);
                }
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
