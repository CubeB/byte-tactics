// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
// Third pass (deepseek-v4.1, issue 1962): 79.3 kept as best. Flipped the reindex loop address spelling three ways (g_game + 0x1b63 + off, off + g_game + 0x1b63, g_game + (0x1b63 + off)); all emit the same `[eax + ecx + 0x1b63]` SIB with base=off/eax while the original encodes base=g_game/ecx, although g_game is in ecx in ours too, so the base/index pick is not reachable from the expression form. Everything below still stands.

// Fourth pass (space-bunny-free, issue 1962): confirmed 79.3% with a real
// check.py run and could not move it. Root cause of the largest remaining
// cluster (four diffs in three blocks) identified below: `pl` is only ever
// USED through its own address at one point, 0x44a759 `mov al,[ebp+0x22]`
// right after 0x44a749 `lea ebp,[esi+eax*2+0x1b63]`. MSVC 5 folds that
// single read back into the g_game-relative form
// (`mov cl,byte ptr [esi+eax*2+0x1b85]`), so the register live range of `pl`
// has a hole from 0x44a750 to 0x44a933 and the allocator never gives it ebp.
// The original's ebp range is continuous, which is why it reloads
// `mov ebp,[esp+0x14]` at 0x44a92f and uses `[ebp+0x27]` at 0x44a956 and
// 0x44ac23. Since the fold is what breaks the range, every change to pl's
// type or spelling is a dead end; it needs a source form in which the first
// read cannot be re-derived from g_game, which MSVC 5's CSE does not permit
// (brief item 18: an inlined function boundary is not a CSE boundary).
// Tried this pass, all scored with check.py --sym, none above 79.3:
// - pl: reading the field through a named byte local (79.3, no change),
//   `&pl->field_22` (79.3), assigning `pl` at the top of the function so the
//   range starts earlier (74.8, the whole block moves), and a second identical
//   pointer value to defeat the CSE (15.9, the compiler duplicates the body).
// - MAXUNITS/METAL/ENERGY: one named local per value (78.1), only the maxunits
//   value (79.3, no change), and reading all three through the existing `u2`
//   instead of repeating `g_game->players[b2].data` (77.7). The original uses
//   edi,edi,esi for those three temporaries and we use edx,edx,eax: the value
//   temporaries want callee-saved registers and eax is free in our version.
// - the reindex loop: reversing every addend order so the compiler picks
//   base=g_game/index=off (79.3, it ignores the addend order), and spelling
//   the byte store as `((unsigned char*)(g_game+0x1b63+off))[0x146]` so the
//   address tree differs from the compare (79.3). The extra
//   `lea edx,[eax+ecx+0x1ca9]` is a speculative hoist MSVC emits for the
//   `= i` store only; the `= 10` store never uses it, in ours or the original.
// - the `w[0x63] < *(*(unsigned short**)(w+0xbe))-1` compare: naming both
//   operands as locals gives the exact original size, 2340 bytes, but scores
//   72.7 because the surrounding block then reorders.
// Second worker pass (deepseek-v4.1, issue 1962): 79.0 -> 79.3, still 2333 bytes
// against the original 2340. Fixed here: the LOGO text rect pair must be
// (rect.left + rect.right) for x but (rect.bottom + rect.top) for y, which
// pins both [esp+0x38]/[esp+0x40] load orders (the mixed form is the only one
// that leaves no diff in that hunk), and the reindex loop wants the dword at
// +0x1b63 tested before the byte at +0x1bd6 is read, which a nested
// `if (v != 0) { ... }` with the byte local inside it buys (equal score, but
// the load order then matches the original).
// Still differing, in size order:
// - `pl` never gets ebp (original: lea ebp / mov [esp+0x14],ebp / mov al,[ebp+0x22],
//   then reloads ebp from [esp+0x14] at 0x44a92f and uses [ebp+0x27] at 0x44a956
//   and 0x44ae00). Ours folds the field_22 load into [esi+eax*2+0x1b85] and keeps
//   pl in its slot, so every pl use costs an extra reload. Tried this pass:
//   `g_game->players + g_game->localPlayer` (byte-identical output, no move).
// - the DAT_00512994 branch: original falls through into `call FUN_004455b0`
//   and jumps over the compaction loop (`jne 0x44a7ec / call / jmp 0x44a933`);
//   ours puts the call at the join point (0x44a92f), so the loop-exit reload
//   `mov ebp,[esp+0x14]` has no counterpart. Inverting the test (the obvious
//   way to force that layout) was already measured at 68.7, so it stays.
// - the compaction reindex loop: ours still materialises
//   `lea edx,[eax+ecx+0x1ca9]` for the byte store (the original issues three
//   separate base+index+disp references) and emits base=off/index=g_game
//   instead of the original's base=g_game/index=off.
// - the first `if`: original hoists `mov eax,[g_game]` between the test and the
//   jne; ours sinks it into the fall-through. Same for a couple of scheduler
//   swaps (LOGO `mov edx,[esp+0x18]` before/after the `lea ecx,[esp+0x34]`,
//   the tail's `xor ebx,ebx` before/after the pl reload) and the mirrored
//   edx/ecx pick in the `w[0x63] < ... - 1` compare.
// Base by deepseek-v4.1-flash, space-bunny-free and GPT-6; continued by deepseek-v4.1.
// Gave up at 79.0% (2333 bytes against 2340). The 1-bit bitfield at
// Unit+0x9d bit 2 fixed the tail; what is left is the initial local-slot
// order, the player-compaction register allocation (original keeps the
// player pointer in ebp and `entries` in ebx; ours folds the address) and
// branch placement. Also left: the `xor ebx,ebx` for the bitfield boolean is
// scheduled after the [esp+0x14] reload instead of before it.
// Tried and did NOT work (do not repeat): computing the boolean into a local
// first (77.6), swapping the LOGO rect pair to (right+left - w) (78.7),
// inverting the DAT_00512994 test so FUN_004455b0 is the fall-through (68.7).
// Second pass (space-bunny-free), all scored with check.py --sym on scratch
// copies, all 78.2 to 79.0 or worse, none above the 79.0 baseline:
// - reindex loop: hoisting a Player* induction variable instead of a raw
//   offset (68.5), `g_game->players[i]` with `i <= 10` (78.2), fully inlined
//   field expressions with no temporaries (78.2), field_73 read inline but
//   field_0 in a local (78.2), the whole condition nested inside
//   `if (v != 0)` (79.0, no change), routing the compare and both stores
//   through one `unsigned char* f146` (78.3), and a `char* q` pointer walk
//   with `q += 0x14b` (65.9). The raw-offset form with the off <= 0xcee
//   bound is the best of these; the pointer forms lose the index+displacement
//   addressing the original uses.
// - `pl` in a callee-saved register: routing its field reads through a
//   boxed `__inline` accessor struct (78.7), spelling it `(Player*)pl` or
//   `*(char*)((char*)pl+0x22)` (79.0, no change), assigning it through a
//   second named pointer (79.0, no change), swapping the declarations of
//   `pl` and `entries` (79.0, no change), and hoisting the `entries` load
//   above the `pl` definition (77.8). None of these move MSVC off the stack
//   slot, so the [esp+0x14] reload pattern persists.
// - the MAXUNITS/METAL/ENERGY block: hoisting all three values into named
//   locals (77.6) and hoisting only the maxunits one (79.0, no change).
// - splitting the first `if` into `if (...) {} else if (...)` so the g_game
//   reload lands before the test (79.0, no change).
#include <string>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
struct Unit_44a680 {
    char name[0x97];
    unsigned char flags;          // +0x97
    char unknown_98[0x9d - 0x98];
    unsigned short low2_9d : 2;   // +0x9d bits 0-1
    unsigned short bit2_9d : 1;   // +0x9d bit 2
    unsigned short high13_9d : 13;
    char unknown_9f[0xa1 - 0x9f];
    unsigned short energy;        // +0xa1
    unsigned short metal;         // +0xa3
    unsigned short maxunits;      // +0xa5
    unsigned char field_a7;       // +0xa7
    unsigned char field_a8;       // +0xa8
    int field_a9;                 // +0xa9
};

