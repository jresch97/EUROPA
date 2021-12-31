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

#include "platform.h"

#ifdef PLATFORM_WIN32

#include "window_win32.h"

#include <Windows.h>

#define WIN_CLASS       "EUROPA WINDOW CLASS"
#define WIN_CLASS_STYLE (CS_HREDRAW | CS_VREDRAW | CS_OWNDC)
#define WIN_STYLE       (WS_OVERLAPPEDWINDOW | WS_VISIBLE)
#define WIN_EX_STYLE    (0)
#define WIN_ICON        (IDI_APPLICATION)
#define WIN_CURSOR      (IDC_ARROW)

typedef struct WIN32_BAKBUF {
        HBITMAP hBitmap;
        void* mem;
} WIN32_BAKBUF;

typedef struct WIN32_WINDAT {
        HWND hWnd;
        WIN32_BAKBUF bakbuf;
} WIN32_WINDAT;

static int     win32_regwc();
static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void winpoll()
{
        MSG msg;
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
                // need to handle quit message
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
        }
}

int win32_regwc()
{
        WNDCLASSEXA wc;
        static int reg = FALSE;
        if (reg) goto suc_ret;
        ZeroMemory(&wc, sizeof(WNDCLASSEXA));
        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.lpszClassName = WIN_CLASS;
        wc.style = WIN_CLASS_STYLE;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hIcon = LoadIcon(NULL, WIN_ICON);
        wc.hIconSm = LoadIcon(NULL, WIN_ICON);
        wc.hCursor = LoadCursor(NULL, WIN_CURSOR);
        wc.lpfnWndProc = WndProc;
        if (!RegisterClassExA(&wc)) goto err_ret;
        reg = TRUE;
suc_ret:
        return 1;
err_ret:
        return 0;
}

int win32_winalloc(WINDOW *win)
{
        WIN32_WINDAT *dat;
        RECT wr;
        if (!win32_regwc()) goto err_ret;
        if (!win)  goto err_ret;
        dat = malloc(sizeof(*dat));
        if (!dat) goto err_ret;
        wr.left = wr.top = 0;
        wr.right = win->w;
        wr.bottom = win->h;
        AdjustWindowRectEx(&wr, WIN_STYLE, FALSE, WIN_EX_STYLE);
        dat->hWnd = CreateWindowExA(WIN_EX_STYLE, WIN_CLASS,
                                    win->title, WIN_STYLE,
                                    win->x == WINPOSUND ? CW_USEDEFAULT : win->x,
                                    win->y == WINPOSUND ? CW_USEDEFAULT : win->y,
                                    wr.right - wr.left, wr.bottom - wr.top,
                                    NULL, NULL, GetModuleHandle(NULL), win);
        if (!dat->hWnd) goto err_free;
        win->dat = dat;
        return 1;
err_free:
        free(dat);
err_ret:
        return 0;
}

void win32_winfree(WINDOW* win)
{
        WIN32_WINDAT *dat;
        if (win && win->dat) {
                dat = win->dat;
                if (dat->hWnd) DestroyWindow(dat->hWnd);
                free(win->dat);
        }
}

// TODO: cache size and position in WndProc

void win32_winsize(WINDOW *win, int *w, int *h)
{
        WIN32_WINDAT *dat;
        RECT cr;
        if (win->dat) {
                dat = win->dat;
                if (dat->hWnd) {
                        GetClientRect(dat->hWnd, &cr);
                        *w = cr.right - cr.left;
                        *h = cr.bottom - cr.top;
                        return;
                }
        }
}

void win32_winpos(WINDOW *win, int *x, int *y)
{
        WIN32_WINDAT *dat;
        RECT cr;
        if (win->dat) {
                dat = win->dat;
                if (dat->hWnd) {
                        GetWindowRect(dat->hWnd, &cr);
                        *x = cr.left;
                        *y = cr.top;
                        return;
                }
        }
}

LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        PAINTSTRUCT ps;
        HDC hDC;
        switch (uMsg) {
        case WM_CLOSE:
                PostQuitMessage(0);
                break;
        case WM_SIZE:
                break;
        case WM_PAINT:
                hDC = BeginPaint(hWnd, &ps);
                FillRect(hDC, &ps.rcPaint, (HBRUSH)BLACK_BRUSH + 1);
                EndPaint(hWnd, &ps);
                break;
        default:
                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
        return 0;
}

#endif