// Decompiled by Space Bunny Free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, edited by deepseek-v4.1-flash, finished by mimo-v2.6-pro, re-verified by space-bunny-free, edited and finished by Claude Opus 5.5. Names are provisional.
// Ballistic launch-angle solver: the two roots
// of the trajectory quadratic, each turned into a launch angle with
// acos(sqrt(root) / speed), pi/2 when a root is not positive.
#include <stdio.h>
#include <math.h>

// PI stays this literal and `use / PI` a separate statement from
// `use * 32768.0`: the constant pool depends on it.
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
short __stdcall SolveLaunchAngle(int x, int height, int z, int speed, float angle)
{
    int g = g_game->gravity;
    int gg = g * g;
    // The redundant parentheses stay, with these exact counts: they steer the
    // x87 schedule. disc is expanded as (s2 + 2*gh) * s2 + h2 * gg.
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
