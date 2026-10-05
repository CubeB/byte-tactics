// Decompiled by Haiku. Names are provisional.

struct PacketManager {
    char unknown_0[0xb2f4];
    int field_b2f4;
    int field_b2f8;

    void ClearSendBuffer();
};

// FUNCTION: 0x4615f0
void PacketManager::ClearSendBuffer()
{
    field_b2f8 = (field_b2f4 == 0 ? 0 : 4);
}
