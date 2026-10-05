// Decompiled by Haiku. Names are provisional.

class UnitScript {
public:
    char unknown_0[0x540];
    void* table;                 // +0x540

    int GetPieceRotation(int param_1, int param_2);
};

// FUNCTION: 0x480cb0
int UnitScript::GetPieceRotation(int param_1, int param_2)
{
    void* ptr = *(void**)((char*)this + 0x540);
    int idx = param_1 + param_1*2;
    int offset = (param_2 + idx*8) + idx;
    unsigned int result = 0;
    result = *(unsigned short*)((char*)ptr + offset*2 + 0x32);
    return result;
}
