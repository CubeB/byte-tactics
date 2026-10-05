// Decompiled by Haiku. Names are provisional.

class CobScript {
public:
    int GetCob();
};

// FUNCTION: 0x4b07a0
int CobScript::GetCob()
{
    return *(int*)((char*)this + 8);
}
