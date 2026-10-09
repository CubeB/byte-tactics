// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
// A method of UnitSync, whose other methods are in unit_sync.cpp.
// Stays in its own file: its PacketSequencer holds Class_0046e610 members, so
// its element view cannot share unit_sync.cpp's PacketSequencer.
#include <list>
#include <map>
#include <vector>

struct Data_0046dad0 {
    char unknown_0[0x94];
    unsigned char kind;     // +0x94
};

#pragma pack(push, 1)
struct PlayerInfo_0046dad0 {
    int field_0; // +0x0
    int field_4; // +0x4
    char unknown_8[0x27 - 0x8];
    Data_0046dad0* data; // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type; // +0x73
};

struct UnitSyncPacket { // 0xe bytes
    unsigned char type;  // +0x0
    unsigned char arg;   // +0x1
    int id;              // +0x2
    int key;             // +0x6
    int value;           // +0xa
};

struct Def_0046dad0 { // 0x249 bytes
    char unknown_0[0x13e];
    unsigned int key; // +0x13e
    int y;            // +0x142
    char unknown_146[0x249 - 0x146];
};

struct Game {
    char unknown_0[0x1438f];
    int count; // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046dad0* defs; // +0x1439b
};
#pragma pack(pop)

// Inline-budget filler, see the header.
inline int Pass(int v) { return v; }

struct UnitSyncEntry {
    int x, y;
    short w, h;
    int unknown_c;
};

// The vector members are modelled by the classes data/symbols.csv names
// for their out-of-line pieces: 0x46e5c0 is the vector constructor body
// (inlined for list_a, a call for list_b), 0x46e5e0 and 0x46e610 are the
// destructors of vectors of 4 and 14 byte elements.
struct Alloc_0046e5c0 {}; // the empty std::allocator temporary

struct Class_0046e5c0 {
    char field_0x0;
    char unknown_1[3];
    int field_0x4;
    int field_0x8;
    int field_0xc;

    Class_0046e5c0* InitTaggedVector(char* param_1) {
        field_0x0 = *param_1;
        field_0x4 = 0;
        field_0x8 = 0;
        field_0xc = 0;
        return this;
    }
};

class Class_0046e5e0 : public Class_0046e5c0 {
  public:
    Class_0046e5e0(const Alloc_0046e5c0& al = Alloc_0046e5c0()) {
        InitTaggedVector((char*)&al);
    }
    ~Class_0046e5e0();
};

class Wrap_0046e5e0 : public Class_0046e5e0 {
  public:
    Wrap_0046e5e0(const Alloc_0046e5c0& al = Alloc_0046e5c0()) : Class_0046e5e0(al) {}
};

class Class_0046e610 {
  public:
    std::vector<int> vec;
    ~Class_0046e610();
};

int __cdecl GetLocalHumanDpid();
int __cdecl GetHostDpid();
void __stdcall SendPacketToPlayer(int a, unsigned int b, void* c, int d);

struct PacketSequencer { // 0x2c bytes, the entry's +0x30 member
    int field_0;
    int field_4;
    int field_8;
    Class_0046e610 list_c; // +0xc
    Class_0046e610 list_d; // +0x1c

    PacketSequencer();
    void SendUnsequenced(unsigned int param_1, void* param_2);
    void Inl(unsigned int param_1, void* param_2) {
        *(int*)((char*)param_2 + 2) = 0;
        SendPacketToPlayer(GetLocalHumanDpid(), param_1, param_2, 0xe);
    }
};

struct Class_0046eaa0 {    // 0x5c bytes, one vector element
    int id;                // +0x0
    Class_0046e5e0 list_a; // +0x4
    Wrap_0046e5e0 list_b;  // +0x14
    int field_24;          // +0x24
    int field_28;          // +0x28
    int field_2c;          // +0x2c
    PacketSequencer sub;   // +0x30

    Class_0046eaa0& operator=(const Class_0046eaa0& src);
};

extern Game* g_game;
extern int DAT_00000000;

int __stdcall ComputeUnitScriptChecksum(Def_0046dad0* def);

// 0x46f7a0 has no name in the exe, so it is modelled as a method of a
// vector subclass to keep the call out of line (it is the out-of-line
// vector<Class_0046eaa0>::insert).
class Vec_0046d860 : public std::vector<Class_0046eaa0> {
  public:
    void InsertPlayerRecord(iterator where, size_type n, const Class_0046eaa0& x);
    void Push(const Class_0046eaa0& x) { InsertPlayerRecord(end(), 1, x); }
};

class UnitSync {
  public:
    std::map<unsigned int, UnitSyncEntry> map; // +0x00
    Vec_0046d860 players;                      // +0x10
    std::list<unsigned int> queue;             // +0x20
    char unknown_2c[0x58 - 0x2c];
    int direct;   // +0x58
    int pendingPlayerCount; // +0x5c
    int checksumProgress; // +0x60
    int disabled; // +0x64

    void ProcessSync();
    void CheckUnitAvailable(unsigned int key, int y);
    void SendSyncPacket(void* target, UnitSyncPacket* packet, int unused);
};

