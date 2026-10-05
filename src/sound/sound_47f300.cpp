// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by space-bunny-free. finished by Claude Sonnet 5.5. Names are provisional.
// MATCH (775 of 775 bytes). Plays the sound at soundIds[index] when the
// position is visible to the local player: explored (fog) map when
// g_game->flags_14281 has bit 1 set, the shared per-player visibility mask
// otherwise. Sends the 0x13 packet first when param_3 is set, and picks the near
// (-585) or far (-1585) variant depending on whether the position is inside the
// screen rectangle.
//
// What made it match (the last 0.6%, after eleven passes of address spellings):
// the original builds the player pointer with `lea eax,[ebp+ecx]` (g_game + pi)
// followed by `lea eax,[eax+edx*2+0x1b63]`, where every spelling written inline
// in the function gave `mov eax,ebp / add eax,ecx`. Taking the address through
// an INLINE MEMBER of the game struct, `Player* Current() { return
// &players[playerIndex]; }` called as `g_game->Current()`, gives the lea while
// keeping `int pi` (and so the `xor ecx,ecx / mov cl,[..]` load). The two
// pointer locals (player for the bounds, player2 for the index expression) and
// the `MapContains` helper negated at the call site are from earlier passes.
#include <windows.h>
// Plays the sound at soundIds[index] when the position is visible to the local
// player: explored (fog) map when g_game->flags_14281 has bit 1 set, the shared
// per-player visibility mask otherwise. Sends the 0x13 packet first when
// param_3 is set, and picks the near (-585) or far (-1585) variant depending on
// whether the position is inside the screen rectangle.

class Class_004cf570 {
public:
    int PlaySampleSet(int a, int b, void* c);
};

class Class_004cfea0 {
public:
    int Is3DEnabled();
};

class Class_004cfeb0 {
public:
    void Set3DDistances(float a, float b);
};

struct Vector3_0047f300 {
    int x, y, z;
};

union Pos_0047f300 {
    struct {
        short xFrac;                   // +0x0
        short x;                       // +0x2
        short yFrac;                   // +0x4
        short y;                       // +0x6
        short zFrac;                   // +0x8
        short z;                       // +0xa
    };
    struct {
        int xVal;                      // +0x0
        int yVal;                      // +0x4
        int zVal;                      // +0x8
    };
};

#pragma pack(push, 1)
struct Packet_0047f300 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int index;                         // +0x2
    Pos_0047f300 pos;                  // +0x6
};

struct Player_0047f300 {
    char unknown_0[0x7c];
    unsigned char* explored;           // +0x7c
    unsigned int exploredWidth;        // +0x80
    unsigned int exploredHeight;       // +0x84
    char unknown_88[0x14b - 0x88];
};

struct Game {
    char unknown_0[0x10];
    Class_004cf570* sound;             // +0x10
    char unknown_14[0x1b63 - 0x14];
    Player_0047f300 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                         // +0x14233
    int height;                        // +0x14237
    int screenTilesX;                  // +0x1423b
    int screenTilesY;                  // +0x1423f
    char unknown_14243[0x14273 - 0x14243];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags_14281;         // +0x14281
    char unknown_14282[0x1431f - 0x14282];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x33a13 - 0x14327];
    int soundIds[1];                   // +0x33a13
    char unknown_33a17[0x37f0c - 0x33a17];
    int field_37f0c;                   // +0x37f0c
    char unknown_37f10[0x37f19 - 0x37f10];
    unsigned char flags_37f19;         // +0x37f19

    Player_0047f300* Current() { return &players[playerIndex]; }
    Player_0047f300* Current2() { int i = playerIndex; return &players[i]; }
};
#pragma pack(pop)

extern Game* g_game;
extern int g_noDirectSound;
extern int g_useWindowsSound;

int __cdecl GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall PlaySoundByIndex(int index, int param_2);
struct Cell_0047f300;
Cell_0047f300* __stdcall FUN_00481550(int x, int y);

