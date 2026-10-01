// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by claude-sonnet-5-5. Names are provisional.
// 88.4 percent (584 of 586 bytes). Levers found (claude-sonnet-5-5):
// - Invert every clamp test (`if (v >= lim) { clamp } else flag = 1;`): MSVC then
//   emits `jl FLAG; clamp; jmp` with one shared flag store, and the limit
//   induction variable starts at 0xd with a -0x30 displacement as in the original.
//   Plain struct reads (e[0], e[4], e[2]) are enough; no explicit k.
// - Add the call result in a separate statement (`v += param_1 * rate`), so the
//   rate is loaded after the call.
// - In the turn block the call result is first stored in r and then copied
//   (`int r = call; int cur = r; r += step;`): that decides which of
//   i/j gets ebp/ebx and which locals are spilled (72 percent to 88).
// Still differs: the original multiplies with the parameter loaded into a
// register first (`mov ebp, [esp+0x2c]; imul ebp, edx` and in the turn block
// `mov ecx, [esp+0x2c]; ... imul ecx, eax`); ours folds the parameter as a
// memory operand (`mov ebp, edx; imul ebp, [esp+0x2c]`), which also reorders the
// loads around it in the turn block. Operand order, locals, helper inlines and
// `*=` forms all compile the same.
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