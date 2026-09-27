// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// Partial (93.7%): clears the kind byte of every cell in each dirty group of
// eight cells, then clears the dirty masks. One dirty word covers 256 cells;
// the last word is bounds-checked against the cell count.
//
// Testing `dirty[i]` and then copying it into `bits` (rather than testing
// `bits`) is what keeps the duplicated `test ebx, ebx` before each group
// loop. The main loop now matches exactly. What still differs is the start
// of the last block: the original copies the dirty word into ebx straight
// after the test, computes `i << 10` in edx and loads the cell array into
// edi (`add edi, edx`); here the copy comes later, so `i << 10` is built in
// edi and the cell array goes through ecx. Swapping the add's operands, a
// static inline row helper, shared or separate `bits`/`p` locals, explicit
// copies, do/while or for loops, and every header set gave the same code.
//
// Possible original bug: in the last block the bounds check uses `(i << 8) + k`
// for every group, without the group's own offset (8 cells per mask bit), so
// only the first 8 cells of the block are really checked against the count.

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
    grid.ClearBlock(i, 1);
}
