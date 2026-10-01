// Decompiled by Space Bunny Free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, edited by deepseek-v4.1-flash. Names are provisional.
// space-bunny-free (issue #3386): 83.6% retained, no MATCH. Ran an x87 stack SIMULATOR over
// the original (build/scratch/0x49a890/sim.py) to recover the exact expression tree, and it
// CORRECTS the note above: the original's disc is NOT `(h2*gg + (s2-gh*-2.0)*s2)*d*d -
// d*d*gg*sum`, it is the mirror image with a different value:
//     d4  = d*d
//     A   = s2 - gh*-2.0            (0x49a91c fsubr)
//     T   = A*s2                    (0x49a930 fmul)
//     h2gg= h2*gg                   (0x49a93a fmul)
//     B   = T + h2gg                (0x49a950 faddp st(3), addends T then h2gg)
//     disc= (d4*gg)*sum - B*d4      (0x49a958, 0x49a95e, 0x49a964 fsubp st(1))
// MSVC x87 temp slots (esp=E-0x30 after the `add esp,0x10`), each REUSED later, so the frame
// only needs six doubles: [E-0x30] height then d2 | [E-0x28] speed | [E-0x20] dist then s2
// then d4 | [E-0x18] h2 then gg then sum | [E-0x10] d4 then h2 | [E-0x08] gh then sqrt(disc).
// Evaluated in this order: gh, d, s2, h2, gh*-2.0, A, d4, T, h2gg, sum, d4*gg, B, *sum, *d4.
// Scoring (all free --sym runs, base 83.6%): the mirrored `d*d*gg*sum - (...)*d*d` with gh
// before d scored 67.8%; the same disc on the base statement order scored 72.7% both ways;
// hoisting `h2*gg` into a named local but keeping the base order scored 79.4% (488 bytes).
// So the disc shape alone is NOT the cause, and matching it does not move the schedule.
// NOT FIXED, and nobody should retry these: (1) moving `gh` ahead of `d`, (2) reversing the
// disc's two terms, (3) either order of the addends inside the disc parentheses.
// Issue #3175 retry by GPT-6.1-sol: root checker confirmed 83.6% (408/488 bytes), no MATCH. Eight source-order/local-expression variants scored 74.6%, 74.6%, 74.6%, 82.2%, 83.6%, 83.6%, 83.6%, and 83.6%; best retained. Remaining mismatch is the post-_hypot x87 schedule, where original loads g then height and spills height before multiplying, while this source schedules height/g and different x87 temporaries.
// #2981 retry by GPT-6.1-sol: six checks retained 83.6%; the lower-scoring
// inline-expression variant did not change the post-_hypot x87 schedule.
// deepseek-v4.1 (issue #2573): 83.6%, exact 488/488 bytes, only ONE diff region left, the
// post-_hypot x87 load/spill schedule (see bottom note). What fixed the old 78.2%: the angle
// scaling must write the first factor back into the SAME `use` local,
// `use = use * 32768.0; return (short)(use * 0.3183098861837907);`, otherwise MSVC5 folds
// 32768.0*0.3183098861837907 into one constant, which costs an fmul per path (+6 bytes), turns
// the shared return tail into two tails (+9 bytes total: 497 vs 488) and forces a different
// local-home layout. Do not merge those two statements back into one expression.
// PARTIAL, best 83.6% (488 of 488 bytes). Ballistic launch-angle solver: two roots of the
// trajectory equation, each tested against zero and turned into a launch angle with
// acos(sqrt(root) / speed) (0x4e67f0 is the CRT's _CIacos: argument in st(0), then
// fpatan(sqrt(1 - x*x), x)), pi/2 substituted when the root is not positive.
//
// Facts settled by the disassembly: __stdcall, five int-sized args (six fild loads, no
// movzx), returns short (mov ax,0x8000 or the _ftol result). g*g is an int product
// (imul) computed BEFORE the _hypot call and converted later, so `gg` is hoisted.
// The `- gh * -2.0` literal survives only when written as a subtraction. The pi/4 tests
// are `<=`; the second root is tested as `low > angle` (fcompp order).
// Reusing `d` for dist, dist^2 and the numerator matters (worth ~5 points), as does
// the `#include <stdio.h>` (headers change the x87 spill choices; math.h alone: 65%).
//
// What still differs (deepseek-v4.1, issue #2573): the only diff hunk left is the
// post-_hypot x87 schedule, 53 lines. Ours filds height, g, speed; the original
// filds g, height, then stores height to slot0 and does `fmul [esp]`, i.e. it
// spills the second conversion and keeps g as the multiplicand. Everything from
// the first `fstp [esp+0x20]` (2*sum) to the end of the function is byte-identical.
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
// deepseek-v4.1-flash (issue 3404): five checker runs, no gain, best still 83.6% (488 bytes).
// Swapping the two gh statements to height-first (74.6%), folding them into one expression
// `double gh = (double)height * (double)g;` (74.6%) or `gh = (double)height * gh;` (83.6,
// byte-identical) all leave the residual hunk untouched, so the gh spelling is not the lever
// for the first diverging fild order.
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
    double distance = _hypot((double)x, (double)z);
    double d = distance * distance;
    double gh = (double)g;
    gh = gh * (double)height;
    double s2 = (double)speed * (double)speed;
    double sum = (double)height * (double)height+d;
    double disc = (((double)height * (double)height) * (double)gg  +  (s2 - gh*-2.0) * s2) * d * d - d * d * (double)gg * sum;
    if (disc < 0.0)
        return 0x8000;
    disc = sqrt(disc);
    d = (s2+gh)*d;
    double high = (disc+d)/(sum+sum);
    double low = (d - disc)/(sum+sum);
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
    use = use * 32768.0;
    return (short)(use * 0.3183098861837907);
}
