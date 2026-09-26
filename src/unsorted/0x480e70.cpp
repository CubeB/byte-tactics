// Decompiled by Haiku. Names are provisional.

class Class_00480e70
{
public:
    char unknown_0[0x540];
    int* field_540;
    unsigned int FUN_00480e70(int param_1);
};

// FUNCTION: 0x480e70
unsigned int Class_00480e70::FUN_00480e70(int param_1)
{
    int* ptr = field_540;
    unsigned char value = *(unsigned char*)((char*)ptr + param_1 * 54 + 0x4a);
    return (value >> 2) & 1;
}
