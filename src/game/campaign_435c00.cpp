// Decompiled by Haiku. Names are provisional.

struct Mission {
    char unknown_0[0xc18];
    int field_c18;
    int field_c1c;

    void FUN_00435c00(int param_1);
    void LoadMission(char* param_1);   // a method: it saves ecx and ends in ret 4
};

// FUNCTION: 0x435c00
void Mission::FUN_00435c00(int param_1)
{
    field_c1c = 0;
    field_c18 = param_1;
    LoadMission(0);
}
