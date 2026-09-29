// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Partial, 87.8%, and exactly the original's 560 bytes. All the code matches
// except two instructions in the rectangle fill, plus the jump table that
// follows the code once it does (its bytes only compare equal once the code
// is identical). The original computes "add ecx, eax", giving the sum y + t to
// the register holding y, and emits it after the store of r.top; this version
// computes "add eax, ecx", giving the sum to the register holding t, and
// hoists both leas above that store.
// Fixed in the retry: the inner switch on the team nibble needs a
// "default: show = 0;" (the original's jump table sends 0 and 9..15 to the
// same show = 0 store as 2, 3, 5, 6 and 7), which took 87.2% to 87.8%.
// Tried and rejected (none gives "add ecx, eax"): swapping the operands, all 24
// orders of the four rectangle stores, r.bottom = r.top + t, chained
// (r.top = y) + t, a temporary for the sum or for y, the rectangle as an
// aggregate, an int[4], a constructor-like inline helper (by value, pointer,
// reference, returning the rectangle, containing t and the height as well),
// wrapping each run of adjacent statements in a static inline function (the
// trick that finished 0x4644d0), r or t declared at an outer scope, unsigned
// or long t and y, assignments to t inside the expression, defining the
// preceding function 0x464000 above, and every header set (with the C++
// headers too).
// The mode 1 arm of the switch leaves the flag local uninitialised in the
// original (see the note on the switch below), which this reproduces exactly.
// Retried by deepseek-v4.1-flash: all 24 store orders, four inline setters, a
// Fill method, free helpers taking (r,y,t), (r,t,y) or (r,l,t,rr,b), a helper
// returning the rect, aggregate init and a four-argument constructor,
// intermediates for the sum / y / t, references and pointers to y and
// r.bottom, r.top + t / r.left + t / height + t, r.bottom = t + y, t + y,
// 0 + y + t, y - (-t) and += spellings all compile byte-identically to this
// file. MSVC canonicalises the sum to "add eax, ecx" because t is already in
// eax from _ftol. Sweeping 0 to 2000 unused declarations or prototypes, the
// C++ headers (<string>, <vector>, <map>, <iostream>, <list>), every
// headers.py set, and defining 0x464000 above are all flat at 87.8. The
// remainder is allocator and scheduler state from the original file's earlier
// contents, not a source shape in this block.
#include <math.h>
#pragma pack(push, 1)

struct Entry_00464060 {                 // 0x48 bytes
    char unknown_0[0x46];
    unsigned char unit;                 // +0x46
    unsigned char flags;                // +0x47
};

struct Player_00464060 {                // 0x14b bytes
    char unknown_0[0x27];
    void* data;                         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_00464060 {
    char unknown_0[0xdcb];
    unsigned char colors[16];           // +0xdcb
    char unknown_ddb[0x12ef - 0xddb];
    Entry_00464060 entries[30];         // +0x12ef
    char unknown_1b5f[0x1b63 - 0x1b5f];
    Player_00464060 players[11];         // +0x1b63
    char unknown_299c[0x2a3e - 0x299c];
    unsigned short tail;                // +0x2a3e
    unsigned short head;                // +0x2a40
    char unknown_2a42[0x37efe - 0x2a42];
    int mode;                           // +0x37efe
    int unit_type_mask;                 // +0x37f02
    char unknown_37f06[0x37f27 - 0x37f06];
    int max_lines;                      // +0x37f27
    char unknown_37f2b[0x391f9 - 0x37f2b];
    int font;                           // +0x391f9
};
#pragma pack(pop)

extern Game_00464060* g_game;

struct Rect_00464060 {
    int left, top, right, bottom;
};

void __stdcall FUN_004c1420(int);
void __stdcall FUN_004c13a0(int, int);
int FUN_004c1450();
void __stdcall FUN_00467c00(void*, void*, Rect_00464060*, int);
void __stdcall FUN_004a50e0(void*, void*, int, int, int, int);

// FUNCTION: 0x464060
void __stdcall FUN_00464060(void* surf)
{
    int max_lines = g_game->max_lines;
    if (max_lines == 0)
        return;
    int i = g_game->tail;
    for (int n = 1; n < max_lines; n++) {
        if (i == g_game->head)
            break;
        i--;
        if (i < 0)
            i = 0x1d;
    }
    FUN_004c1420(g_game->font);
    int start = FUN_004c1450();
    if (g_game->tail == i)
        return;
    int y = 0x34;
    while (g_game->tail != i) {
        int show;
        switch (g_game->mode) {
        case 1:
            // Cavedog never assigns show in this arm when the team field is 2,
            // so the test below reads the previous iteration's value. Kept.
            if ((g_game->entries[i].flags & 0xf) != 2)
                show = 0;
            break;
        case 2:
            show = ((g_game->entries[i].flags & 0xf) != 8);
            break;
        case 3:
            if (g_game->unit_type_mask == 0) {
                switch (g_game->entries[i].flags & 0xf) {
                case 1:
                case 4:
                case 8:
                    show = 1;
                    break;
                case 2:
                case 3:
                case 5:
                case 6:
                case 7:
                    show = 0;
                    break;
                default:
                    show = 0;
                    break;
                }
            } else {
                show = 1;
            }
            break;
        default:
            show = 0;
        }
        if (show) {
            if (g_game->entries[i].flags & 0x20)
                FUN_004c13a0(g_game->colors[10], 0xfe);
            else
                FUN_004c13a0(g_game->colors[15], 0xfe);
            int height = 138;
            int id = g_game->entries[i].unit;
            if (id != 10) {
                int t = (int)(FUN_004c1450() * 0.8);
                Rect_00464060 r;
                r.top = y;
                r.left = 138;
                r.right = t + 138;
                r.bottom = y + t;
                height = (int)(138.0 - t * -1.5);
                FUN_00467c00(surf, &g_game->players[id], &r, 0);
            }
            FUN_004a50e0(surf, &g_game->entries[i], height, y, -1, 0);
            y += start;
        }
        i++;
        if (i == 0x1e)
            i = 0;
    }
}
