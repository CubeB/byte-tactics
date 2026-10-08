// Decompiled by Opus, Sonnet, Haiku, deepseek-v4.1, deepseek-v4.1-flash, GPT-6, space-bunny-free, muse-spark-1.3-free and Claude Opus 5.5. Names are provisional.
// The debug library's memory status dialog, performance settings, timer and
// name table: the working-set report and its dialog, the Cavedog registry key,
// the performance counters and events, the timer, the mapped-file wrapper and
// the name table's tree methods.
//
// 0x4e16b0 (src/debug/debug_lib_4e16b0.cpp) and 0x4e1e50 with 0x4e20a0
// (src/debug/debug_lib_4e1e50.cpp) stay in their own files: each is the source
// of a `gap` region of data/functions.csv and uses inline assembly.
// 0x4e1990 (src/debug/debug_lib_4e1990.cpp) stays in its own file: /Ob2
// inlines NameKey::FUN_004e1a30 at its call site here, where the original
// calls it, while 0x4e2250 needs the same definition inlinable.
// 0x4e21f0 (src/debug/debug_lib_4e21f0.cpp) stays in its own file: its 0.0
// and 5.0 constants sit in a different constant pool from the memory status
// dialog's 0.0, so the original had it in another translation unit.
// 0x4e2580 (src/debug/debug_lib_4e2580.cpp) and 0x4e2620 (src/debug/debug_lib_4e2620.cpp)
// stay in their own files: their views of Class_004e2580 and Class_004e2620
// disagree with the views the tree methods here compile from.
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <yvals.h>
// Renames the allocator's _Charalloc so the call carries the symbol name FUN_004e2b60.
#define _Charalloc FUN_004e2b60
#include <map>
#undef _Charalloc

// The trees' shared _Nil node (0x5292c4). Each tree's methods below name it
// under their own node type, so it is declared as the untyped node pointer.
extern void* DAT_005292c4;

struct Node_004e04e0 {
    Node_004e04e0* left;               // +0x0
    Node_004e04e0* parent;             // +0x4
    Node_004e04e0* right;              // +0x8
};

// Shaped like std::_Tree<...>::_Min(_Nodeptr) from MSVC 5's <xtree>: follows
// left links under a lock until the tree's _Nil node (DAT_005292c4). Its
// caller (0x4df380) uses it for an inlined iterator increment.
// FUNCTION: 0x4e04e0
Node_004e04e0* __cdecl FUN_004e04e0(Node_004e04e0* p)
{
    std::_Lockit lock;
    while (p->left != DAT_005292c4)
        p = p->left;
    return p;
}

// Registry key helper: the constructor opens (or creates) a key under
// HKCU\Software\Cavedog Entertainment, the destructor is empty.
// Note: 0x4e2cb0 is this class's (empty, out-of-line) destructor; it is
// called with ecx = the local key object at the end of its scope.
class CavedogRegistryKey {
public:
    int key;                           // +0x00
    unsigned char readOnly;            // +0x04
    CavedogRegistryKey(int readOnly, char* app, char* section);
    ~CavedogRegistryKey();
};

class Class_004e2fe0 {
public:
    void FUN_004e2fe0(char* name, unsigned char* value, unsigned char def);
};

extern char* DAT_0050d72c;

class Class_004e0520 {
public:
    char unknown_0[0x79];
    unsigned char workingSet;          // +0x79
    void FUN_004e0520(int readOnly);
};

// The original calls this from 0x4e0570 and HandleMemoryStatusMessage rather
// than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e0520
void Class_004e0520::FUN_004e0520(int readOnly)
{
    CavedogRegistryKey key(readOnly, DAT_0050d72c, "CavedogLibrary");
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("WorkingSet", &workingSet, 0);
}
#pragma auto_inline(on)

double __cdecl GetTimeSeconds();

struct Triple_004e0570 {
    int a;
    int b;
    int c;
    Triple_004e0570() { a = 0; b = 0; c = 0; }
};

struct Sub_004e0570 {
    int table[20];                     // +0x00
    Triple_004e0570 triple;            // +0x50
    Sub_004e0570() { memset(table, 0, sizeof(table)); }
};

class Class_004e0570 {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    double time;                       // +0x10
    Sub_004e0570 sub;                  // +0x18
    int unknown_74;                    // +0x74
    unsigned char flag_78;             // +0x78
    Class_004e0570();
    // The empty inline destructor makes MSVC register the empty atexit thunk
    // FUN_004e1450.
    ~Class_004e0570() {}
};

class Class_004e05f0 {
public:
    HWND hwnd;                         // +0x00
    char unknown_4[0x74];
    char flag_78;                      // +0x78
    void SetMemoryStatusWindowVisible(char on);
};

struct Rate_004e0b90 {
    double table[10]; double total; int index; int unknown_5c;
};

class MemoryStatusDialog {
public:
    HWND hwnd;                         // +0x00
    int field_4;                       // +0x04
    int left;                          // +0x08
    int top;                           // +0x0c
    double time;                       // +0x10
    Rate_004e0b90 rates;              // +0x18
    char flag_78;                      // +0x78
    unsigned char workingSet;          // +0x79

    int HandleMemoryStatusMessage(unsigned int msg, int wParam, int lParam);
    void CreateMemoryStatusDialog();
};

// The original calls the constructor from FUN_004e1410 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e0570
Class_004e0570::Class_004e0570()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = -1;
    field_c = -1;
    flag_78 = 0;
    time = GetTimeSeconds();
    ((Class_004e0520*)this)->FUN_004e0520(1);
    ((MemoryStatusDialog*)this)->CreateMemoryStatusDialog();
}
#pragma auto_inline(on)

HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK MemoryStatusDlgProc(HWND, UINT, WPARAM, LPARAM);

extern char DAT_0050d6b4[]; // "Performance dialog failed to open"
extern char DAT_0050c8ac[]; // "Cavedog"

// The original calls this from the timer constructor rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e05c0
void MemoryStatusDialog::CreateMemoryStatusDialog()
{
    HWND result = CreateDialogFromTemplate(0x66, GetDesktopWindow(), (DLGPROC)MemoryStatusDlgProc, (LPARAM)this);
    if (result == 0)
        MessageBoxA(0, DAT_0050d6b4, DAT_0050c8ac, 0);
}
#pragma auto_inline(on)

