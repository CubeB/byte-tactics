// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Samples the ground under a unit at its four surrounding terrain
// vertices and stores the resulting pitch (0x68) and roll (0x70) on the
// unit, plus a heading (0x64) from the two side vertices. The 0x11/0x04
// bytes of a heightmap tile are the two half heights of its edge pair,
// and the corner heights are bilinearly interpolated with a plain / 16
// (MSVC 5 spells that cdq/and 0xf/add/sar 4; there is no second shift).
//
// PARTIAL (77.8%, 857 bytes against 857; Sonnet 5.5 retry #1091 took it from 70.8% with a scripted
// statement-order hill climb, see the end of this comment). Older notes below said 65.5%.
//
//  1. The bilinear block. The original keeps the first tile's low half in
//     ebx and spills the second tile pointer (esp+0x28) and b1 (esp+0x2c);
//     we keep the second tile pointer in ebx and spill b0 and b1 instead.
//     Both spill two values, so the instruction counts agree, only the
//     choice of which value the allocator drops differs. The loads
//     themselves are in the original's order (b0, b1, c0, c1) and every
//     spelling of the two H0/H1 expressions I tried gave the same code.
//  2. The loop back edge. The original carries the corner-array pointer in
//     ecx and the height-record pointer in ebx across the back edge, so
//     its reload block is only the row and map pointers (0x48a4ed,
//     0x48a4f1). We reload all three, so we emit one `mov edx, [esp+0x18]`
//     the original does not. An explicit `int*` induction variable does not
//     change this: MSVC 5 folds it back into the frame slot either way.
//  3. Small register swaps that follow from the two above: the map pointer
//     in the prologue (edx against ecx), the height max (eax against ecx),
//     and `mov esi,2 / sub esi,eax` against `mov ecx,2 / sub ecx,eax`.
//
// What did work, in order of size, and worth keeping:
//
//  * The loop has to be a `for (k = 0; k < 4; k++)`. As a `do { } while
//    (++k < 4)` MSVC 5 rotates it the other way, duplicates the first block
//    of the body into the preheader and the function comes out 174 bytes
//    too long at 39%. That single change is worth 14 points.
//  * The roll and pitch arguments are `(h0+h1)/2` and `(h2+h3)/2`, not
//    `/2/2` twice. The original has three cdq/sub pairs and three sars for
//    the whole tail; `/2/2` on each half makes MSVC 5 emit six.
//  * The 64 bit part has to be one expression. Written as three statements
//    on an `__int64 l`, MSVC 5 keeps the variable live and stores its high
//    dword (an extra `mov [esp+..], edx`); folded into
//    `2 - (int)((((__int64)q << 16) / s) * 2 >> 16)` the value is dead
//    after the low half is taken. 58.6% to 62.3%.
//  * `p` is a `short`, not an int. `short p = (short)(... + u->fix_lo)` is
//    the only spelling that gives the original's 16 bit `add ax, [u+0xaa]`
//    followed by a plain `mov [esp+0x28], eax`: the int versions either
//    sign-extend the addend first or emit a `movsx` after it. 62.3% to
//    63.5%.
//  * The second tile is `tb + g_game->gridW * 13` rather than `tb + gw * 13`
//    (gw is a local), and hz is computed before wx. Together these are what
//    bring the function to within one byte of the original's 857.
//  * `u->type->sight / 2`, not `abs(sight) / 2`. The cdq/sub pair at
//    0x48a695 is MSVC 5's signed /2 with its truncation fixup, deferred
//    past the `sar esi,1` at 0x48a69d; abs() would be cdq/xor/sub.
//  * The heading's second argument is `abs(pts[0].x - pts[1].x) >> 16`
//    and the pitch's is `abs(pts[0].z - pts[3].z) >> 16`, both with no /2,
//    and both `abs()` (cdq/xor/sub, guide's abs note) rather than a
//    hand-written test.
//
// The frame model, read off the disassembly and confirmed by the offsets
// our code now uses: locals run from esp+0x10 to esp+0x8b, with k at
// +0x10, fz (reused as the max() temporary) at +0x14, the corner-array
// pointer at +0x18, the height-record pointer at +0x1c, the spilled row
// and map-info pointers at +0x20 and +0x24, two scratch dwords at +0x28
// and +0x2c, hz at +0x30, the {x,z} pair handed to 0x4b7173 at +0x34, the
// four 8-byte corner positions at +0x3c and the four 12-byte height
// records at +0x5c. The height record is {wx, h, spare} and its .spare is
// hz, not junk: +0x30 is written at 0x48a54e (with the two call arguments
// still on the stack, so that instruction's esp+0x38 is esp+0x30) and read
// back at 0x48a736. The epilogue reads .h at +0x60, +0x6c, +0x78, +0x84.

