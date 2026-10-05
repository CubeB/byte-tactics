// Decompiled by Opus. Names are provisional.

struct Game;
extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

class Class_00437c80 {
public:
    void FlushCache();
};

void __stdcall SetLightVector(int param_1, int param_2, int param_3);

// FUNCTION: 0x4166c0
void __stdcall FUN_004166c0(Class_004b73e0* args)
{
    SetLightVector(args->FUN_004b73e0(1, 0), args->FUN_004b73e0(2, 0), args->FUN_004b73e0(3, 0));
    (*(Class_00437c80**)((char*)g_game + 0x1437b))->FlushCache();
}
