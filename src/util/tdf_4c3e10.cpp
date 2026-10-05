// Decompiled by Haiku. Names are provisional.

struct TdfFile {
    char unknown_0[4];
    int field_0x4;

    void ResetCurrentRecord();
};

// FUNCTION: 0x4c3e10
void TdfFile::ResetCurrentRecord()
{
    this->field_0x4 = 0;
}
