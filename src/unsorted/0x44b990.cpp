// Decompiled by mimo-v2.6-pro and GPT-5.6-Terra, measurement notes by
// space-bunny-free. Names are provisional.
// Opens the "SAVELIST.GUI" save dialog: copies each save name into the
// description buffer with its extension stripped, fills the "GAMES" list,
// wires up the list callbacks and shows the current game's name.
//
// Not finished: 635 of 635 bytes, 200 of 200 instructions, one fault left. In
// the loop head the original keeps the three independent statements in source
// order, ours schedules the count load first:
//   original  mov ebx,[0x5129b0] / xor ebp,ebp / mov eax,[esp+0x10] / test
//   ours      mov eax,[esp+0x10] / mov ebx,[0x5129b0] / xor ebp,ebp / test
// A scheduler tie inside that one basic block, nothing else differs.
// Measured negative (all 200 instructions, same 3 wrong, same size):
//   * loop shape: `for` with and without an init, `while`, `if` + `do/while`,
//     `for(;;)` with the test as a `break`, `if` + `for(;;)` + `break`,
//     the loop test in a `static int NotDone(int,int)` helper;
//   * `static char*& GetSaveDescriptions()` instead of `static char*`;
//   * reading the count through an inlined helper in the guard, or in the
//     loop condition, or through a second global-reading helper;
//   * the zero of the counter through an inlined helper;
//   * source order of the two loop-variable statements: the ptr load is
//     emitted before the `xor` either way, so this is not source order;
//   * headers: none, <stdlib.h>+<math.h>+<memory.h>, <windows.h>+<memory.h>,
//     <windows.h>, <windows.h>+<stdio.h>+<math.h>,
//     <stdlib.h>+<stdio.h>+<string.h>+<math.h> (6 sets, all with <string.h>):;
//   * 21 unused `extern int` declarations in front, N = 0 to 640 in steps of
//     32: the 0x4399f0 calibration does not reach this function.
// The counter declared inside the `if` (0x470c10's shape) moves the `xor`
// past the guard, which is worse. A free whole-function instruction differ
// (build/scratch/0x44b990/d2.py) is what produced these numbers.
// Second sweep (space-bunny-free), 110 more shapes, all the same three
// instructions and the same 200/200 length, so the score never moved:
//   * declaration order: all 12 orderings of count/layer/ptr/i, with the
//     loop variables initialised at the declaration and assigned later
//     (this is the 0x48c390 axis, never tried here before): no effect;
//   * 38 combinations of guard form (i<count, count>i, count-i>0, i!=count),
//     declaration order, head form and loop-body shape, i.e. the
//     "two changes at once" that 0x48c390 needed: no effect;
//   * dependency-creating helpers: `TakePtr(count, DAT_005129b0)` and the
//     argument orders either way, `CmpRef(int&)` in the guard, a ternary
//     that folds to a single load: no effect, MSVC drops the dependency
//     before scheduling;
//   * register pressure: pointer or counter live before or after the loop,
//     an extra value live across it (all cost instructions and score worse);
//   * held discarded return values, unused locals before and after the loop,
//     the uninitialised declaration, `GetSaveDescriptions` vs the bare
//     global vs an identity helper around it: no effect.
// TRANSPLANT FROM A MATCHED NEIGHBOUR, AND WHY IT FAILED. The identical
// five-instruction block exists at 0x44b790, inside 0x44b690, which MATCHES
// (data/progress.csv: matched, 100.0). Both blocks are byte-identical:
//   mov ebx,[0x5129b0]  G    xor ebp,ebp   Z    mov eax,[esp+0x10]   C
//    test eax,eax   jle
// 0x44b690's source for it is the shape this file already had, with ONE
// difference: it reads the bare global, `char* p = DAT_005129b0;`, where this
// file goes through `GetSaveDescriptions()`, and it declares the counter in the
// for initialiser, `for (int i = 0; i < count; i++)`, where this file has
// `int i = 0;` then `for (; i < count; i++)`. Both spellings were listed above
// as already tried INDIVIDUALLY, so I transplanted the PAIR, which the 0x48c390
// result says is the kind of thing a one-factor-at-a-time sweep misses. It does
// not move: still 635 of 635 bytes, still exactly 3 instructions differing, and
// the differ shows the same C G Z against the original's G Z C. So the two
// source differences between the two functions are NOT what makes the orders
// differ, which is worth more than the attempt cost.
// A minimal four-variant probe (build/scratch/0x44b990/probe.cpp, v1 bare
// global + for-init counter, v2 bare global + separate counter, v3 helper +
// for-init, v4 with the count fill last) is the sharper result: ALL FOUR emit
// C G Z, never G Z C. A probe with no callee-saved pushes therefore cannot
// reproduce the neighbour's order at all, which means the order is decided by
// register pressure in the whole function, not by the head's local source
// shape. That is consistent with the harness result above and is why the
// transplant fails. Note the earlier claim that 0x44b790 is an undecompiled
// sibling function is wrong: it is a BLOCK at offset +0x100 inside 0x44b690,
// which is why it was worth having.
// What the multi-function mini harness (build/scratch/0x44b990/mini*.cpp,
// scored by minord.py, one compile for 16 shapes) established, which is the
// useful negative to keep: the count load IS being hoisted, and the only
// shapes that stop the hoist and give the original's G Z C are (a) anything
// at all between the counter zero and the loop, which costs an instruction
// the original does not have, or (b) a store to memory between them, which
// likewise costs instructions, or (c) a two-return helper in the guard, which
// gets the order right and loses the `test eax,eax; jle` fold (0xb_twoout).
// There is no shape in the mini harness that gives G Z C and keeps the fold,
// so the two requirements are in tension and the residual is a scheduler
// tie that the source cannot express here.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0044b990 {                // 0x15b bytes
    char unknown_0[0x1b];
    unsigned int flags;                // +0x1b
    char unknown_1f[0xba - 0x1f];
    short selected;                    // +0xba
    char unknown_bc[0xce - 0xbc];
    void (__stdcall* handler)(int, int); // +0xce
    char unknown_d2[0x15b - 0xd2];
};
#pragma pack(pop)

