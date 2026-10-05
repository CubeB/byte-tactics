// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, GPT-6.1-sol and space-bunny-free, edited by deepseek-v4.1, finished by Sonnet 5.5. Names are provisional.
// Handles one player-list packet. With `direct` set, the packet updates the
// player's entry in the vector at +0x10 (arg 1 stores a word, arg 2 appends to
// the entry's two std::vector<int> members through the out-of-line
// vector::insert at 0x46e640 and then calls 0x46d970, arg 4 raises a
// maximum). Otherwise an arg 3 packet stores a rectangle in the
// std::map<unsigned int, Rect_0046e160> at +0x0 and calls 0x46d860.
//
// The map store is `map[key] = r` with <map>'s operator[] (see Map_0046d6c0):
// it inserts a default-constructed value, which MSVC 5 builds as an
// uninitialised temporary (the copies of uninitialised stack words in the
// original), then the locally built rectangle is copied over the mapped value.
// The rectangle's fields live in registers across the insert call, which is
// where the zero in ebx comes from (`r.y = 0`). Earlier passes wrote the store
// as a hand-built pair and field-by-field writes and stopped at 83.6%.
//
// The tree is declared by hand as a template called _Tree with the real tree's
// first two arguments, only so that the call to its out-of-line insert carries
// the name data/symbols.csv gives 0x46ef50 (check.py derives it from the
// mangled name); the real std::map here expands the whole insert inline (700
// bytes).
#include <utility>
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

struct Game {
    char unknown_0[0x1b67];
    PlayerEntry_0046d6c0 players[10];
};
#pragma pack(pop)

extern Game* g_game;


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

struct Rect_0046e160 {                // 0x10 bytes
    int x;                            // +0x0
    int y;                            // +0x4
    short w;                          // +0x8
    short h;                          // +0xa
    int unknown_c;                    // +0xc
};

// The tree behind a std::map<unsigned int, Rect_0046e160> (MSVC 5's <xtree>
// layout: empty allocator and key_compare at +0 and +1, _Head, _Multi, _Size).
// It is declared by hand, as a template with <xtree>'s own name and first two
// arguments, because the real tree's insert is 0x46ef50 and <xtree>'s inline
// insert would otherwise be expanded here (the real map gives 700 bytes).
struct Kfn_0046d6c0 {};
struct Less_0046d6c0 {};
struct Alloc_0046d6c0 {};

template<class _K, class _Ty, class _Kfn, class _Pr, class _A>
class _Tree {
public:
    struct _Node {
        _Node* _Left;                 // +0x0
        _Node* _Parent;               // +0x4
        _Node* _Right;                // +0x8
        _Ty _Value;                   // +0xc, the pair (key, Rect at +0x10)
    };
    struct _Pairib {
        _Node* first;                 // the iterator
        bool second;
        _Pairib() {}
    };

    char _Alloc;                      // +0x0
    char _Pred;                       // +0x1
    _Node* _Head;                     // +0x4
    int _Multi;                       // +0x8
    int _Size;                        // +0xc

    _Pairib insert(const _Ty& _V);    // 0x46ef50
};

typedef std::pair<const unsigned int, Rect_0046e160> Value_0046d6c0;

// std::map<unsigned int, Rect_0046e160>::operator[] from MSVC 5's <map>: insert
// a default value if the key is new, then hand back the mapped value. The
// default `Rect_0046e160()` is an uninitialised temporary in this compiler,
// which is where the copies of uninitialised stack words in the original come
// from.
class Map_0046d6c0 : public _Tree<unsigned int, Value_0046d6c0, Kfn_0046d6c0,
                                   Less_0046d6c0, Alloc_0046d6c0> {
public:
    Rect_0046e160& operator[](const unsigned int& k)
    {
        _Pairib p = insert(Value_0046d6c0(k, Rect_0046e160()));
        return p.first->_Value.second;
    }
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

class Class_0046d860 {
public:
    void FUN_0046d860(unsigned int key);
    void FUN_0046d970(unsigned int key, int y);
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
            {
                Vec_0046d6c0& v = i->ids;
                v.insert(v.end(), 1, packet->field_6);
            }
            {
                Vec_0046d6c0& w = i->pairs;
                w.insert(w.end(), 1, packet->field_a.all);
            }
            ((Class_0046d860*)this)->FUN_0046d970(packet->field_6, packet->field_a.all);
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
        if (packet->arg != 0 && packet->arg == 3) {
            Rect_0046e160 r;
            r.x = packet->field_6;
            r.y = 0;
            r.w = packet->field_a.part.lo;
            r.h = packet->field_a.part.hi;
            r.unknown_c = packet->field_a.part.top;
            map[packet->field_6] = r;
            ((Class_0046d860*)this)->FUN_0046d860(packet->field_6);
        }
    }
}