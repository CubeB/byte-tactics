// Decompiled by Haiku. Names are provisional.

extern int FUN_004b6220();

// FUNCTION: 0x4c1b00
int __cdecl FUN_004c1b00()
{
    int obj = FUN_004b6220();
    int field_16e = *(int*)(obj + 0x16e);
    int field_172 = *(int*)(obj + 0x172);
    if (field_16e == field_172) {
        return 0;
    }
    return *(int*)(obj + 0xf6 + field_172 * 4);
}
