// Decompiled by Opus. Names are provisional.
// Puts one dword (the message's field_10, or -1 when field_14 is set) in the
// send buffer and sends it; the send is the body of FUN_00461180, inlined.

class Class_0044f940 {
public:
    void FUN_0044f940(void* data, int size);
};

class Class_0044fc10 {
public:
    int FUN_0044fc10(void* session, int from);
};

struct Game_00461900 {
    char unknown_0[0x14];
    char session[4];                 // +0x14
};

extern Game_00461900* g_game;
extern Class_0044f940 DAT_005129d0;  // net condenser
extern int DAT_005129f1;

void FUN_00461170(const char* fmt, ...);

static inline int SendTo(int from, int to, void* data, int size)
{
    FUN_00461170("bytes to send to (DPID)(%ld): %ld\n", to, size);
    void* session = g_game->session;
    DAT_005129f1 = to;
    DAT_005129d0.FUN_0044f940(data, size);
    int result = ((Class_0044fc10*)&DAT_005129d0)->FUN_0044fc10(session, from);
    return result == 0 ? 1 : 0;
}

struct Msg_00461900 {
    char unknown_0[0x10];
    int field_10;                    // +0x10
    int field_14;                    // +0x14
};

class Class_00461900 {
public:
    char unknown_0[0xb2f4];
    int* buffer;                     // +0xb2f4
    int size;                        // +0xb2f8

    int FUN_00461900(int from, Msg_00461900* msg);
};

// FUNCTION: 0x461900
int Class_00461900::FUN_00461900(int from, Msg_00461900* msg)
{
    *buffer = msg->field_14 ? -1 : msg->field_10;
    SendTo(from, msg->field_14, buffer, size);
    size = buffer ? 4 : 0;
    return 1;
}
