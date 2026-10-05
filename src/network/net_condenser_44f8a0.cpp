// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
class NetCondenser {
public:
    char* buffer;                    // +0x0
    int unknown_4;                   // +0x4
    int unknown_8;                   // +0x8
    int unknown_c;                   // +0xc
    char* buffer_10;                 // +0x10
    char* buffer_14;                 // +0x14
    char* buffer_18;                 // +0x18
    unsigned char unknown_1c;        // +0x1c
    int unknown_1d;                  // +0x1d
    int unknown_21;                  // +0x21

    NetCondenser();
};
#pragma pack(pop)

// FUNCTION: 0x44f8a0
NetCondenser::NetCondenser()
{
    buffer = new char[0x6d60];
    unknown_4 = 0;
    unknown_8 = 0;
    unknown_c = 0;
    buffer_10 = new char[0xaf0];
    buffer_18 = new char[0xaf0];
    buffer_14 = new char[0xaf0];
    unknown_1c = 0;
    unknown_1d = 0;
    unknown_21 = 0;
}
