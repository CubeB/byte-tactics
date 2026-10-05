// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <dsound.h>

void __cdecl FUN_004d85a0(void* p);

class Class_004cf4d0 {
public:
    char unknown_0[0x30];
    int count;                              // +0x30
    char unknown_34[4];
    IDirectSoundBuffer* buffers[0x20];      // +0x38
    char unknown_b8[0x80];
    int extra[0x20];                        // +0x138

    void FUN_004cf4d0(IDirectSoundBuffer** set);
};

// FUNCTION: 0x4cf4d0
void Class_004cf4d0::FUN_004cf4d0(IDirectSoundBuffer** set)
{
    if (set == 0)
        return;
    for (int i = 0; i < 4; i++) {
        if (set[i] != 0) {
            set[i]->Stop();
            set[i]->Release();
            for (int j = 0; j < 0x20; j++) {
                if (buffers[j] == set[i]) {
                    buffers[j] = 0;
                    extra[j] = 0;
                    count--;
                    break;
                }
            }
        }
    }
    FUN_004d85a0(set);
}
