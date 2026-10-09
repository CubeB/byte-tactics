// Decompiled by Claude Opus 5.5. Names are provisional.
// <windows.h> is needed for the operand order of the player address
// ([eax + ebp + 0x1b63], g_game as the base); without it MSVC swaps them.
#include <windows.h>
#include <string.h>

// The aligned frame (`and esp, -8`) comes from the default flags, no /Op: the
// unsigned-to-float conversion needs an 8-byte stack temporary.

// The campaign object at g_game+0x391e9 (Mission in 0x435da0.cpp).
#include "../map/mission.h"

#include "../network/player.h"
#include "../network/player_info.h"

#pragma pack(push, 1)
struct Stat_0041dc20 {                 // 0x3a bytes
    char name[30];                     // +0x00
    int kills;                         // +0x1e
    int losses;                        // +0x22
    int energyProduced;                // +0x26
    int metalProduced;                 // +0x2a
    int energyWasted;                  // +0x2e
    int metalWasted;                   // +0x32
    int score;                         // +0x36
};

struct Game_0041dc20 {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x38a47 - 0x2851];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x38dd9 - 0x38a4b];
    Stat_0041dc20 stats[10];           // +0x38dd9
    char unknown_3901d[0x3918f - 0x3901d];
    int maxKills;                      // +0x3918f
    int maxLosses;                     // +0x39193
    int max_26;                        // +0x39197
    int max_2a;                        // +0x3919b
    int max_2e;                        // +0x3919f
    int max_32;                        // +0x391a3
    int maxScore;                      // +0x391a7
    int mission;                       // +0x391ab
    int won;                           // +0x391af
    char unknown_391b3[0x391cf - 0x391b3];
    char results[0x391e9 - 0x391cf];   // +0x391cf
    Mission* campaign;                 // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned short bits0_3923b : 2;    // +0x3923b
    unsigned short bit2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short bit4_3923b : 1;
    unsigned short bits5_3923b : 11;
};
#pragma pack(pop)

extern Game_0041dc20* g_game;

// End of a game: records whether it was won (and, in a campaign, the
// mission's result letter), then fills the per-player statistics table
// and the column maxima the results screen scales its bars by.
// FUNCTION: 0x41dc20
void CollectEndGameStats()
{
    Stat_0041dc20* stats = g_game->stats;
    g_game->won = g_game->bit4_3923b;
    if (g_game->campaign->GetGameType() == 1) {
        g_game->mission = g_game->campaign->GetMissionIndex();
        g_game->results[g_game->mission] = g_game->won ? 'W' : 'L';
    }
    g_game->maxKills = 10;
    g_game->maxLosses = 10;
    g_game->max_26 = 100;
    g_game->max_2a = 100;
    g_game->max_2e = 100;
    g_game->max_32 = 100;
    g_game->maxScore = 100;
    memset(stats, 0, sizeof g_game->stats);
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if ((p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->index != 10 && !p->info->bit6) || p->unitsCreated) {
            if (p->rejectReason == 0) {
                strncpy(stats[i].name, p->name, 30);
                stats[i].kills = p->kills;
                stats[i].losses = p->losses;
                stats[i].energyProduced = (int)p->totalEnergyProduced;
                stats[i].metalProduced = (int)p->totalMetalProduced;
                stats[i].energyWasted = (int)p->energyWasted;
                stats[i].metalWasted = (int)p->metalWasted;
                Mission* c = g_game->campaign;
                int t = (int)(g_game->ticks / 60 * c->timeMul);
                stats[i].score = t + (int)(stats[i].kills * c->killMul);
                if (stats[i].score < 0)
                    stats[i].score = 0;
                if (stats[i].kills > g_game->maxKills)
                    g_game->maxKills = stats[i].kills;
                if (stats[i].losses > g_game->maxLosses)
                    g_game->maxLosses = stats[i].losses;
                if (stats[i].energyProduced > g_game->max_26)
                    g_game->max_26 = stats[i].energyProduced;
                if (stats[i].metalProduced > g_game->max_2a)
                    g_game->max_2a = stats[i].metalProduced;
                if (stats[i].energyWasted > g_game->max_2e)
                    g_game->max_2e = stats[i].energyWasted;
                if (stats[i].metalWasted > g_game->max_32)
                    g_game->max_32 = stats[i].metalWasted;
                if (stats[i].score > g_game->maxScore)
                    g_game->maxScore = stats[i].score;
            }
        }
    }
}
