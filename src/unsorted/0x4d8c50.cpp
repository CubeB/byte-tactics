// Decompiled by deepseek-v4.1. Names are provisional.
#include <string.h>

// Memory block description, 0x30 bytes, passed by value to FUN_004d8790.
struct BlockInfo_004d8790 {
    int address;                     // +0x00
    int size;                        // +0x04
    int allocNumber;                 // +0x08
    char name[0x24];                 // +0x0c
};

// Records one allocation or free site (the object at +0x30 / +0xbc).
class Class_004d89b0 {
public:
    char unknown_0[0x40];
    int line;                        // +0x40
    void FUN_004d89b0(char* buf, int size);
};

class Class_004d8d40 {
public:
    BlockInfo_004d8790 block;        // +0x00
    char unknown_30[0x8c];           // +0x30
    char unknown_bc[0x8c];           // +0xbc
    void FUN_004d8c50(char* buf, int size, char flag);
};

extern void __cdecl FUN_004d8790(BlockInfo_004d8790 info, char* buf, int unused);

// FUNCTION: 0x4d8c50
void Class_004d8d40::FUN_004d8c50(char* buf, int size, char flag)
{
    FUN_004d8790(block, buf, size);
    strcat(buf, ", allocated from:");
    char* p = buf + strlen(buf);
    ((Class_004d89b0*)((char*)this + 0x30))->FUN_004d89b0(p, size - strlen(buf));
    if (flag) {
        strcat(buf, "\n\tfreed from:");
        char* q = buf + strlen(buf);
        ((Class_004d89b0*)((char*)this + 0xbc))->FUN_004d89b0(q, size - strlen(buf));
    }
}
