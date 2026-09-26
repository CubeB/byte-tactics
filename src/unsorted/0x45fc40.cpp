// Decompiled by Haiku. Names are provisional.

extern char* g_game;

// FUNCTION: 0x45fc40
void FUN_0045fc40()
{
    *(unsigned short*)((char*)g_game + 0x37ebe) &= 0xfffe;
    *(char*)((char*)g_game + 0x2bc0) = 3;
}
