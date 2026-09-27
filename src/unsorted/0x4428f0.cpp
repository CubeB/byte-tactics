// Decompiled by Opus. Names are provisional.

struct Holder_004428f0 {
    int unknown_0;
    void* gadgets;                   // +0x4
};

struct Menu_004428f0 {
    char unknown_0[0x18];
    Holder_004428f0* holder;         // +0x18
};

struct Player_004428f0 {
    char unknown_0[0xba];
    short entry;                     // +0xba
};

// 0x102-byte records: a name and a number string.
struct Entry_004428f0 {
    char name[0x81];                 // +0x0
    char number[0x81];               // +0x81
};

extern Entry_004428f0* DAT_00512988;

void __stdcall FUN_004a0bf0(Menu_004428f0* obj, char* name, int param_3, int param_4);
int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004a7830(Menu_004428f0* menu, int index);
int __stdcall FUN_0049fc50(Menu_004428f0* obj, int index);
void __stdcall FUN_0049fa90(Menu_004428f0* menu);

// FUNCTION: 0x4428f0
void __stdcall FUN_004428f0(Menu_004428f0* menu, Player_004428f0* player)
{
    int entry = player->entry;
    if (entry >= 0) {
        FUN_004a0bf0(menu, "NAME", (int)DAT_00512988[entry].name, 0);
        FUN_004a0bf0(menu, "NUMBER", (int)DAT_00512988[entry].number, 0);
        int index = FUN_0049fdf0(menu->holder->gadgets, "NAME", 3);
        FUN_004a7830(menu, index);
        FUN_0049fc50(menu, index);
        FUN_0049fa90(menu);
    }
}
