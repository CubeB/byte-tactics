// Decompiled by Haiku. Names are provisional.

class LosTable
{
public:
    char unknown_0[0x4];
    int field_4;
    int field_8;

    int GetLosLineCount();
};

// FUNCTION: 0x4335c0
int LosTable::GetLosLineCount()
{
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 4;
}
