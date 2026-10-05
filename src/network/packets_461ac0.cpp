// Decompiled by Opus. Names are provisional.
// Destructor of the class built by 0x461a70: frees each block of the pointer
// array at +0x8 (count at +0xc), the array itself, then the block at +0x28.

class PacketChannel {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    void** blocks;                     // +0x8
    unsigned int count;                // +0xc
    char unknown_10[0x28 - 0x10];
    void* field_28;                    // +0x28

    ~PacketChannel();
};

// FUNCTION: 0x461ac0
PacketChannel::~PacketChannel()
{
    if (blocks) {
        for (unsigned int i = 0; i < count; i++) {
            delete blocks[i];
        }
        delete blocks;
    }
    delete field_28;
}
