// Decompiled by Opus, Haiku, Sonnet, space-bunny-free and Space Bunny Free. Names are provisional.
// The std::_Tree rotations and node allocators, the Cavedog registry key
// helper with the window position save and restore, and the GDPERF driver
// calls. 0x4e35b0 (OpenGdperf) is in a gap region and stays in its own file.
#include <yvals.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

enum Redbl { _Red, _Black };

// A red-black tree node of MSVC 5's <xtree>: parent at +4, colour at +0x204.
struct Node_004e2950 {
    Node_004e2950* left;               // +0x0
    Node_004e2950* parent;             // +0x4
    Node_004e2950* right;              // +0x8
    char unknown_c[0x204 - 0xc];       // the key/value payload
    Redbl colour;                      // +0x204
};

// The tree's _Nil node.
extern Node_004e2950* DAT_005292c4;
extern void* DAT_00529e58;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler
extern char* DAT_00529e80;
extern HANDLE DAT_00529e98;            // the GDPERF driver
extern bool DAT_00529e9c;              // the driver is open
extern int DAT_00529ea0;               // the CPU family

// Shaped like std::_Tree<...>::_Lrotate(_Nodeptr) from MSVC 5's <xtree>
// (left rotation of a red-black tree node) under a lock object; head->parent
// is the root. Its _Rrotate is the next function, 0x4e29b0.
class Class_004e2950 {
public:
    int unknown_0;
    Node_004e2950* head;            // +0x4
    void FUN_004e2950(Node_004e2950* x);
};

