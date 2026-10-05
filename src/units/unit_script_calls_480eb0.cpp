// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Slot 12 of UnitScript (vtable 0x4fd698); see 0x485e30.cpp and the
// sibling slot 13 (0x481140) for the class and its +0x540 data block.
//
// First the unit's visibility against the local player's map is tested; if the
// unit is not visible nothing happens. Otherwise the unit's state is snapped
// (FUN_0045ab10) and a two-position record is built: with bit 0x100 of `b`
// set, only one position is needed, otherwise two (the second from the six
// dwords the entry's +0xc pointer aims at). The message id `b` then selects
// which list-append helper receives the pair.

#pragma pack(push, 1)

struct Vec3_00480eb0 {
    int x, y, z;
};

struct Unit {
    char unknown_0[0x6a];
    Vec3_00480eb0 pos;                 // +0x6a
};

struct Entry_00480eb0 {                // stride 0x36
    Vec3_00480eb0 pos;                 // +0x00
    int* offset;                       // +0x0c
    char unknown_10[0x36 - 0x10];
};

struct Player_00480eb0 {
    char unknown_0[0x14b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00480eb0 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x1427f - 0x2a44];
    unsigned char limitY;              // +0x1427f
};

struct Data_00480eb0 {
    char unknown_0[0xc];
    Unit* unit;                        // +0x0c
    char unknown_10[0x38 - 0x10];
    Entry_00480eb0 entries[1];         // +0x38
};

#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00465ac0(Player_00480eb0* player, Unit* unit);
void __stdcall FUN_0045ab10(Unit* unit);
void __stdcall FUN_00472330(int, int, int, int, short);
void __stdcall FUN_00472430(int, int, int, short);
void __stdcall FUN_00472530(int, int, int, short);
void __stdcall FUN_00472810(int, short);
void __stdcall FUN_004728f0(int, short);

class UnitScript {
public:
    char unknown_0[0x540];
    Data_00480eb0* data;               // +0x540

    void EmitSfx(int a, int b);
};

// FUNCTION: 0x480eb0
void UnitScript::EmitSfx(int a, int b)
{
    if (!FUN_00465ac0(&g_game->players[g_game->playerIndex], data->unit))
        return;
    FUN_0045ab10(data->unit);

    Vec3_00480eb0 v1;
    Vec3_00480eb0 v2;

    if (b & 0x100) {
        v1 = data->unit->pos;
        v1.x += data->entries[a].pos.x;
        v1.y += data->entries[a].pos.y;
        v1.z -= data->entries[a].pos.z;
    } else {
        // Binding the unit once is load bearing: without it MSVC reloads
        // data->unit for the second copy and the whole allocation shifts.
        Unit* u = data->unit;
        v2 = u->pos;
        v1 = u->pos;
        v1.x += data->entries[a].offset[0];
        v1.y += data->entries[a].offset[1];
        v1.z -= data->entries[a].offset[2];
        v2.x += data->entries[a].offset[3];
        v2.y += data->entries[a].offset[4];
        v2.z -= data->entries[a].offset[5];
    }

    switch (b) {
    case 0:
        FUN_00472330((int)&v1, (int)&v2, 1, 6, 7);
        break;
    case 1:
        FUN_00472330((int)&v1, (int)&v2, 1, 7, 7);
        break;
    case 2:
        FUN_00472430((int)&v1, (int)&v2, 0x10, 2);
        break;
    case 3:
        FUN_00472430((int)&v1, (int)&v2, 8, 2);
        break;
    case 4:
        FUN_00472430((int)&v2, (int)&v1, 0x10, 2);
        break;
    case 5:
        FUN_00472430((int)&v2, (int)&v1, 8, 2);
        break;
    case 0x101:
        FUN_00472810((int)&v1, 9);
        break;
    case 0x102:
        FUN_004728f0((int)&v1, 9);
        break;
    case 0x103:
        v2.x = v1.x;
        v2.y = v1.y;
        v2.z = v1.z;
        v2.y = g_game->limitY << 16;
        FUN_00472530((int)&v1, (int)&v2, 8, 7);
        break;
    }
}
