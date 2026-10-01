// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// Started by deepseek-v4.1-flash and GPT-6 (their 80.5% version is the basis).
// Partial at 84.6% (1596 bytes against 1586). What still differs:
//  1. The unit-type scan loop: the original zeroes the record byte offset with
//     a separate "xor ebp, ebp" and keeps the item byte offset (0x249 stride)
//     in edi, while ours coalesces the count's zero-init into that register
//     and swaps ebp/edi. Tried an explicit record byte-offset local, declared
//     before and after the item offset, both scored 78.8%; the compiler still
//     coalesces it with the count's zero-init.
//  2. The tail (after the two sprintf/_ftol/ENERGYTEXT/METALTEXT pairs) has
//     scheduling differences: "mov eax, [esp+0x24]" is emitted after the call
//     instead of before "xor edx, edx".
//  3. Computing the scroll panel (g_game+0x519) fresh at each use instead of
//     preserving it in a register scored higher (84.6% vs 80.5%) but is less
//     faithful to the original, which keeps it in esi.
// The scan loop's item pointers are reloaded after every call.
// Tried on 2026-09-30 and all scored below this file:
//  explicit record byte offset (vA 83.0), item local moved into the match block
//  (vM 81.7), no item local at all (vB 80.3), item offset declared at function
//  scope (vJ, identical bytes), a separate record counter r (vL, identical),
//  index form item[i] (vE 83.2), record pointer walk (vV 83.6), item offset
//  through i * 0x249 (vU, identical), an extra off use in the name check
//  (vR 83.2), an explicit n * 0x62 (vY, identical), hoisting the slider
//  handler (vI, identical), defining the real preceding 0x44c7a0 above this
//  function (vZ, identical), and 0/2/4/8/16/24/32/48/64/96/128/192/256/384
//  unused extern declarations (all 84.6%, so it is not TU state). The ebp/edi
//  swap and the downstream tail schedule resist every source shape tried; the
//  variant that wins item offset in edi does so by giving the desc gadget ebp
//  instead of ebx, which costs more than it gains.
// Third session (deepseek-v4.1) re-checked this baseline at 84.9% and tried:
//  (E1) an explicit record byte offset roff used for the sprintf target and the
//  three record stores: 82.7%, 1598 bytes; the ebp/edi roles still do not flip,
//  so the allocator keeps the 0x249 item offset in ebp even when the 0x62 record
//  offset has more uses. (E2) a plain for-loop with off at function scope: 78.3%,
//  1588 bytes. (E3) FUN_004a35a0(g_game->inner, ...) instead of inner->entries,
//  which is what the original pushes at 0x44ccd8: 83.3%, 1595 bytes, the extra
//  deref actually helps the register assignment. (E4) a panel2 local for the
//  0x44cc91 FUN_0044bfd0/FUN_0049fa90 pair: 81.6%, 1585 bytes. All four scored
//  below this file, so this file is kept.
// Fourth session (deepseek-v4.1) found the mechanism behind the scan-loop swap:
// the register the record offset gets is decided by the loop's *use counters*.
// Writing the loop in the original's shape (index form g_game->items[i] for the
// flags load and the name check, the item pointer only inside the block) does
// put the 0x249 item offset in edi, exactly as the original, and the 0x62
// record offset then reuses the register freed by the spilled `entries` local;
// but that same reshape moves the desc gadget from ebx to ebp (the original
// keeps desc in ebx and the record offset in ebp), and the whole-function
// register rotation costs more than the loop gains: 81.9% at 1609 bytes for
// the block-local item pointer, 80.5% at 1606 bytes for the full index form,
// 77.9% at 1623 bytes for index form only on the flags test, 81.5% when i is
// declared before n. So the ebp/edi roles are a function-wide coloring tie
// broken by desc: as long as desc keeps ebx the record offset wins edi.
// Second session (deepseek-v4.1-flash) confirmed this baseline at 84.6% and
// tried: (W1) moving "int n = 0;" out of the declaration into a plain
// "n = 0;" statement (identical 1596 bytes, the merge with xor edi,edi is not
// source-order driven); (W2) using the scrollPanel local for the
// FUN_0044bfd0/FUN_0049fa90 pair, which is what the original does (esi holds
// g_game+0x519 from 0x44cc38), gives 1592 bytes, only 6 over the original and
// the tail hunks shrink, but the score drops to 80.5% because idx and the
// panel then trade registers over the whole tail (the original keeps idx in
// edi and the panel in esi; ours gets idx in esi); (X1) the index form
// g_game->items[i] on top of W2 is byte-identical to W2 (1592); (X2) an item
// pointer walk (item = items+1, item += 0x249) on top of W2 drops to 77.6%
// (1533 bytes). The scan loop's ebp/edi swap (ours: xor edi,edi merged with
// the count store, item offset in ebp; original: mov [esp+0x14],0 immediate,
// xor ebp,ebp record offset, mov edi,0x249 item offset) is unchanged by every
// shape tried, so it is the register allocator's global choice, not the
// source. Kept: this 84.6% version, because the orchestrator scores by the
// percentage.
// Fifth session (deepseek-v4.1, 2026-09-30): the scan loop's register swap was
// attacked as a pair. Making ONLY the flags load use indexed base+off
// addressing (leaving the item pointer materialized for the name test) lifts
// this file from 84.9% to 85.2% at 1595 bytes: the loop now reads
// [edx+ebp+0x245] and lea esi,[edx+ebp], so only the register name (ebp holds
// the 0x249 item offset here, edi in the original) and MSVC's folded bit test
// (test ch,0x80 instead of shr ecx,0xf / test cl,1) differ there.
// Writing that expression with an explicit (unsigned char) cast to force the
// shr/test pair costs 2 bytes and rotates the whole function: 83.5%. Writing
// the name test as an indexed base+off expression too compiles to identical
// bytes (MSVC CSEs it). Declaring the item pointer inside the block rotates
// entries into ebx and desc into ebp: 74.8%. Keeping the scroll panel in a
// local for the FUN_0044bfd0/FUN_0049fa90 pair scores 81.2%. What still
// differs: the loop's ebp/edi role swap (item offset in ebp, record offset in
// edi, whose zero-init coalesces with n = 0 as the "xor edi, edi" before the
// branch) and the tail, where n lives in edi instead of on the stack, idx gets
// esi instead of edi, and the panel (g_game+0x519) is recomputed per call.
// Sixth session (deepseek-v4.1-flash): baseline re-confirmed at 85.2% / 1595
// bytes. Isolated the likely mechanism of the ebp/edi swap. The original stores
// n with "mov dword [esp+0x14],0" (immediate) and only then does "xor ebp,ebp"
// for the record offset and "mov edi,0x249" for the item offset. Ours emits
// "xor edi,edi" first and "mov [esp+0x14],edi", so edi is already live when the
// two offsets are coloured and the record offset inherits it, leaving the item
// offset with ebp. Every attempt to break that inheritance failed: an explicit
// running record byte offset (vE, 76.5%), off declared before n (vG, 78.7%, 1
// byte over), n declared at function scope (vI, 82.9%), n assigned with a plain
// statement (vJ, unchanged), n initialized from a folded expression (vK,
// unchanged), an inline Items() accessor (vF, unchanged), and the union
// bitfield for field_245 (v1, 82.9%). Making ONLY the flags load use an indexed
// base+off expression (the 85.2% file) is the best shape found. The bittest
// itself is stubborn: both "(x>>15)&1" and a 1-bit bitfield compile to
// "test ch,0x80", never the original's "shr ecx,0xf / test cl,1"; the unsigned
// short, mask, and modulo spellings all folded identically, so this is MSVC's
// simplification of a known single-bit mask, not a source lever.
// Seventh pass (deepseek-v4.1-flash, issue 3390): baseline 85.2% (1595 bytes) kept.
// The field_245 test as the 1-bit union bitfield scores 82.9% (1592 bytes), and
// making the item pointer dead after the name test (FUN_004c5740 source argument
// respelled as g_game->items + off + 0xa0 instead of (char*)item + 0xa0) scores
// 84.3% (1608 bytes), so the live item pointer alone is not what blocks the
// original's `add esi,0x20` in place of `lea ecx,[esi+0x20]`.
// Eighth pass (deepseek-v4.1-flash): naming the shifted flags value
// (`unsigned int bits = dword >> 15; if ((bits & 1) == 0 ...)`) is byte-identical
// to the inline `((x >> 15) & 1) == 0` at 85.2% / 1595 bytes, still
// `test ch,0x80`, so the fold is not a statement-shape artifact either.
// Ninth pass (deepseek-v4.1-flash): baseline 85.2% / 1595 bytes kept. Inlining
// `flag == 0` in the three FUN_004a1250 calls instead of the `enabled` local is
// byte-identical (85.2%, the one sete is still CSEd into esi), so the tail's
// `mov eax, [g_game]` schedule around `sete dl` is not source driven either.
// FUN_004a35a0(g_game->inner, ...) on this base scores 83.7% / 1594 bytes.
// Still differs: the scan loop's ebp/edi role swap (item offset ebp vs edi),
// the folded `test ch,0x80` vs the original's `shr ecx,0xf / test cl,1`, the
// name check keeping item live in esi (`lea ecx,[esi+0x20]`) where the original
// clobbers esi (`add esi,0x20` then re-materializes `mov esi,edx / add esi,edi`),
// and the tail schedule (n in edi vs a stack reload into esi, panel and g_game
// load order around the FUN_004a1250 pushes).

