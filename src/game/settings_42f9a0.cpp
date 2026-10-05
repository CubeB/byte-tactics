// Decompiled by deepseek-v4.1-flash, finished by GPT-6; retry confirmed by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
//
// deepseek-v4.1 retry #3 (1852): MATCH, 5472 of 5472 bytes.
// The last 27 register-name hunks were NOT a free allocator pick. The compare
// fold was solved earlier by writing a call result into a NAMED local before the
// test (`int ok = FUN_004b69d0(...); if (ok != 0)`), because the direct spelling
// `if (FUN_004b69d0(...) != 0)` folds to `test eax,eax`. That trick was applied
// at every early option site, including two where the original does NOT fold:
// Gamma (0x4301a5, `test eax,eax / mov ebx,0xa`) and SwitchAlt (0x430215), where
// the constant 10 kills the ebx zero beforehand, so those two compares fold and
// the tail (PlayMovie, AllMissions, after the skirmish loop) gets a fresh
// `xor esi,esi` zero source instead. Reverting exactly those two sites to the
// direct spelling flipped the early zero register from esi to ebx and removed
// all 27 hunks at once: `xor ebx,ebx` at 0x42f9be, every early `cmp eax,ebx`,
// the two `mov dword ptr [..+0x37efa/0x37ef2], ebx` zero stores, `push ebx`,
// `mov esi,2` at 0x42fdc0 (with `or word ptr [..],si` for the Sound Mode mask)
// and `cmp dword ptr [esp+0x10], esi`. So the esi-vs-ebx pick was not allocator
// state at all, it was caused by those two extra register compares keeping the
// early zero live past 0x4301a5.
// Sites that keep the named-local spelling: the 23 early options up to
// DitheredFog, plus PlayMovie and AllMissions in the tail. Everything else
// (the middle options, the whole multi/skirmish block and the six skirmish
// player-loop sites) uses the direct spelling because the original folds there.

#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct SkirmishPlayer_0042f9a0 {
    int controller;      // +0x00
    int side;            // +0x04
    int allyGroup;       // +0x08
    int metal;           // +0x0c
    int energy;          // +0x10
    int color;           // +0x14
};

struct Options_0042f9a0 {
    SkirmishPlayer_0042f9a0 players[10];         // +0x00, 0x18 bytes each
    char unknown_f0[0x108 - 10 * 0x18];
    int skirmishCommanderDeath;                  // +0x108
    int skirmishMapping;                         // +0x10c
    int skirmishLineOfSight;                     // +0x110
    int skirmishLOSType;                         // +0x114
    int fixedLocations;                          // +0x118
    char skirmishMap[0x228 - 0x11c];             // +0x11c
    int skirmishDifficulty;                      // +0x228
};

