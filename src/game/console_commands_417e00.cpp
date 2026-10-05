// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Class_0044f010's override of slot 10 (vtable 0x4fd458, see 0x44f450.cpp),
// although it sits far from the class's other methods in the exe.
// Draws an open polyline stored on the object as 16-bit map points: each
// consecutive pair is converted to screen space (through the 16.16 fixed-point
// helper of 0x417bb0) and one line is drawn. The colour byte comes from a
// two-entry table in game state, selected by bit 0 of the object's field 0x64.

struct Pos_00417bb0 {
    unsigned short x_frac;             // +0x0
    short x;                           // +0x2
    unsigned short y_frac;             // +0x4
    short y;                           // +0x6
    unsigned short z_frac;             // +0x8
    short z;                           // +0xa
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
};
#pragma pack(pop)

extern Game* g_game;

struct Point_00417e00 {
    short x;
    short z;
};

class Class_0044f010 {
public:
    char unknown_4[4];
    char* field_8;                     // +0x8
    Point_00417e00 points[20];         // +0xc
    int count;                         // +0x5c
    char unknown_60[4];
    unsigned char field_64;            // +0x64
    virtual void FUN_0044ef50(void* surface);  // slot 10
};

int __stdcall FUN_00485070(Pos_00417bb0* pos);
// The real callee takes unsigned char; int here reproduces the original's
// loop-invariant widening of the colour byte.
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall FUN_0046b9d0(void* a, short* b, int c, int d);

// FUNCTION: 0x417e00
void Class_0044f010::FUN_0044ef50(void* surface)
{
    FUN_0046b9d0(surface, (short*)(this->field_8 + 0x76), *(int*)(this->field_8 + 0x7e), 0xf);
    unsigned char color = *(unsigned char*)((char*)g_game + 0xdcb + ((this->field_64 & 1) ? 9 : 12));
    for (int i = 0; i < this->count - 1; i++) {
        Pos_00417bb0 p1;
        *(int*)&p1.x_frac = this->points[i].x << 16;
        *(int*)&p1.z_frac = this->points[i].z << 16;
        int h = FUN_00485070(&p1);
        int x1 = p1.x - g_game->scroll_x + 0x80;
        int y1 = p1.z - g_game->scroll_y - (h >> 1) + 0x20;

        Pos_00417bb0 p2;
        *(int*)&p2.x_frac = this->points[i + 1].x << 16;
        *(int*)&p2.z_frac = this->points[i + 1].z << 16;
        h = FUN_00485070(&p2);
        int x2 = p2.x - g_game->scroll_x + 0x80;
        int y2 = p2.z - g_game->scroll_y - (h >> 1) + 0x20;

        FUN_004be950(surface, x1, y1, x2, y2, color);
    }
}
