// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// Retry (mimo-v2.6-pro, part 2). Re-derived the two big issues from the diff:
// (1) A hoisted zero register (ebx): the old block-2 nested if/else (duplicated
// clamp) makes MSVC materialise one zero at function entry, so every compare
// becomes cmp x, ebx and every zero store goes through ebx. Collapsing block 2
// to one clamp behind a single condition (here the !(ternary) form, which keeps
// the clamp shared at the end) removes the hoist: the entry tests go back to
// test/immediate and block 1 regains its regional xor zero (as at 0x4b1c8d).
// (2) Still unresolved: the limit index IV is strength-reduced. Ours keeps k
// as a byte offset (init 0x34, step 0x4c, plus a per-j 4*(k+j) walk in slot
// [esp+0x10]); the target keeps k as an int index (init 0xd, step 0x13 in
// [esp+0x10]) and adds j at each use with [base + (k+j)*4 + disp] addressing.
// Every index spelling tried (j+k, (k-12)+j, k+(j-12), (p+k-12)[j], *p+k+j-12,
// for-header k update) value-numbers to the same scaled form, so the lever is
// elsewhere in the surrounding shape.
// Also still off vs the target: block 1 wants the clamp duplicated in both
// rate-sign arms with the flag=1 block shared after them (goto form gets that
// layout but loads the limit once; the target loads it inside each arm), block 2
// wants direct branches (jle arm2 / jl end / jmp clamp / jg end) instead of
// setcc, e3 += e5 should fold to add [mem], reg instead of load-add-store, and
// the j/i spill slots (target: j in [esp+0x18], i in [esp+0x1c]) and the
// ebp=i / ebx=j callee-saved assignment follow from those.
// The wrap idiom at 0x4b1d66/0x4b1d9d ("cdq; xor; sub; and eax,0xffff; xor;
// sub") is signed "x % 0x10000", so the branches spell (want - cur + 0x10000)
// % 0x10000 and (cur - want + 0x10000) % 0x10000. The three limits are raw
// int-index reads ((int*)ptr14)[k + j - 12], [k + j], [k + j - 6] with k = 0xd
// advanced by 0x13 per record.
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
void Class_004b1c00::FUN_004b1c00(int param_1)
{
    if (param_1 == 0)
        return;
    if (field_18 == 0)
        return;
    field_18 = 0;
    int k = 0xd;
    for (int i = 0; i < field_8->count; i++) {
        if (ptr14[i].flag != 0) {
            ptr14[i].flag = 0;
            for (int j = 0; j <= 2; j++) {
                if (ptr14[i].e[1][j] != 0) {
                    int v = FUN_00480c30(i, j) + param_1 * ptr14[i].e[1][j];
                    if (ptr14[i].e[1][j] > 0) {
                        if (v < ((int*)ptr14)[k + j - 12])
                            ptr14[i].flag = 1;
                        else {
                            v = ((int*)ptr14)[k + j - 12];
                            ptr14[i].e[1][j] = 0;
                        }
                    } else {
                        if (v > ((int*)ptr14)[k + j - 12])
                            ptr14[i].flag = 1;
                        else {
                            v = ((int*)ptr14)[k + j - 12];
                            ptr14[i].e[1][j] = 0;
                        }
                    }
                    FUN_00480c50(i, j, v);
                }
                if (ptr14[i].e[5][j] != 0) {
                    ptr14[i].e[3][j] += ptr14[i].e[5][j];
                    if (!(ptr14[i].e[5][j] > 0 ? ptr14[i].e[3][j] < ((int*)ptr14)[k + j] : ptr14[i].e[3][j] > ((int*)ptr14)[k + j])) {
                        ptr14[i].e[3][j] = ((int*)ptr14)[k + j];
                        ptr14[i].e[5][j] = 0;
                    }
                }
                if (ptr14[i].e[3][j] != 0) {
                    int cur = FUN_00480cb0(i, j);
                    int r = cur + param_1 * ptr14[i].e[3][j];
                    int want = ((int*)ptr14)[k + j - 6];
                    if (want == -1)
                        ptr14[i].flag = 1;
                    else if (ptr14[i].e[3][j] > 0) {
                        if ((want - cur + 0x10000) % 0x10000 > param_1 * ptr14[i].e[3][j])
                            ptr14[i].flag = 1;
                        else {
                            r = want;
                            ptr14[i].e[3][j] = 0;
                        }
                    } else {
                        if ((cur - want + 0x10000) % 0x10000 > -(param_1 * ptr14[i].e[3][j]))
                            ptr14[i].flag = 1;
                        else {
                            r = want;
                            ptr14[i].e[3][j] = 0;
                        }
                    }
                    FUN_00480ce0(i, j, r & 0xffff);
                }
            }
            if (ptr14[i].flag != 0)
                field_18 = 1;
        }
        k += 0x13;
    }
}
