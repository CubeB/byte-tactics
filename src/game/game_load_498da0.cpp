// Decompiled by space-bunny-free. Names are provisional.
// Turns a screen point into a world position. The caller copies the 24-byte
// view rectangle at g_game+0x2c76 onto its own stack and passes the address, so
// this function sees a snapshot of the view.
//
// If the point is inside the rect at +0x142bb and bit 3 of the flags byte is
// clear, the screen offset from the view origin is simply scaled by world size
// over screen size. Otherwise the point is clamped to the visible limit rect at
// +0x37e27 and measured from the world origin, which lets the camera sit still
// at the edge of the map. The three low flag bits record which of the two
// happened: bit 0 direct, bit 1 inside the limit rect, bit 2 both. Then the
// point is converted to a map position, the cell it landed in is stored, and
// that cell's feature id is stored with it.
//
// Layout notes, for whoever reads the neighbours of this code:
// - the view rectangle is 0x18 bytes (int x, int y, then 16 bytes this
//   function never looks at); only x and y are read.
// - +0x2caa is the world position, three 20.12 fixed-point values. The
//   conversion back to a cell is an unsigned `>> 20` into a pair of shorts at
//   +0x2c8e, written as one 4-byte copy, and the feature id of that cell goes
//   to +0x2cbc.
// - +0x2cc6 is one byte of flags reached through a bitfield union. Writing bit
//   0 hoists the byte into bl and stores it after the arithmetic, while
//   clearing bit 1 and setting bit 2 are straight-to-memory masks. Reading the
//   two bits back for bit 2 gives the `test cl, 3` the original does.
// - +0x37e27 is the visible limit rect (left, top, right, bottom). Its address
//   is taken once and kept in a register, because the same rect is both the
//   clamp bounds and the argument of the second PointInRect call.
// - +0x1422b and +0x1422f are the world size, +0x142e7..+0x142ed the view
//   origin and screen size as shorts, +0x1431f and +0x14323 the world origin
//   the clamped point is measured from.
// - the MIN(MAX()) nesting is not decoration: the original expands the max
//   twice, once for the comparison and once for the value it keeps.
// - `Pos v = *p` with only v.x and v.z read is what leaves the store of the
//   middle word in the frame; MSVC folds the other two into their consumers.
#pragma pack(push, 1)

struct Pos_00498da0 {
    unsigned int x;                    // +0x0
    unsigned int y;                    // +0x4
    unsigned int z;                    // +0x8
};

struct Rect_00498da0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Point_00498da0 {
    short x;                           // +0x0
    short y;                           // +0x2
};

struct BitFlags_00498da0 {
    unsigned char b0:1;
    unsigned char b1:1;
    unsigned char b2:1;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char b6:1;
    unsigned char b7:1;
};

union Flags_00498da0 {
    unsigned char value;
    BitFlags_00498da0 bits;
};

struct View_00498da0 {
    int x;                             // +0x0
    int y;                             // +0x4
    char unknown_8[0x18 - 0x8];
};

struct Cell_00498da0 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned char offsetY;             // +0xa
    unsigned char offsetX;             // +0xb
    char unknown_c;
};

struct Game {
    char unknown_0[0x2c8e];
    Point_00498da0 point;              // +0x2c8e
    char unknown_2c92[0x2caa - 0x2c92];
    Pos_00498da0 pos;                  // +0x2caa
    char unknown_2cb6[0x2cbc - 0x2cb6];
    unsigned short cellFeature;        // +0x2cbc
    char unknown_2cbe[0x2cc6 - 0x2cbe];
    Flags_00498da0 flags;              // +0x2cc6
    char unknown_2cc7[0x1422b - 0x2cc7];
    int worldW;                        // +0x1422b
    int worldH;                        // +0x1422f
    char unknown_14233[0x142bb - 0x14233];
    Rect_00498da0 viewLimit;           // +0x142bb
    char unknown_142cb[0x142e7 - 0x142cb];
    short originX;                     // +0x142e7
    short originY;                     // +0x142e9
    short screenW;                     // +0x142eb
    short screenH;                     // +0x142ed
    char unknown_142ef[0x1431f - 0x142ef];
    int mapOriginX;                    // +0x1431f
    int mapOriginY;                    // +0x14323
    char unknown_14327[0x37e27 - 0x14327];
    Rect_00498da0 lim;                 // +0x37e27
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall PointInRect(Rect_00498da0* r, int x, int y);
void __stdcall FUN_00484b50(int x, int y, Pos_00498da0* out);
Cell_00498da0* __stdcall FUN_00481550(int x, int y);
unsigned short __stdcall FUN_00421e60(Cell_00498da0* cell);

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// FUNCTION: 0x498da0
void __stdcall FUN_00498da0(View_00498da0* r)
{
    int mx, my;

    if (PointInRect(&g_game->viewLimit, r->x, r->y) && !(g_game->flags.value & 8)) {
        mx = (r->x - g_game->originX) * g_game->worldW / g_game->screenW;
        my = (r->y - g_game->originY) * g_game->worldH / g_game->screenH;
        g_game->flags.value |= 1;
        g_game->flags.value &= ~2;
    } else {
        Rect_00498da0* lim = &g_game->lim;
        mx = g_game->mapOriginX + MIN(MAX(r->x, lim->left), lim->right) - lim->left;
        my = g_game->mapOriginY + MIN(MAX(r->y, lim->top), lim->bottom) - lim->top;
        g_game->flags.value &= ~1;
        g_game->flags.bits.b1 = PointInRect(lim, r->x, r->y);
    }
    g_game->flags.bits.b2 = g_game->flags.bits.b0 || g_game->flags.bits.b1;
    FUN_00484b50(mx, my, &g_game->pos);
    {
        Pos_00498da0* p = &g_game->pos;
        Point_00498da0 pt;
        Pos_00498da0 v = *p;
        pt.x = (short)(v.x >> 20);
        pt.y = (short)(v.z >> 20);
        g_game->point = pt;
    }
    g_game->cellFeature = FUN_00421e60(FUN_00481550(g_game->point.x, g_game->point.y));
}