struct Player_44a680 {
    int field_0;                  // +0x00
    char unknown_4[0x22 - 4];
    unsigned char field_22;       // +0x22
    char unknown_23[0x27 - 0x23];
    Unit_44a680* data;            // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char field_73;       // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;      // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Table_44a680 {
    int unknown_0;
    void* entries;                // +0x4
};

struct Gui_44a680 {
    char unknown_0[0x18];
    Table_44a680* table;          // +0x18
};

struct Game_44a680 {
    char unknown_0[0x519];
    Gui_44a680 gui;               // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_44a680 players[10];    // +0x1b63
    char unknown_2851[0x2a30 - 0x2851];
    void* field_2a30;             // +0x2a30
    char unknown_2a34[0x2a3c - 0x2a34];
    unsigned short field_2a3c;    // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;    // +0x2a42
    char unknown_2a43[0x2bc0 - 0x2a43];
    unsigned char field_2bc0;     // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    unsigned short field_2bee;    // +0x2bee
    char unknown_2bf0[0x38a47 - 0x2bf0];
    int field_38a47;              // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    void* field_391e9;            // +0x391e9
};
#pragma pack(pop)

struct Class_004358f0 { int FUN_004358f0(); };
struct Class_004373a0 { int FUN_004373a0(); };
struct Class_00435a20 { void FUN_00435a20(Unit_44a680* unit); };
struct Class_00435c30 { char* FUN_00435c30(); };
struct Class_0046e000 { int FUN_0046e000(); };
struct Class_00463c60 { void FUN_00463c60(int param); };

