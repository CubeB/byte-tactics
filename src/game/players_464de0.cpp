// Decompiled by Opus. Names are provisional.

struct Flags_00464de0 {
    unsigned short unknown_bits0 : 2;
    unsigned short flag2 : 1;
    unsigned short unknown_bit3 : 1;
    unsigned short flag4 : 1;
    unsigned short unknown_rest : 11;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x3923b];
    Flags_00464de0 flags;              // +0x3923b
};
#pragma pack(pop)

extern Game* g_game;

struct Screen_00464de0 {
    char unknown_0[4];
    int field_4;                       // +0x4
};

struct Gadget_00464de0 {
    char unknown_0[0x18];
    Screen_00464de0* screen;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int selected;                      // +0x60
};

void BroadcastPlayerInfo();
void __stdcall ReportGameEvent(int param_1);
void __stdcall FUN_0047f1a0(char* str, int flag);
int __stdcall FUN_004a0300(int param1, int param2, char* name);
void __stdcall FUN_004ab0a0(void* param_1);

// FUNCTION: 0x464de0
void __stdcall FUN_00464de0(Gadget_00464de0* gadget)
{
    int screen = gadget->screen->field_4;
    if (gadget->selected == -1)
        return;
    FUN_0047f1a0("BigButton", 0);
    if (FUN_004a0300(screen, gadget->selected, "CHOICE1")) {
        BroadcastPlayerInfo();
        g_game->flags.flag4 = 0;
        ReportGameEvent(4);
        return;
    }
    if (!FUN_004a0300(screen, gadget->selected, "CHOICE2")) {
        FUN_004ab0a0(gadget);
        return;
    }
    g_game->flags.flag2 = 1;
    g_game->flags.flag4 = 0;
}
