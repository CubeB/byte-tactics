// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Opens the "TIMEOUT.GUI" dialog when the player entry with id `id` was
// dropped from the game (the TIMEOUT.GUI page does not exist yet).
// g_game holds ten 0x14b-byte player entries at +0x1b67, each with an int id
// at +0x00, a 0x40-byte name at +0x27 and a byte "in use" flag at +0x6f.
#include <string.h>

#pragma pack(push, 1)

struct Entry_00453a50 {
    int id;                                // +0x00 (g_game + 0x1b67)
    char unknown_4[0x27 - 4];
    char name[0x40];                       // +0x27 (g_game + 0x1b8e)
    char unknown_67[0x6f - 0x67];
    unsigned char valid;                   // +0x6f (g_game + 0x1bd6)
    char unknown_70[0x14b - 0x70];
};

struct Game {
    char unknown_0[0x1b67];
    Entry_00453a50 entries[10];            // +0x1b67
};

// The 0x15b-byte entry table returned by FUN_0049ff90.
struct OutEntry_00453a50 {
    char unknown_0[0x19];
    short field_19;                        // +0x19
};

// The dialog object returned by FUN_004aa8f0.
struct Gui_00453a50 {
    char unknown_0[4];
    void* entries;                         // +0x04
    void (__stdcall* callback)(void*);      // +0x08
    char unknown_c[0x1c - 0xc];
    void (__stdcall* field_1c)(void*);      // +0x1c
};

#pragma pack(pop)

extern Game* g_game;
extern int DAT_005061d8;
extern void* DAT_00512c74;

int __stdcall FUN_004ab060(void* obj, const char* name);
void* __stdcall FUN_004aa8f0(void* obj, const char* name, int size);
void __stdcall FUN_004a9660(void* obj);
OutEntry_00453a50* __stdcall FUN_0049ff90(void* entries, char* name);
int FUN_004a50b0();
void* __cdecl FUN_004d83b0(char* name, int size);
void __stdcall FUN_004a32a0(void* obj, char* name, void* p, int count, int flags);
int __stdcall FUN_0049fdf0(void* entries, char* name, int type);
void __stdcall FUN_004a7190(void* obj, int index);
void __stdcall FUN_004a0bf0(void* obj, char* name, void* out, int flag);
void __stdcall FUN_0049fb10(void* obj, int value);
void __stdcall FUN_004a81e0(void* obj, int value);
void __stdcall FUN_004538f0(void* gadget);
void __stdcall FUN_00453640(void* gadget);

static __inline unsigned char FindSlot_00453a50(int id)
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        int v;
        if (i == 10) {
            v = -1;
        } else {
            v = g_game->entries[i].valid ? g_game->entries[i].id : -1;
        }
        if (v == id) {
            return i;
        }
    }
    return 10;
}

// FUNCTION: 0x453a50
void __stdcall FUN_00453a50(int id)
{
    if (FUN_004ab060((char*)g_game + 0x519, "TIMEOUT.GUI")) {
        if (id == -1) {
            FUN_004a9660((char*)g_game + 0x519);
        }
        return;
    }
    if (id == -1) {
        return;
    }

    int i = FindSlot_00453a50(id);
    if (i == 10) {
        return;
    }

    Gui_00453a50* gui = (Gui_00453a50*)FUN_004aa8f0((char*)g_game + 0x519,
                                                    "TIMEOUT.GUI", 0x800);
    void* entries = gui->entries;
    gui->callback = &FUN_004538f0;
    DAT_005061d8 = id;

    void* p = FUN_004d83b0("LOUNGE CHATTER", 0xa00);
    DAT_00512c74 = p;
    memset(p, 0, 0x780);

    OutEntry_00453a50* out = FUN_0049ff90(entries, "OUTPUT");
    FUN_004a32a0((char*)g_game + 0x519, "OUTPUT", DAT_00512c74,
                 (int)out->field_19 / (FUN_004a50b0() + 2), 0);

    gui->field_1c = &FUN_00453640;
    FUN_004a7190((char*)g_game + 0x519, FUN_0049fdf0(entries, "TALK", 3));

    FUN_004a0bf0((char*)g_game + 0x519, "NAME",
                 g_game->entries[i].name, 0);
    FUN_0049fb10((char*)g_game + 0x519, 1);
    FUN_004a81e0((char*)g_game + 0x519, 0x40);
}
