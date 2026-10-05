// Decompiled by Space Bunny Free, finished by space-bunny-free. Names are provisional.
// Claude Sonnet 5.5 pass (#554): this one depends on compiler state. With N unused
// `extern int dummyK;` declarations after the header comment the score is 87.3 percent
// and 231 bytes for N = 0 to 40 and again for N = 304 to 400, but 221 bytes (the
// original size) and the division order of the original (pos.x, then pos.z, then
// the y_hi term, with `sar ebp, 0x15` deferred to just before the call) for every N
// from 48 to 296. In that window the remaining differences are only the placement of
// the `g_game->field_1485b` load (original: right before its push, after both
// divisions; ours: hoisted above the `push ebx`) and the scheduling of the two
// `movsx` loads of the entry fields. The best of eight statement orders there (x and
// y declaration order, subtraction order, store order) is 85.2 percent (y computed
// before x with the subtractions in x, y order; or y, x throughout), and writing
// `y -= entry->field_6` before `x -= entry->field_4` gives 85.2 as well, so none of
// it reaches MATCH. headers.py finds no header set that reproduces the window
// (best 87.3 percent for every set), so the file keeps the 231 byte form, which
// scores higher than the window.
//
// Space Bunny Free pass (#1146): re-checked the same expression against a batch
// of free scratch scores (build/scratch/0x482830/batch*.py, 13 variants) and the
// scheduler tie is not reachable from the source shape of the y term. Writing
// the y difference as two statements (`int y = pos.z / 0x200000; y -= pos.y_hi
// / 64;`), as two named term locals (`int yz = ...; int yh = ...; int y = yz
// - yh;`), as `-(pos.y_hi / 64) + pos.z / 0x200000`, with an explicit (int) cast
// on either side, with the high half read as `((short*)&pos.y)[1]`, with yz read
// before x, or with the GetGafFrame call written before or between the two
// divisions all give the same 231 byte 87.3 percent build, so MSVC 5
// canonicalises them to one tree and the order is picked after that. What does
// move the number is the ORDER of the two x and y statements, and the three
// things around them: declaring y before x drops it to 223 bytes and 74.8
// percent, taking the `y -= entry->field_6; x -= entry->field_4;` pair in the
// other order 238 bytes and 74.3 percent, storing field_4->y before field_4->x
// 82.4 percent, and dropping the local table pointer from the clamp 223 bytes
// and 81.0 percent. So the 231 byte form below is the best known, and the
// remaining ten bytes are one scheduler tie. Also tried, all still 231 bytes and
// 87.3 percent, so the tie is not a type or expression-tree question either:
// `long` for x and y, an `L` suffix on 0x200000, 0x40 instead of 64 for the
// high half, the two entry offsets read into named `short` locals first, a
// throwaway live local after the call, and `int yz = ...; int y = yz; y -= ...`.
//
// Not a match yet: 87.3 percent, ours is 10 bytes longer (231 against 221).
// Everything matches except the emission order of the two divisions that make
// the y coordinate. The original emits pos.z / 0x200000 first and the high
// half of pos.y divided by 64 second (0x4828ad and 0x4828bb), so the value of
// pos.z / 0x200000 lands in edi and the second term stays in eax for
// "sub edi, eax". Our build emits the 16 bit division first, which takes edi,
// then has to spill the value to [esp+0x18] and reload it into edx for
// "sub edi, edx" (three extra instructions, 10 bytes).
//
// The expression itself is right: the near identical function 0x482ac0 (a
// MATCH, same body inlined) writes the same thing as
//   cell_y = p.pos.z / 0x200000 - ((short*)&p.pos.y)[1] / 64;
// and that build does emit pos.z first. So this is the scheduler's choice in
// this one function, not a wrong expression. None of these source variations
// changed the order: statement order of the x, z and y_hi divisions, temporaries
// versus one expression, the high half read as a short field or as
// ((short*)&pos.y)[1] or ((short*)&pos)[3], a local copy of g_game or of
// params, short* versus struct out pointer, signed char versus unsigned char
// cell_id, the clamp with and without a local table pointer, the nested if
// against the early return form, int versus long, and removing the two
// trailing calls. Removing the GetGafFrame call, or the x division, makes the
// order come out as in the original, so the pressure across that call is what
// picks the wrong order.
//
// space-bunny-free pass: MATCH. Two changes, both above.
// (1) `#include <string.h>` at the top of the file. That is the whole "compiler
// state" story: it moves MSVC 5's commutative-operand choice in the y
// subtraction, so pos.z / 0x200000 is emitted before the y_hi term and the
// value lands in edi, exactly as at 0x4828ad. It is not the dummy-declaration
// padding: the file as it stood, with no dummies at all, was 87.3 percent and
// 231 bytes, and with the include and no dummies it is 221 bytes. windows.h,
// stdio.h and math.h all give the same build as string.h here, so the smallest
// one that works is the one kept (docs/agent-guide.md, "Operand order that no
// rewrite changes can depend on the headers").
// (2) The clamp reads g_game->field_1485b->count twice instead of through the
// local `table` pointer. With the include, the local makes MSVC 5 keep one
// load of g_game->field_1485b alive across both divisions, so the second
// argument is pushed before the sub and the movsx pair after the call is
// rescheduled; without the local the two loads stay separate and every
// instruction lands where the original has it.
//
// What the earlier passes got wrong, re-derived from the operand bytes:
// "Removing the x division makes the order come out as in the original" is not
// true. build/scratch/0x482830/d_nox2.cpp deletes the x division (and its
// store) and is still y_hi-first with a spill. The order does not depend on
// the source order of the two terms either (d_swap_wrong.cpp writes
// `y_hi / 64 - z / 0x200000`, which is 221 bytes but still y_hi-first), nor on
// the divisor of either term (t_swapdiv, t_both64, t_bothbig), nor on
// temporaries, explicit casts, ((short*)&pos.y)[1], inline helpers, a local
// pointer to pos, or a local copy of pos. What does move it is the type of the
// second term, which is why the two higher scoring shapes below are wrong:
// declaring field_6's partner y_hi as `unsigned short` gives 217 bytes and 91.2
// percent (MSVC 5 turns /64 into a plain shr, so negative y_hi is wrong), and a
// `(short)` cast on the y expression, or a `short y` local, gives 224 to 228
// bytes and 74 to 75 percent (the truncation becomes visible). Neither is the
// original: `movsx eax, word ptr [esi + 0x16]` at 0x4828bb is a signed load, and
// the original keeps x and y as 32 bit values across the call (`sub ebp, ecx`
// at 0x4828e7).

