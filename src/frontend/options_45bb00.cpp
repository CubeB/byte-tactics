// Decompiled by space-bunny-free. Names are provisional.
#include <stdio.h>

// The VIDEOVAL menu entry holds the current resolution text. The mode list
// is searched for the mode matching the current screen size (width compared
// against a local read before the loop, height re-read every iteration), and
// the index of that mode is turned into a step index in field_140, the same
// arithmetic as FUN_0045b9b0.

#pragma pack(push, 1)
struct Mode_0045bb00 {
    int width;                      // +0x0
    int height;                     // +0x4
    char unknown_8[4];
};

struct ModeList_0045bb00 {
    int count;                      // +0x0
    Mode_0045bb00* modes;           // +0x4
};

struct Video_0045bb00 {
    char unknown_0[0x136];
    short field_136;                // +0x136
    char unknown_138[0x13c - 0x138];
    int field_13c;                  // +0x13c
    short field_140;                // +0x140
    char unknown_142[0x14a - 0x142];
    ModeList_0045bb00* field_14a;   // +0x14a
};

struct Entry_0045bb00 {
    char unknown_0[0xb6];
    char text[0x80];                // +0xb6
};

struct Layer_0045bb00 {
    char unknown_0[4];
    Entry_0045bb00* entries;        // +0x4
};

struct Menu_0045bb00 {
    char unknown_0[0x18];
    Layer_0045bb00* layer;          // +0x18
};

struct Game {
    char unknown_0[0x37f1b];
    int width;                      // +0x37f1b
    int height;                     // +0x37f1f
};
#pragma pack(pop)

extern Game* g_game;

Entry_0045bb00* __stdcall FUN_004a0180(Entry_0045bb00* entries, char* name);

// FUNCTION: 0x45bb00
void __stdcall FUN_0045bb00(Menu_0045bb00* param_1, Video_0045bb00* param_2)
{
    int i = 0;
    int count = param_2->field_14a->count;
    if (count > 0) {
        Game* g = g_game;
        int w = g->width;
        Mode_0045bb00* m = param_2->field_14a->modes;
        do {
            if (w != m->width)
                goto next;
            if (g->height != m->height)
                goto next;
            {
                int max = param_2->field_13c;
                int n = i;
                if (n > max)
                    n = max;
                float f = (float)n / (float)max * (param_2->field_136 - 1);
                if (f - (int)f != 0.0f)
                    f += 1.0;
                param_2->field_140 = (short)f;

                Entry_0045bb00* e = FUN_004a0180(param_1->layer->entries, "VIDVAL");
                if (e)
                    sprintf(e->text, "%d X %d", m->width, m->height);
                break;
            }
next:
            i++;
            m++;
        } while (i < count);    }
}
