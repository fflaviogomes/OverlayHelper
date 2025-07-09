#define _WIN32_IE 0x0400
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <gdiplus.h>
#include <commdlg.h>   // File dialog
#include <algorithm>

using std::min;
using std::max;
using namespace Gdiplus;

#pragma comment (lib, "Gdiplus.lib")
#pragma comment (lib, "Comdlg32.lib")

ULONG_PTR gdiplusToken;
HBITMAP hBitmap = NULL;
BYTE opacity = 200;  // Initial opacity (0–255)

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HBITMAP LoadImageToBitmap(LPCWSTR filename) {
    Bitmap* bmp = Bitmap::FromFile(filename, FALSE);
    HBITMAP hbm = NULL;
    if (bmp && bmp->GetLastStatus() == Ok) {
        bmp->GetHBITMAP(Color::White, &hbm);
    }
    delete bmp;
    return hbm;
}

void ShowOpenFileDialog(HWND hwnd) {
    OPENFILENAME ofn = { sizeof(ofn) };
    WCHAR szFile[MAX_PATH] = L"";
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Image Files (*.png;*.bmp;*.jpg)\0*.png;*.bmp;*.jpg\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = L"Select an Image";
    if (GetOpenFileName(&ofn)) {
        if (hBitmap) DeleteObject(hBitmap);
        hBitmap = LoadImageToBitmap(ofn.lpstrFile);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

void UpdateOpacity(HWND hwnd, BYTE newOpacity) {
    opacity = newOpacity;
    SetLayeredWindowAttributes(hwnd, 0, opacity, LWA_ALPHA);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    GdiplusStartupInput gdInput;
    GdiplusStartup(&gdiplusToken, &gdInput, NULL);

    const wchar_t CLASS_NAME[] = L"OverlayHelperWin";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT, // <== WS_EX_TRANSPARENT allows click-through
        CLASS_NAME, L"Overlay Helper",
        WS_POPUP | WS_VISIBLE,
        100, 100, 600, 400,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hwnd, SW_SHOW);
    UpdateOpacity(hwnd, opacity);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (hBitmap) DeleteObject(hBitmap);
    GdiplusShutdown(gdiplusToken);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static POINT clickOffset;
    static bool dragging = false;

    switch (msg) {
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
                PostQuitMessage(0);
            else if (wParam == 'O' || wParam == 'o')
                ShowOpenFileDialog(hwnd);
            else if (wParam == VK_UP)
                UpdateOpacity(hwnd, min(255, opacity + 5));
            else if (wParam == VK_DOWN)
                UpdateOpacity(hwnd, max(10, opacity - 5));
            break;

        case WM_LBUTTONDOWN:
            dragging = true;
            clickOffset.x = LOWORD(lParam);
            clickOffset.y = HIWORD(lParam);
            SetCapture(hwnd);
            break;

        case WM_MOUSEMOVE:
            if (dragging && (wParam & MK_LBUTTON)) {
                POINT p; GetCursorPos(&p);
                SetWindowPos(hwnd, HWND_TOPMOST, p.x - clickOffset.x, p.y - clickOffset.y, 0, 0, SWP_NOSIZE);
            }
            break;

        case WM_LBUTTONUP:
            dragging = false;
            ReleaseCapture();
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
            if (hBitmap) {
                HDC memDC = CreateCompatibleDC(hdc);
                SelectObject(memDC, hBitmap);
                BITMAP bm; GetObject(hBitmap, sizeof(bm), &bm);
                BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
                DeleteDC(memDC);
            }
            EndPaint(hwnd, &ps);
        } break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
