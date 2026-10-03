// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5, finished by GPT-6. Names are provisional.
// #5403 Codex retry: 94.9% remains best. The four early flag stores still use
// the wrong OR operand order, and the required symbol-id range needs an
// artificial declaration prefix that cannot be committed.
// #5421 Codex retry: re-confirmed 94.9%; the stored bitfield operands and
// C2 symbol-id window remain unchanged.
// #5348 Claude Opus 5.5 (94.9% from a scratch path, 93.2% from src/; kept):
// a block-scope `extern g_game` does not move the deciding id. Next to the
// file-scope declaration it is the same symbol (byte-identical). Without one,
// each block-scope extern is a separate symbol, so GetUnit's g_game (its own
// extern, or a file-scope one after the function with GetUnit defined last)
// no longer shares the function's load: 66.7% with /Gi, 30.7% without,
// extern first or after the locals alike. Writing GetUnit's body into the
// function instead is 78.0% with /Gi. The window needs this function's own
// id near 64543, about 32000 above what one declaration can move.
// #5330 Codex retry: re-confirmed 94.9%; prior symbol-id and bitfield-order
// sweeps found no new source shape.
// #5300 Codex retry: re-confirmed 94.9%; prior symbol-id and bitfield-order
// sweeps found no new source shape.
// #5258 Codex retry: re-confirmed the existing 94.9% /Gi version. The
// remaining bitfield OR operand order and symbol-id window were already
// tested across source, header and symbol-count variants; no new shape kept.
// FLAGS: /Gi
// Symbol ids, read with `c2prio.py --symbols` (docs/c2-regalloc.md, "Symbol
// ids"; Claude Opus 5.5): without /Gi the match follows this function's own
// symbol id (it is 32641 with <vector> <windows.h>). Declarations between
// g_game and the function move it like those before g_game; g_game's id and
// declarations after the function (the file total) change nothing. It
// MATCHES for ids 64543..64575, 64607..64639, 64671..64703, 64735..64767
// and 64799..64819, and a sweep of the whole 16-bit range in steps of 16
// found no other window. That needs 64307 to 64583 symbols of headers in
// front, where <vector> <windows.h> give 32405. Nearest real sets: what TA's
// imports suggest (<windows.h> <ddraw.h> <dsound.h> <dplay.h> <shlobj.h>
// <imagehlp.h>, six CRT headers, <vector> <list> <map> <algorithm>
// <string>) gives 41247; every DirectX, shell, CRT, STL and old iostream
// header together (docs) gives 52091, function id 52327, 89.0% without /Gi,
// still 12216 short. Only kitchen-sink sets with MAPI, LAN Manager, TAPI and
// ODBC headers get there, so the /Gi version stays.
// #5201 Claude Opus 5.5 (no gain committed; 93.2% from src/unsorted, 94.9%
// from a scratch path). The source shape is right and this TU is probably
// not /Gi: with the FLAGS line removed (32.7% as it stands) and N unused
// `extern int` declarations in front of `g_game`, this exact file MATCHES
// for N = 31904..31935, 31968..31999, 32032..32063, 32096..32127 and
// 32160..32179 (a period-64 pattern; checked from two directories), and
// scores 86% to 98.8% for N from about 4096 to 32880. So it is a symbol
// numbering state (symbol ids behave mod 65536 here, see 0x424c00), which
// /Gi only approximates. No real header set reaches it yet: headers.py
// --cpp (1536 sets) and 600 random sets of 1-3 C++ plus 0-4 C headers on
// top of <vector> <windows.h> give 88.0% at best. With <stdio.h>
// <string.h> instead the window is at N = 63744..64000 (so about 1600
// fewer symbols than those two headers would do); with no headers and
// sprintf/strncpy declared by hand the file is already about 750 past it
// (44.3%); <stdio.h> <windows.h> puts the band end near N = 36000. Dummy
// declarations are not to be committed, so the /Gi version stays.
// The tail difference under /Gi is the operand order
// of each bitfield store's OR (Ghidra shows `old & mask | v << n` for
// cloak, onOff, canAttack and canMove, `v << n | old` for the rest; ours is
// `v << n | old` for all). Byte-identical: canMove/canAttack set through an
// inline Set(int*) or Set(int&), an unused `&canMove`, and declaring
// cloak/onOff/canMove/canAttack (or all of the first block) before
// `g_game->orders.refresh = 1`. c2prio: every flag splits, the tail pieces
// are coloured eax.
// #5167 Claude Opus 5.5 (no gain; 2 diff hunks from a long path, the four
// tail bit writes): c2prio --trace shows all eleven flags split when
// fireOrder takes ebx, and every flag's tail piece is coloured eax (priority
// 340). In a test file, a bitfield store whose value is a plain memory load
// (a global array element) compiles to the original's first-four form
// (`and old,mask; shl val,n; or old,val`, stored from the old word's
// register), while a value that is a register candidate (a parameter)
// compiles to ours (`or val,old`). So in the original cloak, onOff,
// canAttack and canMove most likely have no register piece in the tail,
// while canDefend onwards do; no spelling found makes that happen.
// Tried: explicit `& 3` / `& 1` on the four values (folded, no change), an
// early-return GetUnit (1513 bytes, worse), the 256-set header sweep from
// a long path (flat at 94.9).
// #5134 Claude Opus 5.5: 60.8% to 93.2-94.9%, same 1512 bytes as the original.
// What moved it: /Gi (the default-flags build of this file is 32.7%), plus
// first/count left uninitialised at declaration and zeroed at the top of the
// else (the original's sink at 0x41b3a6, which every note below measured
// without /Gi), plus `cloak` declared before `onOff` (prologue store order).
// Under /Gi 10 of the 11 matched neighbours 0x41a920..0x41bcd0 still match;
// 0x41ace0 drops to 98.6% on one SIB base swap ([ebp+ecx] vs [ecx+ebp]).
// What still differs:
//  - first/count frame slots (0x1c/0x20). Both have the same reference counts,
//    and the tie is broken by the build location: with this exact file,
//    check.py gives 94.9% (42 diff lines, only the tail below) when the
//    object/pdb paths fall one way and 93.2% when they fall the other (it is
//    93.2% from this worktree's src/unsorted). Same instructions otherwise.
//    Writing `count = 0; first = 0;` instead flips which locations win
//    (93.4% here). The original's store order is first then count, so that
//    is what is kept. No spelling tried makes the tie disappear.
//  - the tail bit writes for cloak, onOff, canAttack and canMove: the
//    original ORs into the old word's register (`and edx,mask; shl eax,n;
//    or edx,eax; mov [..],dx`, so the next value load is hoisted above the
//    store); ours ORs into the value's (`or eax,edx; mov [..],ax`). From
//    canDefend on both use `or eax,edx`. Those four are the first flag
//    variables the loop writes, which looks like a register-candidate cut
//    (the original reloads them as split register variables in the tail) and
//    not like a spelling. Flat for this: dummy extern/struct counts 0..1000
//    (no effect at all under /Gi), source path length, every declaration
//    order of the flags, unsigned/short/char value and bitfield types, casts,
//    hand-written mask stores, inline setter helpers, chained zero
//    initialisation, inlining the real 0x41b2a0 (worse: its index test is not
//    folded), deleting each loop statement in turn, and a 20 minute permuter
//    run (no gain).
// The notes below predate /Gi.
// #4099 deepseek-v4.1-flash (10 min): flat at 60.8% / 1533 bytes. Swapping the
// cloak and u declaration order (onOff, cloak, u) is byte-identical, so the
// first=0x10 / cloak=0x14 / onOff=0x18 slot map is not steered by that pair.
// Residual unchanged: prologue first=0/count=0 stores force first to 0x10 (the
// original sinks them to 0x1c/0x20), the ESI/EBP zero split (ours test ax,ax
// where the original has xor esi,esi plus cmp ax,si) and the tail bit writes
// accumulating in EAX/EDX instead of EBX/EDX.