void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void FUN_004e0790(void);

// The original calls this from HandleMemoryStatusMessage and ShowMemoryStatus
// rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e05f0
void Class_004e05f0::SetMemoryStatusWindowVisible(char on)
{
    if (on) {
        if (hwnd != NULL) {
            EnableWindow(hwnd, TRUE);
            SetFocus(hwnd);
            SetForegroundWindow(hwnd);
            FUN_004e33d0(hwnd, DAT_0050d72c, 1.0, 1.0);
            SetTimer(hwnd, 1, 200, NULL);
            return;
        }
        flag_78 = 1;
        return;
    }
    if (IsWindowVisible(hwnd)) {
        KillTimer(hwnd, 1);
        SaveWindowPosition(hwnd, DAT_0050d72c);
        ShowWindow(hwnd, SW_HIDE);
        FUN_004e0790();
    }
}
#pragma auto_inline(on)

// Dialog procedure: on WM_INITDIALOG it stores the object passed as lParam in
// the window's user data (and the window handle in the object's first field),
// then forwards every message to that object's handler. Same shape as 0x4df330.
// FUNCTION: 0x4e06a0
BOOL __stdcall MemoryStatusDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((MemoryStatusDialog*)lParam)->hwnd = hwnd;
    }
    MemoryStatusDialog* obj = (MemoryStatusDialog*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->HandleMemoryStatusMessage(msg, wParam, lParam);
    return 0;
}

// Returns a critical section that is initialised on first use (a
// function-local static; its atexit destructor 0x4e0730 is empty).
class CritSec_004e06f0 {
public:
    CRITICAL_SECTION cs;

    CritSec_004e06f0() { InitializeCriticalSection(&cs); }
    ~CritSec_004e06f0() {}
};

// The original calls this from 0x4e0740, 0x4e0790 and FormatWorkingSet
// rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e06f0
LPCRITICAL_SECTION FUN_004e06f0(void)
{
    static CritSec_004e06f0 lock;
    return &lock.cs;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e0730
void FUN_004e0730(void)
{
}

extern HMODULE DAT_005295bc;
extern int DAT_00529508;
extern char DAT_005295cc;

// FUNCTION: 0x4e0740
void FUN_004e0740(void)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    EnterCriticalSection(cs);
    if (DAT_005295bc != 0) {
        FreeLibrary(DAT_005295bc);
        DAT_005295bc = 0;
        DAT_00529508 = 0;
    }
    DAT_005295cc = 1;
    LeaveCriticalSection(cs);
}

// The original calls this from 0x4e05f0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e0790
void FUN_004e0790(void)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    EnterCriticalSection(cs);
    if (DAT_005295bc != 0) {
        FreeLibrary(DAT_005295bc);
        DAT_005295bc = 0;
        DAT_00529508 = 0;
    }
    DAT_005295cc = 0;
    LeaveCriticalSection(cs);
}
#pragma auto_inline(on)

extern HANDLE DAT_00529530;
extern int DAT_00529528;
extern int DAT_005295b8;
extern int DAT_005295c0;
extern int DAT_005295c8;
extern int DAT_005295d0;
extern int DAT_005295d4;

typedef struct {
    DWORD NumberOfPages;
    unsigned int WorkingSetInfo[99999];
} WSInfo_004e07e0;

typedef BOOL (WINAPI *QueryWorkingSet_004e07e0)(HANDLE, PVOID, DWORD);

// Writes n as a decimal with thousands separators, then reverses it.
static void __inline fmt_004e07e0(char *buf, unsigned int n)
{
    char *f;
    char *w;
    int digits;

    *buf = 0;
    f = buf;
    w = buf;
    digits = 0;
    do {
        *w = (char)('0' + n % 10);
        w++;
        n /= 10;
        digits++;
        if (digits % 3 == 0) {
            if (n != 0) {
                *w = ',';
                w++;
            }
        }
    } while (n);
    *w = 0;
    w--;
    for (; f < w;) {
        char a = *w;
        char b = *f;
        *f++ = a;
        *w-- = b;
    }
}

// Reports the process working set into a caller-supplied buffer, with a
// psapi.dll QueryWorkingSet refresh at most once every ten calls.
// FUNCTION: 0x4e07e0
char __cdecl FormatWorkingSet(char *dest)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    WSInfo_004e07e0 ws;
    char priv[20];
    char max[20];
    char shared[20];
    char total[20];
    char pt[20];
    DWORD *p;
    DWORD w;
    DWORD lo;
    DWORD hi;
    DWORD n;

    EnterCriticalSection(cs);
    if (DAT_005295d0 < 0 || !(--DAT_005295d0 > 0)) {
        if (DAT_005295cc == 0) {
            DAT_005295bc = LoadLibraryA("psapi.dll");
            if (DAT_005295bc != 0) {
                DAT_00529508 = (int)GetProcAddress(DAT_005295bc, "QueryWorkingSet");
            }
            DAT_00529530 = GetCurrentProcess();
            DAT_005295cc = 1;
        }
        if (DAT_00529508 == 0) {
            LeaveCriticalSection(cs);
            return 0;
        }
        if (!((QueryWorkingSet_004e07e0)DAT_00529508)(DAT_00529530, &ws, 0x61a80)) {
            LeaveCriticalSection(cs);
            return 0;
        }
        n = ws.NumberOfPages;
        DAT_005295c8 = n;
        if (n > DAT_005295d4) {
            DAT_005295d4 = n;
        }
        // Chained, not separate statements: decouples register order from store order.
        DAT_00529528 = DAT_005295b8 = DAT_005295c0 = 0;
        // Counters stay globals, incremented in the loop: the formatters reload them.
        if (n > 0) {
            p = (DWORD *)&ws.WorkingSetInfo[0];
            // Separate copy of n, assigned after p: fixes the instruction order.
            DWORD cnt = n;
            do {
                w = *p;
                lo = w & 0xfff;
                hi = w & 0xfffff000;
                if (hi >= 0xc0000000 && hi <= 0xe0000000) {
                    DAT_005295c0++;
                } else if (lo & 0x100) {
                    DAT_005295b8++;
                } else {
                    DAT_00529528++;
                }
                p++;
            } while (--cnt);
        }
        DAT_005295d0 = 10;
    }
    fmt_004e07e0(pt, DAT_005295c0 << 12);
    fmt_004e07e0(shared, DAT_005295b8 << 12);
    fmt_004e07e0(priv, DAT_00529528 << 12);
    fmt_004e07e0(max, DAT_005295d4 << 12);
    fmt_004e07e0(total, DAT_005295c8 << 12);
    sprintf(dest, "\r\nTotal Working set:   %13s\r\nMaximum Working set: %13s\r\nPrivate:             %13s\r\nShared:              %13s\r\nPage Tables:         %13s\r\n", total, max, priv, shared, pt);
    LeaveCriticalSection(cs);
    return 1;
}

