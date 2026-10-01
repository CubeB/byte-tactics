// Decompiled by Claude Sonnet 5.5, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Retry (deepseek-v4.1-flash, 10 min): still 64.7%, no MATCH. Tried and rejected
// (all scored <= current): int credited declared at top / before unit / at point
// of use (all 63.7%, and cmd lands in ebp either way), inlining the parent
// ternary into unit->f0 (no change), unsigned char depth with int credited
// (58.4%), a local `unsigned char* c = cmd` used throughout (58.6%), hoisting
// the leaderboard loop counter int i to the top (no change).
// Conclusion: with MSVC 5 the int-credited variants always give unit=esi,
// credited=edi, cmd=ebp; the original has cmd=ebx. Something in the source makes
// cmd outrank ebp for the first callee-saved slot; declaration order, int vs
// bool credited, and the transient g_game register do not control it.
// Retry #1766, pass 2 (space-bunny-free, 900s budget): 64.7%, still no MATCH.
// Improvements this pass: the g_game+0x37eee arms are DOUBLE literals (-0.5/-0.7,
// not float) and are spelled as a switch, which is what gives the original
// `sub ecx,0 / je` + `dec ecx / jne` chain instead of two `test` compares (+1.7).
// Still wrong, by original address:
//   0x4866d0  callee-saved allocation. Original: esi=unit, ebx=cmd, edi=credited, and
//             g_game is never cached. Ours: esi=unit, edi=cmd, ebp=g_game (kept live
//             across calls), credited in bl. The tail then re-reads cmd from [esp+0x7c]
//             twice where the original keeps one register copy (0x486c9e, 0x486d24).
//             Every diff below is downstream of this one cause.
//   0x4867da  depth temp: the original stores cl to [esp+0x14], reloads the dword and
//             `and edx,0xff`; we keep it in a register. An `unsigned char depth` local
//             does produce that store/reload shape but scores 57.0% on its own,
//             because the extra spill re-shuffles the already wrong allocation.
//   0x486a98  leaderboard: the original spills rec to [esp+0x80] and rank to [esp+0x10]
//             and holds mine in a register (movsx edi,ax); we spill mine instead.
//             Modelling rank/best as int gets the size right (1956 vs 1964) but scores
//             50.8%, so unsigned char is right and the spill is a downstream effect.
// Tried and did NOT work (free scratch scores, 63.0% baseline):
//   unsigned char depth local                 57.0%
//   bitfield union for unit+0x110             61.5%
//   int rank / int best in the leaderboard   50.8% / 54.1%
//   `mine > theirs` instead of `theirs<mine`  no change
//   signed char for the (x>>6)&1 test        no change
//   int t for the rank-bump loop              no change
//   caching cmd[10] in a local               53.4%
//   swapping the unit/credited decl order    no change
//   keeping `parent` live across the calls   58.9%
//   int credited instead of bool             63.7%
//   second g_game local mid-function (CSE
//     split, brief item 18)                  57.6%
//   second g_game local at the top           55.4%
//   unsigned char depth on top of v5         58.7%
//   unsigned char depth = c?3:0; depth += 3  63.2%
//   bitfield union for unit+0x110 on v5      63.3%
//   same with b4/b5 as two 1-bit fields      63.3%
// Next lever to try: the original spills `rec` to the param slot [esp+0x80] and
// `rank` to [esp+0x10]; ours puts `mine` at [esp+0x10]. Whoever owns the spill
// slot is decided by the same callee-saved question as the top of the function.
//
// Pass 3 (deepseek-v4.1-flash, 2026-09-30, ~900s): 64.7%, still no MATCH.
// The original's rec spill is NOT a source choice: ebx is clobbered inside the
// leaderboard loop by `xor ebx,ebx / cmp edi,ecx / setg bl`, so rec is reloaded
// from [esp+0x80] at 0x486bc9 on every iteration. In the original the only
// three callee-saved candidates are unit=esi, credited=edi, cmd=ebx (rec shares
// ebx because its live range is disjoint from cmd's). Ours always hands cmd the
// 2nd slot (edi) because credited never becomes a candidate: as a `bool` it
// lands in bl, and every wide spelling that promotes it to edi then leaves cmd
// in ebp, not ebx. Confirmed the g_game load register (ebp vs the original's
// edx) is downstream of that same choice.
// Free-scored and rejected (all <= 64.7):
//   int/unsigned/short/long credited        -> committed=edi, cmd=ebp (63.7/63.1)
//   char/unsigned char credited             -> cmd=edi (64.7, unchanged)
//   bool credited initialised at the top    -> cmd=edi (60.4)
//   credited=0 moved before FUN_0044fe40    -> cmd=edi (60.6)
//   self-correction chain on credited x1..20 -> folded away, no change (64.7)
//   cmd as a local copied from the parameter -> cmd=edi/ebp (57.7/58.6)
//   `unsigned char depth` local              -> 57.0 (store/reload shape right, allocation worse)
//   N unused extern ints, N=0..400 step 4    -> flat 64.7 (source shape, not compiler state)
//   tools/headers.py, all 128 sets           -> flat 64.7
//   the real preceding function 0x4864b0 defined above in the same file (the
//     original translation unit)             -> flat 64.7
//   fresh expression trees for the 2nd FUN_00435100 call and the bit-2 g_game
//     test, to break the load CSE            -> flat 64.7
// Remaining known structural gap: at 0x4867da the original keeps the depth in
// an `unsigned char` local spilled to [esp+0x14] then reloads/widens it
// (`mov [esp+0x14],cl; mov edx,[esp+0x14]; and edx,0xff`); ours keeps it in a
// register. Fixing the allocation above is the prerequisite for that to help.
//
// Pass 5 (deepseek-v4.1-flash, retry): 64.7%, still no MATCH. New finding: the
// original does NOT keep cmd in one register for the whole function. It reloads
// cmd from the parameter slot [esp+0x7c] at 0x486c9e (into ecx) and 0x486d24
// (into edi), and reuses ebx as a pure scratch from 0x486a03 on (g_game temp,
// then rec, then a zero). Ours keeps cmd live in edi right through those sites.
// Free-scored and rejected: `unsigned char* unit` (64.7, no change); declaring
// the first parameter as a packed `Order_004866d0*` with real member accesses
// for +1/+3/+7/+9/+0xa (64.7, no change). The cmd register is set by the
// allocator's priority, and neither the parameter's type nor its access shape
// moves it.
// Pass 4 (deepseek-v4.1-flash, retry): 64.7%, still no MATCH. Checked the
// assumption behind the int-credited lever: with `int credited` the allocator
// gives unit=esi, credited=edi, cmd=ebp and leaves ebx completely free until
// the leaderboard (g_game scratch stays in edx). So ebx is NOT taken by g_game
// or by any 4th long-lived variable; MSVC simply prefers ebp over ebx for the
// third long-lived local. Also re-confirmed char* cmd (63.3) and
// int-credited-declared-first (63.7) are worse. Reaching the original's
// cmd=ebx needs something that changes the allocator's register preference,
// not the g_game/ebx occupancy as previously guessed.
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

