// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by Claude Fable 5.1. Names are provisional.
// Screenshot writer: renders the map in screen-sized tiles into an offscreen
// bitmap and appends each band to a .bmp file.
// The frame layout that stalled this at 93.9% is decided by MSVC 5's local
// ordering rule (measured in build/scratch/0x495a30/model2.py): locals are
// sorted by memory references divided by size, highest ratio nearest esp,
// ties broken in favour of the earlier declared object. The clip rect is a
// plain 16-byte Rect (2 refs, 0.125) and the sprite reference built by
// FUN_004b8ae0 is 24 bytes, not 20 (3 refs, 0.125): the tie puts the sprite
// at 0x64 and the rect at 0x7c, under the 48-byte surface (5 refs, 0.104).
// With a 20-byte sprite the rect would have to be 20 bytes to put its bottom
// at 0x88, and a 20-byte rect (0.100) sorts above the surface. 0x44c0d0.cpp
// records the same 24-byte record ("0x14-byte sprite reference plus 4
// trailing bytes").
// <windows.h> is load-bearing: without it the score drops to 83.3%.
#include <windows.h>
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1423b];
    int screenTilesX;                       // +0x1423b
    int screenTilesY;                       // +0x1423f
    char unknown_14243[0x14281 - 0x14243];
    unsigned short viewFlags;               // +0x14281
    char unknown_14283[0x1431f - 0x14283];
    int scrollX;                            // +0x1431f
    int scrollY;                            // +0x14323
    char unknown_14327[0x37e27 - 0x14327];
    int field_37e27;                        // +0x37e27
    int field_37e2b;                        // +0x37e2b
    char unknown_37e2f[0x37f27 - 0x37e2f];
    int field_37f27;                        // +0x37f27
    char unknown_37f2b[0x37f2f - 0x37f2b];
    unsigned short field_37f2f;             // +0x37f2f
    char unknown_37f31[0x38a51 - 0x37f31];
    unsigned short field_38a51;             // +0x38a51
};
#pragma pack(pop)

extern Game* g_game;

struct Class_004b8da0;

struct Class_004cb7c0 {
    char unknown_0[0xc];
    int field_c;                            // +0xc
    Class_004cb7c0* FUN_004cb7c0();
};

struct Class_004cb7f0 : public Class_004cb7c0 {
    bool FUN_004cb7f0(const char* name, int width, int height);
};

struct Class_004cb940 {
    bool FUN_004cb940(void* image, int x, int rows, int unused_4, int y, int unused_6, int srcY);
};

struct Class_004cb7d0 {
    char unknown_0[0xc];
    void* file;                             // +0xc
    void FUN_004cb7d0();
};

struct Dst_004b8ae0 {
    unsigned short a;                       // +0x0
    unsigned short b;                       // +0x2
    unsigned short e;                       // +0x4
    unsigned short f;                       // +0x6
    unsigned char flag8;                    // +0x8
    unsigned char flag9;                    // +0x9
    unsigned char flaga;                    // +0xa
    unsigned char flagb;                    // +0xb
    char unknown_c[4];
    int d;                                  // +0x10
    int field_14;                           // +0x14
};

struct Src_004b8ae0 {
    char unknown_0[0xbc];
};

struct SrcHolder_00495a30 {
    char unknown_0[0xbc];
    Src_004b8ae0* frame;                    // +0xbc
};

struct Surface_00495a30 {
    int width;                              // +0x0
    int height;                             // +0x4
    int pitch;                              // +0x8
    unsigned char* bits;                    // +0xc
    int field_10;                           // +0x10
    int field_14;                           // +0x14
    unsigned short x;                       // +0x18
    unsigned short y;                       // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;                 // +0x2c
    unsigned int flag1 : 1;
};

struct Rect_00495a30 {
    int left;                               // +0x0
    int top;                                // +0x4
    int right;                              // +0x8
    int bottom;                             // +0xc
};

struct Class_004c6b10 {
    char unknown_0[0x1c];
    void FUN_004c6b10(Rect_00495a30 r);
};

void __stdcall FUN_00495930(char* out, const char* dir, const char* name, const char* ext);
Class_004b8da0* __stdcall FUN_004b8da0(const char* name, int width, int height);
void __cdecl FUN_004d8e50(int param);
void __stdcall FUN_0049e6f0();
void __stdcall FUN_004b8a80(Surface_00495a30* dst, void* src);
void* __stdcall FUN_004b6220();
void __stdcall FUN_004b8ae0(Dst_004b8ae0* dst, Src_004b8ae0* src);
void __stdcall FUN_0041c4c0(int x, int y, int z);
void __stdcall FUN_0048bae0();
void __stdcall FUN_00468cf0(int a, int b);
void __stdcall FUN_004b7f90(Surface_00495a30* surf, Dst_004b8ae0* pal, int x, int y);
void __stdcall FUN_004b8e50(void* b, int color);
void __stdcall FUN_004816a0(int param);
void __cdecl FUN_004d85a0(void* b);

