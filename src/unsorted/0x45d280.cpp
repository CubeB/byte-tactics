// Decompiled by space-bunny-free. Names are provisional.
// The CD-options menu handler (the sibling of 0x45da90, the sound-options
// handler, which has the same shape): a chain of "command name" tests that
// drives the CD player, the track-mode page and the volume/undo buttons, with
// a shared tail for the CD arms and another for the UNDO/RESTORE arms.
//
// 75.3 percent (1312 of 1333 bytes), 2 real check.py runs, rest scored with
// `check.py --sym` on build/scratch/0x45d280/v*.cpp. NOT MATCH. What still
// differs, block by block, is at the bottom of this comment.
//
// What moved the number most:
//  - The else-if chain is TWO statements, not one. `if (NOTRAK) ... else if
//    (TRACKMODE) ... else if (TRACKTYPE) ...` and then, separately, `if
//    (CDPLAY) ... else if (CDNEXT) ... else if (CDPREV) ... else if (CDSTOP)
//    ...`, then `if (UNDO)`, `if (RESTORE)`, then the fall-through block.
//    MSVC 5 lays a single chain out with its whole else-arm out of line and
//    the LAST arm inline; the original has the NOTRAK body falling straight
//    into the CDPLAY test with only TRACKMODE and TRACKTYPE out of line,
//    which is exactly what two chains give (26 points).
//  - The shared tails are `goto` labels placed at the END of the last arm of
//    their chain (cd_tail inside the CDSTOP arm, apply inside the RESTORE
//    arm). With the tail written out three times instead, MSVC merges blocks
//    and the arms lose 3 points.
//  - `g_game->flags.word = f ^ (((unsigned char)f ^ DAT_00512f46) & 1)`:
//    the explicit byte cast is what makes the inner xor an 8-bit xor and
//    stops MSVC rewriting the whole thing as a bitfield test-and-set (3
//    points), and DAT_00512f46/48 must be `unsigned char` (2 points).
//  - The two bit tests have to be `g_game->prefs` on an `unsigned char`
//    bitfield (a word bitfield gives `test word`, not `test byte`) and
//    `g_game->loaded` on an `unsigned short` bitfield (the store is
//    `and word [...], 0xfffe`).
//
// Still different, and what I tried:
//  1. Every `entries += i; ... entries[i]` site loses the original's
//     `add ebp, edi` (three sites: 0x45d611, 0x45d714, 0x45d777) and with it
//     the `lea eax,[edi*8]` that MSVC only emits when the base pointer is
//     live in its own register. MSVC 5 folds `(base + i)[i]` to `base[i]`
//     here, so the address it computes is one element lower than the
//     original's. Tried and did NOT help: making the pointer a `char*` and
//     casting at the use; a fresh local `Entry* e = entries + i` (adds a
//     load, and the original's block has none); a separate `int k = i` for
//     the index; `entries = entries + i` instead of `+=`.
//  2. The UNDO flag update is 5 instructions from the original: the original
//     keeps `f` in ax and the mask in ecx (`and ecx,1 / xor ecx,eax`), mine
//     materialises a 16-bit mask (`and dl,1 / movzx dx,dl / xor edx,ecx`).
//     MSVC keeps rewriting `f ^ ((f ^ x) & 1)` as "clear bit 0, then set it
//     if (f ^ x) & 1" because the test just above computed the same value.
//     Tried: reading the byte into a local first, and a local `x`; neither
//     stops the rewrite.
//  3. The CD arms. The original has `push <arg>; mov ecx,[g_game+0x10]; jmp
//     tail` in each arm with the shared block starting at the `call`, so the
//     argument dies in each arm. With one `goto` tail the argument is a phi,
//     and MSVC copies it into the shared block (`mov eax,ecx`) and shares the
//     "value 1" block between CDNEXT's clamp and CDSTOP, which the original
//     does not do. Tried: a `track` local assigned in all three arms (that
//     one fixes the clamp's shape but costs the copy at the merge, 70.0),
//     and three separate copies of the tail for MSVC to merge (71.7). The
//     `goto` form is the best of the three.
//  4. Register choices only: g_game in eax vs ecx/edx, `mode` in al vs cl,
//     and the two volume calls' `shl`. The first of these is in the CDPLAY
//     arm, the first block in the function whose allocation differs, so
//     whatever causes it is upstream of all the others. Removing the `mode`
//     local (one fewer live node) changed nothing, so it is not simply a
//     live-range-count problem.
#pragma pack(push, 1)

struct Entry_0045d280 {              // 0x15b-byte gadget entry
    char state;                      // +0x00
    char unknown_1[0x137 - 1];
    unsigned char value;             // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Vtable_0045d280 {
    char unknown_0[8];
    void (__stdcall* FUN_8)(void* obj);
};

struct Holder_0045d280 {
    Vtable_0045d280* field_0;        // +0x00
    Entry_0045d280* entries;         // +0x04
};

struct Object_0045d280 {
    char unknown_0[0x18];
    Holder_0045d280* holder;          // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};

struct Menu_0045d280 {
    char unknown_0[0x18];
    Holder_0045d280* holder;          // +0x18
};

struct Bits_0045d280 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short bits_2_15 : 14;
};

