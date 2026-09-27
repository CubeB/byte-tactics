// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x40d4f0
void __stdcall FUN_0040d4f0(char* param_1, char* param_2, char* param_3)
{
    char* dest = param_3;
    for (; param_1 != param_2; param_1++) {
        if (dest != 0) {
            *dest = *param_1;
        }
        dest++;
    }
}
