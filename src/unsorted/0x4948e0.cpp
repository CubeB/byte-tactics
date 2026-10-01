// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (issue #3454): tested the last untried lever below, folding
// `y += 0xf` into the loop initializer (headers read panel.top, `int y = panel.top
// + 0xf` after them).  It does remove the dead `mov [esp+0x18],ebx` store, but
// panel then lands at 0x14 (the i slot) instead of 0x18 and the function drops to
// 72.7%, so the 78.1% shape stands.
// STATUS (deepseek-v4.1-flash, this session): confirmed best is 78.1% (ours 1451
// vs original 1418), unchanged.  Still differs only in the y stack slot: ours
// gives y a home slot at 0x18 (dual register+memory tracking, dead store
// `mov [esp+0x18],ebx` right after `add ebx,0xf`, and home maintained in sync
// with ebx at the loop increment).  The original keeps y purely in ebx with NO
// home slot, so panel sits at 0x18 and every panel ref is 4 lower.  That home
// slot also lets MSVC mutate ebx (`add ebx,0x25`) and LICM/reorder the dst quad
// build before the search loop; the original uses `lea eax,[ebx+0x25]` and
// interleaves `lea esi`/`xor edi` mid-build.  Fixing the home slot should fix
// all three (offsets, dst scheduling, cleanup) at once.  Ideas tried this run:
// none new (timebox); the note below lists the many prior attempts.  New lever
// identified but untested: fold `y += 0xf` into the initializer (headers drawn
// at `panel.top`, loop y starts at `panel.top + 0xf`) to remove the dead store
// and force y to live only in ebx.
// SUPERSEDED (#3530): see the retry note further down, 78.3% with the player
// pointer hoisted above the dst quad build; everything else below still holds.
// Partial: 78.1% (real check.py run; 1451 bytes against the original 1418).
// Structure, both call sequences, the strcpy/sprintf buffer layout, the src
// quad and the whole player loop body below the panel rect are in place.  A
// single 4-byte stack-slot assignment is left.
//
// THE REMAINING PUZZLE, precisely: the original's local map (frame base = esp
// after the four saved-register pushes, frame 0xd0, so 0x00-0x0f is unused and
// 0xd0-0xdf are the saved registers) is
//   0x10 maxw, 0x14 i, 0x18 panel (4 dwords), 0x2c dst, 0x4c hr, 0x5c src,
//   0x7c buf
// with y in ebx and NO stack slot.  This version has
//   0x10 maxw, 0x14 i, 0x18 y, 0x1c panel, 0x2c dst, 0x4c hr, 0x5c src, 0x7c buf
// so y took a slot at 0x18 and pushed panel to 0x1c, moving every panel
// reference 4 bytes.  dst/hr/src/buf already agree.
//
// WHY y gets a slot here: this version stores y once, dead, right after
// `y += 0xf` (the original has the same pointless re-store of ebx at
// 0x494b6b), and the 0x1c panel then lands in the gap.  The original evidently
// kept y's home while never using it as one.  Register order is ESI,EDI,EBX,EBP:
// the original gives ESI to the player pointer, EDI to both loop counters, EBX
// to y and leaves maxw in memory; as soon as the player pointer loses ESI the
// whole chain shifts and y takes ESI, spills, and i is demoted with it.
//
// MEASURED THIS SESSION (build/scratch/0x4948e0):
//  - The cleanup counter at 0x28 is NOT a frame: the original's
//    `mov [esp+0x28],edi` at 0x494dcb is a fresh store of 10 (edi has just
//    exited the search loop with the value 10) and the latch at 0x494e33
//    decrements and re-stores the SAME slot.  Writing the cleanup as a
//    hand-enumerated `p[i]` walk reproduces the original's exactly
//    (`mov eax,[esp+0x14]; mov esi,ecx; and esi,0xff; cmp esi,edi / jle`,
//    with the plain `n < 10 / jne` search exit and the odd shared-slot reuse),
//    but it adds a g_game keep-alive in the outer loop that costs more than it
//    saves (v2: 74.5%, 1425 bytes).
//  - v2 with an outer-loop `for (;;)` / or with a hoisted `int n;` gives the
//    same edi counter and the same cleanup shape; neither removes y's slot.
//  - Caching `g_game->field_148db` in a local before the draw call (the
//    original's edx at 0x494c6d is live across the two rect draws and is NOT
//    reloaded after them, whereas this version reloads g_game after the second
//    FUN_004bf4d0) fixes that reload but costs 5 points elsewhere
//    (v6: 76.4%, 1459 bytes).
//  - register priority: ESI/EDI are already taken by the two counters in every
//    variant tried, so y cannot reach ebx by adding uses; it needs whichever
//    variable currently holds ESI to be pushed out first.
//
// Tried and worse previously: v7 cleanup (73.2), v7 + chained dst stores
// (66.2), v7 + panel.left+maxw + panel.top (66.2), the src-quad "fix" (77.1),
// char buf[84] (77.1), y += 0x28 in the increment clause (77.6), cleanup as an
// explicit else (73.2), hoisted `int n;` do-while pointer walk (72.3),
// `int y = 0x20;` (67.5), that plus hoisted-n (63.6), a file-scope
// `framepic` fed in both arms (66.0).  Earlier sessions: 67.5, 65.6, 69.5,
// 65.2, 66.6.

