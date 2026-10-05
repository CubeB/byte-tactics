// Decompiled by Space Bunny Free. Names are provisional.
// Finds the GAF entry that draws a button-like object: the object's own name
// (copied out of obj->name) is looked up in the screen's GAF, then in the
// object's own GAF, and only when both fail does it fall back to "CHECKBOX",
// "stagebuttn%d" or "BUTTONS0".
//
// The control flow matters for the match: the two loops that clear the w/h of
// every frame and pick the frame nearest to (obj->x, obj->y) sit INSIDE the
// last `if (entry == 0)` block, so a successful name lookup jumps straight to
// the tail at +0x1a1 and skips them. Moving the loops out (to just before
// obj->entry = entry) still gives 468 bytes, but the three early-exit jumps
// then target the top of the loop block instead of the tail and the bytes
// differ.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Gaf_004b8d40 {
    char unknown_0[1];
};

struct GafEntry_004b8d40 {
    unsigned short count;              // +0
    char unknown_2[2];
};

struct Frame_004a7f70 {
    unsigned short x;                  // +0
    unsigned short y;                  // +2
    unsigned short w;                  // +4
    unsigned short h;                  // +6
};

struct Holder_004a7f70 {
    char unknown_0[0xc0];
    Gaf_004b8d40* gaf;                 // +0xc0
};

struct Screen_004a7f70 {
    char unknown_0[4];
    Holder_004a7f70* holder;           // +4
};

struct Button_004a7f70 {
    char unknown_0[4];
    Gaf_004b8d40* gaf;                 // +4
    char unknown_8[0x18 - 8];
    Screen_004a7f70* screen;           // +0x18
};

#pragma pack(push, 1)
struct Obj_004a7f70 {
    char unknown_0[2];
    char name[0x15];                   // +2
    short x;                           // +0x17
    short y;                           // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x2f - 0x1f];
    GafEntry_004b8d40* entry;          // +0x2f
    char unknown_33[0x136 - 0x33];
    unsigned char stage;               // +0x136
    char unknown_137[0x13b - 0x137];
    unsigned char frame;               // +0x13b
};
#pragma pack(pop)

GafEntry_004b8d40* __stdcall FUN_004b8d40(Gaf_004b8d40* gaf, const char* name);
Frame_004a7f70* __stdcall FUN_004b7f30(GafEntry_004b8d40* table, int index);

// FUNCTION: 0x4a7f70
void __stdcall FUN_004a7f70(Button_004a7f70* button, Obj_004a7f70* obj)
{
    char name[0x10];
    char str[0x20];
    int best;
    GafEntry_004b8d40* entry = 0;
    Holder_004a7f70* holder = button->screen->holder;
    strncpy(name, obj->name, 0x10);
    name[0xf] = 0;
    obj->frame = 0;
    Gaf_004b8d40* gaf = holder->gaf;
    if (gaf)
        entry = FUN_004b8d40(gaf, name);
    if (entry == 0) {
        if (button->gaf != 0) {
            entry = FUN_004b8d40(button->gaf, name);
            if (entry == 0) {
                if (obj->flags & 0x80) {
                    entry = FUN_004b8d40(button->gaf, "CHECKBOX");
                } else if (obj->stage != 0) {
                    int n = obj->stage < 4 ? obj->stage : 4;
                    sprintf(str, "stagebuttn%d", n);
                    entry = FUN_004b8d40(button->gaf, str);
                    if (obj->stage == 1) {
                        obj->stage = 2;
                        obj->flags |= 0x4000;
                    }
                } else {
                    strcpy(str, "BUTTONS0");
                    entry = FUN_004b8d40(button->gaf, str);
                }
                if (entry != 0) {
                    best = 1000;
                    for (int i = 0; i < entry->count; i++) {
                        Frame_004a7f70* f = FUN_004b7f30(entry, i);
                        if (f != 0) {
                            f->h = 0;
                            f->w = 0;
                        }
                    }
                    for (int j = 0; j < entry->count; j += 4) {
                        Frame_004a7f70* f = FUN_004b7f30(entry, j);
                        int d = abs(obj->y - f->y) + abs(obj->x - f->x);
                        if (d < best) {
                            obj->frame = (unsigned char)j;
                            best = d;
                        }
                    }
                }
            }
        }
    }
    obj->entry = entry;
    if (entry != 0) {
        Frame_004a7f70* f = FUN_004b7f30(entry, obj->frame);
        if (f != 0) {
            obj->x = f->x;
            obj->y = f->y;
        }
    }
}
