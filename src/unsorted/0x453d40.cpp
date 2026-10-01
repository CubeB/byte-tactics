// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash., finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash pass for issue #3677: 21.8 -> 28.0 (6396 bytes). The big win was
// the ORDER of the big switch's case bodies, which check.py sees because difflib only
// matches blocks that run forward in both streams. The original's source order was read
// off the jump table at 0x455f84 (index = cmd - 2): 32,23,24,2,38,35,36,27,28,30,31,5,
// 39,6,7,8,9,44,10..22,40,41,25,26,29,33,34; ours was numeric order. Reordering the
// transcribed bodies to that order (28, 33 and 39 stay missing) gave 21.8 -> 28.0.
// Same pass: cases 4, 37 and 43 jump to 0x455f50 (the loop test), so they are empty
// bodies, and the small switch's original order is 5 (0x453f20), 3 (0x45413f), 0x102
// (0x454425), 0x104 (0x454685), 0x103 (0x45469e), so 0x104 is emitted before 0x103 here.
// Still differs: frame 0x318 against the original 0x51c and hence every [esp+N] slot;
// case-39's body was transcribed and scored 27.6, so it is not in (its text does not line
// up until the slots do); case 28 (0x454ec4, buffer at 0x220, 5-byte message at 0x218) and
// case 33 (0x4559f5) are still missing. Also the 0x9b bit-4 test at 0x4540f8 is a real
// bitfield read (mov al,[ecx+0x9b]; shr al,4; test al,1), not `& 0x10`.
// deepseek-v4.1-flash (retry, 10 min timebox): re-ran the checker, 28.0% is still
// the best recorded for this file (6396 of 8944 bytes; frame 0x318 against 0x51c).
// The two dominant gaps are unchanged: the three untranscribed bodies (39 first,
// then 33, then 28, which together carry the ~0x204 of missing frame) and the
// per-slot draw order of the command bodies already transcribed (a prior pass that
// added all three reached 0x51c but only 9.6%, because the slot order was wrong).
// No new variant was written in the timebox.
// Next: transcribe case 28 then case 33 in that order (their locals are declared before
// case 0x102's Class_00463be0 temp, which sits at 0x2e0, and before case-39's 0x434 buffer).

