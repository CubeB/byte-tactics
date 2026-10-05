// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Gadget_00494740 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2bee];
    unsigned short flags;              // +0x2bee
    char unknown_2bf0[0x37ebe - 0x2bf0];
    unsigned char field_37ebe;         // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0049fa70(void* menu);
int __stdcall IsCurrentGadgetNamed(Gadget_00494740* gadget, char* name);
void __stdcall PlaySoundByName(char* name, int param_2);
void FUN_00460cc0();
void OpenShareDialog();
void FUN_004466b0();
void FUN_004478b0();
void __stdcall CloseTopScreen(Gadget_00494740* gadget);
void __stdcall FUN_004ab0a0(Gadget_00494740* gadget);

// FUNCTION: 0x494740
void __stdcall HandleTabMenuEvent(Gadget_00494740* gadget)
{
    if (gadget->field_60 == -1) {
        g_game->flags &= 0xff1f;
        FUN_0049fa70(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OPTIONS")) {
        g_game->field_37ebe |= 1;
        PlaySoundByName("BigButton", 0);
        FUN_00460cc0();
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SHARE")) {
        PlaySoundByName("BigButton", 0);
        OpenShareDialog();
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CONTROL")) {
        PlaySoundByName("BigButton", 0);
        FUN_004466b0();
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "ALLIES")) {
        PlaySoundByName("BigButton", 0);
        FUN_004478b0();
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
        CloseTopScreen(gadget);
        return;
    }
    FUN_004ab0a0(gadget);
}
