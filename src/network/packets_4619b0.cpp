// Decompiled by Sonnet. Names are provisional.

class PacketManager {
public:
    void* FindChannel(int param_1, int param_2);
};

class PacketChannel {
public:
    void* AddPacket(int param_1, void* param_2, unsigned int param_3);
};

class Class_004619b0 {
public:
    void* QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
};

// FUNCTION: 0x4619b0
void* Class_004619b0::QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4)
{
    void* obj = ((PacketManager*)this)->FindChannel(param_2, 1);
    if (obj != 0) {
        return ((PacketChannel*)obj)->AddPacket(param_1, param_3, param_4);
    }
    return 0;
}
