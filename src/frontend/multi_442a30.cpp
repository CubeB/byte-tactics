// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Click handler of the modem/phone dialog.
// NAME/NUMBER select an account; HOST and JOIN move the selected account to the
// front of the modem number list, save it under MODEMNUMBERS and connect; PREV
// goes back; anything else resets the gadget.
// <windows.h> is required: without it MSVC emits the shift loop's address as
// `lea edi, [eax+ecx]` instead of the original `lea edi, [ecx+eax]`.
#include <windows.h>
#include <string.h>

// 0x102-byte records: a name and a number string.
struct Entry_00442a30 {
    char name[0x81];                   // +0x00
    char number[0x81];                 // +0x81
};

// GUI layout entry, as returned by FUN_0049ff90.
struct Layout_00442a30 {
    char unknown_0[0xba];
    short selected;                    // +0xba
};

// Object with the layout entry array at +4.
struct Table_00442a30 {
    int unknown_0;
    Layout_00442a30* entries;          // +0x04
};

struct Gadget_00442a30 {
    char unknown_0[0x18];
    Table_00442a30* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    void* menu;                        // +0x519
    char unknown_51d[0x531 - 0x51d];
    Table_00442a30* table;             // +0x531
    char unknown_535[0x2aaf - 0x535];
    unsigned short bit0 : 1;           // +0x2aaf
    unsigned short bit1 : 1;
    unsigned short bits2 : 14;
    char unknown_2ab1[0x39211 - 0x2ab1];
    int field_39211;                   // +0x39211
    int field_39215;                   // +0x39215
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_00512980;
extern Entry_00442a30* DAT_00512988;
extern char* DAT_0051298c;

int __stdcall FUN_00441c30(int* a, int* b);
void FUN_004426e0(void);
void __stdcall FUN_0042f960(void* key, void* buf, int value);
void __stdcall FUN_0047f1a0(const char* name, int param_2);
void FUN_004257a0(void);
void __stdcall FUN_00425860(int state, int line, const char* file);
Layout_00442a30* __stdcall FUN_0049ff90(void* entries, const char* name);
int __stdcall FUN_0049fdf0(void* entries, const char* name, int flag);
int __stdcall FUN_0049fd60(Gadget_00442a30* gadget, const char* name);
void __stdcall FUN_0049fc50(Gadget_00442a30* gadget, int value);
void __stdcall FUN_0049fa90(Gadget_00442a30* gadget);
void __stdcall FUN_004a0d00(void* menu, const char* key, void* out);
void __stdcall FUN_004a7830(Gadget_00442a30* gadget, int value);
void __stdcall FUN_004ab0a0(Gadget_00442a30* gadget);
char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004abd90(Gadget_00442a30* gadget, char* text, int a,
                            int b, int c);
void __cdecl FUN_004d85a0(void* data);

// The selected account is copied into the NAME and NUMBER gadgets and the
// account list is refreshed.
static inline void LoadAccount_00442a30()
{
    Layout_00442a30* entry = FUN_0049ff90(g_game->table->entries, "ACCOUNTS");
    if (entry != 0 && DAT_00512988 != 0) {
        FUN_004a0d00((char*)g_game + 0x519, "NAME",
                     DAT_00512988[entry->selected].name);
        FUN_004a0d00((char*)g_game + 0x519, "NUMBER",
                     DAT_00512988[entry->selected].number);
        FUN_004426e0();
    }
}

// The last used entry moves to the front of the modem number list, which is
// then written to the registry under MODEMNUMBERS.
static inline void SaveModemNumbers_00442a30()
{
    if (DAT_00512988 != 0) {
        short count = FUN_0049ff90(g_game->table->entries, "ACCOUNTS")->selected;
        if (count > 0) {
            Entry_00442a30 temp;
            memcpy(&temp, &DAT_00512988[count], 0x102);
            for (int i = count; i > 0; i--)
                memcpy(&DAT_00512988[i], &DAT_00512988[i - 1], 0x102);
            memcpy(&DAT_00512988[0], &temp, 0x102);
        }
        FUN_0042f960("MODEMNUMBERS", DAT_00512988, 0x1428);
    }
}

static inline int TryConnect_00442a30()
{
    int a = 0;
    int b = 0;
    int result = FUN_00441c30(&a, &b);
    if (result >= 0) {
        g_game->field_39211 = a;
        g_game->field_39215 = b;
    }
    return result;
}

// FUNCTION: 0x442a30
void __stdcall FUN_00442a30(Gadget_00442a30* gadget)
{
    Layout_00442a30* entries = gadget->layer->entries;
    if (gadget->field_60 == -1) {
        if (DAT_00512980 != 0) {
            FUN_004d85a0(DAT_00512980);
            DAT_00512980 = 0;
        }
        if (DAT_0051298c != 0) {
            FUN_004d85a0(DAT_0051298c);
            DAT_0051298c = 0;
        }
        if (DAT_00512988 != 0) {
            FUN_004d85a0(DAT_00512988);
            DAT_00512988 = 0;
        }
        return;
    }
    if (FUN_0049fd60(gadget, "NAME")) {
        LoadAccount_00442a30();
        FUN_0049fc50(gadget, FUN_0049fdf0(entries, "NUMBER", 3));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (FUN_0049fd60(gadget, "NUMBER")) {
        LoadAccount_00442a30();
        FUN_004a7830(gadget, FUN_0049fdf0(entries, "JOIN", 1));
        FUN_0049fc50(gadget, FUN_0049fdf0(entries, "JOIN", 1));
        strcpy((char*)entries + 0xcc, "JOIN");
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (FUN_0049fdf0(entries, "HOST", 0xe) == gadget->field_60) {
        LoadAccount_00442a30();
        SaveModemNumbers_00442a30();
        g_game->bit0 = 1;
        g_game->bit1 = 1;
        TryConnect_00442a30();
        FUN_0047f1a0("SMLBUTTON", 0);
        FUN_004257a0();
        return;
    }
    if (FUN_0049fdf0(entries, "JOIN", 0xe) != gadget->field_60) {
        if (FUN_0049fd60(gadget, "ACCOUNTS") == 0) {
            if (FUN_0049fd60(gadget, "PREV")) {
                FUN_00425860(0xf, 0x47f, "c:\\cavedog\\wargame\\multi.cpp");
                FUN_0047f1a0("Previous", 0);
                return;
            }
            FUN_004ab0a0(gadget);
            return;
        }
    }
    LoadAccount_00442a30();
    SaveModemNumbers_00442a30();
    g_game->bit1 = 1;
    g_game->bit0 = 0;
    TryConnect_00442a30();
    FUN_0047f1a0("SMLBUTTON", 0);
    FUN_004abd90(gadget, FUN_004c5740("Connecting... press ESC to abort"),
                 0xfa, 1, 1);
}
