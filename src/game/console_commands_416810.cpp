// Decompiled by Haiku. Names are provisional.
struct Game;
extern Game* g_game;

struct Class_004ced40 {
public:
    void StopCdAudio();
};

// FUNCTION: 0x416810
void __stdcall CmdCDStop(int unused)
{
    Class_004ced40* obj = *(Class_004ced40**)((char*)g_game + 0x10);
    obj->StopCdAudio();
}
