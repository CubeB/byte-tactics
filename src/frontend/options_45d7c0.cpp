// Decompiled by space-bunny-free. Names are provisional.
// Loads the music GUI (MUSICRT.GUI when bit 0 of g_game+0x37ebe is set, else
// MUSIC plus the optmusic4x DLL), fills in the two callbacks of the object
// returned by FUN_0045cfc0, positions the MUSICVOL slider from the stored
// volume and hands the track type on to the sound object.
class Class_004ce5a0 {
public:
    int FUN_004ce5a0();
};

class Class_004ce7c0 {
public:
    void FUN_004ce7c0(int index, unsigned char value);
};

#pragma pack(push, 1)
struct Entry_0045d7c0 {
    char unknown_0[0x136];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
    char unknown_142[2];
    void (__stdcall* callback)(void* obj, int value); // +0x144
};

struct Gadget_0045d7c0 {
    char unknown_0[0x137];
    unsigned char value;               // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Holder_0045d7c0 {
    int unknown_0;
    Gadget_0045d7c0* gadgets;          // +0x4
};

struct Object_0045d7c0 {
    char unknown_0[4];
    Entry_0045d7c0* gadgets;           // +0x4
    void (__stdcall* callback8)(int value); // +0x8
    char unknown_c[0x1c - 0xc];
    void (__cdecl* callback1c)();      // +0x1c
};

struct Game {
    char unknown_0[0x10];
    Class_004ce7c0* sound;             // +0x10
    char unknown_14[0x519 - 0x14];
    char menu[1];                      // +0x519
    char unknown_51a[0x531 - 0x51a];
    Holder_0045d7c0* holder;           // +0x531
    char unknown_535[0x37ebe - 0x535];
    unsigned char flag_37ebe;          // +0x37ebe
    char unknown_37ebf[0x37f10 - 0x37ebf];
    short volume2;                     // +0x37f10
    char unknown_37f12[0x37f16 - 0x37f12];
    unsigned char state;               // +0x37f16
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00512fe0;

Object_0045d7c0* FUN_0045cfc0();
void __stdcall FUN_004a81e0(void* obj, int n);
void __cdecl FUN_0045ce80();
int __stdcall FUN_004aa8f0(void* obj, char* name, int size);
void __stdcall FUN_004288d0(char* name, int a, int b, int c);
void __stdcall FUN_0049fa50(void* obj);
void __stdcall FUN_004a1110(void* obj, char* name, int flag);
int __stdcall FUN_0049fdf0(void* list, char* name, int flag);
Entry_0045d7c0* __stdcall FUN_004a0200(Entry_0045d7c0* list, char* name);
void __cdecl FUN_0045d130();
void __cdecl FUN_0045c3f0();
void __stdcall FUN_0049fa90(void* obj);
void __stdcall FUN_0049fb10(void* obj, int n);
void __cdecl FUN_00428b60();

void __stdcall FUN_0045d280(int value);
void __cdecl FUN_0045d0c0();
void __stdcall FUN_0045bea0(void* obj, int value);

// FUNCTION: 0x45d7c0
void FUN_0045d7c0()
{
    Object_0045d7c0* obj = FUN_0045cfc0();
    FUN_004a81e0(g_game->menu, 2);
    FUN_0045ce80();
    if (g_game->flag_37ebe & 1) {
        FUN_004aa8f0(g_game->menu, "MUSICRT.GUI", 0x280);
    } else {
        FUN_004aa8f0(g_game->menu, "MUSIC", 0x200);
        FUN_004288d0("optmusic4x", 0, 0, 0);
    }
    obj->callback8 = FUN_0045d280;
    FUN_0049fa50(g_game->menu);
    obj->callback1c = FUN_0045d0c0;
    FUN_004a1110(g_game->menu, "MUSIC", 1);
    if (FUN_0049fdf0(obj->gadgets, "MUSICVOL", 0xe) != -1) {
        Entry_0045d7c0* e = FUN_004a0200(obj->gadgets, "MUSICVOL");
        e->max = 0x40;
        e->callback = FUN_0045bea0;
        e->pos = g_game->volume2;
        int value = e->pos;
        if (value > 0x40) {
            value = 0x40;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.015625f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
    }
    FUN_0045d130();
    if (g_game->state == 3) {
        DAT_00512fe0 = ((Class_004ce5a0*)g_game->sound)->FUN_004ce5a0();
    }
    FUN_0045c3f0();
    Gadget_0045d7c0* gadgets = g_game->holder->gadgets;
    if (g_game->state == 4) {
        int index = FUN_0049fdf0(gadgets, "TRACKTYPE", 1);
        g_game->sound->FUN_004ce7c0(DAT_00512fe0, gadgets[index].value);
    }
    FUN_0049fa90(g_game->menu);
    FUN_0049fb10(g_game->menu, 1);
    FUN_00428b60();
    FUN_004a81e0(g_game->menu, 0x40);
}
