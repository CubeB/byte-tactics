// Decompiled by Opus. Names are provisional.
// Moves `value` towards `target` by `step` without overshooting.

// FUNCTION: 0x4804e0
int __stdcall FUN_004804e0(int value, int target, int step)
{
    if (value < target) {
        value += step;
        if (value >= target) {
            value = target;
        }
    } else if (value > target) {
        value -= step;
        if (value <= target) {
            value = target;
        }
    }
    return value;
}
