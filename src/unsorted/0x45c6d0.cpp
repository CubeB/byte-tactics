// Decompiled by Haiku. Names are provisional.

extern volatile int g_game;

// FUNCTION: 0x45c6d0
void FUN_0045c6d0()
{
    int ecx = *(int *)&g_game;
    int eax = 10;
    *(int *)((char *)ecx + 0x37f23) = eax;

    int edx = *(int *)&g_game;
    *(int *)((char *)edx + 0x37f27) = eax;

    ecx = *(int *)&g_game;
    *(short *)((char *)ecx + 0x38a4b) = (short)eax;

    edx = *(int *)&g_game;
    *(short *)((char *)edx + 0x38a4d) = (short)eax;

    ecx = *(int *)&g_game;
    *(char *)((char *)ecx + 0x1434d) = 0x20;

    edx = *(int *)&g_game;
    *(int *)((char *)edx + 0x37efa) = 0;

    ecx = *(int *)&g_game;
    *(char *)((char *)ecx + 0x37f17) = (char)eax;

    edx = *(int *)&g_game;
    *(char *)((char *)edx + 0x37f18) = 5;
}
