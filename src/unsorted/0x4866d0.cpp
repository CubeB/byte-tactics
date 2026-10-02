// Decompiled by Claude Sonnet 5.5, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// Pass 13 (Space Bunny Free): 74.4 -> 87.0 percent / 1968 bytes (original 1964). Not a MATCH.
// What moved it (each change free-scored on top of the previous one):
//  - `if (((GameBits*)g_game)->b7 != 0)` rather than the bare bitfield: the `!= 0` keeps the
//    original `mov al,[g+0x37f06]; shr al,7; test al,1` (a bare `if (bit)` folds the whole
//    test to `test byte ptr [m], 0x80`). Worth 2.7 points on its own.
//  - re-read the flags field off the unit for the second mask (twice, no `flags` local)
//    instead of keeping it in a local: +0.7.
//  - in the leaderboard, compute `mine` BEFORE `rank`/`best`, not after. The single biggest
//    lever (+4.1): it moves that block's whole prologue, and only this order puts the mode
//    test and both `mov ax` kill loads where the original has them. Moving them back, or
//    inserting `i` between, costs 10 points.
//  - walk the leaderboard with a `char* p` advanced by `p += 0x14b`, reading fields through
//    `at<char>(p,0x4c)` / `((UnitBits*)(void*)at<int>(p,0))->b6` / `at<short>(p,...)` instead
//    of an `int* p` with `p[0x13]` and `*p`. With an int* MSVC folds the base into ecx
//    (`add ecx, 0x1b8a`); with a char* it keeps the base in eax (`add eax, 0x1b8a`) and the
//    byte temps in cl, exactly as the original. Same score alone, but the loop then matches
//    register-for-register, which the later hunks depend on.
//  - bind the unit's +0x9a field to a local `int* a9a` before testing and nulling it. The
//    virtual call and the `= 0` store then go through that one register, which is what puts
//    the constant 0 in ebx (as in the original) instead of edi: +1.2. Doing the same to
//    +0x9e or to the +0 head pointer regresses, so only the first one.
// Still differs, largest first:
//  - the leaderboard `theirs`: the original does a `movsx ecx, word ptr [eax+K]` in EACH arm
//    of the ternary; ours loads 16-bit into cx and sign-extends once after the join. Every
//    spelling that forces the per-branch movsx (int casts on the arms, if/else, a mode local)
//    costs 15 points and 28 bytes, so the arms stay short-typed. This is the main blocker.
//  - the tail after the leaderboard: the original holds `cmd` in edi for the whole tail; ours
//    reloads it into edx/eax. Copying cmd into a local, hoisting cmd[9]/cmd[10] into locals
//    (char, int or unsigned char), and reading them in both orders all score lower.
//  - the x87 block: the original loads esi+0x92 and does the fmul BEFORE the `vt` compare;
//    ours loads vt first and does the fmul after. Swapping the two source statements, hoisting
//    the 0x92 pointer or the parent pointer, and adding a (float) cast all compile the same.
//  - the sprintf call: the original materialises the format in eax and then the buffer in
//    eax (after the push); ours computes the buffer into ecx before the push.
//  - `unsigned char depth` still needs a 32-bit `add ecx, 3`; every int-typed spelling of
//    `? 3 : 0 + 3` collapses the add to 8 bits and loses the [esp+0x14] spill.
// Tried and rejected, all scoring below the above: a Game/Player/PSub struct for the 0x14b
// entries, indexing players[i] with a for loop, `extern char* g_game`, a Game*-typed g_game,
// all 128 header sets (tools/headers.py: none better), `short` for mine/theirs, a shared
// `zero` local, the do/while rotation of the target loop, and per-field pointer locals for
// +0x9e / the +0 head. tools/permute.py run from 74.4, 77.1 and 85.7 peaked at 79.8, 82.0
// and 85.7 percent, none above this file, and its best diffs are full of `tmp0`/`do{}while(0)`
// artifacts, so nothing from it was taken.
// Pass 14 (DeepSeek V4.1 Flash): no score change, still 87.0 (1968 bytes) / 89.2 ignoring
// the 10 moved jump targets. tools/stackcmp.py reports no moved local. Two 3-minute
// tools/permute.py runs (current file and the 1964-byte bool-comparison variant below) found
// nothing above 87.0. Everything tried this pass scored lower and was reverted:
//  - rank-first in the leaderboard: matches the original prologue byte for byte
//    (`and eax,0xff` / store / `mov edx,eax` / mode test / both `mov ax`), but the loop base
//    then lands in ecx (`add ecx,0x1b8a`) instead of eax and the loop body loses the
//    register-for-register match: 76.4. Adding a `base` local did not stop the coalescing:
//    76.4. rank/mine/best orders B/E/F and a p-first order all scored 74 to 76.
//  - per-arm sign-extend for `theirs`: `if (mode==2) bt = mine > at<short>(...dd) : ...` and
//    `bool bt = cond ? cmp : cmp` make the function exactly 1964 bytes and the same 89.2
//    ignoring jump targets, but shift every internal target (33 moved) so the raw score is
//    81.9. The separate `int theirs; if/else` form spills `rec` to [esp+0x80]: 77.1.
//    `(int)` casts on the ternary arms: 72.0.
//  - the a9a local and `*a9a = 0` are optimal: writing the field back or dropping the local
//    both give 85.8 (the write must be `mov [edi],ebx`).
//  - splitting the x87 multiply into two statements, hoisting the 0x92 or parent pointer, and
//    a `switch` value local all compile to the current code (no change).
//  - `short theirs`, an inline comparison ternary and type changes to `theirs` compile
//    identically to the current line (87.0).
extern void* g_game;
extern char DAT_00508be8[];
extern char DAT_00508bf0[];

