// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
struct Player
{
    char unknown_0[0x27];
    int field_27;
    char unknown_2b[0x73 - 0x2b];
    char field_73;
    void SetType(int param_1);
};
#pragma pack(pop)

// FUNCTION: 0x463c60
void Player::SetType(int param_1)
{
    field_73 = (char)param_1;
    if (param_1 != 3) {
        *(char*)((char*)field_27 + 0x94) = (char)param_1;
    }
}
