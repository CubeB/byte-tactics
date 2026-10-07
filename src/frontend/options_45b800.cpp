// Decompiled by Haiku, Opus, DeepSeek V4.1 Flash, Sonnet, space-bunny-free, Space Bunny Free and deepseek-v4.1-flash. Names are provisional.
//
// The options dialogs of PREFS.GUI: the direct-connect address and flag
// setters, the menu-entry helpers, the slider readers and the sound, video
// and game-setting handlers (0x45b800 to 0x45ce80).
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

struct ModeList_0045b800;

// The 0x15b-byte menu control record: entry 0 holds the count at +0xb6 and
// the gadget's own fields from +0xbc on, the other entries hold NUL
// terminated text there, and a slider keeps its step count, its maximum and
// its position at +0x136.
struct Entry_0045b800 {
    unsigned char type;                // +0x00
    char unknown_1;
    char name[0x11];                   // +0x02
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    char unknown_1b[0x29 - 0x1b];
    unsigned char field_29;            // +0x29
    char unknown_2a[0xb6 - 0x2a];
    union {
        struct {                       // entry 0: the gadget's own fields
            short count;               // +0xb6
            char unknown_b8[0xc4 - 0xb8];
            int field_c4;              // +0xc4
            char unknown_c8[0x136 - 0xc8];
            union {
                short steps;           // +0x136
                struct {
                    unsigned char unknown_136;
                    unsigned char value; // +0x137
                };
            };
            char unknown_138[0x13c - 0x138];
            int max;                   // +0x13c
            short pos;                 // +0x140
            char unknown_142[0x14a - 0x142];
            ModeList_0045b800* list;   // +0x14a
            char unknown_14e[0x15b - 0x14e];
        };
        char text[0x80];               // +0xb6, the other entries' text
    };
};

// The entry table a menu's +0x18 points at.
struct Holder_0045b800 {
    int unknown_0;                      // +0x00
    Entry_0045b800* entries;            // +0x04
};

// The menu object at g_game+0x519.
struct Menu_0045b800 {
    char unknown_0[0x18];
    Holder_0045b800* holder;            // +0x18
};

// One entry of a video mode list at an entry's +0x14a.
struct Mode_0045b800 {
    int width;                          // +0x0
    int height;                         // +0x4
    char unknown_8[4];
};

struct ModeList_0045b800 {
    int count;                          // +0x0
    Mode_0045b800* modes;               // +0x4
};

struct PlayerInfo_45c070 {
    char unknown_0[0x9b];
    unsigned short unknown_9b_0 : 6;  // +0x9b, bits 0 to 5
    unsigned short flag_9b_6 : 1;     // +0x9b, bit 6 (mask 0x40)
    unsigned short unknown_9b_7 : 9;
};

struct Player_45c070 {
    int field_0;                      // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_45c070* info;          // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x519 - 0x14];
    Menu_0045b800 menu;                // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_45c070 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;        // +0x2a42
    char unknown_2a43;
    union {
        unsigned char field_2a44;      // +0x2a44
        struct {
            unsigned short pad_2a44 : 2;
            unsigned short flag_2a44 : 1;
            unsigned short rest_2a44 : 13;
        };
    };
    char unknown_2a46[0x14281 - 0x2a46];
    struct {
        unsigned short bit0 : 1;       // +0x14281
        unsigned short bit1 : 1;
        unsigned short bit2 : 1;
        unsigned short rest : 13;
    } flags14281;
    char unknown_14283[0x1434d - 0x14283];
    unsigned char field_1434d;         // +0x1434d
    char unknown_1434e[0x37ebe - 0x1434e];
    unsigned char field_37ebe;         // +0x37ebe
    char unknown_37ebf[0x37ee6 - 0x37ebf];
    // The 0x53-byte image SaveGameSettings copies is the same bytes as the
    // settings fields that follow.
    union {
        char block[0x53];              // +0x37ee6
        struct {
            char unknown_37ee6[0x37efa - 0x37ee6];
            int field_37efa;           // +0x37efa
            char unknown_37efe[0x37f06 - 0x37efe];
            union {
                unsigned short flags;  // +0x37f06
                struct {
                    unsigned short bit0 : 1;
                    unsigned short bit1 : 1;
                    unsigned short bit2 : 1;
                    unsigned short bit3 : 1;
                    unsigned short bit4 : 1;
                    unsigned short bit5 : 1;
                    unsigned short bit6 : 1;
                    unsigned short rest : 9;
                };
            };
            int brightness;            // +0x37f08
            int volume1;               // +0x37f0c
            int volume2;               // +0x37f10
            union {
                unsigned short flags14; // +0x37f14
                struct {
                    char f_37f14;      // +0x37f14
                    char unknown_37f15;
                };
            };
            unsigned char field_37f16; // +0x37f16
            unsigned char field_37f17; // +0x37f17
            unsigned char field_37f18; // +0x37f18
            // Flag bits at +0x37f19, an unaligned unsigned short in the
            // original, written both as individual bits and as a whole word.
            union Flags_0045b800 {
                unsigned short word;
                struct {
                    unsigned short bit0 : 1;
                    unsigned short bit1 : 1;
                    unsigned short bit2 : 1;
                    unsigned short bit3 : 1;
                    unsigned short bit4 : 1;
                    unsigned short bit5 : 1;
                    unsigned short bit6 : 1;
                    unsigned short rest : 9;
                } bits;
            } soundFlags;              // +0x37f19
            int width;                 // +0x37f1b
            int height;                // +0x37f1f
            int field_37f23;           // +0x37f23
            int field_37f27;           // +0x37f27
            char unknown_37f2b[0x37f39 - 0x37f2b];
        };
    };
    char unknown_37f39[0x38a4b - 0x37f39];
    unsigned short field_38a4b;        // +0x38a4b
    short field_38a4d;                 // +0x38a4d
};

