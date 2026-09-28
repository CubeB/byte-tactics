// Decompiled by Space Bunny Free. Names are provisional.
#include <windows.h>
#include <string.h>

// Registry key helper under HKCU\Software\Cavedog Entertainment (0x4e2be0).
// The window placement is the last member, so the object is 8 + 0x2c = 52
// bytes and sits at frame offset 0x10.
class Class_004e2be0 {
public:
    int key;                            // +0x00
    unsigned char readOnly;             // +0x04
    WINDOWPLACEMENT placement;           // +0x08

    Class_004e2be0(char readOnly, char* app, char* section);
    ~Class_004e2be0();
};

// The two value writers, called on the same object (0x4e2d70, 0x4e2ce0).
class Class_004e2d70 {
public:
    void FUN_004e2d70(LPCSTR name, DWORD value);
};

class Class_004e2ce0 {
public:
    void FUN_004e2ce0(LPCSTR name, DWORD value);
};

// FUNCTION: 0x4e3400
// Saves where a resizable window sits, under
// HKCU\Software\Cavedog Entertainment\Cavedog library\WindowPositions\<name>.
void __cdecl FUN_004e3400(HWND hwnd, char* name)
{
    if (IsIconic(hwnd)) {
        return;
    }
    char buf[200];
    strcpy(buf, "WindowPositions\\");
    strncat(buf, name, sizeof(buf) - 1 - strlen(buf));
    Class_004e2be0 key(0, buf, "Cavedog library");
    RECT r;
    key.placement.length = sizeof(WINDOWPLACEMENT);
    if (GetWindowPlacement(hwnd, &key.placement)) {
        r = key.placement.rcNormalPosition;
        if (!IsZoomed(hwnd)) {
            if (!IsIconic(hwnd)) {
                GetWindowRect(hwnd, &r);
            }
        }
        r.right = r.right - r.left;
        r.bottom = r.bottom - r.top;
        // Both edges are clamped to -499 when they are 500 or further out.
        if (r.left <= -500) {
            r.left = -499;
        }
        if (r.top <= -500) {
            r.top = -499;
        }
        ((Class_004e2d70*)&key)->FUN_004e2d70("LeftEdge", r.left);
        ((Class_004e2d70*)&key)->FUN_004e2d70("TopEdge", r.top);
        if ((GetWindowLongA(hwnd, GWL_STYLE) & WS_THICKFRAME) == WS_THICKFRAME) {
            ((Class_004e2d70*)&key)->FUN_004e2d70("Width", r.right);
            ((Class_004e2d70*)&key)->FUN_004e2d70("Height", r.bottom);
            ((Class_004e2ce0*)&key)->FUN_004e2ce0("Zoomed", IsZoomed(hwnd) != 0);
        }
    }
}
