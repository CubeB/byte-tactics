// Decompiled by GPT-5.6-Terra. Names are provisional.
// Partial: this has the inferred ballistic discriminant and both angle paths,
// but retains too many double locals. It emits a 0x38-byte frame plus esi;
// the original reuses six double slots in a 0x30-byte x87 frame (28.9%).
#include <math.h>

extern "C" double __cdecl _hypot(double x, double y);
double __cdecl FUN_004e67f0(double value);

#pragma pack(push, 1)
struct Game_0049a890 {
    char unknown_0[0x14263];
    int gravity;
};
#pragma pack(pop)

extern Game_0049a890* g_game;

// FUNCTION: 0x49a890
short __stdcall FUN_0049a890(int param_1, int param_2, int param_3,
                             int param_4, float param_5)
{
    int gravity = g_game->gravity;
    double distance = _hypot((double)param_1, (double)param_3);
    double height = (double)param_2;
    double speed2 = (double)param_4 * (double)param_4;
    double distance2 = distance * distance;
    double height_distance2 = height * height + distance2;
    double discriminant = (height * height * (double)(gravity * gravity) +
        (speed2 - (double)gravity * height * -2.0) * speed2) * distance2 * distance2 -
        distance2 * distance2 * (double)(gravity * gravity) * height_distance2;
    if (discriminant < 0.0)
        return (short)0x8000;

    double root = sqrt(discriminant);
    double numerator = (speed2 + (double)gravity * height) * distance2;
    double denominator = height_distance2 + height_distance2;
    double low = (numerator + root) / denominator;
    double high = (numerator - root) / denominator;
    double low_angle = low <= 0.0 ? 1.570796326794895 : FUN_004e67f0(sqrt(low) / distance);
    double high_angle = high <= 0.0 ? 1.570796326794895 : FUN_004e67f0(sqrt(high) / distance);
    if ((param_5 < (float)low_angle && low_angle < 0.7853981633974475) ||
        ((float)param_5 <= high_angle && high_angle < 0.7853981633974475))
        return (short)(low_angle * 32768.0 * 0.318309886183791);
    return (short)0x8000;
}
