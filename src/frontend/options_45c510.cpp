// Decompiled by Opus. Names are provisional.
// When the game is in state 4, looks up the "TRACKTYPE" gadget in the menu
// and passes its value byte on to FUN_004ce7c0.

class Class_004ce7c0 {
public:
    void FUN_004ce7c0(int index, unsigned char value);
};

#pragma pack(push, 1)
struct Gadget_0045c510 {
    char unknown_0[0x137];
    unsigned char value;               // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Holder_0045c510 {
    int unknown_0;
    Gadget_0045c510* gadgets;          // +0x4
};

struct Game_0045c510 {
    char unknown_0[0x10];
    Class_004ce7c0* field_10;          // +0x10
    char unknown_14[0x531 - 0x14];
    Holder_0045c510* holder;           // +0x531
    char unknown_535[0x37f16 - 0x535];
    char state;                        // +0x37f16
};
#pragma pack(pop)

extern Game_0045c510* g_game;
extern int DAT_00512fe0;

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);

// FUNCTION: 0x45c510
void FUN_0045c510()
{
    Gadget_0045c510* gadgets = g_game->holder->gadgets;
    if (g_game->state == 4) {
        int index = FUN_0049fdf0(gadgets, "TRACKTYPE", 1);
        g_game->field_10->FUN_004ce7c0(DAT_00512fe0, gadgets[index].value);
    }
}
