// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Receives one network packet into the shared buffer (g_game+0x2a38),
// growing the buffer ("PACKET DATA AGAIN") while the receive call returns
// 0x8877001e (buffer too small), through DAT_0051e300 when g_usePacketManager is
// set, else through DirectPlay (HAPINET_receivepacket), and counts it in the
// network statistics.
//
// MATCH. The one instruction that used to be out of place is fixed by
// declaring the function itself __stdcall (it still returns with a plain
// `ret`, it only takes no arguments): as __cdecl, MSVC 5 hoists the reload
// of the size local above the two `push 0` of CountPacket, as __stdcall it
// emits it late, as `mov ecx, [esp+8]`, right before the `push ecx`. The
// same trick is used at 0x41f0a0. Notes from Claude Opus 5.5 (#295): the
// include must not be needed, but adding unrelated externs/prototypes or
// headers never changes this now.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a34];
    int field_2a34;                    // +0x2a34
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a44 - 0x2a3c];
    unsigned char flags_2a44;          // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;

class PacketReceiver {
public:
    int ReceiveFrame(void* net, unsigned char* data, int* size);
};

extern PacketReceiver DAT_0051e300;

int __stdcall HAPINET_receivepacket(void* net, void* data, int* size);
void __stdcall CountMessage(unsigned char kind, int amount, int player);
void __stdcall CountPacket(int size, int overhead, int sent);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);

// FUNCTION: 0x4534e0
int __stdcall ReceiveNetPacket(void)
{
    int size;

    if (!(g_game->flags_2a44 & 1))
        return 0;

    size = g_game->field_2a34;
    if (g_usePacketManager != 0) {
        while (1) {
            int result = DAT_0051e300.ReceiveFrame((char*)g_game + 0x14, g_game->buffer, &size);
            if (result == 0) {
                CountMessage(*g_game->buffer, size, 0);
                return 1;
            }
            if (result == 0x887700be)
                return 0;
            if (result != 0x8877001e)
                return 0;
            g_game->field_2a34 = size;
            g_game->buffer = (unsigned char*)FUN_004d84a0(g_game->buffer, "PACKET DATA AGAIN", size);
        }
    } else {
        while (1) {
            int result = HAPINET_receivepacket((char*)g_game + 0x14, g_game->buffer, &size);
            if (result == 0) {
                CountMessage(*g_game->buffer, size, 0);
                CountPacket(size, 0, 0);
                return 1;
            }
            if (result == 0x887700be)
                return 0;
            if (result != 0x8877001e)
                return 0;
            g_game->field_2a34 = size;
            g_game->buffer = (unsigned char*)FUN_004d84a0(g_game->buffer, "PACKET DATA AGAIN", size);
        }
    }
}