// 86.5% (775 bytes, same size as the original). Everything outside the
// visibility block is byte exact, and the whole diff is ONE cause: the player
// pointer.
//
// THE CAUSE. The original computes the player pointer in the pre-branch block
// and keeps it in EAX across the branch:
//     mov eax,ecx / shl eax,5 / add eax,ecx / lea edx,[eax+eax*4]
//     lea eax,[ebp+ecx] / lea eax,[eax+edx*2+0x1b63]
// and both arms then use EAX for it (and reuse EAX as the index accumulator,
// which is why the original MATERIALISES its second width read: `mov
// edi,[eax+0x80] / imul edi,ecx`, because a memory operand whose base is the
// destination cannot be folded). All four callee-saved registers are taken
// (esi=pos, ebx=sound, ebp=g_game, ecx=pi), so the arm roles in the original
// are exactly the preference order: ptr=EAX, ty=ECX, tx=EDX, w=EDI. This file
// gets ptr=EDI and the arm roles rotate with it. Nothing else in the two arms
// is wrong: instruction for instruction they are the original's, only renamed.
//
// The allocator's choice for that pointer is what decides everything, and it
// has two attractors that no spelling reached eax past (both measured with
// check.py --sym, so free, in build/scratch/0x47f300/):
//   * a plain `&&` chain in the arms  -> ptr in EDI, arms rotated  (v1, here)
//   * MapSize::Contains + a nested if -> ptr in EDX, arm SHAPES exact (v10,
//     82.3% overall). Contains also fixes the mask arm's `jb` to its body and
//     stops the two fail blocks being tail merged, exactly as in the original.
// So the Contains spelling is right and is one step short. What is missing is
// whatever promotes the pointer from EDX to EAX.
//
// Tried and rejected, all free-scored, none moved the pointer to eax:
//   int/unsigned/short pi, no pi local (index read twice, as in 0x408090),
//   `g_game->players + pi`, `&g_game->players[pi]`, an explicit
//   `(char*)g_game + 0x1b63 + pi*0x14b` with every parenthesisation of the
//   three-term sum, a static inline LocalPlayer(pi) wrapper, the pointer
//   computed separately in each arm (the address is then NOT hoisted, 61.8%),
//   one inline IsVisible() wrapping both arms (69.3%), the fog arm as a
//   ternary, the mask arm as a nested if instead of a guard, `unsigned int tx`,
//   the nested if without Contains (back to EDI), hoisting visibilityMask.
//   Declaring ty before tx in the arms costs 4 points and 20 bytes.
//
// The v10 (Contains + nested if) shape is the one to build on: it is one
// register away in the pre-branch block and byte exact in the arm bodies. Its
// two other costs are known and small: the tail's two loads of width/height
// come out swapped (`mov edx,[ecx+0x14237]` where the original has
// `mov edx,[ecx+0x14233]`), worth swapping the two terms of that sum to try,
// and its arms fold the second width read into the imul, which follows from
// the pointer register.
//
// The p vector store order was worth +0.4: writing p.z before p.y = 0 lets the
// compiler schedule the p.y store where the original has it.

// Addendum (deepseek-v4.1-flash, second pass): re-swept the two attractors
// with 80 more free-scored variants, all changes confined to the visibility
// block:
//   - 30 combinations of fog and mask arm shapes (the Contains/nested-if
//     helpers, and direct nested-if, guard, ternary and && spellings);
//   - pi as int, unsigned and unsigned char; no-pi (player index read twice);
//   - player as pointer, g_game->players + pi, a reference, an inline
//     GetPlayer accessor, and declaration followed by assignment;
//   - the flags test with == 2, != 0, a bare bit test, a char/int/bool local.
// Result: the pointer's register is a two-state attractor. With the Contains
// helper arms (the original's arm shapes, v10) it is always EDX; with direct
// arms it is always EDI. Not one of the 80 variants produced EAX, so the
// pre-branch block remains exactly one register off and nothing else in the
// function moves. Every new variant scored at or below v10's 82.3 percent,
// well under this file's 86.5, so the file is unchanged apart from this note.
// The earlier conclusion stands: what is still missing is the upstream cause
// that promotes the pointer from EDX/EDI to EAX.

