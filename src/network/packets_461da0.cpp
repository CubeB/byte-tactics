// Decompiled by Haiku. Names are provisional.

struct PacketChannel {
public:
    char unknown_0[0x28];
    int field_28;

    int GetPacketEntry(int param_1);
};

// FUNCTION: 0x461da0
int PacketChannel::GetPacketEntry(int param_1)
{
    return param_1 * 0x20 + field_28;
}
