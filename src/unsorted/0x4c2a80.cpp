// Decompiled by Opus. Names are provisional.
// Starts the worker thread (0x4c2990) that 0x4c2ac0 stops.

void __cdecl FUN_004c2990(void* param_1);
int __stdcall FUN_004b6b20(void* param_1, unsigned int param_2, void* param_3);

#pragma pack(push, 1)
struct Struct_004c2ac0 {
    char unknown_0[0x1ca];
    int active;                     // +0x1ca
    int running;                    // +0x1ce
    char unknown_1d2[0x4];
    int pending;                    // +0x1d6
};
#pragma pack(pop)

// FUNCTION: 0x4c2a80
int __stdcall FUN_004c2a80(Struct_004c2ac0* s)
{
    s->pending = 0;
    s->active = FUN_004b6b20((void*)FUN_004c2990, 0x8000, s);
    if (s->active) {
        s->running = 1;
        return 1;
    }
    return 0;
}
