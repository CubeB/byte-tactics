// Decompiled by Opus. Names are provisional.
// Records a surface and flag in the top slot of the screen lock stack
// (count g_screenLockCount, at most 10 entries at g_screenLocks).

struct Surface_004c5d90;

#pragma pack(push, 1)
struct LockEntry_004c5d90 {
    Surface_004c5d90* surface;         // +0x0
    char flag;                         // +0x4
};
#pragma pack(pop)

extern int g_screenLockCount;
extern LockEntry_004c5d90 g_screenLocks[];

// FUNCTION: 0x4c5d90
void __stdcall SetLockEntry(Surface_004c5d90* surface, char flag)
{
    if (g_screenLockCount < 10) {
        g_screenLocks[g_screenLockCount].surface = surface;
        g_screenLocks[g_screenLockCount].flag = flag;
    }
}
