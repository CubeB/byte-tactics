// Decompiled by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
// Opens the multiplayer battle room (LOUNGE2.GUI): resets the room state,
// copies the lobby's command-line options (DAT_00512d68..DAT_00512d8c) or the
// host's game options into the local player's flags, sets up the chat list,
// MEM, START, the METAL/MAXUNITS/ENERGY sliders and the map, then refreshes
// the whole room.
//
// 95.3% (2747 bytes against 2756). Rebuilt from 74.6% on the structure of
// its matched siblings 0x44a680 and 0x44c7e0:
// - The three slider blocks are the slider set-up 0x445e50 (no callers),
//   defined above unannotated and inlined. ENERGY's handler 0x445d60 (no
//   callers) is inlined through it; METAL's 0x445c70 and MAXUNITS' 0x445b70
//   stay calls, as in the original. MAXUNITS passes `g_game->maxUnits - 20`
//   twice (one CSE, homed in the slot `player` used), not a named local.
// - The flag word at +0x9b is the bitfield layout of 0x447b10; the game's
//   mapping/los/losType/commander and the options' fixedloc are ints read
//   for their low bit. f97 (host) and the word at +0x9d are unsigned short
//   bitfields, as in 0x44a680.
// - `info` is read before `player` is formed: that is what stores the
//   player pointer without its 0x1b63 bias, as the original does.
// - The commander write goes through a block-local Player pointer (the
//   original forms the 0x1b63 address and then reads +0x27).
// What still differs: the three bit writes to `cheating` and `fixedloc`
// (bits 13 and 14). In the original their value is computed before the old
// word is masked (value in ecx, word in edx), unlike the other bit writes.
// Declaring those two fields `short` (signed) reproduces that order and the
// registers of the whole region, which is this file, but then MSVC masks
// with `and dh, 0xdf` where the original has `and edx, 0xdfff`. With
// `unsigned short` (93.0%) the masks are right but the word is masked first
// and the registers rotate one step. 0x445ed0 reads both fields unsigned
// (`shr eax, 0xe; and eax, 1`), so the signed type is probably not the real
// one. Tried with no change from 93.0%: casts of the value to short, ushort,
// uint, __int64; `? 1 : 0`, `!!`, `2 == x`, `+ 0`, `| 0`, `* 1`, a comma;
// explicit mask expressions in either operand order on an unsigned or
// signed flags word; a Lobby struct for the DAT_00512d6x globals; the DATs
// as unsigned or long; block-scope externs for them; a pointer to the flag
// struct; the switch cases in other orders. An int local, an inline getter or
// `&& 1` for the value gives the original's order with the value in eax
// instead of ecx (94.8%); an inline setter, a copy of the flag struct and
// int-typed fields (new storage unit at +0x9d) are worse. /Gi and dropping
// <windows.h> are worse too. A 15-minute permuter run from the unsigned
// version found nothing better.
// Claude Opus 5.5 pass (#5475, about 20 minutes, still 95.3%):
// - Reproduced in isolation: this file's head cut down to `info = ...;
//   if (DAT_00512d80) info->b.cheating = DAT_00512d80 == 2;` still shows
//   it. The type of the 16-bit STORE decides both things at once. Storing to
//   a signed short (a signed field, or `info->sw = ...` through a union)
//   gives the original's value-first order, and C2 then treats 0xdfff as
//   the 16-bit constant -8193 and emits `and ah, 0xdf`. Storing to an
//   unsigned short gives the word first and `and edx, 0xdfff`. The mask's
//   spelling inside the expression (0xdfff, ~0x2000, 0xffffdfff, 0xdfffu,
//   (short) or (unsigned short) casts, an (int) cast, the word read as
//   signed or unsigned) and the operand order of the `|` change nothing; only
//   the store's type does. So no spelling of a 16-bit write reaches the
//   original's pair (value first with `and edx, 0xdfff`). A 32-bit unsigned
//   storage unit also puts the value first, but loads a dword. Only a value
//   whose subtree needs more registers (`(DAT_00512d80 + DAT_00512d84) == 2`)
//   puts the value first on an unsigned 16-bit store. The RTM compiler gives
//   the same code as SP3 here. Dummy declaration counts from 10 to 6000 do
//   not move it either.
// - Also no change from 93.0% with unsigned fields: block-scope `extern`
//   declarations of DAT_00512d80/84 after the locals, a reference to the
//   bitfield struct (`Flags& b = info->b;`, in the block or at function
//   scope after `info` is set; 88.1% at function scope), an `unsigned
//   short&` to the flag word (87.8%), each of the nine locals moved first
//   or last, reversed declarations, bool/char/short/unsigned short locals
//   for the value (short: 94.8%), all 256 sets of headers.py, and the
//   neighbouring per-field mixes (only cheating signed: 93.1%; only
//   fixedloc signed: 94.7%; closed signed too: 95.3%).
// - Lead for the next attempt: the store type alone flips C2's order, so
//   look for a construct that stores 16 bits as signed without the byte
//   peephole, or for a reason (register state, an inlined helper doing the
//   store) that the original evaluated the value first with an unsigned
//   store.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_00449bb0 {
    char map[0x8b];                     // +0x00
    unsigned short width;               // +0x8b
    unsigned short height;              // +0x8d
    char unknown_8f[0x97 - 0x8f];
    unsigned short f97_0 : 1;           // +0x97, host
    unsigned short f97_rest : 15;
    unsigned short memory;              // +0x99
    struct {
        unsigned short low : 4;         // +0x9b
        unsigned short started : 1;
        unsigned short ready : 1;
        unsigned short bit6 : 1;
        unsigned short watching : 1;
        unsigned short mapping : 1;
        unsigned short los : 1;
        unsigned short losType : 1;
        unsigned short commander : 2;
        short cheating : 1;             // signed: see the notes at the top
        short fixedloc : 1;
        unsigned short closed : 1;
    } b;
    unsigned short f9d_0 : 2;           // +0x9d
    unsigned short f9d_2 : 1;
    unsigned short f9d_rest : 13;
    char unknown_9f[0xa1 - 0x9f];
    unsigned short energy;              // +0xa1
    unsigned short metal;               // +0xa3
    unsigned short maxUnits;            // +0xa5
    unsigned char versionMajor;         // +0xa7
    unsigned char versionMinor;         // +0xa8
    int mapCrc;                         // +0xa9
};

