// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Partial (53.5%). Clamp a segment: given A = arg1 (base), B = arg2 (target)
// and a maximum length, write B to *out when |B-A| <= maxLen, else write
// A + (B-A)*maxLen/|B-A| (16.16 fixed point scaling).
//
// The control flow, the arithmetic and the call sequence (__ftol then
// __allshl/__alldiv/__allmul/__allshr) all match. What still differs:
// - The original keeps two copies of the deltas: one struct at [esp+0x10]
//   used by the three 64-bit multiplies, and a second copy at [esp+0x1c]
//   whose only use is the three `fild` operands of the square sum. Here the
//   compiler folds the two copies into one, because the second set is a
//   plain copy of the first. Probing by-value struct helpers (`Length(Vec3)`,
//   `Length(const Vec3&)`, `Length(int,int,int)`, `Dot(Vec3,Vec3)`, an
//   inlined `operator-`) all get SROA'd to one set by this compiler; the
//   original's second, higher copy looks like a materialised by-value
//   argument, but no source shape tried reproduces it. The original frame is
//   also larger (sub esp,0x18 versus 0x0c here) and keeps `a` in ecx, freeing
//   ebx for dx across the runtime calls; here `a` takes ebx and dx is
//   reloaded from the multiply set.
// - Consequence: register allocation in the prologue and in the clamp branch
//   differs, and `a` is not loaded before the callee-save pushes.
#include <math.h>

struct Vec3i_0040beb0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
    Vec3i_0040beb0 operator-(const Vec3i_0040beb0& o) const
    {
        Vec3i_0040beb0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
};

// FUNCTION: 0x40beb0
void __stdcall FUN_0040beb0(Vec3i_0040beb0* out, const Vec3i_0040beb0* a,
                            const Vec3i_0040beb0* b, int maxLen)
{
    Vec3i_0040beb0 d = *b - *a;
    int len = (int)sqrt((double)(b->x - a->x) * (b->x - a->x)
                      + (double)(b->y - a->y) * (b->y - a->y)
                      + (double)(b->z - a->z) * (b->z - a->z));
    if (maxLen >= len) {
        *out = *b;
        return;
    }
    int scale = (int)(((__int64)maxLen << 16) / len);
    out->x = a->x + (int)(((__int64)d.x * scale) >> 16);
    out->y = a->y + (int)(((__int64)d.y * scale) >> 16);
    out->z = a->z + (int)(((__int64)d.z * scale) >> 16);
}
