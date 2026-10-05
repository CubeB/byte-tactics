// Decompiled by Opus. Names are provisional.
// Copies size (+0x200) bytes from src into the buffer at +0x215.
#include <string.h>

#pragma pack(push, 1)
class Class_004ce3e0 {
public:
    char unknown_0[0x200];
    unsigned int size;                 // +0x200
    char unknown_204[0x215 - 0x204];
    char buf[1];                       // +0x215

    void FUN_004ce3e0(const void* src);
};
#pragma pack(pop)

// FUNCTION: 0x4ce3e0
void Class_004ce3e0::FUN_004ce3e0(const void* src)
{
    memcpy(buf, src, size);
}
