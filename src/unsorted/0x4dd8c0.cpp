// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH: check.py prints 32% and the true LCS over normalised
// instructions is 96 of 297. Ours is 294 instructions / 768 bytes against the
// original's 297 / 785, so the control flow, the block order, the four
// division magics, the GlobalAlloc retry, the element size and the field
// layout are all reproduced. What is still wrong is ONE allocator state: the
// rotation of the callee-saved registers, which then permutes otherwise
// identical instructions across all four blocks. Read the note at the bottom
// before trying anything else; the diffs are not four separate problems.
//
// A hand-rolled vector's Insert(iterator where, size_type n, const T& x). The
// object is first / last / end, at +0x4 / +0x8 / +0xc, and the element is 0x30
// bytes, so every count below is a POINTER DIFFERENCE IN ELEMENTS, not a byte
// difference. That matters: `(int)(last - first) / 0x30` compiles to TWO
// divisions by 0x30 (MSVC 5 does not fold the divide into the pointer
// difference, the `(int)` cast is free), and the original has exactly one
// magic per site, so the cast and the divide must both go.
#include <windows.h>

extern void (*DAT_005289bc)();

struct Elem_004dd8c0 {
    char data[0x30];
};

class Class_004dd8c0 {
public:
    int field_0;                        // +0x0
    Elem_004dd8c0* first;               // +0x4
    Elem_004dd8c0* last;                // +0x8
    Elem_004dd8c0* end;                 // +0xc
    void FUN_004dd8c0(Elem_004dd8c0* where, unsigned int n, Elem_004dd8c0* val);
};

#define SIZE_004dd8c0 (first != 0 ? (unsigned int)(last - first) : 0)

static void CopyElem_004dd8c0(Elem_004dd8c0* dst, const Elem_004dd8c0* src)
{
    if (dst != 0) {
        *dst = *src;
    }
}

// FUNCTION: 0x4dd8c0
void Class_004dd8c0::FUN_004dd8c0(Elem_004dd8c0* where, unsigned int n, Elem_004dd8c0* val)
{
    if ((unsigned int)(end - last) < n) {
        unsigned int ncap = SIZE_004dd8c0 + (n < SIZE_004dd8c0 ? SIZE_004dd8c0 : n);
        Elem_004dd8c0* newmem;
        do {
            newmem = (Elem_004dd8c0*)GlobalAlloc(0, ncap * 0x30);
            if (newmem == 0 && DAT_005289bc != 0)
                DAT_005289bc();
        } while (newmem == 0 && DAT_005289bc != 0);

        Elem_004dd8c0* d = newmem;
        for (Elem_004dd8c0* s1 = first; s1 != where; s1++, d++)
            CopyElem_004dd8c0(d, s1);
        for (unsigned int i = 0; i != n; i++, d++)
            CopyElem_004dd8c0(d, val);
        for (Elem_004dd8c0* s2 = where; s2 != last; s2++, d++)
            CopyElem_004dd8c0(d, s2);

        if (first != 0) {
            GlobalFree(first);
        }
        end = newmem + ncap;
        unsigned int oldsize = first != 0 ? (unsigned int)(last - first) : 0;
        first = newmem;
        last = newmem + (n + oldsize);
        return;
    }

    if (n > (unsigned int)(last - where)) {
        Elem_004dd8c0* d = where + n;
        for (Elem_004dd8c0* s2 = where; s2 != last; s2++, d++)
            CopyElem_004dd8c0(d, s2);
        d = last;
        for (unsigned int i = n - (unsigned int)(last - where); i != 0; i--, d++)
            CopyElem_004dd8c0(d, val);
        for (d = where; d != last; d++)
            *d = *val;
    } else {
        if (n == 0) {
            return;
        }
        Elem_004dd8c0* d = last;
        for (Elem_004dd8c0* s3 = last - n; s3 != last; s3++, d++)
            CopyElem_004dd8c0(d, s3);
        for (Elem_004dd8c0* s4 = last - n; s4 != where; ) {
            s4--;
            d--;
            *d = *s4;
        }
        for (d = where; d != where + n; d++)
            *d = *val;
    }
    last += n;
}

