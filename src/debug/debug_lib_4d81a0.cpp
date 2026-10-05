// Decompiled by Opus. Names are provisional.

bool FUN_004d8200();
int GetMemSetValue();

// Checks whether a value lies within `range` of the debug fill pattern, read
// at each of the four byte alignments.
// FUNCTION: 0x4d81a0
bool __cdecl IsNearFillPattern(int value, int range)
{
    if (FUN_004d8200() && GetMemSetValue()) {
        int pattern[2];
        pattern[0] = pattern[1] = GetMemSetValue();
        for (unsigned int i = 0; i < 4; i++) {
            int v = *(int*)((char*)pattern + i);
            if (v - range <= value && value <= v + range) {
                return true;
            }
        }
        return false;
    }
    return false;
}