struct Layer_0044b990 {
    int unknown_0;
    Entry_0044b990* entries;           // +0x04
    void (__stdcall* handler)(int, int); // +0x08
    void* data;                        // +0x0c
};

struct Menu_0044b990 {
    char unknown_0[0x18];
    Layer_0044b990* layer;             // +0x18
};

#pragma pack(push, 1)
struct Game_0044b990 {
    char unknown_0[0x519];
    Menu_0044b990 menu;                // +0x519
};
#pragma pack(pop)

extern Game_0044b990* g_game;
extern char* DAT_005091c8;             // savegame directory
extern char* DAT_005129ac;             // savegame names
extern char* DAT_005129b0;             // savegame descriptions
extern char DAT_005119b8[];

Layer_0044b990* __stdcall FUN_004aa8f0(Menu_0044b990* menu, const char* name, int flags);
void __stdcall FUN_0044b690(int a, int b);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_004bcf00(char* path);
void* __stdcall FUN_0044b4e0(int* out);
void __stdcall FUN_004a0bf0(Menu_0044b990* menu, char* name, char* text, int param_4);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_004a32a0(Menu_0044b990* menu, char* name, void* text, int value, int flag);
void __stdcall FUN_004a0570(Menu_0044b990* menu, char* name, int param_3);
Entry_0044b990* __stdcall FUN_0049ff90(Entry_0044b990* entries, char* name);
int __stdcall FUN_0049fdf0(Entry_0044b990* entries, const char* name, int flag);
void __stdcall FUN_0044b600(int unused1, int unused2);
void __stdcall FUN_004a0880(Menu_0044b990* menu, int index, char* text);
void __stdcall FUN_0049fa90(Menu_0044b990* menu);
void __stdcall FUN_004a7190(Menu_0044b990* menu, int index);
void __stdcall FUN_0049fb10(Menu_0044b990* menu, int value);
void FUN_00428b60();
void __stdcall FUN_0049fa50(Menu_0044b990* menu);
void __stdcall FUN_004a81e0(Menu_0044b990* menu, int value);

static char* GetSaveDescriptions()
{
    return DAT_005129b0;
}

// FUNCTION: 0x44b990
void FUN_0044b990()
{
    int count;
    Layer_0044b990* layer = FUN_004aa8f0(&g_game->menu, "SAVELIST.GUI", 0x880);
    layer->handler = FUN_0044b690;
    layer->data = g_game;
    FUN_004288d0("DSaveList", 0, 0, 0);
    FUN_004bcf00(DAT_005091c8);
    FUN_0044b4e0(&count);
    FUN_004a0bf0(&g_game->menu, "TITLE", "Save Game", 0);
    char* ptr = GetSaveDescriptions();
    int i = 0;
    for (; i < count; i++) {
        strcpy(ptr, FUN_004b6af0(DAT_005129ac, i));
        ptr += strlen(FUN_004b6af0(DAT_005129ac, i));
        while (*ptr != '.')
            ptr--;
        *ptr = 0;
        ptr++;
    }
    FUN_004a32a0(&g_game->menu, "GAMES", DAT_005129b0, count, 0);
    if (count == 0)
        FUN_004a0570(&g_game->menu, "DELETE", 0);
    Entry_0044b990* games = FUN_0049ff90(layer->entries, "GAMES");
    if (games != 0)
        games->handler = FUN_0044b600;
    int index = FUN_0049fdf0(layer->entries, "GAMENAME", 3);
    layer->entries[index].flags |= 2;

    Menu_0044b990* menu = &g_game->menu;
    Entry_0044b990* entries = g_game->menu.layer->entries;
    Entry_0044b990* games2 = FUN_0049ff90(entries, "GAMES");
    int index2 = FUN_0049fdf0(entries, "GAMENAME", 3);
    char* name;
    if (games2->selected > -1 && (name = FUN_004b6af0(DAT_005129b0, games2->selected)) != 0 && strlen(name) != 0)
        FUN_004a0880(menu, index2, name);
    else
        FUN_004a0880(menu, index2, DAT_005119b8);
    FUN_0049fa90(&g_game->menu);

    FUN_004a7190(&g_game->menu, index);
    FUN_0049fb10(&g_game->menu, 1);
    FUN_00428b60();
    FUN_004a0570(&g_game->menu, "LoadGame", 0);
    FUN_0049fa50(&g_game->menu);
    FUN_004a81e0(&g_game->menu, 0x40);
}
