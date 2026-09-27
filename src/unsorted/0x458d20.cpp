// Decompiled by Sonnet, rewritten without volatile by Opus. Names are provisional.
// Never called or referenced. A loop whose body compiles to nothing leaves
// only the store of its counter's first value, here into the dead slot of the
// pointer parameter; any count-down loop from the first field gives it.

struct Count_00458d20 {
    int count;                  // +0x00
};

// FUNCTION: 0x458d20
void __stdcall FUN_00458d20(int param_1, Count_00458d20* list, int param_3)
{
    for (int i = list->count - 1; i >= 0; i--) {
    }
}
