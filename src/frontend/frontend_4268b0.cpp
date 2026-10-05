// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <string.h>

class Class_00435110 {
public:
    void LoadCampaign(char* name);
};

class Class_00435c00 {
public:
    int FUN_00435c00(int value);
};

#pragma pack(push, 1)
struct Flags_004268b0 {
    unsigned short b0 : 1;             // +0x2a44
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short rest : 12;
};

struct Game {
    char unknown_0[0x2a44];
    Flags_004268b0 flags;              // +0x2a44
    char unknown_2a46[0x391e9 - 0x2a46];
    Class_00435110* level;             // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004bcec0(char* param);
void __stdcall FUN_00434ab0(int param);

// FUNCTION: 0x4268b0
void __stdcall LoadWarpLevel(int param_1)
{
    char key[128];
    char path[256];
    char value[256];

    FUN_004bcec0(path);
    strcat(path, "\\Warp.ini");
    wsprintfA(key, "warp%dcampaign", param_1);
    GetPrivateProfileStringA("WARPLEVELS", key, "default", value, 0x100, path);
    wsprintfA(key, "warp%dmission", param_1);
    int n = GetPrivateProfileIntA("WARPLEVELS", key, 0, path);
    FUN_00434ab0(1);
    g_game->level->LoadCampaign(value);
    if (((Class_00435c00*)g_game->level)->FUN_00435c00(n)) {
        g_game->flags.b3 = 1;
        g_game->flags.b2 = 1;
    }
}
