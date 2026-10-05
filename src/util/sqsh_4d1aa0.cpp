// Decompiled by Opus. Names are provisional.
// Scales a value by 120% (mode 1) or 110% (mode 2) and adds 119; any other
// mode gives 0.

// FUNCTION: 0x4d1aa0
unsigned int __stdcall FUN_004d1aa0(unsigned int value, int mode)
{
    unsigned int result;
    switch (mode) {
    case 1:
        result = value * 120 / 100 + 119;
        break;
    case 2:
        result = value * 110 / 100 + 119;
        break;
    default:
        result = 0;
    }
    return result;
}
