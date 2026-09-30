// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// deepseek-v4.1-flash: 90.1 percent (1338 bytes vs 1333), NOT MATCH.
//
// The CD-options menu handler (the sibling of 0x45da90, the sound-options
// handler, which has the same shape): a chain of "command name" tests that
// drives the CD player, the track-mode page and the volume/undo buttons, with
// a shared tail for the CD arms and another for the UNDO/RESTORE arms.
//
// What this pass changed (88.8 -> 90.1):
//  - The UNDO arm's flag update is `int f = flags.word; flags.word = f ^
//    ((f ^ DAT_00512f46) & 1)` with NO `(unsigned char)` cast. Adding the cast
//    makes the whole `& 1` narrow to a byte (`and cl,1`) and pulls in an
//    `xor eax,eax`; removing it also let the apply tail's two volume calls
//    match. `unsigned short f` and `short f` both score 85.2: they make MSVC
//    use the clear-bit-then-set ("and dx,0xfffe / movzx") rewrite.
//  - The TRACKMODE arm must NOT cache the mode in a local; the original
//    reloads `g_game->field_37f16` and re-loads g_game for the compare, so
//    compare the field directly each time. (This kept the score but is the
//    faithful source.)
//
// Earlier findings (space-bunny-free's, still true):
//  - The else-if chain is TWO statements, not one: `if (NOTRAK) ... else if
//    (TRACKMODE) ... else if (TRACKTYPE) ...` and then a separate `if (CDPLAY)
//    ... else if ...` chain, then `if (UNDO)`, `if (RESTORE)`, then the
//    fall-through block.
//  - The shared tails are `goto` labels placed at the END of the last arm of
//    their chain (cd_tail inside the CDSTOP arm, apply inside the RESTORE arm).
//  - The two bit tests have to be `g_game->prefs` on an `unsigned char`
//    bitfield (a word bitfield gives `test word`) and `g_game->loaded` on an
//    `unsigned short` bitfield (the store is `and word [...], 0xfffe`).
//
// Still different, in program order:
//  1. UNDO arm (~+8 bytes). Original: `mov ax, word [g_game+0x37f14]` (no
//     zero-extend), `mov dl, byte [DAT]`, `mov cl, al`, `xor cl, dl`,
//     `and ecx, 1` (32-bit), `xor ecx, eax`, `mov word, cx`. Ours keeps the
//     byte test in al (not dl), reads g_game into edx (not edi), and MSVC
//     rewrites the update as clear-bit-0-then-set: `xor eax,eax; and cl,1;
//     mov ax,word; movzx cx,cl; and eax,0xfffe; xor eax,ecx`. Tried `unsigned
//     short`, `short`, `(unsigned char)` cast, a local `x`; the clear/set
//     rewrite wins whenever f's upper bits are known. This arm's allocation
//     also propagates to the apply tail, so it has to be fixed first.
//  2. Apply tail: ours emits `push ecx` (the arg slot) before `fild
//     [g_game+0x37f08]`, the original after. Pure scheduling.
//  3. Final block: original `lea eax, [edi*8]; add ebp, edi; sub eax, edi`
//     for the `entries += i; entries[i].state` address; ours `mov eax, edi;
//     add ebp, edi; shl eax, 3; sub eax, edi`. Same value, MSVC picked the
//     shift form. (The TRACKTYPE arm at 0x45d777 DOES use `mov ecx,eax;
//     shl ecx,3` and matches, so this is a per-site instruction-selection
//     difference.)
//  4. TRACKMODE mode compare: original holds g_game in ecx and the byte in al
//     (`mov ecx,[g_game]; mov al,[ecx+0x37f16]; cmp al,3`), ours eax/cl.
//     Register pick only.
//  5. Jump targets (`je 0x45d64b` vs 0x45d64f etc.) are the +5-byte size
//     difference and will resolve when the code is byte-identical.

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
    void FUN_004ce7c0(int value, char type);
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
        if (g_game->field_37f16 == 3) {
            DAT_00512fe0 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
            FUN_004ab0a0(obj);
            FUN_0045c3f0();
            return;
        }
        if (g_game->field_37f16 == 4) {
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
        int f = g_game->flags.word;
        g_game->flags.word = f ^ ((f ^ DAT_00512f46) & 1);
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
