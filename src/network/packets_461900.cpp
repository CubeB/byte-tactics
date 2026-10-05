// Decompiled by Opus. Names are provisional.
// Puts one dword (the message's field_10, or -1 when field_14 is set) in the
// send buffer and sends it; the send is the body of SendToDPID, inlined.

class NetCondenser {
public:
    void Accumulate(void* data, int size);
};

class Class_0044fc10 {
public:
    int SendPacket(void* session, int from);
};

struct Game {
    char unknown_0[0x14];
    char session[4];                 // +0x14
};

extern Game* g_game;
extern NetCondenser g_sendCondenser;  // net condenser
extern int DAT_005129f1;

void __cdecl PacketTrace(const char* fmt, ...);

static inline int SendTo(int from, int to, void* data, int size)
{
    PacketTrace("bytes to send to (DPID)(%ld): %ld\n", to, size);
    void* session = g_game->session;
    DAT_005129f1 = to;
    g_sendCondenser.Accumulate(data, size);
    int result = ((Class_0044fc10*)&g_sendCondenser)->SendPacket(session, from);
    return result == 0 ? 1 : 0;
}

struct Msg_00461900 {
    char unknown_0[0x10];
    int field_10;                    // +0x10
    int field_14;                    // +0x14
};

class PacketManager {
public:
    char unknown_0[0xb2f4];
    int* buffer;                     // +0xb2f4
    int size;                        // +0xb2f8

    int FUN_00461900(int from, Msg_00461900* msg);
};

// FUNCTION: 0x461900
int PacketManager::FUN_00461900(int from, Msg_00461900* msg)
{
    *buffer = msg->field_14 ? -1 : msg->field_10;
    SendTo(from, msg->field_14, buffer, size);
    size = buffer ? 4 : 0;
    return 1;
}
