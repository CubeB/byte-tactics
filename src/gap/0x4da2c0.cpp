// Decompiled by Claude Opus 5.5. Names are provisional.
// FLAGS: /Od /Gy
//
// The debug thread FUN_004da1d0 starts, compiled without optimisation like
// the rest of Cavedog's debug helpers.
#include <windows.h>

void FUN_004e1460(void);
void FUN_004dfe80(void);
void __cdecl FUN_004da1d0(unsigned int flags);

extern unsigned char DAT_005289c8;      // the debug thread is running

void __cdecl FUN_004da2e0(void* param);
BOOL __cdecl FUN_004da380(MSG* msg);

// FUNCTION: 0x4da2c0
DWORD __stdcall FUN_004da2c0(void* param)
{
    FUN_004da2e0(param);
    return 0;
}

// Sets up the debug windows at raised priority, says it is running, then
// pumps this thread's messages until WM_QUIT.
//
// Without optimisation the compiler lays the locals out in the order of its
// symbol table, which follows their names rather than their declarations:
// these four names give the original's frame (hThread at -4, ret at -8,
// message at -0x24, priority at -0x28); `thread` / `result` do not.
// FUNCTION: 0x4da2e0
void __cdecl FUN_004da2e0(void* param)
{
    HANDLE hThread = GetCurrentThread();
    int ret;
    MSG message;
    int priority = GetThreadPriority(hThread);
    SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
    FUN_004e1460();
    FUN_004dfe80();
    SetThreadPriority(hThread, priority);
    DAT_005289c8 = 1;
    while ((ret = GetMessageA(&message, 0, 0, 0)) != 0) {
        if (ret == -1) {
        } else if (!FUN_004da380(&message)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
}

// Lets the dialog at the top of msg's window chain handle it (the names give
// the original's frame, as in FUN_004da2e0).
// FUNCTION: 0x4da380
BOOL __cdecl FUN_004da380(MSG* msg)
{
    HWND root;
    HWND parent = msg->hwnd;
    root = parent;
    while (parent) {
        root = parent;
        parent = GetParent(parent);
    }
    if (root && IsDialogMessageA(root, msg))
        return 1;
    return 0;
}

// FUNCTION: 0x4da3e0
void __cdecl FUN_004da3e0(void)
{
    FUN_004da1d0(0);
}
