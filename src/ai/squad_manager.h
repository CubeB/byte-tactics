// SquadManager: the AI controller behind Player::ai, 0x3d bytes, one per
// player (the constructor at 0x408cb0 is called from the player setup). It owns
// ten SquadTimer slots, one per unit group of the player, ticks them and
// retargets the player's weapons round-robin through a unit cursor. The one
// declaration of the class, for ai_player.cpp and every file that reaches it
// through a Player; the types behind the pointers stay private to their own
// files.
#ifndef SQUAD_MANAGER_H
#define SQUAD_MANAGER_H

struct Player;
struct Unit;
struct Vec3;
struct Obj_00406f50;
class SquadTimer;

class SquadManager {
public:
    Player* player;                    // +0x0
    unsigned char index;               // +0x4, the player's index
    int countdown;                     // +0x5
    int unused_9;                      // +0x9
    int nextAction;                    // +0xd
    SquadTimer* timers[10];            // +0x11
    Unit* cursor;                      // +0x39

    SquadManager(Player* p);
    void MarkOwnerNetDirtyFromDamageSplit(Obj_00406f50* obj, int a, int b);
    void AssignSquads();
    void RetargetWeapons(int force);
    void TickIfActive();
    void DeleteTimers();
    Unit* FindNearestEnemyUnit(int x, int y, int z);
    Unit* FindNearestEnemyUnit(Vec3 pos);
    void TickTimers();
};

#endif
