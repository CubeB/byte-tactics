// Decompiled by Claude Opus 5.5. Names are provisional.
// Handles a click on an order button: finds which order the button's name
// contains and selects that order mode (FUN_00419bc0 inlined), plays the
// "immediateorders" or "specialorders" sound and returns 1; returns 0 when the
// name is no order. STOP issues the stop order at once.
//
// PARTIAL. Everything matches except two things that come from one cause:
// the original reads `button` for the else arm of the selection from its
// stack slot (`mov ebp, [esp+0x34]`) although it also keeps `button` in edi,
// and because that use is not a register use, `entries` gets esi and `button`
// edi (ours: the reverse, `mov ebp, esi`). No header set and no number of
// dummy declarations changes it, so the source shape is still wrong.
// Tried without effect: if/else, `!=` forms, a pointer local, a copy of the
// button in a local, reassigning the parameter, inline helpers taking the
// button by value, by pointer or by (const) reference, struct-wrapped or
// `int` parameters, a member function with `this` on the stack, and reading
// the slot through casts. Only an lvalue `?:` over references
// (`Entry*& Pick(Entry*& button, Entry*& s) { return s->type == 1 ? s : button; }`)
// reads the slot and fixes esi/edi (66.6%), but it also stores `s` to the
// stack and selects addresses, so it is not the original either.
// What that variant shows: the original treats `button` as an address-taken
// variable whose address never escapes (its loads are shared in edi, loaded
// after the pushes, with no reloads after calls), so look for a construct
// that takes `&button` locally and leaves no code of its own. Passing
// `&button` to a real function in a branch that is later removed also makes
// it address-taken, but then it is reloaded after every call.
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

struct Game_00419be0 {
    char unknown_0[0x2c76];
    char orders[0x2cc3 - 0x2c76];      // +0x2c76
    unsigned char orderMode;           // +0x2cc3
    char unknown_2cc4[2];
    unsigned char orderFlags;          // +0x2cc6
};
#pragma pack(pop)

extern Game_00419be0* g_game;

void __stdcall FUN_0049fed0(Entry_00419be0* entries, char* name, int index);
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_0048cf30(void* a, int b, Class_00438760 kind, int d, int e, int f);

static inline void SetOrderMode(unsigned char mode)
{
    g_game->orderMode = mode;
    g_game->orderFlags = g_game->orderFlags & 0xf7;
}

// FUNCTION: 0x419be0
int __stdcall FUN_00419be0(Entry_00419be0* button, Entry_00419be0* entries)
{
    char name[32];
    void* orders = g_game->orders;
    Entry_00419be0* e = entries[button->index].type == 1 ? &entries[button->index] : button;

    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "MOVE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(2);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "STOP")) {
        SetOrderMode(1);
        FUN_0048cf30(orders, 0, "STOP", 0, 0, 0);
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "ATTACK")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(3);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "BLAST")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(4);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "DEFEND")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(7);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "REPAIR")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(8);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "PATROL")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(9);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "RECLAIM")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xc);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "CAPTURE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xd);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "UNLOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(5);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "LOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(6);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    return 0;
}
