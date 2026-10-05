// Decompiled by Opus. Names are provisional.
// Sends a one-byte message (type 6) from the shared packet buffer, to
// everyone when `to` is 0, otherwise to that player.

#pragma pack(push, 1)
struct Game_00453320 {
    char unknown_0[0x2a38];
    unsigned char* buffer;             // +0x2a38
};
#pragma pack(pop)

extern Game_00453320* g_game;

int __stdcall FUN_00451df0(int player, void* data, int size);
int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);

// FUNCTION: 0x453320
void __stdcall FUN_00453320(int from, int to)
{
    unsigned char* buf = g_game->buffer;
    *buf = 6;
    if (to == 0) {
        FUN_00451df0(from, buf, 1);
    } else {
        FUN_00451bc0(from, to, buf, 1);
    }
}
