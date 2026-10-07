// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// A method of UnitSync, whose other methods are in unit_sync.cpp; this one
// stays apart because it needs the real <map>, <list> or <vector>
// instantiations its own way.
//
// Sends the "units expected" (packet type 0x1a, sub-type 3) notice to every
// player whose sync record this object holds, then queues the player's id on
// the insertion-ordered list at +0x20 unless it is already there.  The class
// holds a std::map<unsigned int, Event> at +0x00 (its tree is the first
// member, so the out-of-line find at 0x46e9b0 is called on this), a
// std::vector<Player> at +0x10 and a std::list<unsigned int> at +0x20
// (_Head +0x24, _Size +0x28).
#include <list>
#include <map>
#include <vector>

#pragma pack(push, 1)
struct Packet_0046d860 {              // 0xe bytes
    unsigned char type;               // +0x0
    unsigned char arg;                // +0x1
    int field_2;
    int field_6;                      // +0x6
    unsigned char field_a;            // +0xa
    unsigned char field_b;            // +0xb
    short field_c;                    // +0xc
};
#pragma pack(pop)

struct Event_0046d860 {               // 0x10 bytes, the map's value
    unsigned int field_0;             // +0x0
    unsigned int field_4;             // +0x4
    unsigned char field_8;            // +0x8
    unsigned char field_9;            // +0x9
    unsigned char field_a;            // +0xa
    unsigned char field_b;            // +0xb
    short field_c;                    // +0xc
    char unknown_e[0x10 - 0xe];
};

struct Player_0046d860 {              // 0x5c bytes
    unsigned int id;                  // +0x0
    char unknown_4[0x28 - 0x4];
    int sent;                         // +0x28
    char unknown_2c[0x5c - 0x2c];
};

struct Node_0046d860 {
    Node_0046d860* left;              // +0x0
    Node_0046d860* parent;            // +0x4
    Node_0046d860* right;             // +0x8
    unsigned int key;                 // +0xc
    Event_0046d860 value;             // +0x10
};

class Iter_0046d860 {
public:
    Node_0046d860* ptr;

    Iter_0046d860() {}
    Iter_0046d860(Node_0046d860* p) : ptr(p) {}
};

class Class_0046e9b0 {
public:
    char compare[4];                  // the less<> functor, +0x0
    Node_0046d860* head;              // +0x4
    Iter_0046d860 FUN_0046e9b0(const unsigned int& key);
};

class UnitSync {
public:
    std::map<unsigned int, Event_0046d860> map;     // +0x00
    std::vector<Player_0046d860> players;           // +0x10
    std::list<unsigned int> queue;                  // +0x20
    char unknown_2c[0x58 - 0x2c];
    int direct;                                     // +0x58
    char unknown_5c[0x64 - 0x5c];
    int disabled;                                   // +0x64

    void NotifyEntryChanged(unsigned int param_1);
    void SendSyncPacket(Player_0046d860* target, Packet_0046d860* packet, int unused);
};

// FUNCTION: 0x46d860
void UnitSync::NotifyEntryChanged(unsigned int param_1)
{
    if (disabled != 0) {
        return;
    }
    if (direct != 0) {
        Iter_0046d860 it = ((Class_0046e9b0*)this)->FUN_0046e9b0(param_1);
        for (std::vector<Player_0046d860>::iterator i = players.begin(); i != players.end(); ++i) {
            // v is taken before the disabled check: MSVC then keeps it.ptr in eax
            // across the loop. The check stays a positive block, not a continue.
            Event_0046d860* v = &it.ptr->value;
            if (disabled == 0) {
                Packet_0046d860 packet;
                packet.type = 0x1a;
                packet.arg = 3;
                packet.field_6 = v->field_0;
                packet.field_a = v->field_8;
                packet.field_b = v->field_a;
                packet.field_c = v->field_c;
                ((UnitSync*)this)->SendSyncPacket(&*i, &packet, 1);
                i->sent++;
            }
        }
    }
    for (std::list<unsigned int>::iterator it = queue.begin(); it != queue.end(); ++it) {
        if (*it == param_1) {
            return;
        }
    }
    queue.push_back(param_1);
}