//
// deepseek-v4.1 (1208): re-checked from the 77.8% partial; no source shape found that fixes the
// remaining hunks. What still differs, and what was tried: (1) the prologue register roles (m in edx
// and count in ecx in the original, m in ecx and count in edx here) and everything downstream of
// them (the two lea/pointer temps, the pts pointer ecx-vs-edx, H vs sea in the max block, the
// add eax,ecx operand order) cascade from that one allocation choice; retried with maps held in a
// named local, with row computed via m->rows[m->count], and with the pair declared in both orders,
// all scored the same or lower. (2) The p computation: the original keeps `and eax,0x1f` on the
// random value and pushes [esp+0x28] with a plain 32 bit load; declaring p as short with
// FUN_004b7123's first parameter short drops the movsx (856 bytes, 77.0%) but the mask stays dead,
// and int p (75.1%), a separate rand temp (66.4%) and a split p = (short)(p + fix_lo) statement
// (75.8%) all score lower than the present 77.8%. (3) Moving the n = frame - age statement back
// after the 64 bit division, with and without a cached owner pointer, drops to 73.9% even though the
// original schedules it there.
//
// deepseek-v4.1 (1208), second pass: re-scored the whole diff and the mask/movsx hunk. The original's
// p pair (`and eax,0x1f`, then a 16 bit `add ax, [u+0xaa]`, then a dword store and a plain dword
// reload) cannot be produced by aliasing p: `union { int i; unsigned short s; } p; p.i = ...;
// p.s += u->fix_lo;` and `*(unsigned short*)&p += u->fix_lo;` both give 867 bytes at 71.9 (taking p's
// address forces extra spills), so the mask stays dropped here. wx-before-hz scores 76.3 alone and
// 72.5 with n moved after the 64 bit block, so the present order (hz then wx, n before q, which keeps
// q = owner->sight and n = frame - age in the same blocks the original schedules them) stays best.
// Still differs: the m/count ecx/edx swap in the prologue and everything downstream of it, the
// dropped mask, and the p reload spelled movsx instead of a plain dword load.
//
// Sonnet 5.5 retry (#1091), 70.8% to 77.8%: a hill climb over statement positions (moving one
// statement of the loop body or of the sea-level block at a time, with every statement that
// touches the same local kept in order and no statement crossing the call it depends on) found two
// moves worth 3 points each: `int gw = g_game->gridW;` goes before the two bounds checks, and
// `unsigned n = g_game->frame - u->owner->age;` goes up next to `p`. What still differs, in
// order of the diff: (1) m and count swap ecx/edx in the prologue (original: m in edx, count in
// ecx; int n / row-first / return-first spellings did not flip it); (2) the original computes wx
// before hz (edx = t.x + posx, ebp = wx, then hz in ecx) but every wx-first order scores lower
// overall; (3) the original keeps `and eax, 0x1f` on the random value and passes p to
// FUN_004b7123 with a plain `mov eax, [esp+0x28]; push eax`, where this build drops the mask (the
// short cast makes it dead) and emits `movsx eax, word ptr [esp+0x28]`; short, unsigned short and
// int spellings of p and of that callee's first parameter all fail to give the pair;
// (4) u->owner is cached in ebp after the call in the original.
// deepseek-v4.1-flash retry (10 min timebox): confirmed 77.8% is the wall. Tested and all tied or lost:
// (a) index variable (ushort and uint) before the map lookup, maps array in a named local, address-of
// form, reference form, m declared first, count kept in a local int: m stays in ecx and count in edx,
// 77.8% or lower. The m-in-edx assignment is a pure allocator tie-break that nothing in the source
// shape flipped; every downstream hunk (pts pointer, wx/hz order, H vs sea registers, operand order)
// cascades from it. (b) p as two statements (short p = ...; p += u->fix_lo), p as int with a
// truncating cast, p as unsigned short, and the callee first parameter as short: 75.8/75.1/67.5/77.0.
// The original's kept `and eax,0x1f` plus a 16-bit `add ax,[u+0xaa]` plus a plain dword reload can
// only come from a 32-bit variable whose low half is added in place, which needs an address-taken
// union and that spills (71.9%, tried before). (c) Moving n after the 64-bit division to match the
// original schedule drops to 73.9/73.6 even with the owner cached in a local. What still differs:
// items 1 and 3 of the first comment (prologue m/count ecx/edx swap and its cascade, the dropped
// mask and the movsx reload of p).
#include <stdlib.h>

#pragma pack(push, 1)

struct MapVertex_0048a490 {
    int x;                              // +0x0
    int y;                              // +0x4
    int z;                              // +0x8
};

struct MapRow_0048a490 {
    char unknown_0[0xc];
    unsigned short* ids;                // +0xc
    char unknown_10[0x20 - 0x10];
};

struct MapInfo_0048a490 {
    char unknown_0[0xc];
    int count;                          // +0xc
    char unknown_10[0x24 - 0x10];
    MapVertex_0048a490* verts;           // +0x24
    MapRow_0048a490* rows;               // +0x28
};

struct UnitType_0048a490 {
    char unknown_0[0x192];
    int sight;                          // +0x192
    char unknown_196[0x241 - 0x196];
    unsigned int flags;                 // +0x241
};