// FUNCTION: 0x4e2950
void Class_004e2950::FUN_004e2950(Node_004e2950* x)
{
    std::_Lockit lock;
    Node_004e2950* y = x->right;
    x->right = y->left;
    if (y->left != DAT_005292c4)
        y->left->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Shaped like std::_Tree<...>::_Rrotate(_Nodeptr) from MSVC 5's <xtree>
// (right rotation of a red-black tree node) under a lock object.
class Class_004e29b0 {
public:
    int unknown_0;
    Node_004e2950* head;            // +0x4
    void FUN_004e29b0(Node_004e2950* x);
};

// FUNCTION: 0x4e29b0
void Class_004e29b0::FUN_004e29b0(Node_004e2950* x)
{
    std::_Lockit lock;
    Node_004e2950* y = x->left;
    x->left = y->right;
    if (y->right != DAT_005292c4)
        y->right->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

struct Data1 {
    int field_0;
};

struct Data2 {
    char field_0;
};

class Class_004e2a10 {
public:
    int field_0;
    char field_4;

    Class_004e2a10* FUN_004e2a10(const Data1* param_1, const Data2* param_2);
};

// FUNCTION: 0x4e2a10
Class_004e2a10* Class_004e2a10::FUN_004e2a10(const Data1* param_1, const Data2* param_2)
{
    field_0 = param_1->field_0;
    field_4 = param_2->field_0;
    return this;
}

// The pooled allocator inlined into _Buynode: the free list is refilled
// 0x2000 bytes at a time with the generic "carve n-byte pieces" loop (the
// out-of-line copy is 0x4e2b60).
static inline void* PoolAlloc(unsigned int n)
{
    if (DAT_00529e58 == 0) {
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (unsigned int rem = 0x2000; rem >= n; rem -= n) {
            *(void**)block = DAT_00529e58;
            DAT_00529e58 = block;
            block += n;
        }
    }
    void* p = DAT_00529e58;
    DAT_00529e58 = *(void**)p;
    return p;
}

// A method that ignores `this`: its caller (0x4e2250, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.
class Class_004e2a30 {
public:
    Node_004e2950* FUN_004e2a30(int param_1, int param_2);
};

// FUNCTION: 0x4e2a30
Node_004e2950* Class_004e2a30::FUN_004e2a30(int param_1, int param_2)
{
    Node_004e2950* node = (Node_004e2950*)PoolAlloc(sizeof(Node_004e2950));
    node->parent = (Node_004e2950*)param_1;
    node->colour = (Redbl)param_2;
    return node;
}

// Accessors return references, as in <XTREE>: changes how the node pointer is held.
static inline Redbl& Colour(Node_004e2950* p) { return p->colour; }
static inline Node_004e2950*& Left(Node_004e2950* p) { return p->left; }
static inline Node_004e2950*& Parent(Node_004e2950* p) { return p->parent; }
static inline Node_004e2950*& Right(Node_004e2950* p) { return p->right; }

// The _Max helper takes its own lock, the mirror of _Min (0x4e04e0) inside
// _Inc (0x4e0450).
static inline Node_004e2950* Max(Node_004e2950* p)
{
    std::_Lockit lock;
    while (Right(p) != DAT_005292c4)
        p = Right(p);
    return p;
}

class Class_004e2ab0 {
public:
    Node_004e2950* ptr;                // +0x0

    void FUN_004e2ab0();
};

// FUNCTION: 0x4e2ab0
void Class_004e2ab0::FUN_004e2ab0()
{
    std::_Lockit lock;
    if (Colour(ptr) == _Red && Parent(Parent(ptr)) == ptr)
        ptr = Right(ptr);
    else if (Left(ptr) != DAT_005292c4)
        ptr = Max(Left(ptr));
    else {
        Node_004e2950* p;
        while (ptr == Left(p = Parent(ptr)))
            ptr = p;
        ptr = p;
    }
}

// Pool allocator for the DAT_00529e58 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece. A method that ignores `this`: its
// callers (0x4e17c0, 0x4e2620) set ecx to the tree whose nodes it allocates
// (the allocator sits at +0), pushing the node size (0x208).
class Class_004e2b60 {
public:
    void* FUN_004e2b60(unsigned int n);
};

// FUNCTION: 0x4e2b60
void* Class_004e2b60::FUN_004e2b60(unsigned int n)
{
    if (DAT_00529e58 == 0) {
        unsigned int rem = 0x2000;
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (; rem >= n; rem -= n) {
            *(void**)block = DAT_00529e58;
            DAT_00529e58 = block;
            block += n;
        }
    }
    void* p = DAT_00529e58;
    DAT_00529e58 = *(void**)DAT_00529e58;
    return p;
}

// Reads a REG_DWORD value clamped to [minValue, maxValue], or returns
// defaultValue (signed counterpart of 0x4e2d90).
class Class_004e2d00 {
public:
    HKEY key;                        // +0x00
    int ReadInt(LPCSTR name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    HKEY field_0;

    void WriteDword(LPCSTR param_1, DWORD param_2);
};

class Class_004e2cc0 {
public:
    bool FUN_004e2cc0(const char* param1, unsigned int param2);
};

class Class_004e2ce0 {
public:
    void FUN_004e2ce0(LPCSTR param1, DWORD param2);
};

class Class_004e2e00 {
public:
    HKEY key;                          // +0x0

    void WriteInt(LPCSTR name, int value);
};

class CavedogRegistryKey {
public:
    HKEY key;                          // +0x00
    unsigned char readOnly;            // +0x04, set: read the values; clear: write them

    CavedogRegistryKey(char readOnly, char* app, char* section);
    ~CavedogRegistryKey();
    DWORD ReadDword(LPCSTR name, DWORD minValue, DWORD maxValue, DWORD defaultValue);
    void FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue);
    void FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue);
    void FUN_004e2ee0(char* name, short* value, short minValue, short maxValue, short defaultValue);
    void FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue);
    void FUN_004e2f90(char* name, char* value, char minValue, char maxValue, char defaultValue);
    void FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue);
};

// Registry key helper: the constructor opens (or creates) a key under
// HKCU\Software\Cavedog Entertainment, the destructor is empty.
// `readOnly` selects the family: nonzero only opens, zero creates as well.
// A null `section` falls back to the DAT_00529e80 default section name.
// FUNCTION: 0x4e2be0
CavedogRegistryKey::CavedogRegistryKey(char readOnly, char* app, char* section)
{
    HKEY k;
    if (section == 0) {
        section = DAT_00529e80;
    }
    key = 0;
    if (readOnly) {
        if (RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", &k) == ERROR_SUCCESS
            && RegOpenKeyA(k, section, &k) == ERROR_SUCCESS
            && RegOpenKeyA(k, app, &k) == ERROR_SUCCESS) {
            this->readOnly = readOnly;
            key = k;
            return;
        }
    } else {
        if (RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, section, &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, app, &k) == ERROR_SUCCESS) {
            key = k;
        }
    }
    this->readOnly = readOnly;
}

// The original calls this empty destructor out of line.
#pragma auto_inline(off)
// FUNCTION: 0x4e2cb0
CavedogRegistryKey::~CavedogRegistryKey()
{
}
#pragma auto_inline(on)

