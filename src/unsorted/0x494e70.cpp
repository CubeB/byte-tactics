// Decompiled by deepseek-v4.1-flash. Names are provisional.

class Class_004ce680 {
public:
    int FUN_004ce680();
};

class Class_004ce690 {
public:
    void FUN_004ce690(int param_1);
};

#pragma pack(push, 1)
struct Player_00494e70 {                // 0x14b bytes
    char unknown_0[0x144];
    unsigned short count;               // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game_00494e70 {
    char unknown_0[0x10];
    Class_004ce680* field_10;           // +0x10
    char unknown_14[0x1b63 - 0x14];
    Player_00494e70 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    unsigned char field_2a43;           // +0x2a43
    unsigned char flags_2a44;           // +0x2a44
    char unknown_2a45[0x38d75 - 0x2a45];
    unsigned char flags_38d75;          // +0x38d75
};
#pragma pack(pop)

extern Game_00494e70* g_game;

unsigned int FUN_004b6340();
void FUN_0046c8b0();

extern unsigned int DAT_0051f2f8;
extern int DAT_0051f2fc;
extern int DAT_0051f2dc;
extern int DAT_0051e710[];
extern int DAT_005091d0;

// Frame-time / desync watchdog: keeps a 30-entry ring of per-frame counters at
// DAT_0051e710, and when the ring total (or the last five entries) grows too
// large it flips the network state DAT_005091d0 between 0 and 1 and resets the
// counter DAT_0051f2fc.
//
// PARTIAL: everything matches except the two tests on g_game->flags_38d75 at
// 0x494e8b. The original emits two memory-operand bit tests
// (`test byte ptr [eax+0x38d75],1` then `...,2`); this source makes MSVC load
// the byte once (`mov al,[eax+0x38d75]; test al,1; test al,2`), 4 bytes
// shorter. This is the same pattern documented in src/unsorted/0x452800.cpp
// (also 0x4550c2): MSVC 5 always CSEs the byte across the two tests unless a
// real call separates them (0x4ebcf5 reloads only because a store through an
// aliasing pointer sits between its tests). Tried here: `a && !b`, `!a || b`,
// nested ifs, `goto`, a single large `if`, `==`/`!=` forms, 8/16/32-bit
// fields, char/short bitfields, local copies, unions, mixed member/cast
// spellings, a dereferenced local pointer, two separate inline accessors, and
// all 128 header sets (headers.py); every form shares the load. The rest of
// the function, including the ring-sum loop, the state-transition logic and
// the DAT reloads, is byte-identical.

// FUNCTION: 0x494e70
void FUN_00494e70()
{
    if ((g_game->flags_2a44 & 4)
        && (!(g_game->flags_38d75 & 1) || (g_game->flags_38d75 & 2))
        && FUN_004b6340() > DAT_0051f2f8 + 0x1e) {
        int state = g_game->field_10->FUN_004ce680();
        if (++DAT_0051f2fc > 10) {
            int recent = 0;
            int n = 5;
            int i = DAT_0051f2dc - 1;
            int total = 0;
            int* p = &DAT_0051e710[i];
            for (;;) {
                if (i < 0) {
                    i += 30;
                    p += 30;
                }
                int v = *p;
                total += v;
                if (n != 0) {
                    n--;
                    recent += v;
                }
                if (i == DAT_0051f2dc)
                    break;
                i--;
                p--;
            }
            int newstate = DAT_005091d0;
            if (state == 0 && (total > 0x32 || recent > 0x1e)
                && g_game->players[g_game->localPlayer].count > 0x1e)
                newstate = 1;
            else if (state == 1 && total < 10 && recent == 0 && DAT_0051f2fc > 0x3c)
                newstate = 0;
            if (newstate != DAT_005091d0) {
                ((Class_004ce690*)g_game->field_10)->FUN_004ce690(newstate);
                DAT_0051f2fc = 0;
                DAT_005091d0 = newstate;
            }
        }
        if (++DAT_0051f2dc >= 30)
            DAT_0051f2dc = 0;
        DAT_0051e710[DAT_0051f2dc] = 0;
        FUN_0046c8b0();
        DAT_0051f2f8 = FUN_004b6340();
    }
}