struct Player_00449bb0 {                // 0x14b bytes
    int active;                         // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00449bb0* info;          // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Gadget_00449bb0;
struct Gui_00449bb0;
typedef void (__stdcall* Callback_00449bb0)(Gui_00449bb0* gui, int index);

struct Gadget_00449bb0 {                // 0x15b bytes
    char unknown_0[0x1b];
    int field_1b;                       // +0x1b
    char unknown_1f[0x23 - 0x1f];
    int colour;                         // +0x23
    char unknown_27[0x29 - 0x27];
    unsigned char visible;              // +0x29
    char unknown_2a[0xb6 - 0x2a];
    union {
        char text[0x10];                // +0xb6
        struct {                        // the layer's first entry
            short count;                // +0xb6
            char unknown_b8[8];
            void* gaf;                  // +0xc0
        } head;
        struct {
            char unknown_b6[8];
            void* frames;               // +0xbe
        } anim;
    };
    short frame;                        // +0xc6
    unsigned int c8_0 : 1;              // +0xc8
    unsigned int c8_rest : 31;
    char unknown_cc[0x138 - 0xcc];
    unsigned short field_138;           // +0x138
    char unknown_13a[0x13c - 0x13a];
    int max;                            // +0x13c
    short value;                        // +0x140
    short unknown_142;
    Callback_00449bb0 callback;         // +0x144
    char unknown_148[2];
    void* game;                         // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Layer_00449bb0 {
    int unknown_0;
    Gadget_00449bb0* entries;           // +0x4
    void* handler;                      // +0x8
    void* game;                         // +0xc
};

struct Gui_00449bb0 {
    char unknown_0[0x18];
    Layer_00449bb0* table;              // +0x18
};

struct Options_00449bb0 {
    char unknown_0[0x118];
    int fixedloc;                       // +0x118
};

struct Game_00449bb0 {
    char unknown_0[0x519];
    Gui_00449bb0 gui;                   // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_00449bb0 players[10];        // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Options_00449bb0* options;          // +0x29a0
    char unknown_29a4[0x2a30 - 0x29a4];
    void* net;                          // +0x2a30
    char unknown_2a34[0x2a42 - 0x2a34];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2a9b - 0x2a43];
    char* chatter;                      // +0x2a9b
    char unknown_2a9f[0x2bee - 0x2a9f];
    unsigned short dirty : 1;           // +0x2bee
    unsigned short dirty_rest : 15;
    char unknown_2bf0[0x2c28 - 0x2bf0];
    int field_2c28[11];                 // +0x2c28
    char unknown_2c54[0x2c74 - 0x2c54];
    unsigned short locked : 1;          // +0x2c74
    unsigned short locked_rest : 15;
    char unknown_2c76[0x37ee8 - 0x2c76];
    unsigned short field_37ee8;         // +0x37ee8
    unsigned short maxUnits;            // +0x37eea
    unsigned short field_37eec;         // +0x37eec
    char unknown_37eee[0x37ef6 - 0x37eee];
    int field_37ef6;                    // +0x37ef6
    char unknown_37efa[0x37f1b - 0x37efa];
    unsigned short width;               // +0x37f1b
    char unknown_37f1d[2];
    unsigned short height;              // +0x37f1f
    char unknown_37f21[0x391e9 - 0x37f21];
    void* map;                          // +0x391e9
    char unknown_391ed[0x39229 - 0x391ed];
    int commander;                      // +0x39229
    int mapping;                        // +0x3922d
    int los;                            // +0x39231
    int losType;                        // +0x39235
};
#pragma pack(pop)

