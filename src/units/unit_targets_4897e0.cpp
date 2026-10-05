// Decompiled by Opus. Names are provisional.
// Tests bit 1 of three flag bytes 0x1c apart. A single exit through an
// unsigned char local keeps the final AND byte-sized (`mov dl, 2` hoisted,
// `and al, dl`); separate returns widen it to `mov edx, 2` / `and eax, edx`.

class Class_004897e0 {
public:
    char unknown_0[0x1f];
    unsigned char flags_1f;            // +0x1f
    char unknown_20[0x3b - 0x20];
    unsigned char flags_3b;            // +0x3b
    char unknown_3c[0x57 - 0x3c];
    unsigned char flags_57;            // +0x57

    unsigned char ChooseWeapon();
};

// FUNCTION: 0x4897e0
unsigned char Class_004897e0::ChooseWeapon()
{
    unsigned char result;
    if (flags_1f & 2)
        result = 0;
    else if (flags_3b & 2)
        result = 1;
    else
        result = flags_57 & 2;
    return result;
}
