// Decompiled by Space Bunny Free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, edited by deepseek-v4.1-flash, finished by mimo-v2.6-pro, re-verified by space-bunny-free, edited and finished by Claude Opus 5.5. Names are provisional.
// Claude Opus 5.5 (#5569): MATCH. Ballistic launch-angle solver: the two roots
// of the trajectory quadratic, each turned into a launch angle with
// acos(sqrt(root) / speed), pi/2 when a root is not positive.
// Two things made it match after many passes at 83.6% to 97.9%:
//  - The square of (v^2 + g*h) is expanded as `(s2 + 2*gh) * s2 + h2 * gg`.
//    MSVC 5 turns the `+ 2*gh` into the original's `- gh * -2.0` (a constant
//    moved outward takes the sign of the sum) and then schedules that
//    subtraction before the fild of gg, as the original does; spelling
//    `- gh*-2.0` itself never did.
//  - The redundant parentheses are load-bearing. The front end keeps a node
//    for each pair around a floating-point expression (one pair at the top
//    of an initializer is dropped, and pairs around integer expressions or
//    call arguments change nothing), and those nodes steer C2's x87
//    schedule: the pairs on `_hypot` load g before height after the call,
//    the four pairs on h2 put the reload of height before the s2 multiply,
//    and the pairs on (s2 + 2*gh) * s2, on its sum with h2 * gg and on
//    d * d * gg give the original's order (A, gg, A*s2, h2*gg, sum,
//    d*d*gg). The counts are exact: one pair more or fewer on h2 gives 94.4%
//    or 97.9%, and one or two pairs on _hypot give 68%. This is the smallest
//    set a search over paren counts found; no no-op casts are needed (the
//    old 97.9% lead needed `(double)` casts of doubles).
// Constants: 1/PI only matches the pool as 1/3.14159265358979, so PI is that
// literal and `use / PI` stays a separate statement from `use * 32768.0`.
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

extern "C" double __cdecl _hypot(double x, double y);

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14263];
    int gravity;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x49a890
short __stdcall FUN_0049a890(int x, int height, int z, int speed, float angle)
{
    int g = g_game->gravity;
    int gg = g * g;
    double distance = (((_hypot(x, z))));
    double d = distance * distance;
    double gh = (double)g * (double)height;
    double h2 = (((((double)height * (double)height))));
    double s2 = (double)speed * (double)speed;
    double sum = h2 + d;
    double disc = (((((s2 + 2*gh) * s2)) + h2 * gg)) * (d * d) - (((d * d * gg)) * sum);
    if (disc < 0.0)
        return 0x8000;
    disc = sqrt(disc);
    d = (s2 + gh) * d;
    double high = (disc + d) / (2*sum);
    double low = (d - disc) / (2*sum);
    double highAngle;
    double lowAngle;
    if (high > 0.0)
        highAngle = acos(sqrt(high) / (double)speed);
    else
        highAngle = PI / 2;
    if (low > 0.0)
        lowAngle = acos(sqrt(low) / (double)speed);
    else
        lowAngle = PI / 2;
    double use;
    if (angle < highAngle && highAngle <= PI / 4)
        use = highAngle;
    else if (lowAngle > angle && lowAngle <= PI / 4)
        use = lowAngle;
    else
        return 0x8000;
    use = use * 32768.0;
    return (short)(use / PI);
}
