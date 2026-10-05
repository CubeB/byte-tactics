// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class TdfRecord {
public:
    char unknown_0[0x19];
    int field_19;
    int field_1d;

    int GetFieldCount();
};
#pragma pack(pop)

// FUNCTION: 0x4c45c0
int TdfRecord::GetFieldCount()
{
    if (field_19 == 0) {
        return 0;
    }
    return (field_1d - field_19) >> 3;
}