union Flags_0045d280 {
    unsigned short word;
    unsigned char byte;
    Bits_0045d280 bits;
};

struct Game_0045d280 {
    char unknown_0[0x10];
    void* sound;                     // +0x10
    char unknown_14[0x531 - 0x14];
    void* table_531;                 // +0x531
    char unknown_535[0x2a44 - 0x535];
    unsigned char b0_2a44 : 1;       // +0x2a44
    unsigned char b1_2a44 : 1;
    unsigned char prefs : 1;         // bit 2
    unsigned char rest_2a44 : 5;
    char unknown_2a45[0x37ebe - 0x2a45];
    unsigned short loaded : 1;       // +0x37ebe
    unsigned short rest_37ebe : 15;
    char unknown_37ec0[0x37f08 - 0x37ec0];
    int field_37f08;                 // +0x37f08
    int volume1;                     // +0x37f0c
    int volume2;                     // +0x37f10
    Flags_0045d280 flags;            // +0x37f14
    unsigned char field_37f16;       // +0x37f16
};
#pragma pack(pop)

class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

class Class_004ce3e0 {
public:
    void FUN_004ce3e0(char* obj);
};

class Class_004ce450 {
public:
    int FUN_004ce450();
};

class Class_004ce580 {
public:
    int FUN_004ce580(int value);
};

class Class_004ce5a0 {
public:
    int FUN_004ce5a0();
};

class Class_004ce7a0 {
public:
    int FUN_004ce7a0(int value);
};

class Class_004ce7c0 {
public:
    void FUN_004ce7c0(int value, int type);
};

class Class_004ce7e0 {
public:
    int FUN_004ce7e0(int value);
};

class Class_004ce8c0 {
public:
    int FUN_004ce8c0(int value);
};

class Class_004ceb60 {
public:
    void FUN_004ceb60(int value, int flag);
};

class Class_004ced40 {
public:
    void FUN_004ced40();
};

class Class_004cedc0 {
public:
    int FUN_004cedc0(int value);
};

class Class_004d0070 {
public:
    void FUN_004d0070(int level);
};

class Class_004d00d0 {
public:
    void FUN_004d00d0(int level, int flag);
};

extern Game_0045d280* g_game;
extern int DAT_00512fe0;                // current track
extern int DAT_00512f42;
extern unsigned char DAT_00512f46;
extern unsigned char DAT_00512f48;
extern int DAT_00512fd9;
extern char DAT_00512f75[];
extern char DAT_005067bc[];              // "NOTRAK"
extern char DAT_00506984[];              // "TRACKMODE"
extern char DAT_0050692c[];              // "TRACKTYPE"
extern char DAT_0050696c[];              // "CDPLAY"
extern char DAT_00506964[];              // "CDNEXT"
extern char DAT_0050697c[];              // "CDPREV"
extern char DAT_00506974[];              // "CDSTOP"
extern char DAT_00506998[];              // "UNDO"
extern char DAT_00506990[];              // "RESTORE"
extern char DAT_00502b38[];              // "Options"

void __stdcall FUN_0049fa90(void* obj);
int __stdcall FUN_0049fd60(void* obj, char* name);
int __stdcall FUN_0049fdf0(Entry_0045d280* entries, char* name, int type);
int __stdcall FUN_004a0f60(void* obj, char* name);
void __stdcall FUN_004a1080(void* obj, char* name, int value);
void __stdcall FUN_0047f1a0(char* name, int value);
void __stdcall FUN_004ab0a0(void* obj);
void __stdcall FUN_004a9660(void* obj);
void __stdcall FUN_004ba590(float value);
void FUN_0045c3f0();
void FUN_0045d130();
void FUN_0045d7c0();

