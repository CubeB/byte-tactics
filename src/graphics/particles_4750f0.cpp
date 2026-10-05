// Decompiled by Haiku. Names are provisional.

struct UnknownStruct;
extern UnknownStruct* g_game;

class Class_004750f0 {
public:
    bool FUN_004750f0();
};

// FUNCTION: 0x4750f0
bool Class_004750f0::FUN_004750f0()
{
    return *(unsigned int*)((char*)this + 8) <= *(unsigned int*)((char*)g_game + 0x38a47);
}
