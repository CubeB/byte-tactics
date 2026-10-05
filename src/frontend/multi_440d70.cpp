// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>
#include <windows.h>

// Click handler of the "create new game" dialog (NEWMULTI.GUI, opened by
// OpenNewMultiDialog). Clicking a text field gives it the focus and loads its
// contents; OK copies the password (bounded to 11 characters) into g_game and
// the game and player names into two stack buffers, complains if either is
// empty, then starts the game (InitScoreReporting) and sets the pending front-end
// state at g_game+0x2bc0 to 17. CANCEL goes back to the previous dialog.

#pragma pack(push, 1)
struct Entry_00440d70 {               // 0x15b bytes
    char unknown_0[0xb6];
    char text[0x15b - 0xb6];         // +0xb6
};

struct Layer_00440d70 {
    int unknown_0;
    Entry_00440d70* entries;          // +0x4
};

struct Gadget_00440d70 {
    char unknown_0[0x18];
    Layer_00440d70* layer;            // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                     // +0x60, gadget that was clicked (-1 = close)
};

struct Game {
    char unknown_0[0x2bc0];
    unsigned char field_2bc0;         // +0x2bc0, pending front-end state
    char gameName[0x11];              // +0x2bc1
    char nickName[0x11];              // +0x2bd2
    char passWord[0x11];              // +0x2be3
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FindGadgetIndex(Entry_00440d70* entries, const char* name, int flag);
int __stdcall IsCurrentGadgetNamed(Gadget_00440d70* gadget, const char* name);
void __stdcall SelectGadgetByIndex(Gadget_00440d70* gadget, int index);
void __stdcall FUN_004a7190(Gadget_00440d70* gadget, int index);
void __stdcall FUN_0049fc50(Gadget_00440d70* gadget, int index);
void __stdcall FUN_0049fa90(Gadget_00440d70* gadget);
void __stdcall FUN_004ab0a0(Gadget_00440d70* gadget);
void __stdcall PlaySoundByName(const char* name, int flag);
Entry_00440d70* __stdcall FUN_004a0010(Entry_00440d70* entries, const char* name);
void __stdcall CloseTopScreen(Gadget_00440d70* gadget);
void OpenSelectGameDialog();
int InitScoreReporting();
char* __stdcall FUN_004c5740(const char* text);
void __stdcall OpenMessageBox(char* dest, const char* text, int a, int b, int c);

// FUNCTION: 0x440d70
void __stdcall HandleNewMultiClick(Gadget_00440d70* gadget)
{
    Entry_00440d70* entries = gadget->layer->entries;
    if (gadget->field_60 == -1)
        return;
    // The three text fields share one tail: the first two hand the next field
    // the focus, the third one the OK button.
    if (IsCurrentGadgetNamed(gadget, "GAMENAME")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "NICKNAME", 3));
        FUN_0049fc50(gadget, FindGadgetIndex(entries, "NICKNAME", 3));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "NICKNAME")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "PASSWORD", 3));
        FUN_0049fc50(gadget, FindGadgetIndex(entries, "PASSWORD", 3));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "PASSWORD")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "OK", 1));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "OK", 0xe) == gadget->field_60) {
        // 100 bytes each, and the pointer local in front of them, is what
        // puts them at +0x14 and +0x78 of the 0xcc-byte frame.
        char nickbuf[100];
        char namebuf[100];
        char* dst;
        int gi;                        // GAMENAME gadget index
        int ni;                        // NICKNAME gadget index
        PlaySoundByName("BigButton", 0);
        char* pw = (char*)FUN_004a0010(entries, "PASSWORD");
        lstrcpynA(g_game->passWord, pw + 0xb6, 0xb);
        gi = FindGadgetIndex(entries, "GAMENAME", 3);
        dst = namebuf;                 // copied through a pointer, as in the original
        strcpy(dst, entries[gi].text);
        if (strlen(namebuf) == 0) {
            FUN_004a7190(gadget, gi);
            FUN_004ab0a0(gadget);
            OpenMessageBox((char*)gadget, FUN_004c5740("You must enter a game name"), 0x140, 1, 1);
            return;
        }
        ni = FindGadgetIndex(entries, "NICKNAME", 3);
        strcpy(nickbuf, entries[ni].text);
        if (strlen(nickbuf) == 0) {
            FUN_004a7190(gadget, ni);
            FUN_004ab0a0(gadget);
            OpenMessageBox((char*)gadget, FUN_004c5740("You must enter your name"), 0x140, 1, 1);
            return;
        }
        strcpy(g_game->gameName, namebuf);
        strcpy(g_game->nickName, nickbuf);
        if (!InitScoreReporting()) {
            g_game->field_2bc0 = 0x11;
            return;
        }
        FUN_004ab0a0(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "CANCEL", 0xe) == gadget->field_60) {
        PlaySoundByName("Previous", 0);
        CloseTopScreen(gadget);
        OpenSelectGameDialog();
        return;
    }
    FUN_004ab0a0(gadget);
}