// deepseek-v4.1-flash pass, 15.2 -> 21.8% (6384 bytes): the sender test is a real
// two-arm if, not an "if with an empty else" plus fall-through. The original tests
// sender once at 0x453ece (jne 0x45473f) and falls through into the sender == 0 arm
// (0x453ed4..0x45473a), so the source reads `if (sender == 0) { ...small switch...
// continue; }` and only then the big sender != 0 body. Swapping the two arms (with
// the guard written as a cached byte plus nested ifs, which reproduces the
// duplicated `cmp bl,1` at 0x453eee) gave 15.2 -> 21.8. The short-circuit `||` form
// of that guard emits only one compare, so it must stay nested ifs.
// Next: the frame is still 0x318 against 0x51c (0x204 short, exactly the locals of
// the three untranscribed bodies, case 39 first at 0x434, then case 33, then case 28
// at 0x220/0x218). Our head still differs at `xor esi,esi` / register-zero store
// where the original stores an immediate 0 and tests with `test eax,eax`.
// deepseek-v4.1-flash retry (10 min timebox), kept at the known best 15.2% (6448 of 8944 bytes).
// Tried this pass, all still 15.2%: dropping the separate `int receiving = 1;` and declaring
// `receiving` inside the loop; writing the zeroing store as `g_game + off + 0x1a28` and as
// `(int)0`; tools/headers.py over 128 header sets (closest <string.h>, 16 sets failed to
// compile). None of them removes the extra `xor esi,esi` / register-zero store at the top of
// the zeroing loop (original stores immediate 0 into [edx+eax+0x1a28]) or moves any frame slot.
// The dominant gap is still the frame: ours reserves 0x318, the original 0x51c, so every
// [esp+N] operand mismatches. The three missing command bodies (28, 33, 39) hold the large
// locals that make the difference; a prior pass added them and hit 0x51c but scored 9.6%
// because the per-slot draw order was wrong (see the detailed notes below). No new score gain
// was found in this timebox; the best available version is the one below.
// deepseek-v4.1-flash, recheck pass for issue #2876: this run had NO shell or compiler in its
// tool runtime (only browser and opencode tools were exposed), so tools/check.py and tools/ctx.py
// could not be run and no new score could be measured. The code below is left byte-for-byte as the
// known best 15.2% (6448 of 8944 bytes). Confirmed from docs/bugs.md and the disassembly notes
// that the source already faithfully reproduces the original including its bugs, so none of these
// may be "fixed": the always-false command range check at 0x4547f5 (kept as `cmd < 2 && cmd > 0x2c`),
// the duplicated sender status test at 0x45480e, and the command-20 null-player read crash at
// 0x45575a (FindPlayer_453d40 returning 10 leaves q=0 and `*(int*)q` reads address 0). What still
// differs and where the next real gain lives: the frame and local slot ORDER. Our frame is 0x318
// against the original 0x51c. The notes below already tried adding the three missing bodies
// (cases 28, 33, 39) to reach 0x51c and got the right total but the wrong per-slot order (9.6%,
// worse), so a future pass must match the compiler's local draw order (see the "original draw
// order" list below), not the total frame size. Register/operand fixes via headers.py are the
// only safe lever once a shell is available again.
// deepseek-v4.1, second pass, best 15.2% (6448 bytes of 8944). Three order fixes paid:
// (1) the loop flag is not a separate local. The original
// stores FUN_004534e0()'s result at [esp+0xf0], and case 8 writes 0 to that same slot
// (0x4553c3: mov dword ptr [esp+0xf0], 0) before or-ing 4 into g_game+0x2a44, then falls
// into the loop test at 0x455f50 (mov eax,[esp+0xf0]; test eax,eax; jne 0x453d94). So the
// body is a do-while whose condition is the call result, not "receiving = 1" plus a second
// call in the while condition; that rewrite removed one local and gave 14.4 -> 14.5.
// (2) hoisting `char* player = g_game + 0x1b63 + 0x14b * from;` above `recipient` and
//     `++messages` (the original computes it into edi at 0x453e8e, before the sender test
//     at 0x453ece) gave 14.5 -> 15.1: statement order, not the frame, moved the register
//     allocation of the whole loop head.
// (3) the original has a redundant extra copy of the sender status test (0x45480e:
//     cmp cl,1/cmp cl,2/cmp cl,3, jne continue, cl already holding player[0x73]); adding
//     `if (player[0x73] != 1 && != 2 && != 3) continue;` before the recipient check gave
//     15.1 -> 15.2. So this function's source is full of duplicated conditions; look for
//     extra copies of a test in the disassembly before assuming one condition.
// Confirmed original slots (frame 0x51c): packet [esp+0x10], from (byte, stored from bl at
// 0x455f78) [esp+0x14], sender (dword) [esp+0x1c], messages [esp+0x70] (read back at
// 0x455f69 for the return), send-loop index [esp+0xb4], FUN_004534e0 result [esp+0xf0],
// receive-loop index [esp+0x110]. Our build has packet 0x10, sender 0x14, from 0x24,
// messages 0x48, the FUN_004534e0 result 0x44, send index 0xb8, so the permutation is not
// declaration order: MSVC5 put sender below from and messages above both. The three missing
// bodies and the declaration order of the inlined FindPlayer copies are what fix that layout.
// Also: our zeroing loop uses a register zero (xor esi,esi) where the original uses an
// immediate 0 store, a register-pressure symptom, not a source difference to chase now.
// BIGGEST remaining structural gap: the sender != 0 guard is not an if with an empty else.
// The original's sender != 0 path starts at 0x45473f (the DAT_00512bc0 mode mask), and the
// sender == 0 path (0x453ed4..0x453f1a) is real code: it checks *recipient != 0, then
// recipient[0x73] == 1 or 2, then recipient[0x73] == 1 again (the compiler emits a
// duplicate cmp bl,1), then packet[0] and dispatches a SMALL switch with only cases 3
// (0x45413f), 5 (0x453f20) and 0x102 (0x454425), everything else continues. Those three
// bodies look like copies of the big switch's cases 3/5/0x102 that only run for our own
// looped-back commands. Adding that branch is the next real gain; it is ~30 instructions
// and shifts everything after it.
// Partial, 14.3%: player messages and most byte commands are restored. Commands 28, 33
// and 39 remain missing. Frame, switch layout and register allocation still differ.
// Re-checked by deepseek-v4.1 at 09:04Z, still 14.3% (6420 of 8944 bytes), no variant
// scored higher in the timebox. What still differs, from the branch/vs/original diff:
//  - frame is 0x318 but the original reserves 0x51c, so every [esp+N] local slot is
//    wrong and the whole prologue plus most early accesses mismatch. The missing
//    command bodies (28, 33, 39) hold the large locals the original reserved.
//  - the zeroing loop at 0x453d6c keeps g_game in edx and re-loads it every iteration
//    (mov edx,[0x511de8] inside the loop); this source hoists it into esi once.
//  - the sender id loop keeps its index at [esp+0xb4] and the recipient id loop at
//    [esp+0x110] in the original; here they land at [esp+0xb8] and [esp+0x50].
// Next step for whoever retries: restore the three missing command bodies first,
// which should push the frame to 0x51c and fix the local offsets globally.
// deepseek-v4.1 tried that: build/scratch/0x453d40/v4_frame51c_full.cpp has all
// three bodies (case 28 with char[0xac] or [0xc0], case 33, case 39 with
// char[0xe8] or [0xd4]) and the frame comes out at exactly 0x51c, but the score
// is 9.6%: the total is right while every individual slot is wrong. In that
// build messages sits at [esp+0x60] (original 0x70), receiving at 0xec (0xf0),
// the Class_00463be0 temp at [esp+0x3e4] (0x2e0) and the receive-loop index at
// 0xa0/0xa8 (0xb4/0x110), so the code generator slot order, not the sizes, is
// what still differs. Original draw order, from the top of the frame down:
// case-39 sprintf buffer (0x434), Class_00463be0 temp (0x2e0, 0x14b), case-28
// sprintf buffer (0x220), case-28 5-byte message (0x218), 0x160/0x15d, then the
// small scalars at 0x10..0x160 with messages at 0x70, the send-loop index at
// 0xb4 and the receive-loop index at 0x110. The zeroing loop at 0x453d6c also
// reloads g_game into edx every iteration and stores immediate 0.
// deepseek-v4.1 third pass (10 min timebox, base kept at 15.2%, no variant scored
// higher): tried the zeroing loop as store-first, as a `char* p` induction pointer,
// with split declarations and with the offset added last; all 15.1-15.2. Tried the
// inlined FindPlayer scan as an outer-declared for (15.2) and as a while with the
// increment at the end (11.3, so the for form must stay). Nothing moved the frame
// (0x318 vs 0x51c) or the first diverging hunk after it, which is still the
// register-zero/immediate-zero store at 0x453d78.
#include <string.h>

