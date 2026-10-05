// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <stdio.h>
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
struct PlayerOwner_004269d0 {
    char unknown_0[0x95];
    unsigned char flag;              // +0x95
};

struct Player_004269d0 {
    PlayerOwner_004269d0* owner;     // +0
    char unknown_4[0x14b - 4];
};

struct Flags_004269d0 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short rest : 12;
};

struct Game {
    char unknown_0[0x1b8a];
    Player_004269d0 players[10];     // +0x1b8a
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;       // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    Flags_004269d0 flags;            // +0x2a44
    char unknown_2a46[0x2bbe - 0x2a46];
    unsigned char field_2bbe;        // +0x2bbe
    unsigned char field_2bbf;        // +0x2bbf
    unsigned char field_2bc0;        // +0x2bc0
    char unknown_2bc1[0x2c7e - 0x2bc1];
    int field_2c7e;                  // +0x2c7e
    char unknown_2c82[0x37eee - 0x2c82];
    int field_37eee;                 // +0x37eee
    char unknown_37ef2[0x391e9 - 0x37ef2];
    Class_00435110* level;           // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00512288;

int FUN_00428bc0(void);
void __stdcall FUN_00434ab0(int param);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
int GetTicks(void);
void __stdcall FUN_004bcec0(char* dest);
int PopKey(void);

// FUNCTION: 0x4269d0
void FUN_004269d0(void)
{
    char key[128];
    char buf[256];
    char path[256];

    int event = PopKey();
    switch (event) {
    case 'A':
    case 'a':
        g_game->players[g_game->localPlayer].owner->flag = 0;
        return;
    case 'C':
    case 'c':
        g_game->players[g_game->localPlayer].owner->flag = 1;
        return;
    case 'E':
    case 'e':
        g_game->field_37eee = 0;
        return;
    case 'M':
    case 'm':
        g_game->field_37eee = 1;
        return;
    case 'H':
    case 'h':
        g_game->field_37eee = 2;
        return;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        FUN_004bcec0(path);
        strcat(path, "\\Warp.ini");
        wsprintfA(key, "warp%dcampaign", event - 0x30);
        GetPrivateProfileStringA("WARPLEVELS", key, "default", buf, 0x100, path);
        wsprintfA(key, "warp%dmission", event - 0x30);
        {
            int n = GetPrivateProfileIntA("WARPLEVELS", key, 0, path);
            FUN_00434ab0(1);
            g_game->level->LoadCampaign(buf);
            if (((Class_00435c00*)g_game->level)->FUN_00435c00(n)) {
                g_game->flags.b3 = 1;
                g_game->flags.b2 = 1;
            }
        }
        return;
    default:
        if (event == 0 && g_game->field_2c7e == 0
            && DAT_00512288 >= (int)GetTicks())
            return;
        PlaySoundByName("MAINMENU", 0);
        if (FUN_00428bc0()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    938, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox((char*)g_game + 0x519, buf, 500, 1, 1);
        }
        g_game->field_2bbe = 2;
        if (FUN_00428bc0()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    155, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox((char*)g_game + 0x519, buf, 500, 1, 1);
        }
        g_game->field_2bbf = 0;
        g_game->field_2bc0 = 0;
        return;
    }
}
