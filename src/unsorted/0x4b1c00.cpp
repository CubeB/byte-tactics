// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free, finished by
// deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by
// claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// MATCH. The last 2 bytes (584 of 586) were the multiply: the original loads the
// parameter into the destination register first (`mov ebp, [esp+0x2c]; imul ebp, edx`,
// and in the turn block `mov ecx, [esp+0x2c]; ... imul ecx, eax`), so which operand of
// the commutative `*` MSVC loads first matters here.
// Levers that did NOT change it (all compiled to the same folded
// `mov ebp, edx; imul ebp, [esp+0x2c]`): swapping the operands (`param_1 * rate` and
// `rate * param_1`), a `static inline` multiply helper called either way, naming the
// rate in a local, `int rate = param_1;`, separate statements for the sum, `r = cur +
// step` instead of `r += step`, and a no-op `(int)` cast on the product.
// What did work is the file-level header block: adding `#include <windows.h>` makes MSVC
// evaluate `param_1 * rate` with the parameter in the destination register, which also
// restores the load order around it in the turn block. Found by tools/permute.py; its
// other mutations (function-scope `int v, want;`, a repeated `param_1 * rate` in the sum)
// are not needed and are left out, since the header alone matches.
#include <windows.h>

struct Table_004b1c00 {
    char unknown_0[8];
    int count;                         // +0x8
};

struct Data_004b1c00 {
    int flag;                          // +0x0
    int e[6][3];                       // +0x4
};

class Class_004b1c00 {
public:
    int field_4;                       // +0x4
    Table_004b1c00* field_8;           // +0x8
    char unknown_c[8];                 // +0xc
    Data_004b1c00* ptr14;              // +0x14
    int field_18;                      // +0x18

    virtual void FUN_00480c50(int, int, int) = 0;  // slot 0
    virtual void FUN_00480ce0(int, int, int) = 0;  // slot 1
    virtual int FUN_00480d50(int, int) = 0;        // slot 2
    virtual int FUN_00480db0(int, int) = 0;        // slot 3
    virtual int FUN_00480df0(int, int) = 0;        // slot 4
    virtual int FUN_00480c30(int, int) = 0;        // slot 5
    virtual int FUN_00480cb0(int, int) = 0;        // slot 6

    void FUN_004b1c00(int param_1);
};

// FUNCTION: 0x4b1c00
// Advance every moving piece by param_1 (a percentage) of its per-axis speed:
// e[1] is the translation speed toward the e[0] limit, e[5] the turn speed with the
// e[4] limit, e[3] the current angle, e[2] the wanted angle (-1 means none). Each
// element that still moves leaves its record's flag set, and any such flag keeps
// field_18 (the "something is still animating" flag) at 1.
void Class_004b1c00::FUN_004b1c00(int param_1)
{
    if (param_1 == 0)
        return;
    if (field_18 == 0)
        return;
    field_18 = 0;
    for (int i = 0; i < field_8->count; i++) {
        if (ptr14[i].flag != 0) {
            ptr14[i].flag = 0;
            for (int j = 0; j <= 2; j++) {
                if (ptr14[i].e[1][j] != 0) {
                    int v = FUN_00480c30(i, j);
                    v += param_1 * ptr14[i].e[1][j];
                    if (ptr14[i].e[1][j] > 0) {
                        if (v >= ptr14[i].e[0][j]) {
                            v = ptr14[i].e[0][j];
                            ptr14[i].e[1][j] = 0;
                        } else
                            ptr14[i].flag = 1;
                    } else {
                        if (v <= ptr14[i].e[0][j]) {
                            v = ptr14[i].e[0][j];
                            ptr14[i].e[1][j] = 0;
                        } else
                            ptr14[i].flag = 1;
                    }
                    FUN_00480c50(i, j, v);
                }
                if (ptr14[i].e[5][j] != 0) {
                    ptr14[i].e[3][j] += ptr14[i].e[5][j];
                    if (ptr14[i].e[5][j] > 0) {
                        if (ptr14[i].e[3][j] >= ptr14[i].e[4][j]) {
                            ptr14[i].e[3][j] = ptr14[i].e[4][j];
                            ptr14[i].e[5][j] = 0;
                        }
                    } else {
                        if (ptr14[i].e[3][j] <= ptr14[i].e[4][j]) {
                            ptr14[i].e[3][j] = ptr14[i].e[4][j];
                            ptr14[i].e[5][j] = 0;
                        }
                    }
                }
                if (ptr14[i].e[3][j] != 0) {
                    int r = FUN_00480cb0(i, j);
                    int cur = r;
                    int step = param_1 * ptr14[i].e[3][j];
                    r += step;
                    int want = ptr14[i].e[2][j];
                    if (want != -1) {
                        if (ptr14[i].e[3][j] > 0) {
                            if ((want - cur + 0x10000) % 0x10000 <= step) {
                                r = want;
                                ptr14[i].e[3][j] = 0;
                            } else
                                ptr14[i].flag = 1;
                        } else {
                            if ((cur - want + 0x10000) % 0x10000 <= -step) {
                                r = want;
                                ptr14[i].e[3][j] = 0;
                            } else
                                ptr14[i].flag = 1;
                        }
                    } else
                        ptr14[i].flag = 1;
                    FUN_00480ce0(i, j, r & 0xffff);
                }
            }
            if (ptr14[i].flag != 0)
                field_18 = 1;
        }
    }
}
