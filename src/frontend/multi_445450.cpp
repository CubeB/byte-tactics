// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

// Needed: gives the original SIB base/index order in the renumbering loop.
#include <windows.h>

#pragma pack(push, 1)
// A player slot: the same 0x14b byte record the game keeps in g_game->players
// and the class of the callee below (it writes this->type at +0x73).
class Player_00445450 {
public:
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    int field_27;                      // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

class Player {
public:
    void SetType(int param_1);
};

struct Game {
    char unknown_0[0x1b63];
    Player_00445450 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
};
#pragma pack(pop)

extern Game* g_game;

// Compacts the player list: finds the first slot the renumbering pass would
// call dead, moves the next live slot into it, clears the slot it left, then
// renumbers every slot's field_146.
// FUNCTION: 0x445450
void FUN_00445450()
{
    Player_00445450* p = g_game->players;
    Player_00445450* q = g_game->players + 1;
    Player_00445450* end = g_game->players + 10;
    while (1) {
        if (q >= end && p >= end)
            break;
        // Step over slots that are in use, and over type 4 slots.
        while ((p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10)
               || p->type == 4) {
            if (p >= end)
                break;
            p++;
        }
        q = p + 1;
        // Find the next slot that is in use.
        for (; q->active == 0
               || (q->type != 1 && q->type != 2 && q->type != 3)
               || q->field_146 == 10;
             q++) {
            if (q >= end)
                break;
        }
        if (q >= end)
            break;
        if (p >= end)
            break;
        Player_00445450 tmp = *p;
        *p = *q;
        *q = tmp;
        ((Player*)q)->SetType(0);
        q->active = 0;
        for (int i = 0; i <= 10; i++) {
            // Global read into a local first: moves the reload into ecx.
            Game* g = g_game;
            // Through a byte pointer: avoids a reload via an extra lea.
            unsigned char* f = &g->players[i].field_146;
            if (g->players[i].active != 0
                && (g->players[i].type == 1 || g->players[i].type == 2
                    || g->players[i].type == 3) && *f != 10) {
                *f = (unsigned char)i;
            } else {
                *f = 10;
            }
        }
    }
}
