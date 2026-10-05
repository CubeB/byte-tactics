// Decompiled by Haiku. Names are provisional.
extern int g_useWindowsSound;
extern int g_noDirectSound;

// FUNCTION: 0x47efd0
void SetUseWindowsSound()
{
    g_useWindowsSound = 1;
    g_noDirectSound = 1;
}
