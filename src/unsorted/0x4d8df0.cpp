// Decompiled by Sonnet. Names are provisional.

void __cdecl FUN_004d8d70(int, int);

struct S {
    char unknown_0[4];
    int val;  // +4
};

__declspec(thread) S DAT_00529f48;

// FUNCTION: 0x4d8df0
int __cdecl FUN_004d8df0(void)
{
    FUN_004d8d70(0, 0);
    return DAT_00529f48.val;
}