// Seventh session (deepseek-v4.1-flash, issue 3766): baseline 85.2% (1595
// bytes) kept. Naming the +0x245 flags load (`unsigned int flags`) with the
// (flags >> 15) & 1 and (flags & 0x8000) spellings is byte-identical at 85.2%,
// so the folded `test ch,0x80` and the ebp/edi role swap stay unreachable.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)

struct Unit_44c7e0 {
    char unknown_0[0x97];
    unsigned char field_97;            // +0x97
};

struct PlayerEntry_44c7e0 {
    Unit_44c7e0* unit;                 // +0x00
    char unknown_4[0x14b - 4];
};

struct Record_44c7e0 {                 // 0x62-byte slider/picture record
    char name[0x52];                   // +0x00 formatted text
    int field_52;                      // +0x52 unit type index
    int field_56;                      // +0x56
    int field_5a;                      // +0x5a
    int field_5e;                      // +0x5e
};

union Flags_44c7e0 {                   // the dword at +0x245
    unsigned int raw;
    struct {
        unsigned int low : 15;
        unsigned int flag : 1;         // bit 15
        unsigned int high : 16;
    } bits;
};

struct Item_44c7e0 {                   // 0x249-byte unit type instance
    char unknown_0[0x20];
    char name[0x166];                  // +0x20
    float field_186;                   // +0x186
    float field_18a;                   // +0x18a
    char unknown_18e[0x245 - 0x18e];
    Flags_44c7e0 field_245;            // +0x245
};

