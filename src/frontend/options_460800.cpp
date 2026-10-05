// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Gadget_00460800 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

extern int DAT_00512ff8;

void __stdcall FUN_0047f1a0(char* str, int flag);
int __stdcall FUN_0049fd60(Gadget_00460800* gadget, char* name);
void __stdcall FUN_004a9660(Gadget_00460800* gadget);
void FUN_00460680();
void FUN_004604a0();
void __stdcall FUN_004ab0a0(Gadget_00460800* gadget);

// FUNCTION: 0x460800
void __stdcall FUN_00460800(Gadget_00460800* gadget)
{
    if (gadget->field_60 != -1) {
        FUN_0047f1a0("Options", 0);
        if (FUN_0049fd60(gadget, "MAINMENU")) {
            DAT_00512ff8 = 0;
            FUN_004a9660(gadget);
            FUN_00460680();
            return;
        }
        if (FUN_0049fd60(gadget, "EXITGAME")) {
            DAT_00512ff8 = 2;
            FUN_004a9660(gadget);
            FUN_00460680();
            return;
        }
        if (!FUN_0049fd60(gadget, "CANCEL")) {
            if (FUN_0049fd60(gadget, "RESTART")) {
                FUN_004a9660(gadget);
                FUN_004604a0();
                return;
            }
            if (gadget->field_60 != -1)
                FUN_004ab0a0(gadget);
        }
    }
}
