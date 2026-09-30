// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Partial: 84.3%, 848 bytes (the exact original size). Still open:
// RETRY deepseek-v4.1-flash: no improvement this pass. Confirmed the do/while
// form reproduces the original bottom test (`xor edx,edx` preheader, no entry
// test, `cmp edx,0xcee; jl` at the foot) but drops the file to 846 bytes and
// 67.3%: the flag byte load becomes `mov dl,[ecx+0x97]; and edx,1` instead of
// the original `xor eax,eax; mov al,[ecx+0x97]; and eax,1`, which shifts every
// later byte and destroys the score. That 2-byte/latch pair is the whole
// remaining problem; the register rotation is downstream of it. Tried on top
// of the do/while (each still 846 / 67.3, i.e. byte-identical): flag as
// unsigned int, flag split into `int flag = x; flag &= 1;`, flag via a byte
// temp, +0x97 declared as a 1-bit bitfield union member read as `p->data->b0`,
// a named `PlayerData* d = p->data`, and p computed as
// `g_game->players + FUN_0044fe40(id)` instead of `&g_game->players[...]`.
// None changed the flag encoding or the rotation. Earlier notes below stand.
// RETRY deepseek-v4.1-flash (pass 2): confirmed the flag encoding cannot be
// forced from the source. On the bottom-tested loop (`for (i=0;i<10;i++)` and
// `do {} while (i<10)` both give the original latch at 846 / 67.3), tried
// `unsigned int flag = p->data->flags; flag &= 1;`, a 1-bit bitfield read
// (`p->data->b0` via a union), a `(flags & 1) != 0` form, and the `register`
// keyword on p/slot. Every one still emits `mov dl,[ecx+0x97]; and edx,1`,
// because p=ebx, game=edi, slot=esi; the missing `xor eax,eax; mov al` is a
// downstream symptom of that coloring. Also tried `Game* g = g_game;` (early
// and everywhere, 51-63%), a ternary p, an array-base local, and an
// extra g_game reference in the fi==10 arm (all worse, 50-65%). Base stays
// the best at 848 bytes / 84.3%.
//  * the player/slot/game register rotation: the original keeps g_game in ebx,
//    the player pointer in esi and the slot byte in edi; ours allocates
//    ebx=player, esi=slot, edi=g_game. Two extra uses of g_game (one inside
//    the clearing loop, one before the b2 test) and a function-scope
//    `Game* g = g_game;` local did not move the priority order.
//  * the clearing loop: the original is bottom-tested (preheader
//    `xor edx,edx`, no entry test, `cmp edx,0xcee; jl` at the foot), ours is
//    top-tested with `while (1) { if (i >= 10) break; ... }`. A `do/while`
//    gives the original latch but then drops to 846 bytes and 67.3%, because
//    the flag load degenerates to a partial `mov dl, byte ptr [ecx+0x97]`
//    instead of `xor eax,eax; mov al, ...; and eax,1`. So one still has to
//    force the full zero extension of the flag.
//  * the b2 arm is fixed: writing it as three `if (cond) goto after_remove;`
//    plus a trailing `goto do_remove;` (instead of `if (a||b) goto L;`) makes
//    MSVC emit the original's `cmp al,2; je L; jmp S`.
//  * deepseek-v4.1 tried, all byte-identical or worse: swapping the two arms
//    of the fi test (83.6%, the arms change place in the emitted code),
//    writing p as a ternary, `p = g_game->players + idx`, moving the slot,
//    flag and loop-index declarations to the top of the function, declaring
//    the clearing-loop index unsigned char (49.4%) and inlining the
//    FindIndex_00452cc0 loop into the body (63.9%, so the one-use inline
//    helper shape is load-bearing). The rotation is not a declaration-order
//    or use-count lever.
// headers.py tried 128 header sets (all 84.3%), and alternate flag types and
// the existing inline predicate helpers (IsActive12 and friends) did not help.

#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00463c60 {
public:
    void FUN_00463c60(int param_1);
};

struct PlayerData_00452cc0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
    char unknown_95[0x97 - 0x95];
    unsigned char flags;               // +0x97
    char unknown_98[0x9d - 0x98];
    unsigned short word_9d;            // +0x9d
};

