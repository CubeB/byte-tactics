// Decompiled by Haiku, Opus, DeepSeek V4.1 Flash, Sonnet, space-bunny-free, Space Bunny Free, deepseek-v4.1-flash, deepseek-v4.1, GPT-6, GPT-6.1-sol, Claude Sonnet 5.5, Claude Opus 5.5 and claude-opus-5-5. Names are provisional.
//
// The options dialogs and screens of PREFS.GUI: the direct-connect address and
// flag setters, the menu-entry helpers, the slider readers, the sound, video
// and game-setting handlers, and the music, sound, visual, speed, controls,
// help, briefing and in-game options pages with their click handlers
// (0x45b800 to 0x460cc0). The module's files gathered in address order;
// 0x45c820 keeps its own file (its own declaration context decides its match).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/tdf.h"

// The original files included <windows.h>; the only thing the code uses from
// it is wsprintfA, declared here so its many declarations stay out of the file.
extern "C" __declspec(dllimport) int __cdecl wsprintfA(char* out, const char* format, ...);

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

class Class_004ce3e0 {
public:
    void CopyTrackTypeTable(const void* src);
};

class Class_004ce450 {
public:
    int GetTrackCount();
};

class Class_004ce580 {
public:
    void SetLockedTrack(int value);
};

class Class_004ce5a0 {
public:
    int GetLockedTrack();
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

class Class_004ce8c0 {
public:
    int SelectTrack(int value);
};

class Class_004ce910 {
public:
    int PauseCdAudio(int value);
};

class Class_004ced40 {
public:
    int StopCdAudio();
};

class Class_004cedc0 {
public:
    void EnableCdAudio(int value);
};

class Class_004d0070 {
public:
    int SetWaveVolume(int level);
};

class Class_004d00d0 {
public:
    int SetAuxVolume(int level, int flag);
};

// The sound object at g_game+0x10.
#include "../sound/sound.h"

// The mission or game-mode object at g_game+0x391e9.
class Mission {
public:
    int GetGameType();
    char* GetMissionName();
    char* GetMissionName(int player);
};

class CMemoryCache {
public:
    void FlushCache();
};

class Class_004a1080;
class Object_004a0570;
class Object_004a1450;
struct Dialog;
struct Class_004c6a60;
struct Mode_0045e4c0;
struct Gui_0045e100;

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

// The flag bits at g_game+0x2a44, an unaligned word in the original.
union Bits_0045cf60 {
    unsigned char byte;                // +0x2a44, the whole byte
    unsigned short word;
    struct {
        unsigned short bit0 : 1;
        unsigned short bit1 : 1;
        unsigned short bit2 : 1;
        unsigned short rest : 13;
    } bits;
    struct {
        unsigned short pad : 2;
        unsigned short flag : 1;       // bit 2
        unsigned short rest : 13;
    } flag;
    struct {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char prefs : 1;       // bit 2
        unsigned char rest : 5;
    } prefsByte;
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short prefs : 1;      // bit 2
        unsigned short rest : 13;
    } prefsWord;
};

// The flag word at g_game+0x37ebe: the loaded bit, the state bytes and the
// order flags the in-game options clear.
union Flags37ebe_0045cf60 {
    unsigned char byte;
    unsigned short word;
    struct {
        unsigned short loaded : 1;
        unsigned short rest : 15;
    } loadedBits;
    struct {
        unsigned short b0 : 1;
        unsigned short rest : 15;
    } bits;
};

// The flag word at g_game+0x37f06: the anti-aliasing, shadow and shading bits.
union Flags37f06_0045cf60 {
    unsigned char byte;
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short b2 : 1;
        unsigned short b3 : 1;
        unsigned short b4 : 1;
        unsigned short b5 : 1;
        unsigned short b6 : 1;
        unsigned short rest : 9;
    } bits;
};

// The word at g_game+0x37f14: the NOTRAK flag and the sound state bits.
union Flags37f14_0045cf60 {
    char notrak;
    unsigned short word;
};

// The flag word at g_game+0x37f19: the sound mode in bits 0 to 2, the speech
// bit at 6 and the 3D flags.
union SoundFlags_0045cf60 {
    unsigned short word;
    unsigned char byte;
    struct {
        unsigned short mode : 3;
        unsigned short b3 : 1;
        unsigned short b4 : 1;
        unsigned short b5 : 1;
        unsigned short speech : 1;     // bit 6
        unsigned short b7 : 1;
        unsigned short rest : 8;
    } bits;
};

// The line-of-sight flags word at g_game+0x14281.
union Los_0045cf60 {
    unsigned short losFlags;
    struct {
        unsigned short l0 : 1, l1 : 1, l2 : 1, l3 : 1, l4 : 1, l5 : 1, l6 : 1, l7 : 1,
                       l8 : 1, l9 : 1, l10 : 1, l11 : 1, l12 : 1, l13 : 1, l14 : 1, l15 : 1;
    } lb;
    struct {
        unsigned short bit0 : 1, bit1 : 1, bit2 : 1, rest : 13;
    } bits;
};

// The GUI object at g_game+0x519: the logo GAF at +0x4, the menu's holder at
// +0x18 and the settings block's changed flag at +0xcca.
struct Gui_0045cf60 {
    char unknown_0[4];
    void* logos32;                     // +0x04 (g_game+0x51d)
    char unknown_8[0x18 - 0x8];
    void* holder;                      // +0x18 (g_game+0x531)
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char field_dcb;           // +0x8b2 (g_game+0xdcb)
    char unknown_8b3[0xcca - 0x8b3];
    int field_cca;                     // +0xcca
};

// One player's options at the player array's +0x27: the low bits of the word
// at +0x9b are the player's flags, its high bits the cheat and watch settings.
struct Opts_0045f1d0 {                 // 0x14b bytes
    char unknown_0[0x9b];
    union {
        unsigned short value;          // +0x9b
        struct {
            unsigned short unknown_9b_0 : 6;  // +0x9b, bits 0 to 5
            unsigned short flag_9b_6 : 1;     // bit 6 (mask 0x40)
            unsigned short unknown_9b_7 : 9;
        };
        struct {
            unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1,
                           b7 : 1, b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1,
                           b14 : 1, b15 : 1;
        } b;
    } u;
    char unknown_9d[0xa1 - 0x9d];
    unsigned short startEnergy;        // +0xa1
    unsigned short startMetal;         // +0xa3
    char unknown_a5[0x14b - 0xa5];
};

struct Player_45c070 {
    int field_0;                      // +0x00
    char unknown_4[0x27 - 0x4];
    Opts_0045f1d0* info;              // +0x27
    char unknown_2b[0x14b - 0x2b];
};

// Note: Rule_0045f1d0 (24 bytes, startMetal at +0xc, startEnergy at +0x10)
// and RuleSet_0045f1d0 (startType at +0x118) are the SAME memory: the exe
// walks g_game->rules with a 24-byte stride for metal and energy but reads
// startType from element 0, i.e. it ignores playerType for that field.
struct RuleSet_0045f1d0 {              // +0x118 startType, read from element 0
    char unknown_0[0xc];
    int startMetal;                    // +0xc
    int startEnergy;                   // +0x10
    char unknown_14[0x118 - 0x14];
    int startType;                     // +0x118
};

struct Flags_00460680 {
    unsigned short unknown_bit0 : 4;
    unsigned short flag4 : 1;          // bit 4
    unsigned short unknown_rest : 11;
};

