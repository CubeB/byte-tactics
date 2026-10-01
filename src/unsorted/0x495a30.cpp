// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// 2026-10-01 pass 4 (deepseek-v4.1-flash): new best 57.2 (979 vs 1105). The two tail viewFlags lanes used
// to re-derive their bits inline from the 16-bit `fl`; naming them right before the lanes as
// `unsigned char v0 = fl & 1;` and `unsigned char v1 = (fl >> 1) & 1;` lifts 55.0 -> 57.2 (962 -> 979
// bytes): lane 2 now loads the saved dword and emits `and eax,1; shl eax,1; and edx,0xfffd; or edx,eax`
// with `lea edx,[esi+esi]` for the shift, like the original. Rejected: the maskless spelling
// (`v1 = (unsigned char)(fl >> 1)`) scores 57.3 but widens the OR into bits 2..7 of viewFlags, which the
// original's `and eax,1` does not do; `int` versions of v0/v1 score 55.2/55.3.
// Structural gap unchanged: the Class_004cb940 loop-tail call is still missing (scratch v1/v2 carry it
// but score 41.3/40.5 because their new locals grow the frame 0x1b0 -> 0x1bc).
// 2026-10-01 pass (deepseek-v4.1-flash): no change, re-confirmed 55.0 (962 vs 1105 bytes).
// 2026-10-01 pass 3 (deepseek-v4.1-flash): corrected the frame map (see below); the
// best file is still this one at 55.0. build/scratch/0x495a30/v1.cpp and v2.cpp have
// the missing Class_004cb940 block and land at 1106 bytes (1 byte off the original)
// but score 41.3/40.5 because the two or three new int locals spill and grow the
// frame 0x1b0 -> 0x1bc, shifting every pre-loop slot by +0xc.
// Frame facts re-derived from the disassembly (S = esp after sub 0x1b0 plus the four
// register pushes, so [esp+N] in the listing IS S+N once the temporary pushes are
// undone): filename char[260] at S+0xbc (the frame really extends to S+0x1c0 =
// entry esp, since the pushes sit BELOW the 0x1b0 block), surf at S+0x8c, pal at
// S+0x64 (flag8 at S+0x6c), bmp at S+0x54, scalars at S+0x10 var10, S+0x14 bm,
// S+0x18 bh (bh is REASSIGNED, so the halved height reuses the same slot), S+0x1c
// var1c, S+0x20 bw, S+0x24 var24, S+0x28 saved bit6 of field_37f2f, S+0x2c scrollY,
// S+0x30 off2b, S+0x34 scrollX, S+0x38 -bh, S+0x3c saved viewFlags bit1, S+0x40
// saved viewFlags bit0, S+0x44 off27, S+0x48 y+row, S+0x4c saved field_37f27,
// S+0x50 saved bit0 of field_38a51, S+0x88 rect bottom (bh-1). The ctor, the
// FUN_004cb7f0, the FUN_004cb940 and the dtor are ALL called with this = S+0x54
// (the earlier "two subobjects four bytes apart" reading was a push-accounting
// mistake: 0x495a77 and 0x495d54 both subtract their pending pushes), so there is
// one 0x10-byte object per call site.
// The loop tail at 0x495d0d is: esi = var10, edx = 0, if (var1c >= -1) {ecx = row,
// eax = bh} else {ecx = row+1, esi = var1c, eax = bh-1, edx = 1}; ecx += eax;
// if (ecx > h) eax = h + esi; then FUN_004cb940(&surf, w, eax, 0, row, 0, edx) with
// this = S+0x54, and `je S+0xda0` breaks straight to the shared loop-exit reload of
// savedB/saved viewFlags. rows/srcY/sy are REGISTER-ONLY in the original (eax/edx/
// esi), which is why naming them as locals is what breaks the slot map.
// Frame-size lever found in this pass: Class_004cb7f0 and Dst_004b8ae0 are 0x14 in
// this file but 0x10 in the original (bmp S+0x54, pal S+0x64), so this version gets
// those two fields for free. Shrinking both (scratch v3.cpp) drops the frame
// 0x1b0 -> 0x1a8 and moves pal to S+0x5c, i.e. the file is 8 bytes SHORT of the
// original once the structs are honest. Scratch v5/v6/v7 add the Class_004cb940
// block back on top of the shrunk structs and land at 1112/1106/1112 bytes, frame
// 0x1b4 (4 too big) and about 41 percent: the two or three tail int locals always
// take fresh dwords instead of reusing the holes, so the next pass should look for
// which original local occupies the S+0x74..S+0x87 (0x14) hole and the S+0x88
// cached bh-1 rect bottom rather than adding more tail variables. The 0x14 hole is
// exactly a 0x10 by-value Rect plus one dword, which matches the FUN_004c6b10
// argument (the `sub esp,0x10` at 0x495c8c is that argument area, so the Rect
// itself has no frame slot in this build).
// Still differs: the whole pre-loop slot map, the viewFlags save/restore shape and the
// missing Class_004cb940 loop-tail call (see notes below).
// Writes a large screenshot ("BIGSHOT", caller 0x417600) by rendering map
// tiles and copying them into one large BMP surface.
//
// Best so far: 55.0 percent after four checks in this pass. Adding the
// unknown trailing dword to Class_004cb7f0 grows the frame from 0x1ac to the
// original 0x1b0 and improves the score. This field is inferred from frame
// size, not confirmed from another use of the class.
// Still differs: pre-loop values do not use the original stack slots/registers;
// the viewFlags save/restore differs; the inner-loop and tail register choices
// also differ. Callee order, arguments and main loop control flow are close.
//
// Later pass (deepseek-v4.1-flash): 55.0 percent re-confirmed by check.py
// (original 1105 bytes, ours 962, so the tail and save/restore code is far
// smaller than the original). The original tail restores viewFlags bit 1 with
// a single and/or store (and edx,0xfffd / or edx,eax / mov [ecx+0x14281],dx)
// while this version does two separate read-modify-write stores plus the odd
// xor-idiom for bit 0. Also still open: the pre-loop g_game loads should land
// in the original slots ([esp+0x24] bw, [esp+0x1c] bh, [esp+0x48] off27,
// [esp+0x34] off2b, [esp+0x30] scrollY, [esp+0x38] scrollX, [esp+0x40] and
// [esp+0x44] the two saved viewFlags bits). Ideas tried earlier and kept in
// history: trailing dword added to Class_004cb7f0 for the 0x1b0 frame (kept),
// struct layout tweaks for Game_00495a30. No inline asm or pragmas used.
// Stopped on the SHARED.md watchdog stop signal before trying new variants.
//
// Later pass 2 (deepseek-v4.1-flash). Big finding: this file is MISSING the
// Class_004cb940 copy-to-BMP call that sits at the bottom of the outer do-loop
// (0x495d3d..0x495d5d), which is most of the 143-byte size gap. The original
// computes the clipped row count first, calls the method, and breaks out of the
// loop when it returns false: bmp.FUN_004cb940(&surf, w, rows, 0, row, 0, srcY)
// with rows/srcY coming from `int rows = bh; int sy = var10; int srcY = 0;
// if (var1c < -1) { rows = bh - 1; sy = var1c; srcY = 1; }` and then
// `if (row + bh > h) rows = h + sy;`. Note this-relative detail: the
// Class_004cb7c0 ctor and the Class_004cb7d0 destructor are called on bmp+0
// ([esp+0x54] at 0x495a56/0x495e6b) while Class_004cb7f0 and Class_004cb940
// are called on bmp+4 ([esp+0x60] at 0x495a77, [esp+0x70] at 0x495d54), so the
// original really did have two subobjects four bytes apart; a plain
// single-base declaration cannot reproduce those two leas.
// Adding the missing block (both before and after the var24 block) was tried:
// it brings the size to 1084/1095 bytes but drops the score to 35.3/35.8
// because the extra live values rotate the whole register/slot allocation,
// including the prologue (bm moved from [esp+0x14] to [esp+0x18]). Both v
// variants are kept in build/scratch/0x495a30/ (v55_base.cpp is this file,
// v_with_call.cpp has the block). Swapping the off27/off2b declaration order
// also loses (54.6). So the loop tail has to be added together with the frame
// layout, not on its own.
// Derived the original local slot map in frame-relative terms (base B = esp
// after sub 0x1b0 and the four register pushes): B+0x10 var10, B+0x14 bm,
// B+0x18 bh, B+0x1c var1c (starts -1), B+0x20 bw, B+0x24 var24 (starts bh),
// B+0x28 savedB, B+0x2c scrollY, B+0x30 off2b, B+0x34 scrollX, B+0x38 -bh,
// B+0x3c saved bit1, B+0x40 saved bit0, B+0x44 off27, B+0x48 y+row, B+0x4c
// savedC, B+0x50 savedA, pal at B+0x64 (flag8 at +8), rect bottom bh-1 at
// B+0x88, surf at B+0x8c, filename at B+0xac (260 bytes). This version matches
// only B+0x14 (bm) and B+0x18 (bh); the rest are shifted. The original's tail
// restores bit0 of field_38a51 with the xor idiom (xor al,cl / and eax,1 /
// xor eax,ecx) and bit1 of viewFlags with mask-and-or, and restores viewFlags
// bit0 with `xor bl,al / and ebx,1 / xor ebx,eax`, i.e. both saved-bit locals
// stay live in registers across FUN_004d85a0.
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
    int unused_10;
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
    unsigned char v0 = fl & 1;
    unsigned char v1 = (fl >> 1) & 1;
    g_game->viewFlags =
        (unsigned short)(((g_game->viewFlags ^ v0) & 1) ^ g_game->viewFlags);
    g_game->viewFlags =
        (unsigned short)((g_game->viewFlags & ~2) | (v1 << 1));
    FUN_004816a0(1);
    FUN_0048bae0();
    FUN_00468cf0(1, 1);
    }
    }
    ((Class_004cb7d0*)&bmp)->FUN_004cb7d0();
}
