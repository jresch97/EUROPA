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

#define MAP(c, a, s) ((int)(((c >> s) & 0xff) * C[a]) << s)
#define COL(c, a)    (MAP(c, a, 0) + MAP(c, a, 8) + MAP(c, a, 16))

static float C[256];

int txtmeas(FONT *font, const char *s, int *w, int *h)
{
        GLYPH *g;
        int    i, l, tw, th, gh;
        for (i = tw = th = 0, l = strlen(s); i < l; i++) {
                g = fntglyph(font, s[i]);
                if (!g) return 0;
                gh = g->h;
                tw += g->w + (i < (l - 1)) ? g->xadv : 0;
                if (gh > th) th = gh;
        }
        if (w) *w = tw;
        if (h) *h = th;
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
        ss = s, sw = surf->w, sh = surf->h, y += th;
        while ((ch = *ss++)) {
                g = fntglyph(font, ch);
                if (!g) return 0;
                gs = g->surf;
                if (!gs) goto adv;
                gpx = (uint8_t*)gs->px;
                gw  = g->w, gh = g->h, go = y - gh;
                for (gy = 0; gy < gh; gy++) {
                        ty = gy + go;
                        if (ty < 0 || ty >= sh) continue;
                        for (gx = 0; gx < gw; gx++) {
                                tx = gx + xadv;
                                if (tx < 0 || tx >= sw) continue;
                                a = gpx[gx + gy * gw];
                                if (a > 0) px[tx + ty * sw] = COL(c, a);
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
        int       z, j, k, l, tw, th;
        unsigned  i, fc, fps, tfps;
        uint64_t  t0, t1, dt, f, g, a;
        wininit();
        fntinit();
        win  = winalloc("EUROPA FONT TESTS", WINCTR, WINCTR, 640, 480, 32);
        surf = winsurf(win);
        font = fntload(PATH, 24);
        txtmeas(font, MSG, &tw, &th);
        for (j = 0; j < 256; j++) C[j] = j / 255.0;
        i = z = fc = 0, a = 0, fps = UINT_MAX, l = th + 16;
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
                memset(surf->px, 0xff000000, surf->bytes);
                for (j = 0; j < surf->w; j += tw) {
                        for (k = 0; k < surf->h; k += l) {
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
                        g  = f / tfps;
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
