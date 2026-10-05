// Decompiled by Sonnet. Names are provisional.

#include <string.h>

class Sound {
public:
    char unknown_0[0x1ec];
    int format; // +0x1ec

    void FillSilence(void* dest, unsigned int size);
};

// FUNCTION: 0x4cfe40
void Sound::FillSilence(void* dest, unsigned int size)
{
    if (format != 8) {
        if (format != 0x10) {
            return;
        }
        memset(dest, 0, size);
        return;
    }
    memset(dest, 0x80, size);
}
