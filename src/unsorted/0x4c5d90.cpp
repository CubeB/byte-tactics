// Decompiled by Opus. Names are provisional.
// Records a surface and flag in the top slot of the screen lock stack
// (count DAT_0051fe00, at most 10 entries at DAT_0051fe08).

struct Surface_004c5d90;

#pragma pack(push, 1)
struct LockEntry_004c5d90 {
    Surface_004c5d90* surface;         // +0x0
    char flag;                         // +0x4
};
#pragma pack(pop)

extern int DAT_0051fe00;
extern LockEntry_004c5d90 DAT_0051fe08[];

// FUNCTION: 0x4c5d90
void __stdcall FUN_004c5d90(Surface_004c5d90* surface, char flag)
{
    if (DAT_0051fe00 < 10) {
        DAT_0051fe08[DAT_0051fe00].surface = surface;
        DAT_0051fe08[DAT_0051fe00].flag = flag;
    }
}
