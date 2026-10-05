// Decompiled by space-bunny-free. Names are provisional.
//
// Handler of a gadget button (the keyboard/controls options window). "LEFTCLICK"
// and "UNITCHAT" are the two key bindings the gadget itself owns, "UNDO" loads
// the saved settings and "RESTORE" puts the defaults back; anything else is
// only accepted when its option record is enabled (first byte 1), in which case
// the option is re-selected and the link's own reselect callback runs.
//
// The index is deliberately written as `gadget->field_60 * 347` rather than
// reusing the local `i`. Both are the same value, but the second read is one
// more node in the expression graph, and that is what makes MSVC 5 emit the
// original's 7-byte `lea eax, [edi*8 + 0]` (0x45ed07) instead of
// `mov eax, edi / shl eax, 3`. The same shape, and the same fix, appear in
// 0x45e100 (matched), whose identical 0x15b-entry chain also starts with a
// padded `lea ecx, [eax*8 + 0]`.
//
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char window[0x1434d - 0x519];     // +0x519
    unsigned char field_1434d;         // +0x1434d
    char unknown_1434e[0x37ebe - 0x1434e];
    unsigned short flags;              // +0x37ebe
    char unknown_37ec0[0x37efa - 0x37ec0];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x37f17 - 0x37efe];
    unsigned char field_37f17;         // +0x37f17
    unsigned char field_37f18;         // +0x37f18
    char unknown_37f19[0x37f23 - 0x37f19];
    int field_37f23;                   // +0x37f23
    int field_37f27;                   // +0x37f27
    char unknown_37f2b[0x38a4b - 0x37f2b];
    short field_38a4b;                 // +0x38a4b
    short field_38a4d;                 // +0x38a4d
};
#pragma pack(pop)

struct Link_0045ead0 {
    void* obj;                                 // +0x0
    char* data;                                // +0x4
    void (__stdcall* reselect)(void* gadget);  // +0x8
};

struct Gadget_0045ead0 {
    char unknown_0[0x18];
    Link_0045ead0* link;                // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

extern Game* g_game;
extern int DAT_00512f2c;
extern unsigned char DAT_00512f49;
extern unsigned char DAT_00512f4a;
extern int DAT_00512f55;
extern int DAT_00512f59;
extern unsigned short DAT_00512f6d;
extern unsigned char DAT_00512f71;

int __stdcall IsCurrentGadgetNamed(Gadget_0045ead0* gadget, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
int __stdcall GetButtonStageByName(Gadget_0045ead0* gadget, char* name);
void __stdcall CloseTopScreen(Gadget_0045ead0* gadget);
void __stdcall FUN_004ab0a0(Gadget_0045ead0* gadget);
void OpenSpeedOptions();

// FUNCTION: 0x45ead0
void __stdcall FUN_0045ead0(Gadget_0045ead0* gadget)
{
    char* data = gadget->link->data;
    if (gadget->field_60 == -1) {
        g_game->flags &= 0xfffe;
        return;
    }
    if (IsCurrentGadgetNamed((Gadget_0045ead0*)&g_game->window, "LEFTCLICK")) {
        PlaySoundByName("Options", 0);
        g_game->field_37efa = GetButtonStageByName((Gadget_0045ead0*)&g_game->window, "LEFTCLICK");
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed((Gadget_0045ead0*)&g_game->window, "UNITCHAT")) {
        PlaySoundByName("Options", 0);
        FUN_004ab0a0(gadget);
        g_game->field_37f18 = (unsigned char)(GetButtonStageByName((Gadget_0045ead0*)&g_game->window, "UNITCHAT") * 5);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "UNDO")) {
        PlaySoundByName("Options", 0);
        g_game->field_37f23 = DAT_00512f55;
        g_game->field_38a4b = DAT_00512f6d;
        g_game->field_38a4d = DAT_00512f6d;
        g_game->field_1434d = DAT_00512f71;
        g_game->field_37efa = DAT_00512f2c;
        g_game->field_37f17 = DAT_00512f49;
        g_game->field_37f18 = DAT_00512f4a;
        g_game->field_37f27 = DAT_00512f59;
        CloseTopScreen(gadget);
        OpenSpeedOptions();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "RESTORE")) {
        PlaySoundByName("Options", 0);
        g_game->field_37f23 = 10;
        g_game->field_37f27 = 10;
        g_game->field_38a4b = 10;
        g_game->field_38a4d = 10;
        g_game->field_1434d = 0x20;
        g_game->field_37efa = 0;
        g_game->field_37f17 = 10;
        g_game->field_37f18 = 5;
        CloseTopScreen(gadget);
        OpenSpeedOptions();
        return;
    }
    int i = gadget->field_60;
    if (data[gadget->field_60 * 347] != 1) {
        FUN_004ab0a0(gadget);
        return;
    }
    if (i != -1) {
        Link_0045ead0* link = gadget->link;
        void* obj = link->obj;
        CloseTopScreen(gadget);
        gadget->field_60 = i;
        ((Link_0045ead0*)obj)->reselect(gadget);
    }
}
