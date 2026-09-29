// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH. Stopped at the wall clock limit. Best true LCS over instructions
// is 137 of 297 (46.1% of the original, 40.8% of ours); check.py's difflib ratio
// reads about 30% but that number is misleading on this function, see the note at
// the bottom. Every call, argument, callee, branch condition, divide magic
// (0x2aaaaaab then sar edx,3, so a signed divide by 0x30), the GlobalAlloc retry
// through the out-of-memory handler, the element size (0x30, copied as
// `rep movsd` with ecx = 0xc) and the field layout are reproduced. What is
// still wrong is the register allocation, which then permutes otherwise
// identical instructions across all four blocks.
//
// A hand-rolled vector's Insert(iterator where, size_type n, const T& x): the
// object is first / last / end, so +0x4 / +0x8 / +0xc. It either shifts in place
// (enough room at the end) or grows to max(size, n) + size elements, copies the
// three ranges across, then frees the old block.
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

#define SIZE_004dd8c0 (first != 0 ? (unsigned int)((int)(last - first) / 0x30) : 0)

static void CopyElem_004dd8c0(Elem_004dd8c0* dst, const Elem_004dd8c0* src)
{
    if (dst != 0) {
        *dst = *src;
    }
}

// FUNCTION: 0x4dd8c0
void Class_004dd8c0::FUN_004dd8c0(Elem_004dd8c0* where, unsigned int n, Elem_004dd8c0* val)
{
    Elem_004dd8c0* lastp = last;
    if ((unsigned int)((int)(end - lastp) / 0x30) < n) {
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
        for (Elem_004dd8c0* s2 = where; s2 != lastp; s2++, d++)
            CopyElem_004dd8c0(d, s2);

        if (first != 0) {
            GlobalFree(first);
        }
        end = newmem + ncap;
        unsigned int oldsize = lastp != first ? (unsigned int)((int)(lastp - first) / 0x30) : 0;
        first = newmem;
        last = newmem + (n + oldsize) * 0x30;
        return;
    }

    if (n > (unsigned int)((int)(lastp - where) / 0x30)) {
        Elem_004dd8c0* d = where + n;
        for (Elem_004dd8c0* s2 = where; s2 != lastp; s2++, d++)
            CopyElem_004dd8c0(d, s2);
        d = last;
        for (unsigned int i = n - (unsigned int)((int)(last - where) / 0x30); i != 0; i--, d++)
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
// ROOT CAUSE OF THE WHOLE GAP: WHICH CALLEE-SAVED REGISTER `this` AND `last`
// GET, and therefore the order every other value is assigned in. The original's
// prologue is
//     sub esp, 0xc / push ebx / push ebp / push esi / mov esi, ecx
// so esi = this for the whole function (with one copy spilled to [E0+0x10] at
// 0x4dd8e1 and reloaded from there at 0x4dd98e, 0x4dd9e7, 0x4dda20,
// 0x4ddad4, 0x4ddb13, 0x4ddbbc), ebx = this->last (loaded once at 0x4dd8ce
// and live all the way to 0x4ddb9c), and edi = n (0x4dd8d6, kept to
// 0x4ddbf0). This build puts this in edi and n in esi, the two are swapped, and
// every block after the first comes out rotated. THIS IS ONE ALLOCATOR STATE,
// not four independent problems: the first block already differs only in those
// two registers, and the remaining three blocks differ in the same way.
//
// The one lever that worked, and it is worth +7 points of LCS on its own:
// hoisting `last` into a local named `lastp` at the TOP of the function, before
// the room test, and using it in the room test and in the three copy loops of
// the realloc path. That took the LCS from 116 to 137. It works because it
// gives the allocator a reason to keep last in ebx, which is what the original
// does.
//
// NOT REACHED, and the things to try next, in the order I would try them:
//  - hoist `first` the same way. Tried (`firstp` in v13): it drops to 114, so
//    the first pointer must NOT get the same treatment. The asymmetry is
//    itself the clue: `first` is re-read from the object in the original (at
//    0x4dd8f2, 0x4dd996, 0x4dda24, 0x4dda3e) whereas `last` is loaded once
//    into ebx, so the original's source almost certainly does not have a
//    `first` local at all, or has one that dies immediately.
//  - the original recomputes `(last - first) / 0x30` FOUR times in the realloc
//    path (0x4dd8fd, 0x4dd91e, 0x4dd941, 0x4dda4f) and once more at
//    0x4dda4f for the final `last` update, with the divide magic reloaded
//    (`mov eax, 0x2aaaaaab`) before each. It is a plain member function or
//    macro that MSVC 5 will not CSE because `first` and `last` are member
//    loads it must re-issue; a `__inline` accessor is the wrong tool, it CSEs
//    to one. The macro here reproduces the repeats, but the repeats land in
//    the wrong registers. If a source-level `Size()` member (not `__inline`)
//    ever scores better than the macro, prefer it.
//  - the final `last = newmem + (n + oldsize) * 0x30` at 0x4dda65 reads n from
//    edi and oldsize from edx, and computes `(edi + edx) * 3 << 4` with a
//    single `lea ecx, [eax+eax*2] / shl ecx, 4`. Here `oldsize` is computed
//    AFTER `end` is stored and BEFORE `first` is stored, which is the order in
//    the file; the original computes it between the `end` store and the
//    `first` store too, so the statement order is right, only the registers
//    are not.
//  - the argument that gets shifted into edi in the original. `n` is used in
//    the first compare, so it is the first argument to want a register. Here
//    `where` (esi in the original, never named in a register before 0x4dd992)
//    wins it instead. Declaring `n` as a local copy of the parameter, or
//    reordering so the room test is `n > room` with room a named local, both
//    tried and neither moved the LCS, but only the second was tried with the
//    lastp hoist in place, so it is worth one more run.
//
// TRIED, all neutral or worse, none worth repeating:
//  - `sub esp, 0xc` (3 dword locals) never appears in this build: the
//    original has 3 dwords of frame and mine has 2, and the third is the
//    spilled `this` copy at [E0+0x10]. Forcing a fourth local does not help;
//    what is needed is the SPILL, not the frame size.
//  - hoisting the room test into a named `unsigned int room` local: identical
//    output to the inline expression (137 either way), so the divide is
//    rematerialised either way.
//  - spelling the room test as `n > room` instead of `room < n`: identical
//    (137), so the operand order is not what decides that branch.
//  - a `goto in_place` / label to control the block order: 83, much worse.
//    The original's block order (realloc inline, in-place at 0x4dda80 out of
//    line) is what the `if (...) { ... return; }` in this file already gives.
//
// SUSPECTED ORIGINAL BUG, worth reporting. The in-place path at 0x4ddaa1 is
// taken when `n > (last - where) / 0x30`, that is when n is larger than the
// number of elements at and after `where`. It then shifts the tail up by n
// (0x4ddab7, forward, so this copy is correct only because the ranges do not
// overlap) and fills with `n - after` copies (0x4ddafe). The other in-place
// path at 0x4ddb3b, taken when `n <= after`, moves the last n elements up by n
// (0x4ddb57) and then walks the REST backwards from `last - n` down to `where`
// (0x4ddb83, `sub eax, 0x30 / sub edx, 0x30` each iteration, comparing eax
// against ebp, the saved `where`). That backwards walk is correct. So both
// in-place paths are right.
//
// The real suspect is the in-place path's FIRST shift at 0x4ddab7: it copies
// from `eax` (which starts at `where`) forward to `edx` (which starts at
// `where + n`) while both advance, and it runs until eax reaches ebx, the OLD
// `last`. Because n is greater than the element count from `where` to `last` on
// this path, the destination `where + n` is past the old `last`, so the ranges
// do not overlap and the copy is safe. It is correct, but only by exactly the
// margin the branch test guarantees, and the same helper is used on the other
// path where the ranges DO overlap and where the code therefore has to walk
// backwards instead. Two hand-written copies of the same operation, one of
// which is only safe because of the branch it sits under, is the shape worth a
// second look by a human.
//
// A second, smaller one: the growth size is `max(size, n) + size` elements
// (0x4dd912 to 0x4dd937, edx = size and ecx = n, `lea eax, [edx + ecx]`), which
// for n == 1 doubles the vector, but if `n > size` it allocates n + size, so a
// single huge insert into a nearly empty vector over-allocates by a factor that
// depends on n. That is a deliberate policy, not a bug, but it is the sort of
// thing a `max(size, n) + size` written as `max(size, n + size)` would have
// got wrong, and the disassembly shows it was not.
