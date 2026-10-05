// Decompiled by Haiku. Names are provisional.

extern void* g_game;

// FUNCTION: 0x41e260
int IsFadeDone()
{
    void* game = g_game;
    return *(int*)((char*)game + 0x39063);
}
