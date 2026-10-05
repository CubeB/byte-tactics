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
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadBitmapByName(char* name, unsigned char* palette);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall SetOffscreenSurface(int param_1);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall FreeSurface(void* param_1);

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
    BuildDataPath(path, "bitmaps", name, "PCX");
    image = LoadBitmapByName(name, palette);
    SetPaletteColors(palette, 0, 0x100);
    SetOffscreenSurface(g_game->field_37e1b);
    DrawSurface(0, image, 0, 0);
    FreeSurface(image);
    if (lock) {
        FUN_004c2870();
    }
}
