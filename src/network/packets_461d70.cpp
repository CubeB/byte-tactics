// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x461d70
int __fastcall GetCurrentBuffer(int *ecx)
{
    int val = ecx[0];
    int result = 0;
    if (val >= 0) {
        result = ((int*)ecx[2])[val];
    }
    return result;
}
