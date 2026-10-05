// Decompiled by Haiku. Names are provisional.

class TdfFile {
public:
    int GetCurrentRecord();
};

// FUNCTION: 0x4c3e20
int TdfFile::GetCurrentRecord()
{
    return *(int*)((char*)this + 4);
}