extern unsigned int DAT_005289d0;
extern unsigned int DAT_005289d4;
extern unsigned int DAT_005289d8;
extern unsigned int DAT_005289dc;
extern unsigned int DAT_005289f0;
extern unsigned int DAT_005289f8;
extern unsigned int DAT_005289fc;
extern unsigned int DAT_00528a00;
extern unsigned int DAT_00528a04;
extern unsigned int DAT_00528a08;
extern unsigned int DAT_00528a1c;
extern char DAT_005119b8;
extern char DAT_005295d8[];

void __cdecl OpenUrl(HWND hwnd, char* url, char* ext);
void ResetAllocStats(void);
char __cdecl FormatWorkingSet(char* dest);

// Free function shape: the quotient is computed inside the helper, so the
// divisor stays on the x87 stack and the divide becomes three fxch plus
// fdiv st(2), as in the original.
static void __inline rate_add(Rate_004e0b90* r, int delta, double dt)
{
    double speed = (double)delta / dt;
    r->total = speed + r->total - r->table[r->index];
    r->table[r->index] = speed;
    if (++r->index == 10)
        r->index = 0;
}

// Same routine as fmt_004e07e0: decimal with thousands
// separators, then reversed in place.
static void __inline fmt_004e0b90(char* buf, unsigned int n)
{
    char* f;
    char* w;
    int digits;

    *buf = 0;
    f = buf;
    w = buf;
    digits = 0;
    do {
        *w = (char)('0' + n % 10);
        w++;
        n /= 10;
        digits++;
        if (digits % 3 == 0) {
            if (n != 0) {
                *w = ',';
                w++;
            }
        }
    } while (n);
    *w = 0;
    w--;
    for (; f < w;) {
        char a = *w;
        char b = *f;
        *f++ = a;
        *w-- = b;
    }
}

// FUNCTION: 0x4e0b90
int MemoryStatusDialog::HandleMemoryStatusMessage(unsigned int msg, int wParam, int lParam)
{
    switch (msg) {
    case 0x312:
        if (wParam != 10)
            return 0;
        ((Class_004e05f0*)this)->SetMemoryStatusWindowVisible(!IsWindowVisible(hwnd));
        return 0;

    case 0x110: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        left = rect.left;
        top = rect.top;
        RegisterHotKey(hwnd, 10, 1, 0x23);
        CheckDlgButton(hwnd, 0x3f9, workingSet);
        if (flag_78)
            ((Class_004e05f0*)this)->SetMemoryStatusWindowVisible(1);
        return 1;
    }

    case 0x111:
        switch (wParam & 0xffff) {
        case 0x3f9:
            workingSet = (workingSet == 0);
            ((Class_004e0520*)this)->FUN_004e0520(0);
            return 0;
        case 0x3fa:
            OpenUrl(hwnd, "http://10.0.150.18/programming/library/extras/memorystatusdialog.html", ".htm");
            return 0;
        case 0x3eb:
            ResetAllocStats();
            return 0;
        case 1:
        case 2:
            ((Class_004e05f0*)this)->SetMemoryStatusWindowVisible(0);
            return 0;
        default:
            return 0;
        }

    case 0x113: {
        char buf[2000];
        char req[100];
        char copy[20];
        char s0[20];
        char s2[20];
        char b[20];
        char s6[20];
        char t[20];
        char s5[20];
        char t2[20];
        char s4[20];
        char s3[20];
        char s1[20];

        if (!IsWindowVisible(hwnd))
            return 0;

        RECT rect;
        GetWindowRect(hwnd, &rect);
        if (rect.left != left || rect.top != top) {
            left = rect.left;
            top = rect.top;
            SaveWindowPosition(hwnd, DAT_0050d72c);
        }

        int delta = DAT_00528a04 - field_4;
        double now = GetTimeSeconds();
        double speed = now - time;
        time = now;
        rate_add(&rates, delta, speed);
        field_4 = DAT_00528a04;

        buf[0] = DAT_005119b8;
        memset(buf + 1, 0, sizeof(buf) - 1);

        if (DAT_005289fc != 0) {
            fmt_004e0b90(t, DAT_005289dc + 1000000000);
            strcpy(copy, t);
            fmt_004e0b90(b, DAT_005289fc);
            sprintf(req, "%s,%s", b, copy + 2);
        } else {
            fmt_004e0b90(t2, DAT_005289dc);
            sprintf(req, "%s", t2);
        }

        double rate = rates.total * 0.1;
        double rateText = (rate > 0.0) ? rate : 0.0;

        fmt_004e0b90(s0, DAT_005289d0);
        fmt_004e0b90(s1, DAT_005289f0);
        fmt_004e0b90(s2, DAT_005289d8);
        fmt_004e0b90(s3, DAT_005289f8);
        fmt_004e0b90(s4, (unsigned int)field_4);
        fmt_004e0b90(s5, DAT_00528a1c);
        // Written out by hand: a tenth helper call would exceed the inline budget.
        {
            char* buf = s6;
            unsigned int n = DAT_00528a08;
            char* f;
            char* w;
            int digits;
            *buf = 0;
            f = buf;
            w = buf;
            digits = 0;
            do {
                *w = (char)('0' + n % 10);
                w++;
                n /= 10;
                digits++;
                if (digits % 3 == 0) {
                    if (n != 0) {
                        *w = ',';
                        w++;
                    }
                }
            } while (n);
            *w = 0;
            w--;
            for (; f < w;) {
                char a = *w;
                char b = *f;
                *f++ = a;
                *w-- = b;
            }
        }


        sprintf(buf,
                "Current Allocations:  %13s\r\n"
                "Max Allocations:      %13s\r\n"
                "Allocation Requests:  %13s\r\n"
                "Allocations per second:  %10.1f\r\n"
                "\r\n"
                "Current Bytes:        %13s\r\n"
                "Max Bytes:            %13s\r\n"
                "Bytes Requested:  %17s\r\n"
                "\r\n"
                "Next Alloc Location:    %2X:%08lX\r\n"
                "Current with padding: %13s\r\n"
                "Max with padding:     %13s\r\n",
                s6, s5, s4, rateText, s3, s2, req, DAT_00528a00, DAT_005289d4, s1, s0);

        if (workingSet != 0)
            FormatWorkingSet(&buf[strlen(buf)]);

        if (strcmp(DAT_005295d8, buf) == 0)
            return 0;

        SetDlgItemTextA(hwnd, 1000, buf);
        UpdateWindow(hwnd);
        strcpy(DAT_005295d8, buf);
        return 0;
    }

    default:
        return 0;
    }
}

