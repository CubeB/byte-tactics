// Decompiled by space-bunny-free. Names are provisional.
// Picks the best of three 25-byte records with a __stdcall comparison callback
// (0x43c020 compares the name at +0x15 with _strcmpi) and stores it through
// the first argument, then returns that argument: both callers (0x43be6a,
// 0x43c20c) copy 25 bytes out of the returned pointer, so the load of the
// destination has to stay live across the block copy. That is what produces
// `mov eax,[esp+0xc] / ... / mov edi,eax` instead of a folded
// `mov edi,[esp+0xc]`. Same element type and callback style as the rest of
// this sort family (0x43c940 _Unguarded_insert, 0x43c990 the insertion sort,
// 0x43cb20 _Unguarded_partition, 0x43c6b0 lower_bound), which all pass the
// callback as a __stdcall function pointer.
//
// Suspected original bug: the epilogue is `ret 0x5c`, which pops four bytes
// fewer than the callers push (0x60: both also pass a second 0x43c020 that
// this function never reads). The 4-byte error cancels against 0x43cb20, which
// pops 0x28 for 0x24 bytes of arguments, so the pair leaves the stack balanced.

#include <string.h>

#pragma pack(push, 1)
struct Elem_0043ca70 {
    char unknown_0[0x15];
    char* name;                        // +0x15, the field 0x43c020 compares
};
#pragma pack(pop)

typedef int (__stdcall* Pred_0043ca70)(const Elem_0043ca70&, const Elem_0043ca70&);

// FUNCTION: 0x43ca70
Elem_0043ca70* __stdcall FUN_0043ca70(Elem_0043ca70* dest, Elem_0043ca70 a,
                                      Elem_0043ca70 b, Elem_0043ca70 c,
                                      Pred_0043ca70 pred)
{
    if (pred(a, b))
        *dest = pred(b, c) ? b : (pred(a, c) ? c : a);
    else
        *dest = pred(a, c) ? a : (pred(b, c) ? c : b);
    return dest;
}
