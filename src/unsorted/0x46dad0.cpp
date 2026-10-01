// Decompiled by deepseek-v4.1. Names are provisional, finished by deepseek-v4.1-flash.
// deepseek-v4.1-flash retry (10-minute timebox): 29.8% stands, 1049 vs 1024
// bytes; in the check.py diff the MINUS side is the original and the PLUS side
// is ours, and the very first hunk (@@ -2,168 +2,181 @@) shows the split is
// allocation-level from the prologue: original homes `this` in EDI and the
// zero var in EBX (ours ESI / EBP) and the [esp+0x14]/[esp+0x18]/[esp+0x1c]
// slots are permuted, matching the (a) note below.
// Partial: 29.8%, ours 1049 bytes versus original 1024. This pass changed
// FUN_0046d970(k->first, 0) to FUN_0046d970(k->second.x, 0) in the map walk
// (29.5 -> 29.8): the mapped Rect's x always equals the key (0x46d6c0 stores
// r.x = packet->field_6 before map[packet->field_6] = r), so the original
// reads node+0x10 (the mapped value, whose first int is the key) rather than
// node+0xc (the pair's first).
// Still differs:
// (a) Whole-file register rotation. Ours is this=esi / zero=ebp /
//     pinfo-or-flag=edi / vector-ptr=eax; original is this=edi / zero=ebx /
//     flag=esi / vector-ptr=ebp, so nearly every instruction shows a register
//     mismatch. Tested this pass (all flat or worse): bool for the two
//     scan flags (28.0), unsigned int changed (flat), swapping the
//     declaration order of changed and the erase iterator (flat), declaring
//     the iterator uninitialised then assigning (flat). 768 header sets via
//     tools/headers.py --cpp were already flat at 28.9.
// (b) Entry construction (0x46dc31-0x46dcda). Original: inline list_a vector
//     ctor (al byte + three zero dwords), then an OUT-OF-LINE
//     Class_0046e5c0::FUN_0046e5c0 call with this=entry+0x14, then the
//     out-of-line Class_0046cbe0 sub ctor, and the destructors call 0x46e5e0
//     on entry+4 and entry+0x14. Ours inlines both vector ctors (reading the
//     [esp+0x13] allocator byte twice) and emits no 0x46e5c0 call. The two
//     vector ctors are the same type (same dtor at 0x46e5e0), so this is the
//     /Ob2 inline-budget wall; the ctor/dtor symbols in data/symbols.csv are
//     named for two different provisional classes
//     (Class_0046e5c0::FUN_0046e5c0 vs Class_0046e5e0::~Class_0046e5e0),
//     which also blocks modelling the member as one hand-written type.
// (c) Arg-2 direct send (0x46de59). Original uses
//     FUN_00451bc0(FUN_0044fe00(), DAT_00000000, &packet, 0xe) (push 0xe /
//     push &packet / push DAT / mov [esp+0x2e],0 / call 0x44fe00 / push eax /
//     call 0x451bc0), not a FUN_0046cec0 call; writing that with
//     `packet.field_2 = 0;` scored 29.6 (1066 bytes) both before and inside
//     the direct branch, so the FUN_0046cec0 form is kept.
// (d) The arg-2 helper pushes are still ~25 bytes larger overall; the
//     remaining size gap is the inlined list_b ctor from (b).
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
                FUN_0046d970(k->second.x, 0);
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
