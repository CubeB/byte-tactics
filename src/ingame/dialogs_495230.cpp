// Decompiled by Sonnet 5.5, finished by Space Bunny Free. Names are provisional.
// Frame pacing: measures the time since the last call, finds the slowest
// active player (the one with the lowest tick counter that still has units),
// and turns the lag behind the fastest into a speed factor. That factor scales
// the elapsed time into a whole number of game ticks for this frame (the
// fraction is carried over in a float), which is clamped to 0..5 and to 0
// while paused. Too many capped frames in a row lowers the current speed step,
// a long run of idle ones raises it again.
//
// MATCH (601 of 601). The whole fix was to DELETE a cast: the previous attempt
// wrote `g_game->carry = (float)(x - whole);` and this file writes
// `g_game->carry = x - whole;`.
//
// The cast is the natural thing to write, because the field is a `float`, and
// writing it changes the order of the x87 pair. With the explicit cast MSVC 5
// treats the narrowing as a value conversion to be performed at the assignment
// and sinks the store, so the `fsub` drifts to *after* the frames store. With
// no cast the assignment's own double-to-float conversion keeps the pair
// together in source order, and the emission becomes, byte for byte:
//
//   ours      fld QWORD PTR _x$[esp+20] / mov edx,g_game / fsub ST(0),ST(1)
//             / mov [edx+0x38a3b],eax / mov eax,g_game / fstp [eax+0x38a43]
//   original  mov edx,g_game / fsub st(1) / mov [edx+0x38a3b],eax
//             / mov eax,g_game / fstp dword ptr [eax+0x38a43]
//
// Worth recording because the previous attempt had already localised the
// symptom exactly ("the original loads the saved x first and then g_game, here
// g_game is loaded before the fld and the fsub comes after the frames store")
// and then tried six source rearrangements, none of which questioned the cast:
// whole in a local or not, frames and carry in temporaries, swapped store
// order. The cast looked like part of the intended code precisely because the
// field is a float, so it was never the thing under suspicion. This is the same
// shape as 0x4b91b0's `w/2/2` and 0x48a490's three misreadings of MSVC's signed
// division fixup: a spelling that looks obviously right, is locally plausible,
// and nobody asks what it is doing to the code generator.

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

unsigned int FUN_004b6340();

// FUNCTION: 0x495230
void FUN_00495230()
{
    unsigned int now = FUN_004b6340();
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
