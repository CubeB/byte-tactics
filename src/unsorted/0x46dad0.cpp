// Decompiled by deepseek-v4.1. Names are provisional.
// Started by deepseek-v4.1-flash, continued by GPT-6.
// Partial: 28.9%, ours 1049 bytes versus original 1024. The whole register
// allocation is relabelled, which is what keeps the diff large: the original
// keeps `this` in edi and the constant 0 in ebx (xor ebx,ebx right after
// mov edi,ecx), pointers in ebp and the live-player flag in esi, while ours
// puts `this` in esi, 0 in ebp and the flag in edi, so almost every
// instruction shows a register mismatch even where the shape is right.
// What did move the score: the vector-cleanup walk is
// `if (begin != end) { do { scan 10 players; if (!live) { it = players.erase(it);
// --it; changed = 1; } ++it; } while (it != players.end()); }` (the original
// decrements the cached iterator so the bottom ++it lands right: 28.1 -> 28.8),
// and computing (Class_0046cec0*)((char*)this + 0x2c) at each call site instead
// of hoisting it into a local (28.8 -> 28.9).
// Three concrete layout bugs left: (1) in the map walk the original reads the
// node key at [node+0x10] (MSVC5 _Node has _Color/_Isnil before _Value) but
// our real <map> iterator->first reads [node+0xc], so this function needs a
// manual node type with the value at +0x10 and the out-of-line _Inc reached
// through the real std::map iterator's member pointer (0x46ea10), not a real
// map iteration; (2) the temporary entry is built with a cdecl sequence and
// an `add esp,4` where the original has the __stdcall Class_0046e5c0 and
// Class_0046cbe0 calls (the list_b/sub constructors must stay out of line,
// so /Ob2's budget must run out at the same point); (3) in the arg-2 send the
// original's direct path is NOT a FUN_0046cec0 call: it is
// `packet.field_2 = 0; FUN_00451bc0(FUN_0044fe00(), DAT_00000000, &packet, 0xe);`
// (push 0xe / push &packet / push DAT / store [esp+0x2e] / call / push eax /
// call), but writing that scored 28.7% (1062 bytes) twice, so the earlier
// FUN_0046cec0 form is kept; the next attempt should retry it together with a
// fix for the field_2 store position (the original emits it after the three
// argument pushes, which a plain `packet.field_2 = 0;` statement does not).
// Reload the live game count after the network helper, keep the native
// nested-vector types and
// call the real nested constructor to initialise the entry's three dwords.
// Vector erase/temporary cleanup and register allocation still differ. 768
// header sets did not improve the 27.6% version this replaced.
// Retry by deepseek-v4.1-flash: tools/headers.py --cpp (768 header sets) is
// flat at 28.9%, no set beats the base file. Variants tried and all flat or
// worse: boolean guard spellings (`if (disabled)`), an explicit char* alias
// for the player scan, and hoisting `changed` to the top of the function
// (24.8%). The whole-file register rotation this=esi / zero=ebp / pinfo=edi /
// vecptr=eax versus the original this=edi / zero=ebx / pinfo=esi / vecptr=ebp
// is untouched by any of these, and the ~200-byte entry-construction and
// destructor region (Class_0046eaa0 entry, list_a inlined, list_b out of line
// at 0x46e5c0, sub at 0x46cbe0) still emits a different instruction sequence.
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

struct Game_0046dad0 {
    char unknown_0[0x1438f];
    int count; // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046dad0* defs; // +0x1439b
};
#pragma pack(pop)

struct Rect_0046e160 {
    int x, y;
    short w, h;
    int unknown_c;
};

struct Elem_004702a0 {
    int value;
};

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a, b, c;
    short d;
};
#pragma pack(pop)
void __stdcall FUN_00470030(int);
namespace std {
template <> inline void allocator<Elem_0046faf0>::destroy(Elem_0046faf0* p) {
    FUN_00470030((int)p);
}
}

struct Class_0046cbe0 { // 0x2c bytes, the entry's +0x30 member
    int field_0;
    int field_4;
    int field_8;
    std::vector<Elem_0046faf0> list_c; // +0xc
    std::vector<Elem_0046faf0> list_d; // +0x1c

    Class_0046cbe0();
};

struct Class_0046eaa0 {                // 0x5c bytes, one vector element
    int id;                            // +0x0
    std::vector<Elem_004702a0> list_a; // +0x4
    std::vector<Elem_004702a0> list_b; // +0x14
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    Class_0046cbe0 sub;                // +0x30

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
                        p->field_4 == it->id) {
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
                    if (p->field_4 == j->id) {
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
            typedef std::map<unsigned int, Rect_0046e160>::_Imp Tree;
            typedef void (Tree::iterator::*Increment)();
            Increment increment = &Tree::iterator::_Inc;
            for (std::map<unsigned int, Rect_0046e160>::iterator k = map.begin(); k != map.end();
                 (k.*increment)()) {
                FUN_0046d970(k->first, 0);
            }
        }
        return;
    }

    if (field_5c <= 0)
        return;

    Game_0046dad0* game = (Game_0046dad0*)g_game;

    if (field_60 >= game->count) {
        Packet_0046dad0 packet;
        packet.type = 0x1a;
        packet.arg = 4;
        packet.field_6 = 0;
        packet.field_a = field_5c;
        ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(FUN_00450030(), &packet);
        return;
    }

    if (field_60 == 0) {
        if (FUN_00450030() == -1)
            return;
        int v = ((Game_0046dad0*)g_game)->count - 1;
        if (disabled == 0) {
            Packet_0046dad0 packet;
            packet.type = 0x1a;
            packet.arg = 1;
            packet.field_6 = 0;
            packet.field_a = v;
            if (direct != 0)
                ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(DAT_00000000, &packet);
            else
                ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(FUN_00450030(), &packet);
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
                ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(DAT_00000000, &packet);
            else
                ((Class_0046cec0*)((char*)this + 0x2c))->FUN_0046cec0(FUN_00450030(), &packet);
        }
        n++;
        field_60++;
        if (n >= 4)
            break;
    }
}
