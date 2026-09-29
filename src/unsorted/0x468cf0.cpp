// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// STATUS: region-split skeleton, per docs/splitting-huge-functions.md. NOT a
// match. Frame exact (0x214), opening block transcribed, everything after
// 0x468e40 is a stub. See the region list and slot map at the bottom.
//
// 5904 bytes, 133 dwords of locals (`sub esp,0x214`), 2 stack args
// (ret 0x8 -> __stdcall free function), called from 3 places. All addressing
// is esp-relative: there is no `mov ebp,esp`; ebp is a saved data register
// (`lea ebp,[esi+0xdcb]` -> &g_game->colors, `mov ebp,[esi+0x4c]`).
//
// WHAT IT IS. The debug statistics overlay. It draws FRATE, MODE/INFO,
// "Game Time", the network/logic/render timing breakdown ("Network", "Units",
// "Logic", "Render Static", "Render Stuff", "Render Fog", "SFX", "Weapon",
// "Misc"), a crosshair when g_game->mode == 2, and several sub-panels via
// FUN_0046a400 / FUN_0046a430 / FUN_0046a530 / FUN_0046a610.
//
// TRICK THAT MADE THE FRAME EXACT: put every local in ONE pack(1) struct whose
// fields sit at the original's esp offsets, and instantiate it as the only
// true local. MSVC reserves the struct's full size even when most fields are
// dead, so `sub esp,0x214` comes out right. The pack(1) struct is also the
// only way the unaligned float slots (esp+0x79, +0x7d, ...) reproduce.
//
// The compiler then places the struct home at esp+8 (an 8-byte alignment gap
// at the bottom of the frame; it does this for both 0x214- and 0x20c-sized
// structs), so the offsets inside Locals are the absolute esp offsets minus 8.
// That is why `pad0` is 0xc, not 0x14, and why Locals carries a trailing
// `tail[8]`: frame = 8 (gap) + 0x20c (data) + 8 (tail) = 0x214, and
// `L.local_178` really does compile to `[esp+0xac]`. Verified with offsetof.
//
// Remaining prologue differences (still unfixed): the original saves
// ebx/ebp (it needs them for the colors pointer) and emits the surface copy as
// `rep movsd` (a 48-byte struct assignment); ours saves only esi/edi because
// the stubbed tail does not use ebx/ebp, and the copy is now a struct
// assignment so it should be `rep movsd` too. Register allocation across the
// whole function is what decides the prologue pushes, so the only real fix is
// writing the missing regions, not tweaking this block.
#pragma pack(push, 1)

struct Rect { int left, top, right, bottom; };

struct Surface48 { unsigned int words[12]; };   // the 48-byte gfx block at local_1f0

struct Player {
    char pad0[0x27];
    void* data;                         // +0x27
    char pad2b[0x146 - 0x2b];
    char field_1ca9;                    // +0x146, from g_game->1b63[i].0x146
    char pad147[0x14b - 0x147];
};

struct Game {
    char pad0[0xdcb];
    unsigned char colors[16];           // +0xdcb
    char pad_ddb[0x1b63 - 0xddb];
    Player players[10];                 // +0x1b63, stride 0x14b (ends 0x2851)
    char pad2851[0x2a43 - 0x2851];
    unsigned char playerIndex;          // +0x2a43
    unsigned char flags2a44;            // +0x2a44
    char pad2a45[0x2cac - 0x2a45];
    short field_2cac;                   // +0x2cac
    char pad2cae[2];
    short field_2cb0;                   // +0x2cb0
    char pad2cb2[2];
    short field_2cb4;                   // +0x2cb4
    char pad2cb6[0x14280 - 0x2cb6];
    unsigned char mode;                 // +0x14280
    char pad14281[0x1431f - 0x14281];
    int field_1431f;                    // +0x1431f
    int field_14323;                    // +0x14323
    char pad14327[0x1481b - 0x14327];
    int field_1481b;                    // +0x1481b
    char pad1481f[0x37e1b - 0x1481f];
    void* field_37e1b;                  // +0x37e1b (gfx surface handle)
    int field_37e1f;                    // +0x37e1f
    int field_37e23;                    // +0x37e23
    Rect rect_37e27;                    // +0x37e27
    char buf_37e3f[32];                 // +0x37e3f
    char pad37e5f[0x38a51 - 0x37e5f];
    unsigned char flags38a51;           // +0x38a51
    char pad38a52[0x38d85 - 0x38a52];
    int tick_38d85;                     // +0x38d85
    int field_38dd1;                    // +0x38dd1 (+0x4c from tick)
    char pad38dd5[0x391c3 - 0x38dd5];
    int field_391c3;                    // +0x391c3
    int field_391f9;                    // +0x391f9
    char pad391fd[0x3923b - 0x391fd];
    unsigned char flags3923b;           // +0x3923b
};