// The original calls this out of line from RestoreWindowPosition.
#pragma auto_inline(off)
// FUNCTION: 0x4e2cc0
bool Class_004e2cc0::FUN_004e2cc0(const char* param1, unsigned int param2)
{
    int r = ((Class_004e2d00*)this)->ReadInt(param1, 0, 1, param2 & 0xff);
    return r != 0 ? true : false;
}
#pragma auto_inline(on)

// The original calls this out of line from SaveWindowPosition.
#pragma auto_inline(off)
// FUNCTION: 0x4e2ce0
void Class_004e2ce0::FUN_004e2ce0(LPCSTR param1, DWORD param2)
{
    ((Class_004e2d70*)this)->WriteDword(param1, param2 & 0xff);
}
#pragma auto_inline(on)

// The original calls this out of line from its callers.
#pragma auto_inline(off)
// FUNCTION: 0x4e2d00
int Class_004e2d00::ReadInt(LPCSTR name, int minValue, int maxValue, int defaultValue)
{
    int value;
    DWORD size = 4;
    DWORD type = REG_DWORD;
    if (RegQueryValueExA(key, name, 0, &type, (LPBYTE)&value, &size) == ERROR_SUCCESS
        && size == 4 && type == REG_DWORD) {
        if (value < minValue) {
            value = minValue;
        }
        if (value > maxValue) {
            return maxValue;
        }
        return value;
    }
    return defaultValue;
}
#pragma auto_inline(on)

// The original calls this out of line from its callers.
#pragma auto_inline(off)
// FUNCTION: 0x4e2d70
void Class_004e2d70::WriteDword(LPCSTR param_1, DWORD param_2) {
    RegSetValueExA(field_0, param_1, 0, REG_DWORD, (LPBYTE)&param_2, 4);
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4e2e20.
#pragma auto_inline(off)
// FUNCTION: 0x4e2d90
DWORD CavedogRegistryKey::ReadDword(LPCSTR name, DWORD minValue, DWORD maxValue, DWORD defaultValue)
{
    DWORD value;
    DWORD size = 4;
    DWORD type = REG_DWORD;
    if (RegQueryValueExA(key, name, 0, &type, (LPBYTE)&value, &size) == ERROR_SUCCESS
        && size == 4 && type == REG_DWORD) {
        if (value < minValue) {
            value = minValue;
        }
        if (value > maxValue) {
            return maxValue;
        }
        return value;
    }
    return defaultValue;
}
#pragma auto_inline(on)

// Writes an integer registry value (same code as 0x4e2d70).
// The original calls this out of line from 0x4e2e20.
#pragma auto_inline(off)
// FUNCTION: 0x4e2e00
void Class_004e2e00::WriteInt(LPCSTR name, int value)
{
    RegSetValueExA(key, name, 0, REG_DWORD, (LPBYTE)&value, 4);
}
#pragma auto_inline(on)

// Reads or writes a DWORD registry value depending on the mode flag
// (compare 0x4e2ee0, the short version).
class Class_004e2e20 {
public:
    HKEY key;                        // +0x0
    char reading;                    // +0x4
    void FUN_004e2e20(LPCSTR name, DWORD* value, DWORD minValue, DWORD maxValue, DWORD defaultValue);
};

// FUNCTION: 0x4e2e20
void Class_004e2e20::FUN_004e2e20(LPCSTR name, DWORD* value, DWORD minValue, DWORD maxValue, DWORD defaultValue)
{
    if (reading) {
        *value = ((CavedogRegistryKey*)this)->ReadDword(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2e00*)this)->WriteInt(name, *value);
    }
}