// FUNCTION: 0x4866d0
void __stdcall FUN_004866d0(unsigned char* cmd, int param)
{
    char* unit;
    bool credited;

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
        int depth = ((cmd[10] & 0xf0) != 0x30 ? 3 : 0) + 3;
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
            && at<unsigned char>(rec, 0x148) != 0) {
            unsigned char rank = at<unsigned char>(rec, 0x148);
            int mine;
            if (at<int>((void*)g_game, 0x37ef6) == 2)
                mine = at<short>(rec, 0x104);
            else
                mine = at<short>(rec, 0xfc);
            int i = 10;
            int* p = (int*)((char*)g_game + 0x1b8a);
            unsigned char best = rank;
            do {
                if ((char)p[0x13] != 0 && (at<unsigned char>((void*)*p, 0x9b) >> 6 & 1) == 0) {
                    int theirs;
                    if (at<int>((void*)g_game, 0x37ef6) == 2)
                        theirs = at<short>(p, 0xdd);
                    else
                        theirs = at<short>(p, 0xd5);
                    if (theirs < mine && at<unsigned char>(p, 0x121) < best)
                        best = at<unsigned char>(p, 0x121);
                }
                p = (int*)((char*)p + 0x14b);
                i--;
            } while (i != 0);
            if (best < rank) {
                i = 10;
                unsigned char* q = (unsigned char*)g_game + 0x1cab;
                do {
                    if (best <= *q && *q < at<unsigned char>(rec, 0x148))
                        *q = *q + 1;
                    q += 0x14b;
                    i--;
                } while (i != 0);
                at<unsigned char>(rec, 0x148) = best;
                if (best == 0) {
                    char text[100];
                    char* fmt = FUN_004c5740(DAT_00508bf0);
                    short kills;
                    if (at<int>((void*)g_game, 0x37ef6) == 2)
                        kills = at<short>(rec, 0x104);
                    else
                        kills = at<short>(rec, 0xfc);
                    sprintf(text, fmt, rec + 0x2b, kills);
                    FUN_00463ca0(text, 2, 0, 10);
                }
            }
        }
        if ((at<unsigned char>((void*)g_game, 0x37f06) >> 7) & 1)
            FUN_004948b0(at<unsigned char>(unit, 0xf4), at<unsigned char>((void*)at<int>(unit, 0x96), 0x146));
    }
    if ((cmd[10] & 0xf0) == 0x50 && at<char*>(unit, 0xf0) != 0) {
        float f = (1.0f - at<float>(unit, 0x104)) * at<float>((void*)at<int>(unit, 0x92), 0x18a);
        void* vt = (void*)at<int>(at<char*>(unit, 0xf0), 0xec);
        if (*(int*)vt == 0 || at<char>(vt, 0x73) != 2) {
            f = f + at<float>(at<char*>(unit, 0xf0), 0xd4);
        } else {
            switch (at<int>((void*)g_game, 0x37eee)) {
            case 0:
                f = at<float>(at<char*>(unit, 0xf0), 0xd4) - f * -0.5;
                break;
            case 1:
                f = at<float>(at<char*>(unit, 0xf0), 0xd4) - f * -0.7;
                break;
            default:
                f = f + at<float>(at<char*>(unit, 0xf0), 0xd4);
            }
        }
        at<float>(at<char*>(unit, 0xf0), 0xd4) = f;
    }
    if (at<char>(cmd, 9) > 0 && at<float>(unit, 0x104) == 0.0f)
        FUN_0049b000(unit, (cmd[10] & 0xf0) == 0x30);
    if ((cmd[10] & 0xf) != 0)
        FUN_00486360(unit, cmd[10] & 0xf, (cmd[10] & 0xf0) != 0x70);
    FUN_00489740(unit);
    if (at<int>(unit, 0x9a) != 0) {
        (*(void(__stdcall**)(int))(*(int*)at<int>(unit, 0x9a) + 0x50))(1);
        at<int>(unit, 0x9a) = 0;
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
    unsigned int flags = at<unsigned int>(unit, 0x110);
    at<short>(unit, 0xa6) = 0;
    at<unsigned int>(unit, 0x110) = flags & 0xefffffff;
    int t = at<int>((void*)g_game, 0x1439b);
    at<unsigned int>(unit, 0x110) = flags & 0xefffffcf;
    at<int>(unit, 0x92) = t;
    at<short>((void*)at<int>(unit, 0x96), 0x144)--;
    if (at<short>((void*)at<int>(unit, 0x96), 0x144) == 0) {
        if (((Class_00435100*)at<void*>((void*)g_game, 0x391e9))->FUN_00435100() == 3)
            FUN_00450380(at<int>((void*)at<int>(unit, 0x96), 4));
        if (((Class_00435100*)at<void*>((void*)g_game, 0x391e9))->FUN_00435100() == 2)
            FUN_0047bd70((void*)at<int>(unit, 0x96));
    }
}