// The original's whole frame as one struct. Field order is ascending offset;
// the pads are the slots Ghidra left unnamed. The comments give the ABSOLUTE
// esp offset; the compiler's home for this struct starts at esp+8, which is
// why pad0 is 0xc (0xc + 8 = 0x14) and why the trailing tail[8] exists. The
// struct plus the 8-byte home plus the 8-byte tail is the 0x214 frame. Do not
// resize a field without re-deriving the slot map and re-verifying offsetof.
struct Locals {
    char pad0[0xc];
    int local_210;                      // +0x14
    int local_20c;                      // +0x18
    int local_208;                      // +0x1c
    int local_204;                      // +0x20
    int local_200;                      // +0x24
    int local_1fc;                      // +0x28
    int local_1f8;                      // +0x2c
    int local_1f4;                      // +0x30
    unsigned int local_1f0[12];         // +0x34..0x63  (gfx surface block)
    int local_1c0;                      // +0x64
    int local_1bc;                      // +0x68
    int local_1b8;                      // +0x6c
    int local_1b4;                      // +0x70 (byte + 3 pad in the original)
    int local_1b0;                      // +0x74
    char local_1ac;                     // +0x78
    float local_1ab;                    // +0x79 (unaligned)
    float local_1a7;                    // +0x7d
    float local_1a3;                    // +0x81
    float local_19f;                    // +0x85
    float local_19b;                    // +0x89
    float local_197;                    // +0x8d
    float local_193;                    // +0x91
    float local_18f;                    // +0x95
    char pad99[0x9c - 0x99];
    int local_188;                      // +0x9c
    int local_184;                      // +0xa0
    int local_180;                      // +0xa4
    int local_17c;                      // +0xa8
    int local_178;                      // +0xac
    int local_174;                      // +0xb0
    char local_170[32];                 // +0xb4..0xd3
    char local_150[80];                 // +0xd4..0x123 (text line scratch)
    unsigned char local_100[240];       // +0x124..0x213 (big format buffer)
    char tail[8];                       // frame padding (base at esp+0)
};

#pragma pack(pop)

extern Game* g_game;                    // 0x511de8

struct Class_004c6b10 {
    void FUN_004c6b10(Rect r);          // __thiscall, 4 dwords by value
};

void __stdcall FUN_004c69a0(void* surface);
void __stdcall FUN_004c69c0(void* surface);
void __stdcall FUN_004c2470();
unsigned long __stdcall FUN_004b6560();
void __stdcall FUN_00483fa0(void* surface);
void __stdcall FUN_00418310(void* surface);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, unsigned int color);

// FUNCTION: 0x468cf0
void __stdcall FUN_00468cf0(int param_1, int param_2)
{
    Locals L;

    // ---- REGION r0 begin: 0x468cf0-0x468e35 ----
    // Prologue, surface setup, mode==2 crosshair.
    L.local_178 = (g_game->field_37e1f + 0x80) / 2;
    L.local_174 = g_game->field_37e23 / 2;
    FUN_004c69a0(g_game->field_37e1b);

    L.local_1b0 = (int)&g_game->colors[0];
    *(Surface48*)L.local_1f0 = *(Surface48*)g_game->field_37e1b;
    FUN_004c2470();

    ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(g_game->rect_37e27);

    {
        unsigned long now = FUN_004b6560();
        g_game->field_38dd1 += (int)(now - (unsigned long)g_game->tick_38d85);
        g_game->tick_38d85 = (int)now;
    }

    FUN_00483fa0(L.local_1f0);
    FUN_00418310(L.local_1f0);

    {
        int x = (int)g_game->field_2cac - g_game->field_1431f + 0x80;
        int y = ((int)g_game->field_2cb4 - ((int)g_game->field_2cb0 >> 1))
              - g_game->field_14323 + 0x20;
        if (g_game->mode == 2) {
            FUN_004be950(L.local_1f0, x - 2, y, x + 2, y,
                         (unsigned int)g_game->colors[15]);
            FUN_004be950(L.local_1f0, x, y - 2, x, y + 2,
                         (unsigned int)g_game->colors[15]);
        }
    }
    FUN_004c69c0(L.local_1f0);
    // ---- REGION r0 end ----

    // REGION r1..r9 not written yet. See the region list at the top of the
    // file and the call-sequence notes below.

    // Keep every declared slot live so MSVC reserves the full 0x214 frame.
    L.local_210 = param_1;
    L.local_20c = param_2;
    L.local_208 = 0;
    L.local_204 = 0;
    L.local_200 = 0;
    L.local_1fc = 0;
    L.local_1f8 = 0;
    L.local_1f4 = 0;
    L.local_1c0 = 0;
    L.local_1bc = 0;
    L.local_1b8 = 0;
    L.local_1b4 = 0;
    L.local_1ac = 0;
    L.local_1ab = 0;
    L.local_1a7 = 0;
    L.local_1a3 = 0;
    L.local_19f = 0;
    L.local_19b = 0;
    L.local_197 = 0;
    L.local_193 = 0;
    L.local_18f = 0;
    L.local_188 = 0;
    L.local_184 = 0;
    L.local_180 = 0;
    L.local_17c = 0;
    L.local_170[0] = 0;
    L.local_150[0] = 0;
    L.local_100[0] = 0;
    (void)param_1;
    (void)param_2;
}

