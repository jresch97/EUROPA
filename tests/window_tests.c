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
#include <window.h>
#include <platform.h>

#define WINTITLE   "EUROPA SW"
#define GLWINTITLE "EUROPA GL"
#define VKWINTITLE "EUROPA VK"
#define DXWINTITLE "EUROPA DX"
#define WINWIDTH   640
#define WINHEIGHT  480

int main(void)
{
        int x, y, w, h, *px;

        //WINSYS *winsys
        WINDOW   *win;
        GLWINDOW *glwin;
        VKWINDOW *vkwin;
        DXWINDOW *dxwin;

        // Initialize default windowing system.
        defwinsysinit();

        // Allocates a software window.
        win = winalloc(WINTITLE, WINPOSUND, WINPOSUND, WINWIDTH, WINHEIGHT);
        assert(win != NULL);
        //winallocsys(winsys, ...);

        printf("win=%p\n", (void*)win);

        // Allocates a hardware window with an OpenGL 3.0 context. (Or NULL on non-GL platforms.)
        //glwin = glwinalloc(GLWINTITLE, WINPOSUND, WINPOSUND, WINWIDTH, WINHEIGHT, 3, 0);
        //assert(glwin != NULL);
        //glwinallocsys(winsys, ...);

        // Allocates a hardware window with a Vulkan context. (Or NULL on non-VK platforms.)
        //vkwin = vkwinalloc(VKWINTITLE, WINPOSUND, WINPOSUND, WINWIDTH, WINHEIGHT);
        //assert(vkwin != NULL);
        //vkwinallocsys(winsys, ...);

        // Allocates a hardware window with a DirectX context. (Or NULL on non-DX platforms.)
        //dxwin = dxwinalloc(DXWINTITLE, WINPOSUND, WINPOSUND, WINWIDTH, WINHEIGHT);
        //#ifdef PLATFORM_WIN32
        //assert(dxwin != NULL);
        //#endif
        //dxwinallocsys(winsys, ...);

        // Free HW windows.
        //glwinfree(glwin);
        //vkwinfree(vkwin);
        //dxwinfree(dxwin);

        while (1) {
                winpull(win);           // Pulls state changes from OS to structure.

                winpos(win, &x, &y);    // Retrieve SW window position.
                winsize(win, &w, &h);   // Retrieve SW window size.
                //px = (int*)winpx(win);  // Retrieve pixels as int* from SW window HW surface.

                printf("x=%d,y=%d,w=%d,h=%d\n",
                        x, y, w, h);
                // or
                printf("win->x=%d,win->y=%d,win->w=%d,win->h=%d\n",
                        win->x, win->y, win->w, win->h);

                for (int y = 0; y < h; y++) {
                        for (int x = 0; x < w; x++); //*(px + x + y * w) = x & y;
                }

                winpush(win);           // Pushes state changes from structure to OS.
                winupdt(win);           // Equivialent to winpush() and winpull().
                winswap(win);           // Manually swap front/back buffers.
                
                defwinsyspoll();        // Poll events from default windowing system.
        }

        // Free SW window.
        winfree(win);

        // Terminate default windowing system.
        defwinsysterm();

        return EXIT_SUCCESS;
}