// #4008 deepseek-v4.1-flash (10 min): 60.8% / 1533 bytes. Moving the fireOrder=4 / moveOrder=4 declarations to the very top (before onOff) put the ebx=4 / edx=4 pair ahead of the nine zero stores and lifted 60.6 to 60.8. cloak/onOff swap, a live zero16 unitIndex compare, and first/count before the 4-inits are flat at 60.6. Residual: count stays spilled to [esp+0x20] where the original keeps it in ebx, first sits in a slot where the original uses ebp, and the unitIndex test is test ax,ax where the original has xor esi,esi / cmp ax,si.
// #3595 deepseek-v4.1-flash (10 min): re-baselined 59.6%, 1533 bytes. Flat at
// 59.6: first/count declared first, an unsigned short zero16 local routing the
// unitIndex compare, and the nine flag initialisers moved before onOff. Ours
// still emits test ax,ax where the original has xor esi,esi / cmp ax,si, and
// keeps the ESI/EBP zero split plus the first/count register-vs-memory choice.

// #2847 retry by GPT-6.1-sol: the six checker attempts preserved the 59.6%
// best. Masked-word stores and XOR assignment reduced the score; live-zero,
// first/count stack homes, and tail bit-write registers still differ.
// Partial: 59.6% (was 55.8), 1533 bytes versus 1512. <vector>/<windows.h> plus
// declaration order onOff,u,cloak,first,count,flags recovers the 0x240 frame.
// Original slot map: onOff=0x10, loop u=0x14, cloak=0x18, first=0x1c, count=0x20,
// canMove=0x24, canLoad=0x28, canReclaim=0x2c, canAttack=0x30, canCapture=0x34,
// canDefend=0x38, canBlast=0x3c, canStop=0x40, canPatrol=0x44, canRepair=EBP,
// fireOrder=EBX, moveOrder=EDX. Ours still puts first=0x10, cloak=0x14,
// onOff=0x18, u=0x1c, and spills canRepair to stack instead of EBP. MSVC sank
// the original first/count zero-inits into the else branch (offsets 0x1c/0x20);
// ours emits them in the prologue, which is why first takes 0x10. The
// unitIndex test also differs (original reuses esi=0 with cmp ax,si). A 768-set
// header sweep and 80 local/type/lifetime variants were tried by earlier models.
//
// deepseek-v4.1-flash, second pass: the real lever is that the original keeps a
// SECOND live zero in ESI (xor esi,esi at 0x41b33e) used for cmp ax,si, cmp
// word [..+0xa6],si, mov word [..+0x37e9c],si and the else-branch first=0 /
// count=0 stores, while EBP=0 serves only the nine flag locals. Ours reuses one
// zero (EBP) for both, so EBP is live through the if-branch, which blocks EBP
// for `first`; the original keeps first in EBP and count in EBX on that path
// (mov ebp,eax / mov ebx,1) and only spills them in the else loop. Verified
// experiments: every permutation of the onOff/u/cloak/first/count and flag
// declaration order compiles byte-identically (59.6%), so declarations are not
// the cause; the N-declarations sweep 0..66 and all 128 headers.py sets are
// flat at 59.6%; making the first=0/count=0 initialisers absent and zeroing
// them in the else (the sink the original shows at 0x41b3a6) instead makes the
// nine flags spill and drops to 32.8%. The remaining diffs are the ESI/EBP zero
// split, the first/count register-vs-memory choice, and the tail bit-write
// sequence reusing EAX instead of EBX.
//
// deepseek-v4.1-flash, third pass: reconfirmed all of the above and pinned the
// failure mode. Any source shape that puts the first=0/count=0 stores in the
// else (vA/vB2/vA_late, first/count declared early or after the flags) makes the
// allocator pick EBP = &players[i] and EAX = the zero constant instead of
// EBP = zero, ESI = zero, and the frame becomes 0x244/1496 bytes at 32.8%. The
// good 0x240 frame needs EBP to stay the flags' zero only until the else, with a
// second zero in ESI, i.e. canRepair in EBP during the loop (the original's
// mov ebp,esi at 0x41b4f9 and shl ebp,0xf at 0x41b56e). That choice is not
// reachable from the source: flag declaration order (canRepair first/last),
// onOff/u/cloak/first/count permutations and unsigned variants are all
// byte-identical at 59.6%; while+continue is 26.2%; declaring first/count after
// player is 56.6-57.3%. Keeping the initializers at declaration (prologue
// stores) is the only shape that reaches 59.6%.
//
// deepseek-v4.1, fourth pass: still 59.6%, 1533 bytes versus 1512. What still
// differs: the original keeps a second live zero in ESI created at 0x41b33e
// and uses it for cmp ax,si at 0x41b344, cmp word [edx+ecx*8+0xa6],si at
// 0x41b37b, mov word [edi+0x37e9c],si at 0x41b394 and the else-branch
// first=0/count=0 stores at 0x41b3a6/0x41b3aa, while EBP=0 serves only the
// nine flag stores; on the if-branch the original keeps first in EBP and count
// in EBX (mov ebp,eax / mov ebx,1) instead of spilling them to
// [esp+0x1c]/[esp+0x20], and its tail bit writes accumulate in EDX/EBX and
// store with dx/bx/ax where ours always accumulate in EAX/EDX and store ax/dx.
// New experiments this pass, all byte-identical to the 59.6% output: moving
// the first=0/count=0 declarations to after the nine flag initialisers, the
// condition spelled `if (g_game->unitIndex)` instead of `!= 0`, and routing
// the compare through a live `int zero = 0;` local. Sinking first=0/count=0
// into the else (the original's position, right before `last = unitsEnd`)
// reproduces the known 32.8% shape with EBP = &players[i] and a 0x244 frame,
// so the sink is not reachable without losing the 0x240 frame. Remaining work
// needs allocator state the source cannot steer: the ESI/EBP zero split and
// the first/count register homes.
//
// deepseek-v4.1-flash, fifth pass (retry at 59.6%): fresh experiments confirm
// the ceiling and find no new lever. Sinking only count, only first, or both
// (with or without an unsigned short zero local replacing the unitIndex
// compare/store) all stay at 32.8% and 0x244/1496 bytes. Applying the short
// zero to the compare alone (v3), swapping the first/count declaration order,
// and hoisting first/count above onOff are all byte-identical to the 59.6%
// output. Declaring first/count after player or assigning first=0/count=0 just
// before the branch is 57.3%, 1520 bytes. Only the prologue store from the
// declaration initialiser reaches 0x240/1533. What still differs is unchanged:
// ESI/EBP zero split, first/count homes (ours first=0x10, onOff=0x18,
// cloak=0x14, u=0x1c; original onOff=0x10, u=0x14, cloak=0x18, first=0x1c),
// and the tail bit writes using EAX/EBX where the original uses EBX/EDX.
//
// deepseek-v4.1-flash, sixth pass (retry at 59.6%, 3 check runs): the diff was
// re-read in full. Two facts not in the notes above, both from the prologue
// byte order. (1) Ours keeps the declaration initialisers in the prologue,
// so `first` takes slot 0x10 and `onOff` shifts to 0x14, while the original
// never stores first/count there (`onOff`=0x10, `u`=0x14, `first`=0x1c,
// `count`=0x20): the 0x10/0x14 swap is a direct consequence of the prologue
// store, so any variant that moves it out also moves onOff back to 0x10, the
// 32.8% shape the earlier passes measured. (2) The original's nine flag-zero
// stores are ordered canMove(0x24), canAttack(0x30), canDefend(0x38),
// canPatrol(0x44), canLoad(0x28), canCapture(0x34), canReclaim(0x2c),
// canBlast(0x3c), canStop(0x40), i.e. the three groups (canMove,canLoad,
// canReclaim), (canAttack,canCapture,canDefend), (canBlast,canStop,canPatrol)
// interleaved, which is a different order from both the loop's bit tests and
// the current declaration order, so it may be a scheduler artefact rather
// than evidence about the original declarations. No source shape tried here
// changed the ESI/EBP zero split, so the ceiling stands.
//
// deepseek-v4.1-flash, seventh pass (#3639, retry at 59.6%, 2 check runs): the
// nine flag declarations are now in the original store order (canMove,
// canAttack, canDefend, canPatrol, canLoad, canCapture, canReclaim, canBlast,
// canStop), which lifts the score to 60.6% (1533 bytes, 0x240 frame) and makes
// the nine zero stores match the original sequence byte for byte. This shows
// the original declarations were in store order, not slot order. What still
// differs is unchanged: the ESI/EBP zero split (ours test ax,ax where the
// original has xor esi,esi / cmp ax,si), the first/count homes (ours first=0x10
// from the prologue initialiser stores, original first=0x1c), and the tail bit
// writes accumulating in EAX/EDX where the original uses EBX/EDX.