struct Gadget_44c7e0 {
    char unknown_0[0x1b];
    unsigned int field_1b;             // +0x1b
    char unknown_1f[0xba - 0x1f];
    short field_ba;                    // +0xba
    char unknown_bc[0xc0 - 0xbc];
    int count;                         // +0xc0
    char unknown_c4[0xce - 0xc4];
    void* field_ce;                    // +0xce
    Record_44c7e0* field_d2;           // +0xd2
    void* field_d6;                    // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x13c - 0xdc];
    int field_13c;                     // +0x13c
    short field_140;                   // +0x140
    char unknown_142[0x144 - 0x142];
    void* field_144;                   // +0x144
    short field_148;                   // +0x148
    int field_14a;                     // +0x14a
};

struct Info_44c7e0 {                   // 0x10-byte output of FUN_0046e330
    char unknown_0[0xa];
    short field_2;                     // +0xa
    int field_4;                       // +0xc
};

struct Inner_44c7e0 {
    char unknown_0[4];
    void* entries;                     // +0x04
};

struct Layer_44c7e0 {
    char unknown_0[4];
    void* entries;                     // +0x04
    void* handler_8;                   // +0x08
    int field_c;                       // +0x0c
    char unknown_10[0x1c - 0x10];
    void* handler_1c;                  // +0x1c
};

