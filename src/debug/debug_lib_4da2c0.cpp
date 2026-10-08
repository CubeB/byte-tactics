// Decompiled by Claude Opus 5.5. Names are provisional.
// FLAGS: /Od /Gy
//
// The debug thread InitDebugSupport starts.
#include <windows.h>

void StartMemoryStatus(void);
void StartPerformanceStatus(void);
void __cdecl InitDebugSupport(unsigned int flags);

extern unsigned char g_debugThreadRunning;  // the debug thread is running

void __cdecl RunDebugThread(void* param);
BOOL __cdecl HandleDialogMessage(MSG* msg);

// FUNCTION: 0x4da2c0
DWORD __stdcall DebugThreadProc(void* param)
{
    RunDebugThread(param);
    return 0;
}

// Sets up the debug windows at raised priority, says it is running, then
// pumps this thread's messages until WM_QUIT.
// FUNCTION: 0x4da2e0
void __cdecl RunDebugThread(void* param)
{
    // Frame layout follows these local names (hThread, ret, message, priority), not declaration order.
    HANDLE hThread = GetCurrentThread();
    int ret;
    MSG message;
    int priority = GetThreadPriority(hThread);
    SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
    StartMemoryStatus();
    StartPerformanceStatus();
    SetThreadPriority(hThread, priority);
    g_debugThreadRunning = 1;
    while ((ret = GetMessageA(&message, 0, 0, 0)) != 0) {
        if (ret == -1) {
        } else if (!HandleDialogMessage(&message)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
}

// Lets the dialog at the top of msg's window chain handle it.
// FUNCTION: 0x4da380
BOOL __cdecl HandleDialogMessage(MSG* msg)
{
    // Local names set the frame layout, as in RunDebugThread.
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
    InitDebugSupport(0);
}
