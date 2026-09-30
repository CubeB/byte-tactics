// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
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
// Retry pass 3 (space-bunny-free, #1683): still 54.4%, 371 of 380 bytes,
// and every variant below compiles to exactly the same 371 bytes, so the
// rotation is not decided inside the search helper or by its parameter list:
// a `Game* g = g_game;` local with the counter declared before it, the same
// with the pointer first, a helper taking `Game*`, a helper with a second
// dead counter, the whole mode-3 and default loops moved into static inline
// helpers that take an unused `char* text`, a folded ternary argument
// (`int n = text ? 0x41 : 0x41;`), a comma expression in the for-init
// (`for (int i = 0, k = (text, 0); ...)`), a dead `(text, 0);` statement and
// a dead `char* dead = (text, 0);` at the top of each branch. MSVC 5 /O2
// folds every one of those away, so the original's dead `mov eax,[esp+0x14]`
// is not a discarded expression: it has to be a reload MSVC still believes
// it needs, i.e. a register split of `text` whose remaining uses were
// removed, which in turn is what keeps g_game in ecx and the search counter
// in eax. Reproducing that needs a value that is live into the branch blocks
// and whose only use is one MSVC can delete while the block's copy stays.
// Still missing: the two dead text reloads and the eax/ecx home of g_game.
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