struct Game_44c7e0 {
    char unknown_0[0x519];
    char unknown_519[0x531 - 0x519];
    Inner_44c7e0* inner;               // +0x531
    char unknown_535[0x1b8a - 0x535];
    PlayerEntry_44c7e0 players[10];    // +0x1b8a, stride 0x14b
    char unknown_2878[0x2a30 - 0x2878];
    void* queue;                       // +0x2a30
    char unknown_2a34[0x2a42 - 0x2a34];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x1438f - 0x2a43];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Item_44c7e0* items;                // +0x1439b
};

struct Class_0046e330 {
    void FUN_0046e330(Item_44c7e0* item, Info_44c7e0* out);
};

#pragma pack(pop)

extern Game_44c7e0* g_game;
extern Record_44c7e0* DAT_005129b4;
extern int* DAT_005129b8;
extern int* DAT_005129c4;
extern int DAT_00512768;
extern int DAT_005129c0;

void* __stdcall FUN_004aa8f0(void* menu, const char* name, int size);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
Gadget_44c7e0* __stdcall FUN_0049ff90(void* entries, char* name);
int __stdcall FUN_0049fdf0(void* entries, char* name, int type);
Gadget_44c7e0* __stdcall FUN_004a0200(void* entries, char* name);
void __stdcall FUN_004a0bf0(void* menu, char* name, char* text, int flag);
void __stdcall FUN_004a1250(void* menu, char* name, int enabled);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text, int count, int flag);
void __stdcall FUN_004a35a0(void* entries, char* name, void* pics, int count);
void __stdcall FUN_004a81e0(void* menu, int value);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall FUN_0049fb10(void* menu, int value);
void __stdcall FUN_0045b9b0(Gadget_44c7e0* gadget, int value);
void __stdcall FUN_0044bfd0(void* menu, int value);
char* __stdcall FUN_004c5740(char* text);
void FUN_0044c370();
void FUN_0044c220();
void FUN_0044c420();
void FUN_0044be70();
int __cdecl FUN_0044c7a0(const void* a, const void* b);

