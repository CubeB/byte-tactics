// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-alpha. Names are provisional.
// MATCH. The last two instructions were the only difference for five earlier
// passes, which all sat at 87.8%: ours hoisted the rectangle fill's two integer
// computations (lea edx,[eax+0x8a] and the y + t sum) above the store of
// r.top and gave the sum t's register (add eax,ecx), while the original stores
// r.top first and then accumulates into y's register (add ecx,eax).
// The lever was a declaration in the enclosing scope, not a spelling of the
// block: `t` belongs to the function, not to the `if (id != 10)` body. With
// `int t` there the store of r.top comes out first and the add drains ecx;
// declared inside the body it does not, whatever the four stores, the sum, the
// operand order, the read-backs, the helpers, the aggregates or the position
// of the height statement are.
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

struct Game {
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

extern Game* g_game;

struct Rect_00464060 {
    int left, top, right, bottom;
};

void __stdcall SetFont(int);
void __stdcall SetTextColors(int, int);
int GetFontHeight();
void __stdcall FUN_00467c00(void*, void*, Rect_00464060*, int);
void __stdcall FUN_004a50e0(void*, void*, int, int, int, int);

// FUNCTION: 0x464060
void __stdcall DrawMessages(void* surf)
{
    int max_lines = g_game->max_lines;
    int t;
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
    SetFont(g_game->font);
    int start = GetFontHeight();
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
            show = (g_game->entries[i].flags & 0xf) != 8;
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
                SetTextColors(g_game->colors[10], 0xfe);
            else
                SetTextColors(g_game->colors[15], 0xfe);
            int height = 138;
            int id = g_game->entries[i].unit;
            if (id != 10) {
                t = (int)(GetFontHeight() * 0.8);
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
