// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
// Walks a directory tree (the search 0x4bc4b0 allocates, the same one 0x4bcb50
// uses) and, for every plain file, marks the matching entry of every open
// HAPI archive: the entry named "path + file name" is looked up with
// FUN_004bb4e0 in each archive's directory (from the search's current archive
// index on) and gets bit 2 set unless bit 1 is already set. Sub directories
// other than "." and ".." are entered with the path extended by "name\\".
//
// NOT MATCHED: 99.2%, size exact. Exactly one instruction differs, the target
// of the file loop's entry test at 0x4be5d4. When the loop has nothing to do
// (`i >= d->count` on entry) the original jumps PAST the reload of the search
// handle into esi at 0x4be66d and lands on the FUN_004bc640 argument setup at
// 0x4be672; here the guard jumps to the reload. The join block 0x4be66d has
// four predecessors (the two strcmp matches at 0x4be4dd and 0x4be51a, the
// recursion at 0x4be5ba and the loop latch falling through from 0x4be667), and
// the original simply lets the fifth edge, the guard, skip it, which is safe
// because on that edge esi still holds the handle.
//
// A second session (this one) spent about thirty shapes on that one edge and
// every one of them still emits `jge` to the reload, so all of these are ruled
// out, not merely untried:
//   - the inner loop: `for (; i < n; i++)`, `for (i = (i < 0 ? 0 : i + 1); ...)`,
//     `while (i < n) { ...; i++; }`, `if (i < n) do { ... } while (++i < n);`,
//     `for (;;) { if (i >= n) break; ...; i++; }`, `while (1) { if (...) break;
//     ...; i++; }` (that last one is the only one that changes anything: it
//     stops the rotation and drops to 68.0), `for (i = i; ...)`;
//   - the clamp: the `if (i < 0) i = 0; else i++;` three-statement form, the
//     `i = (i < 0) ? 0 : i + 1;` assignment and the clamp as the `for` init;
//   - `i` as int, unsigned (94.1: it also breaks the two strcmp joins) and long;
//   - the handle as a `Find*` throughout with `(Find*)-1` compared, and the
//     local count and the local name pointer pulled out of the loop (61.8 and
//     76.8: both change the whole frame);
//   - the outer loop: `do { } while (cond)`, `while (1) { ...; if (cond ==
//     -1) break; }`, the `if (h == -1) return;` guard versus the whole body and
//     the tail nested inside an `if (h != -1)`, and the whole if/else wrapped in
//     an extra nested block;
//   - the if/else: the two branches swapped, `else if (!(attrib & 0x10))`,
//     `if ((attrib & 0x10) != 0)`, and the file branch ending in `continue`
//     instead of an `if` guard (which is what the original's branchy strcmp
//     wants but does not move the edge either).
// So the edge is not a loop-shape, type, branch-order or local-order effect. The
// only shape that even touched it is one that adds a live local to the loop
// (`Entry* e = 0;` before the body, 92.3), and that goes the wrong way.
//
// A third session (space-bunny-free, about 110 further shapes) confirmed the
// wall and closed off the "second definition of the handle" idea the second
// session left open. Every one of these compiles to the same graph, with the
// guard still jumping to the reload at 0x4be66d:
//   - the shape of the MATCHED neighbour 0x4bca30, `if (h != -1) { do { } while
//     (FUN_004bc640(h, &fd) != -1); if (h != 0) { Find* f = (Find*)h; ... } }`,
//     with the handle block scoped, the whole tail in a block, the epilogue in a
//     block, `Find* f` assigned in the file branch, and `int t = ...; int h = t;`
//     or `int hh = h;` for a second name on the handle;
//   - the loop once more as `while (i < d->count)`, `if (i < d->count) do {} while`,
//     `for(;;)` with a `break`, an empty `for` init with `i++` in the body, `i !=
//     d->count`, `d->count > i`, an `if` guard wrapped round the `for`, and a
//     `goto` that skips the loop (all byte-identical or 94-98);
//   - the clamp as `i = (i < 0) ? 0 : i + 1;`, as `if (i >= 0) i++; else i = 0;`,
//     and with `i` declared then assigned;
//   - the flag update through a local copy of `e->flags`, and the `e` test as a
//     nested `if` or as two `continue`s;
//   - the dir branch as `strcmp(...) && strcmp(...)`, as `!(strcmp(...) == 0 ||
//     strcmp(...) == 0)`, with the operands swapped, and the attrib test as
//     `(fd.attrib & 0x10) == 0x10` or `& 0x10u` (the last two change the bytes);
//   - the outer loop as `while (h != -1) { ... }`, as `for (; h != -1; )`, and as
//     a peeled `if (1) { } while (...) { }`;
//   - a `static inline` helper for the body and for the whole inner loop (both
//     add a second char[256] and drop to 91.9, which is worth knowing: the
//     helper's buffer cannot share the frame slot);
//   - all 768 sets of tools/headers.py --cpp (best 99.2, every set the same),
//     12 permutations of the five callee declarations, four orders of the three
//     local declarations, and dead locals (`int depth = 0;`) in each branch,
//     which do not even grow the frame;
//   - 160 random combinations of 19 independent knobs above. The family is
//     flat at 99.2 for every shape that keeps the bytes, with a second cluster
//     at 96 and a third at 61 to 75 (the shapes that add a live local to the
//     loop), and nothing at 100.
// One reading of the diff that is worth not re-deriving: the fourth line
// (`-call 0x4be400` / `+call <addr>`) is not a difference. check.py only fills
// the placeholder addresses into the disassembly once the bytes match, so until
// they do, every relocated field prints as `<addr>`; the self-call is fine and
// the guard's displacement is the only wrong byte in the 699.
// What is left is a back end decision, not a front end one: MSVC 5 either
// splits the join block and leaves the loop's exit edge pointing past the copy
// (the original) or puts the copy at the head of the join and retargets that
// edge too (this file). The slot-sharing trick that produced the same shape at
// 0x4af320 cannot be tried here, because the frame is exactly h (4) + buf
// (0x100) + fd (0x108) with nothing spare to share.
//
// A fifth session (space-bunny-free) built a free scoring generator
// (build/scratch/0x4be400/gen.py, ~0.35 s per variant, ~500 variants run) over
// twelve independent knobs: the clamp in four spellings, the inner loop in six
// shapes, the outer loop in three, the early return versus an `if (h != -1)`
// wrap, the attrib test in three, the "." / ".." test in three, the entry test
// in four, the tail in three, `i` as int and long, the handle as int and as a
// pointer, and a local `int` for the recursion's state argument. Every variant
// that keeps the bytes is 99.2: the whole family is flat, including the loop
// shapes that were not previously enumerated (`while (i < d->count)`, `for (;;)`
// with a break, `if (i < d->count) do {} while`, `for (i = i; ...)`), which all
// compile to the same graph. Worth recording for the next attempt: the family
// being flat means the guard's successor is decided after the front end has
// thrown the shape away, so a source level search cannot reach it.
// The mechanism, as far as the bytes show: h lives in esi from 0x4be489 and is
// spilled to [esp+0x10] at 0x4be48e; the file loop body clobbers esi, so the
// tail's use of h needs `mov esi,[esp+0x10]`. On the guard edge esi still holds
// h, and the original skips the copy on that edge only, which means MSVC did
// per edge copy insertion there and did not here.
// A fourth session (Sonnet 5.5, #1105) confirmed the wall at 99.2%: the dir
// branch ending in `continue` (file branch after it), the file branch ending in
// `continue`, `h = h;` before the loop condition, the file loop as a helper that
// takes the shared buf as a parameter (three shapes, so no second char[256]),
// and the two branches as two separate `if`s all give the same 99.2%
// or worse. The reading that fits the bytes: the reload of the handle is
// placed on the loop's exit edge and on the strcmp edges only, and the guard edge
// is left alone because esi still holds h there. No source shape tried moves that.
// A sixth session (deepseek-v4.1-flash) re-checked and probed one more shape (the
// inner loop as `if (i < count) do { body; } while (++i < count);` scored with
// `check.py --sym`): still exactly 99.2, the guard's `jge` still points at the
// reload at 0x4be66d. The file is left at the best of the flat 99.2 family; the
// only wrong byte in the 699 is the guard's jump displacement.
#include <io.h>
#include <string.h>

