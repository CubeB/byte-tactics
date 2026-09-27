// Decompiled by Sonnet. Names are provisional.

class Class_00428c90 {
public:
    char flag;                          // +0
    char unknown_1[0x7f];
    char* field_80;                     // +0x80
    char* field_84;                     // +0x84
    char* field_88;                     // +0x88
    int field_8c;                       // +0x8c
    int field_90;                       // +0x90
    int field_94;                       // +0x94
    void FUN_00428c90(char* param_1, int param_2);
};

// FUNCTION: 0x428c90
void Class_00428c90::FUN_00428c90(char* param_1, int param_2)
{
    field_80 = param_1;
    field_84 = param_1 + param_2;
    field_88 = param_1;
    field_8c = 0;
    field_90 = 0;
    field_94 = 0x102;
    flag = 0;
}
