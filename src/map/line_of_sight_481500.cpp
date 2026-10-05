// Decompiled by Opus. Names are provisional.

extern char* g_game;

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x481500
void FUN_00481500()
{
    *(int*)(g_game + 0x14277) = 0;
    *(void**)(g_game + 0x1427b) = FUN_004d83b0("EYEBALL MEMORY", 0x2d0);
}
