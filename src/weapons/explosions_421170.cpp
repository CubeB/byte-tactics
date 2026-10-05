// Decompiled by Opus. Names are provisional.
// Updates every entry of the 100-slot global table (see FindFreeExplodedPieceSlot) with
// UpdateExplodedPiece and clears the slots it reports finished.

struct Obj_00421170;

extern Obj_00421170* DAT_00511df0[];

int __stdcall UpdateExplodedPiece(Obj_00421170* obj);

// FUNCTION: 0x421170
void UpdateExplodedPieces()
{
    for (int i = 0; i < 100; i++) {
        if (DAT_00511df0[i] != 0 && UpdateExplodedPiece(DAT_00511df0[i]) == 0) {
            DAT_00511df0[i] = 0;
        }
    }
}