// ---------------------------------------------------------------------------
// Notes for the next worker.
//
// CALL SEQUENCE in the original (address -> callee), for laying out regions:
//   0x468d30 FUN_004c69a0(g_game->37e1b)
//   0x468d55 FUN_004c2470()
//   0x468d85 Class_004c6b10::FUN_004c6b10  (this = local_1f0, Rect by value)
//   0x468d96 FUN_004b6560() -> GetTickCount, folded into g_game->38d85/38dd1
//   0x468db0 FUN_00483fa0(local_1f0)
//   0x468dba FUN_00418310(local_1f0)
//   0x468e16/0x468e30 FUN_004be950 x2  (crosshair, only when mode == 2)
//   0x468e3a FUN_004c69c0(local_1f0)
//   0x468e8x,0x468edx,0x46910e,0x46918e,0x4691c1,0x46921d,0x469285,
//   0x4692ad,0x469316,0x46933e,0x4693f2,0x469472,0x4694af,0x469511,
//   0x46957e,0x4695d5 _ftol  (float->int, all part of x/8 smoothing + readouts)
//   0x4691d4..0x4695d5 sprintf (8 sites) with "%d", "%dK", "%.1f"
//   0x4691f3..0x46960b FUN_004c14f0 (draw text) many times
//   0x469247,0x46953b FUN_004c1480
//   0x469020 FUN_004c1420, 0x469025/0x4696d2/0x46a0c6/0x46a278 FUN_004c1450
//             (line height), 0x469039/0x4692c8/0x46935e/0x469497/0x469586/
//             0x4695dd/0x4696d2/0x46a261 FUN_004c13f0,
//             0x469045/0x4692d4/0x4694a3/0x469592/0x4695e9/0x46a271 FUN_004c13a0
//   0x469073/0x46a129/0x46a18d/0x46a1bb/0x46a2d2 FUN_004b7f30
//   0x469083 FUN_00467a20, 0x4690ab FUN_00467c00
//   0x46912e/0x469412/0x469492 FUN_004bf6f0
//   0x469615 FUN_0046a860, 0x46961f FUN_00466b00
//   0x469849 FUN_00471f90
//   0x469fea FUN_004c13f0, 0x46a3c2 FUN_0045ffb0
//   0x469f9f FUN_004689c0, 0x469fb8 FUN_00468380, 0x469fcb FUN_00464060
//   0x468f8d-0x468fb4 FUN_00464ab0/FUN_00464ac0/FUN_00464af0/FUN_00464b00
//   0x46a303 FUN_004ab170
//   0x46a330..0x46a3b8 FUN_0046b900 x9 with the labels "Network", "Units",
//             "Logic", "Render Static", "Render Stuff", "Render Fog", "SFX",
//             "Weapon", "Misc"
//   0x46a3c7 FUN_004c2870, 0x46a3db FUN_004c63a0, 0x46a3ee Class_0046a400::FUN_0046a400
//
// Remaining callees not yet placed: FUN_0045ac20, FUN_004658e0, FUN_0046a430,
// FUN_0046a530, FUN_0046a610, FUN_004848e0, FUN_0048c190, FUN_0048cc30,
// FUN_004948e0, FUN_0049be60, FUN_004b6720, FUN_004b7f90, FUN_004bf8c0,
// FUN_004c1480, FUN_004c1b80, FUN_004c5740, FUN_004c63a0, FUN_00417f30,
// FUN_00420b00, Class_00435100::FUN_00435100, FUN_0044c190. Find each call
// site with `grep <addr>` against build/scratch/ctx-468cf0.txt.
//
// KNOWN UNKNOWNS / TRAPS:
//  * local_1b0 is set from `lea ebp,[eax+0xdcb]` (address of colors), but the
//    crosshair color pushed is `mov dl,[ebx+0xf]` = colors[15]. Do not pass the
//    pointer where the byte is wanted.
//  * The x/8 smoothing blocks (r1) use the full MSVC signed-division rounding
//    sequence: `(b - a) / 8` with a sign fixup and a "if quotient is 0 use
//    +/-1" clamp. Get the signedness right or the sar/shr and the fixup differ.
//  * offsets 0x0..0x13 and 0x99..0x9b of the frame are referenced by the
//    original (see the full `[esp+0xNN]` set in the ctx dump) but Ghidra left
//    them unnamed; they are pad in Locals. If a region needs them, add a named
//    field and re-check the total is still 0x214.
//  * The sprintf buffers are local_150+4 (esp+0xd8) and local_100 (esp+0x124).
//    Both are reached as `lea reg,[esp+0xd8]` / `[esp+0x128+]`.
// ---------------------------------------------------------------------------
