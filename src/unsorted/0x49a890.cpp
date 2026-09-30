// Decompiled by Space Bunny Free, finished by GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// Retry #1736: the saved discriminant association was independently re-checked at 78.2% (497/488); no MATCH. The post-_hypot x87 load/spill schedule still differs.
// PARTIAL. The saved body scores 78.2% but is arithmetically WRONG (see the pass note at the
// bottom for the correct algorithm, now established from a full x87 simulation). With the
// correct arithmetic the best this pass reaches is 51.4% (469 of 488 bytes). Ballistic
// launch-angle solver: two roots of the trajectory equation, each tested against zero and
// turned into a launch angle with acos(sqrt(root) / speed) (0x4e67f0 is the CRT's _CIacos:
// argument in st(0), then fpatan(sqrt(1 - x*x), x)), pi/2 substituted when the root is not
// positive.
//
// Facts settled by the disassembly: __stdcall, five int-sized args (six fild loads, no
// movzx), returns short (mov ax,0x8000 or the _ftol result). g*g is an int product
// (imul) computed BEFORE the _hypot call and converted later, so `gg` is hoisted.
// The `- gh * -2.0` literal survives only when written as a subtraction. The pi/4 tests
// are `<=`; the second root is tested as `low > angle` (fcompp order).
// Reusing `d` for dist, dist^2 and the numerator matters (worth ~5 points), as does
// the `#include <stdio.h>` (headers change the x87 spill choices; math.h alone: 65%).
//
// What still differs: after _hypot, the original loads gravity then height, squares
// the gravity-height product, and uses a distinct x87 spill schedule. This source
// loads height before gravity and differs through the discriminant arithmetic;
// its generated function is 9 bytes longer, shifting later branch destinations.
// Tried (~1000 scratch variants): statement orders, operand orders, named/inline
// temporaries, variable reuse, all header sets (headers.py, with and without --cpp).
//
// deepseek-v4.1-flash re-attempted (issue #1104): ~60 more scratch variants, all 67.6%
// or worse. Confirmed headers.py finds no fixing set. Factoring d4 out (v_fact) gives
// 487 bytes (one short) but 66.4%. The first divergence is fixed before any arithmetic:
// the original filds g then height, ours filds height then g, and defers `add esp,0x10`
// to reuse the hypot argument slots for scratch. Swapping the gh operands, splitting gh
// into a helper, changing the d=d*d / d2 model, naming d4/A/h2, reordering the disc
// terms, and 2.0*gh all leave the schedule byte-identical at 67.6%. The middle looks
// like one allocator state seeded by that first g/height load order, not by the disc
// expression. No check.py MATCH.
//
// GPT-6.1-sol (issue #1431): five checker runs. Storing `(double)g` into `gh` before
// multiplying by height raises the best score from 67.6% to 68.3%; separate converted
// operands tie, while spelling out the discriminant temporaries or nesting the angle
// tests scores lower. The remaining first divergence is in the post-_hypot x87 load /
// spill schedule; later branch offsets and return-path layout also differ. Best source
// kept here at 78.2%; no MATCH.
// Lead #1431 tried retaining the squared _hypot result in a separate local; it scored 70.3%. GPT-6.1-sol then associated discriminant factors as left-associative `* d * d - d * d * gg * sum`, scoring 78.2%.
//
// ---- space-bunny-free pass (issue 1954) ----
// The x87 stack was re-simulated instruction by instruction, which settles the ALGORITHM
// (the 78.2% version has the wrong arithmetic; its score was accidental alignment). With
// B = the 0x30 frame base, the argument homes are x=B+0x34, height=B+0x38, z=B+0x3c,
// speed=B+0x40, angle=B+0x44. The two `sub esp,8` before the call make the hypot filds
// read B+0x34 and B+0x3c, so the FIRST fild is x (not z) - earlier notes had this backwards.
// g is stored to B+0x0 and gg OVERWRITES z's dead home B+0x3c, which is why the later
// `fild [esp+0x3c]` loads gg, not z. Angle really is a float (`fld dword ptr`, B+0x44).
// Settled names, from the spill slots:
//   gh = g*h        d = dist*dist (dist = _hypot(x,z))   d4 = d*d
//   s2 = speed*speed  h2 = h*h  (re-read from B+0x00, so height stays live)
// The discriminator is `faddp st(3)`, which writes the DEEPEST slot, so the subtrahend is
// the one built by addition:
//   disc = gg*(h2 + d)*d4 - ((s2 - gh*-2.0)*s2 + h2*gg)*d4
// and the shared denominator, kept in B+0x18/B+0x20 and built by `fadd st(0),st(0)`, is
//   den  = (h2*gg + d4*gg) + itself = 2*gg*(h2 + d4),   num = (s2 + gh)*d
//   high = sqrt(disc)/den      low = (num - sqrt(disc))/den
// Note (h2 + d) uses d2 but (h2 + d4) uses d4, so the two sums are NOT the same expression.
// Best this pass 51.4% (469 of 488), versus 78.2% for the older, arithmetically wrong body:
// our body now has the right maths but is 19 bytes SHORT, and the frame is `sub esp,0x20`
// against the original's 0x30. The original keeps SIX doubles live at once (gh, d, s2, h2,
// d4, dist) and so allocates six homes; naming a `dist` that is used twice to keep it alive
// forces the spill of dist but changes the schedule and scored 51.4%. Sharing `sum2` as one
// variable (which the original does) scores 36.0%, spelling the subtrahend as the factored
// `gg*(h2+d4)` form scores 41.6%: MSVC keeps re-deriving or re-storing the shared terms
// instead of holding the six live values the original's allocator expects. What still
// differs is register pressure, not the algorithm or the calling convention.
// NOTE: an earlier 52.7% reading of this body was a stale build\obj object; after deleting
// build/obj/unsorted it reproduces at 51.4%, which is the figure to trust.
#include <stdio.h>
#include <math.h>

extern "C" double __cdecl _hypot(double x, double y);

#pragma pack(push, 1)
struct Game_0049a890 {
    char unknown_0[0x14263];
    int gravity;
};
#pragma pack(pop)

extern Game_0049a890* g_game;

// FUNCTION: 0x49a890
short __stdcall FUN_0049a890(int x, int height, int z, int speed, float angle)
{
    int g = g_game->gravity;
    int gg = g * g;
    double dist = _hypot((double)x, (double)z);
    double gh = (double)g * (double)height;
    double d = dist * dist;
    double s2 = (double)speed * (double)speed;
    double h2 = (double)height * (double)height;
    double d4 = d * d;
    double disc = (h2 + d) * (h2 * (double)gg + d4 * (double)gg) - ((s2 - gh * -2.0) * s2 * d4);
    if (disc < 0.0)
        return 0x8000;
    double root = sqrt(disc);
    d = (s2 + gh) * d;
    double sum2 = (h2 * (double)gg + d4 * (double)gg) + (h2 * (double)gg + d4 * (double)gg);
    double high = root / sum2;
    double low = (d - root) / sum2;
    double highAngle;
    double lowAngle;
    if (high > 0.0)
        highAngle = acos(sqrt(high) / (double)speed);
    else
        highAngle = 1.570796326794895;
    if (low > 0.0)
        lowAngle = acos(sqrt(low) / (double)speed);
    else
        lowAngle = 1.570796326794895;
    double use;
    if (angle < highAngle && highAngle <= 0.7853981633974475)
        use = highAngle;
    else if (lowAngle > angle && lowAngle <= 0.7853981633974475)
        use = lowAngle;
    else
        return 0x8000;
    return (short)(use * 32768.0 * 0.3183098861837907);
}