// The game state. Every view of it in these files meets here: the fields are
// at the offsets the functions use, and the differently typed views of the
// flag words share a union.
struct Game {
    char unknown_0[0x10];
    Sound* sound;                      // +0x10
    char unknown_14[0x519 - 0x14];
    union {
        Menu_0045b800 menu;            // +0x519, the menu helpers' view
        Gui_0045cf60 gui;              // +0x519, the options screens' view
    };
    char unknown_11e7[0x1b63 - 0x11e7];
    Player_45c070 players[10];         // +0x1b63, the info pointer at +0x27
    char unknown_2851[0x29a0 - 0x2851];
    RuleSet_0045f1d0* rules;           // +0x29a0
    char unknown_29a4[0x2a42 - 0x29a4];
    union {
        unsigned char localPlayer;     // +0x2a42
        unsigned char playerType;      // the same byte
    };
    char unknown_2a43[0x2a44 - 0x2a43];
    union {
        unsigned char flags_2a44;      // +0x2a44
        Bits_0045cf60 bits_2a44;
        struct {
            unsigned short pad_2a44 : 2;
            unsigned short flag_2a44 : 1;
            unsigned short rest_2a44 : 13;
        };
    };
    char unknown_2a46[0x2bc0 - 0x2a46];
    unsigned char frontendSubstateRequest;  // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    Flags_00460680 flags_2bee;         // +0x2bee
    char unknown_2bf0[0x14281 - 0x2bf0];
    union {
        struct {
            unsigned short bit0 : 1;   // +0x14281
            unsigned short bit1 : 1;
            unsigned short bit2 : 1;
            unsigned short rest : 13;
        } flags14281;
        Los_0045cf60 los;              // +0x14281
    };
    char unknown_14283[0x142f1 - 0x14283];
    unsigned short flags_142f1;        // +0x142f1
    char unknown_142f3[0x1434d - 0x142f3];
    unsigned char scrollSpeed;         // +0x1434d
    char unknown_1434e[0x1437b - 0x1434e];
    CMemoryCache* ptr_1437b;           // +0x1437b
    char unknown_1437f[0x37e1b - 0x1437f];
    void* field_37e1b;                 // +0x37e1b
    char unknown_37e1f[0x37e98 - 0x37e1f];
    int field_37e98;                   // +0x37e98
    char unknown_37e9c[0x37ebe - 0x37e9c];
    union {
        unsigned char field_37ebe;     // +0x37ebe
        Flags37ebe_0045cf60 flags_37ebe;
    };
    char unknown_37ec0[0x37ee6 - 0x37ec0];
    union {
        char block[0x53];              // +0x37ee6
        struct {
            unsigned short maxUnits;   // +0x37ee6
            char unknown_37ee8[0x37eee - 0x37ee8];
            int difficulty;            // +0x37eee
            char unknown_37ef2[0x37ef6 - 0x37ef2];
            int commanderDeath;        // +0x37ef6
            int field_37efa;           // +0x37efa
            char unknown_37efe[0x37f06 - 0x37efe];
            union {
                unsigned short flags;  // +0x37f06
                Flags37f06_0045cf60 flags_37f06;
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
            union {
                int volume1;           // +0x37f0c
                short volume1Word;
            };
            union {
                int volume2;           // +0x37f10
                short volume2Word;
            };
            union {
                unsigned short flags14; // +0x37f14
                Flags37f14_0045cf60 flags_37f14;
                struct {
                    char f_37f14;      // +0x37f14
                    char unknown_37f15;
                };
            };
            unsigned char field_37f16; // +0x37f16
            unsigned char field_37f17; // +0x37f17
            unsigned char field_37f18; // +0x37f18
            SoundFlags_0045cf60 soundFlags; // +0x37f19
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
    char unknown_38a4f[0x38a51 - 0x38a4f];
    union {
        unsigned char flags_38a51;     // +0x38a51
        unsigned short orders;         // 4609b0 clears it as a word
    };
    char unknown_38a53[0x391e9 - 0x38a53];
    Mission* mode;                     // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned char flags_3923b;         // +0x3923b
    char unknown_3923c[0x39249 - 0x3923c];
    int field_39249;                   // +0x39249
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

// One gadget inside a .GUI file, 0x15b bytes.
struct Entry_0045d280 {
    char state;                        // +0x00
    char unknown_1[0x137 - 1];
    unsigned char value;               // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Vtable_0045d280 {
    char unknown_0[8];
    void (__stdcall* FUN_8)(void* obj);
};

struct Holder_0045d280 {
    Vtable_0045d280* field_0;          // +0x00
    Entry_0045d280* entries;           // +0x04
};

// The menu layer object OpenOptionsLayout returns.
struct Object_0045d280 {
    char unknown_0[0x18];
    Holder_0045d280* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Entry_0045d7c0 {
    char unknown_0[0x136];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
    char unknown_142[2];
    void (__stdcall* callback)(void* obj, int value); // +0x144
};

struct Gadget_0045d7c0 {
    char unknown_0[0x137];
    unsigned char value;               // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Holder_0045d7c0 {
    int unknown_0;
    Gadget_0045d7c0* gadgets;          // +0x4
};

struct Object_0045d7c0 {
    char unknown_0[4];
    Entry_0045d7c0* gadgets;           // +0x4
    void (__stdcall* callback8)(int value); // +0x8
    char unknown_c[0x1c - 0xc];
    void (__stdcall* callback1c)();    // +0x1c
};

struct Entry_0045da90 {
    unsigned char type;                // +0x00
    char unknown_1;
    char name[0x11];                   // +0x02
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    char unknown_1b[0x137 - 0x1b];
    unsigned char value;               // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Vtable_0045da90 {
    char unknown_0[8];
    void (__stdcall* FUN_8)(void* obj);
};

struct Holder_0045da90 {
    Vtable_0045da90* field_0;          // +0x00
    Entry_0045da90* entries;           // +0x04
};

struct Object_0045da90 {
    char unknown_0[0x18];
    Holder_0045da90* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Entry_0045de30 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1[0xb6 - 1];
    short count;                       // +0xb6 (entry 0 only)
    char unknown_b8[0x136 - 0xb8];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
    char unknown_142[2];
    void (__stdcall *fn)(void* obj, int arg);   // +0x144
    char unknown_148[0x15b - 0x148];
};

struct Object_0045de30 {
    char unknown_0[4];
    Entry_0045de30* entries;           // +0x04
    void (__stdcall *fn)(void* obj, int arg);   // +0x08
};

struct Menu_0045de30 {
    char unknown_0[0x18];
    void* holder;                      // +0x18
};

// One entry of the gadget table, 0x15b bytes each; entry 0 is the header.
struct Entry_0045e100 {
    char state;                        // +0x0
    char unknown_1[1];
    char name[0x10];                   // +0x2
    char unknown_12[0x15b - 0x12];
};

struct Entry_0045e5e0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1;
    char name[0x10];                   // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6 (entry 0 only)
    char unknown_b8[0x136 - 0xb8];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
    char unknown_142[2];
    void (__stdcall* fn)(void* obj, int value);  // +0x144
    char unknown_148[2];
    void* data;                        // +0x14a
    char padding_14e[0x15b - 0x14e];   // stride is 0x15b
};

struct List_0045e5e0 {                 // the "SELECT VIDEO MODE" object
    int count;                         // +0x0
    Mode_0045e4c0* modes;              // +0x4
    char unknown_8[0xc];
    char* buffer;                      // +0x14
};

struct Layer_0045e5e0 {                // object returned by LoadGuiLayer
    char unknown_0[4];
    Entry_0045e5e0* entries;           // +0x4
    void (__stdcall* handler)(void*);  // +0x8
    void* data;                        // +0xc
    char unknown_10[0x15b];
};

struct Holder_0045e5e0 {
    char unknown_0[4];
    Entry_0045e5e0* entries;           // +0x4
};

struct Menu_0045e5e0 {
    char unknown_0[0x18];
    Holder_0045e5e0* holder;           // +0x18
};

struct Entry_0045ed50 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1[0xb6 - 1];
    short count;                       // +0xb6 (entry 0 only)
    char unknown_b8[0x136 - 0xb8];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
    char unknown_142[2];
    void (__stdcall* fn)(void* obj, int arg);   // +0x144
    char unknown_148[0x15b - 0x148];
};

struct Object_0045ed50 {
    char unknown_0[4];
    Entry_0045ed50* entries;           // +0x04
    void (__stdcall* fn)(void* obj, int arg);   // +0x08
};

struct Entry_0045f1d0 {                // 0x15b bytes
    char unknown_0[0x1b];
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        short count;                   // +0xb6 (entry 0 holds the entry count)
        char text[0x15b - 0xb6];
    } u;
};

struct Layer_0045f1d0 {
    int unknown_0;
    Entry_0045f1d0* entries;           // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

struct Rule_0045f1d0 {                 // 0x18 bytes, the record g_game->rules is
    char unknown_0[0xc];               //   walked with (stride 24)
    int startMetal;                    // +0xc
    int startEnergy;                   // +0x10
    char unknown_18[0x18 - 0x14];
};

struct Sub_0045f800 {
    char unknown_0[0x10];
};

struct Entry_0045f800 {
    char unknown_0[0x1b];
    int flags;                         // +0x1b
    char unknown_1f[0x15a - 0x1f];
};

struct Entry_0045f8c0 {                // 0x15b bytes
    char unknown_0[0x1b];
    int field_1b;                      // +0x1b
    char unknown_1f[0x15b - 0x1f];
};

struct Table_0045f8c0 {
    char unknown_0[0xb6];
    short count;                       // +0xb6
};

struct Layer_0045f8c0 {
    char unknown_0[4];
    char* entries;                     // +0x4
};

struct Sub_0045f8c0 {
    char unknown_0[0x18];
    Layer_0045f8c0* layer;             // +0x18
};

struct Sub_0045fb30 {
    char unknown_0[0x10];
};

struct Info_0045fb30 {
    char unknown_0[0xb6];
    short field_b6;                    // +0xb6
};

struct Layer_00460160 {
    char unknown_0[8];
    int (__stdcall* handler)(void*);   // +0x8
    char unknown_c[0x15 - 0xc];
    short field_15;                    // +0x15
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    char unknown_1b[0xbc - 0x1b];
    Class_004c6a60* field_bc;          // +0xbc
};

struct Holder_00460160 {
    char unknown_0[4];
    Layer_00460160* field_4;           // +0x4
};

struct Menu_00460160 {
    char unknown_0[0x18];
};

struct Gadget_004604a0 {
    char unknown_0[0x17];
    short field_17;                    // +0x17
    char unknown_19[0x15b - 0x19];
};

struct Sub_00460680 {
    char unknown_0[0x10];
};

struct Sub_004608b0 {
    char unknown_0[0x1c];
};

struct Sub_004609b0 {
    char unknown_0[0x10];
};

// One gadget inside a .GUI file. The two writes in the MISSION arm reach the
// entry as gadgets + i + i * 0x15a, that is, one gadget stride too far.
struct Entry_004609b0 {
    char unknown_0[0x1b];
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6
    char unknown_b8[0x15a - 0xb8];
};

struct Gui_00460cc0 {
    char unknown_0[0x18];
    void* holder;                      // +0x18
    char unknown_1c[0xa2 - 0x1c];
    int field_a2;                      // +0xa2
    char unknown_a6[0xcca - 0xa6];
    int field_cca;                     // +0xcca
};

#pragma pack(pop)

struct Sub_0045cf60 {
    char unknown_0[0xcca];
    int field_cca;                     // +0xcca
};

// The display mode record, shared by the sorter and the video mode page.
struct Mode_0045e4c0 {
    int width;                       // +0x0
    int height;                      // +0x4
    int refreshRate;                 // +0x8
};

struct ModeList_0045e4c0 {
    int count;                       // +0x0
    Mode_0045e4c0* modes;            // +0x4
};

// One help page's worth of lines: the strings used when a line has no
// translation, and the range of line numbers shown.
struct Page_0045f8c0 {
    char blank[2];                    // +0x0
    char blank2[2];                   // +0x2
    int first;                        // +0x4
    int last;                         // +0x8
};

struct Surface_0045fbc0 {
    int data[12];
};

struct Point_45ffb0 {
    int x;
    int y;
};

struct Quad_45ffb0 {
    Point_45ffb0 p[4];
};

struct Entry_45ffb0 {
    unsigned short w;                  // +0
    unsigned short h;                  // +2
};

struct Sound_45ffb0 {
    char unknown_0[4];
    short start;                       // +4
    short end;                         // +6
};

struct Gadget_0045f190 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Gadget_0045f770 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Gadget_0045fac0 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Gadget_0045fc60 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Gadget_00460340 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Gadget_00460800 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

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

struct GadgetOwner_004605c0 {
    int unknown_0;
    int field_4;                       // +0x4
};

struct Gadget_004605c0 {
    char unknown_0[0x18];
    GadgetOwner_004605c0* owner;       // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Gadget_004609b0 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Gadget_00460680 {
    char unknown_0[0x4];
    char* entries;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

struct Entry_00460cc0 {
    char unknown_0[2];
    char name[0x10];                   // +0x2
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0x15b - 0xb8];
};

struct Gadget_00460cc0 {
    char unknown_0[4];
    Entry_00460cc0* info;              // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

// The surface laid out by AllocSurface: the pixels follow a 0x30-byte header.
struct Class_004c6a60 {
    char unknown_0[0xc];
    char* pixels;                   // +0xc
};

struct Dst_004b8ae0 {
    char unknown_0[0x14];
};

typedef int (__stdcall* Handler_0045e100)(Gui_0045e100*);

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

struct Info_0045f800 {
    char unknown_0[0x4];
    Entry_0045f800* info;              // +0x4
    int (__stdcall* handler)(void*);   // +0x8
};

struct Gadget_0045fb30 {
    char unknown_0[0x4];
    Info_0045fb30* info;               // +0x4
    int (__stdcall* handler)(void*);   // +0x8
};

// A dialog loaded from a .GUI file.
struct Dialog_004604a0 {
    int unknown_0;
    Gadget_004604a0* gadgets;         // +0x4
    void (__stdcall* handler)(void*); // +0x8
};

// The GUI system object, at g_game + 0x519.
struct Menu_004604a0 {
    int unknown_0;
    int unknown_4;
    int field_8;
    int field_c;
    int unknown_10;
    int field_14;                     // +0x14
    char unknown_18[0x1c - 0x18];
};

struct Dialog_004608b0 {
    int unknown_0;                     // +0x0
    void* gadgets;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

struct Info_004609b0 {
    char unknown_0[0x4];
    Entry_004609b0* info;              // +0x4
    int (__stdcall* handler)(void*);   // +0x8
};

// GLOBAL: 0x511de8
extern Game* g_game;

extern int g_optionsShellClosing;
extern char g_optionsSoundName[];      // "Options"
extern char g_bgmSoundName[];          // "BGM"
extern char g_notrakGadgetName[];      // "NOTRAK"
extern char g_fxVolGadgetName[];       // "FXVOL"
extern char g_trackTypeGadgetName[];   // "TRACKTYPE"
extern char g_cdNextGadgetName[];      // "CDNEXT"
extern char g_cdPlayGadgetName[];      // "CDPLAY"
extern char g_cdStopGadgetName[];      // "CDSTOP"
extern char g_cdPrevGadgetName[];      // "CDPREV"
extern char g_trackModeGadgetName[];   // "TRACKMODE"
extern char g_restoreGadgetName[];     // "RESTORE"
extern char g_undoGadgetName[];        // "UNDO"
extern char g_speechGadgetName[];      // "SPEECH"
extern char g_testGadgetName[];        // "TEST"
extern char g_volTextGadgetName[];     // "VOLTEXT"
extern char g_modeGadgetName[];        // "MODE"
extern char g_explodeSoundFile[];      // "sounds\\explode.wav"
extern char DAT_005119b8[];
extern int DAT_00512c80;
extern int DAT_00512c84;
extern char DAT_00512ca8[];
extern char DAT_00512d90[];
extern int g_helpDialogBaseGadgetCount;
extern Entry_45ffb0 g_optionsFlipFrame;
extern int g_optionsLightbarY;
extern int g_optionsLightbarMaxX;
extern int g_optionsBackupInterfaceType;
extern int g_optionsBackupVisualFlags;
extern int g_optionsBackupGamma;
extern int g_optionsBackupFxVolume;
extern int g_optionsBackupMusicVolume;
// The saved flags word: 45d280 reads it as an int, 45fc60 as a short.
extern union {
    int i;
    unsigned short s;
} g_optionsBackupMusicMode;
extern char g_optionsBackupCdMode;
extern unsigned char g_optionsBackupUnitChat;
extern char g_optionsBackupUnitChatText;
// A byte in the original: declared unsigned int to keep it in bl for the bitfield merge.
extern unsigned int g_optionsBackupSoundFlags;
extern int g_optionsBackupDisplayWidth;
extern int g_optionsBackupDisplayHeight;
extern int g_optionsBackupTextScroll;
extern int g_optionsBackupTextLines;
// The saved game-speed word: 45ead0 reads it as a short, 45fc60 as an int.
extern union {
    unsigned short s;
    int i;
} g_optionsBackupGameSpeed;
// The saved option byte: 460160 stores it as an int.
extern union {
    unsigned char b;
    int i;
} g_optionsBackupEdgeScroll;
extern char g_optionsBackupTrackTypes[];
extern int g_optionsBackupLockedTrack;
extern int g_musicUiSelectedTrack;
extern int g_optionsShellActive;
extern Class_004c6a60* g_optionsFlipSurface;
extern int g_optionsLightbarX;
extern int g_optionsLightbarAnim;
extern Class_004c6a60* g_optionsBackupSurface;
extern int g_battleQuitIntent;
// Flags live in this struct, not a standalone global: keeps the load order.
extern Settings_45cde0 g_optionsPrefsSnapshot;

void __stdcall MarkChanged(Menu_0045b800* menu);
void __stdcall SetGadgetActiveByName(Menu_0045b800* menu, char* name, int value);
char* __stdcall FUN_004a0180(Entry_0045b800* entries, char* name);
Entry_0045b800* __stdcall FUN_004a0200(Entry_0045b800* entries, char* name);
void __stdcall SetTranslatedTextByName(Menu_0045b800* obj, char* name, char* text, int param_4);
void __stdcall SetGrayedOut(Menu_0045b800* menu, int index, int value);
void __stdcall SetGrayedOutByName(Menu_0045b800* obj, char* name, int value);
void __stdcall SetGadgetGrayedOutByName(Menu_0045b800* obj, char* name, int param_3);
int __stdcall SetButtonStageByName(Menu_0045b800* obj, char* name, int value);
void __stdcall SetBrightness(float value);
void __stdcall SetGameSpeed(unsigned int param1, int param2);
int __stdcall FindGadgetIndex(Entry_0045b800* entries, char* name, int type);
void __stdcall SetGadgetStatus(Menu_0045b800* menu, int index, short value);
void __stdcall GetGadgetName(Entry_0045b800* entries, char* name, int index);
Entry_0045b800* __stdcall FindGadgetOrNull(Entry_0045b800* entries, char* name);
void RestoreSoundOptions();

void __stdcall RenderLayer(Sub_0045cf60* sub, int value);
void __stdcall RenderLayer(void* obj, int n);
void __stdcall RenderLayer(Menu_0045de30* obj, int value);
void __stdcall RenderLayer(Menu_0045e5e0* menu, int value);
void __stdcall RenderLayer(Layer_0045f1d0* menu, int flag);
void __stdcall RenderLayer(Sub_0045f800* sub, int value);
void __stdcall RenderLayer(Sub_0045fb30* sub, int value);
void __stdcall RenderLayer(Menu_00460160* menu, int value);
void __stdcall RenderLayer(Sub_00460680* sub, int value);
void __stdcall RenderLayer(Sub_004608b0* sub, int value);
void __stdcall RenderLayer(Sub_004609b0* sub, int value);
void __stdcall RenderLayer(Gui_00460cc0* sub, int value);
void __stdcall RenderLayer(char* menu, int value);

void __stdcall MarkChanged(Sub_0045cf60* sub);
void __stdcall MarkChanged(Dialog* obj);
void __stdcall MarkChanged(void* obj);
void __stdcall MarkChanged(Menu_0045de30* obj);
void __stdcall MarkChanged(Gui_0045e100* gui);
void __stdcall MarkChanged(Gadget_0045f770* gadget);
void __stdcall MarkChanged(Sub_0045f8c0* sub);
void __stdcall MarkChanged(char* menu);

void __stdcall BlitMenuLayers(Sub_0045cf60* sub, unsigned int* a, int* b);
void __stdcall BlitMenuLayers(char* menu, int a, int b);

void __stdcall SetOffscreenSurface(void* p);
void __stdcall SetOffscreenSurface(int param_1);
void FlipScreen();

int __stdcall LoadGuiLayer(void* obj, char* buf, int size);
int __stdcall LoadGuiLayer(Menu_0045de30* obj, char* name, int size);
Layer_0045e5e0* __stdcall LoadGuiLayer(Menu_0045e5e0* menu, char* name, int flags);
Layer_0045f1d0* __stdcall LoadGuiLayer(Layer_0045f1d0* menu, const char* name, int flags);
Info_0045f800* __stdcall LoadGuiLayer(Sub_0045f800* sub, const char* name, int flags);
Gadget_0045fb30* __stdcall LoadGuiLayer(Sub_0045fb30* sub, const char* name, int flags);
Dialog_004604a0* __stdcall LoadGuiLayer(Menu_004604a0* menu, const char* name, int flags);
Gadget_00460680* __stdcall LoadGuiLayer(Sub_00460680* sub, const char* name, int flags);
Dialog_004608b0* __stdcall LoadGuiLayer(Sub_004608b0* sub, const char* name, int flags);
Info_004609b0* __stdcall LoadGuiLayer(Sub_004609b0* sub, const char* name, int flags);
Gadget_00460cc0* __stdcall LoadGuiLayer(Gui_00460cc0* sub, const char* name, int flags);

void __stdcall SetGrayedOutByName(void* obj, char* name, int value);
void __stdcall SetGrayedOutByName(Dialog* obj, char* name, int value);
void __stdcall SetGrayedOut(Gui_00460cc0* sub, int index, int value);
int __stdcall GetGadgetText(void* settings, const char* key, char* out);

void __stdcall EnableKeyCommands(void* obj);
void __stdcall EnableKeyCommands(Gui_00460cc0* sub);
void __stdcall DisableKeyCommands(Sub_004609b0* sub);
void __stdcall SetKeyboardInput(void* obj, int value);
void __stdcall SetKeyboardInput(Layer_0045f1d0* menu, int flag);
void __stdcall SetKeyboardInput(Menu_00460160* menu, int value);
void __stdcall SetKeyboardInput(Menu_004604a0* menu, int value);
void __stdcall SetKeyboardInput(Sub_00460680* sub, int value);
void __stdcall SetKeyboardInput(Sub_004608b0* sub, int value);
void __stdcall SetKeyboardInput(Sub_004609b0* sub, int value);
void __stdcall SetKeyboardInput(Gui_00460cc0* sub, int value);

int __stdcall SetButtonStageByName(void* obj, char* name, int value);
int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
void __stdcall SetGadgetActiveByName(void* obj, char* name, int value);
void __stdcall SetGadgetActiveByName(Object_004a0570* obj, char* name, int value);
void __stdcall SetGadgetActiveByName(Menu_0045e5e0* menu, char* name, int value);
void __stdcall SetGadgetActiveByName(Sub_004608b0* sub, const char* name, int value);
void __stdcall SetGadgetGrayedOutByName(void* obj, char* name, int value);
void __stdcall SetGadgetGrayedOutByName(Object_004a1450* obj, char* name, int value);
void __stdcall SetTranslatedTextByName(void* obj, char* name, char* text, int value);
void __stdcall SetTranslatedTextByName(Menu_004604a0* menu, const char* name, int value, int count);
void __stdcall SetTranslatedTextByName(Sub_00460680* sub, const char* name, const char* text, int param_4);
void __stdcall SetTranslatedTextByName(Sub_004608b0* sub, const char* name, int value, int param_4);
void __stdcall SetTranslatedTextByName(Gui_00460cc0* sub, const char* name, const char* value, int flags);

int __stdcall IsCurrentGadgetNamed(void* obj, char* name);
int __stdcall IsCurrentGadgetNamed(Gui_0045e100* gui, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_0045ead0* gadget, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_0045f190* gadget, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_0045f770* gadget, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_0045fac0* gadget, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_00460340* gadget, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_00460800* gadget, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_004609b0* gadget, char* name);

int __stdcall GetButtonStageByName(void* obj, char* name);
int __stdcall GetButtonStageByName(Gui_0045e100* gui, char* name);
int __stdcall GetButtonStageByName(Gadget_0045ead0* gadget, char* name);
int __stdcall GetButtonStageByName(Gadget_0045fac0* gadget, char* name);
int __stdcall GetButtonStageByName(Gadget_00460340* gadget, char* name);

void __stdcall PlaySoundByName(char* name, int value);
void __stdcall PlayLoopingSoundByName(char* name, int value);
void __stdcall PlaySoundFile(char* name);
void __stdcall StopAllSounds();
void __stdcall ClearSelectedGadget(void* obj);
void __stdcall ClearSelectedGadget(Gui_0045e100* gui);
void __stdcall ClearSelectedGadget(Gadget_0045ead0* gadget);
void __stdcall ClearSelectedGadget(Gadget_0045f190* gadget);
void __stdcall ClearSelectedGadget(Gadget_0045f770* gadget);
void __stdcall ClearSelectedGadget(Gadget_0045fac0* gadget);
void __stdcall ClearSelectedGadget(Gadget_0045fc60* gadget);
void __stdcall ClearSelectedGadget(Gadget_004605c0* gadget);
void __stdcall ClearSelectedGadget(Gadget_00460800* gadget);
void __stdcall ClearSelectedGadget(Gadget_004609b0* gadget);

void __stdcall CloseTopScreen(void* queue);
void __stdcall CloseTopScreen(Gui_0045e100* gui);
void __stdcall CloseTopScreen(Gadget_0045ead0* gadget);
void __stdcall CloseTopScreen(Gadget_00460800* gadget);
void __stdcall SetGadgetStatusByName(void* obj, char* name, int value);
void __stdcall SetGadgetStatusByName(Menu_0045e5e0* menu, char* name, int value);
void __stdcall SetGadgetStatus(Gadget_0045fc60* gadget, int id, int flag);
void __stdcall SetButtonStage(Menu_0045e5e0* menu, int index, int value);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
int __stdcall LockScreen(Surface_0045fbc0* out);
void __stdcall FillSurface(Surface_0045fbc0* surface, int color);
int __stdcall UnlockScreen(Surface_0045fbc0* s);

int __stdcall FindGadgetIndex(Entry_0045d280* entries, char* name, int type);
int __stdcall FindGadgetIndex(void* list, char* name, int flag);
int __stdcall FindGadgetIndex(Entry_0045de30* entries, char* name, int type);
int __stdcall FindGadgetIndex(Entry_0045e5e0* entries, char* name, int type);
int __stdcall FindGadgetIndex(Entry_0045ed50* entries, char* name, int type);
int __stdcall FindGadgetIndex(Entry_0045f800* info, const char* name, int type);
int __stdcall FindGadgetIndex(Gadget_004604a0* gadgets, const char* name, int flag);
int __stdcall FindGadgetIndex(void* entries, const char* name, int type);
int __stdcall FindGadgetIndex(Entry_004609b0* info, const char* name, int type);
int __stdcall FindGadgetIndex(Entry_00460cc0* entries, const char* name, int type);

char* __stdcall FUN_004a0180(Entry_0045e5e0* entries, char* name);
Entry_0045d7c0* __stdcall FUN_004a0200(Entry_0045d7c0* list, char* name);
Entry_0045de30* __stdcall FUN_004a0200(Entry_0045de30* entries, char* name);
Entry_0045e5e0* __stdcall FUN_004a0200(Entry_0045e5e0* entries, char* name);
Entry_0045ed50* __stdcall FUN_004a0200(Entry_0045ed50* entries, char* name);

void __stdcall LoadPictureCached(char* name, int a, int b, int c);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall AddTextGadget(Layer_0045f8c0* layer, char* type, char* text, int x, int y,
                            int width, int attr);
void __stdcall AddTextGadget(Layer_0045f1d0* layer, char* type, char* text, int x, int y,
                             int width, int attr);
char* __stdcall SkipTextLines(char* text, int n);
char* __stdcall Translate(char* text);
const char* __stdcall Translate(const char* text);
void __stdcall InitBriefingText();
void __stdcall FreeBlinkWords(int param_1);
void __stdcall AllocBlinkWords(Sub_0045f800* sub, int value);
void __stdcall AllocBlinkWords(Sub_004609b0* sub, int value);
void DrawHelpPage();

int __stdcall HandleBriefingClick(void* gadget);
int __stdcall HandleHelpClick(void* gadget);
void __stdcall FillHelpPage(Sub_0045fb30* sub, int a, int b);
void __stdcall FillHelpPage(Gadget_0045fac0* gadget, int a, int b);
int __stdcall FillHelpPage(Sub_004609b0* sub, int a, int b);
void __stdcall HandleVisualOptionsClick(void* layer);
void __stdcall HandleVideoModeSlider(void* obj, int value);
void __stdcall HandleGammaSlider(void* obj, int value);
void __stdcall HandleMusicOptionsClick(int value);
void __stdcall HandleMusicVolumeSlider(void* obj, int value);
void __stdcall HandleEffectsVolumeSlider(void* obj, int arg);
void __stdcall HandleSoundOptionsClick(void* obj, int arg);
void __stdcall HandleSpeedOptionsClick(void* obj, int arg);
void __stdcall HandleGameSpeedSlider(void* obj, int arg);
void __stdcall HandleScreenSlider(void* obj, int arg);
void __stdcall HandleMaxLinesSlider(void* obj, int arg);
void __stdcall HandleTextScrollSlider(void* obj, int arg);
void __stdcall HandleGameSettingsDialogClick(void*);
void __stdcall HandleRestartDialogClick(void* dialog);
void __stdcall HandleSurrenderChoice(void* gadget);
void __stdcall HandleExitMenuClick(void* gadget);
int __stdcall HandleOptionsPanelClick(void* gadget);
void __stdcall HandleInGameOptionsClick(void* gadget);
void ShowLoadGameScreen();
void ShowSaveGameScreen();

void __cdecl FUN_004d85a0(void* p);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall IsScreenNamed(Gui_0045e100* gui, const char* name);
void __stdcall SortDisplayModes(List_0045e5e0* list);
int __stdcall GetDisplayModes(List_0045e5e0* list);
int FindHostSlot();
char* __cdecl _itoa(int value, char* buf, int radix);
char* __stdcall WordWrapText(Menu_004604a0* menu, char* text, int player);
int __stdcall SetCursorMode(int value);
void ApplyDifficultyButtons();
char __stdcall FindGameCdDrive(int disc);
void __stdcall RegisterDataArchives();
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
int __stdcall IsGadgetNamed(int param1, int param2, char* name);
void ShutdownIngameSystems();
void ShutdownIngameAndQuit();
int __stdcall PopUntilNamedLayout(int force);
void BlankScreen();
void __stdcall SetGameMode(int a);
void __stdcall SelectGadgetByName(Sub_00460680* sub, const char* name);
void __stdcall FrameFromSurface(Dst_004b8ae0* dst, Class_004c6a60* src);
Class_004c6a60* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall DrawSurface(int a, void* surface, int b, int c);
void __stdcall DrawSurface(Class_004c6a60* surface, int a, int b, int c);
void __stdcall FreeSurface(void* surface);
void __stdcall FreeSurface(Class_004c6a60* surface);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrame(int a, void* b, int c, int d);
void __stdcall DrawFrameQuad(void* surf, void* entry, Quad_45ffb0* dst, Quad_45ffb0* src);

void __stdcall UpdateTrackGadgets();
void __stdcall OpenMusicOptions();
void __stdcall OpenSoundOptions();
void __stdcall OpenVisualOptions(int param_1);
void __stdcall OpenSpeedOptions();
void __stdcall OpenExitMenu();
void __stdcall EnsureOptionsPanelGadget();
void RestoreVisualOptions();
void OrLabelAttribs();
void SaveSettings();

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
void __stdcall SetHostGameName(int flag, char* text)
{
    DAT_00512c84 = flag != 0;
    if (text) {
        DAT_00512ca8[0] = 0;
        strncat(DAT_00512ca8, text, 0x3f);
    }
}

// FUNCTION: 0x45b860
void __stdcall SetDirectPlayProvider(int param_1)
{
    if (param_1 == 1 || param_1 == 2 || param_1 == 3 || param_1 == 4) {
        DAT_00512c80 = param_1;
    }
}

// FUNCTION: 0x45b880
void __stdcall SetGadgetsDisabledByPrefix(char* name, int value)
{
    for (int i = 0; i <= g_game->menu.holder->entries->count; i++) {
        if (strncmp(g_game->menu.holder->entries[i].name, name, strlen(name)) == 0 &&
            g_game->menu.holder->entries[i].type == 1) {
            SetGrayedOut(&g_game->menu, i, value);
        }
    }
    MarkChanged(&g_game->menu);
}

// FUNCTION: 0x45b920
void __stdcall DeactivateGadgetsByPrefix(char* prefix)
{
    for (int i = 0; i <= g_game->menu.holder->entries[0].count; i++) {
        if (strncmp(g_game->menu.holder->entries[i].name, prefix, strlen(prefix)) == 0) {
            SetGadgetActiveByName(&g_game->menu, g_game->menu.holder->entries[i].name, 0);
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
void __stdcall SetGadgetStatusAndText(int index, int state, char* offText, char* onText)
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
void __stdcall ClearGadgetText(void* param_1, int param_2)
{
    *(char*)((char*)param_1 + 0xb6) = 0;
}

// The VIDEOVAL menu entry holds the current resolution text. The mode list
// is searched for the mode matching the current screen size (width compared
// against a local read before the loop, height re-read every iteration), and
// the index of that mode is turned into a step index in pos, the same
// arithmetic as SetSliderFromValue.
// FUNCTION: 0x45bb00
void __stdcall UpdateVideoModeLabel(Menu_0045b800* param_1, Entry_0045b800* param_2)
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
    MarkChanged(&g_game->menu);
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
// original name and label (not the formatted text) to SetTranslatedTextByName on the
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
    SetTranslatedTextByName(&g_game->menu, name, label, 0);
}

// FUNCTION: 0x45c010
void __stdcall SetGadgetTextFromValueTable(Entry_0045c010* table, char* name, int value)
{
    char buf[100];
    if (table->format == 0)
        return;
    while (table->format != 0) {
        if (value <= table->min) {
            sprintf(buf, table->format, value);
            SetTranslatedTextByName(&g_game->menu, name, buf, 0);
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
    if (player->field_0 == 0 || !player->info->u.flag_9b_6) {
        Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "GAME");
        if (e != 0) {
            // Written twice, as in the original.
            int value = SliderValue(e);
            g_game->field_38a4b = (unsigned short)(value < 1 ? 1 : SliderValue(e));
            SetGameSpeed(g_game->field_38a4b, 1);
            MarkChanged(obj);
        }
    }
}

// Reads the "SCREEN" slider of the menu object and stores its value in
// g_game->scrollSpeed, writing 1 instead of any value of 1 or less, then
// marks the object changed (MarkChanged sets obj->field_cca = 1).
// FUNCTION: 0x45c170
void __stdcall HandleScreenSlider(Menu_0045b800* obj, int unused)
{
    Entry_0045b800* e = FUN_004a0200(obj->holder->entries, "SCREEN");
    if (e != 0) {
        g_game->scrollSpeed = SliderValue(e) > 1 ? SliderValue(e) : 1;
        MarkChanged(obj);
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
        MarkChanged(obj);
    }
    if (g_game->field_37f27 != 0)
        sprintf(text, "%d", g_game->field_37f27);
    else
        strcpy(text, "None");
    SetTranslatedTextByName(obj, "MAXLINESTEXT", text, 0);
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
        SetTranslatedTextByName(obj, "TEXTSCROLLTEXT", text, 0);
        MarkChanged(obj);
    }
}

// FUNCTION: 0x45c3d0
void __stdcall SetLockedCdTrack(int value)
{
    ((Class_004ce580*)g_game->sound)->SetLockedTrack(value);
}

// FUNCTION: 0x45c3f0
void UpdateTrackGadgets()
{
    char buf[12];
    Menu_0045b800* menu = &g_game->menu;

    if (FindGadgetIndex(menu->holder->entries, "TRACKTYPE", 1) != -1) {
        int disc = g_musicUiSelectedTrack;
        SetGrayedOutByName(menu, "TRACKTYPE",
                     ((g_game->f_37f14 & 1) && g_game->field_37f16 == 4) ? 0 : 1);
        SetButtonStageByName(menu, "TRACKTYPE", ((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(disc));
        if (disc == 0)
            strcpy(buf, "NO DISC");
        else
            sprintf(buf, "%d", disc);
        SetTranslatedTextByName(menu, "TRACKNUM", buf, 0);
    }
    SetGadgetGrayedOutByName(menu, "TRACKNUM", (char)(~g_game->f_37f14) & 1);
    if (g_game->field_37f16 == 3) {
        // the track number is re-read from the global here, not taken from disc
        int track = g_musicUiSelectedTrack;
        ((Class_004ce580*)g_game->sound)->SetLockedTrack(track);
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
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(g_musicUiSelectedTrack, gadgets[index].value);
    }
}

// Sets up the sound state: volume1, three sound flag bits, a redraw of the
// sound object, more flag bits, then brightness and the two volume levels
// scaled by 1024 (same tail as 0x45bcc0).
// FUNCTION: 0x45c570
void ApplyDefaultSoundOptions()
{
    g_game->volume1 = 0x1b;
    g_game->soundFlags.bits.b4 = 1;
    g_game->soundFlags.bits.b5 = 1;
    g_game->soundFlags.bits.speech = 1;
    g_game->sound->Disable3D();
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
void ApplyDefaultMusicOptions()
{
    g_game->volume2 = 0x20;
    g_game->field_37f16 = 4;
    if (!(g_game->flags14 & 1)) {
        g_game->flags14 |= 1;
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    ApplyBrightnessAndVolume();
}

// FUNCTION: 0x45c6d0
void ApplyDefaultUiOptions()
{
    g_game->field_37f23 = 10;
    g_game->field_37f27 = 10;
    g_game->field_38a4b = 10;
    g_game->field_38a4d = 10;
    g_game->scrollSpeed = 0x20;
    g_game->field_37efa = 0;
    g_game->field_37f17 = 10;
    g_game->field_37f18 = 5;
}

// Sets the five share flag bits in the flags word at +0x37f06, sets the
// brightness at +0x37f08, and (unless bit 2 of +0x2a44 is set) the screen
// width and height at +0x37f1b/+0x37f1f and clears bit 6 of the flags word.
// Then applies the brightness and the two volume levels to the sound object.
// FUNCTION: 0x45c740
void ApplyDefaultVisualOptions()
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
    g_game->volume2 = g_optionsBackupMusicVolume;
    ((Class_004ce3e0*)g_game->sound)->CopyTrackTypeTable(&g_optionsBackupTrackTypes);
    g_game->field_37f16 = g_optionsBackupCdMode;
    ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
    if (((unsigned char)g_game->flags14 ^ (unsigned char)g_optionsBackupMusicMode.i) & 1) {
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    unsigned short f = g_game->flags14;
    f = f ^ ((f ^ g_optionsBackupMusicMode.i) & 1);
    g_game->flags14 = f;
    ((Class_004ce580*)g_game->sound)->SetLockedTrack(g_optionsBackupLockedTrack);
    ApplyBrightnessAndVolume();
}

// Copies saved option values (globals around 0x512f2c) into the game.
// FUNCTION: 0x45ca50
void RestoreUiOptions()
{
    g_game->field_37f23 = g_optionsBackupTextScroll;
    g_game->field_38a4b = g_optionsBackupGameSpeed.i;
    g_game->field_38a4d = g_optionsBackupGameSpeed.i;
    g_game->scrollSpeed = g_optionsBackupEdgeScroll.i;
    g_game->field_37efa = g_optionsBackupInterfaceType;
    g_game->field_37f17 = g_optionsBackupUnitChat;
    g_game->field_37f18 = g_optionsBackupUnitChatText;
    g_game->field_37f27 = g_optionsBackupTextLines;
}

// Copies six flag bits out of the saved settings value into the flags word at
// +0x37f06, restores the brightness and resolution defaults, then applies the
// brightness and both volume levels to the object at g_game+0x10 (same tail as
// 0x45bcc0).
// FUNCTION: 0x45cae0
void RestoreVisualOptions()
{
    unsigned short v = g_game->flags;
    g_game->flags = v ^ ((v ^ g_optionsBackupVisualFlags) & 2);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ g_optionsBackupVisualFlags) & 4);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ g_optionsBackupVisualFlags) & 8);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ g_optionsBackupVisualFlags) & 0x10);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ g_optionsBackupVisualFlags) & 0x20);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ g_optionsBackupVisualFlags) & 0x40);