struct Player_00452cc0 {
    int active;                        // +0x00
    unsigned int id;                   // +0x04
    char unknown_8[0xc - 8];
    int field_c;                       // +0x0c
    char unknown_10[0x27 - 0x10];
    PlayerData_00452cc0* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allies[0xb];         // +0x108
    unsigned char field_113[0xb];      // +0x113
    char unknown_11e[0x146 - 0x11e];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00452cc0 {
    char unknown_0[0x14];
    char unknown_14[0x1b63 - 0x14];
    Player_00452cc0 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    union {
        unsigned short value;          // +0x2a44
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short rest : 12;
        };
    } flags;
    char unknown_2a45[0x391e9 - 0x2a46];
    Class_00435100* net;               // +0x391e9
};
#pragma pack(pop)

extern Game_00452cc0* g_game;

int __stdcall FUN_0044ffd0(unsigned char index);
unsigned char __stdcall FUN_0044fe40(int id);
void __stdcall FUN_00486f10(unsigned char player);
void __stdcall FUN_0046c620(int msg);
int __stdcall FUN_004ca780(void* net, int id);

static inline unsigned char FindIndex_00452cc0(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (FUN_0044ffd0(i) == id)
            return i;
    }
    return 10;
}

static inline int PlayerId_00452cc0(unsigned char i)
{
    if (i != 10 && g_game->players[i].type != 0)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00452cc0(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (PlayerId_00452cc0(i) == id)
                return i;
        }
    }
    return 10;
}

static inline int IsActive12_00452cc0(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1 || p->type == 2)
        return 1;
    return 0;
}

static inline int IsType1_00452cc0(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1)
        return 1;
    return 0;
}

static inline int IsType3_00452cc0(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 3)
        return 1;
    return 0;
}

// FUNCTION: 0x452cc0
void __stdcall FUN_00452cc0(int id)
{
    unsigned char fi;
    if (id == -1)
        fi = 10;
    else
        fi = FindIndex_00452cc0(id);

    Player_00452cc0* p;
    if (fi == 10)
        p = 0;
    else
        p = &g_game->players[FUN_0044fe40(id)];

    if (p == 0)
        return;
    if (p->active == 0)
        return;
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return;
    if (p->field_146 == 10)
        return;

    unsigned char slot = p->field_146;
    int flag = p->data->flags & 1;

    int i = 0;
    while (1) {
        if (i >= 10)
            break;
        Player_00452cc0* q = &g_game->players[i];
        if (q->active != 0 && (q->type == 1 || q->type == 2)) {
            q->field_113[slot] = 0;
            q->allies[slot] = 0;
        }
        i++;
    }

    FUN_00486f10(FindPlayerIndex_00452cc0(id));

    if (g_game->flags.b2) {
        if (p->active == 0)
            goto do_remove;
        if (p->type == 1)
            goto after_remove;
        if (p->type == 2)
            goto after_remove;
        goto do_remove;
    }
    if (p->active != 0 && (p->type == 1 || p->type == 2))
        FUN_004ca780((char*)g_game + 0x14, p->id);

do_remove:
    ((Class_00463c60*)p)->FUN_00463c60(0);
    p->active = 0;
    p->id = -1;
    p->field_c = 0;

after_remove:
    g_game->field_2a3c--;
    p->data->word_9d &= 0xfffb;
    memset(&p->allies, 0, 11);

    if (g_game->net->FUN_00435100() == 3)
        FUN_0046c620(3);

    if ((g_game->flags.value & 4) && flag != 0) {
        unsigned int best = 0;
        Player_00452cc0* q = g_game->players;
        int n = 10;
        do {
            if ((q->active != 0 && q->type == 3)
                || (q->active != 0 && q->type == 1)) {
                if (q->id > best)
                    best = q->id;
            }
            q++;
        } while (--n);
        Player_00452cc0* r;
        if (FindPlayerIndex_00452cc0(best) == 10)
            r = 0;
        else
            r = &g_game->players[FindPlayerIndex_00452cc0(best)];
        if (r != 0)
            r->data->flags |= 1;
    }
}