struct Rect_44a680 { int left, top, right, bottom; };

struct Flag2bee_44a680 {
    unsigned short bit0 : 1;
    unsigned short rest : 15;
};

extern Game_44a680* g_game;
extern int DAT_00512994;
extern int DAT_005129a4;
extern unsigned int DAT_005129a8;
extern unsigned int DAT_0050550c;

extern "C" {
int __stdcall FUN_00453d40();
unsigned char __stdcall FUN_00456850();
void __stdcall FUN_004455b0();
void __stdcall FUN_00448c70();
void FUN_00444a20();
void __stdcall FUN_00445b70(Gui_44a680* gui, int index);
void __stdcall FUN_00445c70(Gui_44a680* gui, int index);
void __stdcall FUN_0047f1a0(char* name, int param);
void __stdcall FUN_0049fa90(Gui_44a680* gui);
void __stdcall FUN_0049fad0(Gui_44a680* gui);
int __stdcall FUN_0049fdf0(void* entries, char* name, int type);
void __stdcall FUN_004a0570(Gui_44a680* gui, char* name, int param);
void __stdcall FUN_004a0bf0(Gui_44a680* gui, char* name, char* text, int param);
void __stdcall FUN_004a1250(Gui_44a680* gui, char* name, int param);
void* __stdcall FUN_004a0200(void* entries, char* name);
void* __stdcall FUN_004a0280(void* entries, char* name);
void __stdcall FUN_004a15c0(void* entries, int widget, Rect_44a680* rect);
int __stdcall FUN_004a5030(char* text);
int __stdcall FUN_004a50b0();
void __stdcall FUN_004a50e0(int a, char* text, int x, int y, int w, int h);
void __stdcall FUN_004a5d30(Gui_44a680* gui, int flag);
void __stdcall FUN_004a9660(void* gui);
int __stdcall FUN_004ab060(Gui_44a680* gui, char* name);
void __stdcall FUN_0045b9b0(void* entry, int value);
int __stdcall FUN_0045ba20(void* entry);
unsigned int __stdcall FUN_004b6340();
unsigned char __stdcall FUN_0041d6a0(int param);
void __stdcall FUN_00456310();
void __stdcall FUN_00450f90();
void __stdcall FUN_00451180();
int __stdcall FUN_00456760();
}
class Class_0046dad0 { public: void FUN_0046dad0(); };