class Class_00435920 { public: int FUN_00435920(); };
class Class_00435a20 { public: int FUN_00435a20(char* map); };
class Class_00435c30 { public: char* FUN_00435c30(); };
class Class_00435c40 { public: bool FUN_00435c40(); };
class Class_00435d30 { public: void FUN_00435d30(int param_1); };
class Class_004373a0 { public: int FUN_004373a0(); };
class Class_0046e000 { public: int FUN_0046e000(); };

extern Game_00449bb0* g_game;
extern int DAT_00512994;
extern unsigned int DAT_0050550c;
extern int DAT_00512764;
extern int DAT_00512d68;
extern int DAT_00512d6c;
extern int DAT_00512d70;
extern int DAT_00512d74;
extern int DAT_00512d78;
extern int DAT_00512d7c;
extern int DAT_00512d80;
extern int DAT_00512d84;
extern int DAT_00512d88;
extern int DAT_00512d8c;
extern char DAT_00512ce8[];
extern char* DAT_00505518[];
extern char* DAT_005054b0[];

char __stdcall FUN_0041d6a0(int side);
int __stdcall FUN_004288d0(const char* name, int param_2, int param_3, int param_4);
void FUN_00428b60();
void FUN_004455b0();
void __stdcall FUN_00445b70(Gui_00449bb0* gui, int index);
void __stdcall FUN_00445c70(Gui_00449bb0* gui, int index);
void FUN_00445ed0();
void FUN_00446a50();
void __stdcall FUN_00447b10(Gadget_00449bb0* gadget);
void FUN_00448c70();
void FUN_00450f90();
void FUN_00451180();
int FUN_00456760();
int FUN_00457a50();
int FUN_0045b660();
void __stdcall FUN_0045b9b0(Gadget_00449bb0* gadget, int value);
int __stdcall FUN_0045ba20(Gadget_00449bb0* gadget);
void __stdcall FUN_0046c8e0(int param_1);
void __stdcall FUN_0049fa90(Gui_00449bb0* gui);
void __stdcall FUN_0049fb10(Gui_00449bb0* gui, int value);
int __stdcall FUN_0049fdf0(Gadget_00449bb0* entries, char* name, int type);
Gadget_00449bb0* __stdcall FUN_0049ff90(Gadget_00449bb0* entries, char* name);
Gadget_00449bb0* __stdcall FUN_004a0180(Gadget_00449bb0* entries, char* name);
Gadget_00449bb0* __stdcall FUN_004a0200(Gadget_00449bb0* entries, char* name);
Gadget_00449bb0* __stdcall FUN_004a0280(Gadget_00449bb0* entries, char* name);
void __stdcall FUN_004a0570(Gui_00449bb0* gui, char* name, int value);
void __stdcall FUN_004a0bf0(Gui_00449bb0* gui, char* name, char* text, int size);
void __stdcall FUN_004a1250(Gui_00449bb0* gui, char* name, int value);
void __stdcall FUN_004a1450(Gui_00449bb0* gui, char* name, int value);
void __stdcall FUN_004a32a0(Gui_00449bb0* gui, char* name, char* text, int count, int flag);
void __stdcall FUN_004a7190(Gui_00449bb0* gui, int index);
void __stdcall FUN_004a81e0(Gui_00449bb0* gui, int value);
Layer_00449bb0* __stdcall FUN_004aa8f0(Gui_00449bb0* gui, const char* name, int size);
int __stdcall FUN_004ab060(Gui_00449bb0* gui, char* name);
void __stdcall FUN_004b6290(char* message);
void* __stdcall FUN_004b8d40(void* gaf, const char* name);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// The ENERGY slider handler at 0x445d60, which has no callers: /Ob2 inlined it.
void __stdcall FUN_00445d60(Gui_00449bb0* gui, int unused)
{
    char text[20];
    Gadget_00449bb0* value = FUN_004a0200(gui->table->entries, "ENERGY");

    if (value != 0) {
        int shown = FUN_0045ba20(value) / 100 * 100;
        PlayerInfo_00449bb0* info;

        _itoa(shown, text, 10);
        FUN_004a0bf0(gui, "ENERGYTEXT", text, 0);
        info = g_game->players[g_game->localPlayer].info;
        info->energy = (unsigned short)(shown / 100);
        if (info->f97_0 & 1) {
            FUN_00450f90();
            FUN_00451180();
        }
    }
}

