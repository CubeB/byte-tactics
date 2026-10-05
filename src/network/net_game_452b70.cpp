// Decompiled by Opus. Names are provisional.
// Sends a 14-byte message (type 0x23) carrying both player ids, a byte and a
// dword from the shared packet buffer.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a38];
    unsigned char* buffer;             // +0x2a38
};

struct Packet_00452b70 {
    unsigned char type;                // +0x0
    int from;                          // +0x1
    int to;                            // +0x5
    char value;                        // +0x9
    int extra;                         // +0xa
};
#pragma pack(pop)

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);

// FUNCTION: 0x452b70
int __stdcall FUN_00452b70(int from, int to, char value, int extra)
{
    Packet_00452b70* msg = (Packet_00452b70*)g_game->buffer;
    msg->value = value;
    msg->type = 0x23;
    msg->from = from;
    msg->to = to;
    msg->extra = extra;
    int result = FUN_00451bc0(from, to, msg, 0xe);
    if (DAT_00506dbc != 0) {
        DAT_00513000.FUN_004618a0(1);
    }
    return result;
}
