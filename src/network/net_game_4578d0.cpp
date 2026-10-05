// Decompiled by Haiku. Names are provisional.

class PacketManager
{
public:
    void SetDefaultSendPacing(int param);
};

extern PacketManager g_packetManager;

// FUNCTION: 0x4578d0
void __stdcall SetPacketRate(int param_1)
{
    g_packetManager.SetDefaultSendPacing(param_1);
}
