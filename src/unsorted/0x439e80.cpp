// Decompiled by Haiku. Names are provisional.

extern void* g_game;

class Class_00439e80
{
public:
    void FUN_00439e80(int param);
};

// FUNCTION: 0x439e80
void Class_00439e80::FUN_00439e80(int param)
{
    *(unsigned int*)((char*)this + 0x6) |= 1;
    int val = *(int*)((char*)g_game + 0x38a47);
    *(int*)((char*)this + 0xa) = val + param;
}
