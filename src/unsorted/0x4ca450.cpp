// Decompiled by Opus. Names are provisional.
// HAPINET_passwordrequired: tests flag 0x400 in the current connection's
// description, or in the local copy when there is no connection.

struct Desc_004ca450 {
    int unknown_0;
    unsigned int flags;                // +0x4
};

struct Connection_004ca450 {
    char unknown_0[8];
    Desc_004ca450* desc;               // +0x8
};

#pragma pack(push, 1)
struct Net_004ca450 {
    char unknown_0[0x461];
    unsigned int flags;                // +0x461
    char unknown_465[0x4d1 - 0x465];
    Connection_004ca450* connection;   // +0x4d1
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);

// FUNCTION: 0x4ca450
unsigned int __stdcall FUN_004ca450(Net_004ca450* net)
{
    FUN_004c9740((int)"HAPINET_passwordrequired\n");
    if (net->connection != 0)
        return net->connection->desc->flags & 0x400;
    return net->flags & 0x400;
}