// A row of a label table: the format and the largest value it applies to.
struct Entry_0045c010 {
    char* format;   // +0x0
    int min;        // +0x4
};

// The saved settings block: 0x53 bytes of state, then bits 0 and 1 of the
// word at +0x53 (0x512f6b).
struct Settings_45cde0 {
    char block[0x53];                  // +0x0
    unsigned short bit0 : 1;           // +0x53, bit 0
    unsigned short bit1 : 1;           // +0x53, bit 1
    unsigned short rest : 14;
};

#pragma pack(pop)

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

class Class_004ce3e0 {
public:
    void FUN_004ce3e0(const void* src);
};

class Class_004ce580 {
public:
    void FUN_004ce580(int value);
};

class Class_004ce5a0 {
public:
    int FUN_004ce5a0();
};

class Class_004ce7a0 {
public:
    int SetPlaybackOrder(int value);
};

class Class_004ce7c0 {
public:
    void SetCategoryOfTrack(int index, unsigned char value);
};

class Class_004ce7e0 {
public:
    unsigned char GetCategoryOfTrack(int index);
};

class Class_004d0070 {
public:
    void SetWaveVolume(int level);
};

class Class_004d00d0 {
public:
    void SetAuxVolume(int level, int flag);
};

class Sound {
public:
    char unknown_0[4];
    int field_4;

    void Enable3D();
    void Disable3D();
};

// GLOBAL: 0x511de8
extern Game* g_game;

extern int DAT_00512c80;
extern int DAT_00512c84;
extern char DAT_00512ca8[];
extern char DAT_00512d90[];
extern int DAT_00512f2c;
extern int DAT_00512f38;
extern int DAT_00512f3a;
extern int DAT_00512f3e;
extern int DAT_00512f42;
// Declared int: the flag-sync xor must be computed at int width.
extern int DAT_00512f46;
extern char DAT_00512f48;
extern unsigned char DAT_00512f49;
extern char DAT_00512f4a;
// A byte in the original: declared unsigned int to keep it in bl for the bitfield merge.
extern unsigned int DAT_00512f4b;
extern int DAT_00512f4d;
extern int DAT_00512f51;
extern int DAT_00512f55;
extern int DAT_00512f59;
extern int DAT_00512f6d;
extern int DAT_00512f71;
extern char DAT_00512f75[];
extern int DAT_00512fd9;
extern int DAT_00512fe0;
// Flags live in this struct, not a standalone global: keeps the load order.
extern Settings_45cde0 DAT_00512f18;

