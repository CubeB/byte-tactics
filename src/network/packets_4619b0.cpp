// Decompiled by Sonnet. Names are provisional.

class PacketManager {
public:
    void* FindChannel(int param_1, int param_2);
    void* QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
};

class PacketChannel {
public:
    void* AddPacket(int param_1, void* param_2, unsigned int param_3);
};

// FUNCTION: 0x4619b0
void* PacketManager::QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4)
{
    void* obj = ((PacketManager*)this)->FindChannel(param_2, 1);
    if (obj != 0) {
        return ((PacketChannel*)obj)->AddPacket(param_1, param_3, param_4);
    }
    return 0;
}
