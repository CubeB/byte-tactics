// Decompiled by Haiku. Names are provisional.

extern int DAT_00505490[];
extern int DAT_00512774;

// FUNCTION: 0x442380
void __stdcall FUN_00442380(int param_1, int param_2)
{
    int value = *(short*)((char*)param_2 + 0xba);
    if (value >= 0) {
        DAT_00512774 = DAT_00505490[value];
    }
}
