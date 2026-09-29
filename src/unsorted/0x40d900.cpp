// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5 and
// deepseek-v4.1-flash. Names are provisional.
//
// Partial (97.5%): clears the kind byte of every cell in each dirty group of
// eight cells, then clears the dirty masks. One dirty word covers 256 cells
// (0x400 bytes). Every full block is cleared unconditionally; the last block
// is bounds-checked against the cell count.
//
// The first loop's inlined block matches exactly. For the last block the
// registers now match the original (ebx = dirty word, edx = i << 10, edi =
// base pointer) but the instruction order does not: the original reads the
// dirty word and tests it, copies it to ebx, moves i to edx, stores
// dirty[i] = 0, only then loads cells into edi, shifts edx by 10 and adds.
// Ours loads cells into edi first (because the base pointer must be live
// before the block for the allocator to give the shift to edx) and sinks the
// dirty[i] = 0 store after the add. Declaring the base pointer before the
// guard and using it inside is what fixes the register choice (97.5% vs
// 93.7% with the pointer computed inside the guard); the pointer can also be
// assigned cells inside the guard but then the allocator chooses edi for the
// shift and ecx for the base again. Explicit byte offsets, int off = i << 10,
// p += i * 256, a second grid pointer, unsigned i, a separate ClearLast body
// order, and all 128 tools/headers.py header sets give the same 93.7%/97.5%
// code.
//
// Possible original bug: in the last block the bounds check starts `c` at
// `i << 8` again for every group of eight cells instead of at the group's own
// offset (group g holds cells i*256 + g*8 .. +7). So only the first group is
// really compared against the count; later groups clear cells past the end of
// the grid.

struct Cell_0040d900 {
    unsigned char kind;
    char unknown_1[3];
};

struct Grid_0040d900 {
    Cell_0040d900* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int count;                         // +0xc
    unsigned int* dirty;               // +0x10, one bit per 8 cells

    void ClearBlock(int i, int last)
    {
        if (dirty[i]) {
            unsigned int bits = dirty[i];
            dirty[i] = 0;
            Cell_0040d900* p = &cells[i * 256];
            while (bits) {
                if (bits & 1) {
                    int c = i << 8;
                    Cell_0040d900* q = p;
                    for (int k = 8; k; k--) {
                        if (!last || c < count)
                            q->kind = 0;
                        c++;
                        q++;
                    }
                }
                bits >>= 1;
                p += 8;
            }
        }
    }

    void ClearLast(int i)
    {
        Cell_0040d900* p = cells;
        if (dirty[i]) {
            unsigned int bits = dirty[i];
            dirty[i] = 0;
            p += i * 256;
            while (bits) {
                if (bits & 1) {
                    int c = i << 8;
                    Cell_0040d900* q = p;
                    for (int k = 8; k; k--) {
                        if (c < count)
                            q->kind = 0;
                        c++;
                        q++;
                    }
                }
                bits >>= 1;
                p += 8;
            }
        }
    }
};

class Class_0040d900 {
public:
    char unknown_0[0x1c];
    Grid_0040d900 grid;                // +0x1c

    void FUN_0040d900();
};

// FUNCTION: 0x40d900
void Class_0040d900::FUN_0040d900()
{
    int n = ((grid.count + 0xff) >> 8) - 1;
    int i;
    for (i = 0; i < n; i++)
        grid.ClearBlock(i, 0);
    grid.ClearLast(i);
}
