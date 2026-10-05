// Decompiled by Opus. Names are provisional.
// Stores `value` at +0x1b2 of the object GetDisplay returns while holding
// the 'MAIN' spin lock (DAT_0052a4e8, owner tag DAT_0052a4ec, event
// DAT_0052a4f0); a lock already held by 'MAIN' is not released here.
#include <windows.h>

#pragma pack(push, 1)
struct State_004c2b20 {
    char unknown_0[0x1b2];
    int field_1b2;                     // +0x1b2
};
#pragma pack(pop)

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;

State_004c2b20* GetDisplay(void);

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

// FUNCTION: 0x4c2b20
void __stdcall FUN_004c2b20(int value)
{
    LONG held = Lock();
    GetDisplay()->field_1b2 = value;
    Unlock(held);
}
