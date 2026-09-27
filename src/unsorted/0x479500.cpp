// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

// FUNCTION: 0x479500
int __cdecl FUN_00479500()
{
    int* g_game = (int*)DAT_00511de8;
    int count = 0;
    int num = *(int*)((int)g_game + 0x38d81);

    if (num > 0) {
        int* arr = *(int**)((int)g_game + 0x29a0);
        while (num != 0) {
            if (*arr == 1) {
                count++;
            }
            arr = (int*)((char*)arr + 0x18);
            num--;
        }
    }

    return count;
}
