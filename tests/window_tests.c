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

#define WIN1TTL "EUROPA LINES"
#define WIN2TTL "EUROPA CIRCLES"
#define WINW    320
#define WINH    240
#define WIND    32
#define WINSCL  3

static double lerp(double a, double b, double t)
{
        return a + (b - a) * t;
}

int main(void)
{
        const WINSYS *sys;
        WINDOW       *win1, *win2;
        SURFACE      *surf1, *surf2;
        WINOPTS       winopts;
        int           a, i, c, x, y, px, py, cx, cy, dx, dy;
        int           mx = INT_MAX, my = INT_MAX, r, f;
        a = f = 0;
        sys   = winsysd();
        wininit(sys);
        winopts.sys = sys;
        winopts.scale = WINSCL;
        win1  = winalloc(WIN1TTL, WINCTR, WINCTR, WINW, WINH, WIND, &winopts);
        win2  = winalloc(WIN2TTL, WINCTR, WINCTR, WINW, WINH, WIND, &winopts);
        surf1 = winsurf(win1), surf2 = winsurf(win2);
        while (win1 && winopen(win1) || win2 && winopen(win2)) {
                if (win1 && winopen(win1)) {
                        surfclr(surf1, 0);
                        for (i = 0; i < 100; i++) {
                                c = rand() + (rand() << 8) + (rand() << 16);
                                surfln(winsurf(win1),
                                       rand() % surf1->w, rand() % surf1->h,
                                       rand() % surf1->w, rand() % surf1->h,
                                       c);
                        }
                        surfln(surf1, 0, 0, surf1->w - 1, surf1->h - 1,
                                0xffffffff);
                        surfln(surf1, surf1->w - 1, 0, 0, surf1->h - 1,
                                0xffffffff);
                        winswap(win1);
                }
                else if (win1) {
                        winfree(win1);
                        win1 = NULL;
                }
                if (win2 && winopen(win2)) {
                        surfclr(surf2, 0);
                        px = 0, py = 0;
                        r = (surf2->h / 2) - 16;
                        cx = surf2->w / 2;
                        cy = surf2->h / 2;
                        for (i = 0; i <= 360; i += 2) {
                                x = (int)(cos((double)(i + a)) * r);
                                y = (int)(sin((double)(i + a)) * r);
                                dx = x + cx;
                                dy = y + cy;
                                c  = rand() + (rand() << 8) + (rand() << 16);
                                if (f && dx != 0 && dy != 0 &&
                                         px != 0 && py != 0) {
                                        surfln(surf2, dx, dy, px, py, c);
                                }
                                px = dx, py = dy;
                                f = 1;
                        }
                        a += 32;
                        winswap(win2);
                }
                else if (win2) {
                        winfree(win2);
                        win2 = NULL;
                }
                winpoll(sys);
        }
        winfree(win1);
        winfree(win2);
        winterm(sys);
        return EXIT_SUCCESS;
}