// STILL DIFFERS, and what to try next.
//
// THE WHOLE GAP IS THE CALLEE-SAVED ROTATION. The original's prologue is
//     sub esp, 0xc / push ebx / push ebp / push esi / mov esi, ecx
//     mov eax, 0x2aaaaaab / push edi / mov ebx, [esi+8] / mov ecx, [esi+0xc]
// so, for the whole function, esi = this, edi = n, ebx = this->last (loaded
// once at 0x4dd8ce, still live at 0x4ddb9c), and ebp = the byte count handed
// to GlobalAlloc (0x4dd95d, still live across the retry loop's calls). Every
// temporary count lives in a scratch register: the four `size` results are
// recomputed rather than CSEd and each dies in EDX or ECX within a few
// instructions. This build puts this in EDI, n in EBX, last in ECX and the
// byte count in EBP, so the rotation is shifted by one and each block comes
// out permuted. Since the first block already differs only in those four
// registers, this is ONE cause, not four.
//
// The mechanism, per the guide's rule that any extra live graph node demotes a
// variable one step in the order ESI, EDI, EBX, EBP: the original keeps
// `this` in a register (esi) all the way to 0x4dda3e, across the GlobalFree
// call, and reloads it from the [esp+0x10] spill only five times. This build
// reloads it four times and lets a `size` temporary take the register that
// `this` should have. The lever is to make `this` rank first, which means
// removing whatever currently outranks it: the one long-lived `size` temporary
// in the `ncap` expression.
//
// THE `ncap` EXPRESSION IS THE ONE PIECE OF THE ROTATION I COULD NOT SETTLE.
// The original computes the first `size` at 0x4dd8fd into EDX and compares it
// with n there (0x4dd912), then recomputes the max into ECX (0x4dd91e) and
// recomputes the addend into EDX a third time (0x4dd941), so the add is
// `size + max(size, n)` in that source order: compare, then max, then addend.
// It also emits `lea eax, [edx + ecx]` with the addend first.
//
//  - `SIZE + (n < SIZE ? SIZE : n)` (what this file has) gives the right `lea`
//    operand order but feeds the ADDEND from the FIRST `size` computed, so
//    that value is live from 0x4dd90e to 0x4dd95a, across two branches, and
//    has to take a callee-saved register. LCS 96.
//  - `(n < SIZE ? SIZE : n) + SIZE` gives the right evaluation order and the
//    right `lea`, but MSVC then CSEs one of the three `size` values, so the
//    build is 6 instructions and 17 bytes short, and it puts the max in EDI
//    (n's register) and spills n. LCS 55.
//  So neither spelling is right, which means the addend's `size` and the
//  max's `size` are not textually the same expression in the original. A
//  source-level `Size()` member (not __inline, and not the macro) is the
//  obvious thing to try next: the macro expands to the same tree three times
//  and MSVC is free to CSE it.
//
// THE THIRD REALLOC COPY IS FIVE INSTRUCTIONS SHORT AND I COULD NOT REPRODUCE
// ITS HOIST. The original materialises `n * 0x30` into ECX before the loop
// (0x4dd9eb `lea ecx, [edi+edi*2] / shl ecx, 4`), forms the destination
// `newmem + n * 0x30` (0x4dd9f6), and then derives the loop's SOURCE pointer
// from the DESTINATION rather than reloading `where`:
//     mov eax, edx / sub eax, ebx / add eax, esi / sub eax, ecx
// which is `(dest - newmem) + where - n * 0x30`, i.e. `where`, computed the
// long way round. A plain `for (s = where; s != last; s++, d++)` does not
// produce it (this file), and neither does writing the source as `d - n`
// after `d += n` (tried, LCS 60). I suspect the original does not use a
// simple two-pointer loop here but something that states the gap, for example
// a helper taking (dest, offset, first, last), in which case the two pointers
// are related by a constant and MSVC derives one from the other.
//
// TRIED, all neutral or worse, none worth repeating:
//  - hoisting `this->last` into a named local `lastp` at the top of the
//    function, the way the growth test and the copy loops would suggest: LCS
//    63 against 96 for using the member directly. The original reloads
//    `[esi+8]` at 0x4dda4f for the final `oldsize`, so the original has no
//    such local either.
//  - `oldsize` written as `lastp != first ? (lastp - first) : 0` gives a
//    `cmp ebp, eax`; the original has `test eax, eax / jne` and then reads
//    `[esi+8]` again, so the guard really is `first != 0 ? (last - first) : 0`
//    on the MEMBER. Inlining that expression instead of naming it `oldsize`
//    drops the LCS to 74, so it has to stay a named local.
//  - naming the byte count (`nbytes = ncap * 0x30`) so the `end` update can
//    divide it back out: 306 instructions, 95.
//  - `room` as a named local, `n > room` instead of `room < n`, and a
//    goto/label to reorder the blocks: all neutral or much worse (83 for the
//    label). The original's block order (realloc inline, in-place at
//    0x4dda80 out of line) is what the `if (...) { ... return; }` here gives.
//
// SUSPECTED ORIGINAL BUG, worth reporting. The in-place path at 0x4ddaa1 is
// taken when `n > (last - where)`, that is when n exceeds the number of
// elements at and after `where`. It then shifts the tail up by n
// (0x4ddab7, forward) and refills [where, last) with n - after copies of the
// new value (0x4ddafe, 0x4ddb13). Because the branch guarantees
// where + n > last, the forward shift's two ranges do not overlap, so the
// forward copy is safe, but only by exactly the margin that branch test
// provides. The other in-place path (0x4ddb3b) has ranges that DO overlap and
// therefore walks backwards (0x4ddb83). Two hand-written copies of the same
// operation, one of which is only correct because of the branch it sits under,
// is the shape worth a human's eye.
//
// A second, smaller one: the growth size is `size + max(size, n)` elements
// (0x4dd912 to 0x4dd937, `cmp edi, edx` then `mov ecx, edi` or a recomputed
// size, and `lea eax, [edx + ecx]`), so n == 1 doubles the vector, but a
// single huge insert into a nearly empty vector allocates n + size. That is a
// deliberate policy rather than a bug, but it is the sort of thing
// `max(size, n + size)` would have got wrong, and the disassembly shows the
// addend and the max are the two the author intended.
