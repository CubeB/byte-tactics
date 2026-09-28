// Decompiled by GPT-6-Luna, finished by Space Bunny Free. Names are provisional.
// PARTIAL, 41.9% (508 of 505 bytes; up from 30.4%). The worker that wrote this
// ran out of steps and produced no report, so the numbers below are measured
// from the scratch directory after the fact rather than from its account.
//
// The shape of the win: `flags` at +0x110 wants to be a *union* of a bitfield
// struct and an int, and the flag test wants to be written out twice,
//
//     if (obj->flags.bits.flag26 == 0 && obj->flags.all & 0x20000000)
//
// which is the "write it unidiomatically on purpose" family again, the same
// shape as 0x4da5b0's duplicated `if (p) *p = 0;` and 0x48a1e0's redundant
// guard through a second pointer. The bitfield declaration is what produces the
// original's `shr ecx, 0x1a` rather than a `test` against an immediate, and the
// redundant second test is what stops MSVC proving the pair equivalent. The loop
// also wanted `for (i = 0; i < size.x; i++, cell++, index++)` with both
// pointers advanced in the for-increment, rather than a countdown with
// post-increments in the body.
//
// Measured sweep, all with `check.py --sym` after `rm -rf build/obj`. The
// spread is narrow and the productive axis is the cell/index iteration, not the
// size arithmetic:
//   v3 (in the file)                                    41.9%
//   v2, v_base, v_cellsize, v_cell_size_i0, v_idx,     39.2%
//     v_idx2, v_noc, v_size_cell_i0, v_xy, v_xyc
//   v_size1, v_idx_size_cell_i0, v_xyidx               37.9%
//   v_inc_expr                                          37.0%
//   v_sep_pre                                           37.3%
//   v_sep_post, v_sep_post_u                            38.0%
//   v1                                                 30.0%
//   v_cell_size_i1, v_size_idx_cell_i1                  33.2%
//   v_cell_wsize_i0, v_wsize_cell_i0                   20.6%
//   v_cell_wsize_i1, v_wsize_cell_i1                   19.3%
// The two `wsize` families (20.6% and 19.3%) are the clear dead end: widening
// the width to a `short` costs about twenty points, so the width is a `short`
// loaded and sign-extended, not something computed at 32 bits. `v_ptr`,
// `v_ptr_noidx`, `v_wxh_*` and `v_wxh_size_*` produced no score at all and are
// not worth repeating.
//
#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Cell_0047db20 {
    short field_0;                      // +0x0, id of the unit owning the cell
    short field_2;
    char unknown_4[0xc - 0x4];
    unsigned char field_c;              // +0xc
    char unknown_d[0xd - 0xd];
};

struct Unit_0047db20 {
    char unknown_0[0x14e];
    unsigned char* mask;                // +0x14e, one byte per footprint cell
};

union Flags_0047db20 {
    struct {
        unsigned int unknown_0 : 26;
        unsigned int flag26 : 1;        // bit 26, tested with shr ecx, 0x1a
        unsigned int unknown_1 : 5;
    } bits;
    int all;
};

struct Obj_0047db20 {
    char unknown_0[0x76];
    Point pos;                          // +0x76
    char unknown_7a[4];
    Point size;                         // +0x7e
    int field_82;
    char unknown_86[0x92 - 0x86];
    Unit_0047db20* unit;                // +0x92
    char unknown_96[0xa8 - 0x96];
    short field_a8;                     // +0xa8, the owner's own id
    char unknown_aa[0x110 - 0xaa];
    Flags_0047db20 flags;               // +0x110
};

struct Game_0047db20 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    char unknown_14237[0x14287 - 0x14237];
    Cell_0047db20* cells;               // +0x14287
    char unknown_1428b[0x142b7 - 0x1428b];
    int field_142b7;
};

class Class_0047db20 {
public:
    virtual void FUN_0047ed30();
};
#pragma pack(pop)

extern Game_0047db20* g_game;
extern Class_0047db20 DAT_004fd660[];

void __stdcall FUN_00483210(Point pos, Point size);
void __stdcall FUN_0047e5c0(Point pos, Point size, Class_0047db20* visitor);
void __stdcall FUN_00440a70(Obj_0047db20* obj);

// FUNCTION: 0x47d0e0
void __stdcall FUN_0047d0e0(Obj_0047db20* obj)
{
    if (obj->field_82 != g_game->field_142b7) {
        Point size = obj->size;
        Cell_0047db20* cell = &g_game->cells[obj->pos.y * g_game->width + obj->pos.x];
        int index = 0;
        if (obj->flags.bits.flag26 == 0 && obj->flags.all & 0x20000000) {
            for (int j = size.y; j > 0; j--) {
                for (int i = 0; i < size.x; i++, cell++, index++) {
                    if (cell->field_0 == obj->field_a8) cell->field_0 = 0;
                    if (obj->unit->mask[index] & 1) cell->field_c &= 0xfd;
                }
                cell += g_game->width - size.x;
            }
            Point grown;
            grown.x = size.x + 2;
            grown.y = size.y + 2;
            Point pad;
            pad.x = obj->pos.x - 1;
            pad.y = obj->pos.y - 1;
            FUN_00483210(pad, grown);
        } else if ((obj->flags.all & 3) == 1) {
            for (int j = size.y; j > 0; j--) {
                for (int i = 0; i < size.x; i++, cell++) {
                    if (cell->field_0 == obj->field_a8) cell->field_0 = 0;
                }
                cell += g_game->width - size.x;
            }
        } else if ((obj->flags.all & 3) == 2) {
            for (int j = size.y; j > 0; j--) {
                for (int i = 0; i < size.x; i++, cell++) {
                    if (cell->field_2 == obj->field_a8) cell->field_2 = 0;
                }
                cell += g_game->width - size.x;
            }
        }
    }
    obj->flags.all &= ~0x08000000;
    if (obj->flags.bits.flag26) {
        Class_0047db20 visitor;
        obj->flags.all &= ~0x04000000;
        FUN_0047e5c0(obj->pos, obj->size, &visitor);
    }
    FUN_00440a70(obj);
}