struct PlayerRec_0048a490 {
    char unknown_0[0x20];
    int sight;                          // +0x20
    char unknown_24[0x2a - 0x24];
    int age;                            // +0x2a
};

struct Pos2_0048a490 {
    int x;
    int z;
};

struct Hs_0048a490 {
    int wx;                             // +0x0
    int h;                              // +0x4
    int spare;                          // +0x8
};

struct Unit_0048a490 {
    PlayerRec_0048a490* owner;           // +0x0
    char unknown_4[0x64 - 4];
    unsigned short hdg;                 // +0x64
    unsigned short aim;                 // +0x66
    unsigned short pitch;               // +0x68
    int posx;                           // +0x6a
    unsigned short posy_lo;             // +0x6e
    unsigned short roll;                // +0x70
    int posz;                           // +0x72
    char unknown_76[0x92 - 0x76];
    UnitType_0048a490* type;            // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short map;                 // +0xa6
    char unknown_a8[0xaa - 0xa8];
    unsigned short fix_lo;              // +0xaa
    char unknown_ac[0x110 - 0xac];
    unsigned int flags;                 // +0x110
};

struct Game_0048a490 {
    char unknown_0[0x14233];
    int gridW;                          // +0x14233
    int gridH;                          // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    unsigned char* hmaps;               // +0x14287
    char unknown_1428b[0x14377 - 0x1428b];
    MapInfo_0048a490** maps;            // +0x14377
    char unknown_1437b[0x38a47 - 0x1437b];
    int frame;                          // +0x38a47
};

#pragma pack(pop)

extern Game_0048a490* g_game;

unsigned int FUN_004b6340();
int __cdecl FUN_004b7123(int a, int b);
int __cdecl FUN_004b715a(int x, int y);
void __cdecl FUN_004b7173(unsigned short deg, Pos2_0048a490* p);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x48a490
void __stdcall FUN_0048a490(Unit_0048a490* u)
{
    MapRow_0048a490* row;
    MapInfo_0048a490* m = g_game->maps[u->map];
    row = m->rows + m->count;
    if (m->count < 0)
        return;
    {
        Pos2_0048a490 t;
        Pos2_0048a490 pts[4];
        Hs_0048a490 hs[4];
        int k;
        for (k = 0; k < 4; k++) {
            MapVertex_0048a490* v = m->verts + row->ids[k];
            int vx = v->x;
            t.x = vx;
            pts[k].x = vx;
            int vz = v->z;
            t.z = vz;
            pts[k].z = vz;
            FUN_004b7173(u->aim, &t);
            int hz = (short)((u->posz - t.z) >> 16);
            int wx = (short)((t.x + u->posx) >> 16);
            int fz = hz & 0xf;
            int fx = wx & 0xf;
            int gw = g_game->gridW;
            unsigned gx = (unsigned)wx >> 4;
            unsigned gz = (unsigned)hz >> 4;
            if (gx >= gw - 1)
                return;
            if (gz >= g_game->gridH - 1)
                return;
            unsigned char* tb = g_game->hmaps + (gz * gw + gx) * 13;
            int b0 = tb[4];
            unsigned char* tb1 = tb + g_game->gridW * 13;
            int b1 = tb1[4];
            int c0 = tb[0x11];
            int c1 = tb1[0x11];
            int H0 = b0 + ((c0 - b0) * fx) / 16;
            int H1 = b1 + ((c1 - b1) * fx) / 16;
            hs[k].wx = wx;
            if ((u->type->flags & 0x1000) && (u->flags & 0x10000000)
                && !(u->flags & 0x4000)) {
                int H = H0 + ((H1 - H0) * fz) / 16;
                hs[k].h = H;
                int sea = g_game->seaLevel;
                hs[k].h = max(H, sea);
                short p = (short)((((FUN_004b6340() & 0x1f) + k * 8) << 11) + u->fix_lo);
                int s = u->type->sight / 2;
                unsigned int n = g_game->frame - u->owner->age;
                int q = u->owner->sight;
                if (q >= s)
                    q = s;
                int w = (int)((((__int64)q << 16) / s));
                int mm = 2 - (int)((((__int64)w * 2) >> 16));
                if (n >= 60)
                    n = 60;
                mm -= (unsigned int)(mm * n) / 60;
                hs[k].h = FUN_004b7123(p, mm) + hs[k].h;
            } else {
                hs[k].h = H0 + ((H1 - H0) * fz) / 16;
            }
            hs[k].spare = hz;
        }
        int h0 = hs[0].h;
        int h1 = hs[1].h;
        int h2 = hs[2].h;
        int h3 = hs[3].h;
        int a = (h0 + h1) / 2;
        int b = (h2 + h3) / 2;
        u->roll = (a + b) / 2;
        u->pitch = FUN_004b715a(b - a, (short)(abs(pts[0].z - pts[3].z) >> 16));
        u->hdg = FUN_004b715a(h0 - h1, (short)(abs(pts[0].x - pts[1].x) >> 16));
    }
}
