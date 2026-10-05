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
// `mov [frame+0], eax` of FUN_0049ff90(entries, "GAMES")'s result in the last
// block. It is dead in the source (nothing reads the slot again) and MSVC
// deletes such a store, so it has to be kept by making the variable's ADDRESS
// escape. The only addresses of locals that escape in this function are the
// path buffer (sprintf and FUN_004bbc30) and the count (FUN_00492b10), and
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
//     FUN_00492b10, `save.path` to sprintf and FUN_004bbc30: MATCH.
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
int __stdcall FUN_0049fd60(Gadget_00492df0* gadget, char* name);
void __stdcall FUN_0049fa70(void* menu);
void __stdcall FUN_0047f1a0(char* name, int param_2);
Entry_00492df0* __stdcall FUN_0049ff90(Entry_00492df0* entries, char* name);
int __stdcall FUN_0049fdf0(Entry_00492df0* entries, char* name, int type);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_004bbc30(char* path);
char* __stdcall FUN_00492b10(int* count);
void __stdcall FUN_004ab0a0(Gadget_00492df0* menu);
void FUN_00491ec0();
char* __stdcall FUN_004290f0(char* buf, char* dir, char* name, char* ext);
int __stdcall FUN_004326b0(char* path, char* description, int param_3);

// FUNCTION: 0x492df0
void __stdcall FUN_00492df0(Gadget_00492df0* gadget)
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
    if (FUN_0049fd60(gadget, "CANCEL")) {
        if (g_game->bit2_2a44)
            FUN_0049fa70(g_game->message);
        FUN_0047f1a0("Previous", 0);
        return;
    }
    if (FUN_0049fd60(gadget, "DELETE")) {
        FUN_0047f1a0("SmallButton", 0);
        Entry_00492df0* e = FUN_0049ff90(entries, "GAMES");
        sprintf(save.path, "%s\\%s", DAT_005091c8, FUN_004b6af0(DAT_0051f2e0, e->field_ba));
        FUN_004bbc30(save.path);
        FUN_00492b10(&save.count);
        FUN_004ab0a0(gadget);
        FUN_00491ec0();
        return;
    }
    if (!FUN_0049fd60(gadget, "GAMES") && !FUN_0049fd60(gadget, "LOAD") &&
        !FUN_0049fd60(gadget, "GAMENAME")) {
        if (gadget->field_60 != -1)
            FUN_004ab0a0(gadget);
        return;
    }
    if (g_game->bit2_2a44)
        FUN_0049fa70(g_game->message);
    FUN_0047f1a0("smlbutton", 0);
    int index = FUN_0049fdf0(entries, "GAMENAME", 3);
    char* text = entries[index].text;
    if (strlen(text) != 0) {
        save.games = (int)FUN_0049ff90(entries, "GAMES");
        FUN_004290f0(g_game->saveName, DAT_005091c8, text, "SAV");
        FUN_004326b0(g_game->saveName, text, time(0));
    }
}
