// Decompiled by Sonnet 5.5, finished by Space Bunny Free. Names are provisional.
// Frame pacing: measures the time since the last call, finds the slowest
// active player (the one with the lowest tick counter that still has units),
// and turns the lag behind the fastest into a speed factor. That factor scales
// the elapsed time into a whole number of game ticks for this frame (the
// fraction is carried over in a float), which is clamped to 0..5 and to 0
// while paused. Too many capped frames in a row lowers the current speed step,
// a long run of idle ones raises it again.

#include <math.h>

#pragma pack(push, 1)
struct Player_00495230 {
    int active;                        // +0x0
    char unknown_4[0x18 - 4];
    int tick;                          // +0x18
    char unknown_1c[0x73 - 0x1c];
    unsigned char state;               // +0x73
    char unknown_74[0x144 - 0x74];
    unsigned short units;              // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game {
    char unknown_0[0x511];
    Player_00495230* slowest;          // +0x511
    int lag;                           // +0x515
    char unknown_519[0x1b63 - 0x519];
    Player_00495230 players[10];       // +0x1b63
    char unknown_2851[0x38a37 - 0x2851];
    unsigned int lastTick;             // +0x38a37
    int frames;                        // +0x38a3b
    int elapsed;                       // +0x38a3f
    float carry;                       // +0x38a43
    int leadTick;                      // +0x38a47
    unsigned short maxSpeed;           // +0x38a4b
    unsigned short speed;              // +0x38a4d
    short streak;                      // +0x38a4f
    unsigned short paused : 1;         // +0x38a51
    unsigned short lagging : 1;
    unsigned short faster : 1;
    unsigned short rest : 13;
};
#pragma pack(pop)

extern Game* g_game;

unsigned int GetTicks();

// FUNCTION: 0x495230
void UpdateFramePacing()
{
    unsigned int now = GetTicks();
    g_game->elapsed = now - g_game->lastTick;
    g_game->lastTick = now;
    unsigned short speed = g_game->speed;
    double rate = speed * 0.1;
    g_game->faster = speed < g_game->maxSpeed;

    int best = g_game->leadTick;
    g_game->slowest = 0;
    for (Player_00495230* p = g_game->players; p != g_game->players + 10; p++) {
        if (p->active != 0 && p->state == 3 && p->units > 0) {
            int tick = p->tick;
            if (tick < best) {
                g_game->slowest = p;
                best = tick;
            }
        }
    }
    g_game->lag = g_game->leadTick - best;

    if (g_game->lag >= 900) {
        int lag = g_game->lag;
        if (lag > 3600)
            lag = 3600;
        double f = (3600 - lag) * 0.00037037037037037035;
        if (f < 0.01)
            f = 0.01;
        g_game->lagging = 1;
        rate *= f;
    } else {
        g_game->lagging = 0;
    }

    double x = g_game->elapsed * rate + g_game->carry;
    double whole = floor(x);
    g_game->frames = (int)whole;
    // No (float) cast: with it the fsub moves after the frames store.
    g_game->carry = x - whole;
    if (g_game->frames < 0)
        g_game->frames = 0;
    if (g_game->paused) {
        g_game->frames = 0;
        return;
    }
    if (g_game->frames > 5) {
        g_game->frames = 5;
        g_game->streak++;
        if (g_game->streak > 10) {
            g_game->streak = 0;
            if (g_game->speed > 1)
                g_game->speed--;
        }
    } else {
        g_game->streak--;
        if (g_game->streak < -100) {
            g_game->streak = 0;
            if (g_game->speed < g_game->maxSpeed)
                g_game->speed++;
        }
    }
}
