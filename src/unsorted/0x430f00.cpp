// Decompiled by space-bunny-free. Names are provisional.
// Writes every option back out to the config tree: the display, sound and
// render settings under the "Total Annihilation" section, the strings
// (password, nickname, game name, image output directory, skirmish map), and
// the per-player skirmish settings under "Total Annihilation\Skirmish".
#include <windows.h>

#pragma pack(push, 1)
struct SkirmishPlayer {
    int controller;
    int side;
    int allyGroup;
    int metal;
    int energy;
    int color;
};

struct Options_00430f00 {
    SkirmishPlayer players[10];          // +0x00, 0x18 bytes each
    char unknown_f0[0x108 - 10 * 0x18];
    int skirmishCommanderDeath;          // +0x108
    int skirmishMapping;                 // +0x10c
    int skirmishLineOfSight;             // +0x110
    int skirmishLOSType;                 // +0x114
    int fixedLocations;                  // +0x118
    char skirmishMap[0x228 - 0x11c];     // +0x11c
    int skirmishDifficulty;              // +0x228
};

struct Flags_00430f00 {
    unsigned short damagebars : 1;       // bit 0
    unsigned short antiAlias : 1;        // bit 1
    unsigned short shadows : 1;          // bit 2
    unsigned short vehicleShadows : 1;   // bit 3
    unsigned short featureShadows : 1;   // bit 4
    unsigned short shading : 1;          // bit 5
    unsigned short ditheredFog : 1;      // bit 6
    unsigned short unused7 : 1;          // bit 7
    unsigned short switchAlt : 1;        // bit 8
};

struct Game_00430f00 {
    char unknown_0[0x10];
    void* sound;                         // +0x10
    char unknown_14[0x29a0 - 0x14];
    Options_00430f00* options;           // +0x29a0
    char unknown_29a4[0x2bc1 - 0x29a4];
    char gameName[0x2bd2 - 0x2bc1];      // +0x2bc1
    char nickname[0x2be3 - 0x2bd2];      // +0x2bd2
    char password[0x1434d - 0x2be3];     // +0x2be3
    unsigned char scrollspeed;           // +0x1434d
    char unknown_1434e[0x37eee - 0x1434e];
    int difficulty;                      // +0x37eee
    int side;                            // +0x37ef2
    char unknown_37ef6[0x37efa - 0x37ef6];
    int interfaceType;                   // +0x37efa
    char unknown_37efe[0x37f02 - 0x37efe];
    int screenchat;                      // +0x37f02
    Flags_00430f00 flags_37f06;          // +0x37f06
    int gamma;                           // +0x37f08
    int fxvol;                           // +0x37f0c
    int musicvol;                        // +0x37f10
    unsigned char musicmode;             // +0x37f14
    char unknown_37f15[0x37f16 - 0x37f15];
    unsigned char cdmode;                // +0x37f16
    unsigned char unitchat;              // +0x37f17
    unsigned char unitchattext;          // +0x37f18
    struct SoundFlags {
        unsigned short soundMode : 3;         // +0x37f19, bits 0..2
        unsigned short restoreVolume : 1;     // bit 3
        unsigned short ackfx : 1;             // bit 4
        unsigned short buildfx : 1;           // bit 5
        unsigned short speechfx : 1;          // bit 6
    };
    SoundFlags soundFlags;               // +0x37f19
    int displaymodeWidth;                // +0x37f1b
    int displaymodeHeight;               // +0x37f1f
    int textscroll;                      // +0x37f23
    int textlines;                       // +0x37f27
    int mousespeed;                      // +0x37f2b
    struct ClockFlags {
        unsigned short unused0 : 6;      // +0x37f2f, bits 0..5
        unsigned short clock : 1;             // bit 6
    };
    ClockFlags clockFlags;               // +0x37f2f
    char unknown_37f31[0x38a4b - 0x37f31];
    unsigned short gamespeed;            // +0x38a4b
    char unknown_38a4d[0x38a53 - 0x38a4d];
    char imageOutputDirectory[0x38c57 - 0x38a53]; // +0x38a53
    int movieOutputRate;                 // +0x38c57
    char unknown_38c5b[0x38c5f - 0x38c5b];
    int imageOutputDirty;                // +0x38c5f
    int movieOutputDirty;                // +0x38c63
    char unknown_38c67[0x38d81 - 0x38c67];
    int numSkirmishPlayers;              // +0x38d81
    char unknown_38d85[0x39219 - 0x38d85];
    int singleCommanderDeath;            // +0x39219
    int singleMapping;                   // +0x3921d
    int singleLineOfSight;               // +0x39221
    int singleLOSType;                   // +0x39225
    int multiCommanderDeath;             // +0x39229
    int multiMapping;                    // +0x3922d
    int multiLineOfSight;                // +0x39231
    int multiLOSType;                    // +0x39235
    char unknown_39239[0x3923d - 0x39239];
    int playMovie;                       // +0x3923d
};
#pragma pack(pop)

