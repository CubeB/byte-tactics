// Decompiled by Opus. Names are provisional.
// Updates every entry of the 100-slot global table (see FUN_00421150) with
// FUN_004213b0 and clears the slots it reports finished.

struct Obj_00421170;

extern Obj_00421170* DAT_00511df0[];

int __stdcall FUN_004213b0(Obj_00421170* obj);

// FUNCTION: 0x421170
void FUN_00421170()
{
    for (int i = 0; i < 100; i++) {
        if (DAT_00511df0[i] != 0 && FUN_004213b0(DAT_00511df0[i]) == 0) {
            DAT_00511df0[i] = 0;
        }
    }
}
