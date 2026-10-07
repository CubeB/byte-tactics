// Decompiled by space-bunny-free. Names are provisional.
// Builds the sound options page: opens the SOUNDS / SOUNDSRT.GUI layout,
// adds the SOUND group and an FXVOL slider seeded from the saved effects
// volume, runs every type 4 entry, then writes the speech and mode gadgets
// back out from the sound flag word at g_game+0x37f19.

#pragma pack(push, 1)
struct Entry_0045de30 {                  // 0x15b bytes
    unsigned char type;                  // +0x00
    char unknown_1[0xb6 - 1];
    short count;                         // +0xb6 (entry 0 only)
    char unknown_b8[0x136 - 0xb8];
    short steps;                         // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                             // +0x13c
    short pos;                           // +0x140
    char unknown_142[2];
    void (__stdcall *fn)(void* obj, int arg);   // +0x144
    char unknown_148[0x15b - 0x148];
};

struct Object_0045de30 {
    char unknown_0[4];
    Entry_0045de30* entries;             // +0x04
    void (__stdcall *fn)(void* obj, int arg);   // +0x08
};

struct Menu_0045de30 {
    char unknown_0[0x18];
    void* holder;                        // +0x18
};

struct Bits_0045de30 {
    unsigned short mode : 3;           // bits 0-2
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short b5 : 1;
    unsigned short speech : 1;         // bit 6
    unsigned short b7 : 1;
};

union Flags_0045de30 {
    unsigned short word;
    Bits_0045de30 bits;
};

struct Game {
    char unknown_0[0x519];
    Menu_0045de30 menu;                  // +0x519
    char unknown_535[0x37ebe - 0x535];
    unsigned char flags_37ebe;           // +0x37ebe
    char unknown_37ebf[0x37f0c - 0x37ebf];
    short volume1;                       // +0x37f0c
    char unknown_37f0e[0x37f17 - 0x37f0e];
    unsigned char field_37f17;           // +0x37f17
    char unknown_37f18;
    // Read through an unaligned unsigned short bitfield.
    Flags_0045de30 flags;              // +0x37f19
};
#pragma pack(pop)

extern Game* g_game;

Object_0045de30* __cdecl OpenOptionsLayout();
void FUN_0045ce80();
void __stdcall RenderLayer(Menu_0045de30* obj, int value);
int __stdcall LoadGuiLayer(Menu_0045de30* obj, char* name, int size);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall FUN_0049fa50(Menu_0045de30* obj);
void __stdcall SetGadgetStatusByName(Menu_0045de30* obj, char* name, int value);
int __stdcall FindGadgetIndex(Entry_0045de30* entries, char* name, int type);
Entry_0045de30* __stdcall FUN_004a0200(Entry_0045de30* entries, char* name);
void __stdcall SetButtonStageByName(Menu_0045de30* obj, char* name, int value);
void __stdcall FUN_004a0570(Menu_0045de30* obj, char* name, int value);
void __stdcall FUN_004a1450(Menu_0045de30* obj, char* name, int value);
void __stdcall FUN_0049fa90(Menu_0045de30* obj);
void __stdcall FUN_0049fb10(Menu_0045de30* obj, int value);
void FUN_00428b60();

void __stdcall HandleSoundOptionsClick(void* obj, int arg);
void __stdcall HandleEffectsVolumeSlider(void* obj, int arg);

// FUNCTION: 0x45de30
void OpenSoundOptions()
{
    Object_0045de30* obj = OpenOptionsLayout();
    RenderLayer(&g_game->menu, 2);
    FUN_0045ce80();
    if (g_game->flags_37ebe & 1) {
        LoadGuiLayer(&g_game->menu, "SOUNDSRT.GUI", 0x200);
    } else {
        LoadGuiLayer(&g_game->menu, "SOUNDS", 0x200);
        LoadPictureCached("optsound4x", 0, 0, 0);
    }
    Entry_0045de30* entries = obj->entries;
    obj->fn = HandleSoundOptionsClick;
    FUN_0049fa50(&g_game->menu);
    SetGadgetStatusByName(&g_game->menu, "SOUND", 1);
    // Result in a local; the call reads obj->entries while later calls use entries.
    int found = FindGadgetIndex(obj->entries, "FXVOL", 0xe);
    if (found != -1) {
        Entry_0045de30* e = FUN_004a0200(entries, "FXVOL");
        e->max = 0x40;
        e->fn = HandleEffectsVolumeSlider;
        e->pos = g_game->volume1;
        int value = e->pos;
        if (value > 0x40) {
            value = 0x40;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.015625f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
    }
    for (int i = 1; i <= obj->entries->count; i++) {
        if (obj->entries[i].type == 4)
            obj->entries[i].fn(&g_game->menu, 0);
    }
    SetButtonStageByName(&g_game->menu, "SPEECH", g_game->flags.bits.speech ? g_game->field_37f17 / 5 : 0);
    SetButtonStageByName(&g_game->menu, "MODE", g_game->flags.word & 7);
    FUN_004a0570(&g_game->menu, "VOLTEXT", (g_game->flags.word & 7) != 0);
    FUN_004a1450(&g_game->menu, "FXVOL", (g_game->flags.word & 7) == 0);
    FUN_004a1450(&g_game->menu, "TEST", (g_game->flags.word & 7) == 0);
    FUN_004a1450(&g_game->menu, "SPEECH", (g_game->flags.word & 7) == 0);
    FUN_0049fa90(&g_game->menu);
    FUN_0049fb10(&g_game->menu, 1);
    FUN_00428b60();
    RenderLayer(&g_game->menu, 0x40);
}