// Retry (deepseek-v4.1-flash, #3530): new best 78.3% (1451 bytes, original 1418).
// The gain came from declaring `Player_004948e0* p = g_game->players;` before the
// dst quad build instead of in the search-loop initialiser: the compiler then
// emits `lea esi,[edx+0x1b63]` in the middle of the dst stores exactly like the
// original at 0x494b88, and the `panel.right - 6` value moves from esi to edi.
// ebx is still mutated (`add ebx,0x25`) and y still takes the home slot at 0x18,
// so every panel reference stays 4 bytes high.
//
// Measured this session (all kept the y home slot, all in build/scratch/0x4948e0):
//  - dst x back to the original's `maxw + panel.left` (vA): 62.8%, 1456 bytes;
//    with the p-hoist too (vG): 54.7%, 1450 bytes.  In that form MSVC promotes
//    maxw out of memory into ebx, so y loses ebx entirely.
//  - `panel.left + 0x7d - 6` (vF, 3 live dst values instead of 4): 76.7%,
//    1447 bytes.  It STILL spills y, so the spill is not simple register
//    pressure: the allocator deliberately assigns the shared `y + 0x25` value to
//    ebx and keeps y in its home slot even when a register is free.
//  - dst stores written in the original's emission order
//    (p3.x,p0.x,p1.x,p2.x,p1.y,p0.y,p3.y,p2.y, vC): 77.6%, byte-identical size
//    to the current shape, MSVC's scheduler re-sorts it.
//  - the four dst.y stores before the four dst.x stores (vE): 77.3%.
//  - `int y;` declared beside `panel` and assigned after the panel.right store
//    (vB): byte-identical to the old 78.1% shape.
//  - pointer-walk cleanup (`for (k = n; k != 0; k--, q++)`, which reproduces the
//    original's 0x494dc5 block almost instruction for instruction, vH): 73.0%,
//    1428 bytes, only 10 bytes off the original size but the surrounding
//    register allocation (g_game leaves edx) costs more than the cleanup saves.
//
// Retry (deepseek-v4.1-flash): the y slot at 0x18 is forced by MSVC mutating
// ebx during the dst quad build (`add ebx,0x25`), where the original uses a
// scratch register (`lea eax,[ebx+1]` then `lea eax,[ebx+0x25]`).  Measured
// this session: a pointer-walk do-while cleanup matches the original's cleanup
// shape much more closely and shrinks us to 1426 bytes (original 1418) but
// reallocates g_game from edx to eax and scores 73.5%; the indexed for-j
// cleanup kept here scores 78.1% (1451 bytes).  Grouping the dst stores by
// expression scores 55.6% (maxw form) / 76.6% (right-6 form), x from
// panel.left+maxw scores 62.8%, and explicit y+1/y+0x25 temporaries are
// byte-identical to this version.  NOTE: ctx.py/objdump print the encoded
// [esp+N] displacement, so a live push shifts the real baseline slot by 4;
// maxw is baseline 0x10 and i baseline 0x14.
//
#include <string.h>

