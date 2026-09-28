// Decompiled by space-bunny-free. Names are provisional.
// Sniffs the head of a sound file: 0x4d01b0 returns 1 for a DIGI/HSHD/SDAT
// file, 2 for a RIFF/WAVE file and 0 for anything else. The four byte tag is
// read into one local buffer that MSVC lays over the dead parameter slot.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d01b0
int __stdcall FUN_004d01b0(void* file)
{
    char tag[4];
    FUN_004bb710(file, 0);
    FUN_004bb7c0(file, tag, 4);
    if (strncmp(tag, "DIGI", 4) == 0) {
        FUN_004bb710(file, 8);
        FUN_004bb7c0(file, tag, 4);
        if (strncmp(tag, "HSHD", 4) == 0) {
            FUN_004bb710(file, 0x20);
            FUN_004bb7c0(file, tag, 4);
            if (strncmp(tag, "SDAT", 4) == 0) {
                return 1;
            }
        }
    }
    if (strncmp(tag, "RIFF", 4) == 0) {
        FUN_004bb710(file, 8);
        FUN_004bb7c0(file, tag, 4);
        if (strncmp(tag, "WAVE", 4) == 0) {
            return 2;
        }
    }
    return 0;
}
