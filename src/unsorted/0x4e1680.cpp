// Decompiled by Haiku. Names are provisional.

unsigned char __cdecl FUN_004e35b0(void);

extern unsigned char DAT_00529e6c;
extern unsigned char DAT_00529e70;

// FUNCTION: 0x4e1680
unsigned char __cdecl FUN_004e1680(void)
{
    if (DAT_00529e6c == 0) {
        DAT_00529e6c = 1;
        unsigned char result = FUN_004e35b0();
        if (result != 0) {
            DAT_00529e70 = 1;
        }
    }
    return DAT_00529e70;
}