struct Flags_0042f9a0 {
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

struct SoundFlags_0042f9a0 {
    unsigned short soundMode : 3;        // bits 0..2
    unsigned short restoreVolume : 1;    // bit 3
    unsigned short ackfx : 1;            // bit 4
    unsigned short buildfx : 1;          // bit 5
    unsigned short speechfx : 1;         // bit 6
};

struct MusicFlags_0042f9a0 {
    unsigned short musicmode : 1;        // bit 0
};

struct ClockFlags_0042f9a0 {
    unsigned short unused0 : 1;          // bit 0
    unsigned short bit1 : 1;             // bit 1
    unsigned short bit2 : 1;             // bit 2
    unsigned short bit3 : 1;             // bit 3
    unsigned short bit4 : 1;             // bit 4
    unsigned short unused5 : 1;          // bit 5
    unsigned short clock : 1;            // bit 6
};

struct MissionFlags_0042f9a0 {
    unsigned short allMissions : 1;      // bit 0
};

struct Game {
    char unknown_0[0xc];
    char* field_c;                       // +0x0c
    void* sound;                         // +0x10
    char unknown_14[0x29a0 - 0x14];
    Options_0042f9a0* options;           // +0x29a0
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
    Flags_0042f9a0 flags_37f06;          // +0x37f06
    int gamma;                           // +0x37f08
    int fxvol;                           // +0x37f0c
    int musicvol;                        // +0x37f10
    MusicFlags_0042f9a0 musicFlags_37f14; // +0x37f14
    unsigned char cdmode;                // +0x37f16
    unsigned char unitchat;              // +0x37f17
    unsigned char unitchattext;          // +0x37f18
    SoundFlags_0042f9a0 soundFlags;      // +0x37f19
    int displaymodeWidth;                // +0x37f1b
    int displaymodeHeight;               // +0x37f1f
    int textscroll;                      // +0x37f23
    int textlines;                       // +0x37f27
    int mousespeed;                      // +0x37f2b
    ClockFlags_0042f9a0 flags_37f2f;     // +0x37f2f
    char unknown_37f31[0x38a4b - 0x37f31];
    unsigned short gamespeed;            // +0x38a4b
    unsigned short gamespeed2;           // +0x38a4d
    char unknown_38a4f[0x38a53 - 0x38a4f];
    char imageOutputDirectory[0x38c57 - 0x38a53]; // +0x38a53
    int movieOutputRate;                 // +0x38c57
    char unknown_38c5b[0x38d7f - 0x38c5b];
    MissionFlags_0042f9a0 missionFlags;  // +0x38d7f
    int numSkirmishPlayers;              // +0x38d81
    char unknown_38d85[0x391e9 - 0x38d85];
    void* campaign;                      // +0x391e9
    char unknown_391ed[0x39219 - 0x391ed];
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

extern Game* g_game;
extern char DAT_00512d48[];
extern char DAT_00512ca8[];

int __stdcall FUN_004b69d0(void* section, void* key, void* value);
int __stdcall FUN_004b69b0(void* section, void* key, void* buf, void* size);
void __stdcall FUN_004b6a20(void* section, void* key, void* value);
void __stdcall FUN_004b6a50(void* section, void* key, int value);
int __stdcall FUN_004b6a80(void* buf);
void __stdcall FUN_00434ab0(int mode);
int __cdecl IsOnlineConfigLoaded();

class Class_004cf210 {
public:
    void SetMaxBuffers(int value);
};

class Class_004cfe80 {
public:
    void Enable3D();
};

class Class_004cfe90 {
public:
    void Disable3D();
};

class Class_004cfea0 {
public:
    int Is3DEnabled();
};

class Class_004d0070 {
public:
    int SetWaveVolume(int value);
};

class Class_004d00d0 {
public:
    int SetAuxVolume(int value, int flag);
};

class Class_00435c30 {
public:
    char* FUN_00435c30();
};

class Class_00435d30 {
public:
    void FUN_00435d30(int arg);
};

// FUNCTION: 0x42f9a0
void LoadSettings()
{
    int value;
    char name[32];
    char buf[256];
    int i;

    int ok1 = FUN_004b69d0("Total Annihilation", "Interface Type", &value);
    if (ok1 != 0) {
        if (value > 1) value = 1;
        g_game->interfaceType = value;
    } else {
        g_game->interfaceType = 0;
        FUN_004b6a50("Total Annihilation", "Interface Type", g_game->interfaceType);
    }
    int ok2 = FUN_004b69d0("Total Annihilation", "DisplaymodeWidth", &value);
    if (ok2 != 0) {
        g_game->displaymodeWidth = value;
    } else {
        g_game->displaymodeWidth = 0x280;
        FUN_004b6a50("Total Annihilation", "DisplaymodeWidth", g_game->displaymodeWidth);
    }
    int ok3 = FUN_004b69d0("Total Annihilation", "DisplaymodeHeight", &value);
    if (ok3 != 0) {
        g_game->displaymodeHeight = value;
    } else {
        g_game->displaymodeHeight = 0x1e0;
        FUN_004b6a50("Total Annihilation", "DisplaymodeHeight", g_game->displaymodeHeight);
    }
    int ok4 = FUN_004b69d0("Total Annihilation", "side", &value);
    if (ok4 != 0) {
        g_game->side = value;
    } else {
        g_game->side = 0;
        FUN_004b6a50("Total Annihilation", "side", g_game->side);
    }
    int ok5 = FUN_004b69d0("Total Annihilation", "Difficulty", &value);
    if (ok5 != 0) {
        g_game->difficulty = value & 0xffff;
    } else {
        g_game->difficulty = 1;
        FUN_004b6a50("Total Annihilation", "Difficulty", g_game->difficulty);
    }
    int ok6 = FUN_004b69d0("Total Annihilation", "scrollspeed", &value);
    if (ok6 != 0) {
        g_game->scrollspeed = (unsigned char)value;
    } else {
        g_game->scrollspeed = 0x20;
        FUN_004b6a50("Total Annihilation", "scrollspeed", g_game->scrollspeed);
    }
    int ok7 = FUN_004b69d0("Total Annihilation", "SingleCommanderDeath", &value);
    if (ok7 != 0) {
        g_game->singleCommanderDeath = value;
    } else {
        g_game->singleCommanderDeath = 1;
        FUN_004b6a50("Total Annihilation", "SingleCommanderDeath", g_game->singleCommanderDeath);
    }
    int ok8 = FUN_004b69d0("Total Annihilation", "SingleMapping", &value);
    if (ok8 != 0) {
        g_game->singleMapping = value;
    } else {
        g_game->singleMapping = 1;
        FUN_004b6a50("Total Annihilation", "SingleMapping", g_game->singleMapping);
    }
    int ok9 = FUN_004b69d0("Total Annihilation", "SingleLineOfSight", &value);
    if (ok9 != 0) {
        g_game->singleLineOfSight = value;
    } else {
        g_game->singleLineOfSight = 1;
        FUN_004b6a50("Total Annihilation", "SingleLineOfSight", g_game->singleLineOfSight);
    }
    int ok10 = FUN_004b69d0("Total Annihilation", "SingleLOSType", &value);
    if (ok10 != 0) {
        g_game->singleLOSType = value;
    } else {
        g_game->singleLOSType = 1;
        FUN_004b6a50("Total Annihilation", "SingleLOSType", g_game->singleLOSType);
    }
    int ok11 = FUN_004b69d0("Total Annihilation", "screenchat", &value);
    if (ok11 != 0) {
        g_game->screenchat = value;
    } else {
        g_game->screenchat = 1;
        FUN_004b6a50("Total Annihilation", "screenchat", g_game->screenchat);
    }
    int ok12 = FUN_004b69d0("Total Annihilation", "damagebars", &value);
    if (ok12 != 0) {
        g_game->flags_37f06.damagebars = value;
    } else {
        g_game->flags_37f06.damagebars = 0;
        FUN_004b6a50("Total Annihilation", "damagebars", g_game->flags_37f06.damagebars);
    }
    int ok13 = FUN_004b69d0("Total Annihilation", "Sound Mode", &value);
    if (ok13 != 0) {
        if (value == 2) {
            ((Class_004cfe80*)g_game->sound)->Enable3D();
        } else {
            ((Class_004cfe90*)g_game->sound)->Disable3D();
        }
        g_game->soundFlags.soundMode = value;
    } else {
        FUN_004b6a50("Total Annihilation", "Sound Mode",
                     (((Class_004cfea0*)g_game->sound)->Is3DEnabled() != 0) + 1);
        g_game->soundFlags.soundMode = 1;
    }
    int ok14 = FUN_004b69d0("Total Annihilation", "MixingBuffers", &value);
    if (ok14 != 0) {
        ((Class_004cf210*)g_game->sound)->SetMaxBuffers(value);
    } else {
        ((Class_004cf210*)g_game->sound)->SetMaxBuffers(8);
    }
    int ok15 = FUN_004b69d0("Total Annihilation", "RestoreVolume", &value);
    if (ok15 != 0) {
        g_game->soundFlags.restoreVolume = value;
    } else {
        g_game->soundFlags.restoreVolume = 0;
    }
    if (g_game->soundFlags.restoreVolume) {
        int ok16 = FUN_004b69d0("Total Annihilation", "WaveOutVolume", &value);
        if (ok16 != 0) {
            ((Class_004d0070*)g_game->sound)->SetWaveVolume(value);
        }
        int ok17 = FUN_004b69d0("Total Annihilation", "CDAudioVolume", &value);
        if (ok17 != 0) {
            ((Class_004d00d0*)g_game->sound)->SetAuxVolume(value, 0);
        }
    }
    int ok18 = FUN_004b69d0("Total Annihilation", "Anti-Alias", &value);
    if (ok18 != 0) {
        g_game->flags_37f06.antiAlias = value;
    } else {
        g_game->flags_37f06.antiAlias = 1;
        FUN_004b6a50("Total Annihilation", "Anti-Alias", g_game->flags_37f06.antiAlias);
    }
    int ok19 = FUN_004b69d0("Total Annihilation", "Shadows", &value);
    if (ok19 != 0) {
        g_game->flags_37f06.shadows = value;
    } else {
        g_game->flags_37f06.shadows = 1;
        FUN_004b6a50("Total Annihilation", "Shadows", g_game->flags_37f06.shadows);
    }
    int ok20 = FUN_004b69d0("Total Annihilation", "FeatureShadows", &value);
    if (ok20 != 0) {
        g_game->flags_37f06.featureShadows = value;
    } else {
        g_game->flags_37f06.featureShadows = 1;
        FUN_004b6a50("Total Annihilation", "FeatureShadows", g_game->flags_37f06.featureShadows);
    }
    int ok21 = FUN_004b69d0("Total Annihilation", "VehicleShadows", &value);
    if (ok21 != 0) {
        g_game->flags_37f06.vehicleShadows = value;
    } else {
        g_game->flags_37f06.vehicleShadows = 1;
        FUN_004b6a50("Total Annihilation", "VehicleShadows", g_game->flags_37f06.vehicleShadows);
    }
    int ok22 = FUN_004b69d0("Total Annihilation", "Shading", &value);
    if (ok22 != 0) {
        g_game->flags_37f06.shading = value;
    } else {
        g_game->flags_37f06.shading = 1;
        FUN_004b6a50("Total Annihilation", "Shading", g_game->flags_37f06.shading);
    }
    int ok23 = FUN_004b69d0("Total Annihilation", "DitheredFog", &value);
    if (ok23 != 0) {
        g_game->flags_37f06.ditheredFog = value;
    } else {
        g_game->flags_37f06.ditheredFog = 0;
        FUN_004b6a50("Total Annihilation", "DitheredFog", g_game->flags_37f06.ditheredFog);
    }
    if (FUN_004b69d0("Total Annihilation", "Gamma", &value) != 0) {
        if (value == 10) {
            g_game->gamma = 0xc;
        } else {
            g_game->gamma = value;
        }
    } else {
        g_game->gamma = 0xc;
        FUN_004b6a50("Total Annihilation", "Gamma", g_game->gamma);
    }
    if (FUN_004b69d0("Total Annihilation", "SwitchAlt", &value) != 0) {
        g_game->flags_37f06.switchAlt = value;
    } else {
        g_game->flags_37f06.switchAlt = 0;
        FUN_004b6a50("Total Annihilation", "SwitchAlt", g_game->flags_37f06.switchAlt);
    }
    value = 0xb;
    if (FUN_004b69b0("Total Annihilation", "Password", g_game->password, &value) == 0) {
        g_game->password[0] = 0;
    }
    if (IsOnlineConfigLoaded() != 0 && DAT_00512d48[0] != 0) {
        g_game->nickname[0] = 0;
        strncat(g_game->nickname, DAT_00512d48, 0x10);
    } else {
        value = 0x11;
        if (FUN_004b69b0("Total Annihilation", "Nickname", g_game->nickname, &value) == 0) {
            g_game->nickname[0] = 0;
        }
    }
    if (DAT_00512ca8[0] != 0) {
        g_game->gameName[0] = 0;
        strncat(g_game->gameName, DAT_00512ca8, 0x10);
    } else {
        value = 0x11;
        if (FUN_004b69b0("Total Annihilation", "Game Name", g_game->gameName, &value) == 0) {
            g_game->gameName[0] = 0;
        }
    }
    value = 0x100;
    if (FUN_004b69b0("Total Annihilation", "Image Output Directory",
                     g_game->imageOutputDirectory, &value) == 0) {
        if (FUN_004b6a80(buf) == 0) {
            strcpy(buf, "user_images");
        }
        sprintf(g_game->imageOutputDirectory, "%s\\%s", g_game->field_c + 0x628, buf);
    }
    if (FUN_004b69d0("Total Annihilation", "Movie Output Rate", &value) != 0) {
        g_game->movieOutputRate = value;
    } else {
        g_game->movieOutputRate = 10;
    }
    if (FUN_004b69d0("Total Annihilation", "textlines", &value) != 0) {
        g_game->textlines = value;
    } else {
        g_game->textlines = 10;
    }
    if (FUN_004b69d0("Total Annihilation", "textscroll", &value) != 0) {
        g_game->textscroll = value;
    } else {
        g_game->textscroll = 10;
    }
    if (FUN_004b69d0("Total Annihilation", "mousespeed", &value) != 0) {
        g_game->mousespeed = value;
    } else {
        g_game->mousespeed = 10;
    }
    if (FUN_004b69d0("Total Annihilation", "gamespeed", &value) != 0) {
        g_game->gamespeed = (unsigned short)value;
    } else {
        g_game->gamespeed = 10;
    }
    g_game->gamespeed2 = g_game->gamespeed;
    if (FUN_004b69d0("Total Annihilation", "unitchat", &value) != 0) {
        g_game->unitchat = (unsigned char)value;
    } else {
        g_game->unitchat = 10;
    }
    if (FUN_004b69d0("Total Annihilation", "unitchattext", &value) != 0) {
        g_game->unitchattext = (unsigned char)value;
    } else {
        g_game->unitchattext = 5;
    }
    if (FUN_004b69d0("Total Annihilation", "musicmode", &value) != 0) {
        g_game->musicFlags_37f14.musicmode = value;
    } else {
        g_game->musicFlags_37f14.musicmode = 1;
    }
    if (FUN_004b69d0("Total Annihilation", "cdmode", &value) != 0) {
        g_game->cdmode = (unsigned char)value;
    } else {
        g_game->cdmode = 4;
    }
    if (FUN_004b69d0("Total Annihilation", "ackfx", &value) != 0) {
        g_game->soundFlags.ackfx = value;
    } else {
        g_game->soundFlags.ackfx = 1;
    }
    if (FUN_004b69d0("Total Annihilation", "buildfx", &value) != 0) {
        g_game->soundFlags.buildfx = value;
    } else {
        g_game->soundFlags.buildfx = 1;
    }
    if (FUN_004b69d0("Total Annihilation", "speechfx", &value) != 0) {
        g_game->soundFlags.speechfx = value;
    } else {
        g_game->soundFlags.speechfx = 1;
    }
    if (FUN_004b69d0("Total Annihilation", "fxvol", &value) != 0) {
        g_game->fxvol = value;
    } else {
        g_game->fxvol = 0x1b;
    }
    if (FUN_004b69d0("Total Annihilation", "musicvol", &value) != 0) {
        g_game->musicvol = value;
    } else {
        g_game->musicvol = 0x20;
    }
    if (FUN_004b69d0("Total Annihilation", "clock", &value) != 0) {
        g_game->flags_37f2f.clock = value;
    } else {
        g_game->flags_37f2f.clock = 0;
    }
    if (FUN_004b69d0("Total Annihilation", "NumSkirmishPlayers", &value) != 0) {
        if (value > 1 && value <= 10) {
            g_game->numSkirmishPlayers = value;
        } else {
            g_game->numSkirmishPlayers = value;
        }
    } else {
        g_game->numSkirmishPlayers = 4;
    }
    if (FUN_004b69d0("Total Annihilation", "MultiCommanderDeath", &value) != 0) {
        g_game->multiCommanderDeath = value;
    } else {
        g_game->multiCommanderDeath = 1;
        FUN_004b6a50("Total Annihilation", "MultiCommanderDeath", g_game->multiCommanderDeath);
    }
    if (FUN_004b69d0("Total Annihilation", "MultiMapping", &value) != 0) {
        g_game->multiMapping = value;
    } else {
        g_game->multiMapping = 1;
        FUN_004b6a50("Total Annihilation", "MultiMapping", g_game->multiMapping);
    }
    if (FUN_004b69d0("Total Annihilation", "MultiLineOfSight", &value) != 0) {
        g_game->multiLineOfSight = value;
    } else {
        g_game->multiLineOfSight = 1;
        FUN_004b6a50("Total Annihilation", "MultiLineOfSight", g_game->multiLineOfSight);
    }
    if (FUN_004b69d0("Total Annihilation", "MultiLOSType", &value) != 0) {
        g_game->multiLOSType = value;
    } else {
        g_game->multiLOSType = 1;
        FUN_004b6a50("Total Annihilation", "MultiLOSType", g_game->multiLOSType);
    }
    if (FUN_004b69d0("Total Annihilation", "SkirmishCommanderDeath", &value) != 0) {
        g_game->options->skirmishCommanderDeath = value;
    } else {
        g_game->options->skirmishCommanderDeath = 1;
        FUN_004b6a50("Total Annihilation", "SkirmishCommanderDeath",
                     g_game->options->skirmishCommanderDeath);
    }
    if (FUN_004b69d0("Total Annihilation", "SkirmishMapping", &value) != 0) {
        g_game->options->skirmishMapping = value;
    } else {
        g_game->options->skirmishMapping = 1;
        FUN_004b6a50("Total Annihilation", "SkirmishMapping", g_game->options->skirmishMapping);
    }
    if (FUN_004b69d0("Total Annihilation", "SkirmishLineOfSight", &value) != 0) {
        g_game->options->skirmishLineOfSight = value;
    } else {
        g_game->options->skirmishLineOfSight = 1;
        FUN_004b6a50("Total Annihilation", "SkirmishLineOfSight",
                     g_game->options->skirmishLineOfSight);
    }
    if (FUN_004b69d0("Total Annihilation", "SkirmishLOSType", &value) != 0) {
        g_game->options->skirmishLOSType = value;
    } else {
        g_game->options->skirmishLOSType = 1;
        FUN_004b6a50("Total Annihilation", "SkirmishLOSType",
                     g_game->options->skirmishLOSType);
    }
    if (FUN_004b69d0("Total Annihilation", "SkirmishDifficulty", &value) != 0) {
        g_game->options->skirmishDifficulty = value & 0xffff;
    } else {
        g_game->options->skirmishDifficulty = 1;
        FUN_004b6a50("Total Annihilation", "SkirmishDifficulty",
                     g_game->options->skirmishDifficulty);
    }
    if (FUN_004b69d0("Total Annihilation", "SkirmishLocation", &value) != 0) {
        g_game->options->fixedLocations = value;
    } else {
        g_game->options->fixedLocations = 1;
        FUN_004b6a50("Total Annihilation", "SkirmishLocation",
                     g_game->options->fixedLocations);
    }
    value = 0x100;
    if (FUN_004b69b0("Total Annihilation", "SkirmishMap",
                     g_game->options->skirmishMap, &value) == 0) {
        FUN_00434ab0(2);
        ((Class_00435d30*)g_game->campaign)->FUN_00435d30(0);
        strncpy(g_game->options->skirmishMap,
                ((Class_00435c30*)g_game->campaign)->FUN_00435c30(), 0x100);
        FUN_00434ab0(0);
        FUN_004b6a20("Total Annihilation", "SkirmishMap", g_game->options->skirmishMap);
    }
    for (i = 0; i < g_game->numSkirmishPlayers; i++) {
        wsprintfA(name, "Player%dController", i);
        if (FUN_004b69d0("Total Annihilation\\Skirmish", name, &value) != 0) {
            g_game->options->players[i].controller = value;
        } else {
            g_game->options->players[i].controller = 0;
        }
        wsprintfA(name, "Player%dSide", i);
        if (FUN_004b69d0("Total Annihilation\\Skirmish", name, &value) != 0) {
            g_game->options->players[i].side = value;
        } else {
            g_game->options->players[i].side = i % 2;
        }
        wsprintfA(name, "Player%dColor", i);
        if (FUN_004b69d0("Total Annihilation\\Skirmish", name, &value) != 0) {
            g_game->options->players[i].color = value;
        } else {
            g_game->options->players[i].color = i;
        }
        wsprintfA(name, "Player%dAllyGroup", i);
        if (FUN_004b69d0("Total Annihilation\\Skirmish", name, &value) != 0) {
            g_game->options->players[i].allyGroup = value;
        } else {
            g_game->options->players[i].allyGroup = 5;
        }
        wsprintfA(name, "Player%dMetal", i);
        if (FUN_004b69d0("Total Annihilation\\Skirmish", name, &value) != 0) {
            g_game->options->players[i].metal = value;
        } else {
            g_game->options->players[i].metal = 1000;
        }
        wsprintfA(name, "Player%dEnergy", i);
        if (FUN_004b69d0("Total Annihilation\\Skirmish", name, &value) != 0) {
            g_game->options->players[i].energy = value;
        } else {
            g_game->options->players[i].energy = 1000;
        }
    }
    int ok26 = FUN_004b69d0("Total Annihilation", "PlayMovie", &value);
    if (ok26 != 0) {
        g_game->playMovie = value;
    } else {
        g_game->playMovie = 1;
    }
    int ok27 = FUN_004b69d0("Total Annihilation", "DisplaymodeDepth", &value);
    if (ok27 == 0) {
        value = 0;
    }
    if (value == 0x100) {
        int ok28 = FUN_004b69d0("Total Annihilation", "Games", &value);
        if (ok28 == 0) value=0;
        if (value == 1) g_game->flags_37f2f.bit1=1;
        else g_game->flags_37f2f.bit1=0;
    } else g_game->flags_37f2f.bit1=0;
label_430e7f:
    g_game->flags_37f2f.bit2 = 1;
    g_game->flags_37f2f.bit3 = 1;
    g_game->flags_37f2f.bit4 = 0;
    int ok29 = FUN_004b69d0("Total Annihilation", "AllMissions", &value);
    if (ok29 != 0) {
        g_game->missionFlags.allMissions = value;
        return;
    }
    g_game->missionFlags.allMissions = 0;
}
