// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Takes the 'MAIN' spin lock (DAT_0052a4e8, owner tag DAT_0052a4ec, event
// DAT_0052a4f0), clears field_1b2 of the app object, pops the screen lock
// stack and releases the display, then restores the window to non-topmost.
#include <windows.h>

#pragma pack(push, 1)
struct App_004cbab0 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    char unknown_44[0x1b2 - 0x44];
    int field_1b2;                     // +0x1b2
};
#pragma pack(pop)

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;

App_004cbab0* FUN_004b6220(void);
void __cdecl FUN_004d8e60(int param_1, int param_2);
void __stdcall FUN_004b4ff0(App_004cbab0* app);
void FUN_004c5df0(void);

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (r == 0) {
            DAT_0052a4ec = 0x4d41494e;
            return 0;
        }
        if (DAT_0052a4ec == 0x4d41494e)
            return r;
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
}

static inline void Unlock(LONG held)
{
    if (held == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
}

// FUNCTION: 0x4cbab0
int __stdcall FUN_004cbab0(int param_1, int param_2)
{
    FUN_004d8e60(param_1, param_2);
    App_004cbab0* app = FUN_004b6220();
    if (app != 0) {
        LONG held = Lock();
        app->field_1b2 = 0;
        FUN_004c5df0();
        Unlock(held);
        FUN_004b4ff0(app);
        if (app->hwnd != 0) {
            SetWindowPos(app->hwnd, (HWND)-2, 0, 0, 0, 0, 0x13);
        }
    }
    return 0;
}
