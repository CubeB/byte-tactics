// Decompiled by Opus, Sonnet, space-bunny-free and DeepSeek V4.1 Flash. Names are provisional.
// UnitSync (the object at g_game+0x2a30, built by 0x46c8e0): the check that
// every player has the same units allowed. These methods reach the unit map
// only through its out-of-line lower_bound (FUN_0046fe60), with find()
// written out inline as in MSVC 5's <xtree>.
#include <stdio.h>
#include <vector>

#pragma pack(push, 1)
struct Packet_0046d530 {               // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    union {
        int field_a;                   // +0xa
        struct {
            unsigned char entry_a;     // +0xa
            unsigned char entry_b;     // +0xb
            short entry_c;             // +0xc
        };
    };
};
#pragma pack(pop)

// A player's slot in the sync: its id and how many packets were sent to it.
struct Target_0046d530 {
    unsigned int id;                   // +0x0
    char unknown_4[0x24];
    int sent;                          // +0x28
};

struct Source_0046d630 {
    int field_0;                       // +0x0
    char unknown_4[4];
    unsigned char field_8;             // +0x8
    char unknown_9;
    unsigned char field_a;             // +0xa
    char unknown_b;
    short field_c;                     // +0xc
};

int GetLocalHumanDpid();
unsigned int GetHostDpid();
void __stdcall SendPacketToPlayer(int a, unsigned int b, void* c, int d);

// Inlined copy of Class_0046cec0::SendUnsequenced (a method that ignores this).
static inline void SendPacket(unsigned int to, void* packet)
{
    *(int*)((char*)packet + 2) = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), to, packet, 0xe);
}

struct UnitSyncEntry {                 // the map's value, 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int flag;                          // +0xc
};

struct Node_0046e330 {
    Node_0046e330* left;               // +0x0
    Node_0046e330* parent;             // +0x4
    Node_0046e330* right;              // +0x8
    unsigned int key;                  // +0xc
    UnitSyncEntry value;               // +0x10
};

class Iter_0046e330 {
public:
    Node_0046e330* ptr;
    Iter_0046e330() {}
    Iter_0046e330(Node_0046e330* p) : ptr(p) {}
    bool operator==(const Iter_0046e330& other) const { return ptr == other.ptr; }
};

#pragma pack(push, 1)
struct Unit_0046e330 {
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
};

struct Data_0046e0b0 {
    char unknown_0[0x94];
    unsigned char field_94;              // +0x94
    char unknown_95[0xa7 - 0x95];
    unsigned char count_0;               // +0xa7
    unsigned char count_1;               // +0xa8
};

struct Def_0046d970 {                    // 0x249 bytes
    char unknown_0[0x13e];
    unsigned int key;                    // +0x13e
    int y;                               // +0x142
    char unknown_146[0x249 - 0x146];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                           // +0x1438f
    char unknown_14393[8];
    Def_0046d970* defs;                  // +0x1439b
};

struct Player_0046e0b0 {                // 0x14b bytes
    int field_0;                         // +0x0
    char unknown_4[0x27 - 0x4];
    Data_0046e0b0* data;                 // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                  // +0x73
    char unknown_74[0x14b - 0x74];
};
#pragma pack(pop)

struct Less_0046e330 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

// The map's out-of-line find().
class Class_0046e9b0 {
public:
    Less_0046e330 compare;
    Node_0046e330* head;               // +0x4
    Iter_0046e330 End() { return Iter_0046e330(head); }
    Iter_0046e330 FUN_0046e9b0(const unsigned int& key);
};

class Class_0046fe60 {
public:
    Node_0046e330* FUN_0046fe60(const unsigned int* key);
};

struct Unit_0046e0b0;

struct PlayerSync_0046e0b0 {            // 0x5c bytes
    int id;                              // +0x0
    std::vector<Unit_0046e0b0*> units;   // +0x4
    char unknown_14[0x24 - 0x14];
    int expected;                        // +0x24
    int sent;                            // +0x28
    int ackd;                            // +0x2c
    char unknown_30[0x5c - 0x30];
};

// An entry from +0x8 on, stepped by 0x5c.
struct Ids_0046e000 {
    int* begin;                        // +0x0
    int* end;                          // +0x4
    int* capacity;                     // +0x8
};

