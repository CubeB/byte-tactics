// Decompiled by space-bunny-free, finished by LongCat 2.5 Preview Free,
// GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// mimo-v2.6-pro (retry after deepseek-v4.1-flash): 96.6 percent (312 of 306
// bytes), up from 79.8. NOT A MATCH: the single remaining code difference is
// one instruction, `and edx, 0xff` (the pin, see below), plus the two short
// jmp offsets in arm A that shift because of it. Everything else is now
// byte-identical: prologue, rect, pre-branch, both arms' compare chains, the
// fog arm's whole body (width rematerialised into the dying player-pointer
// register, mask loaded after the imul into the dead row register, cell
// zero-extended through di then copied to edx), the fail blocks (one per arm,
// not tail merged), the phi in edx (`mov edx,1` / `xor edx,edx` in arm A), and
// the whole call sequence (edx=colour, eax=&rect, ecx=surface).
//
// THE TWO LEVERS THAT FIXED IT (both from the sibling 0x473a00's notes):
//   1. `p` and `q`, two spellings of the same `&g_game->players[...]`: the
//      bounds test reads through `p`, the mask index re-reads
//      `q->explored.size.width`. That is what makes MSVC rematerialise the
//      width (`mov edx,[edx+0x80]; imul edx,ecx`) instead of keeping it in a
//      register (the old w/m shape) or folding it into the imul.
//   2. A redundant normalisation statement at the end of the else arm,
//      `visible = (unsigned char)visible;`, pins the whole frame (this -> eax,
//      player pointer -> edx, playerIndex -> esi, g_game -> edi) and keeps the
//      phi in edx. Without any pin the frame rotates and the score is 23.1; a
//      flat 0..1200 extern-int sweep shows the rotation is source-caused, not
//      compiler state. `visible = !!visible` pins too but emits
//      `xor ecx,ecx; test; setne cl` (93.7), `(unsigned char)` emits just
//      `and edx, 0xff` and keeps the phi in edx (96.6). The cast is what a
//      `unsigned char` or `char` field would do; the original has no such and,
//      so its pin is still unknown.
//
// mimo-v2.6-pro (continuation): confirmed the `and` is structurally required to
// pin this->eax, and every spelling that keeps the frame emits it. Sweep of the
// pin at the end of arm B, all 96.6 and all emitting the same 6-byte
// `and edx, 0xff`: `visible & 0xff`, `visible &= 0xff`, `(unsigned char)(short)`,
// a used `unsigned char t` temp, `(int)(unsigned char)`, a union/struct byte
// read at the arm-B tail, and `*(unsigned char*)&visible`. A DEAD byte temp is
// DSE'd (no and) and the frame rotates (23.1), so the byte value must be USED
// (feed the test) to pin, and a used byte value always emits the truncation. The
// natural frame (this->eax) appears without any pin only in the w/m early-return
// shape (cur, 79.8), which merges the fail blocks and folds width, i.e. the
// wrong fog arm. Every no-pin int+if/else structural variant (p/q, p/q/r, extra
// Game*/dead locals, single p) stays rotated at 23.1. Byte-typed `visible`
// (unsigned char/char/bool throughout) emits no and but over-pins to al (this
// moves to edx, 23.1). So the and is the RA perturbation itself and cannot be
// folded away while keeping the frame: this is the register-allocation wall.
//
// STILL DIFFERS: `and edx, 0xff` after arm B's neg/sbb/neg (6 bytes) and the
// two jmp offsets. Tried and worse: `!!` and `(visible != 0)` tails (93.7),
// `bool` round trip (92.3), `if/else` self-correction (88.0), raw AND + tail
// normalisation (91.1, the normalisation becomes a setne), `(bool)` cast
// (93.2), `(short)` cast (93.6), cast at the return instead of the assignment
// (23.1), `& 1` / `|= 0` / `+ 0` / `* 1` / unary `+` tails (23-32), the
// normalisation inside the else or after both arms (23.1), three pointer
// spellings (23.1), bool visible (23.1), caller shapes (`!= 0`, `!!`, temp
// local, early return, 23.1 each), three inline arm helpers (41.5), and
// tools/headers.py (all 128 sets at 93.7 on the !! shape). If the pin could
// be made to emit nothing, the file would match.
//
// SUPERSEDED (79.8 percent era): the mask arm is the ByteMap::Get arm (flags
// & 2 set) and the fog arm is the visibilityMask arm; the decorated name is
// `?FUN_004745e0@Class_004745e0@@QAEXPAXFF@Z` so the surface parameter is
// (void*, short, short); the stride is 0x14b and the flag test is
// `(flags & 2) == 2`. Do not retry the old w/m/four-local spellings or their
// 24 declaration orders (all measured dead); they are in git history.
#include <stddef.h>

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

#pragma pack(push, 1)
struct MapSize_004745e0 {
    unsigned int width;             // +0x80
    unsigned int height;            // +0x84

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_004745e0 {
    unsigned char* data;            // +0x7c
    MapSize_004745e0 size;          // +0x80

    unsigned char Get(int tx, int ty) { return data[size.width * ty + tx]; }
};

struct Map_004745e0 {               // one entry of g_game->players
    char unknown_0[0x7c];
    ByteMap_004745e0 explored;      // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Game_004745e0 {
    char unknown_0[0x1b63];
    Map_004745e0 players[1];         // +0x1b63
    char unknown_1[0x2a43 - 0x1b63 - 0x14b];
    unsigned char playerIndex;       // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;  // +0x14273, one bit per player
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;             // +0x14281, bit 1 (mask 2)
};
#pragma pack(pop)

extern Game_004745e0* g_game;

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);

#pragma pack(push, 1)
struct Pos_004745e0 {
    short x;                        // +0
    char unknown_2[2];
    short height;                   // +4
    char unknown_6[2];
    short y;                        // +8

    int Visible()
    {
        Map_004745e0* p = &g_game->players[g_game->playerIndex];
        Map_004745e0* q = &g_game->players[g_game->playerIndex];
        int visible;
        if ((g_game->flags & 2) == 2) {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            if (p->explored.size.Contains((unsigned int)col, (unsigned int)row) &&
                p->explored.Get(col, row))
                visible = 1;
            else
                visible = 0;
        } else {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            if (!p->explored.size.Contains((unsigned int)col, (unsigned int)row))
                visible = 0;
            else
                visible = (g_game->visibilityMask[q->explored.size.width * row + col] & (1 << g_game->playerIndex)) != 0;
            visible = (unsigned char)visible;
        }
        return visible;
    }
};
#pragma pack(pop)

class Class_004745e0 {              // vector element (see 0x473250.cpp)
public:
    char unknown_0[6];
    Pos_004745e0 pos;               // +0x6
    char unknown_10[0x30 - 0x10];
    int color;                      // +0x30
    char unknown_34[0x44 - 0x34];

    void FUN_004745e0(void* surface, short px, short py);
};

// FUNCTION: 0x4745e0
void Class_004745e0::FUN_004745e0(void* surface, short px, short py)
{
    Rect_004b0510 r;
    short sx = pos.x - px;
    short sy = pos.y - py;
    r.x1 = sx + 0x80;
    r.y1 = sy - (pos.height >> 1) + 0x20;
    r.x2 = r.x1 + 1;
    r.y2 = r.y1 + 1;
    if (pos.Visible())
        FUN_004bf6f0(surface, &r, color);
}
