// Decompiled by space-bunny-free. Names are provisional.
// Per player slot: frees the two heap buffers, zeroes four dwords, calls
// FreeSquads on the entry, then retires the unit at +0x74 (FUN_0040b390 on
// the team byte, then the unit's own method and operator delete).
//
// The body walks the slot through a second pointer q anchored at +0x84, with
// every field reached as an index off it (hence the negative indices for the
// unit at +0x74 and the buffer at +0x7c). That is what leaves the loop with
// the two induction variables the original has: the loop pointer in ebx,
// compared against g_game->players + 10, and q in esi, both stepped by
// 0x14b. With the fields written as p->field_84 and so on, MSVC anchors on
// the loop pointer alone and folds esi away.
//
// FUN_0040b390 takes the team byte as an int parameter: the promotion is what
// gives `xor ecx, ecx; mov cl, [esi + 0xc2]` instead of the bare byte load.
//
// The loop bound is `p <= g_game->players + 10`, one past the last slot, so
// the body also runs once on the fields that follow the array (the `ja` guard
// only skips the loop when the array is empty). The original really does
// that; see the note on the original bug below.

#pragma pack(push, 1)
class Class_00408f10 {
public:
    char unknown_0[0x11];
    void* ptrs[10];

    void FUN_00408f10();
};

struct Player_00464a00 {
    char unknown_0[0x74];
    Class_00408f10* unit;               // +0x74
    char unknown_78[0x7c - 0x78];
    void* buffer;                       // +0x7c
    int field_80;                       // +0x80
    int field_84;                       // +0x84
    int field_88;                       // +0x88
    char unknown_8c[0xec - 0x8c];
    void* buffer2;                      // +0xec
    char unknown_f0[0x146 - 0xf0];
    unsigned char team;                 // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00464a00 players[10];        // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FreeSquads(Player_00464a00* param_1);
void __stdcall FUN_0040b390(int param_1);

// FUNCTION: 0x464a00
void FreePlayers()
{
    for (Player_00464a00* p = g_game->players; p <= g_game->players + 10; p++) {
        int* q = (int*)((char*)p + 0x84);
        q[-1] = 0;
        q[0] = 0;
        delete[] ((void**)q)[-2];
        q[1] = 0;
        ((void**)q)[-2] = 0;
        FreeSquads(p);
        if (((Class_00408f10**)q)[-4]) {
            FUN_0040b390(((unsigned char*)q)[0xc2]);
            if (Class_00408f10* u = ((Class_00408f10**)q)[-4]) {
                u->FUN_00408f10();
                delete u;
            }
            ((Class_00408f10**)q)[-4] = 0;
        }
        if (((void**)q)[0x1a]) {
            delete[] ((void**)q)[0x1a];
            ((void**)q)[0x1a] = 0;
        }
    }
}
