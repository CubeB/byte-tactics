// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial reconstruction. Per-object scheduler tick for the 0x5c-byte entries
// of the std::vector at +0x10 (allocator byte +0x10, _First +0x14, _Last
// +0x18, _End +0x1c).
//
// direct != 0: drop every entry whose owner is no longer a live player
// (PlayerInfo at g_game+0x1b63, stride 0x14b: field_0 nonzero, type at +0x73
// == 3, data->field_94 == 1), then add an entry for every live player with
// none, and if anything changed re-run FUN_0046d970 for every map key.
//
// direct == 0: walk the 0x249-byte unit defs at g_game+0x1439b four at a time,
// sending a 0x1a/2 packet per def, or one 0x1a/1 or 0x1a/4 packet.
//
// STILL DIFFERS (best 27.4%, 1153 bytes against 1024):
//  - Frame 0x80 against 0x7c, and the whole register allocation: the original
//    keeps `this` in edi and zero in ebx, ours uses ebx/ebp.
//  - The original builds the temporary entry at [esp+0x30] with its four
//    sub-vector destructors at [esp+0x34], [esp+0x44], [esp+0x6c], [esp+0x7c];
//    the last overlaps the saved-register slots. Ours emits one out-of-line
//    ~Class_0046eaa0 instead.
//  - The original's erase compacts in place with an out-of-line
//    ??4Class_0046eaa0@@ (0x470040) and _Destroy (0x46eaa0); ours calls
//    _Destroy inline.
//  - The map walk: the original calls _Inc (0x46ea10) directly, ours emits
//    _Lockit/_Tree::_Nil around std::map<unsigned int,int>::iterator.
//  - The two `mov ecx, ds:0` / `mov eax, ds:0` sites (0x46ddb5, 0x46de59) are
//    dead code behind `direct != 0` inside the direct == 0 branch; reproduced
//    here as the DAT_00000000 global.
#include <list>
#include <map>
#include <vector>

struct Data_0046dad0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

#pragma pack(push, 1)
struct PlayerInfo_0046dad0 {
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x27 - 0x8];
    Data_0046dad0* data;               // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
};

struct Packet_0046dad0 {               // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    int field_a;                       // +0xa
};

struct Def_0046dad0 {                  // 0x249 bytes
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
    int y;                             // +0x142
    char unknown_146[0x249 - 0x146];
};

struct Game_0046dad0 {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046dad0* defs;                // +0x1439b
};
#pragma pack(pop)

struct Elem_0046dad0 {
    int value;
};

struct Class_00470560 {                // 0x2c bytes, the entry's +0x30 member
    int field_0;
    int field_4;
    int field_8;
    std::vector<Elem_0046dad0> list_c; // +0xc
    std::vector<Elem_0046dad0> list_d; // +0x1c

    Class_00470560& operator=(const Class_00470560& src);
};

struct Class_0046eaa0 {                // 0x5c bytes, one vector element
    int id;                            // +0x0
    std::vector<Elem_0046dad0> list_a; // +0x4
    std::vector<Elem_0046dad0> list_b; // +0x14
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    Class_00470560 sub;                // +0x30

    Class_0046eaa0& operator=(const Class_0046eaa0& src);
};

class Class_0046cec0 {
public:
    void FUN_0046cec0(unsigned int param_1, void* param_2);
};

class Class_0046d4c0 {
public:
    void FUN_0046d4c0(void* target, Packet_0046dad0* packet, int unused);
};

extern char* g_game;
extern int DAT_00000000;

int __stdcall FUN_0042a610(Def_0046dad0* def);
int __cdecl FUN_0044fe00();
int __cdecl FUN_00450030();
void __stdcall FUN_00451bc0(int a, unsigned int b, void* c, int d);

// 0x46f7a0 has no name in the exe, so it is modelled as a method of a
// vector subclass to keep the call out of line (it is the out-of-line
// vector<Class_0046eaa0>::insert).
class Vec_0046d860 : public std::vector<Class_0046eaa0> {
public:
    void FUN_0046f7a0(iterator where, size_type n, const Class_0046eaa0& x);
};

