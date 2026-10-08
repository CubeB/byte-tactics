// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x406c70
void __stdcall CopyDwordIfNonNull(int* param_1, int* param_2)
{
    if (param_1 != 0) {
        *param_1 = *param_2;
    }
}
