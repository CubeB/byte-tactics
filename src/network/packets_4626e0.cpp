// Decompiled by Opus. Names are provisional.

class NetCondenser {
public:
    void Accumulate(void* data, int size);
};

class Class_0044fc10 {
public:
    int SendPacket(void* session, int from);
};

#pragma pack(push, 1)
class Class_004626e0 {
public:
    char unknown_0[0x21];
    int field_21;                      // +0x21

    void SendPacketTo(void* session, int from, int value, void* data, int size);
};
#pragma pack(pop)

// FUNCTION: 0x4626e0
void Class_004626e0::SendPacketTo(void* session, int from, int value, void* data, int size)
{
    field_21 = value;
    ((NetCondenser*)this)->Accumulate(data, size);
    ((Class_0044fc10*)this)->SendPacket(session, from);
}
