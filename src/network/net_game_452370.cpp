// Decompiled by Opus. Names are provisional.
// Frees the buffer held at +0x1749, then does the same flag-0 cleanup as
// 0x450dd0.

struct Game {
    char unknown_0[0x14];
    char field_14[0x2a44 - 0x14];
    unsigned short flags_2a44;
};

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

#pragma pack(push, 1)
struct Class_00452370 {
    char unknown_0[0x1749];
    int* buffer;                       // +0x1749
};
#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

bool FUN_0046bf20();
void __stdcall HAPINET_uninitmultiplay(void* param_1);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x452370
void __stdcall ReleasePacketData(Class_00452370* obj)
{
    if (obj->buffer) {
        FUN_004d85a0(obj->buffer);
        obj->buffer = 0;
    }
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