// FUNCTION: 0x44a680
void FUN_0044a680()
{
    unsigned char b;
    unsigned int edi;
    Unit_44a680* unit;
    Player_44a680* pl;
    void* entries;

    g_game->field_38a47++;

    if (FUN_00453d40() != 0 ||
        ((Class_004358f0*)g_game->field_391e9)->FUN_004358f0() == 0) {
        ((Flag2bee_44a680*)((char*)g_game + 0x2bee))->bit0 = 1;
    } else {
        b = FUN_00456850();
        unit = 0;
        edi = 0;
        if (b != 10) {
            unit = g_game->players[b].data;
            if (unit->field_a7 >= 2)
                edi = 1;
            else if (unit->field_a7 == 1 && unit->field_a8 >= 2)
                edi = 1;
        }
        if (edi != 0 && ((Class_004373a0*)g_game->field_391e9)->FUN_004373a0() != unit->field_a9)
            ((Flag2bee_44a680*)((char*)g_game + 0x2bee))->bit0 = 1;
    }

    pl = g_game->players + g_game->localPlayer;
    if (pl->field_22 != 0) {
        g_game->field_2bc0 = 3;
        FUN_004a9660(&g_game->gui);
        return;
    }

    entries = g_game->gui.table->entries;
    if (FUN_004ab060(&g_game->gui, "LOUNGE2.GUI") != 0) {
        int idx = FUN_0049fdf0(entries, "PLAYER0", 0xe);
        *(int*)((char*)entries + idx * 0x15b + 0x1f) = 0x18;
    }

    if (((Flag2bee_44a680*)((char*)g_game + 0x2bee))->bit0 != 0) {
        if (DAT_00512994 != 0) {
            Player_44a680* A = &g_game->players[0];
            Player_44a680* B = &g_game->players[1];
            Player_44a680* end = (Player_44a680*)((char*)g_game + 0x2851);
            Player_44a680* savedA;

            for (;;) {
                if (B >= end && A >= end)
                    break;
                while (((A->field_0 != 0 &&
                         (A->field_73 == 1 || A->field_73 == 2 || A->field_73 == 3) &&
                         A->field_146 != 10) ||
                        A->field_73 == 4) &&
                       A < end)
                    A++;
                savedA = A;
                B = A + 1;
                while (!(B->field_0 != 0 &&
                         (B->field_73 == 1 || B->field_73 == 2 || B->field_73 == 3) &&
                         B->field_146 != 10) &&
                       B < end)
                    B++;
                if (B >= end || A >= end)
                    break;
                {
                    Player_44a680 tmp = *A;
                    *A = *B;
                    *B = tmp;
                }
                ((Class_00463c60*)B)->FUN_00463c60(0);
                B->field_0 = 0;

                int i = 0;
                for (int off = 0; off <= 0xcee; off += 0x14b, i++) {
                    int v = *(int*)((char*)g_game + (0x1b63 + off));
                    if (v != 0) {
                        unsigned char f73 = *(unsigned char*)((char*)g_game + (0x1bd6 + off));
                        if ((f73 == 1 || f73 == 2 || f73 == 3) &&
                            *(unsigned char*)((char*)g_game + (0x1ca9 + off)) != 10)
                            *(unsigned char*)((char*)g_game + (0x1ca9 + off)) = (unsigned char)i;
                        else
                            *(unsigned char*)((char*)g_game + (0x1ca9 + off)) = 10;
                    } else {
                        *(unsigned char*)((char*)g_game + (0x1ca9 + off)) = 10;
                    }
                }
                A = savedA;
            }
        } else {
            FUN_004455b0();
        }

        if ((unsigned int)g_game->field_2a3c != DAT_0050550c) {
            DAT_0050550c = g_game->field_2a3c;
            FUN_00451180();
        }

        if ((pl->data->flags & 1) == 0) {
            unsigned char b2 = FUN_00456850();
            if (b2 != 10) {
                if (FUN_004ab060(&g_game->gui, "LOUNGE2.GUI") != 0) {
                    Unit_44a680* u2 = g_game->players[b2].data;
                    ((Class_00435a20*)g_game->field_391e9)->FUN_00435a20(u2);
                    FUN_0045b9b0(FUN_004a0200(g_game->gui.table->entries, "MAXUNITS"), g_game->players[b2].data->maxunits - 0x14);
                    FUN_0045b9b0(FUN_004a0200(g_game->gui.table->entries, "METAL"), g_game->players[b2].data->metal * 100);
                    FUN_0045b9b0(FUN_004a0200(g_game->gui.table->entries, "ENERGY"), g_game->players[b2].data->energy * 100);
                    FUN_00445b70(&g_game->gui, 0);
                    {
                        Gui_44a680* energyPanel = &g_game->gui;
                        void* e = FUN_004a0200(g_game->gui.table->entries, "ENERGY");
                        if (e != 0) {
                            int v = FUN_0045ba20(e);
                            char buf[20];
                            int shown = v / 100 * 100;
                            Unit_44a680* lu;
                            _itoa(shown, buf, 10);
                            FUN_004a0bf0(energyPanel, "ENERGYTEXT", buf, 0);
                            lu = g_game->players[g_game->localPlayer].data;
                            lu->energy = (unsigned short)(shown / 100);
                            if (lu->flags & 1) {
                                FUN_00450f90();
                                FUN_00451180();
                            }
                        }
                    }
                    FUN_00445c70(&g_game->gui, 0);
                } else if (FUN_004ab060(&g_game->gui, "viewmap.gui") != 0) {
                    Unit_44a680* u2 = g_game->players[b2].data;
                    if (strcmp(((Class_00435c30*)g_game->field_391e9)->FUN_00435c30(), u2->name) != 0) {
                        ((Class_00435a20*)g_game->field_391e9)->FUN_00435a20(g_game->players[b2].data);
                        FUN_00444a20();
                        FUN_0049fad0(&g_game->gui);
                    }
                }
            }
        }
        g_game->field_2bee &= 0xfffe;
        if (FUN_004ab060(&g_game->gui, "LOUNGE2.GUI") != 0) {
        if (pl->data->flags & 1) {
            int edi2 = ((Class_0046e000*)g_game->field_2a30)->FUN_0046e000();
            int b5 = FUN_00456760();
            unsigned short* w;

            FUN_004a1250(&g_game->gui, "SYNCHING", 1);
            w = (unsigned short*)FUN_004a0280(entries, "battlestart");
            if ((short)w[0x63] > 0 && DAT_005129a4 < FUN_004b6340()) {
                if ((short)w[0x63] < 8) {
                    w[0x63] = w[0x63] + 1;
                    FUN_0049fa90(&g_game->gui);
                    ((Flag2bee_44a680*)((char*)g_game + 0x2bee))->bit0 = 1;
                }
                DAT_005129a4 += 4;
                if ((short)w[0x63] == 4)
                    FUN_0047f1a0("Panel", 0);
            }
            if ((short)w[0x63] != 0 &&
                (int)(short)w[0x63] < (int)(**(unsigned short**)((char*)w + 0xbe) - 1)) {
                ((Flag2bee_44a680*)((char*)g_game + 0x2bee))->bit0 = 1;
            }
            if (b5 != 0) {
                ((Flag2bee_44a680*)((char*)g_game + 0x2bee))->bit0 = 1;
                if ((short)w[0x63] == 0) {
                    w[0x63] = 1;
                    DAT_005129a4 = FUN_004b6340();
                    FUN_0047f1a0("Options", 0);
                }
                *(unsigned int*)((char*)w + 0xc8) &= 0xfffffffe;
                {
                    int idx = FUN_0049fdf0(entries, "START", 1);
                    unsigned int r = FUN_004b6340() & 0x1f;
                    if (r != *(unsigned int*)((char*)entries + idx * 0x15b + 0x1f)) {
                        *(unsigned int*)((char*)entries + idx * 0x15b + 0x1f) = r;
                        FUN_0049fa90(&g_game->gui);
                    }
                }
            }
            FUN_004a1250(&g_game->gui, "START", b5 == 0);
            FUN_004a0570(&g_game->gui, "START", edi2);
            FUN_004a0570(&g_game->gui, "SYNCHING", edi2 == 0);
        }
        FUN_00448c70();
        FUN_0049fa90(&g_game->gui);
        }
    }

    if (FUN_004ab060(&g_game->gui, "LOUNGE2.GUI") != 0) {
        FUN_004a5d30(&g_game->gui, 1);
        {
            int i = 0;
            int n = 10;
            do {
                Player_44a680* p = &g_game->players[i];
                if (p->field_73 != 0 && p->field_73 != 4) {
                    Rect_44a680 rect;
                    char buf[20];
                    int widget;
                    Unit_44a680* u;
                    int w;
                    int h;
                    sprintf(buf, "LOGO%i", i);
                    widget = FUN_0049fdf0(entries, buf, 0xe);
                    FUN_004a15c0(entries, widget, &rect);
                    u = p->data;
                    sprintf(buf, "%i.%i", u->field_a7, u->field_a8);
                    w = FUN_004a5030(buf);
                    h = FUN_004a50b0();
                    FUN_004a50e0(0, buf,
                                 (rect.left + rect.right - w) / 2,
                                 (rect.bottom + rect.top - h) / 2,
                                 w, 0);
                }
                i++;
                n--;
            } while (n != 0);
        }
        FUN_004a5d30(&g_game->gui, 0);
    }

    ((Class_0046dad0*)g_game->field_2a30)->FUN_0046dad0();
    if (DAT_005129a8 < (unsigned int)FUN_004b6340()) {
        unsigned char r;
        DAT_005129a8 = FUN_004b6340() + 0x3c;
        r = FUN_0041d6a0(1);
        unit = pl->data;
        unit->bit2_9d = (r != 0);
        FUN_00456310();
    }
}

