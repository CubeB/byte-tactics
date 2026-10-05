// Decompiled by Opus. Names are provisional.
// A method of the global g_packetManager (its one caller, 0x451df0, sets ecx to
// it) that ignores `this` and forwards to 0x462710 on the object it is
// given, which that caller passes as &g_packetManager + 8.

class PacketChannel {
public:
    void* AddPacket(int param_1, void* param_2, unsigned int param_3);
};

class PacketManager {
public:
    void QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
};

// FUNCTION: 0x461990
void PacketManager::QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4)
{
    int a = param_4;
    int b = param_3;
    int c = param_1;
    param_2->AddPacket(c, (void*)b, a);
}