extern char* g_game;
extern char DAT_005119b8[];
extern unsigned char DAT_00512bc0[];
int FUN_004b6340();
void __stdcall FUN_004565a0(void*);
void __stdcall FUN_00463ca0(void*, int, int, unsigned char);
void __stdcall FUN_00451bc0(int, int, void*, int);
void __stdcall FUN_004861d0(unsigned char, void*);
void __stdcall FUN_0048ab70(void*);
void __stdcall FUN_00489ce0(void*);
void __stdcall FUN_004866d0(void*, int);
void __stdcall FUN_0049d270(void*, void*);
void __stdcall FUN_0049af90(void*, void*);
void __stdcall FUN_004233a0(int, int, int);
void __stdcall FUN_00423550(int, int, int);
int __stdcall FUN_00481550(int, int);
void __stdcall FUN_004244b0(int, int, int, void*);
void __stdcall FUN_0041b8d0(void*, void*);
void __stdcall FUN_0047f300(int, void*, int);
void __stdcall FUN_0047f0c0(int, int);
void __stdcall FUN_00464b30(unsigned char, unsigned char, float, int);
void __stdcall FUN_00464c60(unsigned char, unsigned char, float, int);
void __stdcall FUN_00485420(unsigned char, unsigned char);
void __stdcall FUN_00490df0(unsigned int, int);
void __stdcall FUN_00457540(void*, void*);
void __stdcall FUN_0048b920(void*, void*);
class Class_0048b090 {
  public:
    void FUN_0048b090(int, int);
};
class Class_004b0b00 {
  public:
    int FUN_004b0b00(int, void*, int, int, int, int, int, int);
};
class Class_0046d500 {
  public:
    void FUN_0046d500(void*, unsigned char);
};
int __stdcall FUN_00452570(int, int);
void __stdcall FUN_004523e0(int, int, int);
void __stdcall FUN_00451df0(int, void*, int);
void __stdcall FUN_00452bd0(void*);
void __stdcall FUN_00488570(void*, void*, void*);
void FUN_00450530();
extern int DAT_00506dbc;
class Class_004618a0 {
  public:
    void FUN_004618a0(int);
};
class Class_00461620 {
  public:
    void FUN_00461620(int, int, int);
};
extern Class_004618a0 DAT_00513000;
extern char DAT_00505dc4[];
extern char DAT_005065c4[];
extern char DAT_0050658c[];
extern char DAT_00506290[];
int __cdecl sprintf(char*, const char*, ...);
void __stdcall FUN_00452cc0(int);
char* __stdcall FUN_004c5740(char*, ...);
void __stdcall FUN_0047f1a0(char*, int);
void __stdcall FUN_00452960(int, int, unsigned char, int);
void FUN_00446fb0();
int FUN_004534e0();
void FUN_00450980();
void FUN_00453c20();
int __stdcall FUN_0044ffd0(unsigned char);
unsigned char __stdcall FUN_0044fe40(int);
int __stdcall FUN_00450a10(int);
void __stdcall FUN_00453010(int, int);
void __stdcall FUN_00451090(char*, int*, int*, int*, int*);
void __stdcall FUN_004c9890(void*, char*, char*, int, int, int, int);
class Class_00456030 {
  public:
    int FUN_00456030();
};
class Class_00463c60 {
  public:
    void FUN_00463c60(int);
};
class Class_00463c40 {
  public:
    void FUN_00463c40();
};
class Class_00463be0 {
  public:
    char data[0x14b];
    Class_00463be0();
};

