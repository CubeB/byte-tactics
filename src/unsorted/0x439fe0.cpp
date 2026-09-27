// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x439fe0
void __stdcall FUN_00439fe0(char* param_1, char* param_2)
{
    char** pp = (char**)(param_1 + 0x5c);
    char* node = *pp;
    for (; node != param_2; node = *(char**)(node + 0x4a)) {
        pp = (char**)(node + 0x4a);
    }
    node = *(char**)(param_2 + 0x4a);
    *pp = node;
    for (; node != 0; node = *(char**)(node + 0x4a)) {
        pp = (char**)(node + 0x4a);
    }
    *pp = param_2;
    *(char**)(param_2 + 0x4a) = 0;
}
