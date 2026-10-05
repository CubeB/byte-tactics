// Decompiled by space-bunny-free. Names are provisional.
// Draws the sliding "scroll bar" marker of a scrolling menu: the scroll
// position DAT_00512fec is animated towards the limit DAT_00512f14, the pixel
// offset DAT_00512ff0 chases it, and the marker is blitted as a trapezoid
// (dst quad) out of the lightbar graphic (src quad) on the given surface.
//
// The offset update is `if (DAT_00512fec < DAT_00512f14) DAT_00512ff0 += 6;`
// (the `<`, so the increase is the fallthrough). The `<` is not cosmetic: the
// opposite spelling still produces the same `cmp eax, ecx` but inverts the
// branch, which puts the load of DAT_00512ff0 inside the else arm and gives
// a memory `add dword ptr [0x512ff0], 6` instead of the original's hoisted
// `mov edx, [0x512ff0]` before the branch with `add edx, 6` after it. A local
// for the offset (int d = DAT_00512ff0) also hoists the load, but then MSVC
// folds the three stores into one shared `mov [0x512ff0], edx` at the join
// and forwards the register into the quad code below, which has to re-read the
// global (87.6%). Reading the global directly and getting the polarity right
// is the whole fix.

struct Point_45ffb0 {
    int x;
    int y;
};

struct Quad_45ffb0 {
    Point_45ffb0 p[4];
};

struct Entry_45ffb0 {
    unsigned short w;                  // +0
    unsigned short h;                  // +2
};

struct Sound_45ffb0 {
    char unknown_0[4];
    short start;                       // +4
    short end;                         // +6
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x51d];
    void* logos32;                     // +0x51d
    char unknown_521[0x142f1 - 0x521];
    unsigned short flags;              // +0x142f1
    char unknown_142f3[0x37e98 - 0x142f3];
    int field_37e98;                   // +0x37e98
};
#pragma pack(pop)

extern Game* g_game;
extern Entry_45ffb0 DAT_00512ef8;
extern int DAT_00512fe4;
extern int DAT_00512fe8;
extern int DAT_00512f10;
extern int DAT_00512f14;
extern int DAT_00512fec;
extern int DAT_00512ff0;

void __stdcall FUN_0047f1a0(char* name, int flag);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrame(int a, void* b, int c, int d);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall DrawFrameQuad(void* surf, void* entry, Quad_45ffb0* dst, Quad_45ffb0* src);

// FUNCTION: 0x45ffb0
void __stdcall FUN_0045ffb0(void* surf)
{
    if (DAT_00512fe4) {
        if (DAT_00512fec < 0x115) {
            int old = DAT_00512fec;
            DAT_00512fec += 0x15;
            if (DAT_00512fec >= 0x115) {
                FUN_0047f1a0("Options", 0);
                DAT_00512fec = 0x115;
            }
            if (DAT_00512fec > DAT_00512f14 && old < DAT_00512f14) {
                void* snd = FindGafEntry(g_game->logos32, "LIGHTBAR");
                Sound_45ffb0* s = (Sound_45ffb0*)GetGafFrame(snd, 2);
                DrawFrame(DAT_00512fe8, s, s->start, s->end);
            }
        }
        if (DAT_00512fec < DAT_00512f14) {
            DAT_00512ff0 += 6;
        } else {
            DAT_00512ff0 -= 6;
            if (DAT_00512ff0 < 0)
                DAT_00512ff0 = 0;
        }
        Quad_45ffb0 src;
        src.p[0].x = 1;
        src.p[0].y = 1;
        src.p[3].x = 1;
        src.p[1].y = 1;
        Quad_45ffb0 dst;
        if (DAT_00512fec > DAT_00512f14) {
            if (DAT_00512fec < 0x115)
                DAT_00512fec++;
            dst.p[1].x = DAT_00512fec;
            dst.p[2].x = DAT_00512fec;
            dst.p[3].x = DAT_00512f14;
            dst.p[0].x = DAT_00512f14;
            dst.p[0].y = DAT_00512f10;
            dst.p[1].y = DAT_00512f10 - DAT_00512ff0;
        } else {
            dst.p[3].x = DAT_00512fec;
            dst.p[0].x = DAT_00512fec;
            dst.p[1].x = 127;
            dst.p[2].x = 127;
            dst.p[0].y = DAT_00512f10 - DAT_00512ff0;
            dst.p[1].y = DAT_00512f10;
        }
        dst.p[2].y = 479;
        dst.p[3].y = 479;
        src.p[1].x = DAT_00512ef8.w - 1;
        src.p[2].x = DAT_00512ef8.w - 1;
        src.p[2].y = DAT_00512ef8.h - 1;
        src.p[3].y = DAT_00512ef8.h - 1;
        DrawFrameQuad(surf, &DAT_00512ef8, &dst, &src);
        FUN_0049fa90((char*)g_game + 0x519);
        g_game->flags |= 2;
        g_game->field_37e98 = 1;
    }
}
