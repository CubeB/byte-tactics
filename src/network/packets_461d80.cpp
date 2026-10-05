// Decompiled by Haiku. Names are provisional.

struct PacketChannel
{
public:
    char unknown_0[0x18];
    int field_18;

    int GetMinRetainMs();
};

// FUNCTION: 0x461d80
int PacketChannel::GetMinRetainMs()
{
    unsigned int val = field_18;
    return (val / 30) * 1000;
}
