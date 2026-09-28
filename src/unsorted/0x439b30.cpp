// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH (29 percent). Best of about a dozen attempts; the notes say
// exactly what still differs and which experiments were tried.
//
// What the function does: for every object linked into the unit's list at
// +0x5c (link field at +0x4a, kind byte at +0x4) it looks up the kind's
// default flags in the table at DAT_00512344 (0x14-byte entries, the flags
// dword at +0xc, indexed by the kind byte) and, masked with the `mask`
// parameter, calls one of five helpers per bit: bit 0 -> 0x438c00, bit 1 ->
// 0x4394e0, bit 2 -> 0x4399f0, bit 3 -> 0x439740, bit 4 -> 0x4390a0. Bit 4 is
// only acted on for the first object of the list, which a byte local (in the
// dead `unit` parameter's slot) remembers. Every helper is __stdcall with five
// dword arguments and takes the position as a pointer to a 12-byte object it
// writes through (see the matched 0x4399f0, whose 4th parameter is the same
// out-position and whose body ends in `*out = p`); all five callees pop 0x14,
// so that 4th argument is one dword, not a by-value struct.
//
// What still differs: the original keeps the position copy in the callee-saved
// registers edi, ebp, ebx (loaded from unit->pos at +0x6a before the loop test)
// and uses the frame slot at [esp+0x10] as a shadow of it: it re-stores the
// three dwords from those registers just before four of the five calls
// (`mov [esp+0x24],edi / +0x28,ebp / +0x2c,ebx`, after the five pushes, so
// they land on the frame slot), reloads them from the slot at the bottom of
// the loop, and skips the re-store for the bit-4 call. With those three
// registers occupied, obj, sel and flag are re-read from their parameter slots
// in every block instead of being held in registers.
//
// MSVC 5 only promotes the aggregate into registers when the source uses it
// by value: passing `pos` by value to the five helpers (plain struct, or a
// class with a destructor) does give the edi/ebp/ebx copy with the same push
// order and the same interleaving of the `done` store, but MSVC then
// materialises the by-value copy in the outgoing argument area
// (`sub esp,0xc; mov eax,esp; mov [eax],edi ...`), so the helper pops 7
// dwords, and the original's helpers pop 5. Declaring the 4th parameter as a
// pointer (or a reference) is what makes the call shape right, and then MSVC
// keeps the position in memory only (no register copy, no shadow, no reload),
// which is what this file compiles to: 308 bytes against the original's 441.
//
// Tried and rejected: pos as a class with an inline copy constructor (v16, the
// copy constructor is not inlined, it becomes a real call), pos re-assigned
// from unit->pos inside each guarded block (MSVC 5 does not hoist the
// invariant load, it spills &unit->pos and re-reads through it), pos declared
// inside the loop body, a second register-only copy of the position with
// `pos = base` before each call, a (void) cast of the local, the flag lookup
// through a static inline helper, and every single-header prefix
// (windows.h, stdio.h, stdlib.h, string.h, math.h, memory.h, ddraw.h,
// string, vector, map, list, iostream): all of them leave the position in
// memory and score the same 29 percent.

#pragma pack(push, 1)
struct Entry_00439b30 {              // 0x14-byte entries, table at DAT_00512344
    char unknown_0[0xc];
    unsigned int flags;             // +0xc, default flags of the kind
    char unknown_10[4];
};

// The 12-byte position, the same layout as Pos_4399f0 in the matched 0x4399f0.
struct Pos_00439b30 {
    unsigned short x_frac;           // +0x0
    short x;                         // +0x2
    unsigned short y_frac;           // +0x4
    short y;                         // +0x6
    unsigned short z_frac;           // +0x8
    short z;                         // +0xa
};

struct Obj_00439b30 {
    char unknown_0[4];
    unsigned char kind;             // +0x4
    char unknown_5[0x4a - 5];
    Obj_00439b30* next;             // +0x4a
};

struct Unit_00439b30 {
    char unknown_0[0x5c];
    Obj_00439b30* first;            // +0x5c
    char unknown_60[0x6a - 0x60];
    Pos_00439b30 pos;               // +0x6a
};
#pragma pack(pop)

extern Entry_00439b30* DAT_00512344;

void __stdcall FUN_00438c00(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall FUN_004390a0(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall FUN_004394e0(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall FUN_00439740(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);
void __stdcall FUN_004399f0(void* a, void* b, Obj_00439b30* e, Pos_00439b30* p, int c);

// FUNCTION: 0x439b30
void __stdcall FUN_00439b30(Unit_00439b30* unit, unsigned int mask, void* obj,
                            void* sel, int flag)
{
    Pos_00439b30 pos = unit->pos;
    bool done = false;
    for (Obj_00439b30* e = unit->first; e != 0; e = e->next) {
        if (DAT_00512344[e->kind].flags & mask & 1)
            FUN_00438c00(obj, sel, e, &pos, flag);
        if (DAT_00512344[e->kind].flags & mask & 2)
            FUN_004394e0(obj, sel, e, &pos, flag);
        if (DAT_00512344[e->kind].flags & mask & 4)
            FUN_004399f0(obj, sel, e, &pos, flag);
        if (DAT_00512344[e->kind].flags & mask & 8)
            FUN_00439740(obj, sel, e, &pos, flag);
        if (DAT_00512344[e->kind].flags & mask & 0x10) {
            if (!done) {
                FUN_004390a0(obj, sel, e, &pos, flag);
                done = true;
            }
        }
    }
}
