// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Screen resolution selection: builds the display mode list, then advances the
// local player's mode to the next (or previous) entry.

#pragma pack(push, 1)
struct Mode_00446310 {
    int width;                         // +0x0
    int height;                        // +0x4
    int field_8;                       // +0x8
};

struct Class_00446310 {
    int count;                         // +0x0
    Mode_00446310* modes;              // +0x4
    char unknown_8[0x14 - 0x8];
    char* available;                   // +0x14
    char unknown_18[0x20 - 0x18];
};

struct PlayerData_00446310 {
    char unknown_0[0x8b];
    unsigned short field_8b;           // +0x8b
    unsigned short field_8d;           // +0x8d
};

struct Player_00446310 {
    char unknown_0[0x27];
    PlayerData_00446310* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Holder_00446310 {
    char unknown_0[0x37];
    int field_37;                      // +0x37
};

struct Game_00446310 {
    char unknown_0[0x531];
    Holder_00446310* holder;           // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_00446310 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37f1b - 0x2a43];
    int field_37f1b;                   // +0x37f1b
    int field_37f1f;                   // +0x37f1f
};
#pragma pack(pop)

extern Game_00446310* g_game;

void* FUN_004d83b0(const char* name, unsigned int size);
void FUN_004d85a0(void* p);
int __stdcall FUN_004b5370(Class_00446310* obj);
void __stdcall FUN_0045e4c0(Class_00446310* obj);
void FUN_00450f90(void);

// FUNCTION: 0x446310
void FUN_00446310(void)
{
    Class_00446310* obj = (Class_00446310*)FUN_004d83b0("SELECT VIDEO MODE", 0x20);
    obj->available = 0;
    obj->modes = (Mode_00446310*)FUN_004d83b0("DISPLAY MODES", 0x4b0);

    if (FUN_004b5370(obj) != 0) {
        FUN_0045e4c0(obj);
        obj->available = (char*)FUN_004d83b0("AVAILABLE MODES", obj->count << 8);
        obj->available[0] = 0;

        Player_00446310* player = &g_game->players[g_game->localPlayer];
        int count = obj->count;
        for (int i = 0; i < count; i++) {
            if (obj->modes[i].width == player->data->field_8b
                && obj->modes[i].height == player->data->field_8d) {
                if (g_game->holder->field_37 == 2) {
                    i--;
                    if (i < 0)
                        i = count - 1;
                } else {
                    i++;
                    if (i >= count)
                        i = 0;
                }
                Mode_00446310& mode = obj->modes[i];
                player->data->field_8b = (unsigned short)mode.width;
                player->data->field_8d = (unsigned short)mode.height;
                FUN_00450f90();
                g_game->field_37f1b = mode.width;
                g_game->field_37f1f = mode.height;
                break;
            }
        }
    }
    FUN_004d85a0(obj->modes);
    FUN_004d85a0(obj);
}
