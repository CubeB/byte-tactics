// Decompiled by space-bunny-free, finished by mimo-v2.6-flash. Names are provisional.
// Runs one game update per pending time step (g_game->steps), profiling each
// phase into g_game->prof.acc[]; the profile object also lives at +0x38d85 with
// total at +4, the display copy at +8 and the accumulators at +0x2c.

unsigned int __cdecl GetMilliseconds(void);

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

class FrameTimers {                    // frame-time profile, embedded at g_game+0x38d85
public:
    int last;                           // +0x00
    int total;                          // +0x04
    int values[9];                      // +0x08
    int acc[9];                         // +0x2c

    void AccumulateProfileTime(int i) {
        unsigned int t = GetMilliseconds();
        int d = t - last;
        int v = acc[i];
        v = v + d;
        acc[i] = v;
        last = t;
    }
};

struct Player_495490 {                  // 0x14b bytes
    char unknown_0[0x14b];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
    Player_495490 players[10];          // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x38a3b - 0x2a43];
    int steps;                          // +0x38a3b
    char unknown_38a3f[0x38a47 - 0x38a3f];
    int ticks;                          // +0x38a47
    char unknown_38a4b[0x38d85 - 0x38a4b];
    FrameTimers prof;                   // +0x38d85
};
#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

void HandleNetPackets(void);
void UpdateAllUnits(void);
void UpdateProjectiles(void);
void UpdateExplosions(void);
void FUN_00464f80(void);
void UpdateFeatures(void);
void StepAllGafSequences(void);
void UpdateWind(void);
void UpdateMeteors(void);
void UpdateCameraFollow(void);
void UpdateParticles(void);
void FUN_00466580(void);
void FUN_00428bd0(void);
void FUN_00428be0(void);
void FUN_00428bf0(void);
void ExpireOldestMessage(void);
void ExpireEyeballs(void);
void __stdcall UpdateResourceSharing(Player_495490* player);

// FUNCTION: 0x495490
void __stdcall RunGameSteps(int showStats)
{
    int n = g_game->steps;

    while (n--) {
        g_game->ticks++;

        if (showStats) {
            HandleNetPackets();
            g_game->prof.AccumulateProfileTime(0);
        }
        UpdateAllUnits();
        g_game->prof.AccumulateProfileTime(1);
        UpdateProjectiles();
        g_game->prof.AccumulateProfileTime(7);
        UpdateExplosions();
        g_game->prof.AccumulateProfileTime(8);
        FUN_00464f80();
        g_game->prof.AccumulateProfileTime(2);

        UpdateFeatures();
        StepAllGafSequences();
        UpdateWind();
        UpdateMeteors();
        UpdateCameraFollow();
        g_game->prof.AccumulateProfileTime(8);

        UpdateParticles();
        g_game->prof.AccumulateProfileTime(6);
        FUN_00466580();
        g_game->prof.AccumulateProfileTime(8);

        if (showStats && g_usePacketManager != 0) {
            UpdateResourceSharing(&g_game->players[g_game->localPlayer]);
            g_packetManager.SendAllQueued(0);
            g_game->prof.AccumulateProfileTime(0);
        }
    }

    FUN_00428bd0();
    FUN_00428be0();
    FUN_00428bf0();
    ExpireOldestMessage();
    ExpireEyeballs();
    g_game->prof.AccumulateProfileTime(8);
}
