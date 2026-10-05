// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int now;                  // +0x38a47
};

struct Target_00408c40 {
    int unknown_0;                     // +0x0
    char unknown_4[0x73 - 0x4];
    char state;                        // +0x73
};
#pragma pack(pop)

class Timer_00408c40 {
public:
    virtual void Fire();
    char unknown_4[0xc - 0x4];
    unsigned int time;                 // +0xc
};

extern Game* g_game;

class Class_00408830 {
public:
    void FUN_00408830();
};

class Class_004089a0 {
public:
    void FUN_004089a0(int param_1);
};

#pragma pack(push, 1)
class Class_00408c40 {
public:
    Target_00408c40* target;           // +0x0
    char unknown_4;
    int countdown;                     // +0x5
    char unknown_9[0x11 - 0x9];
    Timer_00408c40* timers[10];        // +0x11

    void FUN_00408c40();
};
#pragma pack(pop)

// FUNCTION: 0x408c40
void Class_00408c40::FUN_00408c40()
{
    if (target->unknown_0 != 0 && target->state == 2) {
        if (--countdown <= 0) {
            countdown = 30;
            ((Class_00408830*)this)->FUN_00408830();
        }
        for (int i = 0; i < 10; i++) {
            if (timers[i] != 0 && timers[i]->time <= g_game->now) {
                timers[i]->Fire();
            }
        }
        ((Class_004089a0*)this)->FUN_004089a0(1);
        return;
    }
    ((Class_004089a0*)this)->FUN_004089a0(0);
}
