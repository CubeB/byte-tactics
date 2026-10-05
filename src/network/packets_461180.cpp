// Decompiled by Opus. Names are provisional.

void __cdecl PacketTrace(const char* fmt, ...);

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

// Storing the send result in a local first keeps `sete` (a direct
// `== 0 ? 1 : 0` on the call folds to neg/sbb/inc).
// FUNCTION: 0x461180
int __stdcall SendToDPID(int from, int to, void* data, int size)
{
    PacketTrace("bytes to send to (DPID)(%ld): %ld\n", to, size);
    void* session = g_game->session;
    DAT_005129f1 = to;
    g_sendCondenser.Accumulate(data, size);
    int result = ((Class_0044fc10*)&g_sendCondenser)->SendPacket(session, from);
    return result == 0 ? 1 : 0;
}