// Reads or writes an int registry value depending on the mode flag
// (compare 0x4e2e20 and 0x4e2ee0).
// FUNCTION: 0x4e2e60
void CavedogRegistryKey::FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// The int version of 0x4e2ee0: reads a registry value into *value (clamped
// by ReadInt) when reading (readOnly), otherwise writes *value.
// FUNCTION: 0x4e2ea0
void CavedogRegistryKey::FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2ee0
void CavedogRegistryKey::FUN_004e2ee0(char* name, short* value, short minValue, short maxValue, short defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2f30
void CavedogRegistryKey::FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2f90
void CavedogRegistryKey::FUN_004e2f90(char* name, char* value, char minValue, char maxValue, char defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// Reads or writes a bool registry value, like the char/short versions at
// 0x4e2f90 and 0x4e2ee0.
class Class_004e2fe0 {
public:
    HKEY key;                        // +0x0
    char reading;                    // +0x4
    void FUN_004e2fe0(char* name, bool* value, bool defaultValue);
};

// FUNCTION: 0x4e2fe0
void Class_004e2fe0::FUN_004e2fe0(char* name, bool* value, bool defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, 0, 1, defaultValue) ? true : false;
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e3030
void CavedogRegistryKey::FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e3080
// Restores where a window sits from
// HKCU\Software\Cavedog Entertainment\Cavedog library\WindowPositions\<name>.
// Edges of -500 are the "no saved value" sentinel; zoomX/zoomY of 1.0 with no
// saved size resizes the window instead of moving it.
unsigned char __cdecl RestoreWindowPosition(HWND hwnd, char* name, double zoomX, double zoomY, char doSize)
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

    CavedogRegistryKey key(1, buf, "Cavedog library");
    x = ((Class_004e2d00*)&key)->ReadInt("LeftEdge", -500, 50000, -500);
    y = ((Class_004e2d00*)&key)->ReadInt("TopEdge", -500, 50000, -500);

    long style = GetWindowLongA(hwnd, GWL_STYLE);
    resizable = false;
    GetWindowRect(hwnd, &cur);
    if ((style & WS_THICKFRAME) == WS_THICKFRAME) {
        resizable = true;
        w = ((Class_004e2d00*)&key)->ReadInt("Width", 0, work.right - work.left, 0);
        h = ((Class_004e2d00*)&key)->ReadInt("Height", 0, work.bottom - work.top, 0);
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

// FUNCTION: 0x4e33d0
void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b)
{
    RestoreWindowPosition(hwnd, name, a, b, 1);
}

// FUNCTION: 0x4e3400
// Saves where a resizable window sits, under
// HKCU\Software\Cavedog Entertainment\Cavedog library\WindowPositions\<name>.
void __cdecl SaveWindowPosition(HWND hwnd, char* name)
{
    if (IsIconic(hwnd)) {
        return;
    }
    char buf[200];
    strcpy(buf, "WindowPositions\\");
    strncat(buf, name, sizeof(buf) - 1 - strlen(buf));
    CavedogRegistryKey key(0, buf, "Cavedog library");
    WINDOWPLACEMENT placement;
    RECT r;
    placement.length = sizeof(WINDOWPLACEMENT);
    if (GetWindowPlacement(hwnd, &placement)) {
        r = placement.rcNormalPosition;
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
        ((Class_004e2d70*)&key)->WriteDword("LeftEdge", r.left);
        ((Class_004e2d70*)&key)->WriteDword("TopEdge", r.top);
        if ((GetWindowLongA(hwnd, GWL_STYLE) & WS_THICKFRAME) == WS_THICKFRAME) {
            ((Class_004e2d70*)&key)->WriteDword("Width", r.right);
            ((Class_004e2d70*)&key)->WriteDword("Height", r.bottom);
            ((Class_004e2ce0*)&key)->FUN_004e2ce0("Zoomed", IsZoomed(hwnd) != 0);
        }
    }
}

// FUNCTION: 0x4e3710
bool CloseGdperf()
{
    if (DAT_00529e9c) {
        DAT_00529e9c = 0;
        if (DAT_00529e98) {
            BOOL ok = CloseHandle(DAT_00529e98);
            DAT_00529e98 = 0;
            if (ok)
                return true;
        }
    }
    return false;
}

// Unused here: declared early for the symbol ids (see 0x4e39a0).
int GetCpuFamily(void);
extern bool __cdecl ReadGdperf(DWORD a, void* out);
extern bool __cdecl WriteGdperf(DWORD a, DWORD b, DWORD c);
// The same function (0x4e3930), seen by ProgramPerfEvent with the 64-bit
// register value as one argument.
extern bool __cdecl WriteGdperf(DWORD a, __int64 b);

// Sends one effect-descriptor dword to the "Microsoft Game Device" driver that
// 0x4e38e0 opened (\\.\GDPERF, see the caller at 0x4e36c0). Two device families
// are handled, selected by DAT_00529ea0, which holds the CPU family nibble:
// 5 gets a two-call sequence, 6 a single call.
// FUNCTION: 0x4e3750
bool __cdecl ProgramPerfEvent(unsigned int effect, unsigned int level, char f1, char f2)
{
    __int64 val;
    if (level > 1)
        return false;
    if (!DAT_00529e9c)
        return false;
    if ((effect >> 28) != (unsigned int)DAT_00529ea0)
        return false;
    if (((level + 1) & (effect >> 8) & 3) == 0)
        return false;
    if (DAT_00529ea0 == 5) {
        // Each branch has its own unsigned char sub; a signed type changes the reload.
        unsigned char sub = (unsigned char)((effect >> 16) & 0x3f);
        ReadGdperf(0x11, &val);
        val &= (level ? 0xffffu : 0xffff0000u);
        WriteGdperf(0x11, val);
        WriteGdperf(0x12 + (level != 0), 0);
        // Exactly one term uses the mask-and-(?: -1) form: keeps the source operand order.
        val |= (f2 ? 0x40 : 0) | (0x80 & (f1 ? -1 : 0));
        if (sub == 0x3f)
            return WriteGdperf(0x11, val | 0x100);
        return WriteGdperf(0x11, val | sub);
    }
    if (DAT_00529ea0 == 6) {
        unsigned char sub = (unsigned char)(effect >> 16);
        int v = (0x4400 | (effect & 0xff)) * 0x100
              | (f1 ? 0x10000 : 0) | (0x20000 & (f2 ? -1 : 0))
              | sub;
        return WriteGdperf(0x186 + (level != 0), v);
    }
    return false;
}

// Sends one dword to the driver opened in DAT_00529e98 and reads back
// 8 bytes; succeeds only when exactly 8 bytes were returned.
// The original calls this out of line from ProgramPerfEvent.
#pragma auto_inline(off)
// FUNCTION: 0x4e38e0
bool __cdecl ReadGdperf(DWORD a, void* out)
{
    DWORD in = a;
    if (!DAT_00529e9c) {
        return false;
    }
    DWORD returned;
    BOOL ok = DeviceIoControl(DAT_00529e98, 0x9c406404, &in, sizeof(in), out, 8, &returned, 0);
    if (returned != 8) {
        ok = 0;
    }
    return ok != 0;
}
#pragma auto_inline(on)

// The original calls this out of line from ProgramPerfEvent.
#pragma auto_inline(off)
// FUNCTION: 0x4e3930
bool __cdecl WriteGdperf(DWORD a, DWORD b, DWORD c)
{
    if (!DAT_00529e9c) {
        return false;
    }
    DWORD in[3];
    in[0] = a;
    in[1] = b;
    in[2] = c;
    DWORD returned;
    BOOL ok = DeviceIoControl(DAT_00529e98, 0x9c406400, in, sizeof(in), 0, 0, &returned, 0);
    if (returned != 0) {
        ok = 0;
    }
    return ok != 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e39a0
int GetCpuFamily(void)
{
    return DAT_00529ea0;
}

// FUNCTION: 0x4e3e10
int FUN_004e3e10(void)
{
    return 0xfffffffd;
}

extern void __cdecl EmptyAtexitHandler();
void __cdecl FUN_004e4250(void);
void __cdecl FUN_004e4260(void);
void __cdecl FUN_004e4270(void);
void __cdecl FUN_004e4280(void);
void __cdecl FUN_004e4290(void);
void __cdecl FUN_004e42a0(void);

// FUNCTION: 0x4e41e0
void FUN_004e41e0(void)
{
    atexit(FUN_004e42a0);
}

// FUNCTION: 0x4e41f0
void FUN_004e41f0()
{
    atexit(FUN_004e4290);
}

// FUNCTION: 0x4e4200
void FUN_004e4200()
{
    atexit(EmptyAtexitHandler);
}

// FUNCTION: 0x4e4210
void FUN_004e4210()
{
    atexit(FUN_004e4280);
}

// FUNCTION: 0x4e4220
void FUN_004e4220()
{
    atexit(FUN_004e4270);
}

// FUNCTION: 0x4e4230
void FUN_004e4230()
{
    atexit(FUN_004e4260);
}

// FUNCTION: 0x4e4240
void FUN_004e4240()
{
    atexit(FUN_004e4250);
}

// FUNCTION: 0x4e4250
void __cdecl FUN_004e4250(void)
{
}

// FUNCTION: 0x4e4260
void __cdecl FUN_004e4260(void)
{
}

// FUNCTION: 0x4e4270
void __cdecl FUN_004e4270(void)
{
}

// FUNCTION: 0x4e4280
void __cdecl FUN_004e4280(void)
{
}

// FUNCTION: 0x4e4290
void __cdecl FUN_004e4290(void)
{
}

// FUNCTION: 0x4e42a0
void __cdecl FUN_004e42a0(void)
{
}