// Addendum (space-bunny-free, third pass). Free scratch scoring
// (check.py --sym, all in build/scratch/0x47f300/). Nothing beat 86.5%, but two
// of the results narrow the cause:
//   * `int pi` instead of `unsigned char pi` makes the PRE-BRANCH BLOCK nearly
//     exact, including the zero-extension idiom:
//         xor ecx,ecx / mov cl,[ebp+0x2a43] / mov eax,ecx / shl eax,5
//         add eax,ecx / lea edx,[eax+eax*4]
//     which is what the unsigned char spelling only reaches after a spill and
//     reload (`mov [esp+0x30],cl / mov ecx,[esp+0x30] / and ecx,0xff`) that
//     the original does not have. So `pi` really is an int. But it still puts
//     the pointer in EDI, splits `lea eax,[ebp+ecx]` into `mov eax,ebp / add
//     eax,ecx`, and moves every block boundary, and the whole function drops
//     to 79.7% (763 bytes). int/unsigned/short pi and `char pi` all do the
//     same. The pointer register, not the spill, is what costs the 7 points.
//   * The original re-reads the width (`mov edi,[eax+0x80]` twice in the fog
//     arm, `mov eax,[eax+0x80]` in the mask arm) where every spelling here
//     folds the second read into the `imul`. A local `unsigned int w =
//     player->exploredWidth` used only by `tx < w` does NOT break the load
//     CSE: the index expression still reads the field and MSVC merges them
//     (86.5%, 775 bytes, byte-identical output to the no-w version). Same for
//     an `int w`, and for hoisting `exploredHeight` into `h`. Per the guide's
//     item 18, breaking this needs two structurally different expression
//     trees or two separate pointer locals; neither was found.
//   * Also measured, all 86.5% or worse and none moving the pointer off EDI:
//     `bool vis`, a local for the mask word, `>> pi & 1` instead of `& (1<<pi)`
//     (77.7%), `& ((1<<pi)&0xffff)`, `g_game->players + pi`, swapping the two
//     arms' order, and `g_game->flags_14281 & 2` as the test (66.5%, the
//     original compares against 2 explicitly, so keep `== 2`).
//   * `unsigned int tx` (80.4%), `int pi` + `unsigned int tx` (80.6%), and
//     writing the test as `!= 2` with the fog arm as the taken one (86.5%,
//     same output): none of them move the pointer.
// The two things that must both be true and are not yet true together: `pi`
// as an int (no spill, `xor ecx,ecx` kept) AND the pointer in EAX. The two
// spellings that give the right arm shapes put the pointer in EDX, and the two
// that give the right pre-branch block put it in EDI.

