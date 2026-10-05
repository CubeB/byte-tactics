// Decompiled by Haiku. Names are provisional.

class Class_004ce7e0 {
public:
    char unknown_0[0x214];
    unsigned char field_214;

    unsigned char GetCategoryOfTrack(int param_1);
};

// FUNCTION: 0x4ce7e0
unsigned char Class_004ce7e0::GetCategoryOfTrack(int param_1)
{
    return *(unsigned char*)((char*)this + param_1 + 0x214);
}
