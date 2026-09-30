// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, and GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
// deepseek-v4.1 retry (#2430), five further checks, no change: 80.9%, same two
// hunks. The remaining diff is one backend decision (the constant 0 keeps a
// register, edx, across the focus block) and every "real variable" reading of
// it folds to the same immediates:
//   - `int z = e->field_157;` used for all five zero uses (store to field_78,
//     its compare, the off clamp cmp+store, the holder compare): after the
//     `if (field_157) return;` guard MSVC proves z is 0 and folds it, 80.9%.
//   - `int z = 0;` declared before the FUN_004a23b0 call, so its live range
//     crosses a call and the backend would have to rematerialise it after:
//     still folded to immediates, 80.9%.
//   - the per-arm phi (`if (...) { z = 0; ... } else { z = 0; }`): the two
//     constant defs value-number to one constant and fold, 80.9%.
// Note the counts: the original gives the constant 0 a register for five uses,
// ours has only two unavoidable ones (the clamp's cmp and store); the store to
// field_78 becomes `mov [ebp+0x78], eax` off the known-zero call result, and
// both `!= 0` tests become load+`test`. Those extra register uses are the
// difference, and they disappear because the compiler already has a zero in
// eax. A shape that keeps a live value in eax at 0x4a420d, or adds a third
// immediate-zero use inside the block, was not found. Checked in #2430 and also
// worse: naming the FUN_004ab5b0 result (`int r = ...; if (!r)`) shrinks the
// function to 709 bytes and 75.1%, and the `!= 0` spelling of the drag test is
// byte-identical. The second hunk (tail) follows from the same decision: with no
// live zero, MSVC gives the two `field_78 = 0` stores in the FUN_004ab510 arms a
// register zero (edi), which kills p.y and forces `mov edi, [esp+0x34]` before
// `cmp edi, [esp+0x24]`; the original keeps p.y in edi and stores immediates.
// Retry #1758: GPT-6.1-sol confirmed 80.9% after four checks; no MATCH. The focus==index block still changes zero rematerialization, register allocation and branch layout.
// GPT-6.1-sol refinement: six checks found no improvement. A local zero copied
// from field_78 or derived as focus+1 folds to the same immediate; focus-index
// and off-old produce larger, worse code (49.0% and 51.8%). An int* alias for
// field_78 is byte-identical to baseline at 80.9%. Keep the 80.9% source below.
// GPT-6 retry: retained 80.9%. Full-width/partial-width zero value variants
// did not recover the original edx zero; detailed previous notes remain below.
// deepseek-v4.1-flash retry: no change to the code, still 80.9% and NOT a match.
// New facts about the remaining diff, so the next attempt does not repeat them:
// an N-declaration sweep (0 to 400 unused `extern int`) leaves the score flat
// at 80.9%, so this is source shape, not compiler state. Operand-order
// respellings change nothing (`0 == obj->field_78`, `0 > e->off`,
// `0 != obj->holder`, `obj->field_78 == 0` all give byte-identical output).
// A `int zero = 0;` declared at the top of the focus block and used for the
// field_78 store, the `e->off < 0` clamp and the holder test still folds the
// store back to the call result in `eax`, so the zero never survives. `short v`
// in the wheel arm, a per-arm `old`, a single shared `off` with one
// `e->off = off` after the if/else, `e->off--`/`e->off++`, `switch`-style
// conditional stores, an `int f94`, and a `!= 1` spelling of the call test all
// score below 80.9% (between 47% and 74%), several of them by breaking the
// shared store at 0x4a427b. What is still unexplained: the original's
// `xor edx, edx` in BOTH arms of the FUN_004ab5b0 test means the 0 is a phi at
// the merge, i.e. it is live on both paths, yet every literal-zero spelling
// the front end sees is folded or rematerialised. A real variable that is 0 on
// both paths and read after the merge is the only shape left.
// deepseek-v4.1-flash retry (issue 1202): tools/headers.py with all 128 header
// sets (and the file's own structs) is also flat at 80.9%, so the edx zero is
// not a compiler-state or header effect. Rechecked the 0x4a3ef0 lesson by
// declaring `int zero = 0;` right after the FUN_004a23b0 call (not at the top
// of the focus block): still 80.9%, MSVC folds it to `mov [ebp+0x78], eax` and
// `cmp word [ebx+0x140], 0` exactly as before. No change to the code.
// Sonnet 5.5 retry (#1080): no change to the code. /Gz and /Gr give the same 80.9%. The original zero in edx is not reachable with an `int zero = 0` local, a local assigned 0 in both arms, or named locals for the call results, the drag flag, the holder and the offset (about 150 variants): MSVC folds every one back to immediates. In 0x4a3ef0 a zero local that is reassigned LATER (`int lines = 0;` then `lines = ...` in one arm), declared after the last call before the block, did create the zero register, so look for a real variable in this block that starts at 0 and is reassigned on some path.
// Not a match yet, 80.9%. The whole prologue, the entry-address computation,
// the 24-byte point copy, the FUN_004a23b0 call, both FUN_004ab510 tail
// blocks (right-button arm down to `mov [ebp+0x94], dx`) and the whole
// FUN_004ab510/FUN_0049fc50/FUN_004ab690 tail match byte for byte. What is
// left is inside the `obj->focus == index` block, and all of it comes from one
// decision: the original keeps the literal 0 in a register (edx) for the whole
// block, so it spends `xor edx, edx` in both arms of the FUN_004ab5b0 test
// and then uses `cmp [ebp+0x78], edx` / `mov word [ebx+0x140], dx` /
// `cmp eax, edx`. This file lets MSVC rematerialise the 0 as an immediate, so
// it loads `mov eax, [ebp+0x78]; test eax, eax` instead. That costs two
// instructions at the top of the block and then cascades:
//   - edx is free, so MSVC picks `dl` for the flags byte where the original
//     uses `al` (and a memory operand for the same byte in the wheel arm),
//   - the clamp block picks esi/edx where the original picks edi/esi,
//   - in the tail MSVC then needs a zero register, takes edi, kills p.y and
//     reloads it from [esp+0x34] and r1[1] into eax.
// Forcing the constant into a register needs one more literal-0 use in that
// block (an `int zero = 0` local, tried, changes nothing) or one more live
// value in eax; neither was found.
// The one place the original's arithmetic is questionable: `mov ax,
// word [ebp+0x94]; sub ax, word [ebp+0x7c]; add eax, esi` narrows the
// difference to 16 bits and then adds a 32-bit value without sign extension,
// so the high half of the result is whatever eax happened to hold. Reproduced
// here, but it looks like an original bug.
// Shapes that scored worse and are worth knowing about: one merged `v` with a
// single `e->off = v` after the if/else moves `index` into ebx and spills `e`
// (40.8%); a `?:` for the drag arm keeps the shared store but widens the
// subtract to 32 bits (61.6%); duplicating both arms so MSVC cross-jumps the
// tails does not cross-jump at all (49.0%); declaring the old offset
// uninitialised and assigning it in each arm, or hoisting `f94` out of the
// drag arm, both cost the shared store (73.5% and 73.7%).