// The slider set-up at 0x445e50, which has no callers: /Ob2 inlined it.
void __stdcall FUN_00445e50(char* name, int max, int value, Callback_00449bb0 callback)
{
    Gui_00449bb0* gui = &g_game->gui;
    Gadget_00449bb0* gadgets = gui->table->entries;
    int index = FUN_0049fdf0(gadgets, name, 0xe);
    if (index != -1) {
        Gadget_00449bb0* gadget = FUN_004a0200(gadgets, name);
        gadget->max = max;
        gadget->callback = callback;
        gadget->value = value;
        FUN_0045b9b0(gadget, gadget->value);
        gadget->game = g_game;
    }
    callback(gui, index);
    FUN_0049fa90(gui);
}

// FUNCTION: 0x449bb0
void FUN_00449bb0()
{
    int energy = 1000;
    int metal = 1000;
    Layer_00449bb0* layer;
    Player_00449bb0* player;
    PlayerInfo_00449bb0* info;
    Gadget_00449bb0* entries;
    short host;
    short isHost;
    int i;

    DAT_00512994 = 0;
    DAT_0050550c = -1;
    g_game->dirty = 1;
    memset(g_game->field_2c28, 0, sizeof(g_game->field_2c28));
    info = g_game->players[g_game->localPlayer].info;
    player = &g_game->players[g_game->localPlayer];
    host = info->f97_0;
    g_game->field_37ee8 = 0;
    if (host)
        g_game->maxUnits = g_game->field_37eec;
    info->width = g_game->width;
    info->height = g_game->height;
    info->f9d_2 = FUN_0041d6a0(1) != 0;

    layer = FUN_004aa8f0(&g_game->gui, "LOUNGE2.GUI", 0);
    layer->handler = FUN_00447b10;
    layer->game = g_game;
    FUN_004288d0("battleroom", 0, 1, 0);
    entries = g_game->gui.table->entries;
    DAT_00512764 = layer->entries->head.count;

    i = FUN_0049fdf0(entries, "MESSAGE", 3);
    if (i != -1)
        entries[i].field_138 = 0x7f;
    if (!FUN_00457a50()) {
        i = FUN_0049fdf0(entries, "MAP", 1);
        if (i != -1) {
            entries[i].field_1b = 2;
            FUN_004a0bf0(&g_game->gui, "MAP", "View Map", 0);
        }
    }

    if (FUN_0045b660()) {
        if (DAT_00512d6c) {
            info->maxUnits = DAT_00512d6c;
            g_game->maxUnits = DAT_00512d6c;
        }
        g_game->locked = DAT_00512d68 != 0;
        if (DAT_00512d78) {
            int commander = DAT_00512d78 - 1;
            if (commander >= 0 && commander <= 2) {
                g_game->commander = commander;
                g_game->field_37ef6 = commander;
                Player_00449bb0* p = &g_game->players[g_game->localPlayer];
                p->info->b.commander = commander;
            }
        }
        if (DAT_00512d70)
            energy = DAT_00512d70;
        if (DAT_00512d74)
            metal = DAT_00512d74;
        if (DAT_00512d7c) {
            switch (DAT_00512d7c) {
            case 1:
                info->b.los = 1;
                info->b.losType = 1;
                break;
            case 2:
                info->b.los = 1;
                info->b.losType = 0;
                break;
            case 3:
                info->b.los = 0;
                break;
            }
        }
        if (DAT_00512d80)
            info->b.cheating = DAT_00512d80 == 2;
        if (DAT_00512d84)
            info->b.fixedloc = DAT_00512d84 == 1;
        if (DAT_00512d88)
            info->b.mapping = DAT_00512d88 == 1;
        if (DAT_00512d8c)
            info->b.watching = DAT_00512d8c == 2;
    } else if (host) {
        info->b.mapping = g_game->mapping;
        info->b.los = g_game->los;
        info->b.losType = g_game->losType;
        info->b.fixedloc = g_game->options->fixedloc;
        info->b.commander = g_game->commander;
    }

    if (!host || g_game->locked) {
        for (char** p = DAT_00505518; *p; p++)
            FUN_004a1250(&g_game->gui, *p, 1);
    }

    FUN_00445ed0();
    g_game->chatter = (char*)FUN_004d83b0("LOUNGE CHATTER", 0xa00);
    *g_game->chatter = 0;
    {
        Gadget_00449bb0* mem = FUN_004a0180(g_game->gui.table->entries, "MEMx");
        mem->colour = player->info->memory < ((Class_00435920*)g_game->map)->FUN_00435920() ? 0xc : 0;
        sprintf(mem->text, "%d", g_game->players[g_game->localPlayer].info->memory);
    }
    isHost = g_game->players[g_game->localPlayer].info->f97_0;
    FUN_0046c8e0(isHost);
    FUN_004a0570(&g_game->gui, "START", ((Class_0046e000*)g_game->net)->FUN_0046e000());
    FUN_004a1250(&g_game->gui, "START",
                 host && FUN_00456760() && ((Class_0046e000*)g_game->net)->FUN_0046e000() ? 0 : 1);
    FUN_004a1250(&g_game->gui, "RESTRICTIONS", 0);
    FUN_004a32a0(&g_game->gui, "OUTPUT", g_game->chatter, 0, 0);
    {
        Gadget_00449bb0* output = FUN_0049ff90(entries, "OUTPUT");
        output->field_1b |= 0x100;
    }
    FUN_004a0bf0(&g_game->gui, "METALTEXT", "0", 0);
    FUN_004a0bf0(&g_game->gui, "ENERGYTEXT", "0", 0);

    FUN_00445e50("METAL", 0x2711, metal, FUN_00445c70);
    {
        FUN_00445e50("MAXUNITS", g_game->maxUnits - 20, g_game->maxUnits - 20, FUN_00445b70);
    }
    if (!isHost || g_game->locked) {
        FUN_004a1450(&g_game->gui, "MAXUNITS", 1);
        FUN_004a1450(&g_game->gui, "ENERGY", 1);
        FUN_004a1450(&g_game->gui, "METAL", 1);
    }
    FUN_00445e50("ENERGY", 0x2711, energy, FUN_00445d60);

    ((Class_00435d30*)g_game->map)->FUN_00435d30(1);
    if (isHost && FUN_0045b660() && DAT_00512ce8[0])
        ((Class_00435a20*)g_game->map)->FUN_00435a20(DAT_00512ce8);
    if (!((Class_00435c40*)g_game->map)->FUN_00435c40())
        FUN_004b6290("Could not find the multiplayer map!!");
    strcpy(info->map, ((Class_00435c30*)g_game->map)->FUN_00435c30());
    info->mapCrc = ((Class_004373a0*)g_game->map)->FUN_004373a0();
    FUN_00450f90();
    FUN_004a7190(&g_game->gui, FUN_0049fdf0(g_game->gui.table->entries, "MESSAGE", 0xe));

    for (char** p = DAT_005054b0; *p; p++) {
        int k = FUN_0049fdf0(layer->entries, *p, 0xe);
        if (k != -1)
            layer->entries[k].visible = 0;
    }

    FUN_004455b0();
    FUN_00446a50();
    FUN_00448c70();
    FUN_00428b60();

    if (FUN_004ab060(&g_game->gui, "LOUNGE2.GUI")) {
        Gadget_00449bb0* start = FUN_004a0280(layer->entries, "battlestart");
        start->anim.frames = FUN_004b8d40(layer->entries->head.gaf, "battlestart");
        start->frame = 0;
        start->c8_0 = 1;
        FUN_004a0570(&g_game->gui, "battlestart", 1);
    }

    FUN_0049fb10(&g_game->gui, 1);
    FUN_004a81e0(&g_game->gui, 0x40);
}
