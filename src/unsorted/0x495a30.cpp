// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes a large screenshot ("BIGSHOT", caller 0x417600): renders the map in
// screen-sized tiles and copies each rendered tile into one big 8-bit surface,
// saved as a BMP.
//
// Best so far: 50.6 percent (962 of 1105 bytes). Fixes this round:
//  - FUN_004d8e50 and FUN_004d85a0 are __cdecl (the original has `add esp,4`
//    after each call), not __stdcall. That alone took 50.1 -> 50.6.
//  - the missing Class_004cb940::FUN_004cb940 write call was reconstructed
//    (args &surf, w, rows, 0, row, 0, flag with rows/oy/off/flag chosen from
//    var24/m/n/bh); it grows the frame to 0x1b4 vs 0x1b0 and reallocates every
//    register, scoring 43.9. Saved at build/scratch/0x495a30/withcall.cpp.
// What still differs:
//  - frame is 0x1ac; the original is 0x1b0. The filename buffer is at the same
//    absolute address in both minus 4, so the original has one extra 4-byte
//    local somewhere below the filename that is not identified.
//  - the original keeps the four pre-loop values in S+0x20 (screenTilesX<<4),
//    S+0x44 (field_37e27), S+0x30 (field_37e2b), S+0x18 ((screenTilesY<<4)-1);
//    ours keeps three of them in different slots and picks ebx where the
//    original picks ecx for screenTilesX.
//  - the viewFlags block: the original loads with `mov cx,[14281]`, copies to
//    eax, masks 0xffff, and keeps both saved bits in registers, storing only
//    the scrollX/scrollY pair to S+0x34/S+0x2c. Ours stores fl itself and
//    recomputes, so the tail's restore uses a different xor/and sequence (two
//    restores instead of the original's xor-form for bit 0 plus and/or for
//    bit 1).
// Callee call sequence, argument order and the loop control flow all match.
#pragma pack(push, 1)
struct Game_00495a30 {
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

extern Game_00495a30* g_game;

struct Class_004b8da0;

struct Class_004cb7c0 {
    char unknown_0[0xc];
    int field_c;                            // +0xc
    Class_004cb7c0* FUN_004cb7c0();
};

struct Class_004cb7f0 : public Class_004cb7c0 {
    bool FUN_004cb7f0(const char* name, int width, int height);
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
};

struct Src_004b8ae0 {
    char unknown_0[0xbc];
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

struct Image_004cb940 {
    int width;                              // +0x0
    int unknown_4;
    int pitch;                              // +0x8
    unsigned char* pixels;                  // +0xc
};

class Class_004cb940 {
public:
    int width;                              // +0x0
    int height;                             // +0x4
    int dataOffset;                         // +0x8
    void* file;                             // +0xc
    bool FUN_004cb940(Image_004cb940* image, int x, int rows, int unused_4,
                      int y, int unused_6, int srcY);
};

void __stdcall FUN_00495930(char* out, const char* dir, const char* name, const char* ext);
Class_004b8da0* __stdcall FUN_004b8da0(unsigned int name, int width, int height);
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
    char filename[260];
    FUN_00495930(filename, dir, name, "bmp");


    Class_004cb7f0 bmp;
    bmp.FUN_004cb7c0();
    if (bmp.FUN_004cb7f0(filename, w, h)) {
    int bw = g_game->screenTilesX << 4;
    int bh = (g_game->screenTilesY << 4) - 1;
    int off27 = g_game->field_37e27;
    int off2b = g_game->field_37e2b;

    FUN_004d8e50(0);
    Class_004b8da0* bm = FUN_004b8da0(0x5094d8, w, bh);
    if (bm == 0) {
        bh = bh / 2;
        bm = FUN_004b8da0(0x5094d8, w, bh);
    }
    FUN_0049e6f0();
    if (bm != 0) {
    int scrollY = g_game->scrollY;
    unsigned short fl = g_game->viewFlags;
    int scrollX = g_game->scrollX;
    g_game->viewFlags = (unsigned short)(fl & ~1);
    g_game->viewFlags &= ~2;
    FUN_004816a0(1);

    int savedA = g_game->field_38a51 & 1;
    g_game->field_38a51 = (unsigned short)(g_game->field_38a51 & ~1);
    int savedB = (g_game->field_37f2f >> 6) & 1;
    g_game->field_37f2f = (unsigned short)(g_game->field_37f2f & ~0x40);
    int savedC = g_game->field_37f27;
    g_game->field_37f27 = 0;

    Surface_00495a30 surf;
    FUN_004b8a80(&surf, bm);

    Dst_004b8ae0 pal;
    FUN_004b8ae0(&pal, (Src_004b8ae0*)((char*)FUN_004b6220() + 0xbc));
    pal.flag8 = 0;

    int row = 0;
    if (h > 0) {
        int var10 = 0;
        int var1c = -1;
        int var24 = bh;
        int negbh = -bh;
        do {
            FUN_004b8e50(bm, 0);
            if (w > 0) {
                int col = 0;
                do {
                    FUN_0041c4c0(x + col, y + row, 0);
                    FUN_0048bae0();
                    FUN_00468cf0(1, 0);
                    Rect_00495a30 r;
                    r.left = col;
                    r.top = 0;
                    int right = col + bw - 1;
                    if (right >= surf.width) {
                        right = surf.width - 1;
                    }
                    r.right = right;
                    r.bottom = bh - 1;
                    ((Class_004c6b10*)&surf)->FUN_004c6b10(r);
                    FUN_004b7f90(&surf, &pal, scrollX - x - off27,
                                 scrollY - row - y - off2b);
                    col += bw;
                } while (col < w);
            }
            if (var24 < h) {
                row--;
                var10++;
                var1c++;
                var24--;
            }
            row += bh;
            var10 += negbh;
            var1c += negbh;
            var24 += bh;
        } while (row < h);
    }

    FUN_004d85a0(bm);
    g_game->field_38a51 =
        (unsigned short)(((g_game->field_38a51 ^ savedA) & 1) ^ g_game->field_38a51);
    g_game->field_37f2f =
        (unsigned short)((g_game->field_37f2f & ~0x40) | (savedB << 6));
    g_game->field_37f27 = savedC;
    FUN_0041c4c0(scrollX, scrollY, 0);
    g_game->viewFlags =
        (unsigned short)(((g_game->viewFlags ^ (fl & 1)) & 1) ^ g_game->viewFlags);
    g_game->viewFlags =
        (unsigned short)((g_game->viewFlags & ~2) | (((fl >> 1) & 1) << 1));
    FUN_004816a0(1);
    FUN_0048bae0();
    FUN_00468cf0(1, 1);
    }
    }
    ((Class_004cb7d0*)&bmp)->FUN_004cb7d0();
}
