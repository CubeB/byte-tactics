// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, continued by GPT-6.1-sol, continued by Space Bunny Free, matched by Space Bunny Free. Names are provisional.
// MATCH (171 of 171 bytes, every reference checks out).
//
// What finally did it, after 91.2% had stood through four passes: the source
// below is the SAME shape that took 0x47eee0 from 86.2% to a match, and the
// reason it works here is that 0x47ee30's body is byte-for-byte the body
// 0x47eee0 already has. Three things, all of them needed at once:
//  1. a `List_0047f8c0* list` local, and the count pointer DERIVED from it
//     (`int* count = &list->count;`). Earlier passes concluded, correctly, that
//     a local-derived count always folds onto the base and dies (66.7%). What
//     was missing is the one thing that stops the fold,
//  2. `count = &list->count;` as the FIRST statement of the shift loop's BODY.
//     That re-assignment is loop invariant, so LICM hoists the resulting
//     `lea edi, [ebp+0x99]` into the shift loop's preheader, which is exactly
//     where the original materialises it (0x47ee79). This is the remat that
//     four passes of spelling the count through the global could never get,
//     because MSVC 5 never folds a GLOBAL-derived address and so never had
//     anything to hoist.
//  3. no `entries` local: the entries are reached as `list->entries[i]`, and
//     the last entry as a named `Entry* last = &list->entries[i]`. With an
//     `entries` local the same shape is 70 instructions against the original's
//     63: MSVC then puts `lea ebx, [esi+ecx+12]` (the address of `data`) in
//     ebx and stores `mov [ebx], 0`, where the original keeps `lea ebx,
//     [esi+ecx]` (the entry) and stores `mov [ebx+0xc], 0`. Going through
//     `list->entries` is what keeps ebx pointing at the whole entry, which the
//     shift loop then reuses as its own running pointer.
// With those, the list lands in ebp by itself (`mov ebp, [DAT]`, no eax
// detour), the count pointer stays in edi for the whole loop, the guard is a
// separate source-level `if` reading the base, the do/while back edge forwards
// the decrement (`mov eax, ecx`), and the preheader lea appears. All 63
// instructions and all 171 bytes match.
//
// The lever that was aimed here (the self-conditional pin `g = g ? g : g`) was
// NOT what did it: with the `List*` local in place the pin is a no-op, measured
// identical to the source below (variants e, g and n in this pass's scratch
// score exactly the same bytes). It is still the one way found to stop MSVC 5
// folding a derived pointer's uses back onto its base (worth 15.2 points on
// 0x4c54f0), but here the in-loop re-assignment does the same job and does it
// better, because it also creates the preheader hoist.
//
// Things measured this pass and NOT needed (all 63-instruction shape, listed so
// nobody repeats them): the count pointer from the global, with or without the
// pin, with the re-assignment: 71 instructions, no remat, no forward; the same
// with a `while` and a base-form test: exact prologue and remat but a
// reloading back edge; a guard through `*count` instead of the base, `count`
// declared before the guard, the guard re-pinned, and the pin on the count
// pointer: all byte-identical to the source below, because with the `List*`
// local the guard, the pin and the declaration order no longer decide anything.
// The `field_9d = 0` after the loop must go through `list`, and the shift loop's
// re-assignment must be `&list->count` (not `&entries[9].field_0`).
//
// The harness that found this, worth rebuilding on the next stuck function:
// build/scratch/0x47ee30/probe.py compiles up to 30 whole-function variants in
// parallel (`/Fa` listings, one obj dir), normalises both the original's
// disassembly and the listing to the same text, and reports a per-variant
// instruction-level diff plus three counters: the remat `lea edi,[ebp+0x99]`
// count, whether the back edge is the forwarded `mov eax, ecx`, and whether the
// prologue takes the `mov ebp, eax` detour. Screening those three counters
// ranks a sweep instantly and, unlike check.py's byte ratio, does not go blind
// to a pure register choice: 0x47e5c0 is the cautionary case, a 54% shape
// reached a match while six workers permuted from its 67.5% version.
//
// The history that led here, for the record (91.2%, 169/171, was the previous
// best after four passes):
//  * the count is reached through a pointer and the guard reads it through the
//    base: that keeps the list in ebp and the count pointer in edi (the two are
//    otherwise an allocator trade-off, see below);
//  * the loop condition must be spelled through the count POINTER, the same
//    node as the store in front of it, or MSVC reloads instead of forwarding.
//    In C that is a guarded do/while, not a while, hence the shape above;
//  * a count POINTER from the GLOBAL keeps the guard unfolded but costs the
//    `mov eax,[DAT]; mov ebp,eax` detour in the prologue, because with a
//    global-derived address MSVC needs a scratch register before ebp is set;
//  * a count pointer DERIVED from a local folds onto the base in every plain
//    spelling, which cost every earlier attempt its `lea edi,[ebp+0x99]` and
//    with it the whole head. The in-loop re-assignment is what undoes the
//    fold. (The same reasoning is why `int* count = (int*)((char*)entries +
//    0x99);` with `*count--;` is not the answer: MSVC 5 strength-reduces that
//    count ADDRESS into an induction variable and DROPS THE STORE, emitting
//    `mov eax,[edi-4]; sub edi,4` with no write to the count at all, which is
//    a real VC5 codegen bug -- the loop would walk off the list.)
// Checked along the way and ruled out: /O2 /Ob2 /MT, /O2 /Ob1 /MT, /O2 /Ob2
// /MT /Gd, /O2 /Ob2 /MT /Gs, /Gz and /Gr, the 0-to-400 declaration-count sweep,
// and all 128 header sets of headers.py: none of them is the lever.
// Suspected original bugs: none in this function.

void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_0047f8c0 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

struct List_0047f8c0 {
    Entry_0047f8c0 entries[9];         // +0x0
    int count;                         // +0x99
    int field_9d;                      // +0x9d
};
#pragma pack(pop)

struct Table_0047ee30 {                // 0x18 bytes
    int field_0;                       // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int field_c;                       // +0xc
    int unknown_10;                    // +0x10
    int unknown_14;                    // +0x14
};

extern List_0047f8c0* DAT_0051e68c;
extern Table_0047ee30 DAT_005086e0[24];

// FUNCTION: 0x47ee30
void FUN_0047ee30()
{
    List_0047f8c0* list = DAT_0051e68c;
    if (list->count > 0) {
        int* count = &list->count;
        do {
            int i = *count - 1;
            Entry_0047f8c0* last = &list->entries[i];
            if (last->data) {
                FUN_004d85a0(last->data);
                last->data = 0;
            }
            for (int j = i; j < *count; j++) {
                // Re-taken at the top of the body: this is what stops MSVC 5
                // folding &list->count into [ebp+0x99], and because the
                // assignment is loop invariant the lea is hoisted into the
                // shift loop's preheader, which is where the original has it.
                count = &list->count;
                list->entries[j] = list->entries[j + 1];
            }
            (*count)--;
        } while (*count > 0);
    }
    list->field_9d = 0;
    for (int k = 0; k < 24; k++)
        DAT_005086e0[k].field_c = 0;
}