// deepseek-v4.1 retry (issue 2461): no improvement, still 80.9%. New measured
// fact: naming the FUN_004ab5b0 result in an int local before the test
// (`int ok = FUN_004ab5b0(obj, 3); if (ok == 0)`) DOES materialize the wanted
// 32-bit zero in EDX and reproduces the original per-arm `xor edx, edx` plus
// `mov [ebp+0x78], edx`, but it also turns the call test itself into
// `xor edx, edx` / `cmp eax, edx` instead of `test eax, eax`, which costs 5
// bytes, drops the printed score to 75.1% and raises the differing-instruction
// count from 83 to 114, so the two halves are not separable from the source.
// A `union { int full; short word; }` zero (the 0x4a3ef0 trick) gets a memory
// home here (725 bytes, 73.7%). Re-confirmed dead ends: plain int/short/pointer
// zero locals, phi-shaped zero locals, named-zero comparisons, and reference
// aliases of field_78 all stay byte-identical at 80.9%.

#pragma pack(push, 1)
struct Entry_004a4170 {                // 0x15b bytes, the table of 0x4a23b0
    char unknown_00[0x13];
    short x1;                          // +0x13
    short y1;                          // +0x15
    char unknown_17[0x1b - 0x17];
    unsigned char flags;               // +0x1b, bit 1 = vertical, 0x10 = dead
    char unknown_1c[0x136 - 0x1c];
    short field_136;                   // +0x136, largest usable offset
    char unknown_138[0x140 - 0x138];
    short off;                         // +0x140, the scroll offset
    char unknown_142[0x144 - 0x142];
    int (__stdcall *cb)(void*, int);   // +0x144, called when off changed
    char unknown_148[0x14a - 0x148];
    int field_14a;                     // +0x14a, cb's second argument
    char unknown_14e[0x157 - 0x14e];
    int field_157;                     // +0x157, entry is being dragged
};
#pragma pack(pop)

