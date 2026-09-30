// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are provisional.
// BEST 24.0% after 3 check.py runs (586 bytes original, 564 ours). No MATCH.
// The element is Data_004b1ec0 (0x4c):
// flag + six Vec3 of 3 ints, same shape the matched sibling 0x4b1ec0 uses.
// Value arrays are block.e[1], e[3], e[5]; the limits are block.e[0], e[2], e[4],
// reached through a dword index k (0xd, +0x13 per element). Writing the values as
// ptr14[i].block.e[n].v[j] and the limits as ((int*)ptr14)[k + j + c] took the
// score from 13.9% to 24.0%. What still differs:
//  - k is strength-reduced to a BYTE counter (stored 0x34, +0x4c) instead of the
//    original's dword index (0xd, +0x13) used as [base + (k+j)*4 + c]; the
//    original never scales k early.
//  - the value IV is not fused: ours keeps esi = element offset and ebx = 4*j
//    separately ([esi+ebx-0x18]); the original fuses 4*j into esi
//    ([esi+ecx-0x18], base reloaded into ecx) after a one-time add esi,0x28.
//  - frame is 0x1c (7 locals) vs 0x18 (6), and `this` lands in a different rank.
// A while-loop rewrite and all 128 headers kept the score at 24.0%.
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
    while (i < field_8->count) {
        if (ptr14[i].flag != 0) {
            ptr14[i].flag = 0;
            for (int j = 0; j <= 2; j++) {
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
            }
        }
        k += 0x13;
        i++;
    }
}
