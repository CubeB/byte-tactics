// Decompiled by Sonnet. Names are provisional.

extern void* g_noDirectSound;
extern void* g_useWindowsSound;
extern char* g_game;

class Sound {
public:
    void StopAllBuffers();
};

extern void StopWindowsSound();

// FUNCTION: 0x47f750
void StopAllSounds()
{
    if (g_noDirectSound == 0) {
        Sound* obj = *(Sound**)((char*)g_game + 0x10);
        obj->StopAllBuffers();
    }
    if (g_useWindowsSound != 0) {
        StopWindowsSound();
    }
}
