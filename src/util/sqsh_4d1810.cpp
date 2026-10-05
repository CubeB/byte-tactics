// Decompiled by Haiku. Names are provisional.

extern int g_lzssUsePreset;

// FUNCTION: 0x4d1810
int LzssEnablePreset()
{
    g_lzssUsePreset = 1;
    return 0;
}
