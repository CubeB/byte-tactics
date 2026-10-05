// Decompiled by Sonnet. Names are provisional.

class PacketChannel {
public:
    int field_0;
    char unknown_4[0x18];
    int field_1c;
    char unknown_20[4];
    int field_24;

    void InitPools(int a1, int a2, int a3, int a4);

    void ResetChannel();
};

// FUNCTION: 0x462470
void PacketChannel::ResetChannel()
{
    InitPools(-1, 0xc8, 2, 0x64);
    field_0 = -1;
    field_1c = -1;
    field_24 = 0;
}