void __stdcall FUN_0049fa90(Menu_0045b800* menu);
void __stdcall FUN_004a0570(Menu_0045b800* menu, char* name, int value);
char* __stdcall FUN_004a0180(Entry_0045b800* entries, char* name);
Entry_0045b800* __stdcall FUN_004a0200(Entry_0045b800* entries, char* name);
void __stdcall FUN_004a0bf0(Menu_0045b800* obj, char* name, char* text, int param_4);
void __stdcall FUN_004a1200(Menu_0045b800* menu, int index, int value);
void __stdcall FUN_004a1250(Menu_0045b800* obj, char* name, int value);
void __stdcall FUN_004a1450(Menu_0045b800* obj, char* name, int param_3);
int __stdcall SetButtonStageByName(Menu_0045b800* obj, char* name, int value);
void __stdcall SetBrightness(float value);
void __stdcall SetGameSpeed(unsigned int param1, int param2);
int __stdcall FindGadgetIndex(Entry_0045b800* entries, char* name, int type);
void __stdcall SetGadgetStatus(Menu_0045b800* menu, int index, short value);
void __stdcall GetGadgetName(Entry_0045b800* entries, char* name, int index);
Entry_0045b800* __stdcall FindGadgetOrNull(Entry_0045b800* entries, char* name);
// Defined in options_45c820.cpp, which keeps its own view of Game.
void FUN_0045c820();

static inline int SliderValue(Entry_0045b800* e)
{
    if (e->steps <= 1)
        return 0;
    return (int)((float)e->pos / (e->steps - 1) * e->max);
}

static inline void ApplySound()
{
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// FUNCTION: 0x45b800
void __stdcall SetDirectConnectAddress(char* param_1)
{
    DAT_00512d90[0] = 0;
    strncat(DAT_00512d90, param_1, 0x3f);
}

// FUNCTION: 0x45b820
void __stdcall FUN_0045b820(int flag, char* text)
{
    DAT_00512c84 = flag != 0;
    if (text) {
        DAT_00512ca8[0] = 0;
        strncat(DAT_00512ca8, text, 0x3f);
    }
}

// FUNCTION: 0x45b860
void __stdcall FUN_0045b860(int param_1)
{
    if (param_1 == 1 || param_1 == 2 || param_1 == 3 || param_1 == 4) {
        DAT_00512c80 = param_1;
    }
}

// FUNCTION: 0x45b880
void __stdcall FUN_0045b880(char* name, int value)
{
    for (int i = 0; i <= g_game->menu.holder->entries->count; i++) {
        if (strncmp(g_game->menu.holder->entries[i].name, name, strlen(name)) == 0 &&
            g_game->menu.holder->entries[i].type == 1) {
            FUN_004a1200(&g_game->menu, i, value);
        }
    }
    FUN_0049fa90(&g_game->menu);
}

// FUNCTION: 0x45b920
void __stdcall FUN_0045b920(char* prefix)
{
    for (int i = 0; i <= g_game->menu.holder->entries[0].count; i++) {
        if (strncmp(g_game->menu.holder->entries[i].name, prefix, strlen(prefix)) == 0) {
            FUN_004a0570(&g_game->menu, g_game->menu.holder->entries[i].name, 0);
        }
    }
}

// Sets field_140 (a step index out of field_136 steps) from a value in the
// range 0..field_13c, rounding up; the inverse of ReadSliderValue.
// FUNCTION: 0x45b9b0
void __stdcall SetSliderFromValue(Entry_0045b800* param_1, int value)
{
    int max = param_1->max;
    if (value > max)
        value = max;
    float f = (float)value / (float)max * (param_1->steps - 1);
    if (f - (int)f != 0.0f)
        f += 1.0;
    param_1->pos = (short)f;
}

// FUNCTION: 0x45ba20
int __stdcall ReadSliderValue(Entry_0045b800* param_1)
{
    if (param_1->steps <= 1)
        return 0;
    return (int)((float)param_1->pos / (param_1->steps - 1) * param_1->max);
}

// FUNCTION: 0x45ba60
void __stdcall FUN_0045ba60(int index, int state, char* offText, char* onText)
{
    char name[128];
    Entry_0045b800* entries = g_game->menu.holder->entries;
    SetGadgetStatus(&g_game->menu, index, state);
    GetGadgetName(entries, name, index);
    Entry_0045b800* e = FindGadgetOrNull(entries, name);
    // An if/else of two strcpy calls; a ternary argument places the
    // destination lea after the branch instead of before it.
    if (!state) {
        strcpy(e->text, offText);
    } else {
        strcpy(e->text, onText);
    }
}

// FUNCTION: 0x45baf0
void __stdcall FUN_0045baf0(void* param_1, int param_2)
{
    *(char*)((char*)param_1 + 0xb6) = 0;
}

// The VIDEOVAL menu entry holds the current resolution text. The mode list
// is searched for the mode matching the current screen size (width compared
// against a local read before the loop, height re-read every iteration), and
// the index of that mode is turned into a step index in pos, the same
// arithmetic as SetSliderFromValue.
// FUNCTION: 0x45bb00
void __stdcall FUN_0045bb00(Menu_0045b800* param_1, Entry_0045b800* param_2)
{
    int i = 0;
    int count = param_2->list->count;
    if (count > 0) {
        Game* g = g_game;
        int w = g->width;
        Mode_0045b800* m = param_2->list->modes;
        do {
            if (w != m->width)
                goto next;
            if (g->height != m->height)
                goto next;
            {
                int max = param_2->max;
                int n = i;
                if (n > max)
                    n = max;
                float f = (float)n / (float)max * (param_2->steps - 1);
                if (f - (int)f != 0.0f)
                    f += 1.0;
                param_2->pos = (short)f;

                Entry_0045b800* e = (Entry_0045b800*)FUN_004a0180(param_1->holder->entries, "VIDVAL");
                if (e)
                    sprintf(e->text, "%d X %d", m->width, m->height);
                break;
            }
next:
            i++;
            m++;
        } while (i < count);    }
}

// Applies the "VIDSLDR" slider: picks the current resolution entry from the
// slider's table of 3 dword entries, writes its "%d X %d" text into the
// "VIDVAL" name, and copies the resolution into the game state.
// FUNCTION: 0x45bbf0
void __stdcall HandleVideoModeSlider(Menu_0045b800* obj, int unused)
{
    // The original reads e->list before testing e for null, so this load must
    // stay above the if. If FUN_004a0200 ever returned 0 the original would
    // have read through a null pointer; that is a real bug in the game code.
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "VIDSLDR");
    ModeList_0045b800* list = e->list;
    if (e != 0) {
        Mode_0045b800* r = &list->modes[SliderValue(e)];
        sprintf(FUN_004a0180(obj->holder->entries, "VIDVAL") + 0xb6, "%d X %d", r->width, r->height);
        g_game->width = r->width;
        g_game->height = r->height;
    }
    FUN_0049fa90(&g_game->menu);
}

