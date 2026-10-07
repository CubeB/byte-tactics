// Decompiled by Space Bunny Free. Names are provisional.
// Opens the YESORNO.GUI dialog asking "Close Windows CD Player?", relabels
// the CHOICE1 / CHOICE2 gadgets, puts the localised "Yes" and "No" on the two
// gadgets the lookups found, and installs HandleCloseCdPlayerChoice as the
// gadget handler. SelectGadgetByName is then told about "CHOICE1".
#include <string.h>

extern char* g_game;

struct Gadget_00426200 {
    int unknown_0;
    char* entries;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

Gadget_00426200* __stdcall LoadGuiLayer(char* sub, const char* name, int flags);
void __stdcall FUN_0049fb10(char* sub, int value);
int __stdcall FindGadgetIndex(char* entries, const char* name, int type);
void __stdcall FUN_004a0bf0(char* sub, const char* name, const char* text, int param_4);
void __stdcall SelectGadgetByName(char* sub, const char* name);
void __stdcall RenderLayer(char* sub, int value);
char* __stdcall Translate(char* text);
void __stdcall HandleCloseCdPlayerChoice(void* gadget);

// FUNCTION: 0x426200
void __stdcall OpenCloseCdPlayerDialog()
{
    Gadget_00426200* gadget = LoadGuiLayer(g_game + 0x519, "YESORNO.GUI", 0x100);
    if (gadget != 0) {
        FUN_0049fb10(g_game + 0x519, 1);
        char* entries = gadget->entries;
        char* choice1 = entries + 0x15b * FindGadgetIndex(entries, "CHOICE1", 1);
        char* choice2 = entries + 0x15b * FindGadgetIndex(entries, "CHOICE2", 1);
        strcpy(entries + 0xcc, "CHOICE1");
        strcpy(entries + 0xdc, "CHOICE2");
        strcpy(choice1 + 0xb6, Translate("Yes"));
        strcpy(choice2 + 0xb6, Translate("No"));
        FUN_004a0bf0(g_game + 0x519, "TITLE", Translate("Close Windows CD Player?"), 0);
        SelectGadgetByName(g_game + 0x519, "CHOICE1");
        gadget->handler = HandleCloseCdPlayerChoice;
        RenderLayer(g_game + 0x519, 0x40);
    }
}
