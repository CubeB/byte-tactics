// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, edited by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, checked by GPT-6. Names are provisional.
// Codex GPT-6 retry for #5198 (2026-10-03): `/Gi` preserves the 99.4%
// score and both SIB base/index mismatches; the default source remains best.
// GPT-6 retry (#5248): rechecked at 99.4%; both g_game store SIB orders remain swapped.
// (previously: deepseek-v4.1-flash, GPT-6, space-bunny-free.)
//
// GPT-6 retry (#4906): extracted both stores into one static inline helper
// that reads g_game directly. Inlining leaves both SIB operand orders unchanged.
// Rechecked for issue #5117 on 2026-10-03; the two SIB base/index mismatches
// remain unchanged at 99.4%.
//
// Partial: 99.4% (1116 of 1116 bytes, the byte count already matches).
// Two instructions still differ, both the SIB base/index choice of a byte
// store through g_game:
//   original  mov byte ptr [edx + esi + 0x2bf1], al     (edx = g_game base)
//   ours      mov byte ptr [esi + edx + 0x2bf1], al     (index in the base slot)
// and the same pair at the digit-case store (`[edx + eax + 0x2bf1]` against
// `[eax + edx + 0x2bf1]`). The registers are already right (edx = g_game,
// esi = n / eax = d); only which register MSVC puts in the SIB base slot
// differs. Everything else, including the 0x37f2f bit test, the strlen block
// and the memset ordering, matches.
//
// Space Bunny Free (issue #4614), this session: no new best (still 99.4%), but
// the residual is now characterised precisely. Six micro-probe files under
// build/scratch/493bf0 (p1.cpp, p2.cpp, p4.cpp, p6.cpp, p7.cpp, p8.cpp, p9.cpp,
// p10.cpp, each compiled with /Fa so the operand lists can be read directly)
// settle it. Superseding the note further down that claimed "MSVC 5 puts the
// pointer in the ADDRES base slot only when that pointer is a named local":
//
//  * The SIB base slot goes to the *first register-class* address operand, and
//    an operand that is a direct read of a global (or of an absolute address,
//    with the array offset folded into it) is *address*-class and always ends
//    up in the index slot. Measured, base slot in bold:
//      `g_game->field_2bf1[n] = v`            [esi + edx]  n in base
//      `*(g_game->field_2bf1 + n) = v`         [esi + edx]  n in base
//      `*(n + g_game->field_2bf1) = v`         [esi + edx]  n in base
//      `((unsigned char*)g_game)[0x2bf1+n] = v`[esi + edx]  n in base
//      `(&g_game->field_2bf1[0])[n] = v`       [esi + edx]  n in base
//      `g_game->field_2bf1[g_int] = v`         [edx + ecx]  g_int in base
//      `g_game->field_2bf1[n] = g_arr[0]`      [edx + eax]  n in base
//      `Game* g = g_game; g->field_2bf1[n] = v`     [ecx + esi] g in base
//      `Game& g = *g_game; g.field_2bf1[n] = v`     [ecx + esi] g in base
//      `char* c = (char*)g_game; c[0x2bf1+n] = v`   [ecx + esi] c in base
//      `SetSel(g_game, n, v)` (inlined helper)      [ecx + esi] g in base
//    Thirty spellings of the direct-global form all leave it in the index slot
//    (p9.cpp), so no re-spelling of `g_game->field_2bf1[n]` will move it.
//
//  * Register choice is a separate, coupled decision, and it is what makes the
//    residual unreachable from either side. Measured: an operand that is a
//    direct read of a global gets EDX here (the global read in `sa`, p6.cpp,
//    also lands in EDX once EAX is busy), while a named local gets ECX, and
//    four simultaneous pointer locals in x1.cpp come out in the order EAX,
//    ECX, EDX, then a callee-saved one for the fourth. So the materialised
//    global prefers EAX/EDX/ECX and a local prefers EAX/ECX/EDX. At this store
//    EAX is busy holding the byte being stored (`al` is FUN_004a0ff0's
//    return), so the direct global gets EDX (right register, wrong slot) and
//    the alias gets ECX (right slot, wrong register). Getting both would need
//    the pointer to be a materialised global *and* to beat the index for the
//    base slot, and the two rules above say no spelling does both. That is
//    the whole residual: the original has a pointer that keeps EDX and still
//    wins the base slot, and nothing reproduced here produces that.
//
//  * Two by-products worth keeping for other functions:
//    - Dropping the `unsigned char v` local and storing the call result
//      directly (`g_game->field_2bf1[n] = FUN_004a0ff0(...)`) moves the
//      pointer from EDX to ECX without changing the slot, so that local is
//      what buys EDX today. `= (FUN_004a0ff0(...) != 0)` does the same.
//    - A self-conditional on the pointer *inside the address expression*
//      (`(g_game ? g_game : g_game)->field_2bf1[n] = v`) makes the pointer a
//      phi but does NOT move it into the base slot; it just takes ECX. Only a
//      named local does that.
//
//  * Tried and measured with no effect on either site (all leave the emitted
//    code byte-identical): ~15 address re-spellings, the displacement attached
//    to the index term, `n`/`d` as `unsigned int`, `long`, `unsigned long`,
//    `field_2bf1` as `char`/`signed char`/`char[12]`, `v` as `char`, `v & 0xff`,
//    `(unsigned char)v`, `v != 0`, storing the call result with and without the
//    local, a comma expression, `if (n)` around the store, two stores of the
//    same value, a pointer-to-the-array local, and a self-conditional on
//    `g_game` before the store. Tried with the alias (all give ECX, never EDX):
//    a `Base()` by-value helper, a `SetSel()` store helper, `Game* const`,
//    `void*` plus a cast, `*&g_game`, a split declaration, an extra scope
//    block, a second alias, a second pointer local, an array-pointer local
//    with a self-assign, an index local, dead statements (`g = g`, `n = n`,
//    `v = v`, `g->mode_2bf0 = g->mode_2bf0`, `int t = 0; if (t) ...`,
//    `mode = mode | (t & 0)`), the alias at the top of the function, before the
//    LIVEPLYR branch and before the call, and `g = g ? g : g`. The alias
//    declared *before* the call moves it to EBX instead (it must survive the
//    call); `g->mode_2bf0 = 0;` after the alias additionally moves `n` out of
//    ESI into EDI.
//  * Declaring the alias at *function* scope (`Game_00493bf0* g;` in the
//    declaration block, `g = g_game;` inside the branch) loses the base slot
//    again, back to `[esi + ecx + 0x2bf1]`: it is the block-scope declaration
//    that makes the pointer register-class. Same for a `char*` alias and for
//    one declared as the very first local. Found by permute.py.
//
//  * Two runs of tools/permute.py (28 minutes from this 99.4% file with 4 jobs,
//    1250+ candidates, and 15 minutes from the alias shape above) found
//    nothing beyond this: the SIB byte is the only difference and check.py's
//    ratio does not reward register-only moves, so the fine-grained permuter
//    score is the only signal and it stayed at 10 / 25.
//
// Space Bunny Free (issue #4456), earlier session: took the function from 89.7%
// to 99.4%. Three findings, all of them needed:
//  * The strlen block (about 20 instructions of the old diff) only lands when
//    the four statements that open it are in this order: oldmode, to, base,
//    saved. The old file had saved first, which is worth a point, and a
//    `Game* g = g_game` local was tried as well. `saved` really does have to be
//    the last of the four (all 24 orderings scored: saved-first 89.7,
//    anything else 88.4 or 88.7).
//  * The guard on p[1] has to be `if (!cond) goto skip0;` followed by the body
//    and then `skip0:;` immediately before `after:`. Spelling it as the
//    natural `if (cond) { ... }` costs 9 points (99.4 -> 88.7), and so does
//    replacing the extra label with `goto after` (98.4 -> 88.7 by this
//    session's scoring). The empty statement between the two labels is load
//    bearing; see `skip0:;` below.
//  * The 0x37f2f bit test. The original materialises the byte
//    (`mov dl, [ecx + 0x37f2f]; shr dl, 1; test al, dl`) and pushes the flags
//    dword (`push eax`), so `flags` must be an `int`. Declaring the field at
//    0x37f2f as `unsigned short` and casting the shifted value back to
//    `(unsigned char)` is what stops MSVC 5 folding it to
//    `test byte ptr [ecx + 0x37f2f], 2`: the shift is then a 16-bit shift whose
//    result is truncated, so the constant fold `(x >> 1) & 1 -> x & 2` never
//    applies, and MSVC still narrows the load to a byte. Every spelling with a
//    byte field (a plain `unsigned char`, a bitfield, `char`, `int`, a local
//    `raw`, a `static inline` helper, a variable shift count, `/ 2`, `!= 0`,
//    a ternary) folds, whatever the type of `flags`.
//    `int flags` on its own is 1113 bytes and duplicates the `or al, 2`
//    (`test ecx,ecx; je; or al,2; test ecx,ecx; je; or al,2`); the byte count
//    only comes out at 1116 once the shift is spelled the way above.
//
// Earlier leads still worth keeping (from the 89.7% notes):
//  * Both `mode` and `oldmode` must be `int`. With an `unsigned char oldmode`
//    the working mode wins ebx and the strlen block falls apart; with both int,
//    oldmode wins ebx and the working mode lives at [esp+0x10] as in the
//    original. Declaration position of the two does not matter.
//  * Use `gadget->field_60` directly instead of an `id` local, and compute the
//    digit case's `to` before the memset, so the CSE'd players[d] temp lands
//    in ebp.
//  * `sizeof(Player_00493bf0)` is 0x14b; 0x14a breaks the players index maths.
//  * For the SIB slots: MSVC 5 puts the *pointer* in the ADDRES base slot only
//    when that pointer is a named local (a `Game* g = g_game` local, a
//    reference `Game& g = *g_game`, an inline member function, a
//    `static inline void SetSel(Game*, int, unsigned char)` helper, or a
//    two-level chain such as `h->g->f[n]`); otherwise, when the pointer is a
//    single-use temporary loaded straight from the global, it goes in the
//    index slot and the index expression goes in the base slot. Every shape
//    that fixes the slot moves g_game out of edx (into ecx, or into ebx when
//    the local is live across the call) and then breaks
//    `mov eax, [edi + 0x60] / push eax` in the SENDTO branch, for a net loss
//    of about a point. `0x420960` in this tree is matched and shows the
//    wanted form (`[ecx + esi*1 + 0x1ab8f]`, pointer in the base slot), but
//    only because g_game there is loaded once and reused, so the pointer is
//    not a single-use temporary. Not found: a source spelling that keeps the
//    direct `g_game->field_2bf1[n]` form and still gets the base slot.
//    The two stores differ only in the SIB byte (0x32 against 0x16, that is
//    scale 1 with the operands exchanged). About 150 variants left them
//    untouched: named locals and references in every position, helpers and
//    inline members, two-level chains, `*(arr + n)` and `n[arr]`, a
//    `unsigned char (&f)[11]` array reference, a byte pointer to g_game, a
//    nested member struct, `(char*)g_game + 0x2bf1 + n`, a nested struct for
//    field_2bf1, all 24 orders of the four block-opening statements, local
//    declaration and type permutations, and semantically neutral struct
//    padding and filler declarations (to shift MSVC's internal node ids).
//    Two runs of permute.py from 97.6% and from 99.4% (1250 and 5012
//    candidates) found nothing beyond this.
//
// DeepSeek V4.1 Flash (issue #4758), this session: no new best. A fresh run of
// permute.py (3 min, 2220 candidates) found nothing. headers.py tried all 256
// header sets (none match). Optimization-flag variants re-checked: /Oi, /Ot,
// /Op and /Ob1 are byte-identical to the default; /Ow and /Oa change the
// instruction sequence (/Oa relaxes the alias assumptions), 66.6% and 65.3%,
// far worse. Fresh micro-probes under /O2 (with the same eax-busy with a byte
// call result as here) re-confirm the split:
//   * direct `g_game->field_2bf1[n] = v` -> pointer in the index slot, in EDX;
//   * any named-local/array-pointer/reference form -> pointer in the base slot,
//     but the register becomes ECX (`[ecx + esi + 0x2bf1]`), and every such
//     form also drags the SENDTO branch's `mov eax, [edi+0x60]` with it, so the
//     whole function drops to 98.4%.
//   * Forcing the local into EDX needs another value already live in ECX with
//     no extra instructions; none exists at that point (n is ESI, v is EAX, and
//     every added dummy local emits code or crosses a call and lands in a
//     callee-saved register). A local declared before the FUN_004a0ff0 call
//     moves to EBX, as recorded above.
// A brute force over ~30 fresh spellings of the first store (direct, cast,
// comma, unary, local in every form, two locals, split declaration, void*,
// char*, call-result-direct) and ~10 of the second store (including a shared
// local used by both the memset and the store) produced no `[edx + esi ...]`
// or `[edx + eax ...]` store. The residual is the two SIB bytes and nothing
// else; it stays stuck.
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#pragma pack(push, 1)
struct Entry_00493bf0 {                // 0x15b bytes
    char state;                        // +0x00
    char unknown_1;                    // +0x01
    char name[0x10];                   // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0x137 - 0xb8];
    unsigned char field_137;           // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Layer_00493bf0 {
    char unknown_0[4];
    Entry_00493bf0* entries;           // +0x04
    char unknown_8[0x20 - 0x8];
    int field_20;                      // +0x20
};

