// Player: one slot of the player table (Thaldren's PlayerState, 0x14b bytes),
// ten of them at g_game+0x1b63. The one declaration of the struct for the files
// that read or write a slot. packets.cpp, which defines the methods,
// keeps its own view: it also defines the constructor (the slot is built by
// 0x463be0 with a by-value Grid_00463be0 at +0x7c), and a struct with a
// constructor cannot be a member of the unions some Game views put the player
// array in, so the constructor is not declared here. The types behind the
// pointers stay private to their own files.
#ifndef PLAYER_H
#define PLAYER_H

struct PlayerInfo;
struct Unit;
class SquadManager;
struct Group;
struct UnitResources;

#pragma pack(push, 1)
struct Player {
    int active;                        // +0x00, zero when the slot is unused
    unsigned int id;                   // +0x04
    int joinTime;                      // +0x08
    int lobbyDataSynced;                       // +0x0c, a team or a 1 to 10 id: the views disagree
    int messages;                      // +0x10
    int ping;                          // +0x14
    int syncTick;                      // +0x18
    int lastHeard;                     // +0x1c
    unsigned char progress;            // +0x20
    unsigned char keepaliveFlags;      // +0x21
    unsigned char rejectReason;        // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerInfo* info;                  // +0x27
    char name[0x1e];                   // +0x2b
    char fullName[0x1e];               // +0x49
    Unit* unitsBegin;                  // +0x67
    Unit* unitsEnd;                    // +0x6b
    unsigned short firstIndex;         // +0x6f
    unsigned short lastIndex;          // +0x71
    unsigned char type;                // +0x73
    SquadManager* ai;                  // +0x74
    Group* groups;                     // +0x78
    unsigned char* explored;           // +0x7c
    int exploredWidth;                 // +0x80
    int exploredHeight;                // +0x84
    int exploredSize;                  // +0x88
    float energy;                      // +0x8c
    float energyIncome;                // +0x90
    float energyUsage;                 // +0x94
    float metal;                       // +0x98
    float metalIncome;                 // +0x9c
    float metalUsage;                  // +0xa0
    float energyCapacity;              // +0xa4
    float metalCapacity;               // +0xa8
    double totalEnergyProduced;        // +0xac
    double totalMetalProduced;         // +0xb4
    double totalEnergyConsumed;        // +0xbc
    double totalMetalConsumed;         // +0xc4
    double energyWasted;               // +0xcc
    double metalWasted;                // +0xd4
    float energyStorageBonus;          // +0xdc
    float metalStorageBonus;           // +0xe0
    float shareMetal;                  // +0xe4
    float shareEnergy;                 // +0xe8
    UnitResources* econ;               // +0xec
    int updateTime;                    // +0xf0
    int winLoseTime;                   // +0xf4
    int displayTimer;                  // +0xf8
    short kills;                       // +0xfc
    short losses;                      // +0xfe
    unsigned short unused_100;         // +0x100, set to 0xffff when the slot is reset, never read
    unsigned short unused_102;         // +0x102, same
    short commanderKills;              // +0x104
    short commanderLosses;             // +0x106
    unsigned char allied[11];          // +0x108
    unsigned char alliedBy[11];        // +0x113
    unsigned char shareLos[11];        // +0x11e
    unsigned char shareVision[11];     // +0x129
    unsigned char shareMapping[11];    // +0x134
    unsigned char alliance;            // +0x13f
    int unitsCreated;                  // +0x140
    unsigned short unitCount;          // +0x144
    unsigned char index;               // +0x146
    unsigned char startPos;            // +0x147
    unsigned char rank;                // +0x148
    unsigned short flags;              // +0x149

    void FreeSideDataAndFogSightCounts();
    void SetType(int param_1);
};
#pragma pack(pop)

#endif