// Applies the brightness value and the two volume levels (scaled by 1024) to
// the object at g_game+0x10 (same tail as 0x45c630).
// FUNCTION: 0x45bcc0
void ApplyBrightnessAndVolume()
{
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// Reads the "GAMMA" slider into the brightness setting, then applies the
// brightness and both volume levels (inlined helpers as in 0x45bea0).
// FUNCTION: 0x45bd20
void __stdcall HandleGammaSlider(Menu_0045b800* obj, int unused)
{
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "GAMMA");
    if (e != 0) {
        g_game->brightness = SliderValue(e);
        ApplySound();
    }
}

// Reads the "FXVOL" slider into the effects volume setting, then applies the
// brightness and both volume levels (same shape as 0x45bea0).
// FUNCTION: 0x45bde0
void __stdcall HandleEffectsVolumeSlider(Menu_0045b800* obj, int unused)
{
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "FXVOL");
    if (e != 0) {
        g_game->volume1 = SliderValue(e);
        ApplySound();
    }
}

// Reads the "MUSICVOL" slider into the music volume setting, then applies the
// brightness and both volume levels (inlined ReadSliderValue and ApplyBrightnessAndVolume).
// FUNCTION: 0x45bea0
void __stdcall HandleMusicVolumeSlider(Menu_0045b800* obj, int unused)
{
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "MUSICVOL");
    if (e != 0) {
        g_game->volume2 = SliderValue(e);
        ApplySound();
    }
}

// Formats "<label> (<speed>)" for a game-speed setting, then passes the
// original name and label (not the formatted text) to FUN_004a0bf0 on the
// settings block at g_game+0x519.
// FUNCTION: 0x45bf60
void __stdcall SetGameSpeedLabel(char* name, char* label, int speed, int normal)
{
    char buf[200];
    char* text;
    if (speed == normal) {
        text = "Normal";
    } else if (speed < normal / 4) {
        text = "Slow";
    } else if (speed < normal / 2) {
        text = "Slower";
    } else if (speed > normal * 3 / 4) {
        text = "Fast";
    } else {
        text = "Faster";
    }
    sprintf(buf, "%s (%s)", label, text);
    FUN_004a0bf0(&g_game->menu, name, label, 0);
}

