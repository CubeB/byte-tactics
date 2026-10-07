// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Claude Opus 5.5. Names are provisional.
// Handles a click on an order button: finds which order the button's name
// contains and selects that order mode (FUN_00419bc0 inlined), plays the
// "immediateorders" or "specialorders" sound and returns 1; returns 0 when the
// name is no order. STOP issues the stop order at once.
#include <string.h>

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

#pragma pack(push, 1)
struct Entry_00419be0 {
    unsigned char type;                // +0x0
    char unknown_1[0x60 - 0x1];
    int index;                         // +0x60
    char unknown_64[0x138 - 0x64];
    short state;                       // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Game {
    char unknown_0[0x2c76];
    char orders[0x2cc3 - 0x2c76];      // +0x2c76
    unsigned char orderMode;           // +0x2cc3
    char unknown_2cc4[2];
    unsigned char orderFlags;          // +0x2cc6
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall GetGadgetName(Entry_00419be0* entries, char* name, int index);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall IssueOrderToSelection(void* a, int b, Class_00438760 kind, int d, int e, int f);

static inline void SetOrderMode(unsigned char mode)
{
    g_game->orderMode = mode;
    g_game->orderFlags = g_game->orderFlags & 0xf7;
}

// FUNCTION: 0x419be0
int __stdcall HandleOrderButtonClick(Entry_00419be0* button, Entry_00419be0* entries)
{
    char name[32];
    void* orders = g_game->orders;
    // Uninitialised unless the entry is type 1, as in the original: a `: button` fallback changes the code.
    Entry_00419be0* e;
    if (entries[button->index].type == 1) e = &entries[button->index];

    GetGadgetName(entries, name, button->index);
    if (strstr(name, "MOVE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(2);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "STOP")) {
        SetOrderMode(1);
        IssueOrderToSelection(orders, 0, "STOP", 0, 0, 0);
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "ATTACK")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(3);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "BLAST")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(4);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "DEFEND")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(7);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "REPAIR")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(8);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "PATROL")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(9);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "RECLAIM")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xc);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "CAPTURE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xd);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "UNLOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(5);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "LOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(6);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    return 0;
}
