// Decompiled by Haiku. Names are provisional.

extern int g_screenLockCount;

// FUNCTION: 0x4c5dc0
void __stdcall PopScreenLock(int param_1, int param_2)
{
    if (g_screenLockCount > 0) {
        g_screenLockCount--;
    }
}
