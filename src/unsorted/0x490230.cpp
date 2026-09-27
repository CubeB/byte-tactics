// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// PARTIAL: 99.2% (300 of 301 bytes). The mission's defeat check: it does
// nothing unless the mission is active (the dword at +0x88, set to 1 by
// Class_0048df90), then asks the game mode which test to run: 1 runs the
// victory conditions (0x48fed0, here defined inline so /Ob2 inlines it,
// which is also what makes MSVC keep the constant 0 in ebp and push ebp),
// 2 runs the "every other player is dead or allied" loop of 0x48ffd0, and
// 3 calls FUN_00490080.
// What still differs: one SIB byte. The original reads the allied flag as
// [ecx + edi*1 + 0x108] (the loop counter in the base slot, the player record
// pointer in the index slot), every phrasing here gives [edi + ecx*1 + 0x108].
// The identical loop in 0x48ffd0 (matched) uses the base/index order this
// file produces, so the flip is a register-allocation effect of the extra
// code in this function, not of the expression.
// The one thing that does flip it (found by brute force over ~350 scratch
// variants, all scored with check.py --sym): make the subscript a plain
// unsigned int variable instead of the char counter, that is
//     unsigned int j = i;                 // before the i != player test
//     if (i != g_game->player && !me->allied[j] && g_game->players[i].count != 0)
// then MSVC emits the original's `mov al, [ecx + edi + 0x108]`. But the copy
// lives before the branch, so MSVC also hoists `mov ecx, [esp + 0x10]` and
// `and ecx, 0xff` above `cmp dl, bl` (the loop head becomes the reload, the
// back edge targets it) and that costs more than the SIB gains: 98.5%.
// Putting the copy after the `continue`, or inside a nested if, restores the
// placement and loses the flip again. The flip and the sinking are two sides
// of the same thing: the counter value has to be a temp that is live across
// the branch for MSVC to put it in the base slot.
// Second route to the flip, found later, costs more but localises the problem:
// use the copy in BOTH tests, so one temp serves `allied[j]` and
// `players[j].count` the way the original's ecx serves both:
//     if (i == g_game->player) continue;
//     unsigned int j = i;
//     if (me->allied[j]) continue;
//     if (g_game->players[j].count != 0) return 0;
// This does emit the original's SIB order, but the extra live temp costs 20%:
// MSVC gives up ebp as the zero constant for the whole function (`xor ebp,ebp`
// becomes `xor esi,esi`, and the inlined FUN_0048fed0's `cmp [edi+0x40], ebp`
// becomes `cmp [edi+0x40], esi`), the counter temp lands in eax instead of
// ecx, and the flag load becomes `mov cl, [eax + edi + 0x108]`. 78.2% overall.
// So the tie-break is reachable, but not without perturbing the allocation of
// the rest of the function. Note that the 99.2% version already uses exactly
// the original's register plan (dl counter, ecx counter-as-int, edi record
// pointer, ebp zero, esi g_game, bl player) and differs in this one byte only,
// so what is needed is a perturbation that changes the base/index tie-break
// and nothing else. Both routes above change the register plan as well.
// Ruled out (all still [edi + ecx*1 + 0x108], all at 99.2% or less): every
// headers.py set, 13 large headers, 0 to 1200 dummy externs, up to 4000 dummy
// prototypes, the 0x48ffd0 wording as an inlined helper (free, static, extern,
// with an argument, and its continue-chain variant), an inline IsAllied()
// accessor with either argument order, an inline unsigned int accessor,
// int/char/unsigned char/short index types, (int)i and (unsigned char)i
// casts, i*1, i+0, i|0, i&0xff, [10] and [11] flag arrays, bool and char
// flag arrays, a 2D flag array, an array of one-byte flag structs, const and
// non-const pointer and array declarations, a local copy of g_game, a local
// copy of the record pointer, a local copy of the flag row pointer, every
// declaration order of me, i and player (in the case block and at function
// scope), all six permutations of the three && operands, && chains, continue
// chains, nested ifs, while and do/while loops, an if/else dispatch instead
// of the switch (which is ruled out anyway: only a switch gives the dec/je
// chain), the class member layout (array sizes, member order, extra padding
// fields), and address expressions with the terms reversed (i + me + 0x108
// and friends: MSVC canonicalises the int back to the pointer side and then
// scales it by the record size, which is worse).

class Class_00435100 {
public:
    int FUN_00435100();
};

// Called with ecx = the mission, but never reads it (see 0x490080).
class Class_00490080 {
public:
    int FUN_00490080();
};

// Mission victory/defeat condition (see 0x48fed0.cpp). The slots carry the
// names their functions already have for the "destroy all units" victory
// condition, so its vtable matches the original's names.
class Condition_00490230 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_00490230() { satisfied = celebrated = 0; }
    virtual int FUN_0048eb40() = 0;              // IsSatisfied
    virtual void FUN_0048ea10();                 // Slot1
    virtual void FUN_0048ea20();                 // Slot2
    virtual void FUN_0048ea30();                 // Slot3
    virtual void FUN_0048eb80(void* file) = 0;   // Save
    virtual void FUN_0048ebc0(void* file) = 0;   // Load
};

// VictoryCondition_DestroyAllUnits.
class Class_0048eb40 : public Condition_00490230 {
public:
    virtual int FUN_0048eb40();
    virtual void FUN_0048eb80(void* file);
    virtual void FUN_0048ebc0(void* file);
};

#pragma pack(push, 1)
struct Player_00490230 {                // 0x14b bytes
    char unknown_0[0x108];
    unsigned char allied[10];            // +0x108, one entry per other team
    char unknown_112[0x144 - 0x112];
    short count;                         // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game_00490230 {
    char unknown_0[0x1b63];
    Player_00490230 players[10];          // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;                 // +0x2a42
    char unknown_2a43[0x391e9 - 0x2a43];
    Class_00435100* mode;                 // +0x391e9
};
#pragma pack(pop)

extern Game_00490230* g_game;

class Class_0048ff40 {
public:
    Condition_00490230* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_00490230* defeat[16];      // +0x44
    int defeatCount;                     // +0x84
    int active;                          // +0x88, set to 1 by Class_0048df90

    // 0x48fed0, written in the class so that /Ob2 inlines it here.
    inline int FUN_0048fed0()
    {
        if (victoryCount == 0) {
            victory[victoryCount] = new Class_0048eb40;
            victoryCount++;
        }
        for (int i = 0; i < victoryCount; i++) {
            if (!victory[i]->FUN_0048eb40())
                return 0;
        }
        return 1;
    }

    int FUN_00490230();
};

// FUNCTION: 0x490230
int Class_0048ff40::FUN_00490230()
{
    if (active == 0)
        return 0;
    switch (g_game->mode->FUN_00435100()) {
    case 1:
        return FUN_0048fed0();
    case 2: {
        unsigned char player = g_game->player;
        Player_00490230* me = &g_game->players[player];
        for (unsigned char i = 0; i < 10; i++) {
            if (i == g_game->player)
                continue;
            if (me->allied[i])
                continue;
            if (g_game->players[i].count != 0)
                return 0;
        }
        return 1;
    }
    case 3:
        return ((Class_00490080*)this)->FUN_00490080();
    }
    return 0;
}
