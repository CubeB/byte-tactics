// Decompiled by Haiku. Names are provisional.

extern void* g_game;
extern void __stdcall FUN_004816a0(int);

// FUNCTION: 0x417540
void __stdcall CmdNowISee(void*)
{
    void* p_game = g_game;
    unsigned short* p = (unsigned short*)((char*)p_game + 0x14281);
    *p = *p & 0xfffe;

    p_game = g_game;
    p = (unsigned short*)((char*)p_game + 0x14281);
    *p = *p & 0xfffd;

    FUN_004816a0(1);
}
