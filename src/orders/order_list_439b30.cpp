// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5. Names are provisional.
// The list-walking dispatcher of the unit: for every object linked into the unit's
// list at +0x5c (link field at +0x4a, kind byte at +0x4) it looks up the kind's
// default flags in the table at DAT_00512344 (0x19-byte entries, the flags dword
// at +0xc, indexed by the kind byte) and, masked with the `mask` parameter, calls
// one of five helpers per bit: bit 0 -> 0x438c00, bit 1 -> 0x4394e0, bit 2 ->
// 0x4399f0, bit 3 -> 0x439740, bit 4 -> 0x4390a0. Bit 4 is only acted on for the
// first object of the list (`done`). Every helper is __stdcall with five dword
// arguments and takes the position as a pointer to a 12-byte object.

#pragma pack(push, 1)
struct Entry_00439b30 {
    char unknown_0[0xc];
    unsigned int flags;
    char unknown_10[0x19 - 0x10];
};

// Three plain ints, not the x_frac/x pair layout of 0x4399f0.
struct Pos_00439b30 {
    int x;
    int y;
    int z;
};

struct Obj_00439b30 {
    char unknown_0[4];
    unsigned char kind;
    char unknown_5[0x4a - 5];
    Obj_00439b30* next;
};

struct Unit {
    char unknown_0[0x5c];
    Obj_00439b30* first;
    char unknown_60[0x6a - 0x60];
    Pos_00439b30 pos;
};
#pragma pack(pop)

extern Entry_00439b30* DAT_00512344;

void __stdcall DrawBuildFootprint(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall DrawUnitRangeRings(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall FUN_004394e0(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall DrawWeaponCoverage(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall FUN_004399f0(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);

// FUNCTION: 0x439b30
void __stdcall FUN_00439b30(Unit* unit, unsigned int mask, void* obj,
                            void* sel, int flag)
{
    // Two copies, base first: each call but bit 4's is preceded by pos = base,
    // and the loop ends with base = pos.
    Pos_00439b30 base = unit->pos;
    Pos_00439b30 pos = unit->pos;
    bool done = false;
    for (Obj_00439b30* e = unit->first; e != 0; e = e->next) {
        if (DAT_00512344[e->kind].flags & mask & 1) {
            pos = base;
            DrawBuildFootprint(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].flags & mask & 2) {
            pos = base;
            FUN_004394e0(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].flags & mask & 4) {
            pos = base;
            FUN_004399f0(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].flags & mask & 8) {
            pos = base;
            DrawWeaponCoverage(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].flags & mask & 0x10) {
            if (!done) {
                DrawUnitRangeRings(obj, sel, e, &pos, flag);
                done = true;
            }
        }
        base = pos;
    }
}
