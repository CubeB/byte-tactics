// Decompiled by space-bunny-free, edited by deepseek-v4.1 and GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry 7: tested whether the xor operand order steers the
// UNDO block allocation. `game->flags.word = (b & 1) ^ f;` (v1) and the same
// with `int b = DAT ^ (unsigned char)f;` (v2) are byte-identical to the kept
// 91.5% file (1339 B), so MSVC5 canonicalises both commutative-xor orders to
// the same code, f still in EBP. Routing the store through `int r = (b & 1) ^
// f;` + `(unsigned short)r` drops to 89.3% (1337 B) and a byte local `d =
// DAT; int b = (unsigned char)f ^ d;` to 85.0% (1350 B). Kept the 91.5% int
// temp form; the EBP-for-EAX colouring of f is still the one open knot.
// deepseek-v4.1-flash retry 5 (diagnosis, still 91.5%): the whole UNDO flip
// difference reduces to WHICH REGISTER f (the word) lands in. Target is
// `mov ax,[f] / mov dl,[DAT] / mov cl,al / xor cl,dl / and ecx,1 / xor ecx,eax
// / mov [f],cx`: f in EAX so `(unsigned char)f` is the free `mov cl,al`.
// Ours lands f in EBP, and EBP has NO ADDRESSABLE LOW BYTE in 32-bit x86, so
// MSVC must widen `mov eax,ebp / and eax,0xff` and then the DAT operand is
// loaded 32-bit (`xor ecx,ecx / mov cl,[DAT]`) and the xor becomes 32-bit,
// giving 9 instructions/36 B instead of 7/28 B. So the +8-byte size and the
// whole register cascade are one cause: f must not be coloured into EBP.
// Every spelling that keeps `int b` (the baseline form, and n3/n4/n6/p3/p4/
// p7/r1-r4) leaves f in EBP and scores exactly 91.5. Removing `int b` frees
// EBP but MSVC then either folds the flip into `and edx,0xfffe` (fused forms,
// 90.5/1336 B) or drops `game` out of EDI (v1/m1/m2/m5, 86.7-90.5). m2/m3
// reach the exact 1333-byte size with f in EDX (addressable) and a byte DAT
// load, but distribute `(f ^ DAT) & 1` over the known-byte DAT and spend the
// saved bytes on an extra `mov edi,edx / and edi,1`; `game` also leaves EDI.
// The knot: exactly one more live node keeps `game` in EDI (baseline) but
// forces f to EBP; one fewer node frees EBP but lets the known-byte DAT fold
// or floats `game` to EAX. Target needs both EDI for game AND a byte-
// addressable register for f, with DAT still a byte (its if-test uses
// `mov cl,[DAT]`), and no spelling found so far produces all three.
// Everything else in the function (RESTORE/apply reload registers edx/eax vs
// ecx/edx, push-before-fild, tail `lea eax,[edi*8]` vs `mov eax,edi/shl
// eax,3`, TRACKMODE al/cl swap) is the same allocation domino and moves with
// this one cause; do not tune those blocks separately.
// deepseek-v4.1-flash retry 4: re-scored the byte-temp family,
// `unsigned char c = f; c ^= DAT; game->flags.word = f ^ (c & 1);` (and the
// split `unsigned char c = f; int b = c ^ DAT;`). Both reach the target
// byte-width xor but MSVC widens the mask to `and dl,1 / movzx dx,dl` and
// colours game into eax, 87.5% (1334 B). The exact Ghidra literal
// `word = (unsigned char)((unsigned char)word ^ DAT) & 1 ^ word;` and
// `word ^= (unsigned char)(word ^ DAT) & 1;` fold to an in-place memory xor,
// 90.7% (1327 B, 6 short). Nothing beat the 91.5% int-temp form below.
// deepseek-v4.1-flash retry 3 (timebox stop): kept this 91.5% file. Recreated the
// lost scratch variants and scored all three per-use-cast spellings with
// DAT_00512f46 declared `int` and the fused flip `f ^ ((f ^ DAT) & 1)`:
//   varA if-test `((unsigned char)flags.byte ^ (unsigned char)DAT) & 1`, 86.5%;
//   varB if-test `flags.byte ^ (unsigned char)DAT`, 86.5%;
//   varC if-test `((unsigned char)(flags.byte ^ DAT)) & 1`, 86.5%.
// All three are 1330 bytes and give the flip the exact target width shape
// (byte load `mov bl,[DAT]`, byte xor `mov dl,cl / xor dl,bl`, 32-bit
// `and edx,1 / xor edx,ecx`, word store: 31 bytes like the target), but the
// whole UNDO block rotates registers (game in eax not edi, f in ecx not eax,
// D in ebx not edx, temp in edx not ecx) and the rotation leaks downstream
// (apply block loads g_game into edx and pushes before fild; tail chain gets
// `lea eax,[eax+eax*2]` shapes). The per-use cast does hold BOTH width shapes
// (if-test xor stays byte width, flip keeps the 32-bit mask), so the widening
// does not leak across the tests; the failure is purely register coloring of
// the block, which moves as a unit with the DAT type. Still differing vs
// original: UNDO flip widths/registers (7 bytes too many here), RESTORE/apply
// block reload registers (edx/eax vs our ecx/edx) and push-before-fild order,
// tail `lea eax,[edi*8]` vs our `mov eax,edi / shl eax,3`, TRACKMODE al/cl
// swap. Ideas tried and exhausted across passes are listed below.
// Partial, 91.5%, 1339 vs 1333 bytes. Best UNDO flag-update shape so far:
// `unsigned short f = game->flags.word; int b = (unsigned char)f ^ DAT_00512f46;
// game->flags.word = f ^ (b & 1);` with the `game = g_game;` reload kept in the
// if body. That keeps g_game in edi and the `mov edi,[0x511de8]` reload like the
// original, but compiles the update to
//   mov bp,[edi+0x37f14] / xor ecx,ecx / mov cl,[DAT] / mov eax,ebp /
//   and eax,0xff / xor eax,ecx / and eax,1 / xor eax,ebp / mov [edi+..],ax
// where the original is 7 instructions (mov ax / mov dl,[DAT] / mov cl,al /
// xor cl,dl / and ecx,1 / xor ecx,eax / mov [..],cx): the original does the xor
// at byte width and masks 32-bit, MSVC5 here zero-extends with `and eax,0xff`.
// Fusing the bit as `f ^ (((unsigned char)f ^ DAT_00512f46) & 1)` does give the
// byte-width xor but widens the bit through dx (88.1%); dropping the reload
// assignment instead keeps the local live across the callback in edi but loses
// the reload line and still widens (88.1%); the previous compound `^=` form
// scores 90.7%; int bit with fused mask 90.2%; no pointer local 85.8%; byte
// locals 86.7/87.5%.
// New tries (issue 2013 rerun, all worse, best still 91.5): byte temp
// `unsigned char c = (unsigned char)f ^ DAT; game->flags.word = f ^ (c & 1)`
// 87.5, masks in byte width then `movzx dx,dl`; `int b = (f ^ DAT) & 1;`
// 86.7 (distributes the mask over both bytes, and loses `game` in edi);
// one-statement `flags.word = flags.word ^ ((flags.word ^ DAT) & 1)` 90.5,
// fuses to `and edx,0xfffe` plus `and cl,1 / movzx cx,cl`; `int b =
// ((unsigned char)f ^ DAT) & 1;` 90.2. Target UNDO flip is
// `mov ax,[edi+0x37f14] / mov dl,[DAT] / mov cl,al / xor cl,dl / and ecx,1 /
// xor ecx,eax / mov [edi+0x37f14],cx` (29 bytes; ours 36), i.e. a byte width
// xor whose result is masked 32-bit while f stays live in eax; NOTRAK`s
// `f ^ ((f ^ v) & 1)` reaches that byte-xor/32-bit-mask pair (v an int), but
// with the byte global DAT the optimizer instead fuses or narrows the mask.
// Other remaining diffs: RESTORE callback setup uses `mov eax,[0x511de8] /
// mov ecx,[eax+0x10]` where the original keeps ecx through both loads (same
// size); the apply block pushes the FUN_004ba590 argument slot before `fild`
// where the original loads fild first; the final tail index wants
// `lea eax,[edi*8]` where ours emits `mov eax,edi / shl eax,3`; the TRACKMODE
// `field_37f16 == 3` test uses cl/eax where the original uses al/ecx.
// deepseek-v4.1-flash retry 6: scored the unscored varD shape the retry-2 note
// left as "next step": DAT_00512f46 as `int`, fused flip `f ^ ((f ^ DAT) & 1)`
// and the if-test cast back to `(unsigned char)DAT_00512f46`. Confirms the
// note: it keeps the target byte-width xor but the if-test cast reshuffles the
// block, 86.5% / 1330 bytes. So that family is a dead end; the block needs the
// EBP=entries pin on the UNDO path (the original sets ebp=entries at 0x45d28b
// and still has it at 0x45d611/0x45d777, so f is forced to EAX), while our
// compile frees EBP there because the tail use is not on the UNDO/goto apply
// path. Kept the 91.5% file.
// Rerun by space-bunny-free: the UNDO flip is an MSVC5 WIDTH-choice problem,
// and the two halves of it pull in opposite directions, so no spelling gets
// both. Target is a BYTE-width xor of f's low byte with DAT followed by a
// 32-BIT `and ecx,1`; a byte xor is only legal there if MSVC does not know
// the xor result is 0..255 (otherwise it emits an 8-bit mask plus a movzx,
// as the fully fused spelling does). New tries, none better than 91.5:
//   fused `f ^ (((unsigned char)f ^ D) & 1)`        87.5, byte xor but
//     `and dl,1` + `movzx dx,dl` (MSVC tracks 0..255 for a cast of a 16-bit);
//   fused `f ^ ((f ^ (unsigned char)D) & 1)`        90.5, 1336 bytes;
//   `int b = f ^ (unsigned char)D`                  91.5, identical code to
//     the current shape, so moving the cast from f to D changes nothing;
//   same with `(char)D` 86.6, `(short)D` 91.5, `(unsigned short)D` 91.5,
//   `(unsigned char)D ^ f` 91.5, `(unsigned char)` on both operands 91.5,
//   separate `int b` then `int c = b ^ D` 91.5, `char` cast on f 87.8,
//   `int b = (unsigned char)f` used directly in the mask 86.2;
//   `Flags_0045d280 fl = game->flags;` then flipping fl.word from fl.byte
//     89.5 (the union copy does not stay in a register);
//   `f ^ ((game->flags.byte ^ D) & 1)` (second read of the byte member)
//     90.7 but only 1327 bytes, i.e. 6 short: MSVC folds it to
//     `xor cl,[eax+X] / and cl,1 / movzx cx,cl / xor word [eax+X],cx`,
//     an in-place memory xor instead of a load/keep/store of the word.
// So the wanted form is a word in EAX, D in DL, an 8-bit xor into CL and a
// 32-bit mask, with the word stored back from CX rather than xored in place.
// deepseek-v4.1-flash retry 2 (kept this 91.5% file, all new variants lower):
// declaring DAT_00512f46 as `int` and writing the flip as the fused
// `f ^ ((f ^ DAT_00512f46) & 1)` DOES produce the target SHAPE: narrowed byte
// load `mov bl,[DAT]`, byte xor `mov dl,cl / xor dl,bl`, then 32-bit
// `and edx,1 / xor edx,ecx`, i.e. exactly the NOTRAK width pair (int-typed
// xor operand keeps the mask 32-bit while the xor itself runs at byte width).
// But the register roles rotate (game lands in eax not edi, f in ecx, D in
// ebx, temp in edx) and the if-test above it breaks: with D an int,
// `(flags.byte ^ D) & 1` distributes into `(byte & 1) ^ (D & 1)`
// (`mov ecx,[DAT] / and ecx,1 / mov dl,[..] / and edx,1 / xor edx,ecx`), which
// reshuffles the whole block allocation (86.9%, 1333 bytes exact size).
// Casting D back to unsigned char at the if-test only (varD) or wrapping the
// xor in an (unsigned char) cast (varE), and an int local copy of D (varG),
// were written to build/scratch/0x45d280/ but did not finish scoring before
// the timebox. Next step: get the if-test back to `mov cl,[DAT] / mov dl,[..]
// / xor dl,cl / test dl,1` while keeping the int-typed flip xor, then chase
// the register rotation (game eax->edi, f ecx->eax, D ebx->edx, temp
// edx->ecx) via the `game = g_game` reload placement.
// deepseek-v4.1-flash retry: ~30 more spellings of the UNDO flip tried (int,
// unsigned int, short, char and byte temps, fused and split, bitfield writes,
// explicit bool, both member-read orders, local DAT copies, no-cast forms, the
// literal Ghidra shape). None beat 91.5; the closest were the no-cast fused
// form (90.5, 1336 B) and the current int-temp form (91.5, 1339 B). The target
// byte-xor plus 32-bit mask only appears when MSVC leaves f in EAX; every form
// that keeps `game` in EDI (needed for the two identical blocks around the
// callback) spends EAX on the other temp, so f lands in EBP and the byte xor
// becomes `mov eax,ebp / and eax,0xff`. The downstream register choices
// (TRACKMODE cmp al/cl, RESTORE store edx/eax, lea vs mov+shl) shift with the
// same allocation, so they are consequences and not separately fixable.
#include <string>
#include <windows.h>

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
        Game_0045d280* game = g_game;
        if ((game->flags.byte ^ DAT_00512f46) & 1) {
            ((Class_004cdb40*)game->sound)->FUN_004cdb40();
            game = g_game;
        }
        unsigned short f = game->flags.word;
        int b = (unsigned char)f ^ DAT_00512f46;
        game->flags.word = f ^ (b & 1);
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
