// Decompiled by Claude Opus 5.5. Names are provisional.
// Click handler of the end-of-mission screen (ENDMSN.GUI, opened by
// FUN_0041f0a0). On close (field +0x60 == -1) it frees the outcome images
// and the Missions list; otherwise it handles LoadGame, SaveGame,
// Start/Missions (checks the campaign CD and starts the chosen mission),
// MainMenu and Difficulty (cycles easy, medium, hard).

class Class_004ce690 {
public:
    void FUN_004ce690(int param_1);
};

class Class_00435c00 {
public:
    int FUN_00435c00(int param_1);
};

#pragma pack(push, 1)
struct Entry_0041ec50 {                  // 0x15b bytes
    char unknown_0[0xba];
    short field_ba;                      // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Data_0041ec50 {
    char unknown_0[0x14];
    void* items;                         // +0x14
};

struct Layer_0041ec50 {
    int unknown_0;
    Entry_0041ec50* entries;             // +0x04
    void (__stdcall* handler)(void*);    // +0x08
    Data_0041ec50* data;                 // +0x0c
};

struct Gadget_0041ec50 {
    char unknown_0[0x18];
    Layer_0041ec50* layer;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                        // +0x60
};

struct Display_0041ec50 {
    char unknown_0[0x614];
    int field_614;                       // +0x614
};

struct Options_0041ec50 {
    char unknown_0[0x228];
    int difficulty;                      // +0x228
};

struct Game {
    char unknown_0[0x10];
    Class_004ce690* field_10;            // +0x10
    char unknown_14[0x519 - 0x14];
    char message[0x29a0 - 0x519];        // +0x519
    Options_0041ec50* options;           // +0x29a0
    char unknown_29a4[0x2a44 - 0x29a4];
    unsigned short bit0_2a44 : 1;        // +0x2a44
    unsigned short bit1_2a44 : 1;
    unsigned short bit2_2a44 : 1;
    unsigned short bit3_2a44 : 1;
    unsigned short bits4_2a44 : 12;
    char unknown_2a46[0x2bc0 - 0x2a46];
    unsigned char field_2bc0;            // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    unsigned short bits0 : 4;            // +0x2bee
    unsigned short flag4 : 1;            // +0x2bee, bit 4
    unsigned short bits5 : 11;
    char unknown_2bf0[0x37eee - 0x2bf0];
    int difficulty;                      // +0x37eee
    char unknown_37ef2[0x3906f - 0x37ef2];
    int field_3906f;                     // +0x3906f
    char unknown_39073[0x39077 - 0x39073];
    void* image_39077;                   // +0x39077
    void* image_3907b;                   // +0x3907b
    void* buffer_3907f;                  // +0x3907f
    void* buffer_39083;                  // +0x39083
    void* buffer_39087;                  // +0x39087
    void* buffer_3908b;                  // +0x3908b
    char unknown_3908f[0x391e9 - 0x3908f];
    Class_00435c00* campaign;            // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned short bits0_3923b : 2;      // +0x3923b
    unsigned short bit2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short bit4_3923b : 1;
    unsigned short bits5_3923b : 11;
};
#pragma pack(pop)

extern Game* g_game;

void FUN_004257a0();
void __stdcall FreeSurface(void* image);
void __cdecl FUN_004d85a0(void* p);
void LeaveNetGame();
Display_0041ec50* GetDisplay();
int __stdcall IsCurrentGadgetNamed(Gadget_0041ec50* gadget, char* name);
void __stdcall FUN_0047f1a0(char* name, int param_2);
void ShowLoadGameScreen();
void ShowSaveGameScreen();
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall FUN_00425860(int state, int line, char* file);
void __stdcall SetGameMode(int a);
void __stdcall FUN_004c22d0(int param);
void __stdcall FUN_00491c80(int n);
char __stdcall FindGameCdDrive(int param_1);
char* __stdcall FUN_004c5740(char* text);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
void RegisterDataArchives();
Entry_0041ec50* __stdcall FindGadgetChecked(Entry_0041ec50* entries, char* name);
void FUN_00425a90();
void __stdcall FUN_00434ab0(int param);

