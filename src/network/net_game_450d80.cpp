// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14];
    char net[0x4dd];             // +0x14
    int netMode;                 // +0x4f1
    char unknown_4f5[0x39211 - 0x4f5];
    char connection[4];          // +0x39211
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall HAPINET_initmultiplaydefaults(void* net);
int __stdcall InitPacketManager(int a, int b);
void __stdcall HAPINET_initconnection(void* net, void* connection);

// FUNCTION: 0x450d80
void InitNetConnection()
{
    HAPINET_initmultiplaydefaults(g_game->net);
    g_game->netMode = 10;
    if (InitPacketManager(2, 100)) {
        HAPINET_initconnection(g_game->net, g_game->connection);
    }
}
