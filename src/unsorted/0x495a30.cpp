// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// 93.9% best. Wins that got here: the Class_004cb940 loop-tail call with
// sy/srcY/rows/y2 declared at the point of use (register-only); FUN_004b7f90
// coords written with g_game->scrollX / g_game->scrollY inline (else MSVC
// hoists them out of the inner loop); the inner loop is `for (int col = 0;
// col < w; col += bw)` (col = 0 sits before the w guard, and the outer
// `if (row < h)` then compiles to `cmp [h], ebp` instead of `test eax, eax`);
// the loop inits assigned in the original's store order (var24, var10,
// negbh, var1c); `int savedbit0 = fl & 1; int savedbit1 = (fl >> 1) & 1;`
// order; scrollX declared before fl and scrollY after; the tail restores as
// `x ^ ((savedA ^ x) & 1)` xor-idioms with the 37f2f restore written BEFORE
// the 37f27 assign and `int tmp10 = (savedB & 1) << 6;` split out; direct
// `g_game->field_38a51 = (g_game->field_38a51 & ~1);` style stores (the
// `int tmp5 = ...` temp spellings cost 3 bytes each through `xor eax, eax;
// and al, 0xfe` instead of `and eax, 0xfffe`).
// Still differs: ONE thing, the frame layout. The 20-byte box home
// (Box_00495a30 {int pad; Rect r}, its r.bottom = bh-1 cache at +0x10) sorts
// ABOVE the 48-byte surf: ours is [pal 0x64][surf 0x78][box 0xa8], the
// original has [pal 0x64][box 0x78][surf 0x8c]. Everything else matches.
// The gap that follows from it is bigger than it looks: surf at 0x8c makes
// the original's `lea edx, [esp+0x90]` and `mov ecx, [esp+0x8c]` encode as
// disp32 (7 bytes) where ours encode as disp8 (4 bytes), so we are exactly 9
// bytes short (three instructions x 3 bytes) and every in-function branch
// target address differs as a result. Fixing the layout should give 100%.
// What was tried for the layout (all scored with check.py --sym or probed
// with build/scratch/0x495a30/lay.py): the box as {Rect r; int bottom} (home
// disappears, frame 0x19c), as a plain Rect whole-pass (home 16, lands at
// 0x78 = exactly the right slot but 4 bytes short, frame 0x1ac), slicing a
// Box : Rect through FUN_004c6b10(box) (home disappears), *(Rect*)&box
// casts (home shrinks to the 4-byte cache), constructor temporaries (the
// named box home disappears), aggregate initialisers, const boxes, writes
// through pointers, and moving the declaration to every scope (function
// body, outer loop, inner loop) and every position, and moving the struct
// definitions around. All of them either inflate the box above surf (when
// its sub-object box.r is the by-value argument source) or shrink/eliminate
// the home. Plain struct size order explains every other home (16 bmp, 20
// pal, 48 surf, 260 filename, ascending with address), so the original's
// 20-byte home is somehow NOT inflated even though its +0x10 field feeds
// the by-value rect copy. #include <windows.h> is load-bearing: without it
// the score drops to 83.3%, reason unknown.
#include <windows.h>
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

struct Box_00495a30 {
    int pad;                                // +0x0
    Rect_00495a30 r;                        // +0x4 (left, top, right, bottom at +0x10)
};

struct Class_004c6b10 {
    char unknown_0[0x1c];
    void FUN_004c6b10(Rect_00495a30 r);
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
    int var24;
    unsigned int savedC;
    unsigned short fl;
    char filename[260];
    int bw, off27, sy;
    FUN_00495930(filename, dir, name, "bmp");
    bool tmp6;

