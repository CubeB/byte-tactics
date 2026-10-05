// Decompiled by space-bunny-free. Names are provisional.
// Builds the "FLIPSURFACE" and "BKUPSURFACE" surfaces for the layer at
// g_game+0x531, opens the panel layer (FUN_0045cfc0) with FUN_0045fc60 as its
// handler, saves the game settings the same way FUN_0045cde0 does, then pushes
// the panel menu. The bit at g_game+0x2a44 bit 2 suppresses the two video
// calls.
// The holder at g_game+0x531 is a local, but its +4 layer pointer is not: the
// original re-reads [ebp+4] for every statement that uses it, so each use here
// goes through holder->field_4 again.
#include <string.h>

class Class_004ce5a0 {
public:
    int FUN_004ce5a0();
};

class Class_004ce7e0 {
public:
    unsigned char FUN_004ce7e0(int index);
};

// The surface laid out by FUN_004c69f0: the pixels follow a 0x30-byte header.
struct Class_004c6a60 {
    char unknown_0[0xc];
    char* pixels;                   // +0xc
};

struct Dst_004b8ae0 {
    char unknown_0[0x14];
};

#pragma pack(push, 1)
struct Layer_00460160 {
    char unknown_0[8];
    int (__stdcall* handler)(void*);    // +0x8
    char unknown_c[0x15 - 0xc];
    short field_15;                     // +0x15
    short field_17;                     // +0x17
    short field_19;                     // +0x19
    char unknown_1b[0xbc - 0x1b];
    Class_004c6a60* field_bc;           // +0xbc
};

struct Holder_00460160 {
    char unknown_0[4];
    Layer_00460160* field_4;             // +0x4
};

struct Menu_00460160 {
    char unknown_0[0x18];
};

struct Flags_00460160 {
    unsigned short bit0 : 1;             // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};

struct Game {
    char unknown_0[0x10];
    void* field_10;                      // +0x10
    char unknown_14[0x519 - 0x14];
    Menu_00460160 menu;                  // +0x519
    Holder_00460160* holder;             // +0x531
    char unknown_535[0x2a44 - 0x535];
    Flags_00460160 flags;                // +0x2a44
    char unknown_2a46[0x14281 - 0x2a46];
    unsigned short bit0 : 1;             // +0x14281, bit 0
    unsigned short bit1 : 1;             // +0x14281, bit 1 (mask 2)
    unsigned short bit2 : 1;             // +0x14281, bit 2 (mask 4)
    unsigned short rest : 13;
    char unknown_14283[0x1434d - 0x14283];
    unsigned char field_1434d;           // +0x1434d
    char unknown_1434e[0x37ee6 - 0x1434e];
    char field_37ee6[0x53];              // +0x37ee6
    char unknown_37f39[0x38a4b - 0x37f39];
    unsigned short field_38a4b;          // +0x38a4b
};

// The saved settings block: 0x53 bytes of state, then bits 0 and 1 of the
// word at +0x53 (0x512f6b).
struct Settings_00460160 {
    char block[0x53];                    // +0x0
    unsigned short bit0 : 1;             // +0x53, bit 0
    unsigned short bit1 : 1;             // +0x53, bit 1
    unsigned short rest : 14;
};
#pragma pack(pop)

extern Game* g_game;
extern Class_004c6a60* DAT_00512fe8;
extern Class_004c6a60* DAT_00512ff4;
extern Dst_004b8ae0 DAT_00512ef8;
extern Settings_00460160 DAT_00512f18;
extern int DAT_00512fe4;
extern int DAT_00512f10;
extern int DAT_00512f14;
extern int DAT_00512f6d;
extern int DAT_00512f71;
extern int DAT_00512fec;
extern int DAT_00512ff0;
extern int DAT_00512fd9;
extern char DAT_00512f75[];

void FUN_004257a0();
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
Layer_00460160* FUN_0045cfc0();
int __stdcall FUN_0045fc60(void* gadget);
void __stdcall FUN_0047f1a0(char* str, int flag);
void __stdcall FUN_0049fb10(Menu_00460160* menu, int value);
void __stdcall FUN_004a81e0(Menu_00460160* menu, int value);
void __stdcall FUN_004b8ae0(Dst_004b8ae0* dst, Class_004c6a60* src);
Class_004c6a60* __stdcall FUN_004c69f0(char* name, int width, int height);
void __stdcall FUN_004c6b70(Class_004c6a60* surface, int a, int b, int c);

// FUNCTION: 0x460160
void FUN_00460160()
{
    Holder_00460160* holder = g_game->holder;
    if (!g_game->flags.bit2) {
        FUN_004257a0();
    }
    DAT_00512fe8 = FUN_004c69f0("FLIPSURFACE", holder->field_4->field_17, holder->field_4->field_19);
    memcpy(DAT_00512fe8->pixels, holder->field_4->field_bc->pixels,
           holder->field_4->field_17 * holder->field_4->field_19);
    FUN_004b8ae0(&DAT_00512ef8, DAT_00512fe8);
    DAT_00512fec = 0;
    DAT_00512f14 = holder->field_4->field_17 - 1;
    DAT_00512f10 = holder->field_4->field_15;
    DAT_00512fe4 = 1;
    DAT_00512ff4 = FUN_004c69f0("BKUPSURFACE", 300, 480);
    FUN_004c6b70(DAT_00512ff4, 0, 0, 0);
    DAT_00512ff0 = 0;
    Layer_00460160* panel = FUN_0045cfc0();
    if (!g_game->flags.bit2) {
        FUN_004288d0("options4x", 0, 0, 0);
    }
    panel->handler = FUN_0045fc60;
    memcpy(DAT_00512f18.block, (char*)g_game + 0x37ee6, 0x53);
    DAT_00512f18.bit0 = g_game->bit1;
    DAT_00512f18.bit1 = g_game->bit2;
    DAT_00512f6d = g_game->field_38a4b;
    DAT_00512f71 = g_game->field_1434d;
    DAT_00512fd9 = ((Class_004ce5a0*)g_game->field_10)->FUN_004ce5a0();
    for (int i = 0; i < 100; i++) {
        DAT_00512f75[i] = ((Class_004ce7e0*)g_game->field_10)->FUN_004ce7e0(i);
    }
    FUN_0049fb10(&g_game->menu, 1);
    FUN_004a81e0(&g_game->menu, 0xc0);
    if (g_game->flags.bit2) {
        FUN_0047f1a0("Panel", 0);
    }
}
