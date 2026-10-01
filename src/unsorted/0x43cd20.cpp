// Decompiled by Space Bunny Free, finished by space-bunny-free, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro.
// Names are provisional.
//
// mimo-v2.6-pro, 2026-10-01 second retry: 94.8% (original 943 bytes, ours
// 964). Two spellings lifted the 81.7% base (the old negative measurements
// below were all made on the 74.8% base and no longer hold):
//  1) the tail is TWO call statements, one in each arm of
//     `if (d1 > lim && d2 > r) call(unit, unit->type->field_19e); else
//     call(unit, -rate);`. Every select spelling (if/else amount plus one
//     call, ternary as the argument or assigned, braced or not) makes MSVC
//     sink the call into both arms with two epilogues (988 bytes, 83.5);
//     with the call written in each arm the whole register allocation falls
//     into place at once: the ebx<->ebp swap, the prologue register saves
//     and the v3 call setup all match the original (81.7 -> 92.9).
//  2) the hasPath arm is `turn = hasPath; int r2 = unit->type->field_19a;
//     call(unit, -r2);` (store first, rate named after it): that stores ax
//     as in the original instead of an immediate 0 (92.9 -> 94.8).
// Still differs (three things, all small):
//  - the arm's load order: the original loads unit into ecx BEFORE the turn
//    store (eax is still busy with the v5 result, so the scratch rotation
//    runs unit=ecx, type=edx, rate=eax with `neg eax; push eax; push ecx;
//    mov ecx,esi`); ours stores first and the rotation is one step off
//    (unit=eax, type=ecx, rate=edx). Forcing the unit load above the store
//    failed: `Unit* u = unit;` is scalarised and its load sinks past the
//    store (94.8 same), a type-pointer copy before the store (87.1), the
//    comma forms `(turn = hasPath, unit)` and `(turn = hasPath,
//    -unit->type->field_19a)` (identical to store-first), an inline rate
//    chain (identical), `turn = 0` with the literal-reuse trick (94.8
//    same), a doubled `turn = hasPath; turn = hasPath;` pair (dead-store
//    folded, identical) and the fold-away `u += 1; u -= 1` pointer pair
//    (91.1; it does not fold and keeps two adds).
//  - `imul ecx` (one-operand 64-bit multiply) against our `imul eax,ecx;
//    cdq`: the `(__int64)(unsigned short)adiff * field_20` spelling matches
//    those two bytes on its own (81.7 -> 84.2) but breaks the tail's
//    register allocation in every combination with the new arm and tail
//    (92.9 -> 85.8, 94.8 -> 87.7), so the int-product spelling stays.
//  - the tail arm layout: the original merges the two arms before ONE call
//    (`mov eax,[ebx+0x19e]; jmp join; mov eax,[esp+0x14]; neg eax; join:
//    mov ecx,[esp+0x10]; push eax; push edi; call`); the two-call spelling
//    leaves the else arm's copy of the whole call tail out of line after
//    `ret 4` (about 27 diff lines, most of the remaining gap). Also tried:
//    switch on the condition (85.8), a goto label before one shared call
//    (82.6), explicit returns in both arms and else-first (both 94.8
//    same), reverse default-first (77.7) and two `if` assignments of
//    -rate (77.9).
// Earlier attempts (deepseek-v4.1-flash et al, 74.8% base) are kept below for
// the history; their measured negatives still hold where re-measured (the
// 64-bit `(__int64)(unsigned short)adiff * field_20` imul spelling: 71.5
// alone, 67.2 with the merged tail; if/else and ternary selects: 62-66; t1
// with the neg before the tests: 73.4; recompute through ppos: 46.5; ppos
// before the v3 call: 69.6; reversed operator- operands: 71.6).
//
// ---------------------------------------------------------------------------
// Earlier notes (deepseek-v4.1-flash et al), kept for the history:
//
// Decompiled by Space Bunny Free, finished by space-bunny-free, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
//
// deepseek-v4.1-flash, 2026-10-01, second probe: `Vec3* const ppos` is
// byte-identical (74.8%, 964 bytes); moving the hasPath==0 FUN_0043cc20 call
// before `turn = hasPath` regresses to 71.2 (966 bytes); hoisting `int rate`
// above the diff block regresses to 71.8 (978 bytes). Best stays the version
// below.
//

