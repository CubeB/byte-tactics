// Decompiled by Haiku. Names are provisional.

extern int g_lzssUsePreset;

// FUNCTION: 0x4d1800
int LzssDisablePreset()
{
    g_lzssUsePreset = 0;
    return 0;
}
