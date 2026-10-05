// Decompiled by space-bunny-free. Names are provisional.
// Runs the script for this map's network slot 7 (the "ai" scripts), falling
// back to the shipped ai\default.txt, then re-runs the two unit-name loops for
// every player whose type byte is 2.

class Class_004356c0 {
public:
    char* FUN_004356c0(int index);
};

#pragma pack(push, 1)
struct Player_004648e0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004648e0 players[10];       // +0x1b63
    char unknown_2851[0x391e9 - 0x2851];
    Class_004356c0* net;               // +0x391e9
};
#pragma pack(pop)

class Class_004b74f0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
};

class Class_004b73b0 {
public:
    char unknown_0[0xd0];
    int field_d0;
    Class_004b73b0* FUN_004b73b0();
};

extern Game* g_game;

char* __stdcall FUN_004bbe50(const char* name, int* size);
int __stdcall FUN_004b7a30(char* text, int len, Class_004b74f0* vars, int param_4);
void __cdecl FUN_004d85a0(char* text);
void __stdcall FUN_00409f80(int player);
void __stdcall FUN_0040a040(int player);

// FUNCTION: 0x4648e0
void FUN_004648e0()
{
    int size;
    char* name = g_game->net->FUN_004356c0(7);
    char* text = FUN_004bbe50(name, &size);
    if (text == 0) {
        text = FUN_004bbe50("ai\\default.txt", &size);
    }
    if (text != 0) {
        Class_004b74f0 vars;
        ((Class_004b73b0*)&vars)->FUN_004b73b0();
        FUN_004b7a30(text, size, &vars, -1);
        FUN_004d85a0(text);
    }
    for (int i = 0; i < 10; i++) {
        Player_004648e0* p = &g_game->players[i];
        if (p->active && p->type == 2) {
            FUN_00409f80(i);
            FUN_0040a040(i);
        }
    }
}