#pragma pack(push, 1)

struct Rect_004948e0 {
    int left;
    int top;
    int right;
    int bottom;
};

struct Point_004948e0 {
    int x;
    int y;
};

struct Quad_004948e0 {
    Point_004948e0 p[4];
};

struct Team_004948e0 {
    char unknown_0[4];
    unsigned char* data;               // +0x4
};

struct PlayerData_004948e0 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned char field_9b;            // +0x9b
};

struct Player_004948e0 {               // 0x14b bytes
    int field_0;                       // +0x0
    char unknown_4[0x27 - 4];
    PlayerData_004948e0* data;         // +0x27
    char name[0x48];                   // +0x2b
    unsigned char field_73;            // +0x73
    char unknown_74[0xfc - 0x74];
    short field_fc;                    // +0xfc
    short field_fe;                    // +0xfe
    char unknown_100[0x104 - 0x100];
    short field_104;                   // +0x104
    short field_106;                   // +0x106
    char unknown_108[0x140 - 0x108];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x148 - 0x147];
    unsigned char field_148;           // +0x148
    char unknown_149[0x14b - 0x149];
};

struct Game_004948e0 {
    char unknown_0[0x531];
    Team_004948e0* teams;              // +0x531
    char unknown_535[0x57d - 0x535];
    int team_index;                    // +0x57d
    char unknown_581[0x1b63 - 0x581];
    Player_004948e0 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x148db - 0x2a43];
    int field_148db;                   // +0x148db
    char unknown_148df[0x37ef6 - 0x148df];
    int field_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f06 - 0x37efa];
    unsigned char field_37f06;         // +0x37f06
};
#pragma pack(pop)

extern Game_004948e0* g_game;
extern unsigned char DAT_0051f2c8[10];
extern unsigned char DAT_0051e810[10];
extern int DAT_0051f2d8;
extern int DAT_0051f2f4;

unsigned int FUN_004b6340();
int FUN_004b6700();
void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_004c1b80(int key);
void __stdcall FUN_004bf4d0(void* surface, Rect_004948e0* rect, int level);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
int __stdcall FUN_004a5030(char* text);
char* __stdcall FUN_004c5740(char* name);
void* __stdcall FUN_004b7f30(void* glyphs, int c);
void __stdcall FUN_004c7580(void* surface, void* pic, Quad_004948e0* dst, Quad_004948e0* src);
int __cdecl sprintf(char* buf, char* fmt, ...);