Class_004e0570* FUN_004e1410();

// The original calls this from StartMemoryStatus rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1400
void ShowMemoryStatus()
{
    ((Class_004e05f0*)FUN_004e1410())->SetMemoryStatusWindowVisible(1);
}
#pragma auto_inline(on)

// Returns a function-local static Class_004e0570; the empty inline destructor
// makes MSVC register the empty atexit thunk FUN_004e1450.
// The original calls this from 0x4e1400 and 0x4e1460 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1410
Class_004e0570* FUN_004e1410()
{
    static Class_004e0570 timer;
    return &timer;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e1450
void FUN_004e1450(void)
{
}

extern char* __cdecl FindCommandLineSwitch(char*);
void ShowMemoryStatus();

extern char DAT_0050d220[];

// FUNCTION: 0x4e1460
void __cdecl StartMemoryStatus()
{
    FUN_004e1410();
    char* result = FindCommandLineSwitch(DAT_0050d220);
    if (result != 0) {
        ShowMemoryStatus();
    }
}

class Class_004e1590 {
public:
    HANDLE hFile;      // +0x0
    HANDLE hMapping;   // +0x4
    void* view;        // +0x8
    DWORD size;        // +0xc
    int state;         // +0x10

    void OpenMappedFile(const char* fileName);
};

class MappedFile {
public:
    HANDLE hFile;     // +0x0
    HANDLE hMapping;  // +0x4
    void* view;       // +0x8
    int size;         // +0xc
    int state;        // +0x10

    MappedFile(const char* fileName);
    void CloseMappedFile();
};

// Constructor of a memory-mapped-file wrapper: initialises the handle/state
// fields, then opens the file if a name was given (see OpenMappedFile).
// FUNCTION: 0x4e1560
MappedFile::MappedFile(const char* fileName)
{
    hFile = (void*)-1;
    hMapping = 0;
    view = 0;
    size = 0;
    state = 0;
    if (fileName != 0)
        ((Class_004e1590*)this)->OpenMappedFile(fileName);
}

// Opens a memory-mapped file: create the file, map it read-only, then map a
// view. On each failure the handles are closed and a state code is stored
// (1 = file open failed, 2 = mapping failed, 3 = view failed).
// FUNCTION: 0x4e1590
void Class_004e1590::OpenMappedFile(const char* fileName)
{
    hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        state = 1;
        return;
    }
    size = GetFileSize(hFile, NULL);
    hMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == NULL) {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        state = 2;
        return;
    }
    view = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (view == NULL) {
        CloseHandle(hMapping);
        hMapping = NULL;
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        state = 3;
    }
}

// Closes a memory-mapped file (the wrapper built by 0x4e1560): unmaps the
// view, then closes the mapping and file handles.
// FUNCTION: 0x4e1650
void MappedFile::CloseMappedFile()
{
    if (view != 0) {
        UnmapViewOfFile(view);
    }
    if (hMapping != 0) {
        CloseHandle(hMapping);
    }
    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
    }
}

unsigned char __cdecl OpenGdperf(void);

extern unsigned char DAT_00529e6c;
extern unsigned char DAT_00529e70;

// The original calls this from InitPerformanceEvents rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1680
unsigned char __cdecl HasPerfCounters(void)
{
    if (DAT_00529e6c == 0) {
        DAT_00529e6c = 1;
        unsigned char result = OpenGdperf();
        if (result != 0) {
            DAT_00529e70 = 1;
        }
    }
    return DAT_00529e70;
}
#pragma auto_inline(on)

// 0x4e16b0 (src/debug/debug_lib_4e16b0.cpp) stays in its own file: it is the
// source of a gap region and uses inline cpuid.

// FUNCTION: 0x4e1700
char IsPentiumOrBetter(void)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    if (info.dwProcessorType == 0x182 || info.dwProcessorType == 0x1e6)
        return 0;
    return 1;
}

// Returns the current time in seconds: from the performance counter when it
// is available, otherwise from the system time (100 ns file-time units).
// The original calls this from the timer and the dialog rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1730
double __cdecl GetTimeSeconds()
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    BOOL haveFrequency = QueryPerformanceFrequency(&frequency);
    BOOL haveCounter = QueryPerformanceCounter(&counter);
    if (haveFrequency == TRUE && haveCounter == TRUE)
        return (double)counter.QuadPart / (double)frequency.QuadPart;

    SYSTEMTIME systemTime;
    FILETIME fileTime;
    GetSystemTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &fileTime);
    return ((double)fileTime.dwLowDateTime + (double)fileTime.dwHighDateTime * 4294967296.0) * 1e-7;
}
#pragma auto_inline(on)

extern void* DAT_00529e58;             // node free list
extern unsigned int DAT_00529500;      // tree _Nilrefs
extern void (*DAT_005289bc)();         // out-of-memory handler

struct Less_004e17c0 {
    bool operator()(const char* a, const char* b) const
    {
        return a != b && strcmp(a, b) < 0;
    }
};

struct Value_004e17c0 {
    char text[500];
};

typedef std::pair<const char*, Value_004e17c0> ValueType_004e17c0;