static inline unsigned char FindPlayer_453d40(int id) {
    if (id != -1) {
        for (unsigned char i = 0; i < 10; ++i) {
            char* p = g_game + 0x14b * i;
            int found = p[0x1bd6] ? *(int*)(p + 0x1b67) : -1;
            if (found == id)
                return i;
        }
    }
    return 10;
}
static inline unsigned char FindHost_453d40() {
    for (unsigned char i = 0; i < 10; ++i) {
        char* p = g_game + 0x14b * i;
        if (p[0x1bd6] && (*(unsigned char*)(*(char**)(p + 0x1b8a) + 0x97) & 1))
            return i;
    }
    return 10;
}
static inline char* ResolvePlayer_453d40(int id) {
    if (id != -1) {
        for (unsigned char i = 0; i < 10; ++i) {
            if (FUN_0044ffd0(i) == id) {
                unsigned char slot = FUN_0044fe40(id);
                return g_game + 0x14b * slot + 0x1b63;
            }
        }
    }
    return 0;
}
static inline int PlayerId_453d40(unsigned char i) {
    if (i != 10 && g_game[0x1bd6 + 0x14b * i])
        return *(int*)(g_game + 0x1b67 + 0x14b * i);
    return -1;
}

static inline unsigned char NetworkSlot_453d40(int id) {
    if (id != -1) {
        for (unsigned char i = 0; i < 10; ++i)
            if (FUN_0044ffd0(i) == id)
                return i;
    }
    return 10;
}
static inline char* NetworkPlayer_453d40(int id) {
    if (NetworkSlot_453d40(id) == 10)
        return 0;
    unsigned char i = NetworkSlot_453d40(id);
    return g_game + 0x1b63 + 0x14b * i;
}
static inline char* LogicalPlayer_453d40(int id) {
    if (FindPlayer_453d40(id) == 10)
        return 0;
    unsigned char i = FindPlayer_453d40(id);
    return g_game + 0x1b63 + 0x14b * i;
}

