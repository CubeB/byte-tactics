// Decompiled by space-bunny-free, finished by mimo-v2.6-flash. Names are provisional.

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct PlayerInfo_00495010 {
    char unknown_0[0x9b];
    unsigned short bits_9b_0 : 6;      // +0x9b, bits 0 to 5
    unsigned short flag_9b_6 : 1;      // bit 6 (mask 0x40)
    unsigned short bits_9b_7 : 9;
};

struct Player_00495010 {               // 0x14b bytes
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00495010* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Sub_00495010 {
    char unknown_0[0x10];
};

struct Game_00495010 {
    char unknown_0[0x519];
    Sub_00495010 sub;                  // +0x519
    char unknown_529[0x1b63 - 0x529];
    Player_00495010 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    unsigned short flags;              // +0x2bee
    char unknown_2bf0[0x2c74 - 0x2bf0];
    unsigned char field_2c74;          // +0x2c74
    char unknown_2c75[0x391e9 - 0x2c75];
    Class_00435100* net;               // +0x391e9
};
#pragma pack(pop)

struct Gadget_00495010 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Gadget_00495010*); // +0x8
    Game_00495010* owner;              // +0xc
};

extern Game_00495010* g_game;

void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_004ab060(Sub_00495010* sub, const char* name);
void __stdcall FUN_004a9660(Sub_00495010* sub);
void FUN_004c2470();
Gadget_00495010* __stdcall FUN_004aa8f0(Sub_00495010* sub, const char* name, int flags);
void __stdcall FUN_00494740(Gadget_00495010* gadget);
int FUN_00457a50();
void __stdcall FUN_004a0570(Sub_00495010* sub, char* name, int value);
void __stdcall FUN_0049fa50(Sub_00495010* sub);
void __stdcall FUN_004a81e0(Sub_00495010* sub, int value);
void FUN_004c2870();

// FUNCTION: 0x495010
void FUN_00495010()
{
    FUN_0047f1a0("SmallButton", 0);
    unsigned short f = g_game->flags;
    if (f & 0xe0) {
        g_game->flags = f & 0xff1f;
        if (FUN_004ab060(&g_game->sub, "TABMENU.GUI"))
            FUN_004a9660(&g_game->sub);
        return;
    }
    g_game->flags = (f & 0xff3f) | 0x20;
    FUN_004c2470();
    Gadget_00495010* d = FUN_004aa8f0(&g_game->sub, "TABMENU.GUI", 0x800);
    d->owner = g_game;
    d->handler = FUN_00494740;

    int count = 0;
    Player_00495010* p = g_game->players;
    for (int i = 0; i < 10; i++, p++) {
        if (p->active != 0 && p->type == 1)
            continue;
        if (p->info->flag_9b_6)
            continue;
        count++;
    }
    int mode = g_game->net->FUN_00435100();
    if (mode == 3 && !g_game->players[g_game->localPlayer].info->flag_9b_6) {
        int v = count > 0;
        FUN_004a0570(&g_game->sub, "ALLIES", v);
        FUN_004a0570(&g_game->sub, "SHARE", v);
        int ctl = !(g_game->field_2c74 & 1) && FUN_00457a50();
        FUN_004a0570(&g_game->sub, "CONTROL", ctl);
    } else {
        FUN_004a0570(&g_game->sub, "ALLIES", 0);
        FUN_004a0570(&g_game->sub, "SHARE", 0);
        FUN_004a0570(&g_game->sub, "CONTROL", 0);
    }
    FUN_0049fa50(&g_game->sub);
    FUN_004a81e0(&g_game->sub, 0x40);
    FUN_004c2870();
}