// The pooled allocator; its _Charalloc is the out-of-line 0x4e2b60.
class Class_004e2b60 {
public:
    typedef ValueType_004e17c0 value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type n, const void* = 0)
    {
        return (pointer)FUN_004e2b60(n * sizeof(value_type));
    }
    void deallocate(void* p, size_type)
    {
        if (p != 0) {
            *(void**)p = DAT_00529e58;
            DAT_00529e58 = p;
        }
    }
    size_type max_size() const { return (size_type)(-1) / sizeof(value_type); }

    char* FUN_004e2b60(size_type n)
    {
        if (DAT_00529e58 == 0) {
            char* block;
            do {
                block = (char*)GlobalAlloc(0, 0x2000);
                if (block == 0 && DAT_005289bc != 0)
                    DAT_005289bc();
            } while (block == 0 && DAT_005289bc != 0);
            if (block == 0)
                return 0;
            for (unsigned int rem = 0x2000; rem >= n; rem -= n) {
                *(void**)block = DAT_00529e58;
                DAT_00529e58 = block;
                block += n;
            }
        }
        void* p = DAT_00529e58;
        DAT_00529e58 = *(void**)p;
        return (char*)p;
    }
};

typedef std::map<const char*, Value_004e17c0, Less_004e17c0, Class_004e2b60>
    Map_004e17c0;

class NameTable {
public:
    Map_004e17c0 names;                // +0x0
    bool changed;                      // +0x10

    NameTable();
};

// Constructor of the global name-table singleton (allocated by GetNameTable).
// The tree's static _Nil / _Nilrefs are DAT_005292c4 / DAT_00529500 and the
// pooled node free list is DAT_00529e58, exactly as in <xtree>'s _Init with a
// pooled allocator. The map member sits at +0, the "changed" flag at +0x10.
//
// The map's allocator pools 0x208-byte nodes.
// The original calls the constructor from GetNameTable rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e17c0
NameTable::NameTable() : changed(0)
{
}
#pragma auto_inline(on)

// The wrapper around a std::_Tree whose other methods are 0x4e1990 (insert),
// 0x4dfea0 (erase one node) and 0x4e03f0 (_Erase a subtree). The fast path
// erases the whole tree under a std::_Lockit and returns begin(), which the
// caller discards; the slow path walks the nodes with operator++(int).
// DAT_005292c4 is the tree's shared _Nil node.
struct Node_004e18c0 {
    Node_004e18c0* left;               // +0x0
    Node_004e18c0* parent;             // +0x4
    Node_004e18c0* right;              // +0x8
};

class Class_004e0450 {
public:
    Node_004e18c0* ptr;

    void FUN_004e0450();
};

class Iter_004e18c0 {
public:
    Node_004e18c0* ptr;

    Iter_004e18c0() {}
    Iter_004e18c0(Node_004e18c0* p) : ptr(p) {}
    bool operator==(const Iter_004e18c0& x) const { return ptr == x.ptr; }
    bool operator!=(const Iter_004e18c0& x) const { return !(*this == x); }
    Iter_004e18c0& operator++()
    {
        ((Class_004e0450*)&ptr)->FUN_004e0450();
        return *this;
    }
    Iter_004e18c0 operator++(int)
    {
        Iter_004e18c0 tmp = *this;
        ((Class_004e0450*)&ptr)->FUN_004e0450();
        return tmp;
    }
};

class Class_004e03f0 {
public:
    void FUN_004e03f0(Node_004e18c0* x);
};

class Class_004dfea0 {
public:
    Iter_004e18c0 FUN_004dfea0(Iter_004e18c0 it);
};

class Class_004e2240 {
public:
    char unknown_0[4];                 // +0x0
    Node_004e18c0* head;               // +0x4

    Iter_004e18c0 FUN_004e2240();
};

class Class_004e18c0 {
public:
    char unknown_0[4];                 // +0x0
    Node_004e18c0* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc
    bool changed;                      // +0x10

    Iter_004e18c0 begin() { return head->left; }
    Iter_004e18c0 end() { return head; }

    Iter_004e18c0 erase(Iter_004e18c0 _F, Iter_004e18c0 _L)
    {
        if (size == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                ((Class_004dfea0*)this)->FUN_004dfea0(_F++);
            return _F;
        }
        // Early return above, not an else: keeps the return slot apart from the lock.
        std::_Lockit Lk;
        ((Class_004e03f0*)this)->FUN_004e03f0(head->parent);
        head->parent = (Node_004e18c0*)DAT_005292c4;
        size = 0;
        head->left = head;
        head->right = head;
        return ((Class_004e2240*)this)->FUN_004e2240();
    }

    void FUN_004e18c0();
};

// FUNCTION: 0x4e18c0
void Class_004e18c0::FUN_004e18c0()
{
    erase(begin(), end());
    changed = 1;
}

// The key: a C string ordered by strcmp.
class NameKey {
public:
    char* name;                        // +0x0
    bool FUN_004e1a30(const NameKey& other) const;
};

struct Value_004e2250 {
    char text[500];
};

typedef std::pair<const NameKey, Value_004e2250> Pair_004e2250;

// std::less<key>.
struct Less_004e2250 : public std::binary_function<NameKey, NameKey, bool> {
    bool operator()(const NameKey& _X, const NameKey& _Y) const
    {
        return (_X.FUN_004e1a30(_Y));
    }
};

// std::map<...>::_Kfn.
struct Kfn_004e2250 : public std::unary_function<Pair_004e2250, NameKey> {
    const NameKey& operator()(const Pair_004e2250& _X) const
    {
        return (_X.first);
    }
};

class Alloc_004e2b60 {
public:
    char* FUN_004e2b60(unsigned int n);
};

enum _Redbl_004e2250 { _Red, _Black };

struct Node_004e2250 {
    void* _Left;                       // +0x0
    void* _Parent;                     // +0x4
    void* _Right;                      // +0x8
    Pair_004e2250 _Value;              // +0xc
    _Redbl_004e2250 _Color;            // +0x204
};

typedef Node_004e2250* _Nodeptr;

