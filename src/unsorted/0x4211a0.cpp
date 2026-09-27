// Decompiled by Opus. Names are provisional.
// Passes every entry of the 100-slot global table (see FUN_00421150) to
// FUN_00421550 with the argument and clears the slots it reports finished
// (compare 0x421170).

struct Obj_00421170;

extern Obj_00421170* DAT_00511df0[];

int __stdcall FUN_00421550(int param_1, Obj_00421170* obj);

// FUNCTION: 0x4211a0
void __stdcall FUN_004211a0(int param_1)
{
    for (int i = 0; i < 100; i++) {
        if (DAT_00511df0[i] != 0 && FUN_00421550(param_1, DAT_00511df0[i]) == 0) {
            DAT_00511df0[i] = 0;
        }
    }
}