#include <string.h>

struct Out_482830 {
    short x;
    short y;
};

struct Entry_482830 {
    char unknown_0[4];
    short field_4;
    short field_6;
};

struct Table_482830 {
    unsigned short count;         // +0
    char unknown_2[0x28 - 0x2];
    Entry_482830* entries;        // +0x28
};

#pragma pack(push, 1)
struct Pos_482830 {
    int x;                        // +0x10
    short y_lo;                   // +0x14
    short y_hi;                   // +0x16, that is y / 0x10000
    int z;                        // +0x18
};

struct Params_482830 {
    void* field_0;                // +0
    Out_482830* field_4;          // +4
    short field_8;                // +8
    unsigned char field_a;        // +0xa
    char unknown_b;               // +0xb
    char* field_c;                // +0xc
    Pos_482830 pos;               // +0x10
};

struct Game {
    char unknown_0[0x14281];
    unsigned char flags;          // +0x14281
    char unknown_14282[0x1485b - 0x14282];
    Table_482830* field_1485b;    // +0x1485b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall UpdateLineOfSight(Params_482830* params);
void __stdcall AddLineOfSight(Params_482830* params);
void __stdcall FUN_00481930(Params_482830* params);
Entry_482830* __stdcall GetGafFrame(Table_482830* table, int index);

// FUNCTION: 0x482830
void __stdcall FUN_00482830(Params_482830* params)
{
    if ((g_game->flags & 2) != 2) {
        return;
    }
    *params->field_c = 0;
    if ((g_game->flags & 4) == 4) {
        UpdateLineOfSight(params);
        return;
    }
    int lod = params->field_8 / 32 - 5;
    if (lod < 0) {
        lod = 0;
    } else {
        if (lod >= g_game->field_1485b->count) {
            lod = g_game->field_1485b->count - 1;
        }
    }
    int x = params->pos.x / 0x200000;
    int y = params->pos.z / 0x200000 - params->pos.y_hi / 64;
    Entry_482830* entry = GetGafFrame(g_game->field_1485b, lod);
    x -= entry->field_4;
    y -= entry->field_6;
    params->field_4->x = (short)x;
    params->field_4->y = (short)y;
    *params->field_c = (char)lod;
    AddLineOfSight(params);
    FUN_00481930(params);
}