// FUNCTION: 0x41ec50
void __stdcall FUN_0041ec50(Gadget_0041ec50* gadget)
{
    Entry_0041ec50* entries = gadget->layer->entries;
    Data_0041ec50* data = gadget->layer->data;
    if (gadget->field_60 == -1) {
        FUN_004257a0();
        if (g_game->image_39077 != 0)
            FreeSurface(g_game->image_39077);
        if (g_game->image_3907b != 0)
            FreeSurface(g_game->image_3907b);
        if (g_game->buffer_3907f != 0)
            FUN_004d85a0(g_game->buffer_3907f);
        if (g_game->buffer_39083 != 0)
            FUN_004d85a0(g_game->buffer_39083);
        if (g_game->buffer_39087 != 0)
            FUN_004d85a0(g_game->buffer_39087);
        if (g_game->buffer_3908b != 0)
            FUN_004d85a0(g_game->buffer_3908b);
        g_game->image_39077 = 0;
        g_game->image_3907b = 0;
        g_game->buffer_3907f = 0;
        g_game->buffer_39083 = 0;
        g_game->buffer_39087 = 0;
        g_game->buffer_3908b = 0;
        FUN_004d85a0(data->items);
        FUN_004d85a0(data);
        if (g_game->flag4)
            LeaveNetGame();
        g_game->field_10->FUN_004ce690(4);
        Display_0041ec50* display = GetDisplay();
        display->field_614 = g_game->field_3906f;
        return;
    }
    // LoadGame and SaveGame reset the gadget (FUN_004ab0a0) twice in a row;
    // the second call is redundant.
    if (IsCurrentGadgetNamed(gadget, "LoadGame")) {
        FUN_0047f1a0("BigButton", 0);
        ShowLoadGameScreen();
        FUN_004ab0a0(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SaveGame")) {
        FUN_0047f1a0("BigButton", 0);
        ShowSaveGameScreen();
        FUN_004ab0a0(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "Start") || IsCurrentGadgetNamed(gadget, "Missions")) {
        if (!FindGameCdDrive(0)) {
            OpenMessageBox(g_game->message,
                         FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"),
                         200, 1, 1);
            FUN_004ab0a0(g_game->message);
        }
        RegisterDataArchives();
        FUN_0047f1a0("BigButton", 0);
        g_game->field_2bc0 = 10;
        FUN_004c22d0(1);
        FUN_00491c80(0x14);
        if (g_game->campaign->FUN_00435c00(FindGadgetChecked(entries, "Missions")->field_ba)) {
            FUN_00425a90();
            g_game->bit2_2a44 = 0;
            g_game->bit3_2a44 = 1;
            g_game->bit0_2a44 = 0;
            FUN_00434ab0(1);
            g_game->bit4_3923b = 0;
            g_game->bit2_3923b = 0;
            FUN_00425860(13, 757, "c:\\cavedog\\wargame\\endgame.cpp");
            SetGameMode(2);
            return;
        }
    } else if (IsCurrentGadgetNamed(gadget, "MainMenu")) {
        FUN_0047f1a0("BigButton", 0);
        FUN_00425860(2, 770, "c:\\cavedog\\wargame\\endgame.cpp");
        SetGameMode(1);
        FUN_004c22d0(1);
        FUN_00491c80(0x14);
        return;
    } else if (IsCurrentGadgetNamed(gadget, "Difficulty")) {
        FUN_0047f1a0("SKirmish", 0);
        if (g_game->difficulty == 0) {
            g_game->options->difficulty = 1;
            g_game->difficulty = 1;
            FUN_004ab0a0(gadget);
            return;
        }
        if (g_game->difficulty == 1) {
            g_game->options->difficulty = 2;
            g_game->difficulty = 2;
            FUN_004ab0a0(gadget);
            return;
        }
        if (g_game->difficulty == 2) {
            g_game->options->difficulty = 0;
            g_game->difficulty = 0;
            FUN_004ab0a0(gadget);
            return;
        }
    }
    FUN_004ab0a0(gadget);
}