// FUNCTION: 0x4948e0
void __stdcall FUN_004948e0(void* surface)
{
    if (DAT_0051f2f4 < (int)FUN_004b6340()) {
        DAT_0051f2f4 = FUN_004b6340() + 1;
        for (int i = 0; i < 10; i++) {
            if (DAT_0051f2c8[i] > 0)
                DAT_0051f2c8[i] -= 2;
            if (DAT_0051e810[i] > 0)
                DAT_0051e810[i] -= 2;
        }
    }

    if (!(g_game->field_37f06 & 0x80)
        && (FUN_004c1b80(0x20) == 0
            || (g_game->team_index != -1
                && ((unsigned char*)g_game->teams->data)[g_game->team_index * 0x15b] == 3))) {
        if (DAT_0051f2d8 <= 0)
            return;
        if (DAT_0051f2d8 == 0x7d)
            FUN_0047f1a0("Panel", 0);
        int q = DAT_0051f2d8 / 4;
        if (q <= 1)
            q = 1;
        DAT_0051f2d8 -= q;
        if (DAT_0051f2d8 <= 0) {
            DAT_0051f2d8 = 0;
            FUN_0047f1a0("Options", 0);
        }
    } else if (DAT_0051f2d8 < 0x7d) {
        if (DAT_0051f2d8 == 0)
            FUN_0047f1a0("Panel", DAT_0051f2d8);
        int q = (0x7d - DAT_0051f2d8) / 4;
        if (q <= 1)
            q = 1;
        DAT_0051f2d8 += q;
        if (DAT_0051f2d8 >= 0x7d) {
            DAT_0051f2d8 = 0x7d;
            FUN_0047f1a0("Options", 0);
        }
    }

    Rect_004948e0 panel;
    panel.left = FUN_004b6700() - DAT_0051f2d8;
    panel.top = 0x20;
    panel.right = panel.left + 0x7d;
    panel.bottom = g_game->numPlayers * 0x28 + 0x2e;
    FUN_004bf4d0(surface, &panel, -0x18);

    panel.right = panel.left + 0x7d;
    int y = panel.top;
    int maxw = panel.right - panel.left - 6;
    char buf[100];
    Quad_004948e0 src;
    src.p[0].x=1; src.p[0].y=1; src.p[3].x=1; src.p[1].y=1;
    strcpy(buf, FUN_004c5740("Kills"));
    FUN_004a50e0(surface, buf, panel.left + 2, y, maxw, 0);
    strcpy(buf, FUN_004c5740("Losses"));
    FUN_004a50e0(surface, buf, panel.right - FUN_004a5030(buf) - 2, y, maxw, 0);
    y += 0xf;

    for (int i = 0; i < (int)g_game->numPlayers; i++) {
        Player_004948e0* p = g_game->players;
        Quad_004948e0 dst;
        dst.p[0].x = panel.left + 7;
        dst.p[0].y = y + 1;
        dst.p[1].x = panel.right - 6;
        dst.p[1].y = y + 1;
        dst.p[2].x = panel.right - 6;
        dst.p[2].y = y + 0x25;
        dst.p[3].x = panel.left + 7;
        dst.p[3].y = y + 0x25;

        int n;
        for (n = 0; n < 10; n++, p++) {
            if (p->field_0 == 0)
                continue;
            unsigned char c = p->field_73;
            if (c != 1 && c != 2 && c != 3)
                continue;
            if (p->field_146 == 0xa)
                continue;
            if (p->field_144 == 0 && p->field_140 != 0)
                continue;
            if (p->data->field_9b & 0x40)
                continue;
            if (p->field_148 != i)
                continue;
            break;
        }
        if (n == 10) {
            for (int j = 0; j < 10; j++) {
                Player_004948e0* q = &g_game->players[j];
                if (q->field_0 == 0)
                    continue;
                unsigned char c = q->field_73;
                if (c != 1 && c != 2 && c != 3)
                    continue;
                if (q->field_146 == 0xa)
                    continue;
                if (q->field_144 == 0 && q->field_140 != 0)
                    continue;
                if (q->data->field_9b & 0x40)
                    continue;
                if ((unsigned)q->field_148 > (unsigned)i)
                    q->field_148--;
            }
            continue;
        }

        if (n == g_game->localPlayer) {
            Rect_004948e0 hr;
            hr.left = panel.left + 4;
            hr.top = y - 1;
            hr.right = panel.right - 4;
            hr.bottom = y + 0x26;
            FUN_004bf4d0(surface, &hr, 0x1f);
            FUN_004bf4d0(surface, &hr, 0x14);
        }
        unsigned short* frame = (unsigned short*)FUN_004b7f30(
            (void*)g_game->field_148db, p->data->field_96);

        src.p[1].x = frame[0] - 1;
        src.p[2].x = frame[0] - 1;
        src.p[2].y = frame[1] - 1;
        src.p[3].y = frame[1] - 1;
        FUN_004c7580(surface, frame, &dst, &src);

        FUN_004a50e0(surface, p->name, dst.p[0].x + 2, dst.p[0].y + 5, maxw, 0);
        int kills = g_game->field_37ef6 == 2 ? p->field_104 : p->field_fc;
        sprintf(buf, "%d", kills);
        FUN_004a50e0(surface, buf, dst.p[0].x + 2, dst.p[0].y + 0x14, maxw,
                     DAT_0051f2c8[n]);
        int losses = g_game->field_37ef6 == 2 ? p->field_106 : p->field_fe;
        sprintf(buf, "%d", losses);
        FUN_004a50e0(surface, buf, dst.p[2].x - FUN_004a5030(buf) - 2,
                     dst.p[0].y + 0x14, maxw, DAT_0051e810[n]);
        y += 0x28;
    }
}
