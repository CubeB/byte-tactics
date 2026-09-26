// Decompiled by Opus. Names are provisional.
#include <stdio.h>
#include <vector>

struct Unit_0046df40;

struct PlayerSync_0046df40 {                 // 0x5c bytes
    int unknown_0;
    std::vector<Unit_0046df40*> units;       // +0x4
    char unknown_14[0x24 - 0x14];
    int expected;                            // +0x24
    int sent;                                // +0x28
    int ackd;                                // +0x2c
    char unknown_30[0x5c - 0x30];
};

class Class_0046df40 {
public:
    char unknown_0[0x10];
    std::vector<PlayerSync_0046df40> players; // +0x10
    char unknown_20[0x58 - 0x20];
    int active;                              // +0x58

    char* FUN_0046df40();
};

extern char DAT_0051e5a0[];

// FUNCTION: 0x46df40
char* Class_0046df40::FUN_0046df40()
{
    if (active == 0) {
        return 0;
    }
    for (std::vector<PlayerSync_0046df40>::iterator it = players.begin(); it != players.end(); ++it) {
        if (it->expected == 0) {
            return "No units_expected sent from player";
        }
        if (it->units.size() != it->expected) {
            sprintf(DAT_0051e5a0, "expected %d units, got %d", it->expected, it->units.size());
            return DAT_0051e5a0;
        }
        if (it->sent != it->ackd) {
            sprintf(DAT_0051e5a0, "packets sent=%d  ackd=%d", it->sent, it->ackd);
            return DAT_0051e5a0;
        }
    }
    return "OK";
}
