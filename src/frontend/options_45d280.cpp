// Decompiled by space-bunny-free, edited by deepseek-v4.1 and GPT-6, finished by deepseek-v4.1-flash, finished by Claude Sonnet 5.5, finished by claude-opus-5-5. Names are provisional.
// Handler of the sound options screen: NOTRAK, TRACKMODE and TRACKTYPE set
// the music options, CDPLAY/CDNEXT/CDPREV/CDSTOP drive the CD player, UNDO
// and RESTORE reload the saved or the default sound settings, and any other
// selection on a valid entry (state 1) closes the screen and runs the next
// screen's handler. Selection -1 stops or restarts the sound object.

#pragma pack(push, 1)

struct Entry_0045d280 {              // 0x15b-byte gadget entry
    char state;                      // +0x00
    char unknown_1[0x137 - 1];
    unsigned char value;             // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Vtable_0045d280 {
    char unknown_0[8];
    void (__stdcall* FUN_8)(void* obj);
};

struct Holder_0045d280 {
    Vtable_0045d280* field_0;        // +0x00
    Entry_0045d280* entries;         // +0x04
};

struct Object_0045d280 {
    char unknown_0[0x18];
    Holder_0045d280* holder;          // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};

struct Menu_0045d280 {
    char unknown_0[0x18];
    Holder_0045d280* holder;          // +0x18
};

struct Bits_0045d280 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short bits_2_15 : 14;
};

union Flags_0045d280 {
    unsigned short word;
    unsigned char byte;
    Bits_0045d280 bits;
};

struct Game {
    char unknown_0[0x10];
    void* sound;                     // +0x10
    char unknown_14[0x531 - 0x14];
    void* table_531;                 // +0x531
    char unknown_535[0x2a44 - 0x535];
    unsigned char b0_2a44 : 1;       // +0x2a44
    unsigned char b1_2a44 : 1;
    unsigned char prefs : 1;         // bit 2
    unsigned char rest_2a44 : 5;
    char unknown_2a45[0x37ebe - 0x2a45];
    unsigned short loaded : 1;       // +0x37ebe
    unsigned short rest_37ebe : 15;
    char unknown_37ec0[0x37f08 - 0x37ec0];
    int field_37f08;                 // +0x37f08
    int volume1;                     // +0x37f0c
    int volume2;                     // +0x37f10
    Flags_0045d280 flags;            // +0x37f14
    unsigned char field_37f16;       // +0x37f16
};
#pragma pack(pop)

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

class Class_004ce3e0 {
public:
    void FUN_004ce3e0(char* obj);
};

class Class_004ce450 {
public:
    int GetTrackCount();
};

class Class_004ce580 {
public:
    int FUN_004ce580(int value);
};

class Class_004ce5a0 {
public:
    int FUN_004ce5a0();
};

class Class_004ce7a0 {
public:
    int SetPlaybackOrder(int value);
};

class Class_004ce7c0 {
public:
    void SetCategoryOfTrack(int value, char type);
};

class Class_004ce7e0 {
public:
    int GetCategoryOfTrack(int value);
};

class Class_004ce8c0 {
public:
    int SelectTrack(int value);
};

class Sound {
public:
    void PlayCdTrack(int value, int flag);
};

class Class_004ced40 {
public:
    void StopCdAudio();
};

class Class_004cedc0 {
public:
    int EnableCdAudio(int value);
};

class Class_004d0070 {
public:
    void SetWaveVolume(int level);
};

class Class_004d00d0 {
public:
    void SetAuxVolume(int level, int flag);
};

extern Game* g_game;
extern int DAT_00512fe0;                // current track
extern int DAT_00512f42;
extern int DAT_00512f46;
extern unsigned char DAT_00512f48;
extern int DAT_00512fd9;
extern char DAT_00512f75[];
extern char DAT_005067bc[];              // "NOTRAK"
extern char DAT_00506984[];              // "TRACKMODE"
extern char DAT_0050692c[];              // "TRACKTYPE"
extern char DAT_0050696c[];              // "CDPLAY"
extern char DAT_00506964[];              // "CDNEXT"
extern char DAT_0050697c[];              // "CDPREV"
extern char DAT_00506974[];              // "CDSTOP"
extern char DAT_00506998[];              // "UNDO"
extern char DAT_00506990[];              // "RESTORE"
extern char DAT_00502b38[];              // "Options"

void __stdcall FUN_0049fa90(void* obj);
int __stdcall IsCurrentGadgetNamed(void* obj, char* name);
int __stdcall FindGadgetIndex(Entry_0045d280* entries, char* name, int type);
int __stdcall GetButtonStageByName(void* obj, char* name);
void __stdcall SetButtonStageByName(void* obj, char* name, int value);
void __stdcall PlaySoundByName(char* name, int value);
void __stdcall FUN_004ab0a0(void* obj);
void __stdcall CloseTopScreen(void* obj);
void __stdcall SetBrightness(float value);
void UpdateTrackGadgets();
void UpdateMusicGadgets();
void OpenMusicOptions();

