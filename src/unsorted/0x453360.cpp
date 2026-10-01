// Decompiled by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol. Names are provisional.
// Retry #2591 (GPT-6.1-sol): best remains 54.4% (371/380 bytes). Rechecked the
// saved best and tested passing Game* into FindTarget and writing its result
// through an output pointer; neither moved the register allocation. No MATCH.
// PARTIAL (54.4% with the current check.py diff), best found. Sends a
// chat/text message (type 5, up to 64 characters) to the players selected
// by the game's chat mode at +0x2bf0. The whole control flow, every offset
// and constant, and the inlined FUN_0044fe00 target search match. The
// original reloads the `text` argument dead into eax at the start of both
// mode branches, which keeps g_game in ecx; without that dead use MSVC keeps
// g_game in eax and rotates every scratch register by one. Tried and ruled
// out: unused inlined-helper parameters, void-cast locals, folded
// ternaries/commas, text[0] guards, member/__fastcall/__stdcall helpers,
// block-scoped locals, and all 128 header sets. /O2 eliminates every
// construct that could produce the reload.
//
// Notes from Claude Opus 5.5 (#228): the N-declarations test (0 to 400
// unused externs) stays at 54.4% for every N, so the source shape is wrong,
// not the compiler state. Whatever holds eax is already there right after
// strncpy: the original's inlined FUN_0044fe00 loop keeps i in eax and
// g_game in ecx (ours swaps them), and in the mode 1/2 loop the mode byte
// sits in dl and the mode 1 ally test becomes `cmp byte ptr [m], 0` because
// no byte register is free. Also tried, none moving g_game to ecx: loop-only
// inline helpers taking `text` (loop inside or outside the helper), per-index
// helpers for the id, player, selection and ally reads, one function-scope
// `i` for all three loops (with the target search written in place too), a
// switch on the mode, `text[0] == '+'` or the buffer setup moved into inline
// helpers, `if (text) {}` or `strlen(text);` inside and before the loops, a
// `char* t = text` walked with the loop, `text` reused to hold the buffer
// pointer before each send, and an unused `ok` result local.
//
// Retry pass 2 (deepseek-v4.1-flash, #1683): measured the remaining gap as
// exactly (a) the home register of g_game after strncpy (original: ecx; ours:
// eax, which rotates every downstream operand and the find-target index) and
// (b) two missing `mov eax, [esp+0x14]` dead reloads of `text` at the top of
// the mode-3 and default branches (8 of the 9 lost bytes). Those two reloads
// are what keeps eax busy so g_game takes ecx. None of these reproduced the
// reload or the register home: the target search as an inlined member function
// (`g_game->FindTarget()`, same 54.4%), `char* res = strncpy(...)` unused,
// `char* t = text;` declared at the top of each else branch (both 54.4%,
// eliminated), and `text != 0` added to the mode-3 loop guard (52.3%, moved
// text into ebp and changed the prologue). An empty inlined helper called with
// `text` in both branches is folded away (54.4%). The reload is a load with no
// consumer, so it most likely comes from an inlined function or macro that
// takes `text` and drops it in that path, not from any spelling of these loops.
//
// Retry pass (same model): an inline helper for the `text[0] == '+'` test is
// byte-identical to this file; an inline `while(1)` target search (the shape
// required in the sibling 0x453010) scores 46.7, a hoisted `int mode` local
// 46.1, and a live `text` local 46.9, all in the same swapped-register basin.
// So the diff is not the search shape or the '+' test wording.
//
// Retry pass 3 (deepseek-v4.1, #2057): the whole function is byte-identical
// except for ONE anchor. After `call strncpy` the original reloads g_game
// into ECX (`mov ecx,[0x511de8]`) and puts the search counter in EAX
// (`xor eax,eax`); ours swaps them (g_game EAX, counter ECX), which rotates
// every operator in all three arms and the found-target code (`mov edx,eax;
// mov edi,ecx` vs `mov edx,ecx; mov edi,eax`). The only other difference is
// the two dead `mov eax,[esp+0x14]` reloads of `text` at the top of the
// mode-3 and default arms (8 bytes of the 9 byte gap), and by elimination the
// argument slot is the only thing [esp+0x14] can hold (no frame: 4 pushes
// only, and no store to [esp+0x14] exists anywhere in the function).
// Tried this pass, all landing in the same EAX basin (54.4% unless noted):
// search helper as for/while/do-while, while(1) form (51.7), pointer-walk
// helper over players[] with the id still indexed by i, helper taking the
// count / taking char* / returning the index instead of the id (51.2),
// target initialised -1 with the search written in place (46.7, target then
// spills to [esp+0x14]) and the same with the counter hoisted to function
// scope (46.7), `target = FindTarget()` split into a declaration plus
// assignment, mode read first (`g_game->mode == 0 || text[0] == '+'`),
// switch on mode (50.4), inlined send helpers taking (from,to), taking
// (from,to,text) with text unused, taking the packet, whole arms as inlined
// helpers taking (from), empty/pointer-returning inlined helpers called with
// `text` at both arm tops, a real neighbour function (0x453320's body) before
// and after ours in the file, and dead `for (char* p = text; *p; p++)` skip
// loops at the arm tops (50.6, kept by the compiler and it moves the '+' test
// into dl, so `text` is not what the original reloads there).
// Reading of the anchor: the original has EAX taken at 0x45338c by something
// that survives the call (the strncpy return value, or a value the inliner
// materialised), so g_game lands in ECX; nothing in these arms reproduces a
// bare dword load of an otherwise unused value. The earlier passes also
// ruled out header sets (all 128), N unused externs, member/__fastcall
// helpers and function-scope locals, so this looks like front-end allocator
// state (the VC5 inliner's or the symbol table's), not a spelling of these
// loops.
// Retry pass (space-bunny-free, #2380): still 54.4%, byte-identical to the
// previous passes. Eight further spellings of a dead use of `text` inside both
// arms (a local in the loop body, a pointer in the for-init, `text,` as the
// first comma term of the for condition, a `text == text` guard, the counter
// declared before and after the dead local, a do-while with the dead local in
// the body) all compile to the same 371 bytes, so /O2 folds every one of them.
// TWIN TEST: the only matched functions that scan the same fields are 0x493ae0
// and 0x494050 (both `@@YGXXZ`, no arguments), so neither can host the dead
// `mov eax,[esp+0x14]` reload and the test cannot be run here. The reload must
// therefore come from an inlined function's own parameter reference that the
// optimizer drops the use but not the load of.
#include <iostream>
#include <string.h>

