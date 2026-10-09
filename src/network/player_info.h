// PlayerInfo: the per-player block behind Player+0x27 (Player::info), 0xb9
// bytes that the lobby fills in and BroadcastPlayerInfo sends: the map name,
// the side and colour, the sharing and game flags, the resource settings and
// the version and map checksum used to check that every player agrees. The one
// declaration of the struct for the files that read or write it. The flag
// words have several spellings (a byte, a word, bit fields) because the
// functions that use them match only with the width they were written with.
#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#pragma pack(push, 1)
struct PlayerInfo {
    char map[0x80];                    // +0x00, the map name
    char password[0xb];                // +0x80, the game password; bit 0 of +0x9d says one is set
    unsigned short width;              // +0x8b
    unsigned short height;             // +0x8d
    char unknown_8f;                   // +0x8f
    int id;                            // +0x90
    unsigned char kind;                // +0x94
    unsigned char side;                // +0x95, 1 for Core
    unsigned char color;               // +0x96
    union {                            // +0x97
        unsigned char flags_97;        // bit 0: host
        unsigned short flags_97_wide;
        struct {
            unsigned short ready : 1;
            unsigned short shareMetal : 1;
            unsigned short shareEnergy : 1;
            unsigned short shareLOS : 1;
            unsigned short unknownBit4 : 1;
            unsigned short shareMapping : 1;
            unsigned short shareRadar : 1;
            unsigned short unknownRest : 9;
        };
    };
    unsigned short memory;             // +0x99
    union {                            // +0x9b
        unsigned char flags_9b;
        unsigned short flags;
        struct {
            unsigned short low : 4;
            unsigned short started : 1;
            unsigned short bit5 : 1;
            unsigned short bit6 : 1;   // mask 0x40
            unsigned short watching : 1;
            unsigned short mapping : 1;
            unsigned short los : 1;
            unsigned short losType : 1;
            unsigned short commander : 2;
            unsigned short cheating : 1;
            unsigned short fixedloc : 1;
            unsigned short closed : 1;
        };
    };
    union {                            // +0x9d
        unsigned char flags_9d;
        unsigned short flags_9d_wide;
        struct {
            unsigned short hasPassword : 1;
            unsigned short f9d_1 : 1;
            unsigned short f9d_2 : 1;
            unsigned short f9d_rest : 13;
        };
    };
    char unknown_9f[2];                // +0x9f
    unsigned short energy;             // +0xa1
    unsigned short metal;              // +0xa3
    unsigned short maxUnits;           // +0xa5
    unsigned char versionMajor;        // +0xa7
    unsigned char versionMinor;        // +0xa8
    unsigned int mapCrc;               // +0xa9
    char unknown_ad[0xb9 - 0xad];      // +0xad
};
#pragma pack(pop)

#endif
