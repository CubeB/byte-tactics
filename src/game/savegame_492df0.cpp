// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
// Click handler of the save game screen. On close (field +0x60 == -1) it
// frees the four save lists that 0x492b10 built. CANCEL goes back; DELETE
// removes the chosen save file, rebuilds the list and redraws; GAMES, LOAD and
// GAMENAME all take the name typed in the GAMENAME box and, when it is not
// empty, write the save under that name with a timestamp.
//
// MATCHED (618 of 618 bytes, 187 of 187 instructions).
//
// The last fault, and the one that had been open at 98.4%, was the dead store
// `mov [frame+0], eax` of FindGadgetChecked(entries, "GAMES")'s result in the last
// block. It is dead in the source (nothing reads the slot again) and MSVC
// deletes such a store, so it has to be kept by making the variable's ADDRESS
// escape. The only addresses of locals that escape in this function are the
// path buffer (sprintf and RemoveFile) and the count (ListSavedGames), and
// the store is at frame+0 while `&count` is at frame+4 and the buffer at
// frame+8, so all three cannot be separate locals: they are ONE local
// aggregate, {int, int, char[0x100]}, and taking the buffer's address
// (array-to-pointer decay of a member) is what stops the optimiser removing
// the store to the first member. Declaring the two ints separately does not
// work at all: an assigned-but-never-read int gets no stack slot, so the
// frame came out at 0x104 instead of 0x108 and the store disappeared with it.
// Measured, all one compile each, all 618 bytes:
//   * `int` + `int` + `char[0x100]` as separate locals: 0x104 frame, no store.
//   * the same with the path padded to 0x104: 0x108 frame, no store (98.4%).
//   * one local struct {int, int, char[0x100]}, `&save.count` to
//     ListSavedGames, `save.path` to sprintf and RemoveFile: MATCH.
#include <stdio.h>
#include <string.h>
#include <time.h>

#pragma pack(push, 1)
struct Entry_00492df0 {                  // 0x15b bytes
    char unknown_0[0xb6];
    char text[4];                        // +0xb6
    short field_ba;                      // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Layer_00492df0 {
    int unknown_0;
    Entry_00492df0* entries;             // +0x04
};

struct Gadget_00492df0 {
    char unknown_0[0x18];
    Layer_00492df0* layer;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                        // +0x60
};

struct Game {
    char unknown_0[0x519];
    char message[0x2a44 - 0x519];        // +0x519
    unsigned short bits0_2a44 : 2;       // +0x2a44
    unsigned short bit2_2a44 : 1;
    unsigned short bits3_2a44 : 13;
    char unknown_2a46[0x38c6b - 0x2a46];
    char saveName[1];                    // +0x38c6b
};
#pragma pack(pop)

// The DELETE block's path buffer and the count it rebuilds the "GAMES" list
// with, plus the "GAMES" entry the GAMES/LOAD/GAMENAME block looks up and
// never reads. One local, so that the buffer's address is the aggregate's.
struct Save_00492df0 {
    int games;                           // +0, stored, never read
    int count;                           // +4
    char path[0x100];                    // +8
};

extern Game* g_game;
extern char* DAT_005091c8;
extern char* DAT_0051f2e0;
extern char* DAT_0051f2e4;
extern char* DAT_0051f2e8;
extern char* DAT_0051f2ec;

void __stdcall FUN_004ab190(Gadget_00492df0* menu, int flag);
void __cdecl FUN_004d85a0(void* p);
int __stdcall IsCurrentGadgetNamed(Gadget_00492df0* gadget, char* name);
void __stdcall FUN_0049fa70(void* menu);
void __stdcall PlaySoundByName(char* name, int param_2);
Entry_00492df0* __stdcall FindGadgetChecked(Entry_00492df0* entries, char* name);
int __stdcall FindGadgetIndex(Entry_00492df0* entries, char* name, int type);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall RemoveFile(char* path);
char* __stdcall ListSavedGames(int* count);
void __stdcall FUN_004ab0a0(Gadget_00492df0* menu);
void ShowSavedGameInfo();
char* __stdcall BuildDataPath(char* buf, char* dir, char* name, char* ext);
int __stdcall SaveGameFile(char* path, char* description, int param_3);

// FUNCTION: 0x492df0
void __stdcall SaveGameScreenHandler(Gadget_00492df0* gadget)
{
    Entry_00492df0* entries = gadget->layer->entries;
    Save_00492df0 save;
    if (gadget->field_60 == -1) {
        FUN_004ab190(gadget, 1);
        if (DAT_0051f2e0)
            FUN_004d85a0(DAT_0051f2e0);
        if (DAT_0051f2e4)
            FUN_004d85a0(DAT_0051f2e4);
        if (DAT_0051f2e8)
            FUN_004d85a0(DAT_0051f2e8);
        DAT_0051f2e8 = 0;
        DAT_0051f2e4 = 0;
        DAT_0051f2e0 = 0;
        if (DAT_0051f2ec)
            FUN_004d85a0(DAT_0051f2ec);
        DAT_0051f2ec = 0;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
        if (g_game->bit2_2a44)
            FUN_0049fa70(g_game->message);
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "DELETE")) {
        PlaySoundByName("SmallButton", 0);
        Entry_00492df0* e = FindGadgetChecked(entries, "GAMES");
        sprintf(save.path, "%s\\%s", DAT_005091c8, SkipTextLines(DAT_0051f2e0, e->field_ba));
        RemoveFile(save.path);
        ListSavedGames(&save.count);
        FUN_004ab0a0(gadget);
        ShowSavedGameInfo();
        return;
    }
    if (!IsCurrentGadgetNamed(gadget, "GAMES") && !IsCurrentGadgetNamed(gadget, "LOAD") &&
        !IsCurrentGadgetNamed(gadget, "GAMENAME")) {
        if (gadget->field_60 != -1)
            FUN_004ab0a0(gadget);
        return;
    }
    if (g_game->bit2_2a44)
        FUN_0049fa70(g_game->message);
    PlaySoundByName("smlbutton", 0);
    int index = FindGadgetIndex(entries, "GAMENAME", 3);
    char* text = entries[index].text;
    if (strlen(text) != 0) {
        save.games = (int)FindGadgetChecked(entries, "GAMES");
        BuildDataPath(g_game->saveName, DAT_005091c8, text, "SAV");
        SaveGameFile(g_game->saveName, text, time(0));
    }
}
