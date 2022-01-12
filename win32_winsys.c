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

#include "win32_winsys.h"

#include <assert.h>
#include <Windows.h>

#include "pxfmt.h"
#include "window.h"
#include "surface.h"

#define WC         "EUROPA WINDOW CLASS"
#define WCSTYLE    (CS_HREDRAW | CS_VREDRAW | CS_OWNDC)
#define WINSTYLE   (WS_OVERLAPPEDWINDOW | WS_VISIBLE)
#define WINEXSTYLE (0)
#define WINICON    (IDI_APPLICATION)
#define WINCURSOR  (IDC_ARROW)

#define WINPTR(hWnd) ((WINDOW*)GetWindowLongPtr(hWnd, GWLP_USERDATA))
#define WINDAT(win)  ((WIN32_WINDAT*)win->dat)

typedef struct WIN32_DAT {
        HINSTANCE hInstance;
} WIN32_DAT;

typedef struct WIN32_WINDAT {
        HWND        hWnd;
        HDC         hdc;
        BITMAPINFO  inf;
        HBITMAP     hBitmap;
        void       *px;
} WIN32_WINDAT;

static int     win32_regwc   ();
static void    win32_resxy   (WINDOW *win);
static void    win32_pxfmt   (PXFMT *pxfmt);
static HBITMAP win32_diballoc(HDC hdc, BITMAPINFO *inf, int w, int h,
                              PXFMT *pxfmt, void **px);
static LRESULT win32_wndproc (HWND hWnd, UINT uMsg,
                              WPARAM wParam, LPARAM lParam);

static WIN32_DAT d;

int win32_regwc()
{
        static int  r = FALSE;
        WNDCLASSEXA wc;
        if (r) return 1;
        ZeroMemory(&wc, sizeof(wc));
        wc.cbSize        = sizeof(wc);
        wc.lpszClassName = WC;
        wc.style         = WCSTYLE;
        wc.hInstance     = d.hInstance;
        wc.hIcon         = LoadIcon(NULL, WINICON);
        wc.hIconSm       = LoadIcon(NULL, WINICON);
        wc.hCursor       = LoadCursor(NULL, WINCURSOR);
        wc.hbrBackground = (HBRUSH)BLACK_BRUSH + 1;
        wc.lpfnWndProc   = win32_wndproc;
        if (!RegisterClassExA(&wc)) return 0;
        r = TRUE;
        return 1;
}

static void win32_resxy(WINDOW *win)
{
        int s;
        if      (win->x == WINDEF) win->x = CW_USEDEFAULT;
        else if (win->x == WINCTR) {
                s = GetSystemMetrics(SM_CXSCREEN);
                win->x = win->x + ((s - win->w) / 2);
        }
        if      (win->y == WINDEF) win->y = CW_USEDEFAULT;
        else if (win->y == WINCTR) {
                s = GetSystemMetrics(SM_CYSCREEN);
                win->y = win->y + ((s - win->h) / 2);
        }
}

void win32_pxfmt(PXFMT *pxfmt)
{
        *pxfmt = RGB32;
}

HBITMAP win32_diballoc(HDC hdc, BITMAPINFO *inf, int w, int h, PXFMT *pxfmt, void **px)
{
        ZeroMemory(inf, sizeof(*inf));
        inf->bmiHeader.biSize        = sizeof(*inf);
        inf->bmiHeader.biWidth       = w;
        inf->bmiHeader.biHeight      = -h;
        inf->bmiHeader.biPlanes      = 1;
        inf->bmiHeader.biBitCount    = pxfmt->bipp;
        inf->bmiHeader.biCompression = BI_RGB;
        *px = VirtualAlloc(NULL, w * h * pxfmt->bypp,
                           MEM_COMMIT, PAGE_READWRITE);
        if (!*px) return NULL;
        return CreateDIBitmap(hdc, &inf->bmiHeader, CBM_INIT,
                              *px, inf, DIB_RGB_COLORS);
}

int win32_init()
{
        if (d.hInstance)    return 1;
        d.hInstance = GetModuleHandle(NULL);
        if (!d.hInstance)   return 0;
        if (!win32_regwc()) goto err;
        return 1;
err:    d.hInstance = NULL;
        return 0;
}

void win32_term()
{
        assert(d.hInstance != NULL);
        d.hInstance = NULL;
}

