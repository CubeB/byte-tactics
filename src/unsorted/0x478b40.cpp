// Decompiled by Space Bunny Free. Names are provisional.
// Per-frame update of one taskbar item (the 0x15a-byte record whose callback
// pointer sits at +0xb6, set up by 0x479330): throttled to about 40 times a
// second, it blanks the "SHUTUP" gadget when the game is in one of its modal
// states, then paints the item's rectangle and blits the animation frame
// centred on it. DAT_0051e67c is read as the frame counter the sequencer was
// last advanced on but is never written anywhere in the exe, so the animation
// steps on every call rather than once per frame.
#include <windows.h>

#pragma pack(push, 1)

// A dialog record. The root record of a dialog's gadget array holds the
// surface to draw on.
struct Entry_00478b40 {
    char unknown_0[0xbc];
    void* surface;                     // +0xbc
};

struct Table_00478b40 {
    char unknown_0[4];
    Entry_00478b40* entries;           // +0x4
};

// The window object the GUI entry points take.
struct Window_00478b40 {
    char unknown_0[0x18];
    Table_00478b40* table;             // +0x18
};

// One animation frame: size, then the blit offsets.
struct Frame_00478b40 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    unsigned short xoffset;            // +0x4
    unsigned short yoffset;            // +0x6
};

// A colour of a window's palette, indexed from the window itself.
struct Colour_00478b40 {
    char unknown_0[0x8b2];
    unsigned char colour;              // +0x8b2
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Item_00478b40 {
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    char unknown_1b[0x23 - 0x1b];
    Colour_00478b40* colours;          // +0x23
    char unknown_27[0xbe - 0x27];
    void* gaf;                         // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    short frame;                       // +0xc6
};
#pragma pack(pop)

// Frame-sequence state, the one 0x479430 starts.
struct Anim_00478b40 {
    short index;                       // +0x0
    char unknown_2[6];
};

struct Rect_00478b40 {
    int x1;
    int y1;
    int x2;
    int y2;
};

class Class_004cfba0 {
public:
    int FUN_004cfba0();
};

struct Game_00478b40 {
    char unknown_0[0x10];
    Class_004cfba0* f_0x10;            // +0x10
};

extern Game_00478b40* g_game;
extern Anim_00478b40 DAT_0051e640;
extern unsigned int DAT_0051e67c;
extern unsigned int DAT_0051e680;

unsigned int __cdecl FUN_004b6340();
int __stdcall FUN_004b8b90(Anim_00478b40* anim);
int __stdcall FUN_004b7f30(void* gaf, int frame);
void __stdcall FUN_004b7f90(void* surface, void* frame, int x, int y);
void __stdcall FUN_004bf6f0(void* surface, Rect_00478b40* rect, int colour);
void __stdcall FUN_0049fa90(Window_00478b40* window);
void __stdcall FUN_0049fad0(Window_00478b40* window);
int __stdcall FUN_004a0f60(Window_00478b40* window, char* name);
int __stdcall FUN_004a1080(Window_00478b40* window, char* name, char value);

// FUNCTION: 0x478b40
void __stdcall FUN_00478b40(Window_00478b40* arg1, Item_00478b40* arg2)
{
    void* surface = arg1->table->entries->surface;
    if (DAT_0051e680 <= GetTickCount()) {
        DAT_0051e680 = GetTickCount() + 0x19;
        if (g_game->f_0x10->FUN_004cfba0() == 0) {
            if (FUN_004a0f60(arg1, "SHUTUP")) {
                FUN_004a1080(arg1, "SHUTUP", 0);
                FUN_0049fa90(arg1);
            }
        }
        if (arg2->gaf != 0) {
            FUN_0049fad0(arg1);

            int x1 = arg2->x;
            int x2 = x1 + arg2->w - 1;
            int y1 = arg2->y;
            int y2 = y1 + arg2->h - 1;
            Rect_00478b40 rect;
            rect.x1 = x1;
            rect.x2 = x2;
            rect.y1 = y1;
            rect.y2 = y2;

            if (FUN_004b6340() != DAT_0051e67c) {
                FUN_004b8b90(&DAT_0051e640);
                arg2->frame = DAT_0051e640.index;
            }
            Frame_00478b40* frame = (Frame_00478b40*)FUN_004b7f30(arg2->gaf, arg2->frame);
            if (frame == 0) {
                return;
            }
            frame->xoffset = 0;
            int px = arg2->x + arg2->w / 2 - frame->width / 2;
            frame->yoffset = 0;
            int py = arg2->y + arg2->h / 2 - frame->height / 2;
            unsigned char colour =
                ((unsigned char*)arg2->colours)[(int)arg1 + 0x8b2];
            FUN_004bf6f0(surface, &rect, colour);
            FUN_004b7f90(surface, frame, px, py);
        } else {
            FUN_0049fa90(arg1);
        }
    }
}
