// Decompiled by Sonnet. Names are provisional.

extern float g_lightX;
extern float g_lightY;
extern float g_lightZ;

// FUNCTION: 0x4597f0
void __stdcall SetLightVector(int param_1, int param_2, int param_3)
{
    g_lightX = (float)param_1 * 0.01f;
    g_lightY = (float)param_2 * 0.01f;
    g_lightZ = (float)param_3 * 0.01f;
}
