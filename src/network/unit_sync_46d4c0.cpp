// Decompiled by Sonnet. Names are provisional.

int __cdecl GetLocalHumanDpid();
int __cdecl GetHostDpid();
int __stdcall SendPacketToPlayer(int a, int b, void* c, int d);

#pragma pack(push, 1)
struct ParamStruct_0046d4c0
{
    char unknown_0[2];
    int field_2; // +0x2
};
#pragma pack(pop)

class UnitSync
{
public:
    char unknown_0[0x58];
    int field_58; // +0x58

    void SendSyncPacket(unsigned int* param_1, ParamStruct_0046d4c0* param_2, int unused);
};

// FUNCTION: 0x46d4c0
void UnitSync::SendSyncPacket(unsigned int* param_1, ParamStruct_0046d4c0* param_2, int unused)
{
    unsigned int val;
    if (field_58 != 0)
        val = *param_1;
    else
        val = GetHostDpid();

    param_2->field_2 = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), val, param_2, 14);
}