class Class_0046d860 {
public:
    std::map<unsigned int, int> map;        // +0x00
    std::vector<Class_0046eaa0> players;    // +0x10
    std::list<unsigned int> queue;          // +0x20
    char unknown_2c[0x58 - 0x2c];
    int direct;                             // +0x58
    int field_5c;                           // +0x5c
    int field_60;                           // +0x60
    int disabled;                           // +0x64

    void FUN_0046dad0();
    void FUN_0046d970(unsigned int key, int y);
};

// FUNCTION: 0x46dad0
void Class_0046d860::FUN_0046dad0()
{
    if (disabled != 0)
        return;

    if (direct != 0) {
        int changed = 0;
        std::vector<Class_0046eaa0>::iterator it = players.begin();
        while (it != players.end()) {
            int live = 0;
            for (int i = 0; i < 10; i++) {
                PlayerInfo_0046dad0* p =
                    (PlayerInfo_0046dad0*)(g_game + 0x1b63 + i * 0x14b);
                if (p->field_0 != 0 && p->type == 3 && p->data->field_94 == 1
                    && p->field_4 == it->id) {
                    live = 1;
                    break;
                }
            }
            if (live != 0) {
                ++it;
            } else {
                it = players.erase(it);
                changed = 1;
            }
        }

        for (int i = 0; i < 10; i++) {
            PlayerInfo_0046dad0* p =
                (PlayerInfo_0046dad0*)(g_game + 0x1b63 + i * 0x14b);
            if (p->field_0 != 0 && p->type == 3 && p->data->field_94 == 1) {
                int found = 0;
                for (std::vector<Class_0046eaa0>::iterator j = players.begin();
                     j != players.end(); ++j) {
                    if (j->id == p->field_4) {
                        found = 1;
                        break;
                    }
                }
                if (found == 0) {
                    Class_0046eaa0 entry;
                    entry.id = p->field_4;
                    ((Vec_0046d860*)&players)->FUN_0046f7a0(players.end(), 1, entry);
                    Class_0046eaa0* e = &players.back();
                    if (disabled == 0) {
                        Packet_0046dad0 packet;
                        packet.type = 0x1a;
                        packet.arg = 0;
                        packet.field_6 = 0;
                        packet.field_a = 0;
                        ((Class_0046d4c0*)this)->FUN_0046d4c0(e, &packet, 1);
                        e->field_28++;
                    }
                    changed = 1;
                }
            }
        }

        if (changed != 0) {
            for (std::map<unsigned int, int>::iterator k = map.begin();
                 k != map.end(); ++k) {
                FUN_0046d970(k->first, 0);
            }
        }
        return;
    }

    if (field_5c <= 0)
        return;

    Game_0046dad0* game = (Game_0046dad0*)g_game;
    Class_0046cec0* sender = (Class_0046cec0*)((char*)this + 0x2c);

    if (field_60 >= game->count) {
        Packet_0046dad0 packet;
        packet.type = 0x1a;
        packet.arg = 4;
        packet.field_6 = 0;
        packet.field_a = field_5c;
        sender->FUN_0046cec0(FUN_00450030(), &packet);
        return;
    }

    if (field_60 == 0) {
        if (FUN_00450030() == -1)
            return;
        int v = game->count - 1;
        if (disabled == 0) {
            Packet_0046dad0 packet;
            packet.type = 0x1a;
            packet.arg = 1;
            packet.field_6 = 0;
            packet.field_a = v;
            if (direct != 0)
                sender->FUN_0046cec0(DAT_00000000, &packet);
            else
                sender->FUN_0046cec0(FUN_00450030(), &packet);
        }
        field_60 = 1;
        return;
    }

    int n = 0;
    for (;;) {
        game = (Game_0046dad0*)g_game;
        if (field_60 >= game->count)
            break;
        Def_0046dad0* def = &game->defs[field_60];
        FUN_0042a610(def);
        if (disabled == 0) {
            Packet_0046dad0 packet;
            packet.type = 0x1a;
            packet.arg = 2;
            packet.field_6 = def->key;
            packet.field_a = def->y;
            if (direct != 0)
                sender->FUN_0046cec0(DAT_00000000, &packet);
            else
                sender->FUN_0046cec0(FUN_00450030(), &packet);
        }
        n++;
        field_60++;
        if (n >= 4)
            break;
    }
}
