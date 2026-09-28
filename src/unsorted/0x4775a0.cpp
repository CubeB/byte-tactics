// Decompiled by mimo-v2.6-pro. Names are provisional.
// Click handler for a menu: picks up NewCamp, Skirmish, LoadGame, Options,
// PrevMenu or AnyMsn, checks the right CD for the mission buttons and shows a
// message box when the disc is missing.

#pragma pack(push, 1)
struct Gadget_004775a0 {
    char unknown_0[0x60];
    int field_60;                        // +0x60
};

struct Game_004775a0 {
    char unknown_0[0x519];
    char message[0x29a0 - 0x519];        // +0x519
    char unknown_29a0[0x2bc0 - 0x29a0];
    unsigned char field_2bc0;            // +0x2bc0
};
#pragma pack(pop)

extern Game_004775a0* g_game;

int __stdcall FUN_0049fd60(Gadget_004775a0* gadget, char* name);
char __stdcall FUN_0041d6a0(int side);
void FUN_0041d4c0();
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_00491c80(int n);
void FUN_004931d0();
void FUN_00460160();
void __stdcall FUN_004ab0a0(void* param_1);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x4775a0
void __stdcall FUN_004775a0(Gadget_004775a0* gadget)
{
    if (gadget->field_60 == -1)
        return;
    if (FUN_0049fd60(gadget, "NewCamp")) {
        if (FUN_0041d6a0(0)) {
            FUN_0041d4c0();
            FUN_0047f1a0("BigButton", 0);
            g_game->field_2bc0 = 10;
            FUN_00491c80(0x14);
        } else {
            FUN_004abd90(g_game->message, FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"), 200, 1, 1);
            FUN_004ab0a0(g_game->message);
        }
        return;
    }
    if (FUN_0049fd60(gadget, "Skirmish")) {
        if (FUN_0041d6a0(1)) {
            FUN_0041d4c0();
            FUN_0047f1a0("skirmish", 0);
            g_game->field_2bc0 = 11;
            FUN_00491c80(0x14);
        } else {
            FUN_004abd90(g_game->message, FUN_004c5740("Please insert the Multiplayer CD (Disc 1) and try again"), 200, 1, 1);
            FUN_004ab0a0(g_game->message);
        }
        return;
    }
    if (FUN_0049fd60(gadget, "LoadGame")) {
        FUN_0047f1a0("BigButton", 0);
        FUN_00491c80(0x14);
        FUN_004931d0();
        FUN_004ab0a0(gadget);
        return;
    }
    if (FUN_0049fd60(gadget, "Options")) {
        FUN_0047f1a0("options", 0);
        FUN_004ab0a0(gadget);
        FUN_00491c80(0x14);
        FUN_00460160();
        return;
    }
    if (FUN_0049fd60(gadget, "PrevMenu")) {
        FUN_0047f1a0("Previous", 0);
        FUN_00491c80(0x14);
        g_game->field_2bc0 = 3;
        return;
    }
    if (FUN_0049fd60(gadget, "AnyMsn")) {
        if (FUN_0041d6a0(0)) {
            FUN_0041d4c0();
            FUN_0047f1a0("bigButton", 0);
            g_game->field_2bc0 = 14;
            FUN_00491c80(0x14);
        } else {
            FUN_004abd90(g_game->message, FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"), 200, 1, 1);
            FUN_004ab0a0(g_game->message);
        }
        return;
    }
    FUN_004ab0a0(gadget);
}
