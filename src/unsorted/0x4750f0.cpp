// Decompiled by Haiku. Names are provisional.

struct UnknownStruct;
extern UnknownStruct* g_game;

// FUNCTION: 0x4750f0
bool __fastcall FUN_004750f0(void* param_1)
{
    return *(unsigned int*)((char*)param_1 + 8) <= *(unsigned int*)((char*)g_game + 0x38a47);
}