// FUNCTION: 0x45c010
void __stdcall FUN_0045c010(Entry_0045c010* table, char* name, int value)
{
    char buf[100];
    if (table->format == 0)
        return;
    while (table->format != 0) {
        if (value <= table->min) {
            sprintf(buf, table->format, value);
            FUN_004a0bf0(&g_game->menu, name, buf, 0);
            return;
        }
        table++;
    }
}

// A menu control handler: unless the local player is a connected human
// player (a non-empty player slot whose info has bit 6 of the byte at +0x9b
// set), it reads the "GAME" slider, clamps the slider value to at least 1 and
// pushes it into the game setting at g_game+0x38a4b, then applies the control.
// FUNCTION: 0x45c070
void __stdcall HandleGameSpeedSlider(Menu_0045b800* obj, int unused)
{
    Player_45c070* player = &g_game->players[g_game->localPlayer];
    if (player->field_0 == 0 || !player->info->flag_9b_6) {
        Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "GAME");
        if (e != 0) {
            // Written twice, as in the original.
            int value = SliderValue(e);
            g_game->field_38a4b = (unsigned short)(value < 1 ? 1 : SliderValue(e));
            SetGameSpeed(g_game->field_38a4b, 1);
            FUN_0049fa90(obj);
        }
    }
}

// Reads the "SCREEN" slider of the menu object and stores its value in
// g_game->field_1434d, writing 1 instead of any value of 1 or less, then
// marks the object changed (FUN_0049fa90 sets obj->field_cca = 1).
// FUNCTION: 0x45c170
void __stdcall HandleScreenSlider(Menu_0045b800* obj, int unused)
{
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "SCREEN");
    if (e != 0) {
        g_game->field_1434d = SliderValue(e) > 1 ? SliderValue(e) : 1;
        FUN_0049fa90(obj);
    }
}

// Reads the "MAXLINES" slider into the max lines field of g_game and shows it
// as "<n>", or "None" when it is 0. The slider value is evaluated twice, and
// only the second result is kept unless the first one was negative (both are
// identical, so the store is a clamp to 0 either way).
// FUNCTION: 0x45c220
void __stdcall HandleMaxLinesSlider(Menu_0045b800* obj, int unused)
{
    char text[20];
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "MAXLINES");
    if (e != 0) {
        int v = SliderValue(e);
        g_game->field_37f27 = v < 0 ? 0 : SliderValue(e);
        FUN_0049fa90(obj);
    }
    if (g_game->field_37f27 != 0)
        sprintf(text, "%d", g_game->field_37f27);
    else
        strcpy(text, "None");
    FUN_004a0bf0(obj, "MAXLINESTEXT", text, 0);
}

// Reads the "TXTSCROL" slider into the text scroll time and shows it as
// "<n> secs" (the slider value is the inlined ReadSliderValue, as in 0x45bea0).
// FUNCTION: 0x45c330
void __stdcall HandleTextScrollSlider(Menu_0045b800* obj, int unused)
{
    char text[20];
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "TXTSCROL");
    if (e != 0) {
        g_game->field_37f23 = SliderValue(e);
        sprintf(text, "%d secs", g_game->field_37f23);
        FUN_004a0bf0(obj, "TEXTSCROLLTEXT", text, 0);
        FUN_0049fa90(obj);
    }
}

// FUNCTION: 0x45c3d0
void __stdcall FUN_0045c3d0(int value)
{
    ((Class_004ce580*)g_game->sound)->FUN_004ce580(value);
}