template <class T>
inline T& at(void* p, int off)
{
    return *(T*)((char*)p + off);
}

class Class_004904c0 {
public:
    void FUN_004904c0(void* unit);
};

class Class_004b0a70 {
public:
    int FUN_004b0a70(char*, void*, int, int, int, int, int, int);
};

class Class_00435100 {
public:
    int FUN_00435100();
};

void __stdcall FUN_00482910(void* pos, int a, int b, int c);
unsigned char __stdcall FUN_0044fe40(int id);
void __stdcall FUN_00439eb0(void* unit, int flag);
void __stdcall FUN_0047f8c0(void* unit);
void __stdcall FUN_00480250(void* unit, int flag);
void __stdcall FUN_0049c880(void* unit);
void __stdcall FUN_0048aac0(void* unit, int a, char b, int c);
void __stdcall FUN_00489bb0(void* a, void* b, int c, int d, int e);
void __stdcall FUN_0047cbd0(void* unit);
void __stdcall FUN_00482090(void* unit);
int __cdecl FUN_004f8a70(unsigned char* a, unsigned char* b);
void __stdcall FUN_00494ff0(int flag);
char* __stdcall FUN_004c5740(char* text);
int __cdecl sprintf(char* buf, const char* fmt, ...);
void __stdcall FUN_00463ca0(char* text, int a, int b, int c);
void __stdcall FUN_004948b0(int a, int b);
void __stdcall FUN_0049b000(void* unit, int flag);
void __stdcall FUN_00486360(void* unit, int a, int b);
void __stdcall FUN_00489740(void* unit);
void __stdcall FUN_0045aaa0(void* state);
class Class_0043dd10 {
public:
    void FUN_0043dd10();
};
void __cdecl operator delete(void* p);
void __stdcall FUN_00450380(int id);
void __stdcall FUN_0047bd70(void* player);

#pragma pack(push, 1)
struct UnitBits {
    char pad[0x9b];
    unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1, b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;
};

struct GameBits {
    char pad[0x37f06];
    unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1, b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;
};
#pragma pack(pop)

