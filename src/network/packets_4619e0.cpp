// Decompiled by Opus. Names are provisional.

struct Channel_4619e0 {
    unsigned long pacing;        // +0
    char unknown_4[0x1044 - 4];
};

class PacketManager {
public:
    char unknown_0[4];
    unsigned long m_defaultSendPacingMs;   // +4
    char unknown_8[4];
    Channel_4619e0 channels[11];           // +0xc

    void SetDefaultSendPacing(int rate);
};

extern int g_usePacketManager;

void __cdecl PacketTrace(const char* fmt, ...);

// FUNCTION: 0x4619e0
void PacketManager::SetDefaultSendPacing(int rate)
{
    if (rate < 0) {
        g_usePacketManager = 0;
        return;
    }
    if (rate == 0) {
        m_defaultSendPacingMs = 200;
    } else {
        if (rate < 2) {
            rate = 2;
        } else if (rate > 30) {
            rate = 30;
        }
        m_defaultSendPacingMs = 1000 / rate;
    }
    PacketTrace("setting m_defaultSendPacingMs to: %lums\n", m_defaultSendPacingMs);
    for (int i = 0; i < 11; i++) {
        channels[i].pacing = (m_defaultSendPacingMs * 30 + 999) / 1000;
    }
}
