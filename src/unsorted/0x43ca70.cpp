// Decompiled by space-bunny-free. Names are provisional.
// Picks the smallest of three 25-byte records with a __stdcall comparison
// callback (0x43c020 compares the string at +0x15 case-insensitively) and
// stores it through the first argument. Same element type and callback style
// as the rest of this sort family (0x43c940 the unguarded insert, 0x43c990
// the insertion sort, 0x43cb20 the unguarded partition, 0x43c6b0
// lower_bound), which all pass the callback as a __stdcall function pointer.
//
// Still differs (GAVE UP at 85.5%): 157 of 163 bytes. The block structure,
// every branch, every call and both hoisted `lea esi` match; what is left is
// that the original loads the destination pointer into eax and then moves it
// into edi for the `rep movsd`
//
//     mov eax,[esp+0xc] / lea esi,[esp+0x10] / mov ecx,6 / mov edi,eax
//
// while this source folds the load straight into the copy operand
//
//     mov edi,[esp+0xc] / lea esi,[esp+0x10] / mov ecx,6
//
// in each of the three copy blocks (6 bytes). A member access at a non-zero
// offset does produce the register form (MSVC cannot fold it), and a member at
// offset 0 or a plain deref folds, so the original's spelling of the store
// was not found: pointer, reference, char*/void*/int destination, casts,
// memcpy, an inline store helper, a class with an inline operator=, pack(1),
// pack(2), a 4-aligned 25-byte type, a union, a template and a one-expression
// ternary all give the folded form, and no header set changes it
// (tools/headers.py tried all 128 sets, best 85.5%).
//
// A second pass over the same file also ruled out: a `bool`-free `(int)p`-style
// rewritten condition, the element given an inline `operator=` (three bodies:
// memcpy, byte loop, void return), the destination declared as a reference and
// assigned directly, a wrapper class with the element at offset 0 (by pointer
// and by reference) with and without an inline setter, a trailing pad after the
// element, `dest[0] =`, `*(dest + 0) =`, a `Elem* const` parameter, a local
// destination pointer, the nested ternaries swapped, if/else chains with early
// returns, and a trailing `return;`. All stayed at 85.5% or dropped (the
// if/else chains to 74.5%), so the two nested ternaries are the right
// structure and only the destination register form is unexplained. The pattern
// matches the guide's "a value loaded into eax then copied to a callee-saved
// register", but without a loop to hang the "keep a copy of its old value" on,
// there is no natural construct left to try.

#pragma pack(push, 1)
struct Elem_0043ca70 {
    char data[0x19];
};
#pragma pack(pop)

typedef int (__stdcall* Pred_0043ca70)(const Elem_0043ca70&, const Elem_0043ca70&);

// FUNCTION: 0x43ca70
void __stdcall FUN_0043ca70(Elem_0043ca70* dest, Elem_0043ca70 a,
                            Elem_0043ca70 b, Elem_0043ca70 c,
                            Pred_0043ca70 pred)
{
    if (pred(a, b))
        *dest = pred(b, c) ? b : (pred(a, c) ? c : a);
    else
        *dest = pred(a, c) ? a : (pred(b, c) ? c : b);
}
