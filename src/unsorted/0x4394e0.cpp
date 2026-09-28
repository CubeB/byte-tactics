// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// PARTIAL (37.1% best). What the function does: it snapshots the position the
// caller passed in `out`, calls FUN_00439740 (which updates `out` and returns
// the new position), and, when `flag` is set, walks the line from the snapshot
// to the new position in 0x300000 steps drawing the object's animation frame
// at each step, then leaves the snapshot in `out`.
//
// The overall shape matches (parameter load order, the FUN_00439740 call, the
// sqrt of the 16.16 delta length via _ftol, the frame-index division by the
// frame count at anim+0x2c and the tick division by the count at anim+0x0) but
// the register allocation still differs on nearly every instruction. The
// original keeps the snapshot in (edi, ebp, ebx), `out` in esi, and the view
// value in [esp+0x60]; ours keeps them in different slots. Small experiments
// (the __int64 sqrt helper, a Pos local copied into `out`, the +1>>1 form of
// the x/2 and (z-y/2)/2 expressions) did not close the gap.
//
// What still differs:
//   - the sqrt and its float loads (our reassociation is off),
//   - the age computation: original reads g_game->ticks first then
//     obj->field_46 and uses xor/cmp/sbb; ours reads obj first and uses the
//     setle/dec form,
//   - `count` clamping, which the original does through a reused stack slot,
//   - the whole 16.16 position interpolation loop body.
#include <windows.h>
#include <math.h>

struct Pos_4394e0 {
    int x;                               // +0x0 (16.16 fixed point)
    int y;                               // +0x4
    int z;                               // +0x8
};

struct View_4394e0 {
    char pad_0[0x2c];
    int cx;                              // +0x2c
    int cy;                              // +0x30
};

struct Obj_4394e0 {
    char pad_0[0x46];
    int field_46;                        // +0x46
};

struct Game_4394e0 {
    char pad_0[0x148d3];
    char* animTable;                     // +0x148d3
    char pad_148d7[0x38a47 - 0x148d7];
    unsigned int ticks;                  // +0x38a47
};

extern Game_4394e0* g_game;

void __stdcall FUN_00439740(void* surface, View_4394e0* view, Obj_4394e0* obj,
                            Pos_4394e0* out, int flag);
void __stdcall FUN_004b7f90(void* dst, void* bmp, int x, int y);

// FUNCTION: 0x4394e0
void __stdcall FUN_004394e0(void* surface, View_4394e0* view, Obj_4394e0* obj,
                            Pos_4394e0* out, int flag)
{
    int ox = out->x;
    int oy = out->y;
    int oz = out->z;
    FUN_00439740(surface, view, obj, out, flag);
    if (flag == 0)
        return;
    int dx = out->x - ox;
    int dy = out->y - oy;
    int dz = out->z - oz;
    int dist = (int)sqrt((double)dx * dx + (double)dy * dy + (double)dz * dz);
    if (dist < 0x10000)
        return;
    char* anim = g_game->animTable;
    unsigned int age = g_game->ticks - obj->field_46;
    int t = age > 0 ? age : 0;
    unsigned short count = *(unsigned short*)(anim + 0x2c);
    if (count < 1)
        count = 1;
    int idx = t / count % *(unsigned short*)anim;
    out->x = ox;
    out->y = oy;
    out->z = oz;
    for (int i = t % 0x1e * 0x300000 / 0x1e; i < dist; i += 0x300000) {
        int f = (int)(((__int64)i << 16) / dist);
        int x = view->cx - (int)(((__int64)f * dx) >> 16);
        int y = view->cy + ((int)(((__int64)f * dy) >> 16) >> 16 >> 1);
        int z = view->cy - (int)(((__int64)f * dz) >> 16);
        FUN_004b7f90(surface, *(void**)(anim + 0x28 + idx * 8),
                     0x80 - (short)(x >> 16),
                     0x20 - (short)(z >> 16) - (short)(y >> 16));
        idx = (idx + 1) % *(unsigned short*)anim;
    }
}
