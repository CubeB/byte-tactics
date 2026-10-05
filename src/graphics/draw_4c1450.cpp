// Decompiled by Haiku. Names are provisional.

int GetDisplay();

// FUNCTION: 0x4c1450
int GetFontHeight()
{
    int temp = GetDisplay();
    int ptr = *(int*)(temp + 0x204);
    unsigned int result = 0;
    result = *(unsigned char*)ptr;
    return result;
}