    int y2;
    Class_004cb7f0 bmp;
    bmp.FUN_004cb7c0();
    tmp6 = bmp.FUN_004cb7f0(filename, w, h);
    if (tmp6) {
    Class_004b8da0* bm;
    off27 = (unsigned int)g_game->field_37e27;
    int bh, off2b, same1 = off27;
    bw = g_game->screenTilesX * 16;
    off27 = same1;
    off2b = g_game->field_37e2b;
    off2b = off2b;
    bh = (g_game->screenTilesY * 16) - 1;

    FUN_004d8e50(0);
    Class_004b8da0* tmp1, * tmp8 = FUN_004b8da0(0x5094d8, w, bh);
    tmp1 = tmp8;
    bm = tmp1;
    if (((int)(bm == 0))) {
        bh /= 2;
        Class_004b8da0* tmp12 = FUN_004b8da0(0x5094d8, w, bh);
        bm = tmp12;
    }
    FUN_0049e6f0();
    if (bm != 0) {
    int scrollX;
    Surface_00495a30 surf;
    scrollX = g_game->scrollX;
    Dst_004b8ae0 pal;
    fl = g_game->viewFlags;
    int savedbit0, tmp0 = g_game->scrollY, tmp10;
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

    if (((int)(((int)(((unsigned int)(h > row)) != 0)) != 0))) {
        int var1c, same0, negbh;
        var24 = bh;
        unsigned int var10 = 0;
        negbh = -bh;
        var1c = -1;
        while (1) {
                    FUN_004b8e50(bm, 0);
                    int col, rows;
                    col = 0;
                    if (col >= w) goto skip2;
                    do {
                                unsigned int tmp17;
                                int right;
                                Box_00495a30 box;
                                box.r.left = col;
                                FUN_0041c4c0(x + col, y + row, 0);
                                FUN_0048bae0();
                                FUN_00468cf0(1, 0);
                                right = (col + bw) - 1;
                                if (right >= surf.width) right = surf.width - 1;
                                box.r.top = 0;
                                    box.r.right = (int)right;
                                tmp17 = bh - 1;
                                box.r.bottom = tmp17;
                                ((Class_004c6b10*)&surf)->FUN_004c6b10(box.r);
                                FUN_004b7f90(&surf, &pal, g_game->scrollX - x - off27,
                                             g_game->scrollY - row - y - off2b);
                        } while (((col += bw), (((int)col) < w)));
skip2:;
                    sy = (unsigned int)var10;
                    int srcY = 0;
                    sy = sy;
                    rows = bh;
                    y2 = row;
                    y2 = y2;
                    if (!((((int)var1c) < -1) != 0)) goto skip0;
                    y2 = row + 1;
                    sy = var1c;
                    rows = bh - 1;
                    srcY = 1;
    skip0:;
                    if (((int)y2) + rows > h) rows = h + sy;
                    if (!(bmp.FUN_004cb940(&surf, (int)w, rows, 0, row, 0, (int)srcY) != 0)) break;
                    if (var24 >= h) goto skip1;
                    row = row - 1;
                        var10++;
                        var1c = var1c + 1;
                        var24 = var24 - 1;
    skip1:;
                    row += bh;
                    var10 += (int)negbh;
                    var1c = var1c + negbh;
                    var24 += (int)bh;
                    if ((int)(row >= h)) break;
                }
    }
    FUN_004d85a0(bm);
    g_game->field_38a51 = (unsigned short)(g_game->field_38a51 ^ ((savedA ^ g_game->field_38a51) & 1));
    tmp10 = (savedB & 1) << 6;
    g_game->field_37f2f = (unsigned short)((((unsigned short)g_game->field_37f2f) & ~0x40) | tmp10);
    g_game->field_37f27 = savedC;
    FUN_0041c4c0(scrollX, (int)tmp0, 0);
    g_game->viewFlags = (unsigned short)(g_game->viewFlags ^ ((savedbit0 ^ g_game->viewFlags) & 1));
    g_game->viewFlags = (unsigned short)((unsigned short)((g_game->viewFlags & ~2) | ((savedbit1 & 1) << 1)));
    FUN_004816a0(1);
    FUN_0048bae0();
    FUN_00468cf0(1, 1);
    }
    } else {
    }
    ((Class_004cb7d0*)&bmp)->FUN_004cb7d0();
}
