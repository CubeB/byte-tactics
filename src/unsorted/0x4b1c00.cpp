// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// BEST 25.0% (586 bytes original, 569 ours). No MATCH.
// Added the final per-element flag check that sets field_18, which was absent in
// the inherited draft. Rewriting the guarded outer loop as do/while removed its
// duplicate entry test and raised the score from 24.3%. An inner do/while tied.
// Main remaining differences are register and stack allocation: the
// target uses a 0x18-byte frame, keeps the dword index k at 0xd and advances it
// by 0x13, and uses a byte-offset cursor advanced by 0x4c per record. MSVC folds
// k into a byte counter in this source; callback and inner-loop register
// lifetimes also diverge.
#include <stdlib.h>

struct Vec3_004b1c00 {
    int v[3];
};

struct Block_004b1c00 {
    Vec3_004b1c00 e[6];
};

struct Data_004b1c00 {
    int flag;                          // +0x0
    Block_004b1c00 block;              // +0x4
};

struct Table_004b1c00 {
    char unknown_0[8];
    int count;                         // +0x8
};

class Class_004b1c00 {
public:
    char unknown_0[0x4];              // the vtable pointer
    Table_004b1c00* field_8;           // +0x8
    char unknown_c[0x14 - 0xc];
    Data_004b1c00* ptr14;              // +0x14
    int field_18;                      // +0x18

    virtual void FUN_00480c50(int, int, int) = 0;  // slot 0
    virtual void FUN_00480ce0(int, int, int) = 0;  // slot 1
    virtual void FUN_00480d50(int, int) = 0;       // slot 2
    virtual void FUN_00480db0(int, int) = 0;       // slot 3
    virtual void FUN_00480df0(int, int) = 0;       // slot 4
    virtual int FUN_00480c30(int, int) = 0;        // slot 5
    virtual int FUN_00480cb0(int, int) = 0;        // slot 6

    void FUN_004b1c00(int param_1);
};

// FUNCTION: 0x4b1c00
void Class_004b1c00::FUN_004b1c00(int param_1)
{
    if (param_1 == 0)
        return;
    if (field_18 == 0)
        return;
    field_18 = 0;
    if (field_8->count <= 0)
        return;

    int k = 0xd;
    int i = 0;
    do {
        if (ptr14[i].flag != 0) {
            ptr14[i].flag = 0;
            int j = 0;
            do {
                if (ptr14[i].block.e[1].v[j] != 0) {
                    int v = FUN_00480c30(i, j) + param_1 * ptr14[i].block.e[1].v[j];
                    int lim = ((int*)ptr14)[k + j - 12];
                    if (ptr14[i].block.e[1].v[j] > 0) {
                        if (v < lim)
                            ptr14[i].flag = 1;
                        else {
                            v = lim;
                            ptr14[i].block.e[1].v[j] = 0;
                        }
                    } else {
                        if (v > lim)
                            ptr14[i].flag = 1;
                        else {
                            v = lim;
                            ptr14[i].block.e[1].v[j] = 0;
                        }
                    }
                    FUN_00480c50(i, j, v);
                }
                if (ptr14[i].block.e[5].v[j] != 0) {
                    ptr14[i].block.e[3].v[j] += ptr14[i].block.e[5].v[j];
                    if (ptr14[i].block.e[5].v[j] > 0) {
                        if (ptr14[i].block.e[3].v[j] >= ((int*)ptr14)[k + j]) {
                            ptr14[i].block.e[3].v[j] = ((int*)ptr14)[k + j];
                            ptr14[i].block.e[5].v[j] = 0;
                        }
                    } else {
                        if (ptr14[i].block.e[3].v[j] <= ((int*)ptr14)[k + j]) {
                            ptr14[i].block.e[3].v[j] = ((int*)ptr14)[k + j];
                            ptr14[i].block.e[5].v[j] = 0;
                        }
                    }
                }
                if (ptr14[i].block.e[3].v[j] != 0) {
                    int r = FUN_00480cb0(i, j) + param_1 * ptr14[i].block.e[3].v[j];
                    int want = ((int*)ptr14)[k + j - 6];
                    if (want != -1) {
                        if (ptr14[i].block.e[3].v[j] > 0) {
                            if ((abs(want - r + 0x10000) & 0xffff) > param_1 * ptr14[i].block.e[3].v[j])
                                ptr14[i].flag = 1;
                            else {
                                r = want;
                                ptr14[i].block.e[3].v[j] = 0;
                            }
                        } else {
                            if ((abs(r - want + 0x10000) & 0xffff) > -(param_1 * ptr14[i].block.e[3].v[j]))
                                ptr14[i].flag = 1;
                            else {
                                r = want;
                                ptr14[i].block.e[3].v[j] = 0;
                            }
                        }
                    } else {
                        ptr14[i].flag = 1;
                    }
                    FUN_00480ce0(i, j, r & 0xffff);
                }
                j++;
            } while (j <= 2);
            if (ptr14[i].flag != 0)
                field_18 = 1;
        }
        k += 0x13;
        i++;
    } while (i < field_8->count);
}
