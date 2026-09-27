// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

// Memory block description, 0x30 bytes. Same layout as the type used by
// FUN_004d8790 and FUN_004daa30.
class Class_004d87f0 {
public:
    int field_0;                 // +0x00
    int field_4;                 // +0x04
    int field_8;                 // +0x08
    char unknown_c[0x24];        // +0x0c
    Class_004d87f0(void);
};

extern void __cdecl FUN_004daa30(unsigned int address, Class_004d87f0* a, Class_004d87f0* b);
extern void __cdecl FUN_004d8790(Class_004d87f0 info, char* buf, int unused);

// FUNCTION: 0x4dab10
void __cdecl FUN_004dab10(unsigned int address, char* buf, int unused)
{
    Class_004d87f0 local1;
    Class_004d87f0 local2;
    FUN_004daa30(address, &local1, &local2);
    if (local1.field_0 != 0) {
        bool inRange = address >= (unsigned int)local1.field_0
                    && address < (unsigned int)local1.field_4 + (unsigned int)local1.field_0;
        if (inRange) {
            FUN_004d8790(local1, buf, unused);
            return;
        }
    }
    strcpy(buf, "Address is not within an allocated block.");
}
