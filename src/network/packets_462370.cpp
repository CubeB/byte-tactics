// Decompiled by Sonnet. Names are provisional.

class PacketRing {
public:
    int count;          // +0
    char unknown_4[4];
    int writeIdx;       // +8
    int buf[0x400];     // +0xc

    int PushPacket(int value);
};

// FUNCTION: 0x462370
int PacketRing::PushPacket(int value)
{
    if (count < 0x400) {
        writeIdx = writeIdx + 1;
        if (writeIdx >= 0x400) {
            writeIdx = 0;
        }
        buf[writeIdx] = value;
        count = count + 1;
        return 1;
    }
    return 0;
}
