// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Per-frame update of the solar-system gadget (the record whose callback sits
// at +0xb6, set up by 0x479330): jitters the wind speed and its timer, fills
// the SOLARSYSTEM gadget, prints the wind and gravity lines, draws the menu
// word list, then runs the gadget's GAF animation and its "Panmask" overlay.
// Constructs kept exactly as the original: the second fill's colour is read
// through `((unsigned char*)arg2->colours)[(int)arg1 + 0x8b2]`, an index that
// is the window pointer rather than a palette slot (same expression in the
// sibling 0x478b40), and the frame loop runs `k <= n`, one frame past the
// count.
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

struct Entry_00478790 {
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    char unknown_1b[0xbc - 0x1b];
    void* surface;                     // +0xbc
    void* gaf;                         // +0xc0
};

struct Table_00478790 {
    char unknown_0[4];
    Entry_00478790* entries;           // +0x4
};

// The window object the GUI entry points take (arg1).
struct Window_00478790 {
    char unknown_0[0x18];
    Table_00478790* table;             // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour;              // +0x8b2
    char unknown_8b3[0xcca - 0x8b3];
    int field_cca;                     // +0xcca
};

// One animation frame: size, then the blit offsets.
struct Frame_00478790 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    unsigned short xoffset;            // +0x4
    unsigned short yoffset;            // +0x6
};

// A colour table, indexed from the window itself.
struct Colour_00478790 {
    char unknown_0[0x8b2];
    unsigned char colour;              // +0x8b2
};

// The gadget that owns the animation (arg2).
struct Item_00478790 {
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    char unknown_1b[0x23 - 0x1b];
    Colour_00478790* colours;          // +0x23
    char unknown_27[0xbe - 0x27];
    void* gaf;                         // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    short frame;                       // +0xc6
};

struct Rect_00478790 {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct Unit {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
};

struct PlayerEntry_00478790 {
    Unit* unit;                        // +0x0
    char unknown_4[0x14b - 0x4];
};

struct Net_00478790 {
    char unknown_0[0xd34];
    int field_d34;                     // +0xd34
    int field_d38;                     // +0xd38
    int field_d3c;                     // +0xd3c
};

struct Game {
    char unknown_0[0x519];
    Window_00478790 menu;              // +0x519
    char unknown_11e7[0x1b8a - 0x11e7];
    PlayerEntry_00478790 players[10];  // +0x1b8a
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37ef2 - 0x2a43];
    int flag_37ef2;                    // +0x37ef2
    char unknown_37ef6[0x391e9 - 0x37ef6];
    Net_00478790* net;                 // +0x391e9
};

// The colour table at 0x507b70 has a 4-byte stride but only its first byte is
// read.
struct ColourEntry_00478790 {
    unsigned char colour;              // +0x0
    char unknown_1[3];
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e654;
extern int DAT_0051e670;
extern short DAT_0051e674;
extern int DAT_0051e678;
extern ColourEntry_00478790 DAT_00507b70[];

int __stdcall FUN_0049fdf0(Entry_00478790* entries, const char* name, int type);
Entry_00478790* __stdcall FUN_004a0280(Entry_00478790* entries, const char* name);
int __stdcall FUN_004a1810(Entry_00478790* entries, int index);
void __stdcall FUN_0049fa90(Window_00478790* window);
void __stdcall FUN_0049fad0(Window_00478790* window);
unsigned int __cdecl FUN_004b6340();
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
void __stdcall FUN_004b7f90(void* surface, Frame_00478790* frame, int x, int y);
char* __stdcall FUN_004c5740(const char* key);
int FUN_004c13f0();
void __stdcall FUN_004c13a0(int param_1, int param_2);
void __stdcall FUN_004bf6f0(void* surface, Rect_00478790* rect, int colour);
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y,
                            int maxWidth);
void* __stdcall FUN_004b8d40(void* gaf, const char* name);
void __stdcall FUN_004aff00(void* menu);

