// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>
#include <stdio.h>

class Class_004ce1d0 {
public:
    int FUN_004ce1d0();
};

class Class_004ce690 {
public:
    void FUN_004ce690(int value);
};

class Class_004cff20 {
public:
    int FUN_004cff20();
};

#pragma pack(push, 1)
struct Dialog_004263b0 {
    char unknown_0[4];
    char* gadgets;                    // +0x4
    void (__stdcall* handler)(void*); // +0x8
    int field_c;                      // +0xc
    char unknown_10[0x1c - 0x10];
    void (__stdcall* field_1c)();     // +0x1c
};

struct Sub_004263b0 {
    char unknown_0[0x18];
    Dialog_004263b0* current;         // +0x18
};

struct Game {
    char unknown_0[0x10];
    void* field_10;                   // +0x10
    char unknown_14[0x519 - 0x14];
    Sub_004263b0 sub;                 // +0x519
    char unknown_535[0xdda - 0x535];
    unsigned char field_dda;          // +0xdda
    char unknown_ddb[0x37e1b - 0xdda - 1];
    int field_37e1b;                  // +0x37e1b
    char unknown_37e1f[0x391f9 - 0x37e1f];
    void* field_391f9;                // +0x391f9
};

struct Entry_004263b0 {
    void* value;                      // +0x0
    char name[0x24];                  // +0x4
};
#pragma pack(pop)

extern Game* g_game;
extern Entry_004263b0 DAT_005120bc[10];
extern char DAT_004fd050[];

static char* Lookup_004263b0(const char* name)
{
    if (name != 0) {
        for (int i = 0; i < 10; i++) {
            if (strcmp(DAT_005120bc[i].name, name) == 0)
                return (char*)DAT_005120bc[i].value;
        }
    }
    return 0;
}

extern int DAT_00512290;
extern int DAT_00512294;
extern int DAT_0051228c;
extern int DAT_0051229c;
extern char* DAT_00512298;

void __stdcall FUN_004a9660(Sub_004263b0* sub);
void FUN_004c2470();
void __stdcall SetOffscreenSurface(int param);
void __stdcall FillSurface(int a, int b);
void FlipScreen();
void FUN_00491a70();
Dialog_004263b0* __stdcall FUN_004aa8f0(Sub_004263b0* sub, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_0047f210(const char* name, int a);
void __stdcall FUN_0049fa50(Sub_004263b0* sub);
void __stdcall FUN_00425d80(void* gadget);
void __stdcall FUN_00425b80();

void __stdcall FUN_004290f0(char* dest, const char* a, const char* b, const char* c);
void* __stdcall FUN_004bbe50(char* name, int flag);
void __stdcall FUN_004ac7d0(Sub_004263b0* sub, int value, void* palette);
void __cdecl FUN_004d85a0(void* palette);
void __stdcall FUN_004a81e0(Sub_004263b0* sub, int value);
void __stdcall FUN_0049fb10(Sub_004263b0* sub, int value);
void __stdcall SetFont(void* param);
void __stdcall FUN_004a0570(Sub_004263b0* sub, const char* name, int value);
void __stdcall FUN_004a07d0(Sub_004263b0* sub, const char* name, const char* text);
int __stdcall FUN_004a5030(const char* text);
int __stdcall FUN_0049fdf0(char* gadgets, const char* name, int type);
int GetTextKeyColor();
void __stdcall SetTextColors(unsigned int a, int b);
void FUN_004c2870();
void FUN_004c2bb0();
void* __cdecl FUN_004d83b0(const char* name, int size);
void __stdcall FUN_00426200();
int __stdcall CheckDirectXVersion(int a, int b, int c, int d, int e);
char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004abd90(Sub_004263b0* sub, char* text, int a, int b, int c);
void __stdcall FUN_00429000();

// FUNCTION: 0x4263b0
void __stdcall FUN_004263b0()
{
    while (g_game->sub.current != 0) {
        FUN_004a9660(&g_game->sub);
    }

    FUN_004c2470();
    SetOffscreenSurface(g_game->field_37e1b);
    FillSurface(0, 0);
    FlipScreen();
    FUN_00491a70();

    Dialog_004263b0* dialog = FUN_004aa8f0(&g_game->sub, "MAINMENU.GUI", 0x80);
    dialog->handler = FUN_00425d80;
    dialog->field_c = 0;
    dialog->field_1c = FUN_00425b80;

    FUN_004288d0("FrontendX", 1, 1, 0);
    FUN_0047f210("BGM", 0);
    ((Class_004ce690*)g_game->field_10)->FUN_004ce690(4);
    FUN_0049fa50(&g_game->sub);

    char* name = "FrontendX";
    char* found = Lookup_004263b0(name);

    char version[32];
    char palpath[256];
    char text[300];

    FUN_004290f0(palpath, "palettes", "guipal", "PAL");
    void* palette = FUN_004bbe50(palpath, 0);
    FUN_004ac7d0(&g_game->sub, (int)found, palette);
    FUN_004d85a0(palette);
    FUN_004a81e0(&g_game->sub, 0xc0);
    FUN_0049fb10(&g_game->sub, 1);
    SetFont(g_game->field_391f9);

    strcpy(version, "v3.1");
    strcpy(palpath, version);
    FUN_004a0570(&g_game->sub, "DebugString", 1);
    FUN_004a07d0(&g_game->sub, "DebugString", palpath);

    char* gadgets = g_game->sub.current->gadgets;
    int width = FUN_004a5030(palpath);
    short* px = (short*)(gadgets + 0x15b * FUN_0049fdf0(gadgets, "DebugString", 5) + 0x13);
    *px += -(width / 2);

    SetTextColors(g_game->field_dda, GetTextKeyColor());
    FUN_004c2870();
    FUN_004c2bb0();

    DAT_00512298 = (char*)FUN_004d83b0("SPARKS", 0x514);
    memset(DAT_00512298, 0, 0x145 * 4);

    if (DAT_0051229c == 0) {
        if (((Class_004ce1d0*)g_game->field_10)->FUN_004ce1d0()) {
            FUN_00426200();
            DAT_0051229c = 1;
        }
    }

    if (DAT_0051228c == 0) {
        DAT_0051228c = 1;
        if (CheckDirectXVersion(4, 5, 0, 0x9b, 3) == 0) {
            if (_snprintf(text, 300, FUN_004c5740(DAT_004fd050), "\n", "\n", "\n", "\n") < 0) {
                text[299] = 0;
            }
            FUN_004abd90(&g_game->sub, text, 200, 1, 1);
        }
    }

    if (DAT_00512294 == 0) {
        if (((Class_004cff20*)g_game->field_10)->FUN_004cff20()) {
            FUN_004abd90(&g_game->sub, FUN_004c5740("No sound driver is available for use.\n"), 500, 1, 1);
            DAT_00512294 = 1;
        }
    }

    if (DAT_00512290 == 0) {
        FUN_00429000();
        DAT_00512290 = 1;
    }
}
