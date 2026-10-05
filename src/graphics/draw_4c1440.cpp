// Decompiled by Haiku. Names are provisional.

int GetDisplay();

// FUNCTION: 0x4c1440
int GetFont()
{
    return *(int*)(GetDisplay() + 0x204);
}
