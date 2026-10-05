// Decompiled by Haiku. Names are provisional.
struct Game;
extern Game* g_game;

struct Class_004ced40 {
public:
    void FUN_004ced40();
};

// FUNCTION: 0x416810
void __stdcall FUN_00416810(int unused)
{
    Class_004ced40* obj = *(Class_004ced40**)((char*)g_game + 0x10);
    obj->FUN_004ced40();
}