// 0x45bcc0 (matched in its own file).
void ApplyBrightnessAndVolume()
{
    SetBrightness(0.5 - g_game->field_37f08 * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}

// 0x45c510 (matched in its own file).
void ApplyTrackType()
{
    Entry_0045d280* gadgets = ((Holder_0045d280*)g_game->table_531)->entries;
    if (g_game->field_37f16 == 4) {
        int index = FindGadgetIndex(gadgets, DAT_0050692c, 1);
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(DAT_00512fe0, gadgets[index].value);
    }
}

// 0x45c630 (matched in its own file).
void FUN_0045c630()
{
    g_game->volume2 = 0x20;
    g_game->field_37f16 = 4;
    if (!(g_game->flags.word & 1)) {
        g_game->flags.word |= 1;
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    ApplyBrightnessAndVolume();
}

// 0x45c950 (matched in its own file).
void LoadSavedAudioSettings()
{
    g_game->volume2 = DAT_00512f42;
    ((Class_004ce3e0*)g_game->sound)->FUN_004ce3e0(DAT_00512f75);
    g_game->field_37f16 = DAT_00512f48;
    ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
    if (((unsigned char)g_game->flags.word ^ (unsigned char)DAT_00512f46) & 1) {
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
    }
    unsigned short f = g_game->flags.word;
    f = f ^ ((f ^ DAT_00512f46) & 1);
    g_game->flags.word = f;
    ((Class_004ce580*)g_game->sound)->FUN_004ce580(DAT_00512fd9);
    ApplyBrightnessAndVolume();
}

// FUNCTION: 0x45d280
void __stdcall HandleMusicOptionsClick(Object_0045d280* obj)
{
    Entry_0045d280* entries = obj->holder->entries;
    if (obj->field_60 == -1) {
        if (!g_game->prefs) {
            ((Class_004ced40*)g_game->sound)->StopCdAudio();
            g_game->loaded = 0;
            return;
        }
        ((Class_004cdb40*)g_game->sound)->PlayNextTrack();
        g_game->loaded = 0;
        return;
    }
    FUN_0049fa90(obj);
    // Result kept in a local: testing the call directly is longer.
    int notrak = IsCurrentGadgetNamed(obj, DAT_005067bc);
    if (notrak != 0) {              // "NOTRAK"
        PlaySoundByName(DAT_00502b38, 0);
        int v = GetButtonStageByName(obj, DAT_005067bc);
        unsigned short f = g_game->flags.word;
        g_game->flags.word = f ^ ((f ^ v) & 1);
        ((Class_004cedc0*)g_game->sound)->EnableCdAudio(g_game->flags.word & 1);
        FUN_004ab0a0(obj);
        UpdateMusicGadgets();
    } else if (IsCurrentGadgetNamed(obj, DAT_00506984)) {  // "TRACKMODE"
        PlaySoundByName(DAT_00502b38, 0);
        g_game->field_37f16 = GetButtonStageByName(obj, DAT_00506984) + 1;
        ((Class_004ce7a0*)g_game->sound)->SetPlaybackOrder(g_game->field_37f16);
        if (g_game->field_37f16 == 3) {
            DAT_00512fe0 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
            FUN_004ab0a0(obj);
            UpdateTrackGadgets();
            return;
        }
        if (g_game->field_37f16 == 4) {
            SetButtonStageByName(obj, DAT_0050692c, (unsigned char)((Class_004ce7e0*)g_game->sound)->GetCategoryOfTrack(DAT_00512fe0));
            ApplyTrackType();
        }
        FUN_004ab0a0(obj);
        UpdateTrackGadgets();
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_0050692c)) {  // "TRACKTYPE"
        PlaySoundByName(DAT_00502b38, 0);
        // Index with obj->field_60 itself, not the saved copy.
        int i = obj->field_60;
        ((Class_004ce7c0*)g_game->sound)->SetCategoryOfTrack(DAT_00512fe0, entries[i].value);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_0050696c)) {      // "CDPLAY"
        PlaySoundByName(DAT_00502b38, 0);
        ((Sound*)g_game->sound)->PlayCdTrack(DAT_00512fe0, 1);
        FUN_004ab0a0(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_00506964)) {  // "CDNEXT"
        PlaySoundByName(DAT_00502b38, 0);
        DAT_00512fe0 = DAT_00512fe0 + 1;
        int n = ((Class_004ce450*)g_game->sound)->GetTrackCount();
        if (DAT_00512fe0 > n)
            DAT_00512fe0 = 1;
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->SelectTrack(DAT_00512fe0);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_0050697c)) {  // "CDPREV"
        PlaySoundByName(DAT_00502b38, 0);
        DAT_00512fe0 = DAT_00512fe0 - 1;
        if (DAT_00512fe0 < 1)
            DAT_00512fe0 = ((Class_004ce450*)g_game->sound)->GetTrackCount();
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->SelectTrack(DAT_00512fe0);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    } else if (IsCurrentGadgetNamed(obj, DAT_00506974)) {  // "CDSTOP"
        PlaySoundByName(DAT_00502b38, 0);
        ((Class_004ced40*)g_game->sound)->StopCdAudio();
        DAT_00512fe0 = ((Class_004ce8c0*)g_game->sound)->SelectTrack(1);
        UpdateTrackGadgets();
        FUN_004ab0a0(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_00506998)) {      // "UNDO"
        PlaySoundByName(DAT_00502b38, 0);
        LoadSavedAudioSettings();
        CloseTopScreen(obj);
        OpenMusicOptions();
        return;
    }
    if (IsCurrentGadgetNamed(obj, DAT_00506990)) {      // "RESTORE"
        PlaySoundByName(DAT_00502b38, 0);
        FUN_0045c630();
        CloseTopScreen(obj);
        OpenMusicOptions();
        return;
    }
    int save = obj->field_60;
    if (obj->field_60 != -1) {
        if (entries[obj->field_60].state != 1) {
            FUN_004ab0a0(obj);
            return;
        }
        Vtable_0045d280* p = obj->holder->field_0;
        CloseTopScreen(obj);
        obj->field_60 = save;
        p->FUN_8(obj);
    }
}