// Retry by GPT-6.1-sol: baseline verified at 85.2% (1595 bytes). Six scored scratch variants
// using an aliased count, the union bitfield, an out-of-line count reader, a ternary fallback,
// an earlier count declaration, and nested scan guards all stayed at 85.2%. The previous
// session's register-allocation and tail-schedule differences remain unresolved.
// FUNCTION: 0x44c7e0
void FUN_0044c7e0()
{
    unsigned int flag = g_game->players[g_game->localPlayer].unit->field_97 & 1;

    Layer_44c7e0* layer = (Layer_44c7e0*)FUN_004aa8f0((char*)g_game + 0x519, "RESTRICT2.GUI", 0x880);
    layer->handler_8 = (void*)FUN_0044c420;
    layer->field_c = 0;
    layer->handler_1c = (void*)FUN_0044c220;
    DAT_00512768 = 0;
    FUN_004288d0("UnitRestrict5x", 0, 0, 0);

    void* entries = layer->entries;
    void* flags = FUN_004d83b0("FLAGS", g_game->count);
    Gadget_44c7e0* desc = FUN_0049ff90(entries, "DESCLIST");
    desc->field_ce = (void*)FUN_0044c370;
    desc->field_d6 = flags;
    desc->field_da = 0x20;
    desc->field_1b |= 0x100;

    Gadget_44c7e0* pic = FUN_0049ff90(layer->entries, "PICLIST");
    pic->field_d6 = flags;
    pic->field_1b |= 0x180;
    pic->field_da = desc->field_da;

    int* picArray = (int*)FUN_004d83b0("UNITPICARRAY", g_game->count * 0x18);
    memset(picArray, 0, g_game->count * 0x18);

    char* textArray = (char*)FUN_004d83b0("UNITTEXTARRAY", g_game->count << 5);
    *(int*)textArray = 0;

    DAT_005129b4 = (Record_44c7e0*)FUN_004d83b0("UNITSRESTRICTINFO", g_game->count * 0x62);
    desc->field_d2 = DAT_005129b4;
    int i;
    for (i = 0; i < g_game->count; i++)
        DAT_005129b4[i].field_52 = 0;

    DAT_005129b8 = (int*)FUN_004d83b0("UNITSPICS", g_game->count << 2);
    memset(DAT_005129b8, 0, g_game->count << 2);
    memset(DAT_005129b4, 0, g_game->count * 0x62);
    DAT_005129c4 = (int*)FUN_004d83b0("OLDCOUNTS", g_game->count << 2);

    int n = 0;
    i = 1;
    if (g_game->count > 1) {
        int off = 0x249;
        do {
            Item_44c7e0* item = (Item_44c7e0*)((char*)g_game->items + off);
            if ((((*(unsigned int*)((char*)g_game->items + off + 0x245)) >> 15) & 1) == 0 &&
                item->name != 0) {
                Info_44c7e0 info;
                sprintf((char*)&DAT_005129b4[n], "%s\r%s %dM  %dE",
                        (char*)g_game->items + off, FUN_004c5740((char*)item + 0xa0),
                        (int)item->field_18a, (int)item->field_186);
                DAT_005129b4[n].field_52 = i;
                ((Class_0046e330*)g_game->queue)->FUN_0046e330((Item_44c7e0*)((char*)g_game->items + off), &info);
                int value = info.field_4;
                if (value == -1)
                    value = 0x65;
                DAT_005129b4[n].field_5a = value;
                DAT_005129c4[n] = value;
                DAT_005129b4[n].field_5e = info.field_2;
                n++;
            }
            i++;
            off += 0x249;
        } while (i < g_game->count);
    }

    qsort(DAT_005129b4, n, 0x62, FUN_0044c7a0);

    char* dst = textArray;
    for (i = 0; i < g_game->count; i++) {
        strcpy(dst, DAT_005129b4[i].name);
        dst += strlen(DAT_005129b4[i].name) + 1;
    }

    for (i = 0; i < 0xc; i++) {
        char buf[0x14];
        sprintf(buf, "SLIDER%d", i);
        Gadget_44c7e0* slider = FUN_004a0200(entries, buf);
        slider->field_14a = (int)slider;
        slider->field_13c = 0x65;
        slider->field_144 = (void*)FUN_0044be70;
    }

    void* scrollPanel = (char*)g_game + 0x519;
    void* innerEntries = g_game->inner->entries;
    int idx = FUN_0049fdf0(innerEntries, "SCROLLSLIDER", 0xe);
    if (idx != -1) {
        Gadget_44c7e0* scroll = FUN_004a0200(innerEntries, "SCROLLSLIDER");
        scroll->field_13c = 0xd2;
        scroll->field_144 = (void*)FUN_0044bfd0;
        scroll->field_140 = 0;
        FUN_0045b9b0(scroll, 0);
        scroll->field_14a = (int)g_game;
    }
    FUN_0044bfd0((char*)g_game + 0x519, idx);
    FUN_0049fa90((char*)g_game + 0x519);

    FUN_004a32a0((char*)g_game + 0x519, "DESCLIST", textArray, n, 0);
    FUN_004a35a0(g_game->inner->entries, "PICLIST", picArray, n);
    FUN_0044bfd0((char*)g_game + 0x519, 0);

    void* panel = (char*)g_game + 0x519;
    Item_44c7e0* item = &g_game->items[desc->field_d2[desc->field_ba].field_52];
    {
        char buf[0x14];
        sprintf(buf, "%d", (int)item->field_186);
        FUN_004a0bf0(panel, "ENERGYTEXT", buf, 0);
        sprintf(buf, "%d", (int)item->field_18a);
        FUN_004a0bf0(panel, "METALTEXT", buf, 0);
    }

    int enabled = (flag == 0);
    FUN_004a1250((char*)g_game + 0x519, "Load", enabled);
    FUN_004a1250((char*)g_game + 0x519, "Save", enabled);
    FUN_004a1250((char*)g_game + 0x519, "Reset", enabled);
    FUN_004a81e0((char*)g_game + 0x519, 0x40);
    FUN_0049fb10((char*)g_game + 0x519, 1);
    DAT_005129c0 = 0;
}