// The members and static accessors of std::_Tree.
// Kept as the XTREE bodies and accessors: the inline budget follows their shape.
class Tree_004e2250 {
public:
    static _Redbl_004e2250& _Color(_Nodeptr _P)
        {return ((_Redbl_004e2250&)(*_P)._Color); }
    static const NameKey& _Key(_Nodeptr _P)
        {return (Kfn_004e2250()(_Value(_P))); }
    static _Nodeptr& _Left(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Right); }
    static Pair_004e2250& _Value(_Nodeptr _P)
        {return ((Pair_004e2250&)(*_P)._Value); }
    static _Nodeptr _Max(_Nodeptr _P)
        {std::_Lockit _Lk;
        while (_Right(_P) != DAT_005292c4)
            _P = _Right(_P);
        return (_P); }
    _Nodeptr& _Lmost()
        {return (_Left(_Head)); }
    _Nodeptr& _Rmost()
        {return (_Right(_Head)); }
    _Nodeptr& _Root()
        {return (_Parent(_Head)); }

    Alloc_004e2b60 allocator;          // +0x0
    Less_004e2250 key_compare;         // +0x1
    _Nodeptr _Head;                    // +0x4
    bool _Multi;                       // +0x8
    unsigned int _Size;                // +0xc
};

// std::_Tree<...>::iterator; _Dec is 0x4e2ab0.
class Class_004e2ab0 : public std::_Bidit<Pair_004e2250, int> {
public:
    Class_004e2ab0()
        {}
    Class_004e2ab0(_Nodeptr _P)
        : _Ptr(_P) {}
    Class_004e2ab0& operator--()
        {FUN_004e2ab0();
        return (*this); }
    bool operator==(const Class_004e2ab0& _X) const
        {return (_Ptr == _X._Ptr); }
    void FUN_004e2ab0()
        {std::_Lockit _Lk;
        if (Tree_004e2250::_Color(_Ptr) == _Red
            && Tree_004e2250::_Parent(Tree_004e2250::_Parent(_Ptr)) == _Ptr)
            _Ptr = Tree_004e2250::_Right(_Ptr);
        else if (Tree_004e2250::_Left(_Ptr) != DAT_005292c4)
            _Ptr = Tree_004e2250::_Max(Tree_004e2250::_Left(_Ptr));
        else
            {_Nodeptr _P;
            while (_Ptr == Tree_004e2250::_Left(_P = Tree_004e2250::_Parent(_Ptr)))
                _Ptr = _P;
            _Ptr = _P; }}
    _Nodeptr _Mynode() const
        {return (_Ptr); }
protected:
    _Nodeptr _Ptr;
};

// std::pair<iterator, bool>; its constructor is 0x4e2a10.
class Class_004e2a10 {
public:
    Class_004e2a10(const Class_004e2ab0& _V1, const bool& _V2)
        : first(_V1), second(_V2) {}
    Class_004e2ab0 first;
    bool second;
};

// _Rrotate (0x4e29b0).
class Class_004e29b0 : public Tree_004e2250 {
public:
    void FUN_004e29b0(_Nodeptr _X)
        {std::_Lockit _Lk;
        _Nodeptr _Y = _Left(_X);
        _Left(_X) = _Right(_Y);
        if (_Right(_Y) != DAT_005292c4)
            _Parent(_Right(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Right(_Parent(_X)))
            _Right(_Parent(_X)) = _Y;
        else
            _Left(_Parent(_X)) = _Y;
        _Right(_Y) = _X;
        _Parent(_X) = _Y; }
};

// _Lrotate (0x4e2950).
class Class_004e2950 : public Class_004e29b0 {
public:
    void FUN_004e2950(_Nodeptr _X)
        {std::_Lockit _Lk;
        _Nodeptr _Y = _Right(_X);
        _Right(_X) = _Left(_Y);
        if (_Left(_Y) != DAT_005292c4)
            _Parent(_Left(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Left(_Parent(_X)))
            _Left(_Parent(_X)) = _Y;
        else
            _Right(_Parent(_X)) = _Y;
        _Left(_Y) = _X;
        _Parent(_X) = _Y; }
};

// _Buynode (0x4e2a30).
class Class_004e2a30 : public Class_004e2950 {
public:
    _Nodeptr FUN_004e2a30(_Nodeptr _Parg, _Redbl_004e2250 _Carg)
        {_Nodeptr _S = (_Nodeptr)allocator.FUN_004e2b60(
            1 * sizeof (Node_004e2250));
        _Parent(_S) = _Parg;
        _Color(_S) = _Carg;
        return (_S); }
    void _Consval(Pair_004e2250* _P, const Pair_004e2250& _V)
        {std::_Construct(&*_P, _V); }
};

// _Insert (0x4e2620).
class Class_004e2620 : public Class_004e2a30 {
public:
    Class_004e2ab0 FUN_004e2620(_Nodeptr _X, _Nodeptr _Y, const Pair_004e2250& _V)
        {std::_Lockit _Lk;
        _Nodeptr _Z = FUN_004e2a30(_Y, _Red);
        _Left(_Z) = (_Nodeptr)DAT_005292c4, _Right(_Z) = (_Nodeptr)DAT_005292c4;
        _Consval(&_Value(_Z), _V);
        ++_Size;
        if (_Y == _Head || _X != DAT_005292c4
            || key_compare(Kfn_004e2250()(_V), _Key(_Y)))
            {_Left(_Y) = _Z;
            if (_Y == _Head)
                {_Root() = _Z;
                _Rmost() = _Z; }
            else if (_Y == _Lmost())
                _Lmost() = _Z; }
        else
            {_Right(_Y) = _Z;
            if (_Y == _Rmost())
                _Rmost() = _Z; }
        for (_X = _Z; _X != _Root()
            && _Color(_Parent(_X)) == _Red; )
            if (_Parent(_X) == _Left(_Parent(_Parent(_X))))
                {_Y = _Right(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                    {_Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X)); }
                else
                    {if (_X == _Right(_Parent(_X)))
                        {_X = _Parent(_X);
                        FUN_004e2950(_X); }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    FUN_004e29b0(_Parent(_Parent(_X))); }}
            else
                {_Y = _Left(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                    {_Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X)); }
                else
                    {if (_X == _Left(_Parent(_X)))
                        {_X = _Parent(_X);
                        FUN_004e29b0(_X); }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    FUN_004e2950(_Parent(_Parent(_X))); }}
        _Color(_Root()) = _Black;
        return (Class_004e2ab0(_Z)); }
};

