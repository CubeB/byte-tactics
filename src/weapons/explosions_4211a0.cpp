// Decompiled by Opus. Names are provisional.
// Passes every entry of the 100-slot global table (see FindFreeExplodedPieceSlot) to
// DrawExplodedPiece with the argument and clears the slots it reports finished
// (compare 0x421170).

struct Obj_00421170;

extern Obj_00421170* DAT_00511df0[];

int __stdcall DrawExplodedPiece(int param_1, Obj_00421170* obj);

// FUNCTION: 0x4211a0
void __stdcall DrawExplodedPieces(int param_1)
{
    for (int i = 0; i < 100; i++) {
        if (DAT_00511df0[i] != 0 && DrawExplodedPiece(param_1, DAT_00511df0[i]) == 0) {
            DAT_00511df0[i] = 0;
        }
    }
}
