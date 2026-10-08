// Decompiled by Haiku, Opus, DeepSeek V4.1 Flash, space-bunny-free, Space Bunny Free, deepseek-v4.1-flash, deepseek-v4.1, GPT-6, GPT-6.1-sol, Claude Sonnet 5.5, Claude Opus 5.5 and claude-opus-5-5. Names are provisional.
//
// The options screens after the sound settings: the music, sound, visual,
// speed, controls, help, briefing and in-game options pages, their click
// handlers, and the copies of the saved-settings routines they inline
// (0x45cf60 to 0x460cc0).
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
    void FUN_004ce3e0(const void* src);
};

class Class_004ce450 {
public:
    int GetTrackCount();
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
class Sound {
public:
    int GetCurrentTrack();
    void Enable3D();
    void Disable3D();
    int PlayCdTrack(int index, int flag);
    void SetTrackCategory(int mode);
};

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

struct Opts_0045f1d0 {                 // 0x14b bytes
    char unknown_0[0x9b];
    union {
        unsigned short value;          // +0x9b
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

struct PlayerEntry_0045f1d0 {          // 0x14b bytes
    Opts_0045f1d0* info;               // +0x0
    char unknown_4[0x14b - 4];
};

// Note: Rule_0045f1d0 (24 bytes, startMetal at +0xc, startEnergy at +0x10)
// and RuleSet_0045f1d0 (startType at +0x118) are the SAME memory: the exe
// walks g_game->rules with a 24-byte stride for metal and energy but reads
// startType from element 0, i.e. it ignores playerType for that field.
struct Rule_0045f1d0 {                 // 0x18 bytes, the record g_game->rules is
    char unknown_0[0xc];               //   walked with (stride 24)
    int startMetal;                    // +0xc
    int startEnergy;                   // +0x10
    char unknown_18[0x18 - 0x14];
};

struct RuleSet_0045f1d0 {              // +0x118 startType, read from element 0
    char unknown_0[0xc];
    int startMetal;                    // +0xc
    int startEnergy;                   // +0x10
    char unknown_14[0x118 - 0x14];
    int startType;                     // +0x118
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

struct Settings_00460160 {
    char block[0x53];                  // +0x0
    unsigned short bit0 : 1;           // +0x53, bit 0
    unsigned short bit1 : 1;           // +0x53, bit 1
    unsigned short rest : 14;
};

struct Gadget_004604a0 {
    char unknown_0[0x17];
    short field_17;                    // +0x17
    char unknown_19[0x15b - 0x19];
};

struct Sub_00460680 {
    char unknown_0[0x10];
};

struct Flags_00460680 {
    unsigned short unknown_bit0 : 4;
    unsigned short flag4 : 1;          // bit 4
    unsigned short unknown_rest : 11;
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

// The game state. Every view of it in these files meets here: the fields are
// at the offsets the functions use, and the differently typed views of the
// flag words share a union.
struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x519 - 0x14];
    Gui_0045cf60 gui;                  // +0x519
    char unknown_11e7[0x1b8a - 0x11e7];
    PlayerEntry_0045f1d0 players[10];  // +0x1b8a
    char unknown_2878[0x29a0 - 0x2878];
    RuleSet_0045f1d0* rules;           // +0x29a0
    char unknown_29a4[0x2a42 - 0x29a4];
    unsigned char playerType;          // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    Bits_0045cf60 bits_2a44;           // +0x2a44
    char unknown_2a46[0x2bc0 - 0x2a46];
    unsigned char field_2bc0;          // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    Flags_00460680 flags_2bee;         // +0x2bee
    char unknown_2bf0[0x14281 - 0x2bf0];
    Los_0045cf60 los;                  // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned short flags_142f1;        // +0x142f1
    char unknown_142f3[0x1434d - 0x142f3];
    unsigned char field_1434d;         // +0x1434d
    char unknown_1434e[0x1437b - 0x1434e];
    CMemoryCache* ptr_1437b;           // +0x1437b
    char unknown_1437f[0x37e1b - 0x1437f];
    void* field_37e1b;                 // +0x37e1b
    char unknown_37e1f[0x37e98 - 0x37e1f];
    int field_37e98;                   // +0x37e98
    char unknown_37e9c[0x37ebe - 0x37e9c];
    Flags37ebe_0045cf60 flags_37ebe;   // +0x37ebe
    char unknown_37ec0[0x37ee6 - 0x37ec0];
    unsigned short maxUnits;           // +0x37ee6
    char unknown_37ee8[0x37eee - 0x37ee8];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x37ef6 - 0x37ef2];
    int commanderDeath;                // +0x37ef6
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x37f06 - 0x37efe];
    Flags37f06_0045cf60 flags_37f06;   // +0x37f06
    int brightness;                    // +0x37f08
    union {
        int volume1;                   // +0x37f0c
        short volume1Word;             // 45de30 reads it as a short
    };
    union {
        int volume2;                   // +0x37f10
        short volume2Word;             // 45d7c0 reads it as a short
    };
    Flags37f14_0045cf60 flags_37f14;   // +0x37f14
    unsigned char field_37f16;         // +0x37f16
    unsigned char field_37f17;         // +0x37f17
    unsigned char field_37f18;         // +0x37f18
    SoundFlags_0045cf60 soundFlags;    // +0x37f19
    int width;                         // +0x37f1b
    int height;                        // +0x37f1f
    int field_37f23;                   // +0x37f23
    int field_37f27;                   // +0x37f27
    char unknown_37f2b[0x37f39 - 0x37f2b];
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
extern int DAT_00506788;
extern char DAT_00502b38[];            // "Options"
extern char DAT_005031d4[];            // "BGM"
extern char DAT_005067bc[];            // "NOTRAK"
extern char DAT_00506884[];            // "FXVOL"
extern char DAT_0050692c[];            // "TRACKTYPE"
extern char DAT_00506964[];            // "CDNEXT"
extern char DAT_0050696c[];            // "CDPLAY"
extern char DAT_00506974[];            // "CDSTOP"
extern char DAT_0050697c[];            // "CDPREV"
extern char DAT_00506984[];            // "TRACKMODE"
extern char DAT_00506990[];            // "RESTORE"
extern char DAT_00506998[];            // "UNDO"
extern char DAT_005069b8[];            // "SPEECH"
extern char DAT_005069c0[];            // "TEST"
extern char DAT_005069c8[];            // "VOLTEXT"
extern char DAT_005069d0[];            // "MODE"
extern char DAT_005069d8[];            // "sounds\\explode.wav"
extern char DAT_005119b8[];
extern int DAT_00512ef0;
extern Entry_45ffb0 DAT_00512ef8;
extern int DAT_00512f2c;
extern int DAT_00512f42;
// The saved flags word: 45d280 reads it as an int, 45fc60 as a short.
extern union {
    int i;
    unsigned short s;
} DAT_00512f46;
extern unsigned char DAT_00512f48;
extern unsigned char DAT_00512f49;
extern unsigned char DAT_00512f4a;
extern int DAT_00512f55;
extern int DAT_00512f59;
// The saved game-speed word: 45ead0 reads it as a short, 45fc60 as an int.
extern union {
    unsigned short s;
    int i;
} DAT_00512f6d;
// The saved option byte: 460160 stores it as an int.
extern union {
    unsigned char b;
    int i;
} DAT_00512f71;
extern char DAT_00512f75[];
extern int DAT_00512fd9;
extern int DAT_00512fe0;
extern int DAT_00512fe4;
extern Class_004c6a60* DAT_00512fe8;
extern Class_004c6a60* DAT_00512ff4;
extern int DAT_00512ff8;
extern Settings_00460160 DAT_00512f18;
extern int DAT_00512f10;
extern int DAT_00512f14;
extern int DAT_00512fec;
extern int DAT_00512ff0;

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

void __stdcall FUN_0049fa90(Sub_0045cf60* sub);
void __stdcall FUN_0049fa90(Dialog* obj);
void __stdcall FUN_0049fa90(void* obj);
void __stdcall FUN_0049fa90(Menu_0045de30* obj);
void __stdcall FUN_0049fa90(Gui_0045e100* gui);
void __stdcall FUN_0049fa90(Gadget_0045f770* gadget);
void __stdcall FUN_0049fa90(Sub_0045f8c0* sub);
void __stdcall FUN_0049fa90(char* menu);

void __stdcall FUN_004ab170(Sub_0045cf60* sub, unsigned int* a, int* b);
void __stdcall FUN_004ab170(char* menu, int a, int b);

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

void __stdcall FUN_004a1250(void* obj, char* name, int value);
void __stdcall FUN_004a1250(Dialog* obj, char* name, int value);
void __stdcall FUN_004a1200(Gui_00460cc0* sub, int index, int value);
int __stdcall GetGadgetText(void* settings, const char* key, char* out);

void __stdcall FUN_0049fa50(void* obj);
void __stdcall FUN_0049fa50(Gui_00460cc0* sub);
void __stdcall FUN_0049fa70(Sub_004609b0* sub);
void __stdcall FUN_0049fb10(void* obj, int value);
void __stdcall FUN_0049fb10(Layer_0045f1d0* menu, int flag);
void __stdcall FUN_0049fb10(Menu_00460160* menu, int value);
void __stdcall FUN_0049fb10(Menu_004604a0* menu, int value);
void __stdcall FUN_0049fb10(Sub_00460680* sub, int value);
void __stdcall FUN_0049fb10(Sub_004608b0* sub, int value);
void __stdcall FUN_0049fb10(Sub_004609b0* sub, int value);
void __stdcall FUN_0049fb10(Gui_00460cc0* sub, int value);

int __stdcall SetButtonStageByName(void* obj, char* name, int value);
int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_004a0570(void* obj, char* name, int value);
void __stdcall FUN_004a0570(Object_004a0570* obj, char* name, int value);
void __stdcall FUN_004a0570(Menu_0045e5e0* menu, char* name, int value);
void __stdcall FUN_004a0570(Sub_004608b0* sub, const char* name, int value);
void __stdcall FUN_004a1450(void* obj, char* name, int value);
void __stdcall FUN_004a1450(Object_004a1450* obj, char* name, int value);
void __stdcall FUN_004a0bf0(void* obj, char* name, char* text, int value);
void __stdcall FUN_004a0bf0(Menu_004604a0* menu, const char* name, int value, int count);
void __stdcall FUN_004a0bf0(Sub_00460680* sub, const char* name, const char* text, int param_4);
void __stdcall FUN_004a0bf0(Sub_004608b0* sub, const char* name, int value, int param_4);
void __stdcall FUN_004a0bf0(Gui_00460cc0* sub, const char* name, const char* value, int flags);

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
void __stdcall FUN_004ab0a0(void* obj);
void __stdcall FUN_004ab0a0(Gui_0045e100* gui);
void __stdcall FUN_004ab0a0(Gadget_0045ead0* gadget);
void __stdcall FUN_004ab0a0(Gadget_0045f190* gadget);
void __stdcall FUN_004ab0a0(Gadget_0045f770* gadget);
void __stdcall FUN_004ab0a0(Gadget_0045fac0* gadget);
void __stdcall FUN_004ab0a0(Gadget_0045fc60* gadget);
void __stdcall FUN_004ab0a0(Gadget_004605c0* gadget);
void __stdcall FUN_004ab0a0(Gadget_00460800* gadget);
void __stdcall FUN_004ab0a0(Gadget_004609b0* gadget);

void __stdcall CloseTopScreen(void* queue);
void __stdcall CloseTopScreen(Gui_0045e100* gui);
void __stdcall CloseTopScreen(Gadget_0045ead0* gadget);
void __stdcall CloseTopScreen(Gadget_00460800* gadget);
void __stdcall SetBrightness(float value);
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
void __stdcall FUN_00476d80();
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
void __stdcall FUN_0045ead0(void* obj, int arg);
void __stdcall HandleGameSpeedSlider(void* obj, int arg);
void __stdcall HandleScreenSlider(void* obj, int arg);
void __stdcall HandleMaxLinesSlider(void* obj, int arg);
void __stdcall HandleTextScrollSlider(void* obj, int arg);
void __stdcall FUN_0045f190(void*);
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
int __stdcall FUN_00491c80(int value);
void FUN_00477410();
char __stdcall FindGameCdDrive(int disc);
void __stdcall RegisterDataArchives();
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
int __stdcall IsGadgetNamed(int param1, int param2, char* name);
void FUN_00491b60();
void FUN_00491c60();
int __stdcall FUN_00491d70(int force);
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
void __stdcall FUN_0045ce80();
void __stdcall FUN_0045c820();
void FUN_0045cae0();
void FUN_00428b60();
void SaveSettings();


// A bitfield tested for being set gives "mov cl, [m]; shr cl, 2; test cl, 1";
// testing it for being clear ("if (!bit2) {...}") folds to "test byte ptr".
// FUNCTION: 0x45cf60
void FUN_0045cf60()
{
    if (g_game->bits_2a44.bits.bit2) {
        return;
    }
    RenderLayer((Sub_0045cf60*)&g_game->gui, 0x40);
    FUN_0049fa90((Sub_0045cf60*)&g_game->gui);
    FUN_004ab170((Sub_0045cf60*)&g_game->gui, 0, 0);
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
    FUN_004a1250(&g_game->gui, "MUSIC", *(int*)g_game->sound == 0);
    FUN_0049fa50(&g_game->gui);
    if (g_game->bits_2a44.prefsWord.prefs && g_game->mode->GetGameType() != 3) {
        g_game->flags_38a51 |= 1;
    }
    return result;
}
#pragma auto_inline(on)

// FUNCTION: 0x45d0c0
void FUN_0045d0c0()
{
    char value[20];
    GetGadgetText(&g_game->gui, "TRACKNUM", value);
    int track = atoi(value);
    if (track != ((Sound*)g_game->sound)->GetCurrentTrack()) {
        DAT_00512fe0 = ((Sound*)g_game->sound)->GetCurrentTrack();
        UpdateTrackGadgets();
        FUN_0049fa90((Dialog*)&g_game->gui);
    }
}

// FUNCTION: 0x45d130
void UpdateMusicGadgets()
{
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "NOTRAK", g_game->flags_37f14.notrak & 1);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "TRACKMODE", g_game->field_37f16 - 1);
    FUN_004a1450((Object_004a1450*)&g_game->gui, "MUSICVOL", (char)(~g_game->flags_37f14.notrak & 1));
    FUN_004a1250((Dialog*)&g_game->gui, "CDPREV", (char)(~g_game->flags_37f14.notrak & 1));
    FUN_004a1250((Dialog*)&g_game->gui, "CDSTOP", (char)(~g_game->flags_37f14.notrak & 1));
    FUN_004a1250((Dialog*)&g_game->gui, "CDPLAY", (char)(~g_game->flags_37f14.notrak & 1));
    FUN_004a1250((Dialog*)&g_game->gui, "CDNEXT", (char)(~g_game->flags_37f14.notrak & 1));
    FUN_004a1250((Dialog*)&g_game->gui, "TRACKMODE", (char)(~g_game->flags_37f14.notrak & 1));
    // Spelled as one negated test, not an if/else with a call in each arm:
    // MSVC then materialises the value in a register instead of pushing 0/1.
    FUN_004a1250((Dialog*)&g_game->gui, "TRACKTYPE", !((g_game->flags_37f14.notrak & 1) && g_game->field_37f16 == 4));
}

