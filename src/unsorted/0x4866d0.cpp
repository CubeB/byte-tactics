// Decompiled by Claude Sonnet 5.5, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
// Pass 12 (claude-sonnet-5-5): 65.2 -> 74.4 percent / 1968 bytes (original 1964). Not a MATCH.
// What moved the allocator and the layout (each free-scored):
//  - the leaderboard theirs score as a ternary of two short lvalues (one `mov ax` after the
//    join), rank and best as plain int, `mine` as a short-lvalue ternary, and the declaration
//    order rank, best, mine, i, p: with these cmd lands in ebx (as in the original) and rec in
//    ebx inside the leaderboard (rec/best/mine/counter now match the original registers);
//  - `int credited` (dword in edi) together with the above;
//  - `unsigned char depth` gives the original [esp+0x14] spill and reload before FUN_00489bb0;
//  - bit-field containers (ushort, packed) for unit+0x9b bit 6 and g_game+0x37f06 bit 7 give
//    `mov cl,[m]; shr cl,n; test cl,1`, but the unit bit must be copied to a `bool hid` and
//    tested as `if (!hid)`: a direct `!bit` folds to `test byte,mask`;
//  - `bool bt = mine > theirs; if (bt)` gives the original setg + test;
//  - the flags tail as flags &= 0xefffffff; store; load t; flags &= 0xffffffcf; store.
// Still differs: the loop pointer p is in ecx where the original has eax (the original theirs
// score is a movsx in each branch into ecx; every spelling that gives that makes the compiler
// CSE g_game->mode into an [esp+0x14] spill and grow the frame to 0x6c), the tail keeps cmd in
// a volatile register and reloads it where the original holds cmd in edi for the whole tail,
// the metalCost multiply is scheduled after the vt compare in the original, the sprintf text
// pointer is ecx instead of eax, and the flags tail copies eax from ebp one instruction later.
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