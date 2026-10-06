// Decompiled by space-bunny-free. Names are provisional.
// A method of UnitSync, whose other methods are in unit_sync.cpp; this one
// stays apart because it needs the real <map>, <list> or <vector>
// instantiations its own way.
//
// The object begins with a std::map<unsigned int, Rect> (its find() is the
// out-of-line 0x46e9b0; tree layout and field use as in unit_sync.cpp) and
// keeps a vector of 0x5c-byte entries at +0x14, as in unit_sync.cpp. Given the unit's key and a y value, it makes sure the entry's
// Rect has that y (looking the unit type up in g_game when it has none),
// checks that every entry lists the key, and then sets the Rect's height to
// whether all of that held before letting NotifyEntryChanged recompute the entry.

struct Rect_0046d970 {
    int x;                             // +0x10
    int y;                             // +0x14
    short w;                           // +0x18
    short h;                           // +0x1a
    int unknown_c;                     // +0x1c
};

struct Node_0046d970 {
    Node_0046d970* left;               // +0x0
    Node_0046d970* parent;             // +0x4
    Node_0046d970* right;              // +0x8
    unsigned int key;                  // +0xc
    Rect_0046d970 value;               // +0x10
};

class Iter_0046d970 {
public:
    Node_0046d970* ptr;

    Iter_0046d970() {}
    Iter_0046d970(Node_0046d970* p) : ptr(p) {}
    bool operator==(const Iter_0046d970& other) const { return ptr == other.ptr; }
};

struct Less_0046d970 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046e9b0 {
public:
    Less_0046d970 compare;
    Node_0046d970* head;               // +0x4

    Iter_0046d970 End() { return Iter_0046d970(head); }
    Iter_0046d970 FUN_0046e9b0(const unsigned int& key);
};

#pragma pack(push, 1)
struct Data_0046d970 {
    char unknown_0[0xa7];
    unsigned char count_0;             // +0xa7
    unsigned char count_1;             // +0xa8
};

struct Player_0046d970 {
    char unknown_0[0x27];
    Data_0046d970* data;               // +0x27
};

struct Def_0046d970 {                  // 0x249 bytes
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
    int y;                             // +0x142
    char unknown_146[0x249 - 0x146];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[8];
    Def_0046d970* defs;                // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

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

class UnitSync {
public:
    char unknown_0[0x14];
    Entry_0046d970* begin;             // +0x14
    Entry_0046d970* end;               // +0x18
    char unknown_1c[0x64 - 0x1c];
    int field_64;                      // +0x64

    void NotifyEntryChanged(unsigned int key);
    void CheckUnitAvailable(unsigned int key, int y);
};

int __stdcall FUN_0042a610(Def_0046d970* def);
Player_0046d970* __stdcall FindPlayerByDpid(int id);

// FUNCTION: 0x46d970
void UnitSync::CheckUnitAvailable(unsigned int key, int y)
{
    if (field_64 != 0)
        return;

    Iter_0046d970 it = ((Class_0046e9b0*)this)->FUN_0046e9b0(key);
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
        for (Entry_0046d970* e = begin; e != end; e++) {
            int flag;
            if (y != 0) {
                Player_0046d970* pl = FindPlayerByDpid(e->id);
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