#pragma pack(push, 1)
struct Find_004be400 {
    char dir[0x100];         // +0x000
    char pattern[0x100];     // +0x100
    int state;               // +0x200
    char recursive;          // +0x204
    long handle;             // +0x205
    int index;               // +0x209
};

struct Entry_004be400 {                // 9 bytes
    int field_0;
    int field_4;
    unsigned char flags;               // +0x8
};

struct Table_004be400 {
    int count;
    Entry_004be400* entries;
};

struct Header_004be400 {
    char unknown_0[0x10];
    Table_004be400* table;             // +0x10
};

struct File_004be400 {
    char unknown_0[8];
    Header_004be400* header;           // +0x8
};

struct Display_004be400 {
    char unknown_0[0x618];
    File_004be400** files;             // +0x618
    int count;                         // +0x61c
};
#pragma pack(pop)

Display_004be400* FUN_004b6220();
Entry_004be400* __stdcall FUN_004bb4e0(Table_004be400* table, char* name);
int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(int handle, struct _finddata_t* fd);
void FUN_004d85a0(void* p);

// FUNCTION: 0x4be400
void __stdcall FUN_004be400(char* path, int state, int recursive)
{
    Display_004be400* d = FUN_004b6220();
    char buf[0x100];
    struct _finddata_t fd;

    strcpy(buf, path);
    strcat(buf, "*");
    int h = FUN_004bc4b0(buf, &fd, state, recursive);
    if (h == -1)
        return;
    do {
        if (fd.attrib & 0x10) {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0) {
                strcpy(buf, path);
                strcat(buf, fd.name);
                strcat(buf, "\\");
                FUN_004be400(buf, ((Find_004be400*)h)->state, 0);
            }
        } else {
            int i = ((Find_004be400*)h)->state;
            if (i < 0)
                i = 0;
            else
                i++;
            for (; i < d->count; i++) {
                strcpy(buf, path);
                strcat(buf, fd.name);
                Entry_004be400* e = FUN_004bb4e0(d->files[i]->header->table, buf);
                if (e && !(e->flags & 1))
                    e->flags |= 2;
            }
        }
    } while (FUN_004bc640(h, &fd) != -1);
    Find_004be400* f = (Find_004be400*)h;
    if (f) {
        if (f->state < 0)
            _findclose(f->handle);
        FUN_004d85a0(f);
    }
}
