// Decompiled by Space Bunny Free. Names are provisional.
// PARTIAL, 44.1% (491 of 488 bytes; up from 28.9%). Ballistic launch-angle
// solver: two roots of the trajectory equation, each tested against zero and
// converted to a launch angle by atan(sqrt(root)/speed), with pi/2 substituted
// when the root is non-positive.
//
// THE SIGNATURE IS RIGHT AND THE RECORDED MANGLED NAME IS WRONG. A previous
// attempt recorded `?FUN_0049a890@@YGFHHHHM@Z`, i.e. `float __cdecl (float,
// unsigned short x4, float)`, and `tools/check.py` prints that name, so it looks
// authoritative. It is wrong in all three respects, and `data/progress.csv`
// line 1971 shows where it comes from: it is another model's recorded *claim*
// about this function, not something the exe or our own object says. The
// disassembly settles each part:
//   - `ret 0x14`, the callee popping five dwords, so `__stdcall`, not cdecl.
//   - six `fild dword ptr [esp+N]` loads and not one `movzx` anywhere, so the
//     integer parameters are 32-bit signed `int`, not `unsigned short` and not
//     floats. A float parameter would load with `fld`, and a 16-bit one with
//     `fild word` or `movzx`.
//   - the epilogue returns `mov ax, 0x8000` or the `_ftol` result, so the
//     return is `short`, not `float`.
// Do not "fix" this declaration to match the recorded name. I spent a brief
// telling a worker to do exactly that, and the worker was right to refuse.
//
// What is settled: the frame is `sub esp, 0x30` and this file emits 0x38, one
// double too many live at the peak, which shifts every `[esp+N]` operand.
// `FUN_004e67f0` is fastcall, not cdecl: it does `sub esp,8; fst [esp]` itself,
// so the argument arrives in st(0) and declaring it `double __fastcall` removed
// the argument push and pop around both calls. The pi/4 tests are `<=`, not
// `<`: the original uses `test ah,0x41` where a `<` gives `test ah,1`. Writing
// `gravity2` as an inline `(double)(gravity*gravity)` rather than a named local
// is worth about eighteen points, because a `double` local for that term costs a
// frame slot. Precomputing it before the `_hypot` call is worth a further
// thirteen by removing a `push esi`.
//
// What is left, in rough order of size: the 0x38 frame; the original multiplies
// `gh` by a literal -2.0 with `fmul qword [0x4fda60]` and every spelling tried
// is folded by MSVC 5 into `fadd st(0),st(0)`, so the literal never survives (a
// `static const double` at file scope is the untried idea); the order of the
// early multiplications (original gh, d2, v2, h2, d4 against ours h2, gh, v2,
// d2); the second half keeps `root` and `denom` in x87 registers where the
// original spills both to frame slots and reloads; and the `_hypot` argument
// conversions run right-to-left in ours, left-to-right in the original.
//
// NOT a bug, contrary to what an earlier note here said. The first angle block
// was described as testing `low` while using `high`. The disassembly refutes
// that: at 0x49a9b3 the compared value is `[esp+0x28]` and at 0x49a9c4 the
// value taken the square root of is also `[esp+0x28]`, and the second block
// compares and uses `[esp+0x20]`. Each block tests and uses the same root, so
// the original is self-consistent and the apparent mismatch is in this file's
// own transcription of which root is which, not a defect in the game's code.

#include <math.h>

extern "C" double __cdecl _hypot(double x, double y);
double __fastcall FUN_004e67f0(double value);

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
    int gravity = g_game->gravity;
    double dist = _hypot((double)x, (double)z);
    double gravityHeight = (double)gravity * (double)height;
    double height2 = (double)height * (double)height;
    double dist2 = dist * dist;
    double dist4 = dist2 * dist2;
    double dist2Height2 = height2 + dist2;
    double speed2 = (double)speed * (double)speed;
    double disc = ((speed2 + gravityHeight * -2.0) * speed2 +
        (double)gravity * (double)gravity * height2) * dist4 -
        dist4 * (double)gravity * (double)gravity * dist2Height2;
    if (disc < 0.0)
        return 0x8000;
    double root = sqrt(disc);
    double denom = dist2Height2 + dist2Height2;
    double numer = (speed2 + gravityHeight) * dist2;
    double high = (numer + root) / denom;
    double low = (numer - root) / denom;
    double highAngle;
    double lowAngle;
    if (low <= 0.0)
        highAngle = 1.570796326794895;
    else
        highAngle = FUN_004e67f0(sqrt(high) / (double)speed);
    if (low <= 0.0)
        lowAngle = 1.570796326794895;
    else
        lowAngle = FUN_004e67f0(sqrt(low) / (double)speed);
    double use;
    if (angle < highAngle && highAngle <= 0.7853981633974475)
        use = highAngle;
    else if (angle <= lowAngle && lowAngle <= 0.7853981633974475)
        use = lowAngle;
    else
        return 0x8000;
    return (short)(use * 32768.0 * 0.3183098861837907);
}
