// Decompiled by Opus. Names are provisional.
struct Game;
extern Game* g_game;

class Class_004cfea0 {
public:
    int FUN_004cfea0();
};

class Class_004cfe90 {
public:
    void FUN_004cfe90();
};

class Class_004cfe80 {
public:
    void FUN_004cfe80();
};

void FUN_00430f00();

// FUNCTION: 0x416820
void __stdcall FUN_00416820(int unused)
{
    if ((*(Class_004cfea0**)((char*)g_game + 0x10))->FUN_004cfea0()) {
        (*(Class_004cfe90**)((char*)g_game + 0x10))->FUN_004cfe90();
        FUN_00430f00();
    } else {
        (*(Class_004cfe80**)((char*)g_game + 0x10))->FUN_004cfe80();
        FUN_00430f00();
    }
}
