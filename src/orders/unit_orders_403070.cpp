// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Target {
    char pad[0x245];
    unsigned int lowbits : 13;
    unsigned int flag : 1;
};
#pragma pack(pop)

// FUNCTION: 0x403070
int __stdcall FUN_00403070(char* param1, int unused1, int unused2)
{
    Target* p = *(Target**)(param1 + 0x92);
    if (p->flag) {
        *(unsigned int*)(param1 + 0x110) |= 0x800;
    }
    return 5;
}
