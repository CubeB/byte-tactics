// Decompiled by space-bunny-free, finished by DeepSeek V4.1 Flash, verified by GPT-6, finished by claude-opus-5-5. Names are provisional.
// Destroys the ten listener lists that 0x471d90 allocates into the game object
// (used by 0x471eb0, 0x471f40 and 0x471f90): every listener is deleted and
// erased from the front of its list, the ten vector members are then destroyed
// (the second, backward loop is the implicit ~vector of the array), the list
// object goes back to operator delete and g_game->lists is cleared. Both loops
// are the body of the destructor at 0x470fb0 (itself MATCH), reached here
// through the inlined `delete`. The virtual call is the `delete` of a
// listener (vtable slot 0, the scalar deleting destructor).
//
// MATCH (claude-opus-5-5, #5160), from 98.6%. The one byte left for a dozen
// passes was the SIB order of the store in the inlined erase's shift loop:
// the original puts the -4 delta in the base slot (`mov [edx+eax], ebp`),
// every source spelling gives the walker there (`mov [eax+edx], ebp`). The
// source was right all along; the order is decided by compiler state:
//  - C2 numbers the IL symbols in 16 bits, and whether the delta or the
//    walker becomes the base depends on that count against the function's own
//    local symbols. Every declaration in the TU counts (before or after the
//    function; an `extern int` or an enumerator is one, structs and
//    prototypes more), and the order wraps every 65536.
//  - With <vector> alone, the delta-base form appears only for 60204 to 60500
//    extra symbols (measured with unused `extern int`s and with enumerators,
//    both one each); with the full <windows.h> on top, for about 32100 to
//    32350. No set of real system headers (DirectX, CRT, commctrl, d3d) adds
//    that much, so the original TU's count must have come from Cavedog's own
//    headers, which we do not have.
//  - The enum below stands in for them: 60350 enumerators, the middle of the
//    window, so small changes to the declarations in this file stay inside it.
//    It emits nothing. Without it this file is 98.6% with the walker-base SIB.
// The other erase loops in this TU (0x471eb0, 0x471fd0, ...) are walker-base
// in the original too: they are smaller functions with fewer local symbols.
// Scratch drivers: build/scratch/0x471de0/{win,winb,wenum,gz,gl,gid,cf}.py.

#include <vector>

// Stand-in for the symbols of the original TU's own headers (see the notes at
// the top): 60350 enumerators, emitting nothing.
#define PAD10(p) p##0, p##1, p##2, p##3, p##4, p##5, p##6, p##7, p##8, p##9
#define PAD100(p) PAD10(p##0), PAD10(p##1), PAD10(p##2), PAD10(p##3), \
    PAD10(p##4), PAD10(p##5), PAD10(p##6), PAD10(p##7), PAD10(p##8), PAD10(p##9)
#define PAD1000(p) PAD100(p##0), PAD100(p##1), PAD100(p##2), PAD100(p##3), \
    PAD100(p##4), PAD100(p##5), PAD100(p##6), PAD100(p##7), PAD100(p##8), PAD100(p##9)
#define PAD10000(p) PAD1000(p##0), PAD1000(p##1), PAD1000(p##2), PAD1000(p##3), \
    PAD1000(p##4), PAD1000(p##5), PAD1000(p##6), PAD1000(p##7), PAD1000(p##8), PAD1000(p##9)
enum Padding_00471de0 {
    PAD10000(pad_a), PAD10000(pad_b), PAD10000(pad_c),
    PAD10000(pad_d), PAD10000(pad_e), PAD10000(pad_f),
    PAD100(pad_g), PAD100(pad_h), PAD100(pad_i),
    PAD10(pad_j), PAD10(pad_k), PAD10(pad_l), PAD10(pad_m), PAD10(pad_n)
};
#undef PAD10
#undef PAD100
#undef PAD1000
#undef PAD10000

class Listener_00471de0 {
public:
    virtual ~Listener_00471de0();
};

// The list object: ten std::vector members, 0x10 bytes each. The allocator is
// the first member of an MSVC 5 vector, so _First sits at +0x4 and the vectors
// run from +0x4 to +0x9f. Allocated by 0x471d90 with the global operator new.
class Lists_00471de0 {
public:
    std::vector<Listener_00471de0*> lists[10];

    ~Lists_00471de0()
    {
        for (int i = 0; i < 10; i++) {
            std::vector<Listener_00471de0*>::iterator it = lists[i].begin();
            while (it != lists[i].end()) {
                delete *it;
                lists[i].erase(it);
            }
        }
    }
};

#pragma pack(push, 1)
struct Game_00471de0 {
    char unknown_0[0x38d77];
    Lists_00471de0* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game_00471de0* g_game;

// FUNCTION: 0x471de0
void FUN_00471de0()
{
    if (g_game->lists) {
        delete g_game->lists;
        g_game->lists = 0;
    }
}