struct Gadget_00493bf0 {
    char unknown_0[0x18];
    Layer_00493bf0* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Player_00493bf0 {               // 0x14b bytes
    char unknown_0[4];
    int field_4;                       // +0x04
    char unknown_8[0x14b - 0x8];
};

struct Saved_00493bf0 {                 // 10 bytes
    int a;                            // +0x00
    int b;                            // +0x04
    short c;                          // +0x08
};

struct Flags16_00493bf0 {
    unsigned short bits0_7 : 8;
    unsigned short bit8 : 1;
    unsigned short bits9_15 : 7;
};

struct Game_00493bf0 {
    char unknown_0[0x519];
    Gadget_00493bf0 gadget;            // +0x519
    char unknown_57d[0x1b63 - 0x57d];
    Player_00493bf0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    Flags16_00493bf0 field_2bee;       // +0x2bee
    unsigned char mode_2bf0;           // +0x2bf0
    unsigned char field_2bf1[11];      // +0x2bf1
    char unknown_2bfc[0x37ebe - 0x2bfc];
    unsigned short flags_37ebe;        // +0x37ebe
    char unknown_37ec0[0x37f2f - 0x37ec0];
    unsigned short field_37f2f;        // +0x37f2f, bit 1 is the "verbose" bit
};
#pragma pack(pop)

extern Game_00493bf0* g_game;
extern int DAT_005091cc;
extern char DAT_0051e788[];
extern char DAT_0050940c[];            // "LIVEPLYR"
extern char DAT_00503130[];            // "SmallButton"
extern char DAT_00509400[];            // "SENDTYPE"
extern char DAT_005093f8[];            // "SENDTO"
extern char DAT_00506578[];            // "TALK"
extern char DAT_005093f4[];            // ",:;"
extern char DAT_005093ec[];            // "Enemies"
extern char DAT_00508384[];            // "Allies"

void __stdcall FUN_0047f1a0(char* name, int flag);
void __stdcall FUN_004a1080(Gadget_00493bf0* obj, char* name, int value);
int __stdcall FUN_004a0ff0(Gadget_00493bf0* obj, int index);
int __stdcall FUN_004a0f60(Gadget_00493bf0* obj, char* name);
Entry_00493bf0* __stdcall FUN_004a0010(Entry_00493bf0* entries, char* name);
void __stdcall FUN_004a0d00(Gadget_00493bf0* obj, char* name, char* text);
void __stdcall FUN_004a9660(Gadget_00493bf0* obj);
void __stdcall FUN_0049fa90(void* obj);
void __stdcall FUN_004ab0a0(Gadget_00493bf0* obj);
void FUN_00493ae0();
void FUN_00494050();
int __stdcall FUN_00417b50(char* cmd, int flags);
int __stdcall FUN_0049fd60(Gadget_00493bf0* gadget, char* name);
int __stdcall FUN_0049fdf0(Entry_00493bf0* entries, char* name, int type);
void __stdcall FUN_0049fc50(Gadget_00493bf0* obj, int index);
void __stdcall FUN_00463e50(Player_00493bf0* from, char* text, int param_3, char* to);

// FUNCTION: 0x493bf0
void __stdcall FUN_00493bf0(Gadget_00493bf0* gadget)
{
    char buf2[0x12c];
    char buf[0x100];
    int oldmode;
    int mode;
    Entry_00493bf0* entries = gadget->layer->entries;
    if (gadget->field_60 == -1) {
        g_game->flags_37ebe &= ~4;
        return;
    }
    if (_strnicmp(entries[gadget->field_60].name, DAT_0050940c, 8) == 0) {
        FUN_0047f1a0(DAT_00503130, 0);
        g_game->mode_2bf0 = 3;
        FUN_004a1080(gadget, DAT_00509400, g_game->mode_2bf0);
        int n = atoi(&entries[gadget->field_60].name[8]);
        // Kept as the original has it: n is never range checked before it
        // indexes the 11-byte selection mask, so a "LIVEPLYR42" style name
        // writes outside field_2bf1. The neighbouring mode_2bf0 is clamped
        // (`if (g_game->mode_2bf0 >= 4) g_game->mode_2bf0 = 0;`), so the
        // omission looks like an oversight rather than a deliberate choice.
        unsigned char v = (unsigned char)FUN_004a0ff0(gadget, gadget->field_60);
        g_game->field_2bf1[n] = v;
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        goto tail;
    }
    if (FUN_0049fd60(gadget, DAT_005093f8)) {
        FUN_0047f1a0(DAT_00503130, 0);
        unsigned char v = (unsigned char)FUN_004a0ff0(gadget, gadget->field_60);
        g_game->field_2bee.bit8 = v & 1;
        FUN_004a0d00(gadget, DAT_00506578, DAT_0051e788);
        FUN_004a9660(gadget);
        FUN_00494050();
        FUN_004ab0a0(gadget);
        return;
    }
    if (FUN_0049fd60(gadget, DAT_00509400)) {
        FUN_0047f1a0(DAT_00503130, 0);
        g_game->mode_2bf0 = (unsigned char)FUN_004a0f60(gadget, DAT_00509400);
        if (g_game->mode_2bf0 >= 4)
            g_game->mode_2bf0 = 0;
        FUN_00493ae0();
        FUN_004ab0a0(gadget);
        goto tail;
    }
    if (FUN_0049fd60(gadget, DAT_00506578)) {
        Entry_00493bf0* talk = FUN_004a0010(entries, DAT_00506578);
        mode = g_game->mode_2bf0;
        lstrcpynA(buf, (char*)talk + 0xb6, 0x100);
        char* p = buf;
        while (*p && *p == ' ')
            p++;
        if (*p == '+') {
            int flags = 1;
            // The cast keeps the shift a 16-bit one, which is what stops MSVC
            // folding this into `test byte ptr [g_game + 0x37f2f], 2`.
            if (flags & (unsigned char)(g_game->field_37f2f >> 1))
                flags = 7;
            if (DAT_005091cc)
                flags |= 2;
            int r = FUN_00417b50(p + 1, flags);
            entries = gadget->layer->entries;
            if (r & 2)
                mode = 0;
        }
        if (strlen(p) != 0) {
            oldmode = g_game->mode_2bf0;
            char* to = 0;
            Player_00493bf0* base = &g_game->players[g_game->localPlayer];
            Saved_00493bf0 saved = *(Saved_00493bf0*)g_game->field_2bf1;
            // Early out rather than a positive `if`, and the empty statement
            // at skip0 below is load bearing: either change costs 9 points.
            if (!(' ' < p[1] && strchr(DAT_005093f4, p[1]) != 0)) goto skip0;
            if (isdigit(p[0])) {
                int d = p[0] - '0';
                if (d < 0 || d > 9 || g_game->players[d].field_4 == 0)
                    goto clear;
                p += 2;
                mode = 3;
                to = (char*)&g_game->players[d] + 0x2b;
                memset(g_game->field_2bf1, 0, 11);
                g_game->field_2bf1[d] = 1;
            } else {
                int c = tolower(p[0]);
                if (c != 'a') {
                    if (c == 'e') {
                        mode = 2;
                        to = DAT_005093ec;
                    } else {
                        goto after;
                    }
                } else {
                    mode = 1;
                    to = DAT_00508384;
                }
                p += 2;
            }
skip0:;
after:
            g_game->mode_2bf0 = mode;
            memset(buf2, 0, sizeof(buf2));
            FUN_00463e50(base, p, 4, to);
            *(Saved_00493bf0*)g_game->field_2bf1 = saved;
            g_game->mode_2bf0 = oldmode;
        }
clear:
        memset(DAT_0051e788, 0, 0x81);
        g_game->field_2bee.bit8 = 0;
    }
tail:
    int index = FUN_0049fdf0(entries, DAT_00506578, 3);
    FUN_0049fc50(&g_game->gadget, index);
    g_game->gadget.layer->field_20 = FUN_0049fdf0(entries, DAT_00506578, 3);
}
