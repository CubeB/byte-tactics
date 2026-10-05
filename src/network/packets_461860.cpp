// Decompiled by Opus. Names are provisional.
// Resets all 11 entries the same way as 0x462470 (inlined here).

class PacketChannel {
public:
    int field_0;
    char unknown_4[0x18];
    int field_1c;
    char unknown_20[4];
    int field_24;
    char unknown_28[0x1044 - 0x28];

    void InitPools(int a1, int a2, int a3, int a4);
};

static inline void Reset(PacketChannel* e)
{
    e->InitPools(-1, 0xc8, 2, 0x64);
    e->field_0 = -1;
    e->field_1c = -1;
    e->field_24 = 0;
}

class PacketManager {
public:
    char unknown_0[8];
    PacketChannel entries[11];         // +0x8

    void ReleaseAllChannels();
};

// FUNCTION: 0x461860
void PacketManager::ReleaseAllChannels()
{
    for (int i = 0; i < 11; i++) {
        Reset(&entries[i]);
    }
}
