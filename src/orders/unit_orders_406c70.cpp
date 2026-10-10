// Decompiled by Haiku. Names are provisional.
// Stays in its own file: unit_orders.cpp inlines it into the repair patrol's vector insert.

// FUNCTION: 0x406c70
void __stdcall CopyDwordIfNonNull(int* param_1, int* param_2)
{
    if (param_1 != 0) {
        *param_1 = *param_2;
    }
}
