// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// Sets the text of the GUI entry named `name`: entries of type 1 and 5 take
// strncpy of 0x80 bytes, type 3 a plain strcpy, and for type 3, when the entry
// is the current one, the text length is stored. Then the list is marked
// changed. The entry is looked up in `holder->unknown_0->entries` but written
// through `holder->entries`, the two chains the machine code takes.
//
// Suspected original bug: the lookup uses `holder->unknown_0->entries` while
// the type 3 write goes to `holder->entries[index]`, so the text can be stored
// into a different array than the one that was searched. The function itself
// walks both chains (the two-level one at the top, the three-level one inside
// case 3), which is the evidence.
//
// 88.9 percent (check.py), everything matches byte for byte except the shared
// tail. The original keeps `obj->changed = 1` in its own block at 0x4a0f0f and
// leaves the case 3 block ending in `mov [edx+0x74],ecx; jmp 0x4a0f0f`.
//
// What MSVC 5 does here (this is the whole remaining difference, 318 bytes
// against 299): it turns the case 3 path into a function-ending path. The case 3
// block is split after the inlined strlen's `not ecx; dec ecx`, and the 13 byte
// chain (the changed store plus the four pops and `ret 0xc`) is copied into
// the gap, so the copied `mov [edx+0xcca],1` even lands *before* the case 3
// `mov [edx+0x74],ecx`; the `jne` edge is then threaded straight to the shared
// tail block at the end. The resulting block order (case 5, case 3, case 1,
// strncpy, tail) and every register, offset and jump size are already the
// original's, so the missing piece is a CFG shape that makes the tail block
// ineligible for that copy, not a source-level difference I can still find.
//
// Tried in this pass, all producing the identical 318 bytes (the `if/else if`
// chain and the `goto`+label form change the dispatch to `cmp`/`je`, and the
// store-in-every-case form gives three stores, both worse): switch case orders
// 1,3,5 and 3,1,5; `case 1:`/`case 5:` merged; an explicit empty `default:`;
// the switch on `(int)`; the length update as one conditional assignment, as
// `index == obj->current`, as an inline helper returning bool, and as an inline
// void helper; `break` in the middle of case 3; `do { } while (0)`, `while (1)`
// and `for (;;)` around the case 3 body; a dummy local inside case 3; the flag
// store in case 3 plus an explicit `return` (case 3 then gets its own epilogue
// copy and the tail store moves to eax); the flag store written twice, through
// a `self` local, with a trailing explicit `return;`; the function as a
// `__stdcall` member; and four spellings of the FindEntry loop (`for`, `while`,
// `break` plus a trailing test, `do/while`).
// Later pass, which narrows the problem: the way case 3 EXITS is not what
// matters. Three structurally different exits all compile to the identical
// 318 bytes with the tail still duplicated:
// - plain `break` after `if (obj->current == index) { ... }` (the baseline).
// - an explicit `goto done;` with a `done:` label immediately before
//   `obj->changed = 1`, the closest source shape to the original's `jmp 0x4a0f0f`.
// - the inverted test with an early exit, `if (obj->current != index) break;`
//   followed by an unconditional `obj->length = strlen(text);`.
// What does change it, for the worse: putting `case 3` first and merging
// `case 1:`/`case 5:` (78.8%), which moves the block order off the original's.
// So the duplication is not a function of case 3's CFG shape. Note what MSVC
// does when it duplicates: the copied store is scheduled immediately after the
// inlined strlen's `dec ecx` and before the length store, and the `jne` edge is
// retargeted past the store. That is a block being inlined into the middle of
// another block, not a tail appended after it. So the lever is probably to stop
// T (the `mov edx,[esp+0x14]; mov [edx+0xcca],1` pair) from being inlinable at
// all, for instance by giving it another predecessor, or a size that makes
// inlining unprofitable.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0e00 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    char unknown_1[1];
    char name[0x10];                    // +0x02
    char unknown_12[0xb6 - 0x12];
    union {
        short count;                    // +0xb6 (entry 0 holds the entry count)
        char text[0x80];
    } u;
    char unknown_136[0x15b - 0x136];
};

struct Holder_004a0e00 {
    Holder_004a0e00* unknown_0;         // +0x00
    Entry_004a0e00* entries;            // +0x04
};

struct Class_004a0e00 {
    char unknown_0[0x18];
    Holder_004a0e00* holder;            // +0x18
    char unknown_1c[0x64 - 0x1c];
    int current;                        // +0x64
    char unknown_68[0x74 - 0x68];
    int length;                         // +0x74
    char unknown_78[0xcca - 0x78];
    int changed;                        // +0xcca
};
#pragma pack(pop)

static inline int FindEntry(Entry_004a0e00* entries, char* name)
{
    for (int i = 1; i < entries[0].u.count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a0e00
void __stdcall FUN_004a0e00(Class_004a0e00* obj, char* name, char* text)
{
    // Declared before the null test on purpose: the original loads the entries
    // pointer before the `je`, so the local has to be computed there.
    Entry_004a0e00* entries = obj->holder->unknown_0->entries;
    if (obj->holder->unknown_0 != 0) {
        int index = FindEntry(entries, name);
        if (index != -1) {
            switch (entries[index].type) {
            case 3:
                strcpy(obj->holder->entries[index].u.text, text);
                if (obj->current == index) {
                    obj->length = strlen(text);
                }
                break;
            case 1:
                strncpy(entries[index].u.text, text, 0x80);
                break;
            case 5:
                strncpy(entries[index].u.text, text, 0x80);
                break;
            }
            obj->changed = 1;
        }
    }
}
