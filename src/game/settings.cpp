// Decompiled by Opus, deepseek-v4.1-flash, GPT-6, deepseek-v4.1 and space-bunny-free. Names are provisional.

// Full <windows.h> pulls in <rpc.h>/<ole2.h>, whose declarations flip the
// base/index order of a SIB address in 0x431740.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct SkirmishPlayer {
    int controller;      // +0x00
    int side;            // +0x04
    int allyGroup;       // +0x08
    int metal;           // +0x0c
    int energy;          // +0x10
    int color;           // +0x14
};

struct Options {
    SkirmishPlayer players[10];         // +0x00, 0x18 bytes each
    char unknown_f0[0x108 - 10 * 0x18];
    int skirmishCommanderDeath;         // +0x108
    int skirmishMapping;                // +0x10c
    int skirmishLineOfSight;            // +0x110
    int skirmishLOSType;                // +0x114
    int fixedLocations;                 // +0x118
    char skirmishMap[0x228 - 0x11c];    // +0x11c
    int skirmishDifficulty;             // +0x228
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

struct SoundFlags {
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

struct UnitDef_00431740 {
    char unknown_0[0x20];
    char name[0x241 - 0x20];           // +0x20
    unsigned int flags;                // +0x241
    char unknown_245[0x249 - 0x245];
};

class Sound;

struct Game {
    char unknown_0[0xc];
    char* displayContext;                // +0x0c
    Sound* sound;                        // +0x10
    char unknown_14[0x29a0 - 0x14];
    Options* options;                    // +0x29a0
    char unknown_29a4[0x2bc1 - 0x29a4];
    char gameName[0x2bd2 - 0x2bc1];      // +0x2bc1
    char nickname[0x2be3 - 0x2bd2];      // +0x2bd2
    char password[0x1434d - 0x2be3];     // +0x2be3
    unsigned char scrollspeed;           // +0x1434d
    char unknown_1434e[0x1438f - 0x1434e];
    int count;                           // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitDef_00431740* defs;              // +0x1439b
    char unknown_1439f[0x37eee - 0x1439f];
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
    union {
        unsigned char musicmode;         // +0x37f14
        MusicFlags_0042f9a0 musicFlags_37f14;
    };
    unsigned char cdmode;                // +0x37f16
    unsigned char unitchat;              // +0x37f17
    unsigned char unitchattext;          // +0x37f18
    SoundFlags soundFlags;               // +0x37f19
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
    char unknown_38c5b[0x38c5f - 0x38c5b];
    int imageOutputDirty;                // +0x38c5f
    int movieOutputDirty;                // +0x38c63
    char unknown_38c67[0x38d7f - 0x38c67];
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

int __stdcall ReadRegistryDword(void* section, void* key, void* value);
int __stdcall ReadRegistryData(void* section, void* key, void* buf, void* size);
void __stdcall WriteRegistryString(void* section, void* key, void* value);
void __stdcall WriteRegistryDword(void* section, void* key, int value);
int __stdcall ReadRegistryValue(const char* app, const char* key, void* buf, unsigned int* size);
void __stdcall WriteRegistryBinary(void* param_1, void* param_2, void* param_3, int unused);
int __stdcall GetWindowsUserName(void* buf);
void __stdcall SetMissionType(int mode);
int __cdecl IsOnlineConfigLoaded();
void __cdecl ProtectBlockReadWrite(void* param_1);
void __cdecl ProtectBlockReadOnly(void* param_1);

class Sound {
public:
    void SetMaxBuffers(int value);
    void Enable3D();
    void Disable3D();
    int Is3DEnabled();
    int GetMaxBuffers();
};

class Class_004d0070 {
public:
    int SetWaveVolume(int value);
};

class Class_004d00d0 {
public:
    int SetAuxVolume(int value, int flag);
};

class Class_004cfff0 {
public:
    int QueryWaveVolume();
};

class Class_004d0040 {
public:
    int QueryAuxVolume();
};

class TdfFile {
public:
    int field_0;
    void* current;                     // +0x4
    int field_8;
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    void ResetCurrentRecord();
    int SelectRecordAt(int index);
};

class TdfRecord {
public:
    const char* field_0;
    void CopyRecordName(char* dest, size_t count);
};

class Mission {
public:
    char* GetNameSlot(int index);
    char* GetMissionName();
    void RefreshMapList(int arg);
};


// FUNCTION: 0x42f910
void __stdcall SaveTrackSettings(unsigned char* tracks)
{
    char name[12];
    int i;

    for (i = 0; i < 10; i++) {
        sprintf(name, "track%d", i);
        WriteRegistryDword("Total Annihilation", name, tracks[i]);
    }
}

// Writes a value under the game's registry key (see WriteRegistryBinary); the
// sibling of ReadGameRegistryValue.
// FUNCTION: 0x42f960
void __stdcall WriteGameRegistryValue(void* key, void* buf, int value)
{
    WriteRegistryBinary("Total Annihilation", key, buf, value);
}

// Reads a value from the game's registry key (see ReadRegistryValue/0x4b6880).
// FUNCTION: 0x42f980
int __stdcall ReadGameRegistryValue(const char* key, void* buf, unsigned int* size)
{
    return ReadRegistryValue("Total Annihilation", key, buf, size);
}

// Reads a DWORD under the game key into g_game->*field; a missing value stores
// the default and writes it back. value is the caller's scratch local (one of
// the helper's own moves the frame), and the named ok keeps the compare against
// the hoisted zero register.
static inline void ReadSettingInt(const char* key, int Game::* field, int defaultValue, int& value)
{
    int ok = ReadRegistryDword("Total Annihilation", (void*)key, &value);
    if (ok != 0) {
        g_game->*field = value;
    } else {
        g_game->*field = defaultValue;
        WriteRegistryDword("Total Annihilation", (void*)key, g_game->*field);
    }
}

// Same, but a missing value is not written back.
static inline void ReadSettingIntOr(const char* key, int Game::* field, int defaultValue, int& value)
{
    if (ReadRegistryDword("Total Annihilation", (void*)key, &value) != 0) {
        g_game->*field = value;
    } else {
        g_game->*field = defaultValue;
    }
}

// Same for g_game->options.
static inline void ReadSettingInt(const char* key, int Options::* field, int defaultValue, int& value)
{
    int ok = ReadRegistryDword("Total Annihilation", (void*)key, &value);
    if (ok != 0) {
        g_game->options->*field = value;
    } else {
        g_game->options->*field = defaultValue;
        WriteRegistryDword("Total Annihilation", (void*)key, g_game->options->*field);
    }
}

// Reads one per-player skirmish value; a missing value is not written back.
static inline void ReadPlayerSetting(const char* key, int SkirmishPlayer::* field, int defaultValue, int& value, int i)
{
    if (ReadRegistryDword("Total Annihilation\\Skirmish", (void*)key, &value) != 0) {
        g_game->options->players[i].*field = value;
    } else {
        g_game->options->players[i].*field = defaultValue;
    }
}

// Reads up to DitheredFog, PlayMovie and AllMissions go through a named local;
// every other site compares the call directly.
// FUNCTION: 0x42f9a0
void LoadSettings()
{
    int value;
    char name[32];
    char buf[256];
    int i;

    int ok1 = ReadRegistryDword("Total Annihilation", "Interface Type", &value);
    if (ok1 != 0) {
        if (value > 1) value = 1;
        g_game->interfaceType = value;
    } else {
        g_game->interfaceType = 0;
        WriteRegistryDword("Total Annihilation", "Interface Type", g_game->interfaceType);
    }
    ReadSettingInt("DisplaymodeWidth", &Game::displaymodeWidth, 0x280, value);
    ReadSettingInt("DisplaymodeHeight", &Game::displaymodeHeight, 0x1e0, value);
    ReadSettingInt("side", &Game::side, 0, value);
    int ok5 = ReadRegistryDword("Total Annihilation", "Difficulty", &value);
    if (ok5 != 0) {
        g_game->difficulty = value & 0xffff;
    } else {
        g_game->difficulty = 1;
        WriteRegistryDword("Total Annihilation", "Difficulty", g_game->difficulty);
    }
    int ok6 = ReadRegistryDword("Total Annihilation", "scrollspeed", &value);
    if (ok6 != 0) {
        g_game->scrollspeed = (unsigned char)value;
    } else {
        g_game->scrollspeed = 0x20;
        WriteRegistryDword("Total Annihilation", "scrollspeed", g_game->scrollspeed);
    }
    ReadSettingInt("SingleCommanderDeath", &Game::singleCommanderDeath, 1, value);
    ReadSettingInt("SingleMapping", &Game::singleMapping, 1, value);
    ReadSettingInt("SingleLineOfSight", &Game::singleLineOfSight, 1, value);
    ReadSettingInt("SingleLOSType", &Game::singleLOSType, 1, value);
    ReadSettingInt("screenchat", &Game::screenchat, 1, value);
    int ok12 = ReadRegistryDword("Total Annihilation", "damagebars", &value);
    if (ok12 != 0) {
        g_game->flags_37f06.damagebars = value;
    } else {
        g_game->flags_37f06.damagebars = 0;
        WriteRegistryDword("Total Annihilation", "damagebars", g_game->flags_37f06.damagebars);
    }
    int ok13 = ReadRegistryDword("Total Annihilation", "Sound Mode", &value);
    if (ok13 != 0) {
        if (value == 2) {
            g_game->sound->Enable3D();
        } else {
            g_game->sound->Disable3D();
        }
        g_game->soundFlags.soundMode = value;
    } else {
        WriteRegistryDword("Total Annihilation", "Sound Mode",
                     (g_game->sound->Is3DEnabled() != 0) + 1);
        g_game->soundFlags.soundMode = 1;
    }
    int ok14 = ReadRegistryDword("Total Annihilation", "MixingBuffers", &value);
    if (ok14 != 0) {
        g_game->sound->SetMaxBuffers(value);
    } else {
        g_game->sound->SetMaxBuffers(8);
    }
    int ok15 = ReadRegistryDword("Total Annihilation", "RestoreVolume", &value);
    if (ok15 != 0) {
        g_game->soundFlags.restoreVolume = value;
    } else {
        g_game->soundFlags.restoreVolume = 0;
    }
    if (g_game->soundFlags.restoreVolume) {
        int ok16 = ReadRegistryDword("Total Annihilation", "WaveOutVolume", &value);
        if (ok16 != 0) {
            ((Class_004d0070*)g_game->sound)->SetWaveVolume(value);
        }
        int ok17 = ReadRegistryDword("Total Annihilation", "CDAudioVolume", &value);
        if (ok17 != 0) {
            ((Class_004d00d0*)g_game->sound)->SetAuxVolume(value, 0);
        }
    }
    int ok18 = ReadRegistryDword("Total Annihilation", "Anti-Alias", &value);
    if (ok18 != 0) {
        g_game->flags_37f06.antiAlias = value;
    } else {
        g_game->flags_37f06.antiAlias = 1;
        WriteRegistryDword("Total Annihilation", "Anti-Alias", g_game->flags_37f06.antiAlias);
    }
    int ok19 = ReadRegistryDword("Total Annihilation", "Shadows", &value);
    if (ok19 != 0) {
        g_game->flags_37f06.shadows = value;
    } else {
        g_game->flags_37f06.shadows = 1;
        WriteRegistryDword("Total Annihilation", "Shadows", g_game->flags_37f06.shadows);
    }
    int ok20 = ReadRegistryDword("Total Annihilation", "FeatureShadows", &value);
    if (ok20 != 0) {
        g_game->flags_37f06.featureShadows = value;
    } else {
        g_game->flags_37f06.featureShadows = 1;
        WriteRegistryDword("Total Annihilation", "FeatureShadows", g_game->flags_37f06.featureShadows);
    }
    int ok21 = ReadRegistryDword("Total Annihilation", "VehicleShadows", &value);
    if (ok21 != 0) {
        g_game->flags_37f06.vehicleShadows = value;
    } else {
        g_game->flags_37f06.vehicleShadows = 1;
        WriteRegistryDword("Total Annihilation", "VehicleShadows", g_game->flags_37f06.vehicleShadows);
    }
    int ok22 = ReadRegistryDword("Total Annihilation", "Shading", &value);
    if (ok22 != 0) {
        g_game->flags_37f06.shading = value;
    } else {
        g_game->flags_37f06.shading = 1;
        WriteRegistryDword("Total Annihilation", "Shading", g_game->flags_37f06.shading);
    }
    int ok23 = ReadRegistryDword("Total Annihilation", "DitheredFog", &value);
    if (ok23 != 0) {
        g_game->flags_37f06.ditheredFog = value;
    } else {
        g_game->flags_37f06.ditheredFog = 0;
        WriteRegistryDword("Total Annihilation", "DitheredFog", g_game->flags_37f06.ditheredFog);
    }
    if (ReadRegistryDword("Total Annihilation", "Gamma", &value) != 0) {
        if (value == 10) {
            g_game->gamma = 0xc;
        } else {
            g_game->gamma = value;
        }
    } else {
        g_game->gamma = 0xc;
        WriteRegistryDword("Total Annihilation", "Gamma", g_game->gamma);
    }
    if (ReadRegistryDword("Total Annihilation", "SwitchAlt", &value) != 0) {
        g_game->flags_37f06.switchAlt = value;
    } else {
        g_game->flags_37f06.switchAlt = 0;
        WriteRegistryDword("Total Annihilation", "SwitchAlt", g_game->flags_37f06.switchAlt);
    }
    value = 0xb;
    if (ReadRegistryData("Total Annihilation", "Password", g_game->password, &value) == 0) {
        g_game->password[0] = 0;
    }
    if (IsOnlineConfigLoaded() != 0 && DAT_00512d48[0] != 0) {
        g_game->nickname[0] = 0;
        strncat(g_game->nickname, DAT_00512d48, 0x10);
    } else {
        value = 0x11;
        if (ReadRegistryData("Total Annihilation", "Nickname", g_game->nickname, &value) == 0) {
            g_game->nickname[0] = 0;
        }
    }
    if (DAT_00512ca8[0] != 0) {
        g_game->gameName[0] = 0;
        strncat(g_game->gameName, DAT_00512ca8, 0x10);
    } else {
        value = 0x11;
        if (ReadRegistryData("Total Annihilation", "Game Name", g_game->gameName, &value) == 0) {
            g_game->gameName[0] = 0;
        }
    }
    value = 0x100;
    if (ReadRegistryData("Total Annihilation", "Image Output Directory",
                     g_game->imageOutputDirectory, &value) == 0) {
        if (GetWindowsUserName(buf) == 0) {
            strcpy(buf, "user_images");
        }
        sprintf(g_game->imageOutputDirectory, "%s\\%s", g_game->displayContext + 0x628, buf);
    }
    ReadSettingIntOr("Movie Output Rate", &Game::movieOutputRate, 10, value);
    ReadSettingIntOr("textlines", &Game::textlines, 10, value);
    ReadSettingIntOr("textscroll", &Game::textscroll, 10, value);
    ReadSettingIntOr("mousespeed", &Game::mousespeed, 10, value);
    if (ReadRegistryDword("Total Annihilation", "gamespeed", &value) != 0) {
        g_game->gamespeed = (unsigned short)value;
    } else {
        g_game->gamespeed = 10;
    }
    g_game->gamespeed2 = g_game->gamespeed;
    if (ReadRegistryDword("Total Annihilation", "unitchat", &value) != 0) {
        g_game->unitchat = (unsigned char)value;
    } else {
        g_game->unitchat = 10;
    }
    if (ReadRegistryDword("Total Annihilation", "unitchattext", &value) != 0) {
        g_game->unitchattext = (unsigned char)value;
    } else {
        g_game->unitchattext = 5;
    }
    if (ReadRegistryDword("Total Annihilation", "musicmode", &value) != 0) {
        g_game->musicFlags_37f14.musicmode = value;
    } else {
        g_game->musicFlags_37f14.musicmode = 1;
    }
    if (ReadRegistryDword("Total Annihilation", "cdmode", &value) != 0) {
        g_game->cdmode = (unsigned char)value;
    } else {
        g_game->cdmode = 4;
    }
    if (ReadRegistryDword("Total Annihilation", "ackfx", &value) != 0) {
        g_game->soundFlags.ackfx = value;
    } else {
        g_game->soundFlags.ackfx = 1;
    }
    if (ReadRegistryDword("Total Annihilation", "buildfx", &value) != 0) {
        g_game->soundFlags.buildfx = value;
    } else {
        g_game->soundFlags.buildfx = 1;
    }
    if (ReadRegistryDword("Total Annihilation", "speechfx", &value) != 0) {
        g_game->soundFlags.speechfx = value;
    } else {
        g_game->soundFlags.speechfx = 1;
    }
    ReadSettingIntOr("fxvol", &Game::fxvol, 0x1b, value);
    ReadSettingIntOr("musicvol", &Game::musicvol, 0x20, value);
    if (ReadRegistryDword("Total Annihilation", "clock", &value) != 0) {
        g_game->flags_37f2f.clock = value;
    } else {
        g_game->flags_37f2f.clock = 0;
    }
    if (ReadRegistryDword("Total Annihilation", "NumSkirmishPlayers", &value) != 0) {
        if (value > 1 && value <= 10) {
            g_game->numSkirmishPlayers = value;
        } else {
            g_game->numSkirmishPlayers = value;
        }
    } else {
        g_game->numSkirmishPlayers = 4;
    }
    ReadSettingInt("MultiCommanderDeath", &Game::multiCommanderDeath, 1, value);
    ReadSettingInt("MultiMapping", &Game::multiMapping, 1, value);
    ReadSettingInt("MultiLineOfSight", &Game::multiLineOfSight, 1, value);
    ReadSettingInt("MultiLOSType", &Game::multiLOSType, 1, value);
    ReadSettingInt("SkirmishCommanderDeath", &Options::skirmishCommanderDeath, 1, value);
    ReadSettingInt("SkirmishMapping", &Options::skirmishMapping, 1, value);
    ReadSettingInt("SkirmishLineOfSight", &Options::skirmishLineOfSight, 1, value);
    ReadSettingInt("SkirmishLOSType", &Options::skirmishLOSType, 1, value);
    if (ReadRegistryDword("Total Annihilation", "SkirmishDifficulty", &value) != 0) {
        g_game->options->skirmishDifficulty = value & 0xffff;
    } else {
        g_game->options->skirmishDifficulty = 1;
        WriteRegistryDword("Total Annihilation", "SkirmishDifficulty",
                     g_game->options->skirmishDifficulty);
    }
    ReadSettingInt("SkirmishLocation", &Options::fixedLocations, 1, value);
    value = 0x100;
    if (ReadRegistryData("Total Annihilation", "SkirmishMap",
                     g_game->options->skirmishMap, &value) == 0) {
        SetMissionType(2);
        ((Mission*)g_game->campaign)->RefreshMapList(0);
        strncpy(g_game->options->skirmishMap,
                ((Mission*)g_game->campaign)->GetMissionName(), 0x100);
        SetMissionType(0);
        WriteRegistryString("Total Annihilation", "SkirmishMap", g_game->options->skirmishMap);
    }
    for (i = 0; i < g_game->numSkirmishPlayers; i++) {
        wsprintfA(name, "Player%dController", i);
        ReadPlayerSetting(name, &SkirmishPlayer::controller, 0, value, i);
        wsprintfA(name, "Player%dSide", i);
        ReadPlayerSetting(name, &SkirmishPlayer::side, i % 2, value, i);
        wsprintfA(name, "Player%dColor", i);
        ReadPlayerSetting(name, &SkirmishPlayer::color, i, value, i);
        wsprintfA(name, "Player%dAllyGroup", i);
        ReadPlayerSetting(name, &SkirmishPlayer::allyGroup, 5, value, i);
        wsprintfA(name, "Player%dMetal", i);
        ReadPlayerSetting(name, &SkirmishPlayer::metal, 1000, value, i);
        wsprintfA(name, "Player%dEnergy", i);
        ReadPlayerSetting(name, &SkirmishPlayer::energy, 1000, value, i);
    }
    int ok26 = ReadRegistryDword("Total Annihilation", "PlayMovie", &value);
    if (ok26 != 0) {
        g_game->playMovie = value;
    } else {
        g_game->playMovie = 1;
    }
    int ok27 = ReadRegistryDword("Total Annihilation", "DisplaymodeDepth", &value);
    if (ok27 == 0) {
        value = 0;
    }
    if (value == 0x100) {
        int ok28 = ReadRegistryDword("Total Annihilation", "Games", &value);
        if (ok28 == 0) value=0;
        if (value == 1) g_game->flags_37f2f.bit1=1;
        else g_game->flags_37f2f.bit1=0;
    } else g_game->flags_37f2f.bit1=0;
label_430e7f:
    g_game->flags_37f2f.bit2 = 1;
    g_game->flags_37f2f.bit3 = 1;
    g_game->flags_37f2f.bit4 = 0;
    int ok29 = ReadRegistryDword("Total Annihilation", "AllMissions", &value);
    if (ok29 != 0) {
        g_game->missionFlags.allMissions = value;
        return;
    }
    g_game->missionFlags.allMissions = 0;
}

// Writes every option back out to the config tree: the display, sound and
// render settings under the "Total Annihilation" section, the strings
// (password, nickname, game name, image output directory, skirmish map), and
// the per-player skirmish settings under "Total Annihilation\Skirmish".
// FUNCTION: 0x430f00
void SaveSettings()
{
    char name[32];
    int i;

    WriteRegistryDword("Total Annihilation", "Interface Type", g_game->interfaceType);
    WriteRegistryDword("Total Annihilation", "DisplaymodeWidth", g_game->displaymodeWidth);
    WriteRegistryDword("Total Annihilation", "DisplaymodeHeight", g_game->displaymodeHeight);
    WriteRegistryDword("Total Annihilation", "side", g_game->side);
    WriteRegistryDword("Total Annihilation", "FixedLocations", g_game->options->fixedLocations);
    WriteRegistryDword("Total Annihilation", "scrollspeed", g_game->scrollspeed);
    WriteRegistryDword("Total Annihilation", "SingleCommanderDeath", g_game->singleCommanderDeath);
    WriteRegistryDword("Total Annihilation", "SingleMapping", g_game->singleMapping);
    WriteRegistryDword("Total Annihilation", "SingleLineOfSight", g_game->singleLineOfSight);
    WriteRegistryDword("Total Annihilation", "SingleLOSType", g_game->singleLOSType);
    WriteRegistryDword("Total Annihilation", "screenchat", g_game->screenchat);
    WriteRegistryDword("Total Annihilation", "damagebars", g_game->flags_37f06.damagebars);
    WriteRegistryDword("Total Annihilation", "Sound Mode", g_game->soundFlags.soundMode);
    WriteRegistryDword("Total Annihilation", "RestoreVolume", g_game->soundFlags.restoreVolume);
    WriteRegistryDword("Total Annihilation", "MixingBuffers",
                 g_game->sound->GetMaxBuffers());
    if (g_game->soundFlags.restoreVolume) {
        WriteRegistryDword("Total Annihilation", "WaveOutVolume",
                     ((Class_004cfff0*)g_game->sound)->QueryWaveVolume());
        WriteRegistryDword("Total Annihilation", "CDAudioVolume",
                     ((Class_004d0040*)g_game->sound)->QueryAuxVolume());
    }
    WriteRegistryDword("Total Annihilation", "Anti-Alias", g_game->flags_37f06.antiAlias);
    WriteRegistryDword("Total Annihilation", "Shadows", g_game->flags_37f06.shadows);
    WriteRegistryDword("Total Annihilation", "FeatureShadows", g_game->flags_37f06.featureShadows);
    WriteRegistryDword("Total Annihilation", "VehicleShadows", g_game->flags_37f06.vehicleShadows);
    WriteRegistryDword("Total Annihilation", "Shading", g_game->flags_37f06.shading);
    WriteRegistryDword("Total Annihilation", "DitheredFog", g_game->flags_37f06.ditheredFog);
    WriteRegistryDword("Total Annihilation", "Difficulty", g_game->difficulty);
    WriteRegistryDword("Total Annihilation", "Gamma", g_game->gamma);
    WriteRegistryDword("Total Annihilation", "SwitchAlt", g_game->flags_37f06.switchAlt);
    WriteRegistryString("Total Annihilation", "Password", g_game->password);
    WriteRegistryString("Total Annihilation", "Nickname", g_game->nickname);
    WriteRegistryString("Total Annihilation", "Game Name", g_game->gameName);
    if (g_game->imageOutputDirty) {
        WriteRegistryString("Total Annihilation", "Image Output Directory",
                     g_game->imageOutputDirectory);
        g_game->imageOutputDirty = 0;
    }
    if (g_game->movieOutputDirty) {
        WriteRegistryDword("Total Annihilation", "Movie Output Rate", g_game->movieOutputRate);
        g_game->movieOutputDirty = 0;
    }
    WriteRegistryDword("Total Annihilation", "unitchat", g_game->unitchat);
    WriteRegistryDword("Total Annihilation", "unitchattext", g_game->unitchattext);
    WriteRegistryDword("Total Annihilation", "textlines", g_game->textlines);
    WriteRegistryDword("Total Annihilation", "textscroll", g_game->textscroll);
    WriteRegistryDword("Total Annihilation", "mousespeed", g_game->mousespeed);
    WriteRegistryDword("Total Annihilation", "gamespeed", g_game->gamespeed);
    WriteRegistryDword("Total Annihilation", "clock", g_game->flags_37f2f.clock);
    WriteRegistryDword("Total Annihilation", "musicmode", g_game->musicmode & 1);
    WriteRegistryDword("Total Annihilation", "cdmode", g_game->cdmode);
    WriteRegistryDword("Total Annihilation", "ackfx", g_game->soundFlags.ackfx);
    WriteRegistryDword("Total Annihilation", "buildfx", g_game->soundFlags.buildfx);
    WriteRegistryDword("Total Annihilation", "speechfx", g_game->soundFlags.speechfx);
    WriteRegistryDword("Total Annihilation", "fxvol", g_game->fxvol);
    WriteRegistryDword("Total Annihilation", "musicvol", g_game->musicvol);
    WriteRegistryDword("Total Annihilation", "MultiCommanderDeath", g_game->multiCommanderDeath);
    WriteRegistryDword("Total Annihilation", "MultiMapping", g_game->multiMapping);
    WriteRegistryDword("Total Annihilation", "MultiLineOfSight", g_game->multiLineOfSight);
    WriteRegistryDword("Total Annihilation", "MultiLOSType", g_game->multiLOSType);
    WriteRegistryDword("Total Annihilation", "SkirmishCommanderDeath",
                 g_game->options->skirmishCommanderDeath);
    WriteRegistryDword("Total Annihilation", "SkirmishMapping", g_game->options->skirmishMapping);
    WriteRegistryDword("Total Annihilation", "SkirmishLineOfSight",
                 g_game->options->skirmishLineOfSight);
    WriteRegistryDword("Total Annihilation", "SkirmishLOSType", g_game->options->skirmishLOSType);
    WriteRegistryDword("Total Annihilation", "SkirmishLocation", g_game->options->fixedLocations);
    WriteRegistryDword("Total Annihilation", "SkirmishDifficulty",
                 g_game->options->skirmishDifficulty);
    WriteRegistryString("Total Annihilation", "SkirmishMap", g_game->options->skirmishMap);
    for (i = 0; i < g_game->numSkirmishPlayers; i++) {
        wsprintfA(name, "Player%dController", i);
        WriteRegistryDword("Total Annihilation\\Skirmish", name,
                     g_game->options->players[i].controller);
        wsprintfA(name, "Player%dSide", i);
        WriteRegistryDword("Total Annihilation\\Skirmish", name, g_game->options->players[i].side);
        wsprintfA(name, "Player%dColor", i);
        WriteRegistryDword("Total Annihilation\\Skirmish", name, g_game->options->players[i].color);
        wsprintfA(name, "Player%dAllyGroup", i);
        WriteRegistryDword("Total Annihilation\\Skirmish", name,
                     g_game->options->players[i].allyGroup);
        wsprintfA(name, "Player%dMetal", i);
        WriteRegistryDword("Total Annihilation\\Skirmish", name, g_game->options->players[i].metal);
        wsprintfA(name, "Player%dEnergy", i);
        WriteRegistryDword("Total Annihilation\\Skirmish", name,
                     g_game->options->players[i].energy);
    }
    WriteRegistryDword("Total Annihilation", "PlayMovie", g_game->playMovie);
}

// Applies the unit-type restrictions listed in the embedded list entry that
// Mission returns for index 6: clears bit 23 (0x800000) on every unit
// type, then sets it on each type named by the entry.
// FUNCTION: 0x431740
void ApplyUseOnlyUnits()
{
    TdfFile parser;
    char name[256];
    char* file = ((Mission*)g_game->campaign)->GetNameSlot(6);
    if (file == 0)
        return;
    if (!parser.LoadFile(file))
        return;
    {
        ProtectBlockReadWrite(g_game->defs);
        for (int i = 1; i < g_game->count; i++)
            g_game->defs[i].flags &= 0xff7fffff;
        parser.ResetCurrentRecord();
        for (int j = 0; parser.SelectRecordAt(j); j++, parser.ResetCurrentRecord()) {
            ((TdfRecord*)parser.current)->CopyRecordName(name, 0x100);
            for (int k = 0; k < g_game->count; k++) {
                if (_strcmpi(g_game->defs[k].name, name) == 0) {
                    g_game->defs[k].flags |= 0x800000;
                    break;
                }
            }
        }
        ProtectBlockReadOnly(g_game->defs);
    }
}
