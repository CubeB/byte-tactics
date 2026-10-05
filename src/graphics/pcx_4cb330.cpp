// Decompiled by Sonnet. Names are provisional.

struct Blob32_4cb330 {
    int words[8];
};

// FUNCTION: 0x4cb330
void __stdcall SwapPrimitives(Blob32_4cb330* param_1, Blob32_4cb330* param_2)
{
    Blob32_4cb330 tmp = *param_1;
    *param_1 = *param_2;
    *param_2 = tmp;
}