// deepseek-v4.1-flash retry, 2026-10-01. State: 74.8% (original 943 bytes,
// ours 964); kept, no improvement. Also tried hoisting `UnitType* type =
// unit->type;` and using it in the hasPath==0 arm: 70.4% / 959 bytes, so the
// extra dword of frame is not a cached type pointer. The original tail really
// is ONE
// FUN_0043cc20 call shared by both arms (jmp 0x43d0ba with eax preloaded and
// `mov eax,[esp+0x14]; neg eax` as the else arm), but spelling it as an
// if/else amount plus a single call scores 62.4 (986 bytes), so the duplicated
// call below stays. Same for the ternary spelling. Everything else is the
// known unit-in-eax-vs-edi allocator gap.
//
// select tail (`int amount = (d1 > lim && d2 > r) ? unit->type->field_19e :
// -rate;` plus one call) scores 62.4 (986 bytes) and rotates unit from ebx to
// ebp; the duplicated cold epilogue after ret 4 persists even with a single
// call statement, so the tail split comes from the conditional expansion, not
// from the two call statements. Best remains the 74.8 version below.
// Eighth pass (deepseek-v4.1-flash, retry #2896, 74.8%, 0 counting runs, all
// scored with check.py --sym on scratch copies). Still the same two open
// items: frame 0x40 against 0x44 and the ebx<->edi rename (ours unit=ebx /
// ppos=edi, original unit=edi / ppos=ebx). Tried and rejected this pass, all
// <= 74.8:
//   - ppos declared before the v5 call (71.0), before the v3 call (70.9),
//     between the ax and az lines (74.8), forward-declared at the top with
//     the assignment left in place (74.8), declared with ax/az/gap1 up front
//     (74.8): the declaration point does not move the allocator, which
//     follows first definition in the flow graph.
//   - `Vec3& ppos = unit->pos;` with `.` access: compile error (the call
//     site needs a `Vec3*`).
//   - `static inline Vec3* PosOf(Unit*)` and `(Vec3*)((char*)unit + 0x6a)`:
//     byte-identical to the file (74.8).
//   - first delta through ppos for x and z (62.0) or only z (62.0): unit
//     leaves the callee-saved set.
//   - recompute deltas through ppos (46.2), ppos removed entirely with every
//     access spelled unit->pos (63.5).
//   - single select + one tail call, `int amount; if..else`: 62.4, and the
//     disassembly shows unit in ebp (lea edi,[ebp+0x6a]), confirming the
//     earlier note.
//   - `(int)(((__int64)(unsigned short)adiff * field_20) / max_turn)` (the
//     one-operand `imul ecx` the original has): 70.3, so the byte-count
//     optimum still prefers the int product spelled here even though the
//     original multiplies in 64 bits.
//   - naming the hasPath==0 amount in a local before the turn store: 72.7.
// Conclusion unchanged: with this source shape the ebx<->edi swap is an
// allocator tie-break no source rewrite has moved, and every rewrite that
// moves it loses the frame or the ppos=ebx assignment.
//
// Seventh pass (deepseek-v4.1-flash, retry, ~15 scratch/check runs, 74.8%,
// 964 bytes). Two changes raised the score:
//  1) computing the recompute delta as az first then ax
//     (`az = p[1].z - unit->pos.z; ax = p[1].x - unit->pos.x;`)
//     and swapping the order of the bx/bz statements: 72.0 to 74.8.
//  2) with (1) in place the whole main path is now a pure ebx<->edi rename of
//     the original (ours unit=ebx/ppos=edi, original unit=edi/ppos=ebx).
// Measured and rejected:
//   - the tail really is ONE call in the original (0x43d0c0, amount selected
//     into eax by the two `jle` arms). Both `int amount; if..else..` and the
//     same select as a ternary argument give 984-986 bytes, 61.4-62.4%, and
//     move unit to ebp.
//   - ppos introduced before the first delta (first reference to ppos earlier
//     than to unit) gives 62.0-62.4%, so the allocator's choice follows
//     reference order, not a source declaration order we can flip.
//   - a local `Unit_0043cc20* u = unit;` copy is scalarised (byte-identical).
//   - a Vec3 first-delta temp (with dead y) and int[3] with dead index 1 are
//     scalarised too: same 964 bytes.
//   - tools/headers.py: 128 sets, best 74.8% (<math.h> etc), and a sweep of
//     N unused extern declarations from 0 to 200 in steps of 8 is flat at
//     74.8%, so this is not compiler state.
// Still differs: frame 0x40 against 0x44 (the array sits 4 bytes lower, and
// every stack slot is off by 4), and the ebx/edi rename. The hasPath==0 arm
// loads unit after the turn store here, before it in the original. First
// delta args use one slot pair in ours, 8-bytes-apart in the original.
//
// Sixth pass (deepseek-v4.1, 4 more runs, 72.0%, 962 bytes): naming the
// FUN_0048a980 result first is the big lever,
//   short ang = (short)FUN_0048a980(ppos, &p[1]);
//   short diff = ang - unit->heading;
// lifts 61.1 to 72.0 and rotates the callee-saved assignment from
// unit=ebp/ppos=ebx to unit=EBX/ppos=EDI (original unit=edi/ppos=ebx), so the
// remaining register diff is a straight ebx<->edi swap. Still open: the frame
// is 0x40 against 0x44, and the tail else branch is laid out after the
// epilogue (two epilogues, +19 bytes) instead of inline before the shared
// call. `(__int64)(unsigned short)adiff * field_20` (64-bit imul before the
// divide, instead of the int product cast up) scores 67.5, so the int-product
// spelling stays. All other variants measured this pass were byte-identical.
//
// Fifth pass (deepseek-v4.1, 8 further check runs, 61.1%): the q numerator
// must be spelled with a named int temp and a 64-bit shift, not a multiply:
//   int t = (int)(((__int64)field_20 * field_20) >> 16);
//   int q = (int)(((__int64)t << 16) / (2 * rate));
// `* 0x10000` makes VC5 emit _allmul with a constant (939 bytes, 60.9) where
// the original does cdq + _allshl 16 after the _allshr; the temp form lifts
// this to 61.1 and 936 bytes. Register allocation is unchanged (unit ebp,
// original edi; frame 0x40 against 0x44):
//   - A first-delta Vec3 temp (v6/v9: `Vec3 d; d.x=..; d.z=..;` and
//     `int d[3]` with members 0 and 2) compiles byte-identical to the plain
//     int ax/az form: VC5 coalesces both temps into one slot and the frame
//     stays 0x40, so the missing 4 bytes are NOT reclaimable this way.
//   - A local copy `Unit_0043cc20* u = unit;` for the whole main path, a
//     `Vec3& ppos` reference and moving ppos between ax and az all compile
//     byte-identical to this file (60.9 at the time).
//   - Replacing the two tail calls with a common `amount` select (if/else or
//     ternary) collapses the duplicated epilogue but drops to 52.2 (915
//     bytes): the original really lays out the two calls separately.
//
// Fourth pass notes (deepseek-v4.1, 14 further check runs). Still 60.9%; the
// frame stays 0x40 against the original 0x44 and `unit` stays in ebp against
// the original edi. New measurements:
//   - The original's 17-dword frame has a HOLE at [esp+0x28]: the first
//     _hypot's two int args live at [esp+0x24] and [esp+0x2c], 8 bytes apart
//     with the middle dword never written anywhere in the function, i.e. the
//     shape of a 12-byte Vec3 temp whose y member is dead (a Vec3 delta).
//     Ours coalesces both args into the parameter slot (free once `unit` is
//     enregistered), which is exactly the missing 4 bytes.
//   - The original's d1 deltas are plain scalars (z into the reused parameter
//     slot, x in a register), so the Vec3 temp is only the first _hypot's
//     argument pair, and the pull-back recompute is genuinely separate.
//   - Tried and measured (all <= 60.9, most byte-identical to the file):
//     a local `Vec3* ppos` declared before the ax/az pair and used for it
//     (unit moves to eax, 52.6); `Vec3 d = p[1] - *ppos;` with x,y,z and
//     x,0,z operator- bodies and with `p[1] - unit->pos` (unit eax, 52.6);
//     removing ppos entirely (unit lands in EDI, frame 0x3c, 50.7, so the
//     allocator can choose edi but never with ppos also live); `<< 16` for
//     the `* 0x10000` in q (emits _allshl like the original but shrinks the
//     frame to 0x3c, 57.4); `rate + rate` for `2 * rate` (identical);
//     unsigned short temp for the turned product (identical); ax/az declared
//     before the v3 call, swap of the ax/az statements, ppos declared and
//     assigned on separate lines, `if (obj->v5() == 0)` with `turn = 0`,
//     ppos->x used for different operands (all byte-identical, 60.9).
//   - Conclusion: at this source shape the ebp/edi swap is an allocator
//     tie-break (same pattern as 0x4a6ae0 / 0x4866d0 on the shared board);
//     every rewrite that changes it also loses the ppos=ebx assignment or the
//     frame. What is still needed is a source shape that keeps unit in edi
//     AND ppos in ebx AND the 0x44 frame at the same time.
//
// NOT MATCHED (53.5%, 940 bytes against 943). Semantically correct. The frame
// is 0x3c here, the original's is 0x44 (the earlier 45.2% note's 0x38 is stale).
//
// Third pass notes (space-bunny-free, 1 check run). Frame slot map, measured
// from the original, everything relative to the parameter slot (which is where
// the original keeps several dead locals):
//   -0x04 .. -0x08  the three dwords above the array: the array ends 4 bytes
//                   below the parameter, ours ends 8 bytes below it
//   -0x28           &p[0]  (ours: -0x2c)
//   -0x2c, -0x30    the two temps the first _hypot's int args are spilled to
//                   before the two filds (ours reuses ONE slot, -0x34)
//   -0x34           len (gap1 sits at -0x3c)
//   -0x38           a hole, which is where a nested block would end
//   -0x3c           gap1
//   -0x44           dz, then ndz
//   -0x48           the spill of `this` (the top-of-function
//                   `mov [esp+0x10], esi`); ours spills it at -0x44
//   the parameter slot itself (0x00) holds, in turn, dx, ndx, p[1].z - pos.z
//   and the FUN_0048a980 result
// So the original needs 8 more bytes than ours, and it gets them from two more
// live slots below the array plus one dword more above it.
//
// The root cause of nearly every difference is that ours keeps re-reading
// `unit` from its parameter slot (see _unit$[esp+72] three times in the tail)
// while the original parks it in edi with &unit->pos in ebx from just after the
// v3 call to the end. Because the parameter stays live, the compiler never
// frees its slot, so the original's `dx` in the parameter slot (0x43cdd1) has
// nowhere to go in ours. Getting unit into a callee-saved register is the
// thing to chase; rewriting the expressions will not move it.
// The `hasPath == 0` arm wants the parameter load first
// (`mov ecx, [esp+0x58]` then `mov word [esi+0x24], ax`); ours stores the
// turn first whatever the spelling of the two statements was.
// Also tried: naming both _hypot arguments as locals and swapping their order
// (byte-identical output to the inline form, still one temp slot, still 0x3c).
//
// What this function does: the path object (vtable 0x4fd458, see
// 0x44f010.cpp; slot 3 is 0x44f150, slot 5 is 0x44f290) hands over the next
// three waypoints. Waypoint 1 is pulled back along the first segment when the
// unit is farther than 0x500000 from it, the turn toward it is applied and
// clamped to the type's max_turn, and then the distance this frame may travel
// is either the type's +0x19e or the negated rate at +0x19a.
//
// The slow-path amount is -type->field_19a, not -dz. At 0x43d03a
// `mov [esp+0x24],esi` runs after four pushes, so it writes [esp+0x14],
// overwriting the dz slot, and 0x43d0b4 `mov eax,[esp+0x14]; neg eax` reads
// that value back; dz is dead after 0x43ce8c. q's numerator is
// ((field_20*field_20) >> 16) << 16, not >> 32 << 16. lim is
// (turned*turned >> 32) * 4 where turned is the earlier 64-bit quotient
// (stored at 0x43d027, reloaded at 0x43d087); r is q*q >> 32.
//
// What helped: a local `Vec3* ppos = &unit->pos` used for every pos access
// (lever 6) reproduced the original's ebx = &unit->pos and, with the
// operator-/Square member functions added to Vec3 (the idiom the matched
// siblings 0x404730/0x414a80 use), the frame grew from 0x3c to the original
// 0x44 and the score rose from 50.7 to 58.6 (that variant reused the
// pre-branch ax/az for d1, which is wrong because the pull-back modifies
// p[1]; recomputing them is correct but scores 53.5).
//
// What still differs (first hunks): in the hasPath == 0 branch the original
// loads unit into ecx before storing turn, ours stores turn first; and after
// the v3 call the original computes ax = p[1].x - pos.x before az = p[1].z -
// pos.z (slots B+0x14, B+0x1c) while ours computes az first (slots B+0x10,
// B+0x04). unit ends in eax (original edi); d1 ends in ebp in both.
//
// Suspected original bug: none beyond the dead dz store and the reused
// argument home; the store of turned at 0x43d027 is live (0x43d087 reloads it).
//
// Also tried: inline (non-local) recomputation for d1 (49.0), Vec3 delta
// temporaries, and the header sets headers.py covers.

