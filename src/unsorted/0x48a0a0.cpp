// Decompiled by Opus. Names are provisional.
// Sibling of 0x48a060: stores the high words of a fixed-point position's x
// and z into entry param_3 (0x1c bytes each), keeping z away from 0x8000,
// then clears bits 10-14 of the flags at +0xba.

// FUNCTION: 0x48a0a0
void __stdcall FUN_0048a0a0(char* param_1, int* param_2, int param_3)
{
    short* elem = (short*)(param_1 + param_3 * 0x1c + 4);
    elem[0] = (short)(param_2[0] >> 16);
    elem[1] = (short)(param_2[2] >> 16);
    if (elem[1] == (short)0x8000) {
        elem[1] = (short)0x8001;
    }
    *(unsigned short*)(param_1 + 0xba) &= 0x83ff;
}
