// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Applies saved option values to the game: the volume at +0x37f0c, the sound
// flag word at +0x37f19 (bits 4, 5 and 6 copied from g_optionsBackupSoundFlags, bits 0-2
// after the sound object is switched by the low three bits), the byte at
// +0x37f17, then the brightness and both volume levels (same tail as
// 0x45c630 and 0x45bcc0).

class Sound {
public:
    char unknown_0[4];
    int field_4;

    void Enable3D();
    void Disable3D();
};

class Class_004d0070 {
public:
    void SetWaveVolume(int level);
};

class Class_004d00d0 {
public:
    void SetAuxVolume(int level, int flag);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    Sound* sound;                      // +0x10
    char unknown_14[0x37f08 - 0x14];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    char unknown_37f14[0x37f17 - 0x37f14];
    unsigned char field_37f17;         // +0x37f17
    char unknown_37f18[0x37f19 - 0x37f18];
    unsigned short field_37f19;        // +0x37f19
};
#pragma pack(pop)

extern Game* g_game;
extern int g_optionsBackupFxVolume;
extern unsigned char g_optionsBackupUnitChat;
// A byte in the original: declared unsigned int to keep it in bl for the bitfield merge.
extern unsigned int g_optionsBackupSoundFlags;

void __stdcall SetBrightness(float value);

// Stays in its own file: in options.cpp's declaration context the global byte
// at g_optionsBackupSoundFlags lands in dl instead of bl.
// FUNCTION: 0x45c820
void RestoreSoundOptions()
{
    g_game->volume1 = g_optionsBackupFxVolume;
    g_game->field_37f19 = (g_game->field_37f19 & ~0x10) | (g_optionsBackupSoundFlags & 0x10);
    g_game->field_37f19 = (g_game->field_37f19 & ~0x20) | (g_optionsBackupSoundFlags & 0x20);
    g_game->field_37f19 = (g_game->field_37f19 & ~0x40) | ((g_optionsBackupSoundFlags & 0x20) << 1);
    if ((((unsigned char)g_optionsBackupSoundFlags) & 7) == 2)
        g_game->sound->Enable3D();
    else
        g_game->sound->Disable3D();
    g_game->field_37f19 = (g_game->field_37f19 & ~7) | (g_optionsBackupSoundFlags & 7);
    g_game->field_37f17 = g_optionsBackupUnitChat;
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}