struct Sub_0046e000 {
    Ids_0046e000 ids;                 // +0x0
    char unknown_c[0x1c - 0xc];
    int count;                        // +0x1c
    int field_20;                     // +0x20
    int field_24;                     // +0x24
    char unknown_28[0x5c - 0x28];
};

struct Ids_0046d970 {
    int* begin;                        // +0x0
    int* end;                          // +0x4
    int* capacity;                     // +0x8
};

struct Entry_0046d970 {                // 0x5c bytes
    int id;                            // +0x0
    char unknown_4[0x8 - 0x4];
    Ids_0046d970 ids;                  // +0x8
    char unknown_14[0x18 - 0x14];
    int* pairs;                        // +0x18, parallel to ids
    char unknown_1c[0x5c - 0x1c];
};

struct Entry_0046e000 {                // 0x5c bytes
    int id;                            // +0x0
    char unknown_4[0x5c - 0x4];
};

Player_0046e0b0* __stdcall FindPlayerByDpid(int id);
extern char g_unitSyncStatusText[];
extern Game* g_game;
int __stdcall FUN_0042a610(Def_0046d970* def);

class UnitSync {
public:
    Less_0046e330 compare;             // +0x00, the unit map's header
    Node_0046e330* head;               // +0x04
    char multi;                        // +0x08
    int size;                          // +0x0c
    std::vector<PlayerSync_0046e0b0> players;    // +0x10
    char unknown_20[0x58 - 0x20];      // the id list, a packet object, two vectors
    int direct;                        // +0x58
    int* first;                        // +0x5c
    int* last;                         // +0x60
    int disabled;                      // +0x64