class Class_004cf220 {
public:
    int FUN_004cf220();
};

class Class_004cfff0 {
public:
    int FUN_004cfff0();
};

class Class_004d0040 {
public:
    int FUN_004d0040();
};

extern Game_00430f00* g_game;

int __stdcall FUN_004b6a50(char* section, char* name, int value);
int __stdcall FUN_004b6a20(char* section, char* name, char* value);

// FUNCTION: 0x430f00
void FUN_00430f00()
{
    char name[32];
    int i;

    FUN_004b6a50("Total Annihilation", "Interface Type", g_game->interfaceType);
    FUN_004b6a50("Total Annihilation", "DisplaymodeWidth", g_game->displaymodeWidth);
    FUN_004b6a50("Total Annihilation", "DisplaymodeHeight", g_game->displaymodeHeight);
    FUN_004b6a50("Total Annihilation", "side", g_game->side);
    FUN_004b6a50("Total Annihilation", "FixedLocations", g_game->options->fixedLocations);
    FUN_004b6a50("Total Annihilation", "scrollspeed", g_game->scrollspeed);
    FUN_004b6a50("Total Annihilation", "SingleCommanderDeath", g_game->singleCommanderDeath);
    FUN_004b6a50("Total Annihilation", "SingleMapping", g_game->singleMapping);
    FUN_004b6a50("Total Annihilation", "SingleLineOfSight", g_game->singleLineOfSight);
    FUN_004b6a50("Total Annihilation", "SingleLOSType", g_game->singleLOSType);
    FUN_004b6a50("Total Annihilation", "screenchat", g_game->screenchat);
    FUN_004b6a50("Total Annihilation", "damagebars", g_game->flags_37f06.damagebars);
    FUN_004b6a50("Total Annihilation", "Sound Mode", g_game->soundFlags.soundMode);
    FUN_004b6a50("Total Annihilation", "RestoreVolume", g_game->soundFlags.restoreVolume);
    FUN_004b6a50("Total Annihilation", "MixingBuffers",
                 ((Class_004cf220*)g_game->sound)->FUN_004cf220());
    if (g_game->soundFlags.restoreVolume) {
        FUN_004b6a50("Total Annihilation", "WaveOutVolume",
                     ((Class_004cfff0*)g_game->sound)->FUN_004cfff0());
        FUN_004b6a50("Total Annihilation", "CDAudioVolume",
                     ((Class_004d0040*)g_game->sound)->FUN_004d0040());
    }
    FUN_004b6a50("Total Annihilation", "Anti-Alias", g_game->flags_37f06.antiAlias);
    FUN_004b6a50("Total Annihilation", "Shadows", g_game->flags_37f06.shadows);
    FUN_004b6a50("Total Annihilation", "FeatureShadows", g_game->flags_37f06.featureShadows);
    FUN_004b6a50("Total Annihilation", "VehicleShadows", g_game->flags_37f06.vehicleShadows);
    FUN_004b6a50("Total Annihilation", "Shading", g_game->flags_37f06.shading);
    FUN_004b6a50("Total Annihilation", "DitheredFog", g_game->flags_37f06.ditheredFog);
    FUN_004b6a50("Total Annihilation", "Difficulty", g_game->difficulty);
    FUN_004b6a50("Total Annihilation", "Gamma", g_game->gamma);
    FUN_004b6a50("Total Annihilation", "SwitchAlt", g_game->flags_37f06.switchAlt);
    FUN_004b6a20("Total Annihilation", "Password", g_game->password);
    FUN_004b6a20("Total Annihilation", "Nickname", g_game->nickname);
    FUN_004b6a20("Total Annihilation", "Game Name", g_game->gameName);
    if (g_game->imageOutputDirty) {
        FUN_004b6a20("Total Annihilation", "Image Output Directory",
                     g_game->imageOutputDirectory);
        g_game->imageOutputDirty = 0;
    }
    if (g_game->movieOutputDirty) {
        FUN_004b6a50("Total Annihilation", "Movie Output Rate", g_game->movieOutputRate);
        g_game->movieOutputDirty = 0;
    }
    FUN_004b6a50("Total Annihilation", "unitchat", g_game->unitchat);
    FUN_004b6a50("Total Annihilation", "unitchattext", g_game->unitchattext);
    FUN_004b6a50("Total Annihilation", "textlines", g_game->textlines);
    FUN_004b6a50("Total Annihilation", "textscroll", g_game->textscroll);
    FUN_004b6a50("Total Annihilation", "mousespeed", g_game->mousespeed);
    FUN_004b6a50("Total Annihilation", "gamespeed", g_game->gamespeed);
    FUN_004b6a50("Total Annihilation", "clock", g_game->clockFlags.clock);
    FUN_004b6a50("Total Annihilation", "musicmode", g_game->musicmode & 1);
    FUN_004b6a50("Total Annihilation", "cdmode", g_game->cdmode);
    FUN_004b6a50("Total Annihilation", "ackfx", g_game->soundFlags.ackfx);
    FUN_004b6a50("Total Annihilation", "buildfx", g_game->soundFlags.buildfx);
    FUN_004b6a50("Total Annihilation", "speechfx", g_game->soundFlags.speechfx);
    FUN_004b6a50("Total Annihilation", "fxvol", g_game->fxvol);
    FUN_004b6a50("Total Annihilation", "musicvol", g_game->musicvol);
    FUN_004b6a50("Total Annihilation", "MultiCommanderDeath", g_game->multiCommanderDeath);
    FUN_004b6a50("Total Annihilation", "MultiMapping", g_game->multiMapping);
    FUN_004b6a50("Total Annihilation", "MultiLineOfSight", g_game->multiLineOfSight);
    FUN_004b6a50("Total Annihilation", "MultiLOSType", g_game->multiLOSType);
    FUN_004b6a50("Total Annihilation", "SkirmishCommanderDeath",
                 g_game->options->skirmishCommanderDeath);
    FUN_004b6a50("Total Annihilation", "SkirmishMapping", g_game->options->skirmishMapping);
    FUN_004b6a50("Total Annihilation", "SkirmishLineOfSight",
                 g_game->options->skirmishLineOfSight);
    FUN_004b6a50("Total Annihilation", "SkirmishLOSType", g_game->options->skirmishLOSType);
    FUN_004b6a50("Total Annihilation", "SkirmishLocation", g_game->options->fixedLocations);
    FUN_004b6a50("Total Annihilation", "SkirmishDifficulty",
                 g_game->options->skirmishDifficulty);
    FUN_004b6a20("Total Annihilation", "SkirmishMap", g_game->options->skirmishMap);
    for (i = 0; i < g_game->numSkirmishPlayers; i++) {
        wsprintfA(name, "Player%dController", i);
        FUN_004b6a50("Total Annihilation\\Skirmish", name,
                     g_game->options->players[i].controller);
        wsprintfA(name, "Player%dSide", i);
        FUN_004b6a50("Total Annihilation\\Skirmish", name, g_game->options->players[i].side);
        wsprintfA(name, "Player%dColor", i);
        FUN_004b6a50("Total Annihilation\\Skirmish", name, g_game->options->players[i].color);
        wsprintfA(name, "Player%dAllyGroup", i);
        FUN_004b6a50("Total Annihilation\\Skirmish", name,
                     g_game->options->players[i].allyGroup);
        wsprintfA(name, "Player%dMetal", i);
        FUN_004b6a50("Total Annihilation\\Skirmish", name, g_game->options->players[i].metal);
        wsprintfA(name, "Player%dEnergy", i);
        FUN_004b6a50("Total Annihilation\\Skirmish", name,
                     g_game->options->players[i].energy);
    }
    FUN_004b6a50("Total Annihilation", "PlayMovie", g_game->playMovie);
}
