// Decompiled by Haiku. Names are provisional.

int FUN_004b6220();

// FUNCTION: 0x4c1450
int FUN_004c1450()
{
    int temp = FUN_004b6220();
    int ptr = *(int*)(temp + 0x204);
    unsigned int result = 0;
    result = *(unsigned char*)ptr;
    return result;
}