// FUNCTION: 0x495a30
void __stdcall FUN_00495a30(char* dir, char* name, int x, int y, int w, int h)
{
    int var24;
    unsigned int savedC;
    unsigned short fl;
    char filename[260];
    int bw, off27, sy;
    FUN_00495930(filename, dir, name, "bmp");

    int y2;
    Class_004cb7f0 bmp;
    bmp.FUN_004cb7c0();
    if (bmp.FUN_004cb7f0(filename, w, h)) {
        Class_004b8da0* bm;
        off27 = g_game->field_37e27;
        int bh, off2b;
        bw = g_game->screenTilesX * 16;
        off2b = g_game->field_37e2b;
        bh = (g_game->screenTilesY * 16) - 1;

        FUN_004d8e50(0);
        bm = FUN_004b8da0("ScreenShot", w, bh);
        if (bm == 0) {
            bh /= 2;
            bm = FUN_004b8da0("ScreenShot", w, bh);
        }
        FUN_0049e6f0();
        if (bm != 0) {
            int scrollX;
            Surface_00495a30 surf;
            scrollX = g_game->scrollX;
            Dst_004b8ae0 pal;
            fl = g_game->viewFlags;
            int savedbit0, scrollY = g_game->scrollY, bit6;
            int savedbit1;
            savedbit0 = fl & 1;
            savedbit1 = (fl >> 1) & 1;

            int savedA;
            g_game->viewFlags = fl & ~1;
            g_game->viewFlags &= ~2;
            FUN_004816a0(1);
            savedA = g_game->field_38a51 & 1;
            g_game->field_38a51 = (unsigned short)(g_game->field_38a51 & ~1);
            int savedB = (g_game->field_37f2f >> 6) & 1;

            g_game->field_37f2f = (unsigned short)(g_game->field_37f2f & ~0x40);
            savedC = g_game->field_37f27;

            g_game->field_37f27 = 0;
            FUN_004b8a80(&surf, bm);
            FUN_004b8ae0(&pal, ((SrcHolder_00495a30*)FUN_004b6220())->frame);

            pal.flag8 = 0;
            int row = 0;

            if (h > row) {
                int var1c, negbh;
                var24 = bh;
                unsigned int var10 = 0;
                negbh = -bh;
                var1c = -1;
                while (1) {
                    FUN_004b8e50(bm, 0);
                    int col, rows;
                    col = 0;
                    if (col < w) do {
                        int right;
                        Rect_00495a30 box;
                        box.left = col;
                        FUN_0041c4c0(x + col, y + row, 0);
                        FUN_0048bae0();
                        FUN_00468cf0(1, 0);
                        right = (col + bw) - 1;
                        if (right >= surf.width) right = surf.width - 1;
                        box.top = 0;
                        box.right = right;
                        box.bottom = bh - 1;
                        ((Class_004c6b10*)&surf)->FUN_004c6b10(box);
                        FUN_004b7f90(&surf, &pal, g_game->scrollX - x - off27,
                                     g_game->scrollY - row - y - off2b);
                    } while (((col += bw), (col < w)));
                    sy = var10;
                    int srcY = 0;
                    rows = bh;
                    y2 = row;
                    if (var1c < -1) {
                        y2 = row + 1;
                        sy = var1c;
                        rows = bh - 1;
                        srcY = 1;
                    }
                    if (y2 + rows > h) rows = h + sy;
                    if (!((Class_004cb940*)&bmp)->FUN_004cb940(&surf, w, rows, 0, row, 0, srcY)) break;
                    if (var24 < h) {
                        row = row - 1;
                        var10++;
                        var1c = var1c + 1;
                        var24 = var24 - 1;
                    }
                    row += bh;
                    var10 += negbh;
                    var1c = var1c + negbh;
                    var24 += bh;
                    if (row >= h) break;
                }
            }
            FUN_004d85a0(bm);
            g_game->field_38a51 = (unsigned short)(g_game->field_38a51 ^ ((savedA ^ g_game->field_38a51) & 1));
            bit6 = (savedB & 1) << 6;
            g_game->field_37f2f = (unsigned short)((((unsigned short)g_game->field_37f2f) & ~0x40) | bit6);
            g_game->field_37f27 = savedC;
            FUN_0041c4c0(scrollX, scrollY, 0);
            g_game->viewFlags = (unsigned short)(g_game->viewFlags ^ ((savedbit0 ^ g_game->viewFlags) & 1));
            g_game->viewFlags = (unsigned short)((unsigned short)((g_game->viewFlags & ~2) | ((savedbit1 & 1) << 1)));
            FUN_004816a0(1);
            FUN_0048bae0();
            FUN_00468cf0(1, 1);
        }
    }
    ((Class_004cb7d0*)&bmp)->FUN_004cb7d0();
}
