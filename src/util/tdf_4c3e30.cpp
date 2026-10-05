// Decompiled by Haiku. Names are provisional.

class TdfFile {
public:
    char unknown_0[0x4];
    int field_4;

    void SetCurrentRecord(int val);
};

// FUNCTION: 0x4c3e30
void TdfFile::SetCurrentRecord(int val)
{
    field_4 = val;
}
