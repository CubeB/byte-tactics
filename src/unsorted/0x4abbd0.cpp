// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens the YESORNO.GUI dialog, fills its CHC1 / CHC2 / TITL text fields and
// installs FUN_004abba0 as the handler. Same shape as 0x4abb20 / 0x4ac130.
#include <string.h>

struct Sub_004abbd0 {
    char unknown_0[0x18];
    void** field_18;                   // +0x18
};

struct Entry_004abbd0 {
    char unknown_0[0xb6];
    char text[0x15b - 0xb6];           // +0xb6
};

struct Layer_004abbd0 {
    int unknown_0;
    Entry_004abbd0* entries;           // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

Layer_004abbd0* __stdcall FUN_004aa8f0(Sub_004abbd0* sub, const char* name, int flags);
int __stdcall FUN_0049fdf0(void* entries, const char* name, int type);
void __stdcall FUN_004abba0(void* gadget);

// FUNCTION: 0x4abbd0
int __stdcall FUN_004abbd0(Sub_004abbd0* sub, char* param_2, char* param_3, char* param_4)
{
    Layer_004abbd0* layer = FUN_004aa8f0(sub, "YESORNO.GUI", 0x800);
    if (layer) {
        Entry_004abbd0* entries = (Entry_004abbd0*)sub->field_18[1];
        Entry_004abbd0* p1 = &entries[FUN_0049fdf0(entries, "CHC1", 1)];
        Entry_004abbd0* p2 = &entries[FUN_0049fdf0(entries, "CHC2", 1)];
        Entry_004abbd0* p3 = &entries[FUN_0049fdf0(entries, "TITL", 5)];
        strcpy(p1->text, param_4);
        strcpy(p2->text, param_3);
        strcpy(p3->text, param_2);
        layer->handler = FUN_004abba0;
        return 1;
    }
    return 0;
}