// 0x45bcc0 (matched in its own file).
void ApplyBrightnessAndVolume()
{
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// 0x45c510 (matched in its own file).
void ApplyTrackType()
{
    Entry_0045d280* gadgets = ((Holder_0045d280*)g_game->gui.holder)->entries;
    if (g_game->field_37f16 == 4) {
        int index = FindGadgetIndex(gadgets, DAT_0050692c, 1);
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(DAT_00512fe0, gadgets[index].value);
    }
}

// 0x45c630 (matched in its own file).
void FUN_0045c630()
{
    g_game->volume2 = 0x20;
    g_game->field_37f16 = 4;
    if (!(g_game->flags_37f14.word & 1)) {
        g_game->flags_37f14.word |= 1;
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    ApplyBrightnessAndVolume();
}

// 0x45c950 (matched in its own file).
void LoadSavedAudioSettings()
{
    g_game->volume2 = DAT_00512f42;
    ((Class_004ce3e0*)g_game->sound)->FUN_004ce3e0(DAT_00512f75);
    g_game->field_37f16 = DAT_00512f48;
    ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
    if (((unsigned char)g_game->flags_37f14.word ^ (unsigned char)DAT_00512f46.i) & 1) {
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    unsigned short f = g_game->flags_37f14.word;
    f = f ^ ((f ^ DAT_00512f46.i) & 1);
    g_game->flags_37f14.word = f;
    ((Class_004ce580*)g_game->sound)->FUN_004ce580(DAT_00512fd9);
    ApplyBrightnessAndVolume();
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
    FUN_0049fa90(obj);
    // Result kept in a local: testing the call directly is longer.
    int notrak = IsCurrentGadgetNamed(obj, DAT_005067bc);
    if (notrak != 0) {              // "NOTRAK"
        PlaySoundByName(DAT_00502b38, 0);
        int v = GetButtonStageByName(obj, DAT_005067bc);
        unsigned short f = g_game->flags_37f14.word;
        g_game->flags_37f14.word = f ^ ((f ^ v) & 1);
        ((Class_004cedc0*)g_game->sound)->EnableCdAudio(g_game->flags_37f14.word & 1);
        FUN_004ab0a0(obj);
        UpdateMusicGadgets();
    } else if (IsCurrentGadgetNamed(obj, DAT_00506984)) {  // "TRACKMODE"
        PlaySoundByName(DAT_00502b38, 0);
        g_game->field_37f16 = GetButtonStageByName(obj, DAT_00506984) + 1;
        ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
        if (g_game->field_37f16 == 3) {
            DAT_00512fe0 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
            FUN_004ab0a0(obj);
            UpdateTrackGadgets();
            return;
        }
        if (g_game->field_37f16 == 4) {
            SetButtonStageByName(obj, DAT_0050692c, (unsigned char)((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(DAT_00512fe0));
            ApplyTrackType();
        }
        FUN_004ab0a0(obj);
        UpdateTrackGadgets();
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_0050692c)) {  // "TRACKTYPE"
        PlaySoundByName(DAT_00502b38, 0);
        // Index with obj->field_60 itself, not the saved copy.
        int i = obj->field_60;
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(DAT_00512fe0, entries[i].value);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_0050696c)) {      // "CDPLAY"
        PlaySoundByName(DAT_00502b38, 0);
        ((Sound*)g_game->sound)->PlayCdTrack(DAT_00512fe0, 1);
        FUN_004ab0a0(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_00506964)) {  // "CDNEXT"
        PlaySoundByName(DAT_00502b38, 0);
        DAT_00512fe0 = DAT_00512fe0 + 1;
        int n = ((Class_004ce450*)g_game->sound)->GetTrackCount();
        if (DAT_00512fe0 > n)
            DAT_00512fe0 = 1;
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->SelectTrack(DAT_00512fe0);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_0050697c)) {  // "CDPREV"
        PlaySoundByName(DAT_00502b38, 0);
        DAT_00512fe0 = DAT_00512fe0 - 1;
        if (DAT_00512fe0 < 1)
            DAT_00512fe0 = ((Class_004ce450*)g_game->sound)->GetTrackCount();
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->SelectTrack(DAT_00512fe0);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_00506974)) {  // "CDSTOP"
        PlaySoundByName(DAT_00502b38, 0);
        ((Class_004ced40*)g_game->sound)->StopCdAudio();
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->SelectTrack(1);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_00506998)) {      // "UNDO"
        PlaySoundByName(DAT_00502b38, 0);
        LoadSavedAudioSettings();
        CloseTopScreen(obj);
        OpenMusicOptions();
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_00506990)) {      // "RESTORE"
        PlaySoundByName(DAT_00502b38, 0);
        FUN_0045c630();
        CloseTopScreen(obj);
        OpenMusicOptions();
        return;
    }
    int save = obj->field_60;
    if (obj->field_60 != -1) {
        if (entries[obj->field_60].state != 1) {
            FUN_004ab0a0(obj);
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
    FUN_0045ce80();
    if (g_game->flags_37ebe.byte & 1) {
        LoadGuiLayer(&g_game->gui, "MUSICRT.GUI", 0x280);
    } else {
        LoadGuiLayer(&g_game->gui, "MUSIC", 0x200);
        LoadPictureCached("optmusic4x", 0, 0, 0);
    }
    obj->callback8 = HandleMusicOptionsClick;
    FUN_0049fa50(&g_game->gui);
    obj->callback1c = FUN_0045d0c0;
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
        DAT_00512fe0 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
    }
    UpdateTrackGadgets();
    Gadget_0045d7c0* gadgets = ((Holder_0045d7c0*)g_game->gui.holder)->gadgets;
    if (g_game->field_37f16 == 4) {
        int index = FindGadgetIndex(gadgets, "TRACKTYPE", 1);
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(DAT_00512fe0, gadgets[index].value);
    }
    FUN_0049fa90(&g_game->gui);
    FUN_0049fb10(&g_game->gui, 1);
    FUN_00428b60();
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x45d9d0
void UpdateSoundGadgets()
{
    SetButtonStageByName((Class_004a1080*)&g_game->gui, DAT_005069d0, g_game->soundFlags.byte & 7);
    FUN_004a0570((Object_004a0570*)&g_game->gui, DAT_005069c8, (g_game->soundFlags.byte & 7) != 0);
    FUN_004a1450((Object_004a1450*)&g_game->gui, DAT_00506884, (g_game->soundFlags.byte & 7) == 0);
    FUN_004a1450((Object_004a1450*)&g_game->gui, DAT_005069c0, (g_game->soundFlags.byte & 7) == 0);
    FUN_004a1450((Object_004a1450*)&g_game->gui, DAT_005069b8, (g_game->soundFlags.byte & 7) == 0);
}

// FUNCTION: 0x45da90
void __stdcall HandleSoundOptionsClick(Object_0045da90* obj)
{
    Entry_0045da90* entries = obj->holder->entries;
    if (obj->field_60 == -1) {
        g_game->flags_37ebe.loadedBits.loaded = 0;
        return;
    }
    FUN_0049fa90(obj);
    int mode = obj->field_60;
    if (IsCurrentGadgetNamed(obj, DAT_005069b8)) {
        PlaySoundByName(DAT_00502b38, 0);
        g_game->soundFlags.bits.speech = entries[obj->field_60].value != 0;
        g_game->field_37f17 = entries[obj->field_60].value * 5;
        FUN_004ab0a0(obj);
    } else if (IsCurrentGadgetNamed(obj, DAT_005069d0)) {
        int v = GetButtonStageByName(obj, DAT_005069d0);
        unsigned short f = g_game->soundFlags.word;
        g_game->soundFlags.word = f ^ ((f ^ v) & 7);
        if ((g_game->soundFlags.word & 7) == 0)
            StopAllSounds();
        if ((g_game->soundFlags.word & 7) == 2)
            ((Sound*)g_game->sound)->Enable3D();
        else
            ((Sound*)g_game->sound)->Disable3D();
        if ((g_game->soundFlags.word & 7) == 1 && !g_game->bits_2a44.prefsWord.prefs)
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
        OpenSoundOptions();
        PlaySoundByName(DAT_00502b38, 0);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_00506990)) {
        g_game->volume1 = 0x1b;
        g_game->soundFlags.bits.b4 = 1;
        g_game->soundFlags.bits.b5 = 1;
        g_game->soundFlags.bits.speech = 1;
        ((Sound*)g_game->sound)->Disable3D();
        g_game->soundFlags.word = (g_game->soundFlags.word & 0xfff9) | 1;
        g_game->field_37f17 = 10;
        SetBrightness(0.5 - g_game->brightness * -0.041666668f);
        ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
        ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
        CloseTopScreen(obj);
        OpenSoundOptions();
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

// FUNCTION: 0x45de30
void OpenSoundOptions()
{
    Object_0045de30* obj = (Object_0045de30*)OpenOptionsLayout();
    RenderLayer(&g_game->gui, 2);
    FUN_0045ce80();
    if (g_game->flags_37ebe.byte & 1) {
        LoadGuiLayer(&g_game->gui, "SOUNDSRT.GUI", 0x200);
    } else {
        LoadGuiLayer(&g_game->gui, "SOUNDS", 0x200);
        LoadPictureCached("optsound4x", 0, 0, 0);
    }
    Entry_0045de30* entries = obj->entries;
    obj->fn = HandleSoundOptionsClick;
    FUN_0049fa50(&g_game->gui);
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
    FUN_004a0570(&g_game->gui, "VOLTEXT", (g_game->soundFlags.word & 7) != 0);
    FUN_004a1450(&g_game->gui, "FXVOL", (g_game->soundFlags.word & 7) == 0);
    FUN_004a1450(&g_game->gui, "TEST", (g_game->soundFlags.word & 7) == 0);
    FUN_004a1450(&g_game->gui, "SPEECH", (g_game->soundFlags.word & 7) == 0);
    FUN_0049fa90(&g_game->gui);
    FUN_0049fb10(&g_game->gui, 1);
    FUN_00428b60();
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
        FUN_0049fa90(gui);
        FUN_004ab0a0(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "BSHADOWS")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.bits.b4 = GetButtonStageByName(gui, "BSHADOWS") & 1;
        g_game->flags_37f06.bits.b3 = g_game->flags_37f06.bits.b4;
        g_game->flags_37f06.bits.b2 = g_game->flags_37f06.bits.b3;
        if (g_game->flags_37ebe.bits.b0)
            g_game->ptr_1437b->FlushCache();
        FUN_0049fa90(gui);
        FUN_004ab0a0(gui);
        return;
    }

    if (IsCurrentGadgetNamed(gui, "SHADING")) {
        PlaySoundByName("Options", 0);
        g_game->flags_37f06.bits.b5 = GetButtonStageByName(gui, "SHADING") & 1;
        if (g_game->flags_37ebe.bits.b0)
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
            FUN_004ab0a0(gui);
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
        FUN_0045ce80();
        if (g_game->flags_37ebe.byte & 1) {
            LoadGuiLayer(&g_game->gui, "VISUALRT.GUI", 0x200);
        } else {
            LoadGuiLayer(&g_game->gui, "VISUALS.GUI", 0x200);
            LoadPictureCached("optvisual4x", i, i, i);
        }
    }
    FUN_0049fa50(&g_game->gui);
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
                    FUN_004a0570(&g_game->gui, ((Holder_0045e5e0*)g_game->gui.holder)->entries[i].name, 0);
                }
            }
            for (i = 0; i <= ((Holder_0045e5e0*)g_game->gui.holder)->entries->count; i++) {
                if (strncmp(((Holder_0045e5e0*)g_game->gui.holder)->entries[i].name, "VID", strlen("VID")) == 0) {
                    FUN_004a0570(&g_game->gui, ((Holder_0045e5e0*)g_game->gui.holder)->entries[i].name, 0);
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

    FUN_0049fb10(&g_game->gui, 1);
    FUN_00428b60();
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x45ead0
void __stdcall FUN_0045ead0(Gadget_0045ead0* gadget)
{
    char* data = gadget->link->data;
    if (gadget->field_60 == -1) {
        g_game->flags_37ebe.word &= 0xfffe;
        return;
    }
    if (IsCurrentGadgetNamed((Gadget_0045ead0*)&g_game->gui, "LEFTCLICK")) {
        PlaySoundByName("Options", 0);
        g_game->field_37efa = GetButtonStageByName((Gadget_0045ead0*)&g_game->gui, "LEFTCLICK");
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed((Gadget_0045ead0*)&g_game->gui, "UNITCHAT")) {
        PlaySoundByName("Options", 0);
        FUN_004ab0a0(gadget);
        g_game->field_37f18 = (unsigned char)(GetButtonStageByName((Gadget_0045ead0*)&g_game->gui, "UNITCHAT") * 5);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "UNDO")) {
        PlaySoundByName("Options", 0);
        g_game->field_37f23 = DAT_00512f55;
        g_game->field_38a4b = DAT_00512f6d.s;
        g_game->field_38a4d = DAT_00512f6d.s;
        g_game->field_1434d = DAT_00512f71.b;
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
    // Index with gadget->field_60 * 347, not the local i: selects the original lea.
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

// FUNCTION: 0x45ed50
void OpenSpeedOptions()
{
    Object_0045ed50* obj = (Object_0045ed50*)OpenOptionsLayout();
    RenderLayer(&g_game->gui, 2);
    FUN_0045ce80();
    if (g_game->flags_37ebe.byte & 1) {
        LoadGuiLayer(&g_game->gui, "SPEEDSRT.GUI", 0x200);
    } else {
        LoadGuiLayer(&g_game->gui, "SPEEDS.GUI", 0x200);
        LoadPictureCached("optinterface4x", 0, 0, 0);
    }
    obj->fn = FUN_0045ead0;
    FUN_0049fa50(&g_game->gui);
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
        int value = g_game->field_1434d;
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
    FUN_004a0bf0(&g_game->gui, "MAXLINESTEXT", text, 0);
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
    FUN_0049fb10(&g_game->gui, 1);
    FUN_00428b60();
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x45f190
void __stdcall FUN_0045f190(Gadget_0045f190* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            PlaySoundByName("Options", 0);
            return;
        }
        FUN_004ab0a0(gadget);
    }
}

// FUNCTION: 0x45f1d0
void FUN_0045f1d0()
{


    Layer_0045f1d0* layer = LoadGuiLayer((Layer_0045f1d0*)&g_game->gui, "GAMEOPTIONS.GUI", 0x1881);
    Entry_0045f1d0* entries = layer->entries;
    layer->handler = FUN_0045f190;
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
                 Translate(((Mission*)g_game->mode)->GetMissionName()), 0x8c, y,
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
    FUN_0049fb10((Layer_0045f1d0*)&g_game->gui, 1);
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
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
    }
    if (gadget->field_60 != -1)
        FUN_004ab0a0(gadget);
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
    FUN_00476d80();
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
    ((Table_0045f8c0*)layer->entries)->count = DAT_00512ef0;
    TdfFile parser;
    char path[256];
    char key[12];
    char value[0x80];
    BuildDataPath(path, "gamedata", "help", "TDF");
    if (((TdfFile*)&parser)->LoadFile(path)) {
        int y = 0x32;
        if (((TdfFile*)&parser)->SelectRecord("Help")) {
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
    FUN_0049fa90(sub);
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
        FUN_004ab0a0(gadget);
    }
}

// FUNCTION: 0x45fb30
void OpenHelpDialog()
{
    Gadget_0045fb30* g = LoadGuiLayer((Sub_0045fb30*)&g_game->gui, "HELP.GUI", 0x1881);
    g->handler = HandleHelpClick;
    LoadPictureCached("dhelp", 0, 0, 0);
    DAT_00512ef0 = g->info->field_b6;
    FillHelpPage((Sub_0045fb30*)&g_game->gui, 0, 0x11);
    FUN_0049fb10((Sub_0045fb30*)&g_game->gui, 1);
    RenderLayer((Sub_0045fb30*)&g_game->gui, 0x40);
}

// FUNCTION: 0x45fbc0
void FUN_0045fbc0()
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
void FUN_0045fc40()
{
    g_game->flags_37ebe.word &= 0xfffe;
    g_game->field_2bc0 = 3;
}

// FUNCTION: 0x45fc60
void __stdcall HandleOptionsPanelClick(Gadget_0045fc60* gadget)
{
    // goto, not a return: the label after the DAT_00506788 = 0 store keeps the
    // teardown tail shared.
    if (gadget->field_60 == -1)
        goto cleanup;
    {
        SetGadgetStatus(gadget, gadget->field_60, 1);
        // Empty then-arm: the positive test alone changes the codegen.
        if (g_game->bits_2a44.bits.bit2) {
        } else {
            RenderLayer((char*)&g_game->gui, 0x40);
            FUN_0049fa90((char*)&g_game->gui);
            FUN_004ab170((char*)&g_game->gui, 0, 0);
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
            DAT_00506788 = 1;
            return;
        } else if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
            PlaySoundByName("Previous", 0);
            FUN_0045c820();
            g_game->volume2 = DAT_00512f42;
            ((Class_004ce3e0*)g_game->sound)->FUN_004ce3e0(&DAT_00512f75);
            g_game->field_37f16 = DAT_00512f48;
            ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
            if (((unsigned char)g_game->flags_37f14.word ^ (unsigned char)DAT_00512f46.s) & 1) {
                ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
            }
            unsigned short f = g_game->flags_37f14.word;
            g_game->flags_37f14.word = f ^ ((f ^ DAT_00512f46.s) & 1);
            ((Class_004ce580*)g_game->sound)->FUN_004ce580(DAT_00512fd9);
            SetBrightness(0.5 - g_game->brightness * -0.041666668f);
            ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
            ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
            g_game->field_37f23 = DAT_00512f55;
            g_game->field_38a4b = DAT_00512f6d.i;
            g_game->field_38a4d = DAT_00512f6d.i;
            g_game->field_1434d = DAT_00512f71.b;
            g_game->field_37efa = DAT_00512f2c;
            g_game->field_37f17 = DAT_00512f49;
            g_game->field_37f18 = DAT_00512f4a;
            g_game->field_37f27 = DAT_00512f59;
            FUN_0045cae0();
            DAT_00506788 = 1;
            return;
        } else if (IsCurrentGadgetNamed(gadget, "SOUND")) {
            PlaySoundByName("Options", 0);
            OpenSoundOptions();
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

// FUNCTION: 0x45ffb0
void __stdcall DrawOptionsScrollBar(void* surf)
{
    if (DAT_00512fe4) {
        if (DAT_00512fec < 0x115) {
            int old = DAT_00512fec;
            DAT_00512fec += 0x15;
            if (DAT_00512fec >= 0x115) {
                PlaySoundByName("Options", 0);
                DAT_00512fec = 0x115;
            }
            if (DAT_00512fec > DAT_00512f14 && old < DAT_00512f14) {
                void* snd = FindGafEntry(g_game->gui.logos32, "LIGHTBAR");
                Sound_45ffb0* s = (Sound_45ffb0*)GetGafFrame(snd, 2);
                DrawFrame((int)DAT_00512fe8, s, s->start, s->end);
            }
        }
        // Test with `<` so the increase is the fallthrough, and use the global
        // directly, not a local: keeps the load before the branch.
        if (DAT_00512fec < DAT_00512f14) {
            DAT_00512ff0 += 6;
        } else {
            DAT_00512ff0 -= 6;
            if (DAT_00512ff0 < 0)
                DAT_00512ff0 = 0;
        }
        Quad_45ffb0 src;
        src.p[0].x = 1;
        src.p[0].y = 1;
        src.p[3].x = 1;
        src.p[1].y = 1;
        Quad_45ffb0 dst;
        if (DAT_00512fec > DAT_00512f14) {
            if (DAT_00512fec < 0x115)
                DAT_00512fec++;
            dst.p[1].x = DAT_00512fec;
            dst.p[2].x = DAT_00512fec;
            dst.p[3].x = DAT_00512f14;
            dst.p[0].x = DAT_00512f14;
            dst.p[0].y = DAT_00512f10;
            dst.p[1].y = DAT_00512f10 - DAT_00512ff0;
        } else {
            dst.p[3].x = DAT_00512fec;
            dst.p[0].x = DAT_00512fec;
            dst.p[1].x = 127;
            dst.p[2].x = 127;
            dst.p[0].y = DAT_00512f10 - DAT_00512ff0;
            dst.p[1].y = DAT_00512f10;
        }
        dst.p[2].y = 479;
        dst.p[3].y = 479;
        src.p[1].x = DAT_00512ef8.w - 1;
        src.p[2].x = DAT_00512ef8.w - 1;
        src.p[2].y = DAT_00512ef8.h - 1;
        src.p[3].y = DAT_00512ef8.h - 1;
        DrawFrameQuad(surf, &DAT_00512ef8, &dst, &src);
        FUN_0049fa90((char*)&g_game->gui);
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
    DAT_00512fe8 = AllocSurface("FLIPSURFACE", holder->field_4->field_17, holder->field_4->field_19);
    memcpy(DAT_00512fe8->pixels, holder->field_4->field_bc->pixels,
           holder->field_4->field_17 * holder->field_4->field_19);
    FrameFromSurface((Dst_004b8ae0*)&DAT_00512ef8, DAT_00512fe8);
    DAT_00512fec = 0;
    DAT_00512f14 = holder->field_4->field_17 - 1;
    DAT_00512f10 = holder->field_4->field_15;
    DAT_00512fe4 = 1;
    DAT_00512ff4 = AllocSurface("BKUPSURFACE", 300, 480);
    DrawSurface(DAT_00512ff4, 0, 0, 0);
    DAT_00512ff0 = 0;
    Layer_00460160* panel = (Layer_00460160*)OpenOptionsLayout();
    if (!g_game->bits_2a44.bits.bit2) {
        LoadPictureCached("options4x", 0, 0, 0);
    }
    panel->handler = HandleOptionsPanelClick;
    memcpy(DAT_00512f18.block, (char*)g_game + 0x37ee6, 0x53);
    DAT_00512f18.bit0 = g_game->los.bits.bit1;
    DAT_00512f18.bit1 = g_game->los.bits.bit2;
    DAT_00512f6d.i = g_game->field_38a4b;
    DAT_00512f71.i = g_game->field_1434d;
    DAT_00512fd9 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
    for (int i = 0; i < 100; i++) {
        DAT_00512f75[i] = ((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(i);
    }
    FUN_0049fb10((Menu_00460160*)&g_game->gui, 1);
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
                FUN_004ab0a0((char*)&g_game->gui);
                return;
            }
            ok = true;
            break;
        case 2:
            if (!FindGameCdDrive(1)) {
                OpenMessageBox((char*)&g_game->gui,
                             Translate("Please insert the Multiplayer CD (Disc 1) and try again"),
                             200, 1, 1);
                FUN_004ab0a0((char*)&g_game->gui);
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
        FUN_004ab0a0(gadget);
    } else if (!IsCurrentGadgetNamed(gadget, "CANCEL") && gadget->field_60 != -1) {
        FUN_004ab0a0(gadget);
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
    FUN_004a0bf0((Menu_004604a0*)&g_game->gui, "MISSIONNAME", (int)first, 0x80);
    char* second = strtok(0, "\n");
    if (second) {
        FUN_004a0bf0((Menu_004604a0*)&g_game->gui, "MISSIONNAME1", (int)second, 0x80);
    }
    FUN_00477410();
    FUN_0049fb10((Menu_004604a0*)&g_game->gui, 1);
    RenderLayer((Menu_004604a0*)&g_game->gui, 0x40);
    FUN_00491c80(0x13);
}

// FUNCTION: 0x4605c0
void __stdcall HandleSurrenderChoice(Gadget_004605c0* gadget)
{
    int owner = gadget->owner->field_4;
    if (gadget->field_60 == -1)
        return;
    PlaySoundByName("Exit", 0);
    if (IsGadgetNamed(owner, gadget->field_60, "CHOICE1")) {
        ((Sound*)g_game->sound)->SetTrackCategory(4);
        switch (DAT_00512ff8) {
        case 0:
        case 1:
            FUN_00491b60();
            FUN_00491d70(1);
            CloseTopScreen(&g_game->gui);
            BlankScreen();
            SetGameMode(1);
            return;
        case 2:
            g_game->flags_3923b |= 4;
            FUN_00491c60();
            return;
        }
    } else if (!IsGadgetNamed(owner, gadget->field_60, "CHOICE2")) {
        FUN_004ab0a0(gadget);
    }
}

// FUNCTION: 0x460680
void OpenSurrenderDialog()
{
    Gadget_00460680* gadget = LoadGuiLayer((Sub_00460680*)&g_game->gui, "YESORNO.GUI", 0x1000);
    if (gadget == 0) {
        return;
    }
    FUN_0049fb10((Sub_00460680*)&g_game->gui, 1);
    char* entries = gadget->entries;
    FindGadgetIndex(entries, "CHOICE1", 1);
    FindGadgetIndex(entries, "CHOICE2", 1);
    // Suspected original bug: the sibling dialog setup at 0x464e70 copies
    // "CHOICE1" to entries+0xcc and "CHOICE2" to entries+0xdc, but this
    // function copies "CHOICE2" (0x503120) into both fields.
    strcpy(entries + 0xcc, "CHOICE2");
    strcpy(entries + 0xdc, "CHOICE2");
    FUN_004a0bf0((Sub_00460680*)&g_game->gui, "CHOICE1", "Yes", 0);
    FUN_004a0bf0((Sub_00460680*)&g_game->gui, "CHOICE2", "No", 0);
    if (DAT_00512ff8 == 0) {
        FUN_004a0bf0((Sub_00460680*)&g_game->gui, "TITLE", "Surrender this battle and return to main menu?", 0);
    } else if (DAT_00512ff8 == 2) {
        const char* title = g_game->flags_2bee.flag4 ? "Exit the Battle"
                                                : "Surrender this battle and exit to Windows?";
        FUN_004a0bf0((Sub_00460680*)&g_game->gui, "TITLE", title, 0);
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
            DAT_00512ff8 = 0;
            CloseTopScreen(gadget);
            OpenSurrenderDialog();
            return;
        }
        if (IsCurrentGadgetNamed(gadget, "EXITGAME")) {
            DAT_00512ff8 = 2;
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
                FUN_004ab0a0(gadget);
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
        FUN_004a0570((Sub_004608b0*)&g_game->gui, "RESTART", 1);
        FUN_004a0bf0((Sub_004608b0*)&g_game->gui, "RESTART", (int)Translate("Restart"), 0x80);
        goto tail;
    }
    // Restart body written twice on purpose: the compiler merges them and lays
    // restart out before main-menu.
    if (g_game->mode->GetGameType() == 2) {
        FUN_004a0570((Sub_004608b0*)&g_game->gui, "RESTART", 1);
        FUN_004a0bf0((Sub_004608b0*)&g_game->gui, "RESTART", (int)Translate("Restart"), 0x80);
        goto tail;
    }
    if (g_game->flags_2bee.flag4) {
        FUN_004a0570((Sub_004608b0*)&g_game->gui, "MAINMENU", 0);
    }
tail:
    FUN_0049fb10((Sub_004608b0*)&g_game->gui, 1);
    RenderLayer((Sub_004608b0*)&g_game->gui, 0x40);
}

// FUNCTION: 0x4609a0
void __cdecl FUN_004609a0(int)
{
    DAT_00512ff8 = 2;
    OpenSurrenderDialog();
}

// FUNCTION: 0x4609b0
void __stdcall HandleInGameOptionsClick(Gadget_004609b0* gadget)
{
    if (gadget->field_60 == -1) {
        FUN_0049fa70((Sub_004609b0*)&g_game->gui);
        DAT_00512fe4 = 0;
        if (DAT_00512fe8) {
            FreeSurface(DAT_00512fe8);
            DAT_00512fe8 = 0;
        }
        if (DAT_00512ff4) {
            FreeSurface(DAT_00512ff4);
            DAT_00512ff4 = 0;
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
        DAT_00512ef0 = g->info->count;
        FillHelpPage((Sub_004609b0*)&g_game->gui, 0, 0x11);
        FUN_0049fb10((Sub_004609b0*)&g_game->gui, 1);
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
            FUN_00476d80();
            RenderLayer((Sub_004609b0*)&g_game->gui, 0x40);
            return;
        }
        FUN_0045f1d0();
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
        FUN_004ab0a0(gadget);
}

// FUNCTION: 0x460cc0
void OpenInGameOptions()
{
    Gadget_00460cc0* gadget = LoadGuiLayer((Gui_00460cc0*)&g_game->gui, "ARMOPT.GUI", 0x800);
    gadget->handler = HandleInGameOptionsClick;
    FUN_004a1200((Gui_00460cc0*)&g_game->gui, FindGadgetIndex(gadget->info, "SAVEGAME", 1),
                 g_game->mode->GetGameType() == 3);
    FUN_004a1200((Gui_00460cc0*)&g_game->gui, FindGadgetIndex(gadget->info, "LOADGAME", 1),
                 g_game->mode->GetGameType() == 3);
    if (g_game->mode->GetGameType() == 3 || g_game->mode->GetGameType() == 2) {
        FUN_004a0bf0((Gui_00460cc0*)&g_game->gui, "MISSION", Translate("Settings"), 0x80);
    }
    FUN_0049fa50((Gui_00460cc0*)&g_game->gui);
    FUN_0049fb10((Gui_00460cc0*)&g_game->gui, 1);
    RenderLayer((Gui_00460cc0*)&g_game->gui, 0x40);
    if (g_game->mode->GetGameType() != 3) {
        g_game->flags_38a51 |= 1;
    }
    ((Class_004ce910*)g_game->sound)->PauseCdAudio(1);
}
