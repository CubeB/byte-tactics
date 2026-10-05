// Decompiled by Opus. Names are provisional.

struct Game {
    char unknown_0[0x14];
    char field_14[0x2a44 - 0x14];
    unsigned short flags_2a44;
};

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

bool FUN_0046bf20();
void __stdcall HAPINET_uninitmultiplay(void* param_1);

// FUNCTION: 0x450dd0
void CloseNetSession()
{
    if (g_game->flags_2a44 & 1) {
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        if (!FUN_0046bf20()) {
            HAPINET_uninitmultiplay(g_game->field_14);
        }
        g_game->flags_2a44 &= 0xfffe;
    }
}
