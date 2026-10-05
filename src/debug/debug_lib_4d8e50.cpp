// Decompiled by Haiku. Names are provisional.
extern int DAT_005289bc;

// FUNCTION: 0x4d8e50
int __cdecl SetOutOfMemoryHandler(int param_1)
{
    int temp = DAT_005289bc;
    DAT_005289bc = param_1;
    return temp;
}