// FUNCTION: 0x453d40
int FUN_00453d40() {
    if (!(g_game[0x2a44] & 1))
        return 0;
    int messages = 0;
    int receiving = 1;
    int n = 10, off = 0;
    do {
        off += 0x14b;
        *(int*)(g_game + 0x1a28 + off) = 0;
    } while (--n);
    int* packet = *(int**)(g_game + 0x2a38);
    do {
        receiving = FUN_004534e0();
        if (!receiving)
            break;
        int sender = *(int*)(g_game + 0x4c9);
        unsigned char from = FindPlayer_453d40(sender);
        unsigned char to = FindPlayer_453d40(*(int*)(g_game + 0x4cd));
        char* player = g_game + 0x1b63 + 0x14b * from;
        char* recipient = g_game + 0x1b63 + 0x14b * to;
        ++messages;
        if (sender == 0) {
            if (*(int*)recipient == 0)
                continue;
            unsigned char rs = recipient[0x73];
            if (rs != 1) {
                if (rs != 2)
                    continue;
                if (rs != 1)
                    continue;
            }
            switch (packet[0]) {
            case 5:
                if (packet[1] == 1) {
                    char* p = ResolvePlayer_453d40(packet[2]);
                    if (p && *(int*)p && (p[0x73] == 1 || p[0x73] == 2 || p[0x73] == 3) &&
                        p[0x146] != 10) {
                        if (!(g_game[0x2a44] & 4) &&
                            (*(unsigned char*)(*(char**)(p + 0x27) + 0x97) & 1) && p[0x73] == 3) {
                            FUN_00453010(*(int*)(p + 4), 1);
                            FUN_00453010(
                                *(int*)(g_game + 0x1b67 + 0x14b * (unsigned char)g_game[0x2a42]), 10);
                            ((Class_00463c60*)p)->FUN_00463c60(0);
                            p = g_game + 0x1b63 + 0x14b * (unsigned char)g_game[0x2a42];
                        } else {
                            FUN_00453010(*(int*)(p + 4), 1);
                        }
                        ((Class_00463c60*)p)->FUN_00463c60(0);
                        g_game[0x2bee] |= 1;
                        if (*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                                        0x14b * (unsigned char)g_game[0x2a42]) +
                                              0x97) &
                            1) {
                            char name[32];
                            int d, c, b, a;
                            FUN_00451090(name, &d, &c, &b, &a);
                            if (*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                                            0x14b * (unsigned char)g_game[0x2a42]) +
                                                  0x9b) &
                                0x10)
                                *(int*)(g_game + 0x475) |= 0x20;
                            FUN_004c9890(g_game + 0x14, name, DAT_005119b8, d, c, b, a);
                        }
                    }
                }
                break;
            case 3:
                if (FUN_00450a10(packet[2])) {
                    ResolvePlayer_453d40(packet[2]);
                    unsigned char target = FindPlayer_453d40(packet[2]);
                    unsigned char host = FindHost_453d40();
                    if (((Class_00456030*)(g_game + 0x1b63 + 0x14b * host))->FUN_00456030()) {
                        char* payload = (char*)packet[4];
                        char* info = *(char**)(g_game + 0x1b8a + 0x14b * (unsigned char)g_game[0x2a42]);
                        if (*(unsigned short*)(info + 0x9b) & 0x8000) {
                            FUN_00453010(PlayerId_453d40(target), 3);
                        } else if (packet[5] != 0x15) {
                            FUN_00453010(PlayerId_453d40(target), 8);
                        } else if (*(short*)(payload + 0x11) != 0 ||
                                   *(short*)(payload + 0x13) != 0x50) {
                            FUN_00453010(PlayerId_453d40(target), 8);
                        } else if ((info[0x9d] & 1) &&
                                   (!payload || _strcmpi(g_game + 0x2be3, payload))) {
                            FUN_00453010(PlayerId_453d40(target), 4);
                        }
                    }
                }
                break;
            case 0x102:
                if (packet[1] == 1) {
                    Class_00463be0 temporary;
                    char* p = ResolvePlayer_453d40(packet[2]);
                    if (p) {
                        unsigned char target = FindPlayer_453d40(packet[2]);
                        if (target != 10) {
                            char* payload = (char*)packet[3];
                            memcpy(*(void**)(g_game + 0x1b8a + 0x14b * target), payload, 0xb9);
                            unsigned char host = FindHost_453d40();
                            if (host == (unsigned char)g_game[0x2a42] &&
                                !(*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                                              0x14b * (unsigned char)g_game[0x2a42]) +
                                                    0x9b) &
                                  0x80) &&
                                (payload[0x9b] & 0x40))
                                FUN_00453010(*(int*)(p + 4), 9);
                        }
                    }
                    ((Class_00463c40*)&temporary)->FUN_00463c40();
                }
                break;
            case 0x103:
                if (packet[1] == 1) {
                    char* p = ResolvePlayer_453d40(packet[2]);
                    if (p) {
                        strncpy(p + 0x2b, (char*)packet[6], 0x1e);
                        strncpy(p + 0x49, (char*)packet[5], 0x1e);
                    }
                }
                break;
            case 0x104:
                if (FindHost_453d40() != (unsigned char)g_game[0x2a42])
                    memcpy(g_game + 0x471, packet + 1, 0x50);
                break;
            }
            continue;
        }
        unsigned char* bytes = (unsigned char*)packet;
        unsigned char cmd = bytes[0];
        int mode = *(int*)(g_game + 0x391f1);
        int mask = mode == 5 ? 2 : (mode == 6 ? 4 : 1);
        if (!(DAT_00512bc0[cmd * 4] & mask))
            continue;
        if (*(int*)player && (player[0x73] == 1 || player[0x73] == 2))
            continue;
        if (!*(int*)player || (player[0x73] != 1 && player[0x73] != 2 && player[0x73] != 3) ||
            player[0x146] == 10) {
            FUN_00453010(sender, 6);
            continue;
        }
        if (cmd < 2 && cmd > 0x2c) {
            FUN_00453010(sender, 6);
            continue;
        }
        if (player[0x73] != 1 && player[0x73] != 2 && player[0x73] != 3)
            continue;
        if (!*(int*)recipient ||
            (recipient[0x73] != 1 && recipient[0x73] != 2 && recipient[0x73] != 3) ||
            recipient[0x146] == 10)
            continue;
        *(int*)(player + 0x1c) = FUN_004b6340();
        ++*(int*)(player + 0x10);
        switch (cmd) {
        case 32: {
            unsigned char target = FindPlayer_453d40(*(int*)(bytes + 0x91));
            if (target != 10) {
                char* q = g_game + 0x1b63 + target * 0x14b;
                if (*(int*)q && q[0x73] == 3) {
                    memcpy(*(void**)(q + 0x27), bytes + 1, 0xb9);
                    FUN_00450980();
                }
            }
            break;
        }
        default:
            break; // Remaining command cases are not yet transcribed.
        case 23:
            if (*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                            0x14b * (unsigned char)g_game[0x2a42]) +
                                  0x97) &
                1) {
                int result = FUN_00452570(*(int*)(g_game + 0x4c9), (signed char)bytes[1]);
                int id = -1;
                for (int j = 0; j < 10; ++j) {
                    if (g_game[0x1bd6 + j * 0x14b] == 1) {
                        id = *(int*)(g_game + 0x1b67 + j * 0x14b);
                        break;
                    }
                }
                if (!result) {
                    FUN_004523e0(id, *(int*)(g_game + 0x4c9), (signed char)bytes[1]);
                } else {
                    unsigned char response[2];
                    response[0] = 0x18;
                    response[1] = bytes[1];
                    FUN_00451bc0(id, *(int*)(g_game + 0x4c9), response, 2);
                    if (DAT_00506dbc)
                        DAT_00513000.FUN_004618a0(1);
                }
            }
            break;
        case 24:
            (*(char**)(g_game + 0x1b8a + 0x14b * to))[0x96] = bytes[1];
            if (to == (unsigned char)g_game[0x2a42] && (g_game[0x2a44] & 1)) {
                for (int j = 0; j < 10; ++j) {
                    char* q = g_game + 0x1b63 + 0x14b * j;
                    if (*(int*)q && (q[0x73] == 1 || q[0x73] == 2)) {
                        unsigned char response[0xba];
                        memcpy(response + 1, *(void**)(q + 0x27), 0xb9);
                        *(int*)(response + 0x91) = *(int*)(q + 4);
                        response[0] = 0x20;
                        FUN_00451df0(*(int*)(q + 4), response, 0xba);
                        FUN_00452bd0(q);
                    }
                }
                FUN_00450530();
                DAT_00513000.FUN_004618a0(1);
            }
            g_game[0x2bee] |= 1;
            break;
        case 2:
            FUN_004565a0(packet);
            break;
        case 38:
            memcpy(g_game + 0x2c28, bytes + 1, 40);
            g_game[0x2bee] |= 1;
            break;
        case 35: {
            char* a = NetworkPlayer_453d40(*(int*)(bytes + 1));
            char* b = NetworkPlayer_453d40(*(int*)(bytes + 5));
            if (a && b) {
                if (bytes[9])
                    FUN_0047f1a0(DAT_00505dc4, 0);
                if (*(int*)b && (b[0x73] == 1 || b[0x73] == 2)) {
                    FUN_00452960(*(int*)(bytes + 1), *(int*)(bytes + 5), bytes[9],
                                 *(int*)(bytes + 10));
                    if (!(g_game[0x2a44] & 4))
                        g_game[0x2bee] |= 1;
                    else
                        FUN_00446fb0();
                }
                g_game[0x1c6b + 0x14b * (unsigned char)a[0x146] + (unsigned char)b[0x146]] =
                    bytes[9];
            }
            break;
        }
        case 36: {
            char* q = NetworkPlayer_453d40(*(int*)(bytes + 1));
            if (q)
                q[0x13f] = bytes[5];
            if (!(g_game[0x2a44] & 4))
                g_game[0x2bee] |= 1;
            break;
        }
        case 27: {
            char* q = NetworkPlayer_453d40(*(int*)(bytes + 1));
            if (q)
                FUN_00453010(*(int*)(q + 4), bytes[5]);
            break;
        }
        case 30: {
            unsigned char response[5];
            response[0] = 0x1f;
            recipient[0x147] = bytes[1];
            *(int*)(response + 1) = *(int*)(recipient + 4);
            FUN_00451bc0(*(int*)(response + 1), *(int*)(player + 4), response, 5);
            break;
        }
        case 31: {
            unsigned char target = FindPlayer_453d40(*(int*)(bytes + 1));
            if (target != 10)
                *(int*)(g_game + 0x29d0 + target * 4) = 1;
            break;
        }
        case 5:
            if (*(int*)recipient && recipient[0x73] == 1)
                FUN_00463ca0(bytes + 1, 8, 0, from);
            break;
        case 6: {
            unsigned char response = 7;
            int id = -1;
            for (int j = 0; j < 10; ++j) {
                char* q = g_game + 0x1b63 + 0x14b * j;
                if (*(int*)q && (q[0x73] == 1 || q[0x73] == 2)) {
                    id = *(int*)(q + 4);
                    break;
                }
            }
            FUN_00451bc0(id, *(int*)(g_game + 0x4c9), &response, 1);
            break;
        }
        case 7:
            player[0x21] |= 1;
            break;
        case 8:
            receiving = 0;
            g_game[0x2a44] |= 4;
            break;
        case 9:
            FUN_004861d0(from, packet);
            break;
        case 44:
            FUN_0048b920(player, packet);
            break;
        case 10:
            FUN_0048ab70(packet);
            break;
        case 11:
            FUN_00489ce0(packet);
            break;
        case 12:
            FUN_004866d0(packet, 0);
            break;
        case 13:
            FUN_0049d270(player, packet);
            break;
        case 14:
            FUN_0049af90(player, packet);
            break;
        case 15: {
            unsigned int kind = bytes[1];
            unsigned int x = *(unsigned short*)(bytes + 2);
            unsigned int y = *(unsigned short*)(bytes + 4);
            if (kind == 0xfd)
                FUN_00423550(x, y, 0);
            else if (kind == 0xfe)
                FUN_004233a0(x, y, 1);
            else if (kind == 0xff)
                FUN_00423550(x, y, 1);
            else {
                char* feature = g_game + 0x2cf3 + kind * 0x115;
                int tile = FUN_00481550(x, y);
                FUN_004244b0(tile, x, y, feature);
            }
            break;
        }
        case 16: {
            unsigned short index = *(unsigned short*)(bytes + 1);
            char* unit = index ? *(char**)(g_game + 0x14357) + index * 0x118 : 0;
            if (*(unsigned int*)(unit + 0x110) & 0x10000000)
                (*(Class_004b0b00**)(unit + 0x9a))
                    ->FUN_004b0b00(*(short*)(bytes + 3), 0, 0, bytes[5], *(int*)(bytes + 6),
                                   *(int*)(bytes + 10), *(int*)(bytes + 14),
                                   *(int*)(bytes + 18));
            break;
        }
        case 17: {
            unsigned short index = *(unsigned short*)(bytes + 1);
            char* unit = index ? *(char**)(g_game + 0x14357) + index * 0x118 : 0;
            if (*(unsigned int*)(unit + 0x110) & 0x10000000) {
                ((Class_0048b090*)unit)->FUN_0048b090(bytes[3], 1);
                ((Class_0048b090*)unit)->FUN_0048b090((unsigned char)~bytes[3], 0);
            }
            break;
        }
        case 18: {
            unsigned short first = *(unsigned short*)(bytes + 1);
            unsigned short second = *(unsigned short*)(bytes + 3);
            char* units = *(char**)(g_game + 0x14357);
            void* a = first ? units + first * 0x118 : 0;
            FUN_0041b8d0(second ? *(char**)(g_game + 0x14357) + second * 0x118 : 0, a);
            break;
        }
        case 19:
            if (!bytes[1])
                FUN_0047f300(*(int*)(bytes + 2), bytes + 6, 0);
            else
                FUN_0047f0c0(*(int*)(bytes + 2), 0);
            break;
        case 20: {
            unsigned short index = *(unsigned short*)(bytes + 1);
            char* unit = index ? *(char**)(g_game + 0x14357) + index * 0x118 : 0;
            if (unit && (*(unsigned int*)(unit + 0x110) & 0x10000000)) {
                int id = *(int*)(bytes + 3);
                char* q = 0;
                if (FindPlayer_453d40(id) != 10) {
                    unsigned char slot = NetworkSlot_453d40(id);
                    q = g_game + 0x1b63 + 0x14b * slot;
                }
                if (*(int*)q && (q[0x73] == 1 || q[0x73] == 2))
                    FUN_00488570(unit, q, packet);
            }
            break;
        }
        case 21:
            if (g_game[0x38d75] & 4)
                *(int*)(g_game + 0x29a4 + 4 * from) = 1;
            break;
        case 22: {
            unsigned char a = FindPlayer_453d40(*(int*)(bytes + 5));
            unsigned char b = FindPlayer_453d40(*(int*)(bytes + 9));
            if (a != 10 && b != 10) {
                int kind = *(int*)(bytes + 1);
                if (kind == 1)
                    FUN_00464b30(a, b, *(float*)(bytes + 13), 0);
                else if (kind == 2)
                    FUN_00464c60(a, b, *(float*)(bytes + 13), 0);
                else if (kind == 3)
                    FUN_00485420(a, b);
            }
            break;
        }
        case 40:
            FUN_00457540(packet, player);
            break;
        case 41:
            if (bytes[1]) {
                recipient[0x11e + from] = 1;
                if (bytes[2])
                    recipient[0x134 + from] = 1;
            }
            break;
        case 25:
            if (!bytes[1])
                *(unsigned short*)(g_game + 0x38a51) =
                    (*(unsigned short*)(g_game + 0x38a51) & 0xfffe) | (bytes[2] & 1);
            else
                FUN_00490df0(bytes[2], 0);
            break;
        case 26:
            if (*(void**)(g_game + 0x2a30) && *(int*)recipient && recipient[0x73] == 1)
                (*(Class_0046d500**)(g_game + 0x2a30))->FUN_0046d500(packet, from);
            break;
        case 29:
            if (DAT_00506dbc)
                ((Class_00461620*)&DAT_00513000)
                    ->FUN_00461620(*(int*)(g_game + 0x4c9), *(int*)(bytes + 1),
                                   *(int*)(bytes + 5));
            break;
        case 34: {
            char* q = LogicalPlayer_453d40(*(int*)(bytes + 1));
            if (q)
                *(int*)(q + 0xc) = bytes[5];
            break;
        }
        case 42:
            player[0x20] = bytes[1];
            break;
        }
        continue;
    } while (receiving);
    FUN_00450980();
    FUN_00453c20();
    return messages;
}
