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

#ifndef EUROPA_WINSYS_H
#define EUROPA_WINSYS_H

typedef struct WINSYS    WINSYS;
typedef struct WINDOW    WINDOW;
typedef struct GLWINDOW  GLWINDOW;
typedef struct VKWINDOW  VKWINDOW;
typedef struct DXWINDOW  DXWINDOW;
typedef struct HWSURFACE HWSURFACE;

struct WINSYS {
        const char *name;
        const struct {
                int  (*winsysinitcb)  ();
                void (*winsystermcb)  ();
                void (*winsyspollcb)  ();
                int  (*winalloccb)    (WINDOW *win);
                void (*winfreecb)     (WINDOW *win);
                void (*winposcb)      (WINDOW *win, int *x, int *y);
                void (*winsizecb)     (WINDOW *win, int *w, int *h);
                int  (*hwsurfalloccb) (HWSURFACE *surf);
                void (*hwsurffreecb)  (HWSURFACE *surf);
        } drv;
};

const WINSYS* winsys     (const char *name);
int           winsysinit (const WINSYS *sys);
void          winsysterm (const WINSYS *sys);
void          winsyspoll (const WINSYS *sys);

const WINSYS* defwinsys     ();
int           defwinsysinit ();
void          defwinsysterm ();
void          defwinsyspoll ();

#endif