    g_game->brightness = g_optionsBackupGamma;
    if (!(g_game->flags_2a44 & 4)) {
        g_game->width = g_optionsBackupDisplayWidth;
        g_game->height = g_optionsBackupDisplayHeight;
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
    RestoreSoundOptions();
    g_game->volume2 = g_optionsBackupMusicVolume;
    ((Class_004ce3e0*)g_game->sound)->CopyTrackTypeTable(&g_optionsBackupTrackTypes);
    g_game->field_37f16 = g_optionsBackupCdMode;
    ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
    if (((unsigned char)g_game->flags14 ^ (unsigned char)g_optionsBackupMusicMode.i) & 1) {
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    unsigned short f = g_game->flags14;
    g_game->flags14 = f ^ ((f ^ g_optionsBackupMusicMode.i) & 1);
    ((Class_004ce580*)g_game->sound)->SetLockedTrack(g_optionsBackupLockedTrack);
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
    g_game->field_37f23 = g_optionsBackupTextScroll;
    g_game->field_38a4b = g_optionsBackupGameSpeed.i;
    g_game->field_38a4d = g_optionsBackupGameSpeed.i;
    g_game->scrollSpeed = g_optionsBackupEdgeScroll.i;
    g_game->field_37efa = g_optionsBackupInterfaceType;
    g_game->field_37f17 = g_optionsBackupUnitChat;
    g_game->field_37f18 = g_optionsBackupUnitChatText;
    g_game->field_37f27 = g_optionsBackupTextLines;
    RestoreVisualOptions();
}

// Saves the current game settings: copies the 0x53-byte block at
// g_game+0x37ee6 into the settings block at g_optionsPrefsSnapshot, saves two game
// flags into bits 0 and 1 of that block's trailing word, then saves the
// track number, the option byte, the current track index and the 100
// track-name characters read from the object at g_game+0x10.
// FUNCTION: 0x45cde0
void SaveGameSettings()
{
    memcpy(g_optionsPrefsSnapshot.block, (char*)g_game + 0x37ee6, 0x53);
    g_optionsPrefsSnapshot.bit0 = g_game->flags14281.bit1;
    g_optionsPrefsSnapshot.bit1 = g_game->flags14281.bit2;
    g_optionsBackupGameSpeed.i = g_game->field_38a4b;
    g_optionsBackupEdgeScroll.i = g_game->scrollSpeed;
    g_optionsBackupLockedTrack = ((Class_004ce5a0*)g_game->sound)->GetLockedTrack();
    for (int i = 0; i < 100; i++) {
        g_optionsBackupTrackTypes[i] = ((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(i);
    }
}

// Looks up the "PANEL" gadget in the menu's entry table (entry 0 holds the
// count as a short at +0xb6). When the game flag at +0x37ebe is set the panel
// layout grows by 0x96, and if there is no PANEL entry yet a cleared one is
// appended: type 0xb, x = 0x80, its width shrunk by x, height copied from the
// table, named "PANEL", and the table's +0xc4 field copied into it.
// FUNCTION: 0x45ce80
void EnsureOptionsPanelGadget()
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

// FUNCTION: 0x45cf60
void BlitOptionsPanel()
{
    if (g_game->bits_2a44.bits.bit2) {
        return;
    }
    RenderLayer((Sub_0045cf60*)&g_game->gui, 0x40);
    MarkChanged((Sub_0045cf60*)&g_game->gui);
    BlitMenuLayers((Sub_0045cf60*)&g_game->gui, 0, 0);
    SetOffscreenSurface(g_game->field_37e1b);
    FlipScreen();
}

// The original called this from other translation units; in this file
// MSVC would inline it into every caller, so keep it out of line.
#pragma auto_inline(off)
// FUNCTION: 0x45cfc0
int __cdecl OpenOptionsLayout()
{
    char buf[256];
    if (g_game->bits_2a44.prefsWord.prefs) {
        strcpy(buf, "PREFS.GUI");
        g_game->flags_37ebe.loadedBits.loaded |= 1;
    } else {
        strcpy(buf, "STARTOPT.GUI");
    }
    int result = LoadGuiLayer(&g_game->gui, buf, 0x80);
    SetGrayedOutByName(&g_game->gui, "MUSIC", *(int*)g_game->sound == 0);
    EnableKeyCommands(&g_game->gui);
    if (g_game->bits_2a44.prefsWord.prefs && g_game->mode->GetGameType() != 3) {
        g_game->flags_38a51 |= 1;
    }
    return result;
}
#pragma auto_inline(on)

// FUNCTION: 0x45d0c0
void TickMusicOptions()
{
    char value[20];
    GetGadgetText(&g_game->gui, "TRACKNUM", value);
    int track = atoi(value);
    if (track != g_game->sound->GetCurrentTrack()) {
        g_musicUiSelectedTrack = g_game->sound->GetCurrentTrack();
        UpdateTrackGadgets();
        MarkChanged((Dialog*)&g_game->gui);
    }
}

// FUNCTION: 0x45d130
void UpdateMusicGadgets()
{
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "NOTRAK", g_game->flags_37f14.notrak & 1);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "TRACKMODE", g_game->field_37f16 - 1);
    SetGadgetGrayedOutByName((Object_004a1450*)&g_game->gui, "MUSICVOL", (char)(~g_game->flags_37f14.notrak & 1));
    SetGrayedOutByName((Dialog*)&g_game->gui, "CDPREV", (char)(~g_game->flags_37f14.notrak & 1));
    SetGrayedOutByName((Dialog*)&g_game->gui, "CDSTOP", (char)(~g_game->flags_37f14.notrak & 1));
    SetGrayedOutByName((Dialog*)&g_game->gui, "CDPLAY", (char)(~g_game->flags_37f14.notrak & 1));
    SetGrayedOutByName((Dialog*)&g_game->gui, "CDNEXT", (char)(~g_game->flags_37f14.notrak & 1));
    SetGrayedOutByName((Dialog*)&g_game->gui, "TRACKMODE", (char)(~g_game->flags_37f14.notrak & 1));
    // Spelled as one negated test, not an if/else with a call in each arm:
    // MSVC then materialises the value in a register instead of pushing 0/1.
    SetGrayedOutByName((Dialog*)&g_game->gui, "TRACKTYPE", !((g_game->flags_37f14.notrak & 1) && g_game->field_37f16 == 4));
}

// FUNCTION: 0x45d280
void __stdcall HandleMusicOptionsClick(Object_0045d280* obj)
{
    Entry_0045d280* entries = obj->holder->entries;
    if (obj->field_60 == -1) {
        if (!g_game->bits_2a44.prefsByte.prefs) {
            ((Class_004ced40*)g_game->sound)->StopCdAudio();
            g_game->flags_37ebe.loadedBits.loaded = 0;
            return;
        }
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
        g_game->flags_37ebe.loadedBits.loaded = 0;
        return;
    }
    MarkChanged(obj);
    // Result kept in a local: testing the call directly is longer.
    int notrak = IsCurrentGadgetNamed(obj, g_notrakGadgetName);
    if (notrak != 0) {              // "NOTRAK"
        PlaySoundByName(g_optionsSoundName, 0);
        int v = GetButtonStageByName(obj, g_notrakGadgetName);
        unsigned short f = g_game->flags_37f14.word;
        g_game->flags_37f14.word = f ^ ((f ^ v) & 1);
        ((Class_004cedc0*)g_game->sound)->EnableCdAudio(g_game->flags_37f14.word & 1);
        ClearSelectedGadget(obj);
        UpdateMusicGadgets();
    } else if (IsCurrentGadgetNamed(obj, g_trackModeGadgetName)) {  // "TRACKMODE"
        PlaySoundByName(g_optionsSoundName, 0);
        g_game->field_37f16 = GetButtonStageByName(obj, g_trackModeGadgetName) + 1;
        ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
        if (g_game->field_37f16 == 3) {
            g_musicUiSelectedTrack = ((Class_004ce5a0*)g_game->sound)->GetLockedTrack();
            ClearSelectedGadget(obj);
            UpdateTrackGadgets();
            return;
        }
        if (g_game->field_37f16 == 4) {
            SetButtonStageByName(obj, g_trackTypeGadgetName, (unsigned char)((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(g_musicUiSelectedTrack));
            ApplyTrackType();
        }
        ClearSelectedGadget(obj);
        UpdateTrackGadgets();
        return;
    } else if (IsCurrentGadgetNamed(obj, g_trackTypeGadgetName)) {  // "TRACKTYPE"
        PlaySoundByName(g_optionsSoundName, 0);
        // Index with obj->field_60 itself, not the saved copy.
        int i = obj->field_60;
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(g_musicUiSelectedTrack, entries[i].value);
        UpdateTrackGadgets();
        ClearSelectedGadget(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, g_cdPlayGadgetName)) {  // "CDPLAY"
        PlaySoundByName(g_optionsSoundName, 0);
        g_game->sound->PlayCdTrack(g_musicUiSelectedTrack, 1);
        ClearSelectedGadget(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, g_cdNextGadgetName)) {  // "CDNEXT"
        PlaySoundByName(g_optionsSoundName, 0);
        g_musicUiSelectedTrack = g_musicUiSelectedTrack + 1;
        int n = ((Class_004ce450*)g_game->sound)->GetTrackCount();
        if (g_musicUiSelectedTrack > n)
            g_musicUiSelectedTrack = 1;
        g_musicUiSelectedTrack = ((Class_004ce8c0*)g_game->sound)->SelectTrack(g_musicUiSelectedTrack);
        UpdateTrackGadgets();
        ClearSelectedGadget(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, g_cdPrevGadgetName)) {  // "CDPREV"
        PlaySoundByName(g_optionsSoundName, 0);
        g_musicUiSelectedTrack = g_musicUiSelectedTrack - 1;
        if (g_musicUiSelectedTrack < 1)
            g_musicUiSelectedTrack = ((Class_004ce450*)g_game->sound)->GetTrackCount();
        g_musicUiSelectedTrack = ((Class_004ce8c0*)g_game->sound)->SelectTrack(g_musicUiSelectedTrack);
        UpdateTrackGadgets();
        ClearSelectedGadget(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, g_cdStopGadgetName)) {  // "CDSTOP"
        PlaySoundByName(g_optionsSoundName, 0);
        ((Class_004ced40*)g_game->sound)->StopCdAudio();
        g_musicUiSelectedTrack = ((Class_004ce8c0*)g_game->sound)->SelectTrack(1);
        UpdateTrackGadgets();
        ClearSelectedGadget(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, g_undoGadgetName)) {  // "UNDO"
        PlaySoundByName(g_optionsSoundName, 0);
        LoadSavedAudioSettings();
        CloseTopScreen(obj);
        OpenMusicOptions();
        return;
    }
    if (IsCurrentGadgetNamed(obj, g_restoreGadgetName)) {  // "RESTORE"
        PlaySoundByName(g_optionsSoundName, 0);
        ApplyDefaultMusicOptions();
        CloseTopScreen(obj);
        OpenMusicOptions();
        return;
    }
    int save = obj->field_60;
    if (obj->field_60 != -1) {
        if (entries[obj->field_60].state != 1) {
            ClearSelectedGadget(obj);
            return;
        }
        Vtable_0045d280* p = obj->holder->field_0;
        CloseTopScreen(obj);
        obj->field_60 = save;
        p->FUN_8(obj);
    }
}

// FUNCTION: 0x45d7c0
void OpenMusicOptions()
{
    Object_0045d7c0* obj = (Object_0045d7c0*)OpenOptionsLayout();
    RenderLayer(&g_game->gui, 2);
    EnsureOptionsPanelGadget();
    if (g_game->flags_37ebe.byte & 1) {
        LoadGuiLayer(&g_game->gui, "MUSICRT.GUI", 0x280);
    } else {
        LoadGuiLayer(&g_game->gui, "MUSIC", 0x200);
        LoadPictureCached("optmusic4x", 0, 0, 0);
    }
    obj->callback8 = HandleMusicOptionsClick;
    EnableKeyCommands(&g_game->gui);
    obj->callback1c = TickMusicOptions;
    SetGadgetStatusByName(&g_game->gui, "MUSIC", 1);
    if (FindGadgetIndex(obj->gadgets, "MUSICVOL", 0xe) != -1) {
        Entry_0045d7c0* e = FUN_004a0200(obj->gadgets, "MUSICVOL");
        e->max = 0x40;
        e->callback = HandleMusicVolumeSlider;
        e->pos = g_game->volume2Word;
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
    UpdateMusicGadgets();
    if (g_game->field_37f16 == 3) {
        g_musicUiSelectedTrack = ((Class_004ce5a0*)g_game->sound)->GetLockedTrack();
    }
    UpdateTrackGadgets();
    Gadget_0045d7c0* gadgets = ((Holder_0045d7c0*)g_game->gui.holder)->gadgets;
    if (g_game->field_37f16 == 4) {
        int index = FindGadgetIndex(gadgets, "TRACKTYPE", 1);
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(g_musicUiSelectedTrack, gadgets[index].value);
    }
    MarkChanged(&g_game->gui);
    SetKeyboardInput(&g_game->gui, 1);
    OrLabelAttribs();
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x45d9d0
void UpdateSoundGadgets()
{
    SetButtonStageByName((Class_004a1080*)&g_game->gui, g_modeGadgetName, g_game->soundFlags.byte & 7);
    SetGadgetActiveByName((Object_004a0570*)&g_game->gui, g_volTextGadgetName, (g_game->soundFlags.byte & 7) != 0);
    SetGadgetGrayedOutByName((Object_004a1450*)&g_game->gui, g_fxVolGadgetName, (g_game->soundFlags.byte & 7) == 0);
    SetGadgetGrayedOutByName((Object_004a1450*)&g_game->gui, g_testGadgetName, (g_game->soundFlags.byte & 7) == 0);
    SetGadgetGrayedOutByName((Object_004a1450*)&g_game->gui, g_speechGadgetName, (g_game->soundFlags.byte & 7) == 0);
}

// FUNCTION: 0x45da90
void __stdcall HandleSoundOptionsClick(Object_0045da90* obj)
{
    Entry_0045da90* entries = obj->holder->entries;
    if (obj->field_60 == -1) {
        g_game->flags_37ebe.loadedBits.loaded = 0;
        return;
    }
    MarkChanged(obj);
    int mode = obj->field_60;
    if (IsCurrentGadgetNamed(obj, g_speechGadgetName)) {
        PlaySoundByName(g_optionsSoundName, 0);
        g_game->soundFlags.bits.speech = entries[obj->field_60].value != 0;
        g_game->field_37f17 = entries[obj->field_60].value * 5;
        ClearSelectedGadget(obj);
    } else if (IsCurrentGadgetNamed(obj, g_modeGadgetName)) {
        int v = GetButtonStageByName(obj, g_modeGadgetName);
        unsigned short f = g_game->soundFlags.word;
        g_game->soundFlags.word = f ^ ((f ^ v) & 7);
        if ((g_game->soundFlags.word & 7) == 0)
            StopAllSounds();
        if ((g_game->soundFlags.word & 7) == 2)
            g_game->sound->Enable3D();
        else
            g_game->sound->Disable3D();
        if ((g_game->soundFlags.word & 7) == 1 && !g_game->bits_2a44.prefsWord.prefs)
            PlayLoopingSoundByName(g_bgmSoundName, 0);
        SetButtonStageByName(&g_game->gui, g_modeGadgetName, g_game->soundFlags.word & 7);
        SetGadgetActiveByName(&g_game->gui, g_volTextGadgetName, (g_game->soundFlags.word & 7) != 0);
        SetGadgetGrayedOutByName(&g_game->gui, g_fxVolGadgetName, (g_game->soundFlags.word & 7) == 0);
        SetGadgetGrayedOutByName(&g_game->gui, g_testGadgetName, (g_game->soundFlags.word & 7) == 0);
        SetGadgetGrayedOutByName(&g_game->gui, g_speechGadgetName, (g_game->soundFlags.word & 7) == 0);
        ClearSelectedGadget(obj);
        PlaySoundByName(g_optionsSoundName, 0);
        return;
    }
    if (IsCurrentGadgetNamed(obj, g_undoGadgetName)) {
        RestoreSoundOptions();
        CloseTopScreen(obj);
        OpenSoundOptions();
        PlaySoundByName(g_optionsSoundName, 0);
        return;
    }
    if (IsCurrentGadgetNamed(obj, g_restoreGadgetName)) {
        g_game->volume1 = 0x1b;
        g_game->soundFlags.bits.b4 = 1;
        g_game->soundFlags.bits.b5 = 1;
        g_game->soundFlags.bits.speech = 1;
        g_game->sound->Disable3D();
        g_game->soundFlags.word = (g_game->soundFlags.word & 0xfff9) | 1;
        g_game->field_37f17 = 10;
        SetBrightness(0.5 - g_game->brightness * -0.041666668f);
        ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
        ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
        CloseTopScreen(obj);
        OpenSoundOptions();
        PlaySoundByName(g_optionsSoundName, 0);
        return;
    }
    if (IsCurrentGadgetNamed(obj, g_testGadgetName)) {
        PlaySoundFile(g_explodeSoundFile);
        ClearSelectedGadget(obj);
        return;
    }
    if (obj->field_60 != -1) {
        if (entries[mode].type != 1) {
            ClearSelectedGadget(obj);
            return;
        }
        Vtable_0045da90* p = obj->holder->field_0;
        CloseTopScreen(obj);
        obj->field_60 = mode;
        p->FUN_8(obj);
    }
}

// FUNCTION: 0x45de30
void OpenSoundOptions()
{
    Object_0045de30* obj = (Object_0045de30*)OpenOptionsLayout();
    RenderLayer(&g_game->gui, 2);
    EnsureOptionsPanelGadget();
    if (g_game->flags_37ebe.byte & 1) {
        LoadGuiLayer(&g_game->gui, "SOUNDSRT.GUI", 0x200);
    } else {
        LoadGuiLayer(&g_game->gui, "SOUNDS", 0x200);
        LoadPictureCached("optsound4x", 0, 0, 0);
    }
    Entry_0045de30* entries = obj->entries;
    obj->fn = HandleSoundOptionsClick;
    EnableKeyCommands(&g_game->gui);
    SetGadgetStatusByName(&g_game->gui, "SOUND", 1);
    // Result in a local; the call reads obj->entries while later calls use entries.
    int found = FindGadgetIndex(obj->entries, "FXVOL", 0xe);
    if (found != -1) {
        Entry_0045de30* e = FUN_004a0200(entries, "FXVOL");
        e->max = 0x40;
        e->fn = HandleEffectsVolumeSlider;
        e->pos = g_game->volume1Word;
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
            obj->entries[i].fn(&g_game->gui, 0);
    }
    SetButtonStageByName(&g_game->gui, "SPEECH", g_game->soundFlags.bits.speech ? g_game->field_37f17 / 5 : 0);
    SetButtonStageByName(&g_game->gui, "MODE", g_game->soundFlags.word & 7);
    SetGadgetActiveByName(&g_game->gui, "VOLTEXT", (g_game->soundFlags.word & 7) != 0);
    SetGadgetGrayedOutByName(&g_game->gui, "FXVOL", (g_game->soundFlags.word & 7) == 0);
    SetGadgetGrayedOutByName(&g_game->gui, "TEST", (g_game->soundFlags.word & 7) == 0);
    SetGadgetGrayedOutByName(&g_game->gui, "SPEECH", (g_game->soundFlags.word & 7) == 0);
    MarkChanged(&g_game->gui);
    SetKeyboardInput(&g_game->gui, 1);
    OrLabelAttribs();
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x45e100
void __stdcall HandleVisualOptionsClick(Gui_0045e100* gui)
{
    int save = gui->field_60;
    Screen_0045e100* top = gui->top;
    Entry_0045e100* entries = top->entries;
    FreeObj_0045e100* obj = (FreeObj_0045e100*)top->field_c;

    if (gui->field_60 == -1) {
        if (obj) {
            if (!g_game->flags_37ebe.bits.b0) {
                FUN_004d85a0(obj->field_14);
                FUN_004d85a0(obj->field_4);
            }
            FUN_004d85a0(obj);
            gui->top->field_c = 0;
        }
        g_game->flags_37ebe.bits.b0 = 0;
        return;
    }

    if (IsCurrentGadgetNamed(gui, "ANTI")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.bits.b1 = GetButtonStageByName(gui, "ANTI") & 1;
        if (g_game->flags_37ebe.bits.b0)
            g_game->ptr_1437b->FlushCache();
        MarkChanged(gui);
        ClearSelectedGadget(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "BSHADOWS")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.bits.b4 = GetButtonStageByName(gui, "BSHADOWS") & 1;
        g_game->flags_37f06.bits.b3 = g_game->flags_37f06.bits.b4;
        g_game->flags_37f06.bits.b2 = g_game->flags_37f06.bits.b3;
        if (g_game->flags_37ebe.bits.b0)
            g_game->ptr_1437b->FlushCache();
        MarkChanged(gui);
        ClearSelectedGadget(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "SHADING")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.bits.b5 = GetButtonStageByName(gui, "SHADING") & 1;
        if (g_game->flags_37ebe.bits.b0)
            g_game->ptr_1437b->FlushCache();
        MarkChanged(gui);
        ClearSelectedGadget(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "UNDO")) {
        PlaySoundByName("Options", 0);
        RestoreVisualOptions();
        CloseTopScreen(gui);
        OpenVisualOptions(0);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "RESTORE")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.bits.b1 = 1;
        g_game->flags_37f06.bits.b2 = 1;
        g_game->flags_37f06.bits.b3 = 1;
        g_game->flags_37f06.bits.b4 = 1;
        g_game->flags_37f06.bits.b5 = 1;
        g_game->brightness = 12;
        if (!g_game->bits_2a44.flag.flag) {
            g_game->width = 640;
            g_game->height = 480;
            g_game->flags_37f06.bits.b6 = 0;
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
            ClearSelectedGadget(gui);
            return;
        }
        Screen_0045e100* next = gui->top->next;
        CloseTopScreen(gui);
        gui->field_60 = save;
        next->handler(gui);
    }
}

// FUNCTION: 0x45e4c0
void __stdcall SortDisplayModes(ModeList_0045e4c0* list)
{
    // j first: the merged file's declaration context needs this order.
    int j, i;
    for (i = 0; i < list->count; i++) {
        for (j = list->count - 1; j > i; j--) {
            if (list->modes[j].width < list->modes[i].width
                || (list->modes[j].width == list->modes[i].width
                    && list->modes[j].height < list->modes[i].height)) {
                Mode_0045e4c0 tmp = list->modes[i];
                list->modes[i] = list->modes[j];
                list->modes[j] = tmp;
            }
        }
    }
    for (i = 0; i < list->count; i++) {
        if (list->modes[i].width < 640 || list->modes[i].height < 480) {
            if (i < list->count - 1) {
                for (j = i; j < list->count - 1; j++)
                    list->modes[j] = list->modes[j + 1];
            }
            i--;
            list->count--;
        }
    }
}

// FUNCTION: 0x45e5e0
void __stdcall OpenVisualOptions(int param_1)
{
    // Declared up front: esi holds this zero for the later args and stores.
    int i = 0;
    Layer_0045e5e0* layer;
    Menu_0045e5e0* menu;

    if (param_1 != i) {
        layer = LoadGuiLayer((Menu_0045e5e0*)&g_game->gui, "SELVMODE.GUI", 0x800);
    } else {
        layer = (Layer_0045e5e0*)OpenOptionsLayout();
        RenderLayer(&g_game->gui, 2);
        EnsureOptionsPanelGadget();
        if (g_game->flags_37ebe.byte & 1) {
            LoadGuiLayer(&g_game->gui, "VISUALRT.GUI", 0x200);
        } else {
            LoadGuiLayer(&g_game->gui, "VISUALS.GUI", 0x200);
            LoadPictureCached("optvisual4x", i, i, i);
        }
    }
    EnableKeyCommands(&g_game->gui);
    layer->handler = HandleVisualOptionsClick;

    if (!(g_game->flags_37ebe.byte & 1)) {
        List_0045e5e0* list = (List_0045e5e0*)FUN_004d83b0("SELECT VIDEO MODE", 0x20);
        layer->data = list;
        list->modes = (Mode_0045e4c0*)FUN_004d83b0("DISPLAY MODES", 0x4b0);
        if (GetDisplayModes(list) != 0) {
            SortDisplayModes(list);
            list->buffer = (char*)FUN_004d83b0("AVAILABLE MODES", list->count << 8);
            list->buffer[0] = 0;
            if (FindGadgetIndex(layer->entries, "VIDSLDR", 0xe) != -1) {
                Entry_0045e5e0* e = FUN_004a0200(layer->entries, "VIDSLDR");
                e->max = list->count - 1;
                e->fn = HandleVideoModeSlider;
                e->data = list;
                menu = (Menu_0045e5e0*)&g_game->gui;
                for (int j = 0; j < list->count; j++) {
                    // Computed inside the loop body: hoisted after the count guard.
                    int w = g_game->width;
                    Mode_0045e4c0* mode = &list->modes[j];
                    if (w == mode->width && g_game->height == mode->height) {
                        int max = e->max;
                        int value = j;
                        if (value > max)
                            value = max;
                        float f = (float)value / (float)max * (float)(e->steps - 1);
                        if (f - (int)f != 0.0f)
                            f += 1.0;
                        e->pos = (short)f;
                        char* p = FUN_004a0180(menu->holder->entries, "VIDVAL");
                        if (p != 0) {
                            sprintf(p + 0xb6, "%d X %d", mode->width, mode->height);
                        }
                        break;
                    }
                }
            }
        }
    } else {
        layer->data = (void*)i;
        // Redundant test: keeps the MAP/VID loops in this shape.
        if (g_game->flags_37ebe.byte & 1) {
            for (i = 0; i <= ((Holder_0045e5e0*)g_game->gui.holder)->entries->count; i++) {
                if (strncmp(((Holder_0045e5e0*)g_game->gui.holder)->entries[i].name, "MAP", strlen("MAP")) == 0) {
                    SetGadgetActiveByName(&g_game->gui, ((Holder_0045e5e0*)g_game->gui.holder)->entries[i].name, 0);
                }
            }
            for (i = 0; i <= ((Holder_0045e5e0*)g_game->gui.holder)->entries->count; i++) {
                if (strncmp(((Holder_0045e5e0*)g_game->gui.holder)->entries[i].name, "VID", strlen("VID")) == 0) {
                    SetGadgetActiveByName(&g_game->gui, ((Holder_0045e5e0*)g_game->gui.holder)->entries[i].name, 0);
                }
            }
        }
    }

    if (param_1 == 0) {
        SetGadgetStatusByName(&g_game->gui, "VISUALS", 1);
        int found = FindGadgetIndex(layer->entries, "ANTI", 1);
        if (found != -1) {
            SetButtonStage((Menu_0045e5e0*)&g_game->gui, found, (g_game->flags_37f06.byte >> 1) & 1);
        }
        found = FindGadgetIndex(layer->entries, "BSHADOWS", 1);
        if (found != -1) {
            SetButtonStage((Menu_0045e5e0*)&g_game->gui, found, (g_game->flags_37f06.byte >> 4) & 1);
        }
        found = FindGadgetIndex(layer->entries, "SHADING", 1);
        if (found != -1) {
            SetButtonStage((Menu_0045e5e0*)&g_game->gui, found, (g_game->flags_37f06.byte >> 5) & 1);
        }
        found = FindGadgetIndex(layer->entries, "GAMMA", 0xe);
        if (found != -1) {
            Entry_0045e5e0* e = FUN_004a0200(layer->entries, "GAMMA");
            e->max = 0x14;
            e->fn = HandleGammaSlider;
            int value = g_game->brightness;
            if (value > 0x14)
                value = 0x14;
            float f = (float)value * (float)(e->steps - 1) * 0.05f;
            if (f - (int)f != 0.0f)
                f += 1.0;
            e->pos = (short)f;
        }
    }

    SetKeyboardInput(&g_game->gui, 1);
    OrLabelAttribs();
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x45ead0
void __stdcall HandleSpeedOptionsClick(Gadget_0045ead0* gadget)
{
    char* data = gadget->link->data;
    if (gadget->field_60 == -1) {
        g_game->flags_37ebe.word &= 0xfffe;
        return;
    }
    if (IsCurrentGadgetNamed((Gadget_0045ead0*)&g_game->gui, "LEFTCLICK")) {
        PlaySoundByName("Options", 0);
        g_game->field_37efa = GetButtonStageByName((Gadget_0045ead0*)&g_game->gui, "LEFTCLICK");
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed((Gadget_0045ead0*)&g_game->gui, "UNITCHAT")) {
        PlaySoundByName("Options", 0);
        ClearSelectedGadget(gadget);
        g_game->field_37f18 = (unsigned char)(GetButtonStageByName((Gadget_0045ead0*)&g_game->gui, "UNITCHAT") * 5);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "UNDO")) {
        PlaySoundByName("Options", 0);
        g_game->field_37f23 = g_optionsBackupTextScroll;
        g_game->field_38a4b = g_optionsBackupGameSpeed.s;
        g_game->field_38a4d = g_optionsBackupGameSpeed.s;
        g_game->scrollSpeed = g_optionsBackupEdgeScroll.b;
        g_game->field_37efa = g_optionsBackupInterfaceType;
        g_game->field_37f17 = g_optionsBackupUnitChat;
        g_game->field_37f18 = g_optionsBackupUnitChatText;
        g_game->field_37f27 = g_optionsBackupTextLines;
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
        g_game->scrollSpeed = 0x20;
        g_game->field_37efa = 0;
        g_game->field_37f17 = 10;
        g_game->field_37f18 = 5;
        CloseTopScreen(gadget);
        OpenSpeedOptions();
        return;
    }
    int i = gadget->field_60;
    // Index with gadget->field_60 * 347, not the local i: selects the original lea.
    if (data[gadget->field_60 * 347] != 1) {
        ClearSelectedGadget(gadget);
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

// FUNCTION: 0x45ed50
void OpenSpeedOptions()
{
    Object_0045ed50* obj = (Object_0045ed50*)OpenOptionsLayout();
    RenderLayer(&g_game->gui, 2);
    EnsureOptionsPanelGadget();
    if (g_game->flags_37ebe.byte & 1) {
        LoadGuiLayer(&g_game->gui, "SPEEDSRT.GUI", 0x200);
    } else {
        LoadGuiLayer(&g_game->gui, "SPEEDS.GUI", 0x200);
        LoadPictureCached("optinterface4x", 0, 0, 0);
    }
    obj->fn = HandleSpeedOptionsClick;
    EnableKeyCommands(&g_game->gui);
    int found = FindGadgetIndex(obj->entries, "GAME", 0xe);
    SetGadgetStatusByName(&g_game->gui, "SPEEDS", 1);
    if (found != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "GAME");
        e->max = 0x15;
        e->fn = HandleGameSpeedSlider;
        int value = g_game->field_38a4b;
        if (value > 0x15) {
            value = 0x15;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.0476190485060215f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
    }
    if (FindGadgetIndex(obj->entries, "SCREEN", 0xe) != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "SCREEN");
        e->max = 0x41;
        int value = g_game->scrollSpeed;
        if (value > 0x41) {
            value = 0x41;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.015384615398943424f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
        e->fn = HandleScreenSlider;
    }
    SetButtonStageByName(&g_game->gui, "UNITCHAT", g_game->field_37f18 / 5);
    SetButtonStageByName(&g_game->gui, "LEFTCLICK", g_game->field_37efa);
    char text[20];
    sprintf(text, g_game->field_37f27 ? "%d" : "None", g_game->field_37f27);
    SetTranslatedTextByName(&g_game->gui, "MAXLINESTEXT", text, 0);
    if (FindGadgetIndex(obj->entries, "MAXLINES", 4) != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "MAXLINES");
        e->max = 0x1e;
        int value = g_game->field_37f27;
        if (value > 0x1e) {
            value = 0x1e;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.03333333507180214f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
        e->fn = HandleMaxLinesSlider;
    }
    if (FindGadgetIndex(obj->entries, "TXTSCROL", 0xe) != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "TXTSCROL");
        e->max = 0x14;
        int value = g_game->field_37f23;
        if (value > 0x14) {
            value = 0x14;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.05000000074505806f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
        e->fn = HandleTextScrollSlider;
    }
    for (int i = 1; i <= obj->entries->count; i++) {
        if (obj->entries[i].type == 4)
            obj->entries[i].fn(&g_game->gui, 0);
    }
    SetKeyboardInput(&g_game->gui, 1);
    OrLabelAttribs();
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x45f190
void __stdcall HandleGameSettingsDialogClick(Gadget_0045f190* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            PlaySoundByName("Options", 0);
            return;
        }
        ClearSelectedGadget(gadget);
    }
}

// FUNCTION: 0x45f1d0
void ShowGameSettingsDialog()
{


    Layer_0045f1d0* layer = LoadGuiLayer((Layer_0045f1d0*)&g_game->gui, "GAMEOPTIONS.GUI", 0x1881);
    Entry_0045f1d0* entries = layer->entries;
    layer->handler = HandleGameSettingsDialogClick;
    LoadPictureCached("GameSettings", 0, 0, 0);
    int count = layer->entries->u.count;
    // Mask in its own statement, and compute opts before rule: keeps the
    // global register allocation.
    unsigned int index = FindHostSlot();
    index &= 0xff;
    char num[0x40];
    Opts_0045f1d0* opts = g_game->players[index].info;
    Rule_0045f1d0* rule = &((Rule_0045f1d0*)g_game->rules)[g_game->playerType];
    char* deathStrs[3] = { "Game Continues", "Game Ends", "Deathmatch" };
    char* locStrs[2] = { "Random", "Fixed" };
    char* mapStrs[2] = { "Mapped", "Unmapped" };
    char* losStrs[3] = { "True", "Circular", "Permanent" };
    char* cheatStrs[2] = { "Disallowed", "Allowed" };
    char* watchStrs[2] = { "Disallowed", "Allowed" };
    char* diffStrs[3] = { "Easy", "Medium", "Hard" };
    AddTextGadget(layer, "TEXT", Translate("Commander Death:"), 0x12, 0x5a, 0x6e, 2);
    AddTextGadget(layer, "TEXT", Translate(deathStrs[g_game->commanderDeath]), 0x8c,
                 0x5a, 0x78, 2);
    AddTextGadget(layer, "TEXT", Translate("Starting Locations:"), 0x12, 0x6c, 0x6e, 2);
    if (g_game->mode->GetGameType() == 2) {
        AddTextGadget(layer, "TEXT", Translate(locStrs[g_game->rules->startType]), 0x8c,
                     0x6c, 0x78, 2);
    } else {
        AddTextGadget(layer, "TEXT", Translate(locStrs[opts->u.b.b14]), 0x8c, 0x6c,
                     0x78, 2);
    }
    AddTextGadget(layer, "TEXT", Translate("Mapping Mode:"), 0x12, 0x7e, 0x6e, 2);
    AddTextGadget(layer, "TEXT", Translate(mapStrs[g_game->los.lb.l0]), 0x8c, 0x7e, 0x78, 2);
    AddTextGadget(layer, "TEXT", Translate("Line of Sight:"), 0x12, 0x90, 0x6e, 2);
    unsigned short losFlags = g_game->los.losFlags;
    int losIdx = !(losFlags & 2) ? 2 : (int)(((unsigned char)~losFlags >> 2) & 1);
    AddTextGadget(layer, "TEXT", Translate(losStrs[losIdx]), 0x8c, 0x90, 0x78, 2);
    int y;
    if (g_game->mode->GetGameType() == 3) {
        AddTextGadget(layer, "TEXT", Translate("Cheat Codes:"), 0x12, 0xa2, 0x6e, 2);
        AddTextGadget(layer, "TEXT", Translate(cheatStrs[opts->u.b.b13]), 0x8c,
                     0xa2, 0x78, 2);
        AddTextGadget(layer, "TEXT", Translate("Watching:"), 0x12, 0xb4, 0x6e, 2);
        AddTextGadget(layer, "TEXT", Translate(watchStrs[opts->u.b.b7]), 0x8c, 0xb4,
                     0x78, 2);
        y = 0xc6;
    } else {
        AddTextGadget(layer, "TEXT", Translate("Difficulty:"), 0x12, 0xa2, 0x6e, 2);
        AddTextGadget(layer, "TEXT", Translate(diffStrs[g_game->difficulty]), 0x8c, 0xa2,
                     0x78, 2);
        y = 0xb4;
    }
    AddTextGadget(layer, "TEXT", Translate("Map:"), 0x12, y, 0x6e, 2);
    AddTextGadget(layer, "TEXT",
                 Translate(g_game->mode->GetMissionName()), 0x8c, y,
                 0x78, 2);
    y += 0x12;
    AddTextGadget(layer, "TEXT", Translate("Starting Metal:"), 0x12, y, 0x6e, 2);
    if (g_game->mode->GetGameType() == 3) {
        AddTextGadget(layer, "TEXT", _itoa(opts->startMetal * 100, num, 10), 0x8c, y, 0x78,
                     2);
    } else {
        AddTextGadget(layer, "TEXT", _itoa(rule->startMetal, num, 10), 0x8c, y, 0x78, 2);
    }
    y += 0x12;
    AddTextGadget(layer, "TEXT", Translate("Starting Energy:"), 0x12, y, 0x6e, 2);
    if (g_game->mode->GetGameType() == 3) {
        AddTextGadget(layer, "TEXT", _itoa(opts->startEnergy * 100, num, 10), 0x8c, y,
                     0x78, 2);
    } else {
        AddTextGadget(layer, "TEXT", _itoa(rule->startEnergy, num, 10), 0x8c, y, 0x78, 2);
    }
    y += 0x12;
    AddTextGadget(layer, "TEXT", Translate("Max Units:"), 0x12, y, 0x6e, 2);
    AddTextGadget(layer, "TEXT", _itoa(g_game->maxUnits, num, 10), 0x8c, y, 0x78, 2);
    int i;
    for (i = count + 1; i <= layer->entries->u.count; i++)
        entries[i].flags = 1;
    SetKeyboardInput((Layer_0045f1d0*)&g_game->gui, 1);
    RenderLayer((Layer_0045f1d0*)&g_game->gui, 0x40);
}

// FUNCTION: 0x45f770
void __stdcall HandleBriefingClick(Gadget_0045f770* gadget)
{
    if (gadget->field_60 == -1) {
        FreeBlinkWords((int)&g_game->gui);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OK")) {
        PlaySoundByName("Options", 0);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "TextRegion") || IsCurrentGadgetNamed(gadget, "MOREBAR")) {
        PlaySoundByName("Options", 0);
        DrawHelpPage();
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
    }
    if (gadget->field_60 != -1)
        ClearSelectedGadget(gadget);
}

// FUNCTION: 0x45f800
void OpenBriefingDialog()
{
    Info_0045f800* g = LoadGuiLayer((Sub_0045f800*)&g_game->gui, "BRIEFING.GUI", 0);
    Entry_0045f800* gadgets = g->info;
    g->handler = HandleBriefingClick;
    int i = FindGadgetIndex(gadgets, "MOREBAR", 0xe);
    // Suspected original bug: the entry is reached as gadgets + i + i * 0x15a
    // instead of gadgets + i * 0x15a, so this clears the flag of a different
    // gadget (or walks off the array) than the one just looked up.
    ((Entry_0045f800*)((char*)gadgets + i))[i].flags &= ~0x10;
    i = FindGadgetIndex(gadgets, "TextRegion", 0xe);
    ((Entry_0045f800*)((char*)gadgets + i))[i].flags &= ~0x10;
    LoadPictureCached("igmbrief", 0, 0, 0);
    AllocBlinkWords((Sub_0045f800*)&g_game->gui, 0xf);
    InitBriefingText();
    RenderLayer((Sub_0045f800*)&g_game->gui, 0x40);
}

// Emits one help line as its two text fields, left of the '|' and right of it,
// and marks both entries as used.
static inline void AddLine(Page_0045f8c0* page, Layer_0045f8c0* layer, char* value, int y)
{
    if (value[0] == '|') {
        strcpy(value, page->blank);
    } else {
        // Keep the scanned char an expression, not a local.
        char* p = value + 1;
        do {
        } while (*p++ != '|');
        p[-1] = 0;
    }
    AddTextGadget(layer, "TEXT", Translate(SkipTextLines(value, 0)), 0x28, y, 0x4e, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
    AddTextGadget(layer, "TEXT", Translate(SkipTextLines(value, 1)), 0x7d, y, 0x12c, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
}

// FUNCTION: 0x45f8c0
void __stdcall FillHelpPage(Sub_0045f8c0* sub, int page, int lineCount)
{
    Layer_0045f8c0* layer = sub->layer;
    ((Table_0045f8c0*)layer->entries)->count = g_helpDialogBaseGadgetCount;
    TdfFile parser;
    char path[256];
    char key[12];
    char value[0x80];
    BuildDataPath(path, "gamedata", "help", "TDF");
    if ((&parser)->LoadFile(path)) {
        int y = 0x32;
        if ((&parser)->SelectRecord("Help")) {
            Page_0045f8c0 lines;
            Page_0045f8c0* pp = &lines;
            // Operands read through non-bare-load locals, declared in the opposite
            // order to the multiply: load order follows declaration order.
            int p2 = (page ? page : page);
            int n = (lineCount ? lineCount : lineCount);
            int first = (n ? n : n) * (p2 ? p2 : p2);
            int last = first + n;
            pp->blank[0] = ' ';
            pp->blank[1] = 0;
            pp->blank2[0] = ' ';
            pp->blank2[1] = 0;
            pp->first = first;
            pp->last = last;
            if (pp->first < pp->last) {
                do {
                    wsprintfA(key, "Line%d", pp->first);
                    if (parser.current->GetFieldString(value, key, 0x80, DAT_005119b8)) {
                        AddLine(pp, layer, value, y);
                        y += 0x12;
                    }
                } while (++pp->first < pp->last);
            }
        }
    }
    MarkChanged(sub);
}

// FUNCTION: 0x45fac0
void __stdcall HandleHelpClick(Gadget_0045fac0* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            PlaySoundByName("Options", 0);
            return;
        }
        if (IsCurrentGadgetNamed(gadget, "Page")) {
            PlaySoundByName("Options", 0);
            FillHelpPage(gadget, GetButtonStageByName(gadget, "Page"), 0x11);
        }
        ClearSelectedGadget(gadget);
    }
}

// FUNCTION: 0x45fb30
void OpenHelpDialog()
{
    Gadget_0045fb30* g = LoadGuiLayer((Sub_0045fb30*)&g_game->gui, "HELP.GUI", 0x1881);
    g->handler = HandleHelpClick;
    LoadPictureCached("dhelp", 0, 0, 0);
    g_helpDialogBaseGadgetCount = g->info->field_b6;
    FillHelpPage((Sub_0045fb30*)&g_game->gui, 0, 0x11);
    SetKeyboardInput((Sub_0045fb30*)&g_game->gui, 1);
    RenderLayer((Sub_0045fb30*)&g_game->gui, 0x40);
}

// FUNCTION: 0x45fbc0
void ClearScreenWithHudPalette()
{
    Surface_0045fbc0 screen;
    unsigned char palette[0x400];

    memset(palette, 0, sizeof(palette));
    SetPaletteColors(palette, 0, 0x100);
    SetOffscreenSurface((int)g_game->field_37e1b);
    if (LockScreen(&screen)) {
        FillSurface(&screen, g_game->gui.field_dcb);
        UnlockScreen(&screen);
        FlipScreen();
    }
}

// FUNCTION: 0x45fc40
void CloseOptionsPanel()
{
    g_game->flags_37ebe.word &= 0xfffe;
    g_game->frontendSubstateRequest = 3;
}

// FUNCTION: 0x45fc60
void __stdcall HandleOptionsPanelClick(Gadget_0045fc60* gadget)
{
    // goto, not a return: the label after the g_optionsShellClosing = 0 store keeps the
    // teardown tail shared.
    if (gadget->field_60 == -1)
        goto cleanup;
    {
        SetGadgetStatus(gadget, gadget->field_60, 1);
        // Empty then-arm: the positive test alone changes the codegen.
        if (g_game->bits_2a44.bits.bit2) {
        } else {
            RenderLayer((char*)&g_game->gui, 0x40);
            MarkChanged((char*)&g_game->gui);
            BlitMenuLayers((char*)&g_game->gui, 0, 0);
            SetOffscreenSurface((int)g_game->field_37e1b);
            FlipScreen();
        }
        if (IsCurrentGadgetNamed(gadget, "SPEEDS")) {
            PlaySoundByName("Options", 0);
            OpenSpeedOptions();
        } else if (IsCurrentGadgetNamed(gadget, "VISUALS")) {
            PlaySoundByName("Options", 0);
            OpenVisualOptions(0);
        } else if (IsCurrentGadgetNamed(gadget, "MUSIC")) {
            PlaySoundByName("Options", 0);
            OpenMusicOptions();
        } else if (IsCurrentGadgetNamed(gadget, "PREV")) {
            PlaySoundByName("Options", 0);
            SaveSettings();
            g_optionsShellClosing = 1;
            return;
        } else if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
            PlaySoundByName("Previous", 0);
            RestoreSoundOptions();
            g_game->volume2 = g_optionsBackupMusicVolume;
            ((Class_004ce3e0*)g_game->sound)->CopyTrackTypeTable(&g_optionsBackupTrackTypes);
            g_game->field_37f16 = g_optionsBackupCdMode;
            ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
            if (((unsigned char)g_game->flags_37f14.word ^ (unsigned char)g_optionsBackupMusicMode.s) & 1) {
                ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
            }
            unsigned short f = g_game->flags_37f14.word;
            g_game->flags_37f14.word = f ^ ((f ^ g_optionsBackupMusicMode.s) & 1);
            ((Class_004ce580*)g_game->sound)->SetLockedTrack(g_optionsBackupLockedTrack);
            SetBrightness(0.5 - g_game->brightness * -0.041666668f);
            ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
            ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
            g_game->field_37f23 = g_optionsBackupTextScroll;
            g_game->field_38a4b = g_optionsBackupGameSpeed.i;
            g_game->field_38a4d = g_optionsBackupGameSpeed.i;
            g_game->scrollSpeed = g_optionsBackupEdgeScroll.b;
            g_game->field_37efa = g_optionsBackupInterfaceType;
            g_game->field_37f17 = g_optionsBackupUnitChat;
            g_game->field_37f18 = g_optionsBackupUnitChatText;
            g_game->field_37f27 = g_optionsBackupTextLines;
            RestoreVisualOptions();
            g_optionsShellClosing = 1;
            return;
        } else if (IsCurrentGadgetNamed(gadget, "SOUND")) {
            PlaySoundByName("Options", 0);
            OpenSoundOptions();
        } else {
            if (gadget->field_60 != -1)
                ClearSelectedGadget(gadget);
            return;
        }
        g_optionsShellClosing = 0;
cleanup:
        if (g_optionsBackupSurface) {
            DrawSurface(0, g_optionsBackupSurface, 0, 0);
            FreeSurface(g_optionsBackupSurface);
            g_optionsBackupSurface = 0;
        }
        if (g_optionsFlipSurface) {
            FreeSurface(g_optionsFlipSurface);
            g_optionsFlipSurface = 0;
        }
        g_optionsShellActive = 0;
        return;
    }
}

// FUNCTION: 0x45ffb0
void __stdcall DrawOptionsScrollBar(void* surf)
{
    if (g_optionsShellActive) {
        if (g_optionsLightbarX < 0x115) {
            int old = g_optionsLightbarX;
            g_optionsLightbarX += 0x15;
            if (g_optionsLightbarX >= 0x115) {
                PlaySoundByName("Options", 0);
                g_optionsLightbarX = 0x115;
            }
            if (g_optionsLightbarX > g_optionsLightbarMaxX && old < g_optionsLightbarMaxX) {
                void* snd = FindGafEntry(g_game->gui.logos32, "LIGHTBAR");
                Sound_45ffb0* s = (Sound_45ffb0*)GetGafFrame(snd, 2);
                DrawFrame((int)g_optionsFlipSurface, s, s->start, s->end);
            }
        }
        // Test with `<` so the increase is the fallthrough, and use the global
        // directly, not a local: keeps the load before the branch.
        if (g_optionsLightbarX < g_optionsLightbarMaxX) {
            g_optionsLightbarAnim += 6;
        } else {
            g_optionsLightbarAnim -= 6;
            if (g_optionsLightbarAnim < 0)
                g_optionsLightbarAnim = 0;
        }
        Quad_45ffb0 src;
        src.p[0].x = 1;
        src.p[0].y = 1;
        src.p[3].x = 1;
        src.p[1].y = 1;
        Quad_45ffb0 dst;
        if (g_optionsLightbarX > g_optionsLightbarMaxX) {
            if (g_optionsLightbarX < 0x115)
                g_optionsLightbarX++;
            dst.p[1].x = g_optionsLightbarX;
            dst.p[2].x = g_optionsLightbarX;
            dst.p[3].x = g_optionsLightbarMaxX;
            dst.p[0].x = g_optionsLightbarMaxX;
            dst.p[0].y = g_optionsLightbarY;
            dst.p[1].y = g_optionsLightbarY - g_optionsLightbarAnim;
        } else {
            dst.p[3].x = g_optionsLightbarX;
            dst.p[0].x = g_optionsLightbarX;
            dst.p[1].x = 127;
            dst.p[2].x = 127;
            dst.p[0].y = g_optionsLightbarY - g_optionsLightbarAnim;
            dst.p[1].y = g_optionsLightbarY;
        }
        dst.p[2].y = 479;
        dst.p[3].y = 479;
        src.p[1].x = g_optionsFlipFrame.w - 1;
        src.p[2].x = g_optionsFlipFrame.w - 1;
        src.p[2].y = g_optionsFlipFrame.h - 1;
        src.p[3].y = g_optionsFlipFrame.h - 1;
        DrawFrameQuad(surf, &g_optionsFlipFrame, &dst, &src);
        MarkChanged((char*)&g_game->gui);
        g_game->flags_142f1 |= 2;
        g_game->field_37e98 = 1;
    }
}

// FUNCTION: 0x460160
void OpenOptionsPanel()
{
    Holder_00460160* holder = (Holder_00460160*)g_game->gui.holder;
    if (!g_game->bits_2a44.bits.bit2) {
        BlankScreen();
    }
    // Re-read holder->field_4 at every use; do not cache it in a local.
    g_optionsFlipSurface = AllocSurface("FLIPSURFACE", holder->field_4->field_17, holder->field_4->field_19);
    memcpy(g_optionsFlipSurface->pixels, holder->field_4->field_bc->pixels,
           holder->field_4->field_17 * holder->field_4->field_19);
    FrameFromSurface((Dst_004b8ae0*)&g_optionsFlipFrame, g_optionsFlipSurface);
    g_optionsLightbarX = 0;
    g_optionsLightbarMaxX = holder->field_4->field_17 - 1;
    g_optionsLightbarY = holder->field_4->field_15;
    g_optionsShellActive = 1;
    g_optionsBackupSurface = AllocSurface("BKUPSURFACE", 300, 480);
    DrawSurface(g_optionsBackupSurface, 0, 0, 0);
    g_optionsLightbarAnim = 0;
    Layer_00460160* panel = (Layer_00460160*)OpenOptionsLayout();
    if (!g_game->bits_2a44.bits.bit2) {
        LoadPictureCached("options4x", 0, 0, 0);
    }
    panel->handler = HandleOptionsPanelClick;
    memcpy(g_optionsPrefsSnapshot.block, (char*)g_game + 0x37ee6, 0x53);
    g_optionsPrefsSnapshot.bit0 = g_game->los.bits.bit1;
    g_optionsPrefsSnapshot.bit1 = g_game->los.bits.bit2;
    g_optionsBackupGameSpeed.i = g_game->field_38a4b;
    g_optionsBackupEdgeScroll.i = g_game->scrollSpeed;
    g_optionsBackupLockedTrack = ((Class_004ce5a0*)g_game->sound)->GetLockedTrack();
    for (int i = 0; i < 100; i++) {
        g_optionsBackupTrackTypes[i] = ((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(i);
    }
    SetKeyboardInput((Menu_00460160*)&g_game->gui, 1);
    RenderLayer((Menu_00460160*)&g_game->gui, 0xc0);
    if (g_game->bits_2a44.bits.bit2) {
        PlaySoundByName("Panel", 0);
    }
}

// FUNCTION: 0x460340
void __stdcall HandleRestartDialogClick(Gadget_00460340* gadget)
{
    if (gadget->field_60 == -1)
        return;
    PlaySoundByName("Options", 0);
    if (IsCurrentGadgetNamed(gadget, "RESTART")) {
        int ok = 0;
        int mode = g_game->mode->GetGameType();
        switch (mode) {
        case 1:
            if (!FindGameCdDrive(0)) {
                OpenMessageBox((char*)&g_game->gui,
                             Translate("Please insert the Campaign CD (Disc 2) and try again"),
                             200, 1, 1);
                ClearSelectedGadget((char*)&g_game->gui);
                return;
            }
            ok = true;
            break;
        case 2:
            if (!FindGameCdDrive(1)) {
                OpenMessageBox((char*)&g_game->gui,
                             Translate("Please insert the Multiplayer CD (Disc 1) and try again"),
                             200, 1, 1);
                ClearSelectedGadget((char*)&g_game->gui);
                return;
            }
            ok = true;
            break;
        }
        if (!ok)
            return;
        RegisterDataArchives();
        g_game->difficulty = GetButtonStageByName(gadget, "Difficulty");
        g_game->field_39249 = 1;
    } else if (IsCurrentGadgetNamed(gadget, "Difficulty")) {
        PlaySoundByName("Options", 0);
        ClearSelectedGadget(gadget);
    } else if (!IsCurrentGadgetNamed(gadget, "CANCEL") && gadget->field_60 != -1) {
        ClearSelectedGadget(gadget);
    }
}

// FUNCTION: 0x4604a0
void OpenRestartDialog()
{
    Menu_004604a0* menu = (Menu_004604a0*)&g_game->gui;
    Dialog_004604a0* dialog = LoadGuiLayer(menu, "RESTART.GUI", 0x1000);
    Gadget_004604a0* gadgets = dialog->gadgets;
    dialog->handler = HandleRestartDialogClick;
    LoadPictureCached("drestart", 0, 0, 0);
    int index = FindGadgetIndex(gadgets, "MISSIONNAME", 5);
    menu->field_14 = menu->field_c;
    char* text = WordWrapText((Menu_004604a0*)&g_game->gui,
                              g_game->mode->GetMissionName(gadgets[index].field_17),
                              -1);
    menu->field_14 = menu->field_8;
    char* first = strtok(text, "\n");
    SetTranslatedTextByName((Menu_004604a0*)&g_game->gui, "MISSIONNAME", (int)first, 0x80);
    char* second = strtok(0, "\n");
    if (second) {
        SetTranslatedTextByName((Menu_004604a0*)&g_game->gui, "MISSIONNAME1", (int)second, 0x80);
    }
    ApplyDifficultyButtons();
    SetKeyboardInput((Menu_004604a0*)&g_game->gui, 1);
    RenderLayer((Menu_004604a0*)&g_game->gui, 0x40);
    SetCursorMode(0x13);
}

// FUNCTION: 0x4605c0
void __stdcall HandleSurrenderChoice(Gadget_004605c0* gadget)
{
    int owner = gadget->owner->field_4;
    if (gadget->field_60 == -1)
        return;
    PlaySoundByName("Exit", 0);
    if (IsGadgetNamed(owner, gadget->field_60, "CHOICE1")) {
        g_game->sound->SetTrackCategory(4);
        switch (g_battleQuitIntent) {
        case 0:
        case 1:
            ShutdownIngameSystems();
            PopUntilNamedLayout(1);
            CloseTopScreen(&g_game->gui);
            BlankScreen();
            SetGameMode(1);
            return;
        case 2:
            g_game->flags_3923b |= 4;
            ShutdownIngameAndQuit();
            return;
        }
    } else if (!IsGadgetNamed(owner, gadget->field_60, "CHOICE2")) {
        ClearSelectedGadget(gadget);
    }
}

// FUNCTION: 0x460680
void OpenSurrenderDialog()
{
    Gadget_00460680* gadget = LoadGuiLayer((Sub_00460680*)&g_game->gui, "YESORNO.GUI", 0x1000);
    if (gadget == 0) {
        return;
    }
    SetKeyboardInput((Sub_00460680*)&g_game->gui, 1);
    char* entries = gadget->entries;
    FindGadgetIndex(entries, "CHOICE1", 1);
    FindGadgetIndex(entries, "CHOICE2", 1);
    // Suspected original bug: the sibling dialog setup at 0x464e70 copies
    // "CHOICE1" to entries+0xcc and "CHOICE2" to entries+0xdc, but this
    // function copies "CHOICE2" (0x503120) into both fields.
    strcpy(entries + 0xcc, "CHOICE2");
    strcpy(entries + 0xdc, "CHOICE2");
    SetTranslatedTextByName((Sub_00460680*)&g_game->gui, "CHOICE1", "Yes", 0);
    SetTranslatedTextByName((Sub_00460680*)&g_game->gui, "CHOICE2", "No", 0);
    if (g_battleQuitIntent == 0) {
        SetTranslatedTextByName((Sub_00460680*)&g_game->gui, "TITLE", "Surrender this battle and return to main menu?", 0);
    } else if (g_battleQuitIntent == 2) {
        const char* title = g_game->flags_2bee.flag4 ? "Exit the Battle"
                                                : "Surrender this battle and exit to Windows?";
        SetTranslatedTextByName((Sub_00460680*)&g_game->gui, "TITLE", title, 0);
    }
    SelectGadgetByName((Sub_00460680*)&g_game->gui, "CHOICE2");
    gadget->handler = HandleSurrenderChoice;
    RenderLayer((Sub_00460680*)&g_game->gui, 0x40);
}

// FUNCTION: 0x460800
void __stdcall HandleExitMenuClick(Gadget_00460800* gadget)
{
    if (gadget->field_60 != -1) {
        PlaySoundByName("Options", 0);
        if (IsCurrentGadgetNamed(gadget, "MAINMENU")) {
            g_battleQuitIntent = 0;
            CloseTopScreen(gadget);
            OpenSurrenderDialog();
            return;
        }
        if (IsCurrentGadgetNamed(gadget, "EXITGAME")) {
            g_battleQuitIntent = 2;
            CloseTopScreen(gadget);
            OpenSurrenderDialog();
            return;
        }
        if (!IsCurrentGadgetNamed(gadget, "CANCEL")) {
            if (IsCurrentGadgetNamed(gadget, "RESTART")) {
                CloseTopScreen(gadget);
                OpenRestartDialog();
                return;
            }
            if (gadget->field_60 != -1)
                ClearSelectedGadget(gadget);
        }
    }
}

// FUNCTION: 0x4608b0
void OpenExitMenu()
{
    Dialog_004608b0* dialog = LoadGuiLayer((Sub_004608b0*)&g_game->gui, "EXITMENU.GUI", 0x1800);
    dialog->handler = HandleExitMenuClick;
    FindGadgetIndex(dialog->gadgets, "RESTART", 1);
    if (g_game->mode->GetGameType() == 1) {
        SetGadgetActiveByName((Sub_004608b0*)&g_game->gui, "RESTART", 1);
        SetTranslatedTextByName((Sub_004608b0*)&g_game->gui, "RESTART", (int)Translate("Restart"), 0x80);
        goto tail;
    }
    // Restart body written twice on purpose: the compiler merges them and lays
    // restart out before main-menu.
    if (g_game->mode->GetGameType() == 2) {
        SetGadgetActiveByName((Sub_004608b0*)&g_game->gui, "RESTART", 1);
        SetTranslatedTextByName((Sub_004608b0*)&g_game->gui, "RESTART", (int)Translate("Restart"), 0x80);
        goto tail;
    }
    if (g_game->flags_2bee.flag4) {
        SetGadgetActiveByName((Sub_004608b0*)&g_game->gui, "MAINMENU", 0);
    }
tail:
    SetKeyboardInput((Sub_004608b0*)&g_game->gui, 1);
    RenderLayer((Sub_004608b0*)&g_game->gui, 0x40);
}

// FUNCTION: 0x4609a0
void __cdecl HandleBattleQuitPrompt(int)
{
    g_battleQuitIntent = 2;
    OpenSurrenderDialog();
}

// FUNCTION: 0x4609b0
void __stdcall HandleInGameOptionsClick(Gadget_004609b0* gadget)
{
    if (gadget->field_60 == -1) {
        DisableKeyCommands((Sub_004609b0*)&g_game->gui);
        g_optionsShellActive = 0;
        if (g_optionsFlipSurface) {
            FreeSurface(g_optionsFlipSurface);
            g_optionsFlipSurface = 0;
        }
        if (g_optionsBackupSurface) {
            FreeSurface(g_optionsBackupSurface);
            g_optionsBackupSurface = 0;
        }
        if (g_game->bits_2a44.byte & 4) {
            if (g_game->mode->GetGameType() != 3)
                g_game->orders &= 0xfffe;
        }
        g_game->flags_37ebe.word &= 0xfffe;
        ((Class_004ce910*)g_game->sound)->PauseCdAudio(0);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "LOADGAME")) {
        PlaySoundByName("Options", 0);
        ShowLoadGameScreen();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SAVEGAME")) {
        PlaySoundByName("Options", 0);
        ShowSaveGameScreen();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "PREFS")) {
        PlaySoundByName("Options", 0);
        OpenOptionsPanel();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "HELP")) {
        PlaySoundByName("Options", 0);
        Info_004609b0* g = LoadGuiLayer((Sub_004609b0*)&g_game->gui, "HELP.GUI", 0x1881);
        g->handler = HandleHelpClick;
        LoadPictureCached("dhelp", 0, 0, 0);
        g_helpDialogBaseGadgetCount = g->info->count;
        FillHelpPage((Sub_004609b0*)&g_game->gui, 0, 0x11);
        SetKeyboardInput((Sub_004609b0*)&g_game->gui, 1);
        RenderLayer((Sub_004609b0*)&g_game->gui, 0x40);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "MISSION")) {
        PlaySoundByName("Options", 0);
        if (g_game->mode->GetGameType() == 1) {
            Info_004609b0* g = LoadGuiLayer((Sub_004609b0*)&g_game->gui, "BRIEFING.GUI", 0);
            Entry_004609b0* gadgets = g->info;
            g->handler = HandleBriefingClick;
            int i = FindGadgetIndex(gadgets, "MOREBAR", 0xe);
            // The entry is reached as gadgets + i + i * 0x15a, not gadgets +
            // i * 0x15a, so this clears the flag of a gadget one stride past
            // the one just looked up.
            ((Entry_004609b0*)((char*)gadgets + i))[i].flags &= ~0x10;
            i = FindGadgetIndex(gadgets, "TextRegion", 0xe);
            ((Entry_004609b0*)((char*)gadgets + i))[i].flags &= ~0x10;
            LoadPictureCached("igmbrief", 0, 0, 0);
            AllocBlinkWords((Sub_004609b0*)&g_game->gui, 0xf);
            InitBriefingText();
            RenderLayer((Sub_004609b0*)&g_game->gui, 0x40);
            return;
        }
        ShowGameSettingsDialog();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "EXIT")) {
        PlaySoundByName("Options", 0);
        OpenExitMenu();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OK")) {
        PlaySoundByName("Options", 0);
        return;
    }
    if (gadget->field_60 != -1)
        ClearSelectedGadget(gadget);
}

// FUNCTION: 0x460cc0
void OpenInGameOptions()
{
    Gadget_00460cc0* gadget = LoadGuiLayer((Gui_00460cc0*)&g_game->gui, "ARMOPT.GUI", 0x800);
    gadget->handler = HandleInGameOptionsClick;
    SetGrayedOut((Gui_00460cc0*)&g_game->gui, FindGadgetIndex(gadget->info, "SAVEGAME", 1),
                 g_game->mode->GetGameType() == 3);
    SetGrayedOut((Gui_00460cc0*)&g_game->gui, FindGadgetIndex(gadget->info, "LOADGAME", 1),
                 g_game->mode->GetGameType() == 3);
    if (g_game->mode->GetGameType() == 3 || g_game->mode->GetGameType() == 2) {
        SetTranslatedTextByName((Gui_00460cc0*)&g_game->gui, "MISSION", Translate("Settings"), 0x80);
    }
    EnableKeyCommands((Gui_00460cc0*)&g_game->gui);
    SetKeyboardInput((Gui_00460cc0*)&g_game->gui, 1);
    RenderLayer((Gui_00460cc0*)&g_game->gui, 0x40);
    if (g_game->mode->GetGameType() != 3) {
        g_game->flags_38a51 |= 1;
    }
    ((Class_004ce910*)g_game->sound)->PauseCdAudio(1);
}
