// Decompiled by mimo-v2.6-flash. Names are provisional.

#pragma pack(push, 1)
struct Entry_4934b0 {
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0xba - 0x12];
    short selected;                    // +0xba
    char unknown_bc[0xd2 - 0xbc];
    void* field_d2;                    // +0xd2
};

struct Layer_4934b0 {
    int unknown_0;
    Entry_4934b0* entries;             // +0x04
};

struct Gadget_4934b0 {
    char unknown_0[0x18];
    Layer_4934b0* layer;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};

struct PlayerInfo_4934b0 {
    char unknown_0[0x9b];
    unsigned char flags;               // +0x9b
};

struct Player_4934b0 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_4934b0* info;           // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x140 - 0x74];
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_4934b0 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37ebe - 0x2a43];
    unsigned short flags;              // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e6d0[];

Entry_4934b0* __stdcall FUN_0049ff90(Entry_4934b0* entries, char* name);
int __stdcall FUN_0049fd60(Gadget_4934b0* obj, char* name);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_0049fa90(Gadget_4934b0* obj);
void __stdcall FUN_0047f1a0(char* name, int flag);
void __stdcall FUN_004ab0a0(Gadget_4934b0* obj);
Entry_4934b0* __stdcall FUN_004a0200(Entry_4934b0* entries, char* name);
int __stdcall FUN_0045ba20(Entry_4934b0* entry);
void __stdcall FUN_00464c60(unsigned char from, unsigned char to, float amount, int flag);
void __stdcall FUN_00464b30(unsigned char from, unsigned char to, float amount, int flag);
int __stdcall FUN_004a0f60(Gadget_4934b0* obj, char* name);
void __stdcall FUN_004933e0(unsigned char player);
unsigned char __stdcall FindSlotByDpid(int id);
void __stdcall ShareMapInfo(unsigned char from, unsigned char to);
void __stdcall SendShareMapInfo(unsigned char from, unsigned char to);

static inline int IsPlaying_4934b0(Player_4934b0* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted_4934b0(Player_4934b0* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

// FUNCTION: 0x4934b0
void __stdcall FUN_004934b0(Gadget_4934b0* obj)
{
    Entry_4934b0* data = obj->layer->entries;

    if (obj->current == -1) {
        Entry_4934b0* e = FUN_0049ff90(data, "PLYRLIST");
        FUN_004d85a0(e->field_d2);
        g_game->flags &= ~0x40;
        return;
    }
    if (FUN_0049fd60(obj, "MAPINFO")) {
        FUN_0049fa90(obj);
        FUN_0047f1a0("Options", 0);
        FUN_004ab0a0(obj);
        return;
    }
    if (FUN_0049fd60(obj, "SHARUNIT")) {
        FUN_0049fa90(obj);
        FUN_0047f1a0("Options", 0);
        FUN_004ab0a0(obj);
        return;
    }
    if (FUN_0049fd60(obj, "OK")) {
        FUN_0047f1a0("Options", 0);
        Entry_4934b0* plyr = FUN_0049ff90(data, "PLYRLIST");
        short idx = plyr->selected;
        if (idx < 0)
            return;
        int pi = FindSlotByDpid(DAT_0051e6d0[idx]);
        Player_4934b0* p = &g_game->players[pi];
        if (IsPlaying_4934b0(p) && !(p->info->flags & 0x40) && IsCounted_4934b0(p)) {
            FUN_00464c60(g_game->localPlayer, pi,
                         (float)FUN_0045ba20(FUN_004a0200(data, "METAL")), 1);
            FUN_00464b30(g_game->localPlayer, pi,
                         (float)FUN_0045ba20(FUN_004a0200(data, "ENERGY")), 1);
            if (FUN_004a0f60(obj, "SHARUNIT"))
                FUN_004933e0(pi);
            if (FUN_004a0f60(obj, "MAPINFO")) {
                ShareMapInfo(g_game->localPlayer, pi);
                SendShareMapInfo(g_game->localPlayer, pi);
            }
        }
        return;
    }
    if (FUN_0049fd60(obj, "CANCEL")) {
        FUN_0047f1a0("Previous", 0);
        return;
    }
    if (obj->current != -1)
        FUN_004ab0a0(obj);
}