// FUNCTION: 0x4866d0
void __stdcall FUN_004866d0(unsigned char* cmd, int param)
{
    char* unit;
    int credited;

    if (at<unsigned short>(cmd, 1) == 0)
        unit = 0;
    else
        unit = (char*)(at<int>((void*)g_game, 0x14357) + at<unsigned short>(cmd, 1) * 0x118);
    if ((at<unsigned int>(unit, 0x110) & 0x10000000) == 0)
        return;

    if (at<char>((void*)at<int>(unit, 0x96), 0x146) == at<char>((void*)g_game, 0x2a43)) {
        FUN_00482910(unit + 0x6a, at<short>((void*)at<int>(unit, 0x92), 0x202),
                     at<short>((void*)at<int>(unit, 0x92), 0x170), 0x3c);
    }
    char* parent;
    if (at<unsigned short>(cmd, 7) == 0)
        parent = 0;
    else
        parent = (char*)(at<int>((void*)g_game, 0x14357) + at<unsigned short>(cmd, 7) * 0x118);
    at<char*>(unit, 0xf0) = parent;
    at<unsigned char>(unit, 0xf4) = FUN_0044fe40(at<int>(cmd, 3));
    ((Class_004904c0*)at<void*>((void*)g_game, 0x391ed))->FUN_004904c0(unit);
    FUN_00439eb0(unit, 1);
    FUN_0047f8c0(unit);
    FUN_00480250(unit, -1);
    FUN_0049c880(unit);
    if (at<int>(unit, 0x86) != 0)
        FUN_0048aac0(unit, 0, -1, 1);
    while (at<int>(unit, 0x8a) != 0) {
        unsigned char depth = (cmd[10] & 0xf0) != 0x30 ? 6 : 3;
        FUN_00489bb0(at<char*>(unit, 0xf0), (void*)at<int>(unit, 0x8a), 30000, depth, 0);
        FUN_0048aac0((void*)at<int>(unit, 0x8a), 0, -1, 1);
    }
    FUN_0047cbd0(unit);
    if ((at<unsigned char>((void*)g_game, 0x14281) & 2) == 2)
        FUN_00482090(unit);
    if (param == 0 && at<char>(cmd, 9) > 0) {
        ((Class_004b0a70*)at<void*>(unit, 0x9a))->FUN_004b0a70(DAT_00508be8, 0, 1, 1, at<char>(cmd, 9), 0, 0, 0);
    }
    credited = 0;
    switch (cmd[10] >> 4) {
    case 5:
        if (at<unsigned char>(unit, 0xf4) == 10 || at<char>(unit, 0xf4) == at<char>(unit, 0xff))
            break;
    case 1:
    case 6:
        if (at<int>(unit, 0x96) != 0) {
            at<short>((void*)at<int>(unit, 0x96), 0xfe)++;
            if (at<unsigned char>(unit, 0xf4) != 10 && at<float>(unit, 0x104) == 0.0f
                && at<unsigned char>(unit, 0xff) != at<unsigned char>(unit, 0xf4)) {
                at<short>((char*)g_game + at<unsigned char>(unit, 0xf4) * 0x14b, 0x1c5f)++;
            }
            param = FUN_004f8a70((unsigned char*)g_game + 0x37f5f
                                     + at<unsigned char>((void*)at<int>((void*)at<int>(unit, 0x96), 0x27), 0x95) * 0x232,
                                 (unsigned char*)at<int>(unit, 0x92) + 0x20) == 0;
            if (param) {
                if (at<unsigned char>(unit, 0xf4) != 10)
                    at<short>((char*)g_game + at<unsigned char>(unit, 0xf4) * 0x14b, 0x1c67)++;
                at<short>((void*)at<int>(unit, 0x96), 0x106)++;
            }
            if (at<char*>(unit, 0xf0) != 0 && at<float>(unit, 0x104) == 0.0f
                && at<char>(unit, 0xff) != at<char>(unit, 0xf4)) {
                at<short>(at<char*>(unit, 0xf0), 0xb8)++;
            }
            if (at<char>(unit, 0xf4) == at<char>((void*)g_game, 0x2a42))
                FUN_00494ff0(5);
            credited = 1;
        }
        break;
    case 3: {
        int owner = at<int>(unit, 0x96);
        if (owner != 0
            && at<char>((void*)g_game,
                        at<unsigned char>((void*)owner, 0x146) + 0x1c8c + at<unsigned char>((void*)g_game, 0x2a42) * 0x14b) == 0) {
            at<short>((void*)owner, 0xfe)++;
            param = FUN_004f8a70((unsigned char*)g_game + 0x37f5f
                                     + at<unsigned char>((void*)at<int>((void*)at<int>(unit, 0x96), 0x27), 0x95) * 0x232,
                                 (unsigned char*)at<int>(unit, 0x92) + 0x20) == 0;
            if (param) {
                at<short>((void*)at<int>(unit, 0x96), 0x106)++;
            }
            credited = 1;
        }
        break;
    }
    }
    if (credited && at<unsigned char>(unit, 0xf4) != 10) {
        char* rec = (char*)g_game + at<unsigned char>(unit, 0xf4) * 0x14b + 0x1b63;
        if (at<int>(rec, 0) != 0
            && (at<char>(rec, 0x73) == 1 || at<char>(rec, 0x73) == 2 || at<char>(rec, 0x73) == 3)
            && at<char>(rec, 0x146) != 10
            && (((Class_00435100*)at<void*>((void*)g_game, 0x391e9))->FUN_00435100() == 3
                || ((Class_00435100*)at<void*>((void*)g_game, 0x391e9))->FUN_00435100() == 2)
            && at<unsigned char>(rec, 0x148) > 0) {
            int mine = at<int>((void*)g_game, 0x37ef6) == 2 ? at<short>(rec, 0x104) : at<short>(rec, 0xfc);
            int rank = at<unsigned char>(rec, 0x148);
            int best = rank;
            int i = 10;
            char* p = (char*)g_game + 0x1b8a;
            do {
                if (at<char>(p, 0x4c) != 0) {
                    bool hid = ((UnitBits*)(void*)at<int>(p, 0))->b6;
                    if (!hid) {
                        int theirs = at<int>((void*)g_game, 0x37ef6) == 2 ? at<short>((void*)p, 0xdd) : at<short>((void*)p, 0xd5);
                        bool bt = mine > theirs;
                        if (bt) {
                            if (at<unsigned char>((void*)p, 0x121) < best)
                                best = at<unsigned char>((void*)p, 0x121);
                        }
                    }
                }
                p += 0x14b;
                i--;
            } while (i != 0);
            if (best < rank) {
                i = 10;
                unsigned char* q = (unsigned char*)g_game + 0x1cab;
                do {
                    if (*q >= best && *q < at<unsigned char>(rec, 0x148))
                        *q = *q + 1;
                    q += 0x14b;
                    i--;
                } while (i != 0);
                at<unsigned char>(rec, 0x148) = best;
                if (best == 0) {
                    char text[100];
                    sprintf(text, FUN_004c5740(DAT_00508bf0), rec + 0x2b,
                            at<int>((void*)g_game, 0x37ef6) == 2 ? at<short>(rec, 0x104) : at<short>(rec, 0xfc));
                    FUN_00463ca0(text, 2, 0, 10);
                }
            }
        }
        if (((GameBits*)g_game)->b7)
            FUN_004948b0(at<unsigned char>(unit, 0xf4), at<unsigned char>((void*)at<int>(unit, 0x96), 0x146));
    }
    if ((cmd[10] & 0xf0) == 0x50 && at<char*>(unit, 0xf0) != 0) {
        char* par = at<char*>(unit, 0xf0);
        float f = (1.0f - at<float>(unit, 0x104)) * at<float>((void*)at<int>(unit, 0x92), 0x18a);
        void* vt = (void*)at<int>(par, 0xec);
        if (*(int*)vt == 0 || at<char>(vt, 0x73) != 2) {
            f = f + at<float>(par, 0xd4);
        } else {
            switch (at<int>((void*)g_game, 0x37eee)) {
            case 0:
                f = at<float>(par, 0xd4) - f * -0.5;
                break;
            case 1:
                f = at<float>(par, 0xd4) - f * -0.7;
                break;
            default:
                f = f + at<float>(par, 0xd4);
            }
        }
        at<float>(par, 0xd4) = f;
    }
    if (at<char>(cmd, 9) > 0 && at<float>(unit, 0x104) == 0.0f)
        FUN_0049b000(unit, (cmd[10] & 0xf0) == 0x30);
    if ((cmd[10] & 0xf) != 0)
        FUN_00486360(unit, cmd[10] & 0xf, (cmd[10] & 0xf0) != 0x70);
    FUN_00489740(unit);
    int* a9a = (int*)at<int>(unit, 0x9a);
    if (a9a != 0) {
        (*(void(__stdcall**)(int))(*(int*)a9a + 0x50))(1);
        *a9a = 0;
    }
    if (at<int>(unit, 0x9e) != 0) {
        FUN_0045aaa0((void*)at<int>(unit, 0x9e));
        at<int>(unit, 0x9e) = 0;
    }
    if (*(int*)unit != 0) {
        ((Class_0043dd10*)*(int*)unit)->FUN_0043dd10();
        operator delete((void*)*(int*)unit);
        *(int*)unit = 0;
    }
    at<short>(unit, 0xa6) = 0;
    at<unsigned int>(unit, 0x110) = at<unsigned int>(unit, 0x110) & 0xefffffff;
    int t = at<int>((void*)g_game, 0x1439b);
    at<unsigned int>(unit, 0x110) = at<unsigned int>(unit, 0x110) & 0xffffffcf;
    at<int>(unit, 0x92) = t;
    at<short>((void*)at<int>(unit, 0x96), 0x144)--;
    if (at<short>((void*)at<int>(unit, 0x96), 0x144) == 0) {
        if (((Class_00435100*)at<void*>((void*)g_game, 0x391e9))->FUN_00435100() == 3)
            FUN_00450380(at<int>((void*)at<int>(unit, 0x96), 4));
        if (((Class_00435100*)at<void*>((void*)g_game, 0x391e9))->FUN_00435100() == 2)
            FUN_0047bd70((void*)at<int>(unit, 0x96));
    }
}