// Decompiled by space-bunny-free. Names are provisional.
// Handler of the options dialog (OPTIONS.GUI): "RESTART" restarts the game
// after checking that the right CD is in the drive, "Difficulty" and "CANCEL"
// just close the dialog.

class Class_00435100 {
public:
    int FUN_00435100();
};

struct Gadget_00460340 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game_00460340 {
    char unknown_0[0x519];
    char message[0x37eee - 0x519];     // +0x519
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x391e9 - 0x37ef2];
    Class_00435100* mode;              // +0x391e9
    char unknown_391ed[0x39249 - 0x391ed];
    int field_39249;                   // +0x39249
};
#pragma pack(pop)

extern Game_00460340* g_game;

int __stdcall FUN_0049fd60(Gadget_00460340* gadget, char* name);
void __stdcall FUN_0047f1a0(char* str, int flag);
int __stdcall FUN_004a0f60(Gadget_00460340* gadget, char* name);
char __stdcall FUN_0041d6a0(int disc);
void __stdcall FUN_0041d4c0();
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004ab0a0(void* gadget);

// FUNCTION: 0x460340
void __stdcall FUN_00460340(Gadget_00460340* gadget)
{
    if (gadget->field_60 == -1)
        return;
    FUN_0047f1a0("Options", 0);
    if (FUN_0049fd60(gadget, "RESTART")) {
        int ok = 0;
        int mode = g_game->mode->FUN_00435100();
        switch (mode) {
        case 1:
            if (!FUN_0041d6a0(0)) {
                FUN_004abd90(g_game->message,
                             FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"),
                             200, 1, 1);
                FUN_004ab0a0(g_game->message);
                return;
            }
            ok = true;
            break;
        case 2:
            if (!FUN_0041d6a0(1)) {
                FUN_004abd90(g_game->message,
                             FUN_004c5740("Please insert the Multiplayer CD (Disc 1) and try again"),
                             200, 1, 1);
                FUN_004ab0a0(g_game->message);
                return;
            }
            ok = true;
            break;
        }
        if (!ok)
            return;
        FUN_0041d4c0();
        g_game->difficulty = FUN_004a0f60(gadget, "Difficulty");
        g_game->field_39249 = 1;
    } else if (FUN_0049fd60(gadget, "Difficulty")) {
        FUN_0047f1a0("Options", 0);
        FUN_004ab0a0(gadget);
    } else if (!FUN_0049fd60(gadget, "CANCEL") && gadget->field_60 != -1) {
        FUN_004ab0a0(gadget);
    }
}
