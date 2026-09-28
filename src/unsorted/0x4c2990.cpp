// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Spins on the 'SUOM' lock tag (DAT_0052a4e8, owner tag DAT_0052a4ec, event
// DAT_0052a4f0) while field_1d6 of the object is zero; releases the lock each
// pass and calls FUN_004c25e0 when field_1b2 is set, then clears field_1d6.
#include <windows.h>

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;

void __stdcall FUN_004c25e0(int param);

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&DAT_0052a4e8, 0x4d4f5553);
        if (r == 0) {
            DAT_0052a4ec = 0x4d4f5553;
            return 0;
        }
        if (DAT_0052a4ec == 0x4d4f5553)
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

// FUNCTION: 0x4c2990
void __cdecl FUN_004c2990(int param)
{
    SetThreadPriority(GetCurrentThread(), 2);
    while (*(int*)(param + 0x1d6) == 0) {
        int start = GetTickCount() + 0x21;
        LONG held = Lock();
        if (*(int*)(param + 0x1b2) != 0)
            FUN_004c25e0(param);
        Unlock(held);
        Sleep(max(1, start - (int)GetTickCount()));
    }
    *(int*)(param + 0x1d6) = 0;
}
