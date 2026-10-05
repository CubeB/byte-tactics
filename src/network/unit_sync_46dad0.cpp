// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
// MATCH (was 29.8%). What made it match:
// - The tail (field_5c > 0) is a nested if/else (field_60 < count, else the
//   arg-4 packet last), with `for (n = 0; n < 4;) { ...; n++; field_60++; }`
//   whose first statement re-reads g_game and returns when field_60 >= count.
// - The loop's direct send is an inlined copy of FUN_0046cec0 (Class_0046cec0::Inl:
//   field_2 = 0, then FUN_00451bc0(FUN_0044fe00(), id, &packet, 0xe)); all other
//   sends are real FUN_0046cec0 calls with the id read into a local first.
// - key and y are read into locals (y first) before the `disabled` test.
// - The vector members of Class_0046eaa0 are the classes symbols.csv names:
//   Class_0046e5c0::FUN_0046e5c0 is the vector ctor body (inline, so list_a gets
//   it inlined and list_b, one wrapper level deeper, keeps the call), and
//   ~Class_0046e5e0 / ~Class_0046e610 are the vector dtors. field_24/28/2c are
//   zeroed by assignments after id (the compiler sinks them below the pushes).
// - Which STL helpers stay out of line (vector::_Destroy in the inlined erase,
//   the list_b ctor) is the /Ob2 inline budget: Pass() below is a trivial inline
//   used 10 deep in the map walk purely to spend that budget. 9 or fewer keeps
//   _Destroy inlined, 11 or more un-inlines the list_b wrapper too.
// - Push() on the Vec_0046d860 subclass models the inlined push_back, so
//   `lea ecx, [this+0x10]` comes before the end() load.
#include <list>
#include <map>
#include <vector>

struct Data_0046dad0 {
    char unknown_0[0x94];
    unsigned char field_94; // +0x94
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

struct Packet_0046dad0 { // 0xe bytes
    unsigned char type;  // +0x0
    unsigned char arg;   // +0x1
    int field_2;         // +0x2
    int field_6;         // +0x6
    int field_a;         // +0xa
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

struct Rect_0046e160 {
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

    Class_0046e5c0* FUN_0046e5c0(char* param_1) {
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
        FUN_0046e5c0((char*)&al);
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

struct Class_0046cbe0 { // 0x2c bytes, the entry's +0x30 member
    int field_0;
    int field_4;
    int field_8;
    Class_0046e610 list_c; // +0xc
    Class_0046e610 list_d; // +0x1c

    Class_0046cbe0();
};

struct Class_0046eaa0 {    // 0x5c bytes, one vector element
    int id;                // +0x0
    Class_0046e5e0 list_a; // +0x4
    Wrap_0046e5e0 list_b;  // +0x14
    int field_24;          // +0x24
    int field_28;          // +0x28
    int field_2c;          // +0x2c
    Class_0046cbe0 sub;    // +0x30

    Class_0046eaa0& operator=(const Class_0046eaa0& src);
};

int __cdecl FUN_0044fe00();
int __cdecl FUN_00450030();
void __stdcall FUN_00451bc0(int a, unsigned int b, void* c, int d);

class Class_0046cec0 {
  public:
    void FUN_0046cec0(unsigned int param_1, void* param_2);
    void Inl(unsigned int param_1, void* param_2) {
        *(int*)((char*)param_2 + 2) = 0;
        FUN_00451bc0(FUN_0044fe00(), param_1, param_2, 0xe);
    }
};

class Class_0046d4c0 {
  public:
    void FUN_0046d4c0(void* target, Packet_0046dad0* packet, int unused);
};

extern char* g_game;
extern int DAT_00000000;

int __stdcall FUN_0042a610(Def_0046dad0* def);

// 0x46f7a0 has no name in the exe, so it is modelled as a method of a
// vector subclass to keep the call out of line (it is the out-of-line
// vector<Class_0046eaa0>::insert).
class Vec_0046d860 : public std::vector<Class_0046eaa0> {
  public:
    void FUN_0046f7a0(iterator where, size_type n, const Class_0046eaa0& x);
    void Push(const Class_0046eaa0& x) { FUN_0046f7a0(end(), 1, x); }
};

class Class_0046d860 {
  public:
    std::map<unsigned int, Rect_0046e160> map; // +0x00
    std::vector<Class_0046eaa0> players;       // +0x10
    std::list<unsigned int> queue;             // +0x20
    char unknown_2c[0x58 - 0x2c];
    int direct;   // +0x58
    int field_5c; // +0x5c
    int field_60; // +0x60
    int disabled; // +0x64

    void FUN_0046dad0();
    void FUN_0046d970(unsigned int key, int y);
};

// FUNCTION: 0x46dad0
void Class_0046d860::FUN_0046dad0() {
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
                        (PlayerInfo_0046dad0*)(g_game + 0x1b63 + i * 0x14b);
                    if (p->field_0 != 0 && p->type == 3 && p->data->field_94 == 1 &&
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
            PlayerInfo_0046dad0* p = (PlayerInfo_0046dad0*)(g_game + 0x1b63 + i * 0x14b);
            if (p->field_0 != 0 && p->type == 3 && p->data->field_94 == 1) {
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
                    entry.field_24 = 0;
                    entry.field_28 = 0;
                    entry.field_2c = 0;
                    ((Vec_0046d860*)&players)->Push(entry);
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
            typedef std::map<unsigned int, Rect_0046e160>::_Imp Tree;
            typedef void (Tree::iterator::*Increment)();
            Increment increment = &Tree::iterator::_Inc;
            for (std::map<unsigned int, Rect_0046e160>::iterator k = map.begin(); k != map.end();
                 (k.*increment)()) {
                FUN_0046d970(Pass(Pass(Pass(Pass(Pass(Pass(Pass(Pass(Pass(Pass(k->second.x)))))))))), 0);
            }
        }
        return;
    }

    if (field_5c > 0) {
        if (field_60 < ((Game*)g_game)->count) {
            if (field_60 == 0) {
                if (FUN_00450030() == -1)
                    return;
                int v = ((Game*)g_game)->count - 1;
                if (disabled == 0) {
                    Packet_0046dad0 packet;
                    packet.type = 0x1a;
                    packet.arg = 1;
                    packet.field_6 = 0;
                    packet.field_a = v;
                    if (direct != 0) {
                        ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(DAT_00000000, &packet);
                    } else {
                        unsigned int id = FUN_00450030();
                        ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(id, &packet);
                    }
                }
                field_60 = 1;
                return;
            }

            for (int n = 0; n < 4;) {
                Game* game = (Game*)g_game;
                if (field_60 >= game->count)
                    return;
                Def_0046dad0* def = &game->defs[field_60];
                FUN_0042a610(def);
                int y = def->y;
                unsigned int key = def->key;
                if (disabled == 0) {
                    Packet_0046dad0 packet;
                    packet.type = 0x1a;
                    packet.arg = 2;
                    packet.field_6 = key;
                    packet.field_a = y;
                    if (direct != 0) {
                        ((Class_0046cec0*)((char*)this + 0x2c))->Inl(DAT_00000000, &packet);
                    } else {
                        unsigned int id = FUN_00450030();
                        ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(id, &packet);
                    }
                }
                n++;
                field_60++;
            }
        } else {
            Packet_0046dad0 packet;
            packet.type = 0x1a;
            packet.arg = 4;
            packet.field_6 = 0;
            packet.field_a = field_5c;
            unsigned int id = FUN_00450030();
            ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(id, &packet);
        }
    }
}