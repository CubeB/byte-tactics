// Decompiled by Opus. Names are provisional.

bool FUN_004d8200();
int FUN_004d8260();

// Checks whether a value lies within `range` of the debug fill pattern, read
// at each of the four byte alignments.
// FUNCTION: 0x4d81a0
bool __cdecl FUN_004d81a0(int value, int range)
{
    if (FUN_004d8200() && FUN_004d8260()) {
        int pattern[2];
        pattern[0] = pattern[1] = FUN_004d8260();
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
