// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x476e90
char* __stdcall FUN_00476e90(char* param_1, int param_2, int param_3)
{
    if (param_3 == 0) {
        return param_1;
    }

    char* p = param_1;
    int found = 0;
    int count = 0;

    while (*p != 0) {
        char c = *p;
        if (c == (char)0xff) break;
        if (found) break;
        p++;
        if (c == '\n') {
            count++;
            if (count == param_3 * param_2) {
                found = 1;
            }
        }
    }

    return found ? p : 0;
}
