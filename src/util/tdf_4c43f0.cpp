// Decompiled by Sonnet. Names are provisional.

int __stdcall ComputeChecksum(unsigned int a, unsigned int b);

#pragma pack(push, 1)
class TdfRecord {
public:
    char unknown_0[0x25];
    int field_25;                       // +0x25
    void ComputeRecordChecksum(unsigned int param_1, unsigned int param_2);
};
#pragma pack(pop)

// FUNCTION: 0x4c43f0
void TdfRecord::ComputeRecordChecksum(unsigned int param_1, unsigned int param_2)
{
    if (param_1 <= param_2) {
        field_25 = ComputeChecksum(param_1, param_2 - param_1 - 1);
    } else {
        field_25 = 0;
    }
}