// FUNCTION: 0x45d280
void __stdcall FUN_0045d280(Object_0045d280* obj)
{
    Entry_0045d280* entries = obj->holder->entries;
    if (obj->field_60 == -1) {
        if (!g_game->prefs) {
            ((Class_004ced40*)g_game->sound)->FUN_004ced40();
            g_game->loaded = 0;
            return;
        }
        ((Class_004cdb40*)g_game->sound)->FUN_004cdb40();
        g_game->loaded = 0;
        return;
    }
    FUN_0049fa90(obj);
    if (FUN_0049fd60(obj, DAT_005067bc)) {              // "NOTRAK"
        FUN_0047f1a0(DAT_00502b38, 0);
        int v = FUN_004a0f60(obj, DAT_005067bc);
        unsigned short f = g_game->flags.word;
        g_game->flags.word = f ^ ((f ^ v) & 1);
        ((Class_004cedc0*)g_game->sound)->FUN_004cedc0(g_game->flags.word & 1);
        FUN_004ab0a0(obj);
        FUN_0045d130();
    } else if (FUN_0049fd60(obj, DAT_00506984)) {       // "TRACKMODE"
        FUN_0047f1a0(DAT_00502b38, 0);
        g_game->field_37f16 = FUN_004a0f60(obj, DAT_00506984) + 1;
        ((Class_004ce7a0*)g_game->sound)->FUN_004ce7a0(g_game->field_37f16);
        unsigned char mode = g_game->field_37f16;
        if (mode == 3) {
            DAT_00512fe0 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
            FUN_004ab0a0(obj);
            FUN_0045c3f0();
            return;
        }
        if (mode == 4) {
            FUN_004a1080(obj, DAT_0050692c, (unsigned char)((Class_004ce7e0*)g_game->sound)->FUN_004ce7e0(DAT_00512fe0));
            Entry_0045d280* list = ((Holder_0045d280*)g_game->table_531)->entries;
            if (g_game->field_37f16 == 4) {
                int found = FUN_0049fdf0(list, DAT_0050692c, 1);
                ((Class_004ce7c0*)g_game->sound)->FUN_004ce7c0(DAT_00512fe0, list[found].value);
            }
        }
        FUN_004ab0a0(obj);
        FUN_0045c3f0();
        return;
    } else if (FUN_0049fd60(obj, DAT_0050692c)) {       // "TRACKTYPE"
        FUN_0047f1a0(DAT_00502b38, 0);
        int i = obj->field_60;
        ((Class_004ce7c0*)g_game->sound)->FUN_004ce7c0(DAT_00512fe0, entries[i].value);
        FUN_0045c3f0();
        FUN_004ab0a0(obj);
        return;
    }
    if (FUN_0049fd60(obj, DAT_0050696c)) {              // "CDPLAY"
        FUN_0047f1a0(DAT_00502b38, 0);
        ((Class_004ceb60*)g_game->sound)->FUN_004ceb60(DAT_00512fe0, 1);
        FUN_004ab0a0(obj);
        return;
    } else if (FUN_0049fd60(obj, DAT_00506964)) {       // "CDNEXT"
        FUN_0047f1a0(DAT_00502b38, 0);
        DAT_00512fe0 = DAT_00512fe0 + 1;
        int n = ((Class_004ce450*)g_game->sound)->FUN_004ce450();
        if (DAT_00512fe0 > n)
            DAT_00512fe0 = 1;
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->FUN_004ce8c0(DAT_00512fe0);
        FUN_0045c3f0();
        FUN_004ab0a0(obj);
        return;
    } else if (FUN_0049fd60(obj, DAT_0050697c)) {       // "CDPREV"
        FUN_0047f1a0(DAT_00502b38, 0);
        DAT_00512fe0 = DAT_00512fe0 - 1;
        if (DAT_00512fe0 < 1)
            DAT_00512fe0 = ((Class_004ce450*)g_game->sound)->FUN_004ce450();
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->FUN_004ce8c0(DAT_00512fe0);
        FUN_0045c3f0();
        FUN_004ab0a0(obj);
        return;
    } else if (FUN_0049fd60(obj, DAT_00506974)) {       // "CDSTOP"
        FUN_0047f1a0(DAT_00502b38, 0);
        ((Class_004ced40*)g_game->sound)->FUN_004ced40();
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->FUN_004ce8c0(1);
        FUN_0045c3f0();
        FUN_004ab0a0(obj);
        return;
    }
    if (FUN_0049fd60(obj, DAT_00506998)) {              // "UNDO"
        FUN_0047f1a0(DAT_00502b38, 0);
        g_game->volume2 = DAT_00512f42;
        ((Class_004ce3e0*)g_game->sound)->FUN_004ce3e0(DAT_00512f75);
        g_game->field_37f16 = DAT_00512f48;
        ((Class_004ce7a0*)g_game->sound)->FUN_004ce7a0(g_game->field_37f16);
        if ((g_game->flags.byte ^ DAT_00512f46) & 1)
            ((Class_004cdb40*)g_game->sound)->FUN_004cdb40();
        unsigned short f = g_game->flags.word;
        g_game->flags.word = f ^ (((unsigned char)f ^ DAT_00512f46) & 1);
        ((Class_004ce580*)g_game->sound)->FUN_004ce580(DAT_00512fd9);
        goto apply;
    }
    if (FUN_0049fd60(obj, DAT_00506990)) {              // "RESTORE"
        FUN_0047f1a0(DAT_00502b38, 0);
        g_game->volume2 = 0x20;
        g_game->field_37f16 = 4;
        if ((g_game->flags.word & 1) == 0) {
            g_game->flags.bits.b0 = 1;
            ((Class_004cdb40*)g_game->sound)->FUN_004cdb40();
        }
apply:
        FUN_004ba590(0.5 - g_game->field_37f08 * -0.041666668f);
        ((Class_004d0070*)g_game->sound)->FUN_004d0070(g_game->volume1 << 10);
        ((Class_004d00d0*)g_game->sound)->FUN_004d00d0(g_game->volume2 << 10, 0);
        FUN_004a9660(obj);
        FUN_0045d7c0();
        return;
    }
    int i = obj->field_60;
    if (i != -1) {
        if (entries[i].state != 1) {
            FUN_004ab0a0(obj);
            return;
        }
        Vtable_0045d280* p = obj->holder->field_0;
        FUN_004a9660(obj);
        obj->field_60 = i;
        p->FUN_8(obj);
    }
}
