// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1b];
    int field_37e1b;                   // +0x37e1b
};
#pragma pack(pop)

extern Game* g_game;

void FUN_004c2470();
void FUN_004257a0();
void FUN_004c2870();
void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_00429290(char* name, unsigned char* palette);
void __stdcall FUN_004ba200(unsigned char* palette, int first, int count);
void __stdcall FUN_004c69a0(int param_1);
void __stdcall FUN_004c6b70(void* dest, void* image, int x, int y);
void __stdcall FUN_004c6ac0(void* param_1);

// FUNCTION: 0x476740
void __stdcall FUN_00476740(char* name, int lock)
{
    char path[0x100];
    unsigned char palette[0x400];
    void* image;

    if (lock) {
        FUN_004c2470();
        FUN_004257a0();
    }
    FUN_004290f0(path, "bitmaps", name, "PCX");
    image = FUN_00429290(name, palette);
    FUN_004ba200(palette, 0, 0x100);
    FUN_004c69a0(g_game->field_37e1b);
    FUN_004c6b70(0, image, 0, 0);
    FUN_004c6ac0(image);
    if (lock) {
        FUN_004c2870();
    }
}
