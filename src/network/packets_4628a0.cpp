// Decompiled by Sonnet. Names are provisional.

class Class_004628a0 {
public:
    char unknown_0[4];
    unsigned int field_4;  // +4
    void SetSendPacingMs(int param_1);
};

// FUNCTION: 0x4628a0
void Class_004628a0::SetSendPacingMs(int param_1)
{
    unsigned int v = param_1 * 30 + 999;
    field_4 = v / 1000u;
}
