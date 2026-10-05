// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Handler of the Options screen. "ANTI", "BSHADOWS" and "SHADING" copy the
// gadget's value into the matching bit of the flags word at g_game+0x37f06
// (BSHADOWS also cascades the bit down to bits 2 and 3); "UNDO" and "RESTORE"
// reload the saved settings; "OK" closes the screen; any other selection on a
// valid entry (state 1) closes the screen and runs the next screen's handler.
// The selection -1 branch frees the screen's object and clears flag bit 0.

class CMemoryCache {
public:
    void FlushCache();
};

class Class_004d0070 {
public:
    void SetWaveVolume(int level);
};

class Class_004d00d0 {
public:
    void SetAuxVolume(int level, int flag);
};

struct Gui_0045e100;

typedef int (__stdcall* Handler_0045e100)(Gui_0045e100*);

// One entry of the gadget table, 0x15b bytes each; entry 0 is the header.
#pragma pack(push, 1)
struct Entry_0045e100 {
    char state;                        // +0x0
    char unknown_1[1];
    char name[0x10];                   // +0x2
    char unknown_12[0x15b - 0x12];
};
#pragma pack(pop)

struct Screen_0045e100 {
    Screen_0045e100* next;             // +0x0
    Entry_0045e100* entries;           // +0x4
    Handler_0045e100 handler;          // +0x8
    void* field_c;                     // +0xc
    unsigned int flags;                // +0x10
    int active;                        // +0x14
};

struct FreeObj_0045e100 {
    char unknown_0[4];
    void* field_4;                     // +0x4
    char unknown_8[0x14 - 0x8];
    void* field_14;                    // +0x14
};

struct Gui_0045e100 {
    char unknown_0[0x18];
    Screen_0045e100* top;              // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Flags_37ebe {
    unsigned short b0 : 1;             // +0x37ebe
    unsigned short rest : 15;
};

struct Flags_37f06 {
    unsigned short b0 : 1;             // +0x37f06
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short b5 : 1;
    unsigned short b6 : 1;
    unsigned short rest : 9;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned short pad_2a44 : 2;       // +0x2a44
    unsigned short flag_2a44 : 1;
    unsigned short rest_2a44 : 13;
    char unknown_2a46[0x1437b - 0x2a46];
    CMemoryCache* ptr_1437b;           // +0x1437b
    char unknown_1437f[0x37ebe - 0x1437f];
    Flags_37ebe flags_37ebe;           // +0x37ebe
    char unknown_37ec0[0x37f06 - 0x37ec0];
    Flags_37f06 flags_37f06;           // +0x37f06
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    char unknown_37f14[0x37f1b - 0x37f14];
    int width;                         // +0x37f1b
    int height;                        // +0x37f1f
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d85a0(void* p);
void FUN_0045cae0();
void __stdcall OpenVisualOptions(int param_1);
int __stdcall IsCurrentGadgetNamed(Gui_0045e100* gui, char* name);
void __stdcall PlaySoundByName(char* name, int param_2);
int __stdcall GetButtonStageByName(Gui_0045e100* gui, char* name);
void __stdcall FUN_0049fa90(Gui_0045e100* gui);
void __stdcall FUN_004ab0a0(Gui_0045e100* gui);
int __stdcall IsScreenNamed(Gui_0045e100* gui, const char* name);
void __stdcall CloseTopScreen(Gui_0045e100* gui);
void __stdcall SetBrightness(float value);

// FUNCTION: 0x45e100
void __stdcall HandleVisualOptionsClick(Gui_0045e100* gui)
{
    int save = gui->field_60;
    Screen_0045e100* top = gui->top;
    Entry_0045e100* entries = top->entries;
    FreeObj_0045e100* obj = (FreeObj_0045e100*)top->field_c;

    if (gui->field_60 == -1) {
        if (obj) {
            if (!g_game->flags_37ebe.b0) {
                FUN_004d85a0(obj->field_14);
                FUN_004d85a0(obj->field_4);
            }
            FUN_004d85a0(obj);
            gui->top->field_c = 0;
        }
        g_game->flags_37ebe.b0 = 0;
        return;
    }

    if (IsCurrentGadgetNamed(gui, "ANTI")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.b1 = GetButtonStageByName(gui, "ANTI") & 1;
        if (g_game->flags_37ebe.b0)
            g_game->ptr_1437b->FlushCache();
        FUN_0049fa90(gui);
        FUN_004ab0a0(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "BSHADOWS")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.b4 = GetButtonStageByName(gui, "BSHADOWS") & 1;
        g_game->flags_37f06.b3 = g_game->flags_37f06.b4;
        g_game->flags_37f06.b2 = g_game->flags_37f06.b3;
        if (g_game->flags_37ebe.b0)
            g_game->ptr_1437b->FlushCache();
        FUN_0049fa90(gui);
        FUN_004ab0a0(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "SHADING")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.b5 = GetButtonStageByName(gui, "SHADING") & 1;
        if (g_game->flags_37ebe.b0)
            g_game->ptr_1437b->FlushCache();
        FUN_0049fa90(gui);
        FUN_004ab0a0(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "UNDO")) {
        PlaySoundByName("Options", 0);
        FUN_0045cae0();
        CloseTopScreen(gui);
        OpenVisualOptions(0);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "RESTORE")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.b1 = 1;
        g_game->flags_37f06.b2 = 1;
        g_game->flags_37f06.b3 = 1;
        g_game->flags_37f06.b4 = 1;
        g_game->flags_37f06.b5 = 1;
        g_game->brightness = 12;
        if (!g_game->flag_2a44) {
            g_game->width = 640;
            g_game->height = 480;
            g_game->flags_37f06.b6 = 0;
        }
        SetBrightness(0.5 - g_game->brightness * -0.041666668f);
        ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
        ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
        CloseTopScreen(gui);
        OpenVisualOptions(0);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "OK") && IsScreenNamed(gui, "selvmode.gui")) {
        PlaySoundByName("Options", 0);
        return;
    }

    if (gui->field_60 != -1) {
        if (entries[gui->field_60].state != 1) {
            FUN_004ab0a0(gui);
            return;
        }
        Screen_0045e100* next = gui->top->next;
        CloseTopScreen(gui);
        gui->field_60 = save;
        next->handler(gui);
    }
}
