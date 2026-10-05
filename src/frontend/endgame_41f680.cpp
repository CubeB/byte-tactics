// Decompiled by Opus. Names are provisional.

struct Gadget_41f680 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char message[0x39057 - 0x519];     // +0x519
    int field_39057;                   // +0x39057
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall IsCurrentGadgetNamed(Gadget_41f680* gadget, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
char __stdcall FindGameCdDrive(int param_1);
char* __stdcall FUN_004c5740(char* text);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004ab0a0(Gadget_41f680* gadget);

// FUNCTION: 0x41f680
void __stdcall FUN_0041f680(Gadget_41f680* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            PlaySoundByName("Options", 0);
            if (FindGameCdDrive(0)) {
                g_game->field_39057 = 5;
                return;
            }
            OpenMessageBox(g_game->message,
                         FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"),
                         200, 1, 1);
        }
        FUN_004ab0a0(gadget);
    }
}
