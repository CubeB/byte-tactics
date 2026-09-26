// Decompiled by Haiku. Names are provisional.

class Class_004ce7c0 {
public:
    void FUN_004ce7c0(int index, unsigned char value);
};

// FUNCTION: 0x4ce7c0
void Class_004ce7c0::FUN_004ce7c0(int index, unsigned char value)
{
    *(unsigned char*)((char*)this + 0x214 + index) = value;
}
