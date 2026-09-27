// Decompiled by Sonnet. Names are provisional.
//
// This constructor stores the address 0x4fc9b0, which is exactly 8 bytes
// into the vtable that ctx.py (and data/symbols.csv) calls
// "??_7Class_00408810@@6B@" (0x4fc9a8, used by src/unsorted/0x4085d0.cpp's
// Class_00408810::Class_00408810). The two entries actually stored there
// (FUN_004086d0, Class_00408810::FUN_00408810) are the tail two of that same
// four-entry table, so this looks like it should be a distinct class that
// happens to reuse the last two virtual slots of "Class_00408810" (perhaps a
// class that shares them via inheritance, with the earlier agent's naming of
// the primary vtable-bearing class as "Class_00408810" leaving no name free
// for this second, two-entry table). Modelling it as a real virtual class
// with a matching field layout reproduces the target bytes exactly (down to
// the interleaved vtable-store position, which a hand-assigned vtable
// pointer could not reproduce), but the compiler then emits its own
// "??_7Class_00408810@@6B@" for this class's two-entry vtable, colliding
// with the different, already-established four-entry vtable at 0x4fc9a8
// that data/symbols.csv binds to that exact mangled name. I could not find
// a class layout that both reproduces these bytes and avoids that name
// collision (real multiple inheritance from a shared base needs a second,
// separate vtable-pointer store that the original does not have, and a
// hand-rolled/non-virtual vtable field cannot reproduce the interleaved
// store order). This is my best (byte-identical) attempt; the only
// remaining mismatch is that reference, which I believe is a case where the
// existing "Class_00408810" name from 0x4085d0.cpp is being reused for what
// is actually a second, different vtable.

class Class_00408810 {
public:
    virtual void FUN_004086d0();
    virtual void* FUN_00408810(unsigned char flag);

    int field_4;
    int field_8;
    int field_c;
    unsigned int field_10;

    Class_00408810(int param_1, int param_2);
};

// FUNCTION: 0x4087e0
Class_00408810::Class_00408810(int param_1, int param_2)
    : field_4(param_1), field_8(param_2), field_c(0), field_10(*(unsigned char*)(param_1 + 4))
{
}
