// Decompiled by Space Bunny Free. Names are provisional.
// Plays the sound of a unit-type index (0xffff means "none"): directly through
// the sound object, or, in a network game, by sending the 0x13 message to the
// other players first.

#pragma pack(push, 1)
struct Packet_0047f0c0 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int index;                         // +0x2
    int unknown_6[3];                  // +0x6
};

class Sound {
public:
    int PlayLooping(int a, int b);
    int PlaySampleSet(int a, int b, int c);
};

struct Game {
    char unknown_0[0x10];
    Sound* sound;             // +0x10
    char unknown_14[0x33a13 - 0x14];
    char soundIds[0x37f0c - 0x33a13];  // +0x33a13
    int field_37f0c;                   // +0x37f0c
    char unknown_37f10[0x37f19 - 0x37f10];
    unsigned char flags_37f19;         // +0x37f19
};
#pragma pack(pop)

extern Game* g_game;
extern int g_noDirectSound;
extern int g_useWindowsSound;
extern int g_playLooping;

int __cdecl GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall PlayWavMemory(int sound);
int __stdcall PlayLoopingWavMemory(int sound);

// FUNCTION: 0x47f0c0
int __stdcall PlaySoundByIndex(int index, int param_2)
{
    if (index != 0xffff) {
        int sound = *(int*)(g_game->soundIds + index * 4);
        if (g_useWindowsSound) {
            if (g_playLooping)
                return PlayLoopingWavMemory(sound);
            return PlayWavMemory(sound);
        }
        if (g_game->field_37f0c != 0 && (g_game->flags_37f19 & 7) != 0
            && g_noDirectSound == 0) {
            if (param_2) {
                Packet_0047f0c0 packet;
                packet.type = 0x13;
                packet.flag = 1;
                packet.index = index;
                BroadcastPacket(GetLocalDpid(), &packet, sizeof(packet));
            }
            if (g_playLooping)
                return ((Sound*)g_game->sound)->PlayLooping(sound, -585);
            return g_game->sound->PlaySampleSet(sound, -585, 0);
        }
    }
    return 0;
}
