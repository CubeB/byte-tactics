// Decompiled by GPT-6, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// The rest of the class is in ai_player_407e70.cpp.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
};
#pragma pack(pop)

extern Game* g_game;

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    // The declaration of ax, the double and the assignment are three separate
    // statements on purpose: folding them into `int ax = (int)(game->baseX / 2
    // * 65536.0);` moves the constructor's `pop edi` back between c.y and c.z
    // and the function stops matching.
    Vec3_00407d40(Game* game) {
        int ax;
        double halfX = ((double)(((game->baseX / 2) * 65536.0)));
        ax = (int)halfX;
        *this = Vec3_00407d40(ax, 0, (int)((game->baseY / 2) * 65536.0));
    }
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

struct SquadManager {                  // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    SquadTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
};

extern void* DAT_004fc980[];
extern void* DAT_004fc9a0[];

class Class_00407d40 {
public:
    // Plain field, not a virtual class: both vtable stores come from source.
    void* vptr_slot;                   // +0x0
    SquadManager* owner;               // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10
    Vec3_00407d40 a;                   // +0x14
    Vec3_00407d40 b;                   // +0x20
    Vec3_00407d40 c;                   // +0x2c
    int field_38;                      // +0x38

    Class_00407d40(SquadManager* p, void* q);
};

SquadTimer::SquadTimer(SquadManager* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4) {}

// FUNCTION: 0x407d40
Class_00407d40::Class_00407d40(SquadManager* p, void* q)
{
    owner = p;
    field_8 = q;
    field_c = 0;
    field_10 = p->field_4;
    vptr_slot = DAT_004fc980;
    a = Vec3_00407d40(g_game);
    b = Vec3_00407d40(g_game);
    Vec3_00407d40 temp(g_game);
    // Order matters: field_38, then the late vtable store, then c.
    field_38 = 0;
    vptr_slot = DAT_004fc9a0;
    c = temp;
}
