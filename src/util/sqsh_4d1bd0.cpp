// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x4d1bd0
int __stdcall SquashChecksum(unsigned char* param_1, int param_2)
{
    int sum = 0;
    unsigned char* end = param_1 + param_2;
    for (; param_1 < end; param_1++) {
        sum += *param_1;
    }
    return sum;
}
