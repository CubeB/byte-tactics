// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Gadget_00460800 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

extern int DAT_00512ff8;

void __stdcall PlaySoundByName(char* str, int flag);
int __stdcall IsCurrentGadgetNamed(Gadget_00460800* gadget, char* name);
void __stdcall CloseTopScreen(Gadget_00460800* gadget);
void FUN_00460680();
void FUN_004604a0();
void __stdcall FUN_004ab0a0(Gadget_00460800* gadget);

// FUNCTION: 0x460800
void __stdcall FUN_00460800(Gadget_00460800* gadget)
{
    if (gadget->field_60 != -1) {
        PlaySoundByName("Options", 0);
        if (IsCurrentGadgetNamed(gadget, "MAINMENU")) {
            DAT_00512ff8 = 0;
            CloseTopScreen(gadget);
            FUN_00460680();
            return;
        }
        if (IsCurrentGadgetNamed(gadget, "EXITGAME")) {
            DAT_00512ff8 = 2;
            CloseTopScreen(gadget);
            FUN_00460680();
            return;
        }
        if (!IsCurrentGadgetNamed(gadget, "CANCEL")) {
            if (IsCurrentGadgetNamed(gadget, "RESTART")) {
                CloseTopScreen(gadget);
                FUN_004604a0();
                return;
            }
            if (gadget->field_60 != -1)
                FUN_004ab0a0(gadget);
        }
    }
}
