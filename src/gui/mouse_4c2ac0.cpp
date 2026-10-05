// Decompiled by Opus. Names are provisional.

void __stdcall FUN_004b6b50(unsigned int param_1);

#pragma pack(push, 1)
struct Struct_004c2ac0 {
    char unknown_0[0x1ca];
    int active;                     // +0x1ca
    char unknown_1ce[0x8];
    int pending;                    // +0x1d6
};
#pragma pack(pop)

// FUNCTION: 0x4c2ac0
int __stdcall StopMouseThread(Struct_004c2ac0* s)
{
    if (s->active == 0)
        return 1;
    s->pending = 1;
    int tries = 0;
    while (s->pending != 0) {
        if (++tries > 20)
            return 0;
        FUN_004b6b50(100);
    }
    s->active = 0;
    return 1;
}
