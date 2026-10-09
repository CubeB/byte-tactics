// Settings: the multiplayer game settings block, 0x50 bytes, embedded at
// g_game+0x471 (the name suffixes are the field's offset in Game). It holds
// the game type in field_471, the flags in the union at flags_475 (bit 5 is
// flag_475_5) and the rest. The one declaration of the class for the files
// that read it; net_game.cpp defines what the flags mean.
#ifndef SETTINGS_H
#define SETTINGS_H

#pragma pack(push, 1)

struct Settings {
    int field_471;                     // +0x471
    union {
        int flags_475;                 // +0x475
        struct {
            unsigned int unknown_475_0 : 5;
            unsigned int flag_475_5 : 1;
            unsigned int unknown_475_6 : 26;
        } bits_475;
    };
    char unknown_479[0x48];
};

#pragma pack(pop)

#endif