#include <math.h>
#include <stdlib.h>

struct Vec3 {
    int x, y, z;
    Vec3 operator-(const Vec3& other) const {
        Vec3 r; r.x = x - other.x; r.y = y - other.y; r.z = z - other.z; return r;
    }
    int Square() const {
        __int64 a = x, b = z;
        return (int)((a*a) >> 32) + (int)((b*b) >> 32);
    }
};

#pragma pack(push, 1)
struct UnitType_0043cd20 {
    char unknown_0[0x19a];
    int field_19a;                    // +0x19a, the rate
    int field_19e;                    // +0x19e, the long-step distance
    char unknown_1a2[0x1ba - 0x1a2];
    unsigned short max_turn;          // +0x1ba
};

struct Unit_0043cc20 {
    char unknown_0[0x66];
    short heading;                    // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3 pos;                         // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0043cd20* type;          // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags_0 : 16;        // +0x110
    unsigned int moved : 1;           // +0x110 bit 16
    unsigned int flags_17 : 15;
};
#pragma pack(pop)

// Hand-written fixed-point atan2 in the gap at 0x4b70a0.
int __stdcall FUN_0048a980(Vec3* from, Vec3* to);

// The path object: slot 5 (vtable +0x14) says whether a path is active, slot 3
// (vtable +0xc) copies `count` points out starting at `first`.
class Iface_0043dd20 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3(Vec3* out, int first, int count);
    virtual void v4();
    virtual int v5();
};

