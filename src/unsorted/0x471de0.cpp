// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL (68.5%). What still differs, all in the outer loop's register
// allocation, not in the code shape:
//   * the original keeps the `g_game->lists` pointer in ebx and the slot cursor
//     in esi (`lea esi, [ebx + 8]`), with a zero constant in ebp that is also
//     used for the null test (`cmp ebx, ebp`); mine folds the pointer and the
//     cursor into one esi (`add esi, 8`) and tests it with `test esi, esi`.
//     So the original pushes ebx, ebp, edi, esi and mine pushes esi, edi, ebp,
//     ebx, which shifts every epilogue pop.
//   * the original's induction base is `&v._Mylast` (slot + 8, so the fields
//     are read at [esi-4] and [esi]); mine is `&v` (fields at [esi+4], [esi+8]).
//   * consequently the reload of the spill slot holding the lists pointer is
//     inside the outer loop body in the original, after it in mine.
//   * the inlined _Copy of vector::erase stores to [edx + eax] in the original
//     and to [eax + edx] in mine (same address, other base/index choice).
// The first loop body and the whole second loop (the empty destructor of the
// ten std::vector members inlined into `delete l`) match instruction for
// instruction. Next step: try sources that force the lists pointer into ebx,
// e.g. an explicit cursor pointer that MSVC cannot fold back into it, or a
// hand-written free of the three {_Myfirst,_Mylast,_Myend} fields with the
// zero constant hoisted by a local.
#include <vector>

class Listener_00471de0 {
public:
    virtual ~Listener_00471de0();
};

// Ten listener lists, one per message type. 0x471d90 allocates it (0xa0 bytes:
// ten 16-byte std::vector members, _Myfirst at +4, _Mylast at +8, _Myend at
// +0xc) and 0x471d90's loop also stores its char argument in the allocator
// byte at +0 of each one. 0x471eb0, 0x471f40 and 0x471f90 walk the same lists.
struct Lists_00471de0 {
    std::vector<Listener_00471de0*> lists[10];
};

#pragma pack(push, 1)
struct Game_00471de0 {
    char unknown_0[0x38d77];
    Lists_00471de0* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game_00471de0* g_game;

// Frees the ten listener lists made by 0x471d90: every listener is deleted and
// erased from its own list, then the lists object itself is freed and the
// global pointer cleared.
// FUNCTION: 0x471de0
void FUN_00471de0()
{
    Lists_00471de0* l = g_game->lists;
    if (l) {
        for (int i = 0; i < 10; i++) {
            std::vector<Listener_00471de0*>& v = l->lists[i];
            std::vector<Listener_00471de0*>::iterator it = v.begin();
            while (it != v.end()) {
                delete *it;
                v.erase(it);
            }
        }
        delete l;
        g_game->lists = 0;
    }
}
