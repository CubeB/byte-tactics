// Decompiled by Opus. Names are provisional.
// Frees the buffer held at +0x1749, then does the same flag-0 cleanup as
// 0x450dd0.

struct Game {
    char unknown_0[0x14];
    char field_14[0x2a44 - 0x14];
    unsigned short flags_2a44;
};

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

#pragma pack(push, 1)
struct Class_00452370 {
    char unknown_0[0x1749];
    int* buffer;                       // +0x1749
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

bool FUN_0046bf20();
void __stdcall FUN_004c9b70(void* param_1);
void FUN_004d85a0(int* param_1);

// FUNCTION: 0x452370
void __stdcall FUN_00452370(Class_00452370* obj)
{
    if (obj->buffer) {
        FUN_004d85a0(obj->buffer);
        obj->buffer = 0;
    }
    if (g_game->flags_2a44 & 1) {
        if (DAT_00506dbc != 0) {
            DAT_00513000.FUN_004618a0(1);
        }
        if (!FUN_0046bf20()) {
            FUN_004c9b70(g_game->field_14);
        }
        g_game->flags_2a44 &= 0xfffe;
    }
}