// std::_Tree<...>::insert(const value_type&).
class Class_004e2250 : public Class_004e2620 {
public:
    Class_004e2ab0 begin()
        {return (Class_004e2ab0(_Lmost())); }
    Class_004e2a10 FUN_004e2250(const Pair_004e2250& _V);
};

class CritSec_004e1ac0 {
public:
    CRITICAL_SECTION cs;

    CritSec_004e1ac0() { InitializeCriticalSection(&cs); }
    ~CritSec_004e1ac0() {}
};

CritSec_004e1ac0* FUN_004e1ac0();

// 0x4e1990 (src/debug/debug_lib_4e1990.cpp) stays in its own file: this merge
// would make NameKey::FUN_004e1a30 (0x4e1a30, below) inlinable at its call
// site, and /Ob2 inlines it there, where the original calls it; 0x4e2250
// needs the same definition inlinable, so the two cannot share one file.

// FUNCTION: 0x4e1a30
bool NameKey::FUN_004e1a30(const NameKey& other) const
{
    return name != other.name && strcmp(name, other.name) < 0;
}

// The original calls this from GetNameTable rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1a80
void* __cdecl FUN_004e1a80(unsigned int size)
{
    return GlobalAlloc(0, size);
}
#pragma auto_inline(on)

// Lazily creates the global NameTable object, allocated with
// FUN_004e1a80 (a GlobalAlloc wrapper), and returns it. It is a placement
// new into that block: the constructor is 0x4e17c0.
extern NameTable* DAT_00529e7c;

// FUNCTION: 0x4e1a90
NameTable* GetNameTable()
{
    if (DAT_00529e7c == 0)
        DAT_00529e7c = new (FUN_004e1a80(sizeof(NameTable))) NameTable;
    return DAT_00529e7c;
}

// Returns a function-local static critical section, initialised on first
// use (same shape as 0x4da780). The empty inline destructor makes MSVC
// register the empty atexit thunk FUN_004e1b00.
// The original calls this from 0x4e1990 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1ac0
CritSec_004e1ac0* FUN_004e1ac0()
{
    static CritSec_004e1ac0 lock;
    return &lock;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e1b00
void FUN_004e1b00(void)
{
}

struct EventEntry {
    int id;                            // +0x0
    int unk4;                          // +0x4
    int unk8;                          // +0x8
    int unkc;                          // +0xc
};

extern EventEntry* DAT_00529df8;
extern int DAT_00529dcc;
extern EventEntry DAT_0050da00[];
extern EventEntry DAT_0050d980[];
extern EventEntry DAT_00529e00;        // "Event0"
extern EventEntry DAT_00529e10;        // "Event1"

class Class_004e2e20 {
public:
    void FUN_004e2e20(char* name, unsigned int* value, int minValue, int maxValue, int defaultValue);
};

extern unsigned char DAT_00529dd8;
extern unsigned char DAT_00529dd4;
extern unsigned char DAT_00529ddc;
extern unsigned char DAT_00529e64;
extern unsigned char DAT_00529dc8;

// Loads (or saves) the PerformanceSettings block of the Cavedog registry key.
// The original calls this from InitPerformanceEvents rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1b10
void __cdecl SyncPerformanceSettings(int readOnly)
{
    CavedogRegistryKey key(readOnly, "PerformanceSettings", "CavedogLibrary");
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("EnabledInRelease", &DAT_00529dd8, 0);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("RaisePriority", &DAT_00529dd4, 1);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("DisplayInDebugger", &DAT_00529ddc, 0);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("DisplayInWindow", &DAT_00529e64, 1);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("AutoPairing", &DAT_00529dc8, 1);
    ((Class_004e2e20*)&key)->FUN_004e2e20("Event0", (unsigned int*)&DAT_00529e00, 0, -1, 0);
    ((Class_004e2e20*)&key)->FUN_004e2e20("Event1", (unsigned int*)&DAT_00529e10, 0, -1, 0);
}
#pragma auto_inline(on)

void __cdecl SyncPerformanceSettings(int arg);
int GetCpuFamily(void);

// FUNCTION: 0x4e1be0
void InitPerformanceEvents(void)
{
    if (DAT_00529df8 == 0) {
        DAT_00529df8 = DAT_0050da00;
        DAT_00529dcc = 0x11;
        SyncPerformanceSettings(1);
        if (HasPerfCounters() != 0) {
            if (GetCpuFamily() < 6) {
                DAT_00529df8 = DAT_0050d980;
                DAT_00529dcc = 8;
            }
            EventEntry* table = DAT_00529df8;
            int count = DAT_00529dcc;
            // The pointer locals (declared in this order) make MSVC hoist the
            // two Event ids into the loop preheader in the original order.
            EventEntry* e1 = &DAT_00529e10;
            EventEntry* e0 = &DAT_00529e00;
            for (int i = 0; i < count; i++) {
                if (e0->id == table[i].id)
                    *e0 = table[i];
                if (e1->id == table[i].id)
                    *e1 = table[i];
            }
            if (DAT_00529e00.unk4 == 0)
                DAT_00529e00 = table[0];
            if (DAT_00529e10.unk4 == 0)
                DAT_00529e10 = table[0];
        }
    }
}

class Class_004e1d60 {
public:
    char unknown_0[0x40];
    int field_40;                      // +0x40
    int field_44;                      // +0x44
    char field_48;                     // +0x48
    char boosted;                      // +0x49
    char unknown_4a[2];
    DWORD oldPriorityClass;            // +0x4c
    int oldThreadPriority;             // +0x50

    void FUN_004e1d60(int a, int b);
};

class Class_004e20a0 {
public:
    double RestartTimer();
};

class Class_004e1e50 {
public:
    void ReportElapsedTime(char* text);
};

extern int DAT_00529dd0;
extern char DAT_00529e20[];

// While stopped, `time` holds the elapsed time; while running, it holds the
// start time.
class Timer {
public:
    double time;                       // +0x00
    char unknown_8[0x18 - 0x8];
    double history[5];                 // +0x18, the last five sample times
    char unknown_40[0x44 - 0x40];
    char* name;                        // +0x44
    char stopped;                      // +0x48
    char boosted;                      // +0x49
    char unknown_4a[2];
    DWORD oldPriorityClass;            // +0x4c
    int oldThreadPriority;             // +0x50

    ~Timer();
    Timer(int param_1);
    Timer(int a, int b);
    double GetElapsedSeconds();
    void ResumeTimer();
    void ResetTimer();
    void FUN_004e21a0(double delta);
    void FUN_004e21c0(double elapsed);
    double FUN_004e21f0();
};

// A timer object: starts timing (0x4e1d60, which saves and raises the thread
// priority) and discards a first elapsed-time reading (0x4e20a0).
// FUNCTION: 0x4e1d20 ??0Timer@@QAE@H@Z
Timer::Timer(int param_1)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(0, param_1);
    ((Class_004e20a0*)this)->RestartTimer();
}