#include <vector>
#include <windows.h>

#pragma pack(push, 1)
struct UnitType_0041b2e0 {
    char unknown_0[0x22e];
    unsigned char field_22e;           // +0x22e
    char unknown_22f[0x241 - 0x22f];
    unsigned int flags_241;            // +0x241
    unsigned int canMoveOrder : 1;     // +0x245 bit 0
    unsigned int canFireOrder : 1;     // bit 1
    unsigned int canOnOff : 1;         // bit 2
    unsigned int canStop : 1;          // bit 3
    unsigned int canAttack : 1;        // bit 4
    unsigned int canDefend : 1;        // bit 5
    unsigned int canPatrol : 1;        // bit 6
    unsigned int canMove : 1;          // bit 7
    unsigned int canLoad : 1;          // bit 8
    unsigned int canRepair : 1;        // bit 9
    unsigned int canReclaim : 1;       // bit 10
    unsigned int bit11 : 1;
    unsigned int canCapture : 1;       // bit 12
    unsigned int canCloak : 1;         // bit 13
    unsigned int canBlast : 1;         // bit 14
    unsigned int bits15 : 17;
};

struct UnitFlags_0041b2e0 {
    unsigned int bits0 : 4;
    unsigned int selected : 1;         // bit 4
    unsigned int bits5 : 6;
    unsigned int cloak : 1;            // bit 11
    unsigned int bits12 : 6;
    unsigned int moveOrder : 2;        // bits 18-19
    unsigned int fireOrder : 2;        // bits 20-21
    unsigned int buildPage : 1;        // bit 22
    unsigned int page : 3;             // bits 23-25
    unsigned int bits26 : 6;
};

