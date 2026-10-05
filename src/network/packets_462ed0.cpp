// Decompiled by Opus. Names are provisional.
// Makes sure the 0x42a-byte buffer at +0x18 exists (reusing the spare one at
// +0x1c if there is one), resets the length at +0x22c and detaches the
// object at +0x14. Returns 0 only when the allocation fails.

struct Link_00462ed0 {
    char unknown_0[0xc];
    int field_c;                       // +0xc
};

class PacketReceiver {
public:
    char unknown_0[0x14];
    Link_00462ed0* link;               // +0x14
    char* buffer;                      // +0x18
    char* spare;                       // +0x1c
    char unknown_20[0x228 - 0x20];
    int capacity;                      // +0x228
    int length;                        // +0x22c

    int ResetReceiveBuffer();
};

// FUNCTION: 0x462ed0
int PacketReceiver::ResetReceiveBuffer()
{
    if (buffer == 0) {
        if (spare != 0) {
            buffer = spare;
            spare = 0;
        } else {
            buffer = new char[0x42a];
            if (buffer == 0) {
                return 0;
            }
            capacity = 0x42a;
        }
    }
    length = 0;
    if (link != 0) {
        link->field_c = 0;
        link = 0;
    }
    return 1;
}
