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

#include "platform.h"

#ifdef PLATFORM_WIN32

#include <assert.h>
#include <Windows.h>

#include "winsys32.h"
#include "window.h"

#define WC32       "EUROPA WINDOW CLASS"
#define WCS32      (CS_HREDRAW | CS_VREDRAW | CS_OWNDC)
#define WS32       (WS_OVERLAPPEDWINDOW | WS_VISIBLE)
#define WEXS32     (0)
#define WICON32    (IDI_APPLICATION)
#define WCURSOR32  (IDC_ARROW)
#define WBG32      ((HBRUSH)BLACK_BRUSH + 1)
#define WP32(hWnd) ((WINDOW*)GetWindowLongPtr(hWnd, GWLP_USERDATA))
#define WD32(win)  ((WINDAT32*)win->dat)
#define SD32(surf) ((SURFDAT32*)surf->dat);
#define ASSERT32() (assert(d.hInstance != NULL))

typedef struct DAT32 {
        HINSTANCE hInstance;
} DAT32;

typedef struct WINDAT32 {
        HWND hWnd;
        HDC  hdc;
} WINDAT32;

typedef struct SURFDAT32 {
        BITMAPINFO bmi;
        HBITMAP    hBitmap;
} SURFDAT32;

static int     winregwc32();
static void    winresxy32(WINDOW *win);
static int     diballoc32(SURFACE *surf);
static void    dibfree32 (SURFACE *surf);
static LRESULT wndproc32 (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static DAT32 d;

int winregwc32()
{
        static int  r = FALSE;
        WNDCLASSEXA wc;
        if (r) return 1;
        ZeroMemory(&wc, sizeof(wc));
        wc.cbSize        = sizeof(wc);
        wc.lpszClassName = WC32;
        wc.style         = WCS32;
        wc.hInstance     = d.hInstance;
        wc.hIcon         = LoadIcon(NULL, WICON32);
        wc.hIconSm       = LoadIcon(NULL, WICON32);
        wc.hCursor       = LoadCursor(NULL, WCURSOR32);
        wc.hbrBackground = WBG32;
        wc.lpfnWndProc   = &wndproc32;
        if (!RegisterClassExA(&wc)) return 0;
        r = TRUE;
        return 1;
}

static void winresxy32(WINDOW *win)
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

int diballoc32(SURFACE *surf)
{
        SURFDAT32 *sd;
        ASSERT32();
        assert(surf != NULL);
        sd = SD32(surf);
        ZeroMemory(&sd->bmi, sizeof(BITMAPINFO));
        sd->bmi.bmiHeader.biSize        = sizeof(BITMAPINFO);
        sd->bmi.bmiHeader.biWidth       =  surf->w;
        sd->bmi.bmiHeader.biHeight      = -surf->h;
        sd->bmi.bmiHeader.biPlanes      = 1;
        sd->bmi.bmiHeader.biBitCount    = surf->pxfmt.bipp;
        sd->bmi.bmiHeader.biCompression = BI_RGB;
        surf->bytes = surf->w * surf->h * surf->pxfmt.bypp;
        surf->px = VirtualAlloc(NULL, surf->bytes, MEM_COMMIT, PAGE_READWRITE);
        if (!surf->px) return 0;
        sd->hBitmap = CreateDIBitmap(GetDC(NULL), &sd->bmi.bmiHeader, CBM_INIT,
                                     surf->px, &sd->bmi, DIB_RGB_COLORS);
        if (!sd->hBitmap) goto errfpx;
        return 1;
errfpx: if (surf->px) VirtualFree(surf->px, 0, MEM_RELEASE);
        return 0;
}

void dibfree32(SURFACE *surf)
{
        SURFDAT32* sd;
        ASSERT32();
        assert(surf != NULL);
        if (surf && surf->dat) {
                sd = SD32(surf);
                if (sd->hBitmap) DeleteObject(sd->hBitmap);
                if (surf->px)    VirtualFree(surf->px, 0, MEM_RELEASE);
        }
}

int wininit32()
{
        if (d.hInstance)   return 1;
        d.hInstance = GetModuleHandle(NULL);
        if (!d.hInstance)  return 0;
        if (!winregwc32()) goto err;
        return 1;
err:    d.hInstance = NULL;
        return 0;
}

void winterm32()
{
        ASSERT32();
        d.hInstance = NULL;
}

void winpoll32()
{
        MSG msg;
        ASSERT32();
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
        }
}