struct Unit_0041b2e0 {
    char unknown_0[0x92];
    UnitType_0041b2e0* type;           // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short typeIndex;          // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0x10e - 0xaa];
    unsigned char onOff;               // +0x10e
    char unknown_10f;
    UnitFlags_0041b2e0 flags;          // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0041b2e0 {
    char unknown_0[0x67];
    Unit_0041b2e0* unitsBegin;         // +0x67
    Unit_0041b2e0* unitsEnd;           // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct BuildType_0041b2e0 {
    char unknown_0[0x20];
    char name[0x229];                  // +0x20
};

struct Orders_0041b2e0 {
    unsigned short unknown_0 : 1;      // +0x37ebe
    unsigned short refresh : 1;        // bit 1
    unsigned short unknown_2 : 10;
    unsigned short fireOrder : 3;      // bits 12-14
    unsigned short unknown_15 : 1;
    unsigned short moveOrder : 3;      // +0x37ec0 bits 0-2
    unsigned short cloak : 2;          // bits 3-4
    unsigned short onOff : 2;          // bits 5-6
    unsigned short canMove : 1;        // bit 7
    unsigned short canStop : 1;        // bit 8
    unsigned short canAttack : 1;      // bit 9
    unsigned short canDefend : 1;      // bit 10
    unsigned short canPatrol : 1;      // bit 11
    unsigned short canLoad : 1;        // bit 12
    unsigned short canReclaim : 1;     // bit 13
    unsigned short canCapture : 1;     // bit 14
    unsigned short canRepair : 1;      // bit 15
    unsigned short canBlast : 1;       // +0x37ec2 bit 0
    unsigned short unknown_1 : 15;
};

