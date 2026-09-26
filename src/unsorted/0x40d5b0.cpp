// Decompiled by Sonnet. Names are provisional.

struct Pair_40d5b0 {
    int a;
    int b;
};

// FUNCTION: 0x40d5b0
void __stdcall FUN_0040d5b0(Pair_40d5b0* param_1, unsigned int param_2, Pair_40d5b0* param_3)
{
    for (; param_2 > 0; param_2--) {
        if (param_1 != 0) {
            param_1->a = param_3->a;
            param_1->b = param_3->b;
        }
        param_1++;
    }
}
