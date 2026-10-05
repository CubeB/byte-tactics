// Decompiled by space-bunny-free. Names are provisional.
// A method of Class_0046d040 (the object at g_game+0x2a30, built by
// 0x46c8e0): it reports whether one player's copy of the shared unit list has
// caught up. The player whose id matches is looked up with FUN_0044fed0, the
// four early "already done" cases are one || chain, and the entry's
// std::vector of 0x5c-byte per-player records (the same records 0x46df40
// reports on) is scanned for the id. The last test of a record is a
// sent == acked comparison written as `return 1; break;`, which is what keeps
// the shared return-0 block the loop's fallthrough instead of a copy of it.
#include <vector>

struct Unit_0046e0b0;

#pragma pack(push, 1)
struct Data_0046e0b0 {
    char unknown_0[0x94];
    unsigned char field_94;              // +0x94
};

struct Player_0046e0b0 {                // 0x14b bytes
    int field_0;                         // +0x0
    char unknown_4[0x27 - 0x4];
    Data_0046e0b0* data;                 // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                  // +0x73
    char unknown_74[0x14b - 0x74];
};
#pragma pack(pop)

struct PlayerSync_0046e0b0 {            // 0x5c bytes
    int id;                              // +0x0
    std::vector<Unit_0046e0b0*> units;   // +0x4
    char unknown_14[0x24 - 0x14];
    int expected;                        // +0x24
    int sent;                            // +0x28
    int ackd;                            // +0x2c
    char unknown_30[0x5c - 0x30];
};

Player_0046e0b0* __stdcall FUN_0044fed0(int id);

class Class_0046d040 {
public:
    char unknown_0[0x10];
    std::vector<PlayerSync_0046e0b0> players;    // +0x10
    char unknown_20[0x58 - 0x20];
    int field_58;                        // +0x58
    char unknown_5c[0x64 - 0x5c];
    int field_64;                        // +0x64

    int FUN_0046e0b0(int id);
};

// FUNCTION: 0x46e0b0
int Class_0046d040::FUN_0046e0b0(int id)
{
    if (field_58 == 0)
        return 0;
    Player_0046e0b0* player;
    if (field_64 != 0
        || (player = FUN_0044fed0(id)) == 0
        || (player->field_0 != 0 && player->type == 3 && player->data->field_94 == 2)
        || (player->field_0 != 0 && player->type == 2))
        return 1;
    for (std::vector<PlayerSync_0046e0b0>::iterator it = players.begin(); it != players.end(); ++it) {
        if (it->id != id)
            continue;
        if (it->expected == 0 || it->units.size() != it->expected)
            return 0;
        if (it->sent == it->ackd)
            return 1;
        break;
    }
    return 0;
}
