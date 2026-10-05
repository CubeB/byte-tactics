// Decompiled by space-bunny-free. Names are provisional.
#include <windows.h>
#include <string.h>

class Class_004e2d00 {
public:
    int FUN_004e2d00(const char* name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2cc0 {
public:
    bool FUN_004e2cc0(const char* name, unsigned int defaultValue);
};

class Class_004e2be0 {
public:
    int key;                           // +0x00
    unsigned char readOnly;            // +0x04
    Class_004e2be0(char readOnly, char* app, char* section);
    ~Class_004e2be0();
};

// FUNCTION: 0x4e3080
// Restores where a window sits from
// HKCU\Software\Cavedog Entertainment\Cavedog library\WindowPositions\<name>.
// Edges of -500 are the "no saved value" sentinel; zoomX/zoomY of 1.0 with no
// saved size resizes the window instead of moving it.
unsigned char __cdecl FUN_004e3080(HWND hwnd, char* name, double zoomX, double zoomY, char doSize)
{
    char buf[200];
    WINDOWPLACEMENT placement;
    RECT cur;
    RECT full;
    RECT work;
    int y;
    int flags;
    bool resizable;
    bool zoomed;
    int x;
    int w;
    int h;

    // The screen metrics are only a seed: SPI_GETWORKAREA overwrites all four
    // words, so left and top are dead and right and bottom never survive.
    work.left = 0;
    work.top = 0;
    work.right = GetSystemMetrics(SM_CXSCREEN);
    work.bottom = GetSystemMetrics(SM_CYSCREEN);
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);

    placement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &placement);

    strcpy(buf, "WindowPositions\\");
    strncat(buf, name, sizeof(buf) - 1 - strlen(buf));

    Class_004e2be0 key(1, buf, "Cavedog library");
    x = ((Class_004e2d00*)&key)->FUN_004e2d00("LeftEdge", -500, 50000, -500);
    y = ((Class_004e2d00*)&key)->FUN_004e2d00("TopEdge", -500, 50000, -500);

    long style = GetWindowLongA(hwnd, GWL_STYLE);
    resizable = false;
    GetWindowRect(hwnd, &cur);
    if ((style & WS_THICKFRAME) == WS_THICKFRAME) {
        resizable = true;
        w = ((Class_004e2d00*)&key)->FUN_004e2d00("Width", 0, work.right - work.left, 0);
        h = ((Class_004e2d00*)&key)->FUN_004e2d00("Height", 0, work.bottom - work.top, 0);
        zoomed = ((Class_004e2cc0*)&key)->FUN_004e2cc0("Zoomed", 0);
        if (w < 100) {
            w = 100;
        }
        if (h < 50) {
            h = 50;
        }
    } else {
        w = cur.right - cur.left;
        h = cur.bottom - cur.top;
        zoomed = false;
    }

    flags = SWP_NOZORDER;
    if (doSize) {
        flags = SWP_NOZORDER | SWP_SHOWWINDOW;
    }

    if (x == -500 || y == -500 || w <= 0 || h <= 0) {
        if (zoomX == 1.0 && zoomY == 1.0) {
            if (doSize) {
                ShowWindow(hwnd, SW_SHOWNORMAL);
            }
            return 0;
        }
        GetWindowRect(hwnd, &full);
        SetWindowPos(hwnd, 0, 0, 0,
                     (int)((full.right - full.left) * zoomX),
                     (int)((full.bottom - full.top) * zoomY),
                     flags | SWP_NOMOVE);
        return 1;
    }

    if (x < work.left - w / 2) {
        x = work.left - w / 2;
    }
    if (y < work.top) {
        y = work.top;
    }
    if (x + w / 2 > work.right) {
        x = work.right - w / 2;
    }
    if (y + h / 2 > work.bottom) {
        y = work.bottom - h / 2;
    }
    SetWindowPos(hwnd, 0, x, y, w, h, flags);
    if (resizable && zoomed) {
        return 1;
    }
    return 0;
}
