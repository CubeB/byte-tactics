// Decompiled by Opus. Names are provisional.
struct Game;
extern Game* g_game;

class Sound {
public:
    int Is3DEnabled();
    void Disable3D();
    void Enable3D();
};

void SaveSettings();

// FUNCTION: 0x416820
void __stdcall CmdSound3D(int unused)
{
    if ((*(Sound**)((char*)g_game + 0x10))->Is3DEnabled()) {
        (*(Sound**)((char*)g_game + 0x10))->Disable3D();
        SaveSettings();
    } else {
        (*(Sound**)((char*)g_game + 0x10))->Enable3D();
        SaveSettings();
    }
}