// FUNCTION: 0x45c3f0
void UpdateTrackGadgets()
{
    char buf[12];
    Menu_0045b800* menu = &g_game->menu;

    if (FindGadgetIndex(menu->holder->entries, "TRACKTYPE", 1) != -1) {
        int disc = DAT_00512fe0;
        FUN_004a1250(menu, "TRACKTYPE",
                     ((g_game->f_37f14 & 1) && g_game->field_37f16 == 4) ? 0 : 1);
        SetButtonStageByName(menu, "TRACKTYPE", ((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(disc));
        if (disc == 0)
            strcpy(buf, "NO DISC");
        else
            sprintf(buf, "%d", disc);
        FUN_004a0bf0(menu, "TRACKNUM", buf, 0);
    }
    FUN_004a1450(menu, "TRACKNUM", (char)(~g_game->f_37f14) & 1);
    if (g_game->field_37f16 == 3) {
        // the track number is re-read from the global here, not taken from disc
        int track = DAT_00512fe0;
        ((Class_004ce580*)g_game->sound)->FUN_004ce580(track);
    }
}

// When the game is in state 4, looks up the "TRACKTYPE" gadget in the menu
// and passes its value byte on to SetCategoryOfTrack.
// FUNCTION: 0x45c510
void ApplyTrackType()
{
    Entry_0045b800* gadgets = g_game->menu.holder->entries;
    if (g_game->field_37f16 == 4) {
        int index = FindGadgetIndex(gadgets, "TRACKTYPE", 1);
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(DAT_00512fe0, gadgets[index].value);
    }
}

// Sets up the sound state: volume1, three sound flag bits, a redraw of the
// sound object, more flag bits, then brightness and the two volume levels
// scaled by 1024 (same tail as 0x45bcc0).
// FUNCTION: 0x45c570
void FUN_0045c570()
{
    g_game->volume1 = 0x1b;
    g_game->soundFlags.bits.bit4 = 1;
    g_game->soundFlags.bits.bit5 = 1;
    g_game->soundFlags.bits.bit6 = 1;
    ((Sound*)g_game->sound)->Disable3D();
    g_game->soundFlags.word = (g_game->soundFlags.word & 0xfff9) | 1;
    g_game->field_37f17 = 10;
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// Resets two settings (0x20 at +0x37f10, 4 at +0x37f16), enables the object at
// g_game+0x10 once (bit 0 of +0x37f14), then applies the brightness value and
// the two volume levels (scaled by 1024) to it.
// FUNCTION: 0x45c630
void FUN_0045c630()
{
    g_game->volume2 = 0x20;
    g_game->field_37f16 = 4;
    if (!(g_game->flags14 & 1)) {
        g_game->flags14 |= 1;
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// FUNCTION: 0x45c6d0
void FUN_0045c6d0()
{
    g_game->field_37f23 = 10;
    g_game->field_37f27 = 10;
    g_game->field_38a4b = 10;
    g_game->field_38a4d = 10;
    g_game->field_1434d = 0x20;
    g_game->field_37efa = 0;
    g_game->field_37f17 = 10;
    g_game->field_37f18 = 5;
}

// Sets the five share flag bits in the flags word at +0x37f06, sets the
// brightness at +0x37f08, and (unless bit 2 of +0x2a44 is set) the screen
// width and height at +0x37f1b/+0x37f1f and clears bit 6 of the flags word.
// Then applies the brightness and the two volume levels to the sound object.
// FUNCTION: 0x45c740
void FUN_0045c740()
{
    g_game->bit1 = 1;
    g_game->bit2 = 1;
    g_game->bit3 = 1;
    g_game->bit4 = 1;
    g_game->bit5 = 1;
    g_game->brightness = 12;
    if (!g_game->flag_2a44) {
        g_game->width = 640;
        g_game->height = 480;
        g_game->bit6 = 0;
    }
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// Loads the saved audio settings (globals around 0x512f42) into the game and
// applies them to the sound object at g_game+0x10.
// FUNCTION: 0x45c950
void LoadSavedAudioSettings()
{
    g_game->volume2 = DAT_00512f42;
    ((Class_004ce3e0*)g_game->sound)->FUN_004ce3e0(&DAT_00512f75);
    g_game->field_37f16 = DAT_00512f48;
    ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
    if (((unsigned char)g_game->flags14 ^ (unsigned char)DAT_00512f46) & 1) {
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    unsigned short f = g_game->flags14;
    f = f ^ ((f ^ DAT_00512f46) & 1);
    g_game->flags14 = f;
    ((Class_004ce580*)g_game->sound)->FUN_004ce580(DAT_00512fd9);
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// Copies saved option values (globals around 0x512f2c) into the game.
// FUNCTION: 0x45ca50
void FUN_0045ca50()
{
    g_game->field_37f23 = DAT_00512f55;
    g_game->field_38a4b = DAT_00512f6d;
    g_game->field_38a4d = DAT_00512f6d;
    g_game->field_1434d = DAT_00512f71;
    g_game->field_37efa = DAT_00512f2c;
    g_game->field_37f17 = DAT_00512f49;
    g_game->field_37f18 = DAT_00512f4a;
    g_game->field_37f27 = DAT_00512f59;
}

// Copies six flag bits out of the saved settings value into the flags word at
// +0x37f06, restores the brightness and resolution defaults, then applies the
// brightness and both volume levels to the object at g_game+0x10 (same tail as
// 0x45bcc0).
// FUNCTION: 0x45cae0
void FUN_0045cae0()
{
    unsigned short v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 2);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 4);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 8);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 0x10);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 0x20);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 0x40);

    g_game->brightness = DAT_00512f3a;
    if (!(g_game->field_2a44 & 4)) {
        g_game->width = DAT_00512f4d;
        g_game->height = DAT_00512f51;
    }

    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// Loads the saved game settings (globals around 0x512f42) into the game and
// applies them to the sound object at g_game+0x10, then copies the remaining
// saved options (as 0x45ca50 does) and runs the post-load fixups (0x45cae0).
// FUNCTION: 0x45cc50
void LoadSavedSettings()
{
    FUN_0045c820();
    g_game->volume2 = DAT_00512f42;
    ((Class_004ce3e0*)g_game->sound)->FUN_004ce3e0(&DAT_00512f75);
    g_game->field_37f16 = DAT_00512f48;
    ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
    if (((unsigned char)g_game->flags14 ^ (unsigned char)DAT_00512f46) & 1) {
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    unsigned short f = g_game->flags14;
    g_game->flags14 = f ^ ((f ^ DAT_00512f46) & 1);
    ((Class_004ce580*)g_game->sound)->FUN_004ce580(DAT_00512fd9);
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
    g_game->field_37f23 = DAT_00512f55;
    g_game->field_38a4b = DAT_00512f6d;
    g_game->field_38a4d = DAT_00512f6d;
    g_game->field_1434d = DAT_00512f71;
    g_game->field_37efa = DAT_00512f2c;
    g_game->field_37f17 = DAT_00512f49;
    g_game->field_37f18 = DAT_00512f4a;
    g_game->field_37f27 = DAT_00512f59;
    FUN_0045cae0();
}

// Saves the current game settings: copies the 0x53-byte block at
// g_game+0x37ee6 into the settings block at DAT_00512f18, saves two game
// flags into bits 0 and 1 of that block's trailing word, then saves the
// track number, the option byte, the current track index and the 100
// track-name characters read from the object at g_game+0x10.
// FUNCTION: 0x45cde0
void SaveGameSettings()
{
    memcpy(DAT_00512f18.block, (char*)g_game + 0x37ee6, 0x53);
    DAT_00512f18.bit0 = g_game->flags14281.bit1;
    DAT_00512f18.bit1 = g_game->flags14281.bit2;
    DAT_00512f6d = g_game->field_38a4b;
    DAT_00512f71 = g_game->field_1434d;
    DAT_00512fd9 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
    for (int i = 0; i < 100; i++) {
        DAT_00512f75[i] = ((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(i);
    }
}

// Looks up the "PANEL" gadget in the menu's entry table (entry 0 holds the
// count as a short at +0xb6). When the game flag at +0x37ebe is set the panel
// layout grows by 0x96, and if there is no PANEL entry yet a cleared one is
// appended: type 0xb, x = 0x80, its width shrunk by x, height copied from the
// table, named "PANEL", and the table's +0xc4 field copied into it.
// FUNCTION: 0x45ce80
void FUN_0045ce80()
{
    Entry_0045b800* entries = g_game->menu.holder->entries;
    int index = FindGadgetIndex(entries, "PANEL", 0xe);
    if (g_game->field_37ebe & 1) {
        entries->width += 0x96;
        if (index == -1) {
            short c = entries->count;
            int i = c;
            i++;
            c++;
            entries->count = c;
            memset(&entries[i], 0, sizeof(Entry_0045b800));
            // Index entries[i] at every field, not a local pointer: keeps the string setup order.
            entries[i].type = 0xb;
            entries[i].x = 0x80;
            entries[i].width = entries->width;
            entries[i].y = 0;
            entries[i].width -= entries[i].x;
            entries[i].height = entries->height;
            strcpy(entries[i].name, "PANEL");
            entries[i].field_29 = 1;
            entries[i].field_c4 = entries->field_c4;
        }
    }
}
