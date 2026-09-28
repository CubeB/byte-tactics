// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// Finds the GUI layout entry named `name` (0x15b-byte entries whose entry 0
// stores the entry count as a short at +0xb6) and gives it the row array
// `rows` and its count: +0xc6 gets the array, +0xc0 the count, flags +0x1b
// gets bit 7, and +0xbe the index of the topmost row that still fits in the
// entry's height (+0x19) when the row heights are summed from the bottom.
// Each row is 0x18 bytes with an unsigned short height at +0x2; when the
// entry's +0xda is non-zero it overrides the row height with its own value.
// A missing entry is reported and then treated as a null entry.
//
// The row pointer must be computed right after the +0xc6 store (from
// `rows + count - 1`, which MSVC expands as `rows + count*0x18 - 0x18`) and
// before the +0xc0 store; that ordering is what makes MSVC load the count and
// the row array into ecx then edx and keeps the whole tail's stores in the
// original's order. The loop test must be `i > -1` (the original compares with
// -1) rather than `i >= 0` (which it compiles to the sign flag), and the
// `if (i != -1)` form of the entry lookup keeps the success path as the
// fall-through.
//
// 96.9 percent (check.py): 244 bytes against the original 246. Every
// instruction matches except the loop's `remain < 0` test. The original emits
// `sub edi,edx; cmp edi,ebp; jl` (comparing against the zero register it
// already holds for the +0xba/+0xbc stores); this source gets the two-byte
// shorter `sub edi,edx; js`. Tried and rejected, all leaving that one pair as
// the only difference: `remain = remain - h`, a separate `h` local, an
// `int`/`short`/`char` zero local reused for the stores and the test, a
// reference and a struct member for `remain`, `remain < 0L`/`(long)remain`,
// comparing `!e->f_da` and the row field in both branches, the loop with the
// decrements in the body, `while`/`do`/`for(;;)` shapes, `goto`/`return`
// instead of `break`, `i > -1` vs `i >= 0` (the latter breaks the counter
// test), the unsigned/sign-bit spellings, an inlined helper for the height or
// the subtraction, `BT_TOOLCHAIN=msvc5-rtm`, and tools/headers.py. The zero
// register and every other instruction are already the original's.
//
// A second pass added six more shapes for this last pair, none of which moved
// it: the test as `remain <= -1` (96.2%), as `0 > remain`, against a named
// `int zero` local, the subtraction in its own statement with the row height in
// a local, the whole step through a `static inline int Step(int, int)` helper
// (all 96.9%), and accumulating upwards as `used += h; if (used > height)`
// which is much worse at 79.5% because it re-reads the entry's height.
//
// Note the shape of what is left, because it recurs: the original tests the
// subtraction result against a register that already holds zero
// (`cmp edi, ebp; jl`) where MSVC gives the shorter sign-flag form (`js`).
// 0x4b6880 is the same two bytes the other way round, with `cmp eax, ebx`
// against a zero register where we give `test eax, eax`. Both are late
// back-end choices about comparing against a known zero, and in both cases
// every source shape tried, including a named zero local reused for the stores
// and the test, produces the short form. Treat it as compiler state.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a35a0 {                // 0x15b-byte GUI entry
    char unknown_0[0x2];
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short height;                      // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0 holds the entry count)
    char unknown_b8[0xba - 0xb8];
    short f_ba;                        // +0xba
    short f_bc;                        // +0xbc
    short first;                       // +0xbe
    short num;                         // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int f_c6;                          // +0xc6
    char unknown_ca[0xda - 0xca];
    short f_da;                        // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Row_004a35a0 {
    char unknown_0[2];
    unsigned short height;             // +0x2
    char unknown_4[0x18 - 4];
};

struct Table_004a35a0 {
    int unknown_0;
    Entry_004a35a0* entries;           // +0x4
};
#pragma pack(pop)

void __stdcall FUN_004b6290(const char* msg);

static inline int FindEntry(Entry_004a35a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a35a0
void __stdcall FUN_004a35a0(Table_004a35a0* table, char* name,
                            Row_004a35a0* rows, int count)
{
    Entry_004a35a0* entries = table->entries;
    int index = FindEntry(entries, name);
    Entry_004a35a0* e;
    if (index != -1) {
        e = &entries[index];
    } else {
        FUN_004b6290("Error in GUI layout");
        e = 0;
    }
    e->f_c6 = (int)rows;
    Row_004a35a0* row = rows + count - 1;
    e->num = (short)count;
    e->flags |= 0x80;
    e->f_bc = 0;
    e->f_ba = 0;
    e->first = (short)(count - 1);
    int remain = e->height;
    for (int i = count - 1; i > -1; i--, row--) {
        remain -= e->f_da != 0 ? (int)e->f_da : (int)row->height;
        if (remain < 0)
            break;
        e->first = (short)i;
    }
}
