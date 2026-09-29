// Decompiled by Space Bunny Free. Names are provisional.
// PARTIAL, 67.6% (503 of 488 bytes). Ballistic launch-angle solver: two roots of the
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
// What still differs: the frame is 0x38 (ours) against 0x30, i.e. one double temp too
// many, and the original flushes the `add esp,0x10` after _hypot before its first
// spill ([esp] as the slot of (double)height) where MSVC here defers it. The original
// order of evaluation is g*h, d*d, speed*speed, h*h, d2*d2, then A = s2 - gh*-2.0,
// A*s2, h2*gg, (h2 + d2), (d4*gg)*sum; only the middle of the function differs, the
// tail after the discriminant test matches instruction for instruction.
// Tried (~1000 scratch variants): statement orders, operand orders, named/inline
// temporaries, variable reuse, all header sets (headers.py, with and without --cpp).
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
    double d = _hypot((double)x, (double)z);
    d = d * d;
    double gh = (double)g * (double)height;
    double s2 = (double)speed * (double)speed;
    double sum = (double)height * (double)height+d;
    double disc = (((double)height * (double)height) * (double)gg  +  (s2 - gh*-2.0) * s2) * (d * d) - (d * d) * ((double)gg * sum);
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
    return (short)(use * 32768.0 * 0.3183098861837907);
}