#pragma pack(push, 1)
struct Player_00453360 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x73 - 0x8];
    char state;                        // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allied[0x3e];        // +0x108
    char unknown_146[0x14b - 0x146];
};

struct Game_00453360 {
    char unknown_0[0x1b63];
    Player_00453360 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    char* buffer;                      // +0x2a38
    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bf0 - 0x2a43];
    unsigned char mode;                // +0x2bf0
    unsigned char field_2bf1[10];      // +0x2bf1
};
#pragma pack(pop)

extern Game_00453360* g_game;

int __stdcall FUN_00451df0(int player, void* data, int size);
int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);

// Same body as FUN_0044fe00, inlined here.
static inline int FindTarget()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].state == 1)
            return g_game->players[i].id;
    }
    return -1;
}

// See the notes at the top for what still differs.
// FUNCTION: 0x453360
void __stdcall FUN_00453360(char* text)
{
    g_game->buffer[0] = 5;
    strncpy(g_game->buffer + 1, text, 0x40);

    int target = FindTarget();

    if (text[0] == '+' || g_game->mode == 0) {
        FUN_00451df0(target, g_game->buffer, 0x41);
    } else if (g_game->mode == 3) {
        for (int i = 0; i < 10; i++) {
            if (g_game->field_2bf1[i] != 0) {
                int id = g_game->players[i].id;
                if (id != 0)
                    FUN_00451bc0(target, id, g_game->buffer, 0x41);
            }
        }
    } else {
        Player_00453360* lp = &g_game->players[g_game->localPlayer];
        for (int i = 0; i < 10; i++) {
            Player_00453360* p = &g_game->players[i];
            if (p->active != 0 && p->state == 3) {
                if ((g_game->mode == 1 && lp->allied[i] != 0) ||
                    (g_game->mode == 2 && lp->allied[i] == 0))
                    FUN_00451bc0(target, p->id, g_game->buffer, 0x41);
            }
        }
    }
}
