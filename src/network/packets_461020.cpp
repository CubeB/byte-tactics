// Decompiled by space-bunny-free. Names are provisional.
// The object at g_packetManager holds m_defaultSendPacingMs at +4 and the
// 0x1044 byte entries at +8.
#include <windows.h>

void* __cdecl operator new(unsigned int size);
void __cdecl PacketTrace(const char* fmt, ...);

class PacketChannel {
public:
    int field_0;                       // +0x00
    unsigned long m_pacingMs;          // +0x04
    char unknown_8[0x14 - 0x8];
    int field_14;                      // +0x14
    char unknown_18[0x1c - 0x18];
    int field_1c;                      // +0x1c
    char unknown_20[4];
    int field_24;                      // +0x24
    char unknown_28[0x1044 - 0x28];

    void InitPools(int a1, int a2, int a3, int a4);
};

class PacketManager {
public:
    char unknown_0[4];
    unsigned long m_defaultSendPacingMs;   // +0x04
    PacketChannel entries[11];             // +0x08

    void SetDefaultSendPacing(int rate);
};

struct Buffer_461020 {
    char unknown_0[0xc];
    int field_c;
};

extern PacketManager g_packetManager;
extern int DAT_00512c94;
extern int g_usePacketManager;
extern Buffer_461020* DAT_0051e314;
extern void* DAT_0051e318;
extern void* DAT_0051e31c;
extern int DAT_0051e528;
extern int DAT_0051e52c;

// Must be __inline: written out in the body, the second `rate == 0` test is
// folded away; called out of line, it is not inlined at all.
__inline void PacketManager::SetDefaultSendPacing(int rate)
{
    unsigned long pace;
    if (rate < 0) {
        g_usePacketManager = 0;
        return;
    }
    if (rate == 0) {
        pace = 200;
    } else {
        if (rate < 2) {
            rate = 2;
        } else if (rate > 30) {
            rate = 30;
        }
        pace = 1000 / rate;
    }
    m_defaultSendPacingMs = pace;
    PacketTrace("setting m_defaultSendPacingMs to: %lums\n", pace);
    PacketChannel* p = &entries[0];
    for (int i = 0; i < 11; i++, p++) {
        p->m_pacingMs = (m_defaultSendPacingMs * 30 + 999) / 1000;
    }
}

// FUNCTION: 0x461020
int __stdcall InitPacketManager(int param_1, int param_2)
{
    if (DAT_00512c94 != 0) {
        g_packetManager.SetDefaultSendPacing(DAT_00512c94);
    }
    if (g_usePacketManager != 0) {
        // The do/while region must end before SetThreadPriority, which stays
        // inside the if.
        do {
            if (DAT_0051e318 == 0) {
                if (DAT_0051e31c != 0) {
                    DAT_0051e318 = DAT_0051e31c;
                    DAT_0051e31c = 0;
                } else {
                    DAT_0051e318 = operator new(0x42a);
                    if (DAT_0051e318 == 0) {
                        return 0;
                    }
                    DAT_0051e528 = 0x42a;
                }
            }
            DAT_0051e52c = 0;
            if (DAT_0051e314 != 0) {
                DAT_0051e314->field_c = 0;
                DAT_0051e314 = 0;
            }
            g_packetManager.entries[0].InitPools(0, g_packetManager.m_defaultSendPacingMs, param_1, param_2);
            PacketChannel* p = &g_packetManager.entries[1];
            for (int i = 1; i < 11; i++, p++) {
                p->InitPools(-1, g_packetManager.m_defaultSendPacingMs, 2, 100);
            }
        } while (0);
        SetThreadPriority(GetCurrentThread(), -2);
    }
    return 1;
}