class Class_0043cc20 {
public:
    void FUN_0043cc20(Unit_0043cc20* unit, int amount);
};

class Class_0043cd20 {
public:
    Iface_0043dd20* obj;              // +0x0
    char unknown_4[0x20 - 0x4];
    int field_20;                     // +0x20
    short turn;                       // +0x24

    void FUN_0043cd20(Unit_0043cc20* unit);
};

// FUNCTION: 0x43cd20
void Class_0043cd20::FUN_0043cd20(Unit_0043cc20* unit)
{
    int hasPath = obj->v5();
    if (hasPath == 0) {
        turn = hasPath;
        int r2 = unit->type->field_19a;
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, -r2);
        return;
    }

    Vec3 p[3];
    obj->v3(p, 0, 3);

    Vec3* ppos = &unit->pos;
    Vec3 d = p[1] - *ppos;
    int gap1 = (int)_hypot(d.x, d.z);

    int dz;
    if (gap1 > 0x500000) {
        int dx = p[1].x - p[0].x;
        dz = p[1].z - p[0].z;
        int len = (int)_hypot(dx, dz);
        if (len >= 0x10000) {
            int ndx = (int)(((__int64)dx << 16) / len);
            int ndz = (int)(((__int64)dz << 16) / len);
            int back = gap1 - 0x500000;
            if (back > len)
                back = len;
            p[1].x -= (int)(((__int64)ndx * back) >> 16);
            p[1].z -= (int)(((__int64)ndz * back) >> 16);
        }
    }

    int az = p[1].z - unit->pos.z;
    int ax = p[1].x - unit->pos.x;
    int d1 = (int)(((__int64)ax * ax) >> 32) + (int)(((__int64)az * az) >> 32);

    short ang = (short)FUN_0048a980(ppos, &p[1]);
    short diff = ang - unit->heading;
    int sdiff = diff;
    int adiff = abs(sdiff);

    int bz = p[2].z - unit->pos.z;
    int bx = p[2].x - unit->pos.x;
    int d2 = (int)(((__int64)bx * bx) >> 32) + (int)(((__int64)bz * bz) >> 32);

    if (diff != 0) {
        unsigned short max = unit->type->max_turn;
        if (sdiff >= max)
            turn = max;
        else if (sdiff <= -max)
            turn = -max;
        else
            turn = diff;
        unit->heading += turn;
        unit->moved = 1;
    } else {
        turn = 0;
    }

    int turned = (int)(((__int64)((unsigned short)adiff * field_20)
                        / unit->type->max_turn));
    int rate = unit->type->field_19a;
    int t = (int)(((__int64)field_20 * field_20) >> 16);
    int q = (int)(((__int64)t << 16) / (2 * rate));
    int r = (int)(((__int64)q * q) >> 32);
    int lim = (int)(((__int64)turned * turned) >> 32) * 4;

    if (d1 > lim && d2 > r)
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, unit->type->field_19e);
    else
        ((Class_0043cc20*)this)->FUN_0043cc20(unit, -rate);
}