int winalloc32(WINDOW *win)
{
        WINDAT32 *wd;
        RECT      r;
        ASSERT32();
        assert(win != NULL);
        wd = malloc(sizeof(*wd));
        if (!wd) return 0;
        winresxy32(win);
        r.left   = r.top = 0;
        r.right  = win->w;
        r.bottom = win->h;
        if (!AdjustWindowRectEx(&r, WS32, FALSE, WEXS32)) goto errfwd;
        r.right  = r.right  - r.left;
        r.bottom = r.bottom - r.top;
        wd->hWnd = CreateWindowExA(WEXS32, WC32, win->cap, WS32,
                                   win->x, win->y, r.right, r.bottom,
                                   NULL, NULL, d.hInstance, win);
        wd->hdc  = GetDC(wd->hWnd);
        if (!wd->hWnd) goto errfwd;
        win->dat = wd;
        return 1;
errfwd: free(wd);
        return 0;
}

void winfree32(WINDOW *win)
{
        WINDAT32 *wd;
        ASSERT32();
        assert(win != NULL);
        if (win) {
                wd = WD32(win);
                if (wd->hWnd) DestroyWindow(wd->hWnd);
                free(wd);
        }
}

void winrecap32(WINDOW *win, const char *title)
{
        ASSERT32();
        assert(win != NULL);
}

void winmove32(WINDOW* win, int x, int y)
{
        ASSERT32();
        assert(win != NULL);
}

void winresize32(WINDOW* win, int w, int h)
{
        ASSERT32();
        assert(win != NULL);
}

void winswap32(WINDOW *win)
{
        SURFACE   *surf;
        WINDAT32  *wd;
        SURFDAT32 *sd;
        ASSERT32();
        assert(win != NULL);
        surf = win->surf;
        wd   = WD32(win);
        sd   = SD32(win->surf);
        StretchDIBits(wd->hdc, 0, 0, win->w, win->h,
                               0, 0, surf->w, surf->h,
                               surf->px, &sd->bmi,
                               DIB_RGB_COLORS, SRCCOPY);
}

int surfalloc32(SURFACE *surf, PXFMT *pxfmt)
{
        SURFDAT32 *sd;
        ASSERT32();
        assert(surf != NULL);
        sd = malloc(sizeof(*sd));
        if (!sd) goto erret;
        surf->dat = sd;
        surf->bytes = surf->bytes * RGBA32.bypp;
        surf->pxfmt = RGBA32;
        if (!diballoc32(surf)) goto errfsd;
        return true;
errfsd: free(sd);
erret:  return false;
}

void surffree32(SURFACE *surf)
{
        SURFDAT32 *sd;
        ASSERT32();
        assert(surf != NULL);
        if (surf) {
                sd = SD32(surf);
                if (sd->hBitmap) DeleteObject(sd->hBitmap);
                if (surf->px)    VirtualFree(surf->px, 0, MEM_RELEASE);
        }   
}

LRESULT wndproc32(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        WINDOW   *win;
        SURFACE  *surf;
        WINDAT32 *wd;
        LONG_PTR  wlp;
        switch (uMsg) {
        case WM_CREATE:
                wlp = (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams;
                SetWindowLongPtr(hWnd, GWLP_USERDATA, wlp);
                break;
        case WM_CLOSE:
                win = WP32(hWnd);
                if (!win) break;
                win->open = 0;
                break;
        case WM_MOVE:
                win = WP32(hWnd);
                if (!win) break;
                win->x = LOWORD(lParam);
                win->y = HIWORD(lParam);
                break;
        case WM_SIZE:
                win        = WP32(hWnd);
                if (!win || !win->dat) break;
                wd         = WD32(win);
                surf       = win->surf;
                win->w     = LOWORD(lParam);
                win->h     = HIWORD(lParam);
                surf->w    = win->w;
                surf->h    = win->h;
                dibfree32(surf);
                diballoc32(surf);
                break;
        default: 
                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
        return 0;
}

#endif
