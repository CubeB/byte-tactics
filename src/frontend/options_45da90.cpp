// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Sound-options menu handler: applies the chosen sound mode, handles the
// SPEECH toggle, UNDO, RESTORE and TEST buttons, and pushes the mode back
// into the menu gadgets.

#pragma pack(push, 1)
struct Entry_0045da90 {              // 0x15b-byte gadget entry
    unsigned char type;              // +0x00
    char unknown_1;
    char name[0x11];                 // +0x02
    short x;                         // +0x13
    short y;                         // +0x15
    short width;                     // +0x17
    short height;                    // +0x19
    char unknown_1b[0x137 - 0x1b];
    unsigned char value;             // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Vtable_0045da90 {
    char unknown_0[8];
    void (__stdcall* FUN_8)(void* obj);
};

struct Holder_0045da90 {
    Vtable_0045da90* field_0;        // +0x00
    Entry_0045da90* entries;         // +0x04
};

struct Object_0045da90 {
    char unknown_0[0x18];
    Holder_0045da90* holder;         // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};

struct Bits_0045da90 {
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short bit4 : 1;
    unsigned short bit5 : 1;
    unsigned short bit6 : 1;
    unsigned short bits_7_15 : 9;
};

union Flags_0045da90 {
    unsigned short word;
    Bits_0045da90 bits;
};

struct Game {
    char unknown_0[0x10];
    void* sound;                     // +0x10
    char unknown_14[0x519 - 0x14];
    char gui[1];                     // +0x519
    char unknown_51a[0x2a44 - 0x51a];
    unsigned short bit0 : 1;         // +0x2a44
    unsigned short bit1 : 1;
    unsigned short prefs : 1;        // bit 2
    unsigned short bits_3_15 : 13;
    char unknown_2a46[0x37ebe - 0x2a46];
    unsigned short loaded : 1;       // +0x37ebe
    unsigned short bits_37ebe_1 : 15;
    char unknown_37ec0[0x37f08 - 0x37ec0];
    int brightness;                  // +0x37f08
    int volume1;                     // +0x37f0c
    int volume2;                     // +0x37f10
    char unknown_37f14[0x37f17 - 0x37f14];
    unsigned char field_37f17;       // +0x37f17
    char unknown_37f18;
    Flags_0045da90 soundFlags;       // +0x37f19
};
#pragma pack(pop)

class Class_004cfe80 {
public:
    void Enable3D();
};

class Class_004cfe90 {
public:
    void Disable3D();
};

class Class_004d0070 {
public:
    void SetWaveVolume(int level);
};

class Class_004d00d0 {
public:
    void SetAuxVolume(int level, int flag);
};

extern Game* g_game;
extern char DAT_005069b8[];           // "SPEECH"
extern char DAT_005069d0[];           // "MODE"
extern char DAT_00506998[];           // "UNDO"
extern char DAT_00506990[];           // "RESTORE"
extern char DAT_005069c0[];           // "TEST"
extern char DAT_005069d8[];           // "sounds\\explode.wav"
extern char DAT_00502b38[];           // "Options"
extern char DAT_005031d4[];           // "BGM"
extern char DAT_005069c8[];           // "VOLTEXT"
extern char DAT_00506884[];           // "FXVOL"

void __stdcall FUN_0049fa90(void* obj);
int __stdcall IsCurrentGadgetNamed(void* obj, char* name);
void __stdcall PlaySoundByName(char* name, int value);
void __stdcall PlayLoopingSoundByName(char* name, int value);
void __stdcall PlaySoundFile(char* name);
void __stdcall StopAllSounds();
void __stdcall FUN_004ab0a0(void* obj);
int __stdcall GetButtonStageByName(void* obj, char* name);
int __stdcall SetButtonStageByName(void* obj, char* name, int value);
void __stdcall FUN_004a0570(void* obj, char* name, int value);
void __stdcall FUN_004a1450(void* obj, char* name, int value);
void __stdcall CloseTopScreen(void* obj);
void FUN_0045de30();
void FUN_0045c820();
void __stdcall SetBrightness(float value);

// FUNCTION: 0x45da90
void __stdcall FUN_0045da90(Object_0045da90* obj)
{
    Entry_0045da90* entries = obj->holder->entries;
    if (obj->field_60 == -1) {
        g_game->loaded = 0;
        return;
    }
    FUN_0049fa90(obj);
    int mode = obj->field_60;
    if (IsCurrentGadgetNamed(obj, DAT_005069b8)) {
        PlaySoundByName(DAT_00502b38, 0);
        g_game->soundFlags.bits.bit6 = entries[obj->field_60].value != 0;
        g_game->field_37f17 = entries[obj->field_60].value * 5;
        FUN_004ab0a0(obj);
    } else if (IsCurrentGadgetNamed(obj, DAT_005069d0)) {
        int v = GetButtonStageByName(obj, DAT_005069d0);
        unsigned short f = g_game->soundFlags.word;
        g_game->soundFlags.word = f ^ ((f ^ v) & 7);
        if ((g_game->soundFlags.word & 7) == 0)
            StopAllSounds();
        if ((g_game->soundFlags.word & 7) == 2)
            ((Class_004cfe80*)g_game->sound)->Enable3D();
        else
            ((Class_004cfe90*)g_game->sound)->Disable3D();
        if ((g_game->soundFlags.word & 7) == 1 && !g_game->prefs)
            PlayLoopingSoundByName(DAT_005031d4, 0);
        SetButtonStageByName(&g_game->gui, DAT_005069d0, g_game->soundFlags.word & 7);
        FUN_004a0570(&g_game->gui, DAT_005069c8, (g_game->soundFlags.word & 7) != 0);
        FUN_004a1450(&g_game->gui, DAT_00506884, (g_game->soundFlags.word & 7) == 0);
        FUN_004a1450(&g_game->gui, DAT_005069c0, (g_game->soundFlags.word & 7) == 0);
        FUN_004a1450(&g_game->gui, DAT_005069b8, (g_game->soundFlags.word & 7) == 0);
        FUN_004ab0a0(obj);
        PlaySoundByName(DAT_00502b38, 0);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_00506998)) {
        FUN_0045c820();
        CloseTopScreen(obj);
        FUN_0045de30();
        PlaySoundByName(DAT_00502b38, 0);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_00506990)) {
        g_game->volume1 = 0x1b;
        g_game->soundFlags.bits.bit4 = 1;
        g_game->soundFlags.bits.bit5 = 1;
        g_game->soundFlags.bits.bit6 = 1;
        ((Class_004cfe90*)g_game->sound)->Disable3D();
        g_game->soundFlags.word = (g_game->soundFlags.word & 0xfff9) | 1;
        g_game->field_37f17 = 10;
        SetBrightness(0.5 - g_game->brightness * -0.041666668f);
        ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
        ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
        CloseTopScreen(obj);
        FUN_0045de30();
        PlaySoundByName(DAT_00502b38, 0);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_005069c0)) {
        PlaySoundFile(DAT_005069d8);
        FUN_004ab0a0(obj);
        return;
    }
    if (obj->field_60 != -1) {
        if (entries[mode].type != 1) {
            FUN_004ab0a0(obj);
            return;
        }
        Vtable_0045da90* p = obj->holder->field_0;
        CloseTopScreen(obj);
        obj->field_60 = mode;
        p->FUN_8(obj);
    }
}
