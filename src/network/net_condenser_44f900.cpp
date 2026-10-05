// Decompiled by Opus. Names are provisional.
// The out-of-line destructor of the net condenser class whose constructor is
// 0x44f8a0 (the global copies at 0x44f720/0x44f7e0 inline the same body).

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
    ~NetCondenser();
};
#pragma pack(pop)

// FUNCTION: 0x44f900
NetCondenser::~NetCondenser()
{
    delete[] buffer;
    delete[] buffer_10;
    delete[] buffer_18;
    delete[] buffer_14;
}
