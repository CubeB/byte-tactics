// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
class NetCondenser {
public:
    char unknown_0[0x21];
    int field_21;                      // +0x21

    void SendPacketTo(void* session, int from, int value, void* data, int size);
    void Accumulate(void* data, int size);
    int SendPacket(void* session, int from);
};
#pragma pack(pop)

// FUNCTION: 0x4626e0
void NetCondenser::SendPacketTo(void* session, int from, int value, void* data, int size)
{
    field_21 = value;
    ((NetCondenser*)this)->Accumulate(data, size);
    ((NetCondenser*)this)->SendPacket(session, from);
}