// Remaining differences (best 79.3%, ours 2333 bytes vs original 2340):
// - Register allocation is one step off through the whole function. The
//   original holds `entries` in ebx and `pl` in ebp (it spills ebp at
//   0x44a750 and reloads it with `mov ebp,[esp+0x14]` at 0x44a92f); we hold
//   `entries` in ebx and spill `pl` to [esp+0x14], so the swap loop's A/end run
//   in ecx/edx like the original but `pl` costs a reload at 0x44a92f, 0x44a956
//   and 0x44ac23. See the top of the file: the fold of the one early
//   `pl->field_22` read is what keeps pl out of a register.
// - The first condition reloads g_game after the test in ours and before it
//   in the original (`mov eax,[g_game] / test eax,eax / jne`).
// - The reindex loop after each swap: the original tests the dword at
//   g_game+0x1b63+off before loading the byte at +0x1bd6+off and never
//   materialises the +0x1ca9 address; ours loads both up front and emits one
//   extra `lea edx,[eax+ecx+0x1ca9]`. Rewriting it as
//   `g_game->players[i]` with a do-while scored 73.4, so the raw offsets stay.
// - the `DAT_00512994` branch: re-measured this pass. Inverting the test to
//   `if (DAT_00512994 == 0) { FUN_004455b0(); } else { <loop> }` does give the
//   original's `test / jne LOOP / call / jmp JOIN` shape, but MSVC then sinks
//   the whole loop out of line (to 0x44ae58) instead of placing it just after
//   the call, and drops the `mov ebx,[esp+0x18] / mov ebp,[esp+0x14]` pair at
//   the loop exit, for 69.0%. Keeping `!= 0` costs only the two-instruction
//   hunk, so it stays.
// - `(r != 0) ? 4 : 0` at 0x44af80 compiles to neg/sbb/and; the original has
//   `test al,al / setne bl / and ebx,1 / shl ebx,2 / or edx,ebx`. A
//   `unsigned int bits; if (r) bits=1; else bits=0; bits<<=2;` self-correction
//   gives neg/sbb/neg/shl (2220 bytes, 74.4), `bits = (r != 0) & 1;
//   bits <<= 2;` gives neg/sbb/neg/and/shl (no setne), and a
//   `bool flag = (r != 0); ... ((unsigned)flag << 2)` gives neg/sbb/neg/movzx
//   (2224, 74.3). None reach setne. `r` must stay unsigned char (FUN_0041d6a0
//   returns it in al); if it is widened to int the `test al,al` is lost.
// - In the LOGO block the original computes the pair (left,right) as
//   `[esp+0x38]` into eax and `[esp+0x40]` into edx; ours has them swapped.
// - Tried and did NOT work (do not repeat): spelling
//   `g_game->gui.table->entries` inline at the MAXUNITS/METAL/ENERGY call
//   sites (73.6), dropping the `u2` local for inline
//   `g_game->players[b2].data` (72.0), both together (67.3). The original
//   really does reload those after every call, but forcing the reload by
//   spelling the expression out adds instructions and demotes `entries` out
//   of ebx.
// Suspected original bug: the reindex loop after each swap runs
// `off <= 0xcee` (11 iterations at stride 0x14b), so it reads and writes
// g_game+0x1ca9 one element past the 10-entry player array (players[] runs
// g_game+0x1b63 to g_game+0x2851, so +0x1ca9 with off=0xcee is
// g_game+0x2918). The reader of that byte, 0x44a7f2's loop bound
// `lea ebp,[edx+0x1cae]`, stops one element short, so the 11th write is dead.