// FUNCTION: 0x478790
void __stdcall FUN_00478790(Window_00478790* arg1, Item_00478790* arg2)
{
    int windMin = g_game->net->field_d34;
    int windMax = g_game->net->field_d38;
    void* surface = arg1->table->entries->surface;

    if (--DAT_0051e670 <= 0) {
        DAT_0051e654 += rand() % 5 - 2;
        if (DAT_0051e654 < windMin)
            DAT_0051e654 = windMin;
        if (DAT_0051e654 > windMax)
            DAT_0051e654 = windMax;
        DAT_0051e670 = rand() % 0x3f;
    }

    int i = FUN_0049fdf0(arg1->table->entries, "SOLARSYSTEM", 0xe);
    Entry_00478790* g = FUN_004a0280(arg1->table->entries, "SOLARSYSTEM");

    Rect_00478790 rect;
    rect.x1 = g->x;
    rect.y1 = g->y;
    rect.x2 = rect.x1 + g->w - 1;
    rect.y2 = rect.y1 + g->h - 1;

    FUN_004bf6f0(surface, &rect, arg1->colour);
    FUN_004a1810(arg1->table->entries, i);

    char text[0x34];
    sprintf(text, "%s : %d", FUN_004c5740("Wind Speed"), DAT_0051e654);
    FUN_004c13a0(DAT_00507b70[g_game->flag_37ef2].colour, FUN_004c13f0());
    FUN_004c14f0(surface, text, rect.x1 + 0x50, rect.y1 + 0x14,
                 rect.x2 - rect.x1 - 0x50);

    sprintf(text, "%s : %.1f", FUN_004c5740("Gravity"),
            (double)g_game->net->field_d3c * 0.008928571428571428);
    FUN_004c14f0(surface, text, rect.x1 + 0x50, rect.y1 + 0x28,
                 rect.x2 - rect.x1 - 0x50);

    FUN_004aff00(&g_game->menu);

    if (arg2->gaf != 0) {
        int total = 0;
        for (int j = 0; j < *(unsigned short*)arg2->gaf; j++)
            total += ((Frame_00478790*)FUN_004b7f30((unsigned short*)arg2->gaf, j))->width;

        if (DAT_0051e678 < (int)FUN_004b6340()) {
            DAT_0051e674++;
            if ((int)DAT_0051e674 >= total)
                DAT_0051e674 = 0;
            DAT_0051e678 = FUN_004b6340() + 2;
        }

        FUN_0049fad0(arg1);

        Rect_00478790 rect2;
        rect2.x1 = arg2->x;
        rect2.y1 = arg2->y;
        rect2.x2 = arg2->x + arg2->w - 1;
        rect2.y2 = arg2->y + arg2->h - 1;
        void* gaf = arg2->gaf;
        surface = arg1->table->entries->surface;

        int now = (int)FUN_004b6340();
        int idx = now / 3 % *(unsigned short*)gaf;
        arg2->frame = (short)idx;
        Frame_00478790* f = (Frame_00478790*)FUN_004b7f30((unsigned short*)gaf, arg2->frame);
        if (f == 0)
            return;

        unsigned char colour =
            ((unsigned char*)arg2->colours)[(int)arg1 + 0x8b2];
        FUN_004bf6f0(surface, &rect2, colour);

        int x = arg2->x - DAT_0051e674;
        int y = arg2->y;
        int n = *(unsigned short*)arg2->gaf;
        for (int k = 0; k <= n; k++) {
            Frame_00478790* fr =
                (Frame_00478790*)FUN_004b7f30((unsigned short*)arg2->gaf, k % n);
            fr->xoffset = 0;
            fr->yoffset = 0;
            FUN_004b7f90(surface, fr, x, y);
            x += fr->width;
            n = *(unsigned short*)arg2->gaf;
        }

        void* panGaf = arg1->table->entries->gaf;
        void* pan = FUN_004b8d40(panGaf, "Panmask");
        Unit* unit =
            g_game->players[g_game->localPlayer].unit;
        Frame_00478790* pf = (Frame_00478790*)FUN_004b7f30((unsigned short*)pan, unit->side);
        pf->yoffset = 0;
        pf->xoffset = 0;
        FUN_004b7f90(surface, pf, 0, 0);
    } else {
        FUN_0049fa90(arg1);
    }
}
