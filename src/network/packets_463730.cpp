// Decompiled by Opus. Names are provisional.
// Resets the object and allocates its 0x180c-byte buffer if it has none;
// returns 1 when the buffer already existed.

struct Buffer_00463730 {               // 0x180c bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    char unknown_c[0x180c - 0xc];

    Buffer_00463730() : field_0(0), field_4(0), field_8(-1) {}
};

class FrameQueue {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    Buffer_00463730* buffer;           // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    int ResetFrames();
};

// FUNCTION: 0x463730
int FrameQueue::ResetFrames()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
    field_14 = -1;
    field_18 = -1;
    if (!buffer) {
        buffer = new Buffer_00463730;
        return 0;
    }
    return 1;
}