void win32_poll()
{
        MSG msg;
        assert(d.hInstance != NULL);
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
        }
}

int win32_winalloc(WINDOW *win, PXFMT *pxfmt, void **px)
{
        WIN32_WINDAT *wd;
        RECT          r;
        assert(win != NULL);
        assert(pxfmt != NULL);
        assert(px != NULL);
        assert(d.hInstance != NULL);
        if (!win || !d.hInstance || !pxfmt || !px) return 0;
        wd = malloc(sizeof(*wd));
        if (!wd) return 0;
        win32_resxy(win);
        r.left   = r.top = 0;
        r.right  = win->w;
        r.bottom = win->h;
        AdjustWindowRectEx(&r, WINSTYLE, FALSE, WINEXSTYLE);
        r.right  = r.right  - r.left;
        r.bottom = r.bottom - r.top;
        wd->hWnd = CreateWindowExA(WINEXSTYLE, WC, win->title, WINSTYLE,
                                   win->x, win->y, r.right, r.bottom,
                                   NULL, NULL, d.hInstance, win);
        wd->hdc  = GetDC(wd->hWnd);
        if (!wd->hWnd) goto errfwd;
        win32_pxfmt(pxfmt);
        wd->hBitmap = win32_diballoc(wd->hdc, &wd->inf,
                                     (int)(win->w / win->scale),
                                     (int)(win->h / win->scale),
                                     pxfmt, &wd->px);
        if (!wd->hBitmap) goto errdw;
        *px = wd->px;
        win->dat = wd;
        return 1;
errdw:  DestroyWindow(wd->hWnd);
errfwd: free(wd);
        return 0;
}

void win32_winfree(WINDOW *win)
{
        WIN32_WINDAT *wd;
        assert(win != NULL);
        assert(d.hInstance != NULL);
        if (win) {
                wd = WINDAT(win);
                if (wd->hWnd)    DestroyWindow(wd->hWnd);
                if (wd->hBitmap) DeleteObject(wd->hBitmap);
                if (wd->px)      VirtualFree(wd->px, 0, MEM_RELEASE);
                free(wd);
        }
}

void win32_winrettl(WINDOW *win, const char *title)
{
        assert(win != NULL);
        assert(d.hInstance != NULL);
}

void win32_winmov(WINDOW* win, int x, int y)
{
        assert(win != NULL);
        assert(d.hInstance != NULL);
}

void win32_winresz(WINDOW* win, int w, int h)
{
        assert(win != NULL);
        assert(d.hInstance != NULL);
}

void win32_winswap(WINDOW *win)
{
        WIN32_WINDAT* wd;
        assert(win != NULL);
        assert(d.hInstance != NULL);
        wd = WINDAT(win);
        StretchDIBits(wd->hdc,
                      0, 0,
                      win->w, win->h,
                      0, 0,
                      win->surf->w, win->surf->h,
                      wd->px,
                      &wd->inf,
                      DIB_RGB_COLORS,
                      SRCCOPY);
}

LRESULT win32_wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        WINDOW       *win;
        WIN32_WINDAT *wd;
        LONG_PTR      wlp;
        switch (uMsg) {
        case WM_CREATE:
                wlp = (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams;
                SetWindowLongPtr(hWnd, GWLP_USERDATA, wlp);
                break;
        case WM_CLOSE:
                win = WINPTR(hWnd);
                if (!win) return 0;
                win->open = 0;
                break;
        case WM_MOVE:
                win = WINPTR(hWnd);
                if (!win) return 0;
                win->x = LOWORD(lParam);
                win->y = HIWORD(lParam);
                break;
        case WM_SIZE:
                win = WINPTR(hWnd);
                if (!win || !win->dat) return 0;
                wd = WINDAT(win);
                win->w = LOWORD(lParam);
                win->h = HIWORD(lParam);
                if (wd->hBitmap) DeleteObject(wd->hBitmap);
                if (wd->px)      VirtualFree(wd->px, 0, MEM_RELEASE);
                win->surf->w = (int)(win->w / win->scale);
                win->surf->h = (int)(win->h / win->scale);
                wd->hBitmap = win32_diballoc(wd->hdc, &wd->inf,
                                             win->surf->w, win->surf->h,
                                             &win->surf->pxfmt, &wd->px);
                win->surf->px = wd->px;
                break;
        default: 
                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
        return 0;
}

#endif