    Iter_0046e330 End() { return Iter_0046e330(head); }
    Iter_0046e330 Find(const unsigned int* key)
    {
        Iter_0046e330 p = Iter_0046e330(((Class_0046fe60*)this)->FUN_0046fe60(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
    void Send(Target_0046d530* target, void* packet)
    {
        if (direct != 0) {
            SendPacket(target->id, packet);
        } else {
            SendPacket(GetHostDpid(), packet);
        }
    }

    void SendSyncPacket(unsigned int* param_1, Packet_0046d530* param_2, int unused);
    void SendSyncMessage(unsigned char arg, int a, int b, int unused);
    void SendSyncMessageTo(Target_0046d530* target, unsigned char arg, int a, int b, int unused);
    void SendEntryTo(Target_0046d530* target, unsigned char arg, Source_0046d630* src, int unused);
    // The constructor (0x46d040), the destructor (0x46d1a0), ResetEntries
    // (0x46d2e0), HandleSyncPacket (0x46d6c0), this one (0x46d860),
    // ProcessSync (0x46dad0) and
    // PopChangedEntry (0x46e280) are in unit_sync_<address>.cpp: each needs
    // the real <map>, <list> or <vector> instantiations its own way, which
    // this file's view of the unit map cannot share.
    void NotifyEntryChanged(unsigned int key);
    void CheckUnitAvailable(unsigned int key, int y);
    char* GetSyncStatusText();
    int AllPlayersSynced();
    int IsPlayerSynced(int id);
    int GetUnitEntry(Unit_0046e330* unit, UnitSyncEntry* out);
    int ToggleUnitAllowed(Unit_0046e330* unit);
    int DisallowUnit(Unit_0046e330* unit);
    int AllowUnit(Unit_0046e330* unit);
    void SetUnitLimit(Unit_0046e330* unit, int value);
};

// Sends a sync packet to the given player in direct mode, else to the host.
// FUNCTION: 0x46d4c0
void UnitSync::SendSyncPacket(unsigned int* param_1, Packet_0046d530* param_2, int unused)
{
    unsigned int val;
    if (direct != 0)
        val = *param_1;
    else
        val = GetHostDpid();

    param_2->field_2 = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), val, param_2, 14);
}

// Builds a 0xe-byte packet of type 0x1a and sends it, unless sending is
// disabled. The send goes through an inline helper taking a target, called
// here with no target: in "direct" mode it reads the id through that null
// pointer, which is the `mov eax, [0]` in the original.
// Sibling of SendEntryTo (same object, same packet type).

// FUNCTION: 0x46d530
void UnitSync::SendSyncMessage(unsigned char arg, int a, int b, int unused)
{
    if (disabled == 0) {
        Packet_0046d530 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = a;
        packet.field_a = b;
        Send(0, &packet);
    }
}

// Builds a 0xe-byte packet of type 0x1a and sends it to the target, unless
// sending is disabled, then counts it on the target. Sibling of 0x46d530
// (same object and packet) and 0x46d630.

// FUNCTION: 0x46d5b0
void UnitSync::SendSyncMessageTo(Target_0046d530* target, unsigned char arg, int a, int b, int unused)
{
    if (disabled == 0) {
        Packet_0046d530 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = a;
        packet.field_a = b;
        if (direct != 0) {
            SendPacket(target->id, &packet);
        } else {
            SendPacket(GetHostDpid(), &packet);
        }
        target->sent++;
    }
}

// Sends an entry of the unit list, like SendSyncMessageTo.
// FUNCTION: 0x46d630
void UnitSync::SendEntryTo(Target_0046d530* target, unsigned char arg, Source_0046d630* src, int unused)
{
    if (disabled == 0) {
        Packet_0046d530 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = src->field_0;
        packet.entry_a = src->field_8;
        packet.entry_b = src->field_a;
        packet.entry_c = src->field_c;
        if (direct != 0) {
            SendPacket(target->id, &packet);
        } else {
            SendPacket(GetHostDpid(), &packet);
        }
        target->sent++;
    }
}

// Given the unit's key and a y value, makes sure the entry has that y (looking
// the unit type up in g_game when it has none), checks that every player
// lists the key, and then sets the entry's height to whether all of that held
// before letting NotifyEntryChanged recompute the entry.

// FUNCTION: 0x46d970
void UnitSync::CheckUnitAvailable(unsigned int key, int y)
{
    if (disabled != 0)
        return;

    Iter_0046e330 it = ((Class_0046e9b0*)this)->FUN_0046e9b0(key);
    if (it == ((Class_0046e9b0*)this)->End())
        return;

    int h = 1;
    if (y != 0) {
        if (it.ptr->value.y == 0) {
            int n = g_game->count;
            for (int i = 1; i < n; i++) {
                Def_0046d970* def = &g_game->defs[i];
                if (def->key == key) {
                    FUN_0042a610(def);
                    it.ptr->value.y = def->y;
                    break;
                }
            }
        }
    }
    if (y != 0) {
        if (y != it.ptr->value.y)
            h = 0;
    }

    {
        for (Entry_0046d970* e = (Entry_0046d970*)players.begin(); e != (Entry_0046d970*)players.end(); e++) {
            int flag;
            if (y != 0) {
                Player_0046e0b0* pl = FindPlayerByDpid(e->id);
                if (pl == 0)
                    break;
                flag = pl->data->count_0 >= 2 ? 1 : (pl->data->count_0 == 1 && pl->data->count_1 >= 2 ? 1 : 0);
            } else {
                flag = 0;
            }
            int* p2 = e->pairs;
            int* p1 = e->ids.begin;
            int* p3 = e->ids.end;
            while (p1 != p3) {
                if (*p1 == key) {
                    if (flag && *p2 != y)
                        break;
                    // the entry lists the key, so go on with the next one
                    goto next_entry;
                }
                p1++;
                p2++;
            }
            // the key is missing, or its pair disagrees with y
            h = 0;
            break;
        next_entry:
            ;
        }
    }

    it.ptr->value.h = h;
    NotifyEntryChanged(key);
}

// The sync status line: the first player whose units or packets are not
// all accounted for, or "OK".
// FUNCTION: 0x46df40
char* UnitSync::GetSyncStatusText()
{
    if (direct == 0) {
        return 0;
    }
    for (std::vector<PlayerSync_0046e0b0>::iterator it = players.begin(); it != players.end(); ++it) {
        if (it->expected == 0) {
            return "No units_expected sent from player";
        }
        if (it->units.size() != it->expected) {
            sprintf(g_unitSyncStatusText, "expected %d units, got %d", it->expected, it->units.size());
            return g_unitSyncStatusText;
        }
        if (it->sent != it->ackd) {
            sprintf(g_unitSyncStatusText, "packets sent=%d  ackd=%d", it->sent, it->ackd);
            return g_unitSyncStatusText;
        }
    }
    return "OK";
}

// Checks every 0x5c-byte entry of the player vector at +0x10. An entry passes when its
// owner is still a live player of a type that needs no bookkeeping (type 2, or
// type 3 whose team data->field_94 is 2) and, otherwise, when its cached count
// is not zero, matches the size of its id vector, and its two counters agree.
// Returns 1 when nothing needs checking (disabled set, direct clear, no
// entries) or when every entry passes, 0 on the first entry that does not.
// The entry's second half is walked through a second pointer that steps with
// the entry's stride, which is what the original's code does.

// FUNCTION: 0x46e000
int UnitSync::AllPlayersSynced()
{
    if (disabled != 0)
        return 1;
    if (direct == 0)
        return 1;
    Entry_0046e000* p = (Entry_0046e000*)players.begin();
    if (p == (Entry_0046e000*)players.end())
        return 1;
    Sub_0046e000* s = (Sub_0046e000*)((char*)p + 8);
    for (; p != (Entry_0046e000*)players.end(); p++, s++) {
        Player_0046e0b0* pl = FindPlayerByDpid(p->id);
        if (pl != 0) {
            // pl->field_0 is tested again in the second test: the original
            // reloads it rather than reusing the first test's result.
            if (pl->field_0 != 0 && pl->type == 3 && pl->data->field_94 == 2)
                continue;
            if (pl->field_0 != 0 && pl->type == 2)
                continue;
            if (s->count == 0)
                return 0;
            // The count comes from the byte distance between the list's ends,
            // which keeps it a plain arithmetic shift.
            if ((s->ids.begin == 0 ? 0 : ((char*)s->ids.end - (char*)s->ids.begin) >> 2)
                != s->count)
                return 0;
            if (s->field_20 != s->field_24)
                return 0;
        }
    }
    return 1;
}

// Reports whether one player's copy of the shared unit list has
// caught up. The player whose id matches is looked up with FindPlayerByDpid, the
// four early "already done" cases are one || chain, and the entry's
// std::vector of 0x5c-byte per-player records (the same records 0x46df40
// reports on) is scanned for the id. The last test of a record is a
// sent == acked comparison written as `return 1; break;`, which is what keeps
// the shared return-0 block the loop's fallthrough instead of a copy of it.
// FUNCTION: 0x46e0b0
int UnitSync::IsPlayerSynced(int id)
{
    if (direct == 0)
        return 0;
    Player_0046e0b0* player;
    if (disabled != 0
        || (player = FindPlayerByDpid(id)) == 0
        || (player->field_0 != 0 && player->type == 3 && player->data->field_94 == 2)
        || (player->field_0 != 0 && player->type == 2))
        return 1;
    for (std::vector<PlayerSync_0046e0b0>::iterator it = players.begin(); it != players.end(); ++it) {
        if (it->id != id)
            continue;
        if (it->expected == 0 || it->units.size() != it->expected)
            return 0;
        if (it->sent == it->ackd)
            return 1;
        break;
    }
    return 0;
}

// Copies out the unit's entry and returns whether it has a non-empty size.
// FUNCTION: 0x46e330
int UnitSync::GetUnitEntry(Unit_0046e330* unit, UnitSyncEntry* out)
{
    *out = Find(&unit->key).ptr->value;
    return out->w != 0 && out->h != 0;
}

// Toggles the entry's width; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e3c0
int UnitSync::ToggleUnitAllowed(Unit_0046e330* unit)
{
    Node_0046e330* n = Find(&unit->key).ptr;
    n->value.w = (n->value.w == 0);
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Clears the entry's width; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e450
int UnitSync::DisallowUnit(Unit_0046e330* unit)
{
    Node_0046e330* n = Find(&unit->key).ptr;
    n->value.w = 0;
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Sets the entry's width to 1; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e4d0
int UnitSync::AllowUnit(Unit_0046e330* unit)
{
    Node_0046e330* n = Find(&unit->key).ptr;
    n->value.w = 1;
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Sets the last field of the unit's entry and has NotifyEntryChanged look at it.
// FUNCTION: 0x46e550
void UnitSync::SetUnitLimit(Unit_0046e330* unit, int value)
{
    Iter_0046e330 it = Find(&unit->key);
    if (!(it == End())) {
        it.ptr->value.flag = value;
        NotifyEntryChanged(unit->key);
    }
}