struct Holder_004a4170 {
    char unknown_00[4];
    Entry_004a4170* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14, set when off changed
};

struct Point_004a4170 {                // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Object_004a4170 {
    char unknown_00[0x18];
    Holder_004a4170* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4170 point;              // +0x3c, the mouse, table relative
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64, -1 when nothing has the focus
    char unknown_68[0x78 - 0x68];
    int field_78;                      // +0x78, non-zero while dragging
    Point_004a4170 saved;              // +0x7c, the mouse when the drag began
    short field_94;                    // +0x94, off when the drag began
};

void __stdcall FUN_0049fc50(Object_004a4170* obj, int index);
void __stdcall FUN_004a23b0(Entry_004a4170* base, int index, int* r1, int* r2);
void __stdcall FUN_004a2580(Object_004a4170* obj, int index);
void __stdcall FUN_004a2be0(Object_004a4170* obj, int index);
int __stdcall FUN_004ab510(Object_004a4170* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Object_004a4170* obj, unsigned int mask);
void __stdcall FUN_004ab690(Object_004a4170* obj, int param_2);

// FUNCTION: 0x4a4170
void __stdcall FUN_004a4170(Object_004a4170* obj, int index)
{
    Entry_004a4170* entries = obj->holder->entries;
    Entry_004a4170* e = &entries[index];
    if (e->flags & 0x10)
        return;
    if (e->field_157)
        return;

    Point_004a4170 p = obj->point;
    p.x -= entries->x1;
    p.y -= entries->y1;
    int r1[4];
    int r2[4];
    FUN_004a23b0(entries, index, r1, r2);

    if (obj->focus == index) {
        if (!FUN_004ab5b0(obj, 3)) {
            obj->focus = -1;
            obj->field_78 = 0;
        }
        int old = e->off;
        if (obj->field_78) {
            short f94 = obj->field_94;
            if (e->flags & 1)
                e->off = f94 - obj->saved.x + p.x;
            else
                e->off = f94 - obj->saved.y + p.y;
        } else {
            int v = e->off;
            if (e->flags & 1) {
                if (p.x < r2[0])
                    v--;
                else if (p.x > r2[2])
                    v++;
            } else {
                if (p.y < r2[1])
                    v--;
                else if (p.y > r2[3])
                    v++;
            }
            e->off = v;
        }
        if (e->off > e->field_136 - 1)
            e->off = e->field_136 - 1;
        if (e->off < 0)
            e->off = 0;
        if (e->off == old)
            return;
        if (obj->holder)
            obj->holder->field_14 = 1;
        FUN_004a2580(obj, index);
        FUN_004a2be0(obj, index);
        if (e->cb)
            e->cb(obj, e->field_14a);
        return;
    }

    if (obj->field_78)
        return;
    if (FUN_004ab510(obj, 1)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 1);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
        return;
    }
    if (FUN_004ab510(obj, 2)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 2);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
    }
}