struct Menu_0041b2e0 {
    char unknown_0[0x10];
};

struct Game_0041b2e0 {
    char unknown_0[0x519];
    Menu_0041b2e0 menu;                // +0x519
    char unknown_529[0x1b63 - 0x529];
    Player_0041b2e0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit_0041b2e0* units;              // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    BuildType_0041b2e0* buildTypes;    // +0x1439b
    char unknown_1439f[0x37e9c - 0x1439f];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    Orders_0041b2e0 orders;            // +0x37ebe
};
#pragma pack(pop)

extern Game_0041b2e0* g_game;

int __stdcall FUN_00491d70(int force);
int __stdcall FUN_004ab060(Menu_0041b2e0* menu, const char* name);
void __stdcall FUN_0041ace0(Unit_0041b2e0* unit, char* guiName, int page);
void __stdcall FUN_0041b0f0(Unit_0041b2e0* unit);

static inline Unit_0041b2e0* GetUnit(unsigned short index)
{
    Unit_0041b2e0* u = &g_game->units[index];
    if (u->typeIndex == 0)
        u = 0;
    return u;
}

// FUNCTION: 0x41b2e0
void FUN_0041b2e0()
{
    g_game->orders.refresh = 1;
    int fireOrder = 4;
    int moveOrder = 4;
    int cloak = 3;
    Unit_0041b2e0* u;
    int onOff = 3;
    Unit_0041b2e0* first;
    int count;
    int canMove = 0;
    int canAttack = 0;
    int canDefend = 0;
    int canPatrol = 0;
    int canLoad = 0;
    int canCapture = 0;
    int canReclaim = 0;
    int canBlast = 0;
    int canStop = 0;
    int canRepair = 0;
    Player_0041b2e0* player = &g_game->players[g_game->localPlayer];
    if (g_game->unitIndex != 0) {
        count = 1;
        first = GetUnit(g_game->unitIndex);
        if (first == 0) {
            g_game->unitIndex = 0;
            return;
        }
    } else {
        first = 0;
        count = 0;
        Unit_0041b2e0* last = player->unitsEnd;
        for (u = player->unitsBegin; u <= last; u++) {
            if (u->typeIndex == 0)
                continue;
            UnitFlags_0041b2e0 flags = u->flags;
            if (!flags.selected)
                continue;
            if (first == 0)
                first = u;
            UnitType_0041b2e0* type = u->type;
            if (type->canFireOrder) {
                if (fireOrder == 4)
                    fireOrder = flags.fireOrder;
                else if (fireOrder != flags.fireOrder)
                    fireOrder = 3;
            }
            if (type->canMoveOrder) {
                if (moveOrder == 4)
                    moveOrder = flags.moveOrder;
                else if (moveOrder != flags.moveOrder)
                    moveOrder = 3;
            }
            if (type->canOnOff) {
                if (onOff == 3)
                    onOff = u->onOff & 1;
                else if (onOff != (u->onOff & 1))
                    onOff = 2;
            }
            if (type->canCloak) {
                if (cloak == 3)
                    cloak = flags.cloak;
                else
                    cloak = 2;
            }
            if (type->canMove)
                canMove = 1;
            if (type->canStop)
                canStop = 1;
            if (type->canAttack)
                canAttack = 1;
            if (type->canDefend)
                canDefend = 1;
            if (type->canPatrol)
                canPatrol = 1;
            if (type->canLoad)
                canLoad = 1;
            if (type->canRepair)
                canRepair = 1;
            if (type->canCapture)
                canCapture = 1;
            if (type->canReclaim)
                canReclaim = 1;
            if (type->canBlast)
                canBlast = 1;
            count++;
        }
        g_game->orders.fireOrder = fireOrder;
        g_game->orders.moveOrder = moveOrder;
        g_game->orders.cloak = cloak;
        g_game->orders.onOff = onOff;
        g_game->orders.canStop = canStop;
        g_game->orders.canAttack = canAttack;
        g_game->orders.canMove = canMove;
        g_game->orders.canDefend = canDefend;
        g_game->orders.canPatrol = canPatrol;
        g_game->orders.canLoad = canLoad;
        g_game->orders.canRepair = canRepair;
        g_game->orders.canReclaim = canReclaim;
        g_game->orders.canCapture = canCapture;
        g_game->orders.canBlast = canBlast;
    }
    if (count == 0) {
        FUN_00491d70(0);
        g_game->orders.refresh = 0;
    } else if (count == 1 && first->type->field_22e) {
        int page = first->flags.buildPage ? first->flags.page : 0;
        if (page > 0 || (first->type->flags_241 & 0x80000000)) {
            char name[256];
            char gui[256];
            strncpy(name, g_game->buildTypes[first->typeIndex].name, 0x20);
            name[0x1f] = 0;
            sprintf(gui, "%s%d.GUI", name, page);
            if ((FUN_004ab060(&g_game->menu, gui) == 0 || g_game->unitIndex != first->id)
                && FUN_00491d70(0))
                FUN_0041ace0(first, gui, page);
            g_game->orders.refresh = 0;
        }
    }
    if (g_game->orders.refresh) {
        if (FUN_00491d70(0)) {
            if (count == 1) {
                FUN_0041b0f0(first);
                return;
            }
            FUN_0041b0f0(0);
        }
    }
}
