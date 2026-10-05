// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Target_004030a0 {
    char unknown_0[0x245];
    unsigned int lowbits : 13;
    unsigned int flag : 1;               // +0x245, bit 13
};
#pragma pack(pop)

// FUNCTION: 0x4030a0
int __stdcall FUN_004030a0(char* unit, int unused1, int unused2)
{
    Target_004030a0* p = *(Target_004030a0**)(unit + 0x92);
    if (p->flag) {
        *(unsigned int*)(unit + 0x110) &= ~0x800;
    }
    return 5;
}