// FUNCTION: 0x46dad0
void UnitSync::ProcessSync() {
    if (disabled != 0)
        return;

    if (direct != 0) {
        int changed = 0;
        std::vector<Class_0046eaa0>::iterator it = players.begin();
        if (it != players.end()) {
            do {
                int live = 0;
                for (int i = 0; i < 10; i++) {
                    PlayerInfo_0046dad0* p =
                        (PlayerInfo_0046dad0*)((char*)g_game + 0x1b63 + i * 0x14b);
                    if (p->field_0 != 0 && p->type == 3 && p->data->kind == 1 &&
                        it->id == p->field_4) {
                        live = 1;
                        break;
                    }
                }
                if (live == 0) {
                    it = players.erase(it);
                    --it;
                    changed = 1;
                }
                ++it;
            } while (it != players.end());
        }

        for (int i = 0; i < 10; i++) {
            PlayerInfo_0046dad0* p = (PlayerInfo_0046dad0*)((char*)g_game + 0x1b63 + i * 0x14b);
            if (p->field_0 != 0 && p->type == 3 && p->data->kind == 1) {
                int found = 0;
                for (std::vector<Class_0046eaa0>::iterator j = players.begin(); j != players.end();
                     ++j) {
                    if (j->id == p->field_4) {
                        found = 1;
                        break;
                    }
                }
                if (found == 0) {
                    Class_0046eaa0 entry;
                    entry.id = p->field_4;
                    // field_24/28/2c are zeroed by assignments after id.
                    entry.field_24 = 0;
                    entry.field_28 = 0;
                    entry.field_2c = 0;
                    // Push models the inlined push_back: the lea of this+0x10
                    // comes before the end() load.
                    players.Push(entry);
                    Class_0046eaa0* e = &players.back();
                    if (disabled == 0) {
                        UnitSyncPacket packet;
                        packet.type = 0x1a;
                        packet.arg = 0;
                        packet.key = 0;
                        packet.value = 0;
                        this->SendSyncPacket(e, &packet, 1);
                        e->field_28++;
                    }
                    changed = 1;
                }
            }
        }

        if (changed != 0) {
            typedef std::map<unsigned int, UnitSyncEntry>::_Imp Tree;
            typedef void (Tree::iterator::*Increment)();
            Increment increment = &Tree::iterator::_Inc;
            // Exactly ten Pass calls spend the inline budget: 9 or fewer inlines
            // vector::_Destroy, 11 or more also un-inlines the list_b wrapper.
            for (std::map<unsigned int, UnitSyncEntry>::iterator k = map.begin(); k != map.end();
                 (k.*increment)()) {
                CheckUnitAvailable(Pass(Pass(Pass(Pass(Pass(Pass(Pass(Pass(Pass(Pass(k->second.x)))))))))), 0);
            }
        }
        return;
    }

    if (pendingPlayerCount > 0) {
        if (checksumProgress < g_game->count) {
            if (checksumProgress == 0) {
                if (GetHostDpid() == -1)
                    return;
                int v = g_game->count - 1;
                if (disabled == 0) {
                    UnitSyncPacket packet;
                    packet.type = 0x1a;
                    packet.arg = 1;
                    packet.key = 0;
                    packet.value = v;
                    if (direct != 0) {
                        ((PacketSequencer*)((char*)this + 0x2c))->SendUnsequenced(DAT_00000000, &packet);
                    } else {
                        unsigned int id = GetHostDpid();
                        ((PacketSequencer*)((char*)this + 0x2c))->SendUnsequenced(id, &packet);
                    }
                }
                checksumProgress = 1;
                return;
            }

            // Nested if/else with this loop: it re-reads g_game first and returns
            // when checksumProgress >= count.
            for (int n = 0; n < 4;) {
                Game* game = g_game;
                if (checksumProgress >= game->count)
                    return;
                Def_0046dad0* def = &game->defs[checksumProgress];
                ComputeUnitScriptChecksum(def);
                // y then key, read into locals before the disabled test.
                int y = def->y;
                unsigned int key = def->key;
                if (disabled == 0) {
                    UnitSyncPacket packet;
                    packet.type = 0x1a;
                    packet.arg = 2;
                    packet.key = key;
                    packet.value = y;
                    // Only this send is the inlined copy (Inl); all others are real
                    // SendUnsequenced calls with the id read into a local first.
                    if (direct != 0) {
                        ((PacketSequencer*)((char*)this + 0x2c))->Inl(DAT_00000000, &packet);
                    } else {
                        unsigned int id = GetHostDpid();
                        ((PacketSequencer*)((char*)this + 0x2c))->SendUnsequenced(id, &packet);
                    }
                }
                n++;
                checksumProgress++;
            }
        } else {
            UnitSyncPacket packet;
            packet.type = 0x1a;
            packet.arg = 4;
            packet.key = 0;
            packet.value = pendingPlayerCount;
            unsigned int id = GetHostDpid();
            ((PacketSequencer*)((char*)this + 0x2c))->SendUnsequenced(id, &packet);
        }
    }
}