// Addendum (space-bunny-free, fourth pass). Baseline re-confirmed with a real
// check.py run: 86.5%, 775 bytes, same size as the original. Free scratch
// scoring (check.py --sym) of 40 more variants, all in build/scratch/0x47f300/;
// nothing beat the file, so the file is unchanged apart from this note.
// New results worth recording:
//   * The Contains spelling re-measured, this time with the players[] array
//     given its real 0x14b stride (a Map type without the 0x88..0x14b tail
//     silently shrinks players[10] and shifts every g_game offset, which is
//     what makes a hand-rolled Map score 59 percent): 81.3% with Contains in
//     both arms, 81.3% in the mask arm only, 78.9% in the fog arm only, all
//     765-769 bytes. Same 82-ish plateau as before, and in every one of them
//     the player pointer is in EDX, never EAX. Without Contains it is always
//     EDI. So Contains and EAX are two different attractors, as measured.
//   * The Map type WITHOUT Contains, i.e. `map->size.width` /
//     `map->size.height` / `map->fog` read directly, scores 85.7% at the
//     original's 775 bytes: one point under the file, and it is the closest
//     anyone has got to the original with a nested-struct spelling.
//   * Item 18's "two identical pointer values via a second local" does NOT
//     break the width load CSE here: a second `Player* p2 = player;` used for
//     the two width reads compiles to byte-identical output to the file
//     (86.5%, 775 bytes). The two pointer live ranges are coalesced into one
//     register, so the base is the same register and the two loads merge again.
//   * A named `unsigned int w` local does not break the CSE either, and which
//     arm it is in changes the score: fog arm only 81.3% (771 bytes), mask arm
//     only 83.2% (775 bytes), both arms 84.4% (775 bytes), so the mask arm's
//     named local is the closest of the three but still 3 points under.
//   * A named `unsigned char* fog` local in the fog arm collapses the arm:
//     65.4%, 779 bytes. Do not try it.
//   * `int vis` as a `char` or a `bool` is 79.1% (772 bytes): the arms are
//     fine but the 1/0 pair at the arm exit changes. Keep `int vis`.
//   * Writing either arm as a plain `&&` expression (`vis = a && b && c;`)
//     instead of an if/else is a large regression, 61.7% (776 bytes) for the
//     mask arm and for both arms: the mask arm's three-way `&&` tail-merges
//     the two fail blocks and the fog arm's loses its 1/0 pair.
//   * Hoisting tx/ty above the branch so both arms share them is 56.1%
//     (756 bytes). The arms genuinely compute them twice.
//   * No pi local (`&g_game->players[g_game->playerIndex]`, index read again
//     for the shift) is 80.2% (763 bytes); a second `Player* base` plus
//     `base[pi]` spellings are 70.5% (778 bytes). `unsigned char pi` in the
//     file is still the best of the index spellings.
//   * Free of effect, output byte-identical to the file (86.5%): declaring ty
//     before tx in both arms, the fog byte test without `!= 0`, the mask value
//     compared with `!= 0` instead of a ternary, the pointer spelled
//     `(Player*)((char*)g_game->players + pi * 0x14b)`, and the fog arm as one
//     `? 1 : 0` ternary. Note that ty-before-tx is 85.7%, NOT identical, so
//     the two are not the same variant.
//
// The one register that is still wrong is unchanged: the player pointer is
// built in EDI where the original builds it in EAX, and because it is the
// base register of the materialised width load the whole arm rotation follows
// from it. The original's arm roles (ptr, ty, tx, width) map exactly onto
// (EAX, ECX, EDX, EDI), which is the register preference order, so the
// original's allocator simply gave the pointer the top register and the
// widest live range in the block gets it. Nothing in the source shape moved
// that in 220-odd variants now.

// Addendum (deepseek-v4.1, fifth pass). Baseline re-confirmed: 86.5%, 775 bytes.
// New free-scored variants, all in build/scratch/0x47f300/, none beat the file:
//   * the two-inline-helper split of the matched 0x4658e0 pattern
//     (IsExplored/IsSeen taking the player pointer, `char vis`): with a second
//     read of g_game->playerIndex for the shift it is 76.2%, with the index
//     passed in as an argument 68.4%. Neither moves the pointer off EDI.
//   * four guard-style mask arms (nested if on the cheap first test, as in the
//     MATCHED 0x408090): int pi 66.5%, unsigned char pi 64.3%, short pi 66.3%,
//     and with the pointer spelled `g_game->players + pi` 64.3/66.5%. The
//     guard form in the mask arm alone is far below the file's plain `&&`
//     arms, so the file's arm shape stays.
// Conclusion unchanged: the only thing left is the player pointer's home
// register (EDI in every spelling measured so far, EAX in the original). The
// matched siblings that compute `&g_game->players[byte]` (0x401070) do put it
// in EAX, but they have no byte index that must stay live to the mask shift,
// and in this function that live ECX (plus ebp/esi/ebx held by g_game/pos/
// sound) leaves the allocator a different starting set.

// Addendum (deepseek-v4.1, sixth pass, 1 real check run). Baseline re-confirmed
// unchanged: 86.5%, 775 bytes, same size as the original. This pass re-read the
// original disassembly end to end and confirms the single remaining difference
// is the player pointer's home register: the original ends the address chain
// with `lea eax,[eax+edx*2+0x1b63]` and reads width/explored through EAX in
// both arms (re-loading [eax+0x80] three times), while every spelling here ends
// in EDI (or EDX with the Contains-style arms) and folds the width re-load into
// the imul. The pre-branch pi zero-extension is a second, smaller tell: the
// original is `xor ecx,ecx / mov cl,[ebp+0x2a43]` (int pi), which the int-pi
// variant reaches exactly, but int pi then splits `lea eax,[ebp+ecx]` into
// `mov eax,ebp / add eax,ecx` and drops the function to 79.7% (763 bytes). No
// new variant was found that puts the pointer in EAX; the file stays at its
// best 86.5%.

// Addendum (deepseek-v4.1-flash, ninth pass, retry). Baseline re-confirmed:
// 86.5%, 775 bytes, same size as the original. Two compiler-state levers the
// earlier passes had not run here are both FLAT:
//   * tools/headers.py over all 128 header sets: 86.5% at its best (windows.h,
//     stdio.h, math.h, ddraw.h and windows.h+stdio.h all 86.5%), so the
//     include set does not move the player pointer.
//   * the 0..400 unused `extern int dummyN;` declaration sweep at file scope
//     (step 4, scored with check.py --sym): only 85.7% and 86.5%, never
//     above the file, so no dummy count reaches the original's state either.
// Two more source shapes, both worse: dropping the `player` local and inlining
// `g_game->players[pi].exploredWidth` in both arms is 56.0% (804 bytes, the
// address is not CSE-hoisted); prepending the neighbouring 0x47f290 source in
// the same TU does not compile (duplicate class definitions). The single
// remaining cause is unchanged: the player pointer is built into EDI (EDX
// with Contains arms) where the original builds it into EAX, and every arm
// register rotation follows from it. The no-pi-local spelling is closest to
// the original in the pre-branch block (it reproduces `xor ecx,ecx; mov
// cl,[ebp+0x2a43]` and `lea eax,[ebp+ecx]` exactly, 763 bytes) but its final
// `lea` still lands in EDI and the byte-count shift costs more than the spill
// it removes, so it scores 80.2%.
static inline int MapContains(unsigned int w, unsigned int h, int tx, int ty)
{
    return tx < w && ty < h;
}
// FUNCTION: 0x47f300
int __stdcall PlaySoundAt(int index, Pos_0047f300* pos, int param_3)
{
    if (g_useWindowsSound)
        return PlaySoundByIndex(index, param_3);
    if (index == 0xffff)
        return 0;
    if (g_game->field_37f0c == 0)
        return 0;
    if ((g_game->flags_37f19 & 7) == 0)
        return 0;
    if (g_noDirectSound != 0)
        return 0;

    int sound = g_game->soundIds[index];

    if (param_3) {
        Packet_0047f300 packet;
        packet.type = 0x13;
        packet.flag = 1;
        packet.index = index;
        packet.pos = *pos;
        BroadcastPacket(GetLocalDpid(), &packet, 0x12);
    }

    if (FUN_00481550(pos->xVal / (1 << 20), pos->zVal / (1 << 20)) == 0)
        return 0;

    int pi = g_game->playerIndex;
    Player_0047f300* player = g_game->Current();
    Player_0047f300* player2 = g_game->Current();
    int vis;
    if ((g_game->flags_14281 & 2) == 2) {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (tx < player->exploredWidth && ty < player->exploredHeight
            && player->explored[player2->exploredWidth * ty + tx] != 0)
            vis = 1;
        else
            vis = 0;
    } else {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (!MapContains(player->exploredWidth, player->exploredHeight, tx, ty))
            vis = 0;
        else
            vis = (g_game->visibilityMask[player2->exploredWidth * ty + tx]
                & (1 << pi)) ? 1 : 0;
    }
    if (vis != 0) {
        if (((Class_004cfea0*)g_game->sound)->Is3DEnabled()) {
            Vector3_0047f300 p;
            p.x = pos->x - g_game->scrollX - (g_game->screenTilesX / 2) * 16;
            p.z = g_game->scrollY + (g_game->screenTilesY / 2) * 16
                + (pos->y >> 1) - pos->z;
            p.y = 0;
            ((Class_004cfeb0*)g_game->sound)->Set3DDistances(
                (float)(((g_game->screenTilesX + g_game->screenTilesY) / 2) * 16),
                (float)((g_game->width + g_game->height) * 16));
            return g_game->sound->PlaySampleSet(sound, -585, &p);
        } else {
            if (g_game->scrollX > pos->x || g_game->scrollY > pos->z
                || g_game->scrollX + g_game->screenTilesX * 16 < pos->x
                || g_game->scrollY + g_game->screenTilesY * 16 < pos->z)
                return g_game->sound->PlaySampleSet(sound, -1585, 0);
            return g_game->sound->PlaySampleSet(sound, -585, 0);
        }
    }
    return 0;
}