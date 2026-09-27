// Decompiled by space-bunny-free. Names are provisional.
#include <vector>

#pragma pack(push, 1)
struct Field_0046d6c0 {               // 4 bytes at +0xa
    union {
        int all;
        struct {
            unsigned char lo;         // +0x0
            unsigned char hi;         // +0x1
            short top;                // +0x2
        } part;
    };
};

struct Packet_0046d6c0 {              // 0xe bytes
    unsigned char type;               // +0x0
    unsigned char arg;                // +0x1
    int field_2;                      // +0x2
    int field_6;                      // +0x6
    Field_0046d6c0 field_a;           // +0xa
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PlayerEntry_0046d6c0 {         // 0x14b bytes
    int id;                           // +0x0, g_game + 0x1b67
    char unknown_4[0x14b - 4];
};

struct Game_0046d6c0 {
    char unknown_0[0x1b67];
    PlayerEntry_0046d6c0 players[10];
};
#pragma pack(pop)

extern Game_0046d6c0* g_game;

struct Shorts_0046d6c0 {
    unsigned short a;                 // +0x0
    unsigned short b;                 // +0x2
};

struct Tail_0046d6c0 {                // 0x10 bytes
    int field_0;                      // +0x0
    Shorts_0046d6c0 field_4;          // +0x4
    int field_8;                      // +0x8
    int field_c;                      // +0xc
};

struct Val_0046d6c0 {                 // 0x14 bytes, the map's value
    int key;                          // +0x0
    Tail_0046d6c0 tail;               // +0x4
};

struct Node_0046d6c0 {
    Node_0046d6c0* left;              // +0x0
    Node_0046d6c0* parent;            // +0x4
    Node_0046d6c0* right;             // +0x8
    unsigned int key;                 // +0xc
    Val_0046d6c0 value;               // +0x10
};

struct Handle_0046d6c0 {              // 5 bytes
    Node_0046d6c0* head;              // +0x0
    unsigned char flag;               // +0x4
};

struct Loc_0046d6c0 {                 // 0x18 bytes
    Handle_0046d6c0 h;                // +0x0
    Tail_0046d6c0 tail;               // +0x8
};

// A std::vector<int>, whose insert() is the out-of-line 0x46e640. Leaving the
// method undefined here is what keeps the call out of line.
class Vec_0046d6c0 {                  // 0x10 bytes
public:
    int pad;                          // +0x0
    int* first;                       // +0x4
    int* last;                        // +0x8
    int* cap;                         // +0xc

    int* begin() { return first; }
    int* end() { return last; }
    void insert(int* pos, int n, int const& val);
};

class Map_0046d6c0 {                  // the map at +0x00
public:
    char compare[4];                  // +0x0
    Node_0046d6c0* head;              // +0x4, the tree's _Head node
    int multi;                        // +0x8
    int size;                         // +0xc

    void FUN_0046ef50(Handle_0046d6c0* out, const Val_0046d6c0* val);
};

struct Entry_0046d6c0 {               // 0x5c bytes
    int id;                           // +0x0
    Vec_0046d6c0 ids;                 // +0x4
    Vec_0046d6c0 pairs;               // +0x14
    int field_24;                     // +0x24
    char unknown_28[0x2c - 0x28];
    unsigned int field_2c;            // +0x2c
    char unknown_30[0x5c - 0x30];
};

class Class_0046d6c0 {
public:
    Map_0046d6c0 map;                            // +0x00
    std::vector<Entry_0046d6c0> players;         // +0x10
    char unknown_20[0x58 - 0x20];
    int direct;                                  // +0x58
    int field_5c;                                // +0x5c
    char unknown_60[0x64 - 0x60];
    int disabled;                                // +0x64

    void FUN_0046d6c0(Packet_0046d6c0* packet, unsigned char player);
    void FUN_0046d860(unsigned int key);
    void FUN_0046d970(unsigned int key, int y);
};

// FUNCTION: 0x46d6c0
void Class_0046d6c0::FUN_0046d6c0(Packet_0046d6c0* packet, unsigned char player)
{
    if (disabled != 0) {
        return;
    }
    field_5c++;
    if (direct != 0) {
        std::vector<Entry_0046d6c0>::iterator i = players.begin();
        if (i != players.end()) {
            unsigned int id = g_game->players[player].id;
            for (; i != players.end(); i++) {
                if (i->id == id) {
                    break;
                }
            }
        }

        switch (packet->arg) {
        case 0:
            break;

        case 1:
            i->field_24 = packet->field_a.all;
            break;

        case 2:
            {
                int* j = i->ids.begin();
                while (j != i->ids.end()) {
                    if (*j == packet->field_6) {
                        break;
                    }
                    j++;
                }
                if (j != i->ids.end()) {
                    return;
                }
            }
            i->ids.insert(i->ids.end(), 1, packet->field_6);
            i->pairs.insert(i->pairs.end(), 1, *(int*)&packet->field_a.all);
            FUN_0046d970(packet->field_6, *(int*)&packet->field_a.all);
            break;

        case 3:
            break;

        case 4:
            if (i->field_2c < (unsigned int)packet->field_a.all) {
                i->field_2c = packet->field_a.all;
            }
            break;
        }
    } else {
        Loc_0046d6c0 loc;
        Shorts_0046d6c0 s;
        Val_0046d6c0 v;
        if (packet->arg != 0 && packet->arg == 3) {
            int f6 = packet->field_6;
            int fc = packet->field_a.part.top;
            s.a = packet->field_a.part.lo;
            s.b = packet->field_a.part.hi;
            v.tail = loc.tail;
            v.key = f6;
            ((Map_0046d6c0*)this)->FUN_0046ef50(&loc.h, &v);
            Node_0046d6c0* node = loc.h.head;
            node->value.key = f6;
            node->value.tail.field_0 = 0;
            node->value.tail.field_4 = s;
            node->value.tail.field_8 = fc;
            FUN_0046d860(packet->field_6);
        }
    }
}
