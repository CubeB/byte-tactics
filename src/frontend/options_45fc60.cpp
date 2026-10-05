// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Options-panel gadget handler. The gadget's +0x60 field is -1 when the panel
// is closed; that path, and the shared tail every handled button falls into,
// tears the flip surfaces down. Each named button loads the saved settings and
// calls its page's setup routine; PREV/CANCEL reload the saved settings inline
// (as 0x45cc50 does) and mark DAT_00506788 = 1, the rest mark it 0.
// Two constructs decide the code. The +0x2a44 bit-2 test must read as
// `if (bit2) {} else { ... }`: the standalone positive form makes MSVC emit
// the original's `mov cl,[eax+0x2a44]; shr cl,2; test cl,1` instead of
// `test byte ptr [eax+0x2a44],4`. And the field_60 == -1 path must be a
// `goto cleanup;` whose label sits after the DAT_00506788 = 0 store, so MSVC
// keeps the tail shared (jmp 0x45ff4a) instead of duplicating the whole
// teardown into the SPEEDS arm.
class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

class Class_004ce3e0 {
public:
    void FUN_004ce3e0(const void* src);
};

class Class_004ce580 {
public:
    void FUN_004ce580(int value);
};

class Class_004ce7a0 {
public:
    int FUN_004ce7a0(int value);
};

class Class_004d0070 {
public:
    void FUN_004d0070(int level);
};

class Class_004d00d0 {
public:
    void FUN_004d00d0(int level, int flag);
};

struct Gadget_0045fc60 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* field_10;                    // +0x10
    char unknown_14[0x519 - 0x14];
    char menu_519[1];                  // +0x519
    char unknown_51a[0x2a44 - 0x51a];
    unsigned short bit0 : 1;           // +0x2a44, bit 0
    unsigned short bit1 : 1;           // +0x2a44, bit 1 (mask 2)
    unsigned short bit2 : 1;           // +0x2a44, bit 2 (mask 4)
    unsigned short rest : 13;
    char unknown_2a46[0x1434d - 0x2a46];
    unsigned char field_1434d;         // +0x1434d
    char unknown_1434e[0x37efa - 0x1434e];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x37f08 - 0x37efe];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    unsigned short flags;              // +0x37f14
    unsigned char field_37f16;         // +0x37f16
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

extern Game* g_game;
extern int DAT_00512f2c;
extern int DAT_00512f42;
extern unsigned short DAT_00512f46;
extern char DAT_00512f48;
extern char DAT_00512f49;
extern char DAT_00512f4a;
extern int DAT_00512f55;
extern int DAT_00512f59;
extern int DAT_00512f6d;
extern char DAT_00512f71;
extern char DAT_00512f75[];
extern int DAT_00512fd9;
extern void* DAT_00512fe4;
extern void* DAT_00512fe8;
extern void* DAT_00512ff4;
extern int DAT_00506788;

void __stdcall SetGadgetStatus(Gadget_0045fc60* gadget, int id, int flag);
void __stdcall RenderLayer(char* menu, int value);
void __stdcall FUN_0049fa90(char* menu);
void __stdcall FUN_004ab170(char* menu, int a, int b);
void __stdcall SetOffscreenSurface(int value);
void FlipScreen();
int __stdcall IsCurrentGadgetNamed(Gadget_0045fc60* gadget, char* name);
void __stdcall FUN_0047f1a0(char* str, int flag);
void FUN_0045ed50();
void __stdcall FUN_0045e5e0(int flag);
void FUN_0045d7c0();
void SaveSettings();
void __stdcall FUN_0045c820();
void FUN_0045cae0();
void FUN_0045de30();
void __stdcall FUN_004ab0a0(Gadget_0045fc60* gadget);
void __stdcall SetBrightness(float value);
void __stdcall DrawSurface(int a, void* surface, int b, int c);
void __stdcall FreeSurface(void* surface);

// FUNCTION: 0x45fc60
void __stdcall FUN_0045fc60(Gadget_0045fc60* gadget)
{
    if (gadget->field_60 == -1)
        goto cleanup;
    {
        SetGadgetStatus(gadget, gadget->field_60, 1);
        if (g_game->bit2) {
        } else {
            RenderLayer(g_game->menu_519, 0x40);
            FUN_0049fa90(g_game->menu_519);
            FUN_004ab170(g_game->menu_519, 0, 0);
            SetOffscreenSurface(*(int*)((char*)g_game + 0x37e1b));
            FlipScreen();
        }
        if (IsCurrentGadgetNamed(gadget, "SPEEDS")) {
            FUN_0047f1a0("Options", 0);
            FUN_0045ed50();
        } else if (IsCurrentGadgetNamed(gadget, "VISUALS")) {
            FUN_0047f1a0("Options", 0);
            FUN_0045e5e0(0);
        } else if (IsCurrentGadgetNamed(gadget, "MUSIC")) {
            FUN_0047f1a0("Options", 0);
            FUN_0045d7c0();
        } else if (IsCurrentGadgetNamed(gadget, "PREV")) {
            FUN_0047f1a0("Options", 0);
            SaveSettings();
            DAT_00506788 = 1;
            return;
        } else if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
            FUN_0047f1a0("Previous", 0);
            FUN_0045c820();
            g_game->volume2 = DAT_00512f42;
            ((Class_004ce3e0*)g_game->field_10)->FUN_004ce3e0(&DAT_00512f75);
            g_game->field_37f16 = DAT_00512f48;
            ((Class_004ce7a0*)g_game->field_10)->FUN_004ce7a0(g_game->field_37f16);
            if (((unsigned char)g_game->flags ^ (unsigned char)DAT_00512f46) & 1) {
                ((Class_004cdb40*)g_game->field_10)->FUN_004cdb40();
            }
            unsigned short f = g_game->flags;
            g_game->flags = f ^ ((f ^ DAT_00512f46) & 1);
            ((Class_004ce580*)g_game->field_10)->FUN_004ce580(DAT_00512fd9);
            SetBrightness(0.5 - g_game->brightness * -0.041666668f);
            ((Class_004d0070*)g_game->field_10)->FUN_004d0070(g_game->volume1 << 10);
            ((Class_004d00d0*)g_game->field_10)->FUN_004d00d0(g_game->volume2 << 10, 0);
            g_game->field_37f23 = DAT_00512f55;
            g_game->field_38a4b = DAT_00512f6d;
            g_game->field_38a4d = DAT_00512f6d;
            g_game->field_1434d = DAT_00512f71;
            g_game->field_37efa = DAT_00512f2c;
            g_game->field_37f17 = DAT_00512f49;
            g_game->field_37f18 = DAT_00512f4a;
            g_game->field_37f27 = DAT_00512f59;
            FUN_0045cae0();
            DAT_00506788 = 1;
            return;
        } else if (IsCurrentGadgetNamed(gadget, "SOUND")) {
            FUN_0047f1a0("Options", 0);
            FUN_0045de30();
        } else {
            if (gadget->field_60 != -1)
                FUN_004ab0a0(gadget);
            return;
        }
        DAT_00506788 = 0;
cleanup:
        if (DAT_00512ff4) {
            DrawSurface(0, DAT_00512ff4, 0, 0);
            FreeSurface(DAT_00512ff4);
            DAT_00512ff4 = 0;
        }
        if (DAT_00512fe8) {
            FreeSurface(DAT_00512fe8);
            DAT_00512fe8 = 0;
        }
        DAT_00512fe4 = 0;
        return;
    }
}
