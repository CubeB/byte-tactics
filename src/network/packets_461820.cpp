// Decompiled by Opus. Names are provisional.
// Looks up an entry and resets it the same way as 0x462470.

class PacketManager {
public:
    void* FindChannel(int param_1, int param_2);
    void ReleaseChannel(int id);
};

class PacketChannel {
public:
    int field_0;
    char unknown_4[0x18];
    int field_1c;
    char unknown_20[4];
    int field_24;

    void InitPools(int a1, int a2, int a3, int a4);
};

// FUNCTION: 0x461820
void PacketManager::ReleaseChannel(int id)
{
    PacketChannel* e = (PacketChannel*)((PacketManager*)this)->FindChannel(id, 0);
    if (e != 0) {
        e->InitPools(-1, 0xc8, 2, 0x64);
        e->field_0 = -1;
        e->field_1c = -1;
        e->field_24 = 0;
    }
}
