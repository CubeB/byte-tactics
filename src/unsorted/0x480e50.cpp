// Decompiled by Haiku. Names are provisional.

class Class_00480e50
{
public:
    unsigned char FUN_00480e50(int param1);
};

// FUNCTION: 0x480e50
unsigned char Class_00480e50::FUN_00480e50(int param1)
{
    void* ptr = *(void**)((char*)this + 0x540);
    int index = param1 + param1 * 2;
    index = index + index * 8;
    unsigned char val = *(unsigned char*)((char*)ptr + index * 2 + 0x4a);
    return (val >> 1) & 1;
}
