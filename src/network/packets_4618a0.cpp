// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Entry_004618a0 {
    char unknown_0[0x14];
    int field_14;                        // +0x14
    char unknown_18[0x1044 - 0x18];
};

class Class_004624a0 {
public:
    int SendQueued(int param_1);
};

class PacketManager {
public:
    char unknown_0[8];
    Entry_004618a0 entries[11];          // +0x8

    int SendAllQueued(int param_1);
};

extern int g_usePacketManager;

// FUNCTION: 0x4618a0
int PacketManager::SendAllQueued(int param_1)
{
    if (g_usePacketManager == 0) {
        return 0;
    }
    for (unsigned int i = 0; i <= 10; i++) {
        if (entries[i].field_14 != -1) {
            if (((Class_004624a0*)&entries[i])->SendQueued(param_1) == 0) {
                return 0;
            }
        }
    }
    return 1;
}
