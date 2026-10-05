// Decompiled by Opus. Names are provisional.
// Sends a one-byte message (type 6) from the shared packet buffer, to
// everyone when `to` is 0, otherwise to that player.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a38];
    unsigned char* buffer;             // +0x2a38
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);

// FUNCTION: 0x453320
void __stdcall FUN_00453320(int from, int to)
{
    unsigned char* buf = g_game->buffer;
    *buf = 6;
    if (to == 0) {
        BroadcastPacket(from, buf, 1);
    } else {
        SendPacketToPlayer(from, to, buf, 1);
    }
}