// A second constructor of the timer object of 0x4e1d20: starts timing
// (0x4e1d60) with both values given, without the first reading.
// FUNCTION: 0x4e1d40 ??0Timer@@QAE@HH@Z
Timer::Timer(int a, int b)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(a, b);
}

// The original calls this from the two Timer constructors rather than
// inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1d60
void Class_004e1d60::FUN_004e1d60(int a, int b)
{
    field_40 = b;
    field_44 = a;
    DAT_00529e20[DAT_00529dd0++] = 9;
    boosted = DAT_00529dd4;
    if (boosted) {
        HANDLE thread = GetCurrentThread();
        oldThreadPriority = GetThreadPriority(thread);
        SetThreadPriority(thread, THREAD_PRIORITY_ABOVE_NORMAL);
        HANDLE process = GetCurrentProcess();
        oldPriorityClass = GetPriorityClass(process);
        SetPriorityClass(process, HIGH_PRIORITY_CLASS);
    }
    ((Class_004e20a0*)this)->RestartTimer();
}
#pragma auto_inline(on)

// The timer's counterpart to 0x4e1d60: pops the profiling nesting level,
// reports the elapsed time when the timer has a name (the hand-written
// routine at 0x4e1e50), and restores the process and thread priorities that
// 0x4e1d60 saved before raising them.
// FUNCTION: 0x4e1de0
Timer::~Timer()
{
    DAT_00529e20[--DAT_00529dd0] = 0;
    if (name) {
        ((Class_004e1e50*)this)->ReportElapsedTime(0);
    }
    if (boosted) {
        HANDLE process = GetCurrentProcess();
        SetPriorityClass(process, oldPriorityClass);
        HANDLE thread = GetCurrentThread();
        SetThreadPriority(thread, oldThreadPriority);
    }
}

// Timer: while stopped (flag at +0x48) the double at +0 holds the elapsed
// time; while running it holds the start time and the elapsed time is the
// current time (GetTimeSeconds) minus it. Callers set ecx to the timer
// (0x4e1eda, 0x4e2150), so this is a method.
// The original calls this from 0x4e2150 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e1e30
double Timer::GetElapsedSeconds()
{
    if (stopped) {
        return time;
    }
    return GetTimeSeconds() - time;
}
#pragma auto_inline(on)

// 0x4e1e50 and 0x4e20a0 (src/debug/debug_lib_4e1e50.cpp) stay in their own
// file: it is the source of a gap region and uses inline rdpmc.

// The timer's elapsed-time getter (0x4e1e30), a method on the same object.
struct Class_004e2150 {
public:
    double field_0;
    char unknown_8[0x40];
    unsigned char field_48;
    
    void StopTimer();
};

// FUNCTION: 0x4e2150
void Class_004e2150::StopTimer()
{
    field_0 = ((Timer*)this)->GetElapsedSeconds();
    field_48 = 1;
}

// FUNCTION: 0x4e2160
void Timer::ResumeTimer()
{
    if (stopped != 0) {
        double now = GetTimeSeconds();
        time = now - time;
        stopped = 0;
    }
}

// FUNCTION: 0x4e2180
void Timer::ResetTimer()
{
    time = 0;
    stopped = 1;
}

// FUNCTION: 0x4e21a0
void Timer::FUN_004e21a0(double delta)
{
    if (stopped) {
        time -= delta;
    } else {
        // Without /Op the float conversion emits nothing, but it makes MSVC
        // load delta first (fld delta; fadd time) instead of hoisting a shared
        // `fld time` above the branch.
        time += (float)delta;
    }
}

// FUNCTION: 0x4e21c0
void Timer::FUN_004e21c0(double elapsed)
{
    if (stopped) {
        time = elapsed;
    } else {
        time = GetTimeSeconds() - elapsed;
    }
}

// 0x4e21f0 (src/debug/debug_lib_4e21f0.cpp) stays in its own file: its 0.0
// and 5.0 constants sit in a different constant pool from the memory status
// dialog's 0.0, so the original had it in another translation unit.


// The original calls this from 0x4e18c0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4e2240
Iter_004e18c0 Class_004e2240::FUN_004e2240()
{
    return head->left;
}
#pragma auto_inline(on)

// FUNCTION: 0x4e2250
Class_004e2a10 Class_004e2250::FUN_004e2250(const Pair_004e2250& _V)
{
    _Nodeptr _X = _Root();
    _Nodeptr _Y = _Head;
    bool _Ans = true;
    {
        std::_Lockit Lk;
        while (_X != DAT_005292c4) {
            _Y = _X;
            _Ans = key_compare(Kfn_004e2250()(_V), _Key(_X));
            _X = _Ans ? _Left(_X) : _Right(_X);
        }
    }
    if (_Multi)
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    Class_004e2ab0 _P = Class_004e2ab0(_Y);
    if (!_Ans)
        ;
    else if (_P == begin())
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    else
        --_P;
    if (key_compare(_Key(_P._Mynode()), Kfn_004e2250()(_V)))
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    return (Class_004e2a10(_P, false));
}

// 0x4e2580 (src/debug/debug_lib_4e2580.cpp) stays in its own file: its view
// of Class_004e2580 is keyed by const char* and returns Iter_004e2580, which
// cannot be one class with the NameKey-keyed view the tree methods above use.
// 0x4e2620 (src/debug/debug_lib_4e2620.cpp) stays in its own file: its
// Class_004e2620 carries the tree's fields and its own _Lrotate/_Rrotate,
// while 0x4e2250's Class_004e2620 inherits them from the XTREE chain above.
