// Decompiled by Sonnet. Names are provisional.

struct Quad { int a, b, c, d; };

// FUNCTION: 0x4c69c0
void __stdcall FUN_004c69c0(int* param_1)
{
    Quad q;
    q.a = 0;
    q.b = 0;
    q.c = param_1[0] - 1;
    q.d = param_1[1] - 1;
    *(Quad*)(param_1 + 7) = q;
}
