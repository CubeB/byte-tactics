// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x4b6e80
double __stdcall DotProduct(float param_1, float param_2, float param_3,
                               float param_4, float param_5, float param_6)
{
    double result = param_4 * param_1;
    result = result + param_5 * param_2;
    result = result + param_6 * param_3;
    return result;
}
