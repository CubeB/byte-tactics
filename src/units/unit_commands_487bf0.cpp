// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by mimo-v2.6-flash, finished by Sonnet 5.5, Opus and Haiku. Names are provisional.
// The unit commands: the initial mission string of a unit (RunInitialMission),
// the units of a recorded game, handing a unit to another player, the unit
// type tables and the unit category sets.
#include <ctype.h>
#include <stdio.h>
#include <string.h>

struct Item_00485940;

#pragma pack(push, 1)
struct Vec3_00487bf0 {
    int x, y, z;
};

struct Tail_00488570 {
    short f64;                         // +0x0
    short f66;                         // +0x2
    unsigned short b;                  // +0x4
};

struct Player_00488310 {              // 0x14b bytes
    int active;                        // +0x0
    int f4;                            // +0x4
    char unknown_8[0x73 - 8];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f146;                // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Def_00488310 {
    char unknown_0[0x1fa];
    int f1fa;                          // +0x1fa
};

struct Unit {
    int field_0;                       // +0x0
    char unknown_4[0x1e - 4];
    unsigned char b1e;                 // +0x1e
    unsigned char b1f;                 // +0x1f
    char unknown_20[0x3a - 0x20];
    unsigned char b3a;                 // +0x3a
    unsigned char b3b;                 // +0x3b
    char unknown_3c[0x56 - 0x3c];
    unsigned char b56;                 // +0x56
    unsigned char b57;                 // +0x57
    char unknown_58[0x64 - 0x58];
    Tail_00488570 tail;                // +0x64
    Vec3_00487bf0 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    Def_00488310* def;                 // +0x92
    Player_00488310* player;           // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short type;               // +0xa6
    short team;                        // +0xa8
    char unknown_aa[0xfb - 0xaa];
    int fb;                            // +0xfb
    char unknown_ff[0x104 - 0xff];
    union {
        float speed;                   // +0x104
        int speed_bits;
    };
    short s108;                        // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char state;               // +0x10e
    char unknown_10f[0x110 - 0x10f];
    unsigned int flags;                // +0x110
    char unknown_114[0x1c6 - 0x114];
    float field_1c6;                   // +0x1c6
    char unknown_1ca[0x1d2 - 0x1ca];
    float field_1d2;                   // +0x1d2
    float field_1d6;                   // +0x1d6
    void SetStateBits(unsigned char mask, int set);
};

struct Table_00487bf0 {
    char allocator_byte;
    char pad[3];
    Unit** ids;                        // +0x4
};
#pragma pack(pop)

class Class_00438760 {
public:
    unsigned char index;
    char pad[3]; // makes each Outs_t member a dword slot like the original
    Class_00438760(const char* name);
    Class_00438760() {}
};

void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* owner,
                            int id, Vec3_00487bf0* pos, int param_6, int param_7);
void __stdcall GetOrderType(Class_00438760* out, int mode, Unit* unit,
                            int target, Vec3_00487bf0* pos);
int __stdcall FindMissionUnit(char* name, Table_00487bf0* table, int value);
unsigned short __stdcall FindUnitTypeId(const char* name);
void __stdcall AttachUnitToPiece(Unit* unit, int target, int a, int b);

struct Outs_t {
    Class_00438760 g, a, m, u, p;
};

// FUNCTION: 0x487bf0
void __stdcall RunInitialMission(Unit* unit, char* text, Table_00487bf0* table)
{
    int count;
    // Locals stay grouped in small structs: loose scalars get a different frame order.
    struct { float f1, f2; int n; Vec3_00487bf0 pos; int move; } L;
    Outs_t out;
    char buf[256];
    int processed = 0, selected = 0;
    struct { float wf; float pf; int fire; } M2;

    while (*text != 0) {
        if (isspace(*text)) {
            do
                text += 1;
            while (isspace(*text));
        }
        L.n = strcspn(text, ",");
        strncpy(buf, text, L.n);
        text += L.n;
        buf[L.n] = 0;
        if (*text == ',')
            text = text + 1;

        switch (buf[0]) {
        case 'O':
        case 'o': {
            // One of the two reads of unit->flags goes through this pointer: stops the load CSE.
            unsigned int* flags = &unit->flags;
            // Frame 0x28 holds (flags>>18)&3, is the first %d and is read back for the combine,
            // so nothing uninitialised is combined.
            L.move = (unit->flags >> 0x12) & 3;
            M2.fire = (int)((*flags >> 0x14) & 3);
            sscanf(buf + 1, " %d %d", &L.move, &M2.fire);
            unit->flags = (0xffc3ffff & unit->flags)
                          | ((((3 & M2.fire) << 2) | (3 & L.move)) << 0x12);
            break;
        }
        case 'M':
        case 'm': {
            sscanf(buf + 1, " %f %f", &L.f1, &L.f2);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(L.f2 * 65536.0);
            GetOrderType(&out.m, 2, unit, 0, &L.pos);
            AddOrder(out.m, 1, unit, 0, &L.pos, 0, 0);
            processed = 1;
            break;
        }
        case 'U':
        case 'u': {
            sscanf(buf + 1, " %f %f", &L.f1, &L.f2);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(65536.0 * L.f2);
            GetOrderType(&out.u, 5, unit, 0, &L.pos);
            AddOrder(out.u, 1, unit, 0, &L.pos, 0, 0);
            processed = 1;
            break;
        }
        case 'G':
        case 'g': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FindMissionUnit(buf, table, 0);
            if (target != 0) {
                GetOrderType(&out.g, 7, unit, target, 0);
                AddOrder(out.g, 1, unit, target, 0, 0, 0);
                processed = 1;
            }
            break;
        }
        case 'P':
        case 'p': {
            M2.pf = 0.0f;
            sscanf(buf + 1, " %f %f %f", &L.f1, &L.f2, &M2.pf);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(L.f2 * 65536.0);
            GetOrderType(&out.p, 9, unit, 0, &L.pos);
            // The (int)(f * 30.0f) goes straight into AddOrder: no float local.
            AddOrder(out.p, 1, unit, 0, &L.pos, (int)(M2.pf * 30.0f), 0);
            processed = 1;
            selected = 1;
            break;
        }
        case 'A':
        case 'a': {
            if (sscanf(buf + 1, " %f %f", &L.f1, &L.f2) == 2) {
                L.pos.x = (int)(L.f1 * 65536.0);
                L.pos.y = 0;
                L.pos.z = (int)(L.f2 * 65536.0);
                GetOrderType(&out.a, 3, unit, 0, &L.pos);
                AddOrder(out.a, 1, unit, 0, &L.pos, 0, 0);
                selected = 1;
                processed = 1;
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
                unsigned short id = FindUnitTypeId(buf);
                if (id != 0) {
                    // String kinds are implicit conversions in the argument, never a named temporary.
                    AddOrder("ATTACKUTYPE", 1, unit, 0, 0, id, 0);
                    processed = 1;
                }
            }
            break;
        }
        case 'B':
        case 'b': {
            L.n = 1;
            if (buf[1] == 'w' || buf[1] == 'W') {
                sscanf(buf + 2, " %d", &L.n);
                AddOrder("BUILDWEAPON", 1, unit, 0, 0, 0, L.n);
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.] %d %f %f", buf, &L.n, &L.f1, &L.f2);
                L.pos.x = (int)(L.f1 * 65536.0);
                L.pos.y = 0;
                L.pos.z = (int)(65536.0 * L.f2);
                unsigned short id = FindUnitTypeId(buf);
                if (id != 0) {
                    if (unit->field_0 != 0)
                        AddOrder("MOBILEBUILD", 1, unit, 0,
                                     &L.pos, id, L.n);
                    else
                        AddOrder("BUILDINGBUILD", 1, unit, 0,
                                     0, id, L.n);
                    processed = 1;
                }
            }
            break;
        }
        case 'W':
        case 'w':
            if (buf[1] == 'a' || buf[1] == 'A') {
                // count and target are separate locals: leaves target in eax.
                count = sscanf(buf + 2, " %[a-zA-Z0-9.]", buf);
                int target = 0;
                if (count == 1)
                    target = FindMissionUnit(buf, table, 0);
                if (target == 0)
                    target = (int)unit;
                AddOrder("WAITFORATTACK", 1, unit, target,
                             0, 0, 0);
                processed = 1;
            } else {
                M2.wf = 0.0f;
                L.move = 0;
                sscanf(buf + 1, " %f %d", &M2.wf, &L.move);
                AddOrder("WAIT", 1, unit, 0, 0, (int)(M2.wf * 30.0f), L.move);
                processed = 1;
            }
            break;
        case 'D':
        case 'd':
            AddOrder("SELFDESTRUCTFG", 1, unit, 0, 0, 1, 0);
            // After the AddOrder call, not before it.
            processed = 1;
            selected = 1;
            break;
        case 'S':
        case 's':
            AddOrder("MAKESELECTABLE", 1, unit, 0, 0, 0, 0);
            processed = 1;
            selected = 1;
            break;
        case 'I':
        case 'i': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FindMissionUnit(buf, table, 0);
            if (target != 0)
                AttachUnitToPiece(unit, target, -1, 0);
            break;
        }
        }
    }
    if (processed) {
        unit->flags &= ~0x20u;
        if (selected == 0)
            AddOrder("MAKESELECTABLE", 1, unit, 0, 0, 0, 0);
    }
}

#pragma pack(push, 1)
struct UnitType_00488b10 {              // 0x249 bytes
    char unknown_0[0x20];
    char name[0x21e - 0x20];            // +0x20
    unsigned short id;                  // +0x21e
    char unknown_220[0x249 - 0x220];
};

struct Entry_00488310 {               // 0x24 bytes
    char* name;                       // +0x0
    char* unknown_4;                  // +0x4
    char* extra;                      // +0x8
    Vec3_00487bf0 pos;                // +0xc
    short f18;                        // +0x18
    short f1a;                        // +0x1a
    char unknown_1c[0x22 - 0x1c];
    unsigned char player;             // +0x22
    unsigned char flags;              // +0x23
};

class Mission {
public:
    char unknown_0[0xdac];
    Entry_00488310* list;             // +0xdac
    int count;                        // +0xdb0
};

class MissionConditions {
public:
    char unknown_0[0x88];
    int field_88;

    void FUN_004904b0();
    void NotifyUnitCaptured(Unit* unit);
};

struct Game {
    char unknown_0[0x1b63];
    Player_00488310 players[10];      // +0x1b63
    char unknown_2851[0x14267 - 0x2851];
    float field_14267;                // +0x14267
    char unknown_1426b[0x1438f - 0x1426b];
    int count;                        // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_00488b10* types;         // +0x1439b
    char unknown_1439f[0x37ede - 0x1439f];
    float field_37ede;                // +0x37ede
    char unknown_37ee2[0x391e9 - 0x37ee2];
    Mission* net;                     // +0x391e9
    MissionConditions* mission;       // +0x391ed
};
#pragma pack(pop)

extern Game* g_game;

UnitType_00488b10* __stdcall FindUnitType(const char* name);
void __stdcall FUN_0047ddc0(UnitType_00488b10* type, Vec3_00487bf0* pos);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short id,
                                      Vec3_00487bf0 pos, int a, int b, int c);
void __stdcall FatalError(char* message);

// Needed though unused: dropping it moves the f108 multiply onto the other register.
#include <math.h>
// Included only for its declarations, which the symbol counter of CreateMissionUnits needs.
#include <memory.h>
#include <vector>

// std::vector<Unit*> is specialised here rather than taken from <vector>
// because MSVC 5 inlines _Ufill's body into the real (count, value)
// constructor, while the original calls the out-of-line copy (0x406c40) that
// the game's own vector object holds. Everything else is the real container:
// allocator::allocate gives the `if (_N < 0) _N = 0` clamp and operator new
// (not new[]), and ~vector's deallocate is the operator delete at the end.
namespace std {
template<> class vector<Unit*, allocator<Unit*> > {
public:
    typedef Unit* T;
    typedef allocator<Unit*> A;
    typedef A::size_type size_type;

    vector(size_type _N, const T& _V = T(), const A& _Al = A())
        : allocator(_Al)
    {
        _First = allocator.allocate(_N, (void *)0);
        _Ufill(_First, _N, _V);
        _Last = _First + _N;
        _End = _Last;
    }
    ~vector()
        { allocator.deallocate(_First, _End - _First); }
    T& operator[](size_type _P)
        { return (*(_First + _P)); }

protected:
    A allocator;                       // +0x0
    T* _First;                         // +0x4
    T* _Last;                          // +0x8
    T* _End;                           // +0xc
    void _Ufill(T* _F, size_type _N, const T& _X);
};
}

// FUNCTION: 0x488310
void __cdecl CreateMissionUnits()
{
    Player_00488310* p;
    char buf[100];
    int n = g_game->net->count;        // only the constructor takes it; the
    std::vector<Unit*> units(n);       // loops re-read net->count themselves
    for (int i = 0; i < g_game->net->count; i++) {
        Entry_00488310* e = &g_game->net->list[i];
        UnitType_00488b10* item = FindUnitType(e->name);
        if (item == 0) {
            units[i] = 0;
            continue;
        }
        unsigned char pl = (unsigned char)(e->player - 1);
        if (pl >= 10
            || (p = &g_game->players[pl], g_game->players[pl].active == 0)
            || (p->type != 1 && p->type != 2 && p->type != 3)
            || p->f146 == 10) {
            sprintf(buf, "Player number %d invalid for unit %s", e->player, e->name);
            FatalError(buf);
        }
        FUN_0047ddc0(item, &e->pos);
        Unit* u = CreateUnit((unsigned char)(e->player - 1), item->id, e->pos, 1, 1, 0);
        if (u) {
            // The mask is 0xffffff80, not 0x80: keeps the byte zero-extension.
            u->flags = (u->flags & ~0x8000) | ((e->flags & 0xffffff80) << 8);
            u->s108 = (unsigned short)((unsigned)(u->def->f1fa * e->f1a) / 100);
            u->tail.f66 = e->f18;
            units[i] = u;
        }
    }
    for (int j = 0; j < g_game->net->count; j++) {
        Entry_00488310* e = &g_game->net->list[j];
        if (e->extra && units[j])
            RunInitialMission(units[j], e->extra, (Table_00487bf0*)&units);
    }
    if (g_game->net->count <= 0)
        g_game->mission->FUN_004904b0();
}

// Moves a unit to another player. Does nothing when the unit already belongs
// to that player, is not live (0x10000000) or is flagged 0x4000. When the
// current owner is a real player (state 1 or 2) and the new owner is in state
// 3, the unit is instead handed over in place: it is marked (+0xfb = 0x96),
// a 0x18 byte record describing it is sent to the new owner with 0x451df0 and
// it is damaged for the full 30000 (0x489bb0). Otherwise a copy is created
// for the new owner with 0x485f50 and filled either from the record passed in
// or from the old unit, and the old unit's state bits are moved across.
#pragma pack(push, 1)
struct Packet_00488570 {               // 0x18 bytes
    unsigned char type;                // +0x0
    short team;                        // +0x1
    int who;                           // +0x3
    int x;                             // +0x7
    int y;                             // +0xb
    Tail_00488570 tail;                // +0xf
    unsigned char b15;                 // +0x15
    unsigned char b16;                 // +0x16
    unsigned char b17;                 // +0x17
};
#pragma pack(pop)

int __stdcall GetPlayerDpid(Player_00488310* player);
void __stdcall BroadcastPacket(int who, Packet_00488570* packet, int size);
void __stdcall DamageUnit(Unit* source, Unit* target, int amount, int type,
                            unsigned short extra);

// FUNCTION: 0x488570
void __stdcall GiveUnitToPlayer(Unit* unit, Player_00488310* other, Packet_00488570* p)
{
    if (unit->player == other)
        return;
    if (!(unit->flags & 0x10000000))
        return;
    if (unit->flags & 0x4000)
        return;
    g_game->mission->NotifyUnitCaptured(unit);

    Player_00488310* cur = unit->player;
    if (cur->active != 0 && (cur->type == 1 || cur->type == 2)) {
        if (other->active == 0)
            return;
        if (other->type == 3) {
            Packet_00488570 pk;
            unit->flags &= ~0x10;
            unit->fb = 0x96;
            pk.type = 0x14;
            pk.who = GetPlayerDpid(other);
            pk.team = unit->team;
            pk.x = (int)unit->speed;
            pk.y = unit->s108;
            pk.tail = unit->tail;
            unsigned char a = unit->b1f & 2;
            pk.b15 = a ? unit->b1e : 0;
            pk.b16 = a ? unit->b3a : 0;
            pk.b17 = a ? unit->b56 : 0;
            BroadcastPacket(unit->player->f4, &pk, 0x18);
            DamageUnit(0, unit, 30000, 4, 0);
            return;
        }
    }
    if (other->active == 0)
        return;
    if (other->type != 1 && other->type != 2)
        return;

    Unit* n = CreateUnit(other->f146, unit->type, unit->pos, 1, unit->flags & 3, 0);
    if (!n)
        return;
    n->flags &= 0xffc3ffff;
    if (p) {
        n->s108 = (short)p->y;
        n->speed = (float)p->x;
        n->tail = p->tail;
        if (n->b1f & 2)
            n->b1e = p->b15;
        if (n->b3b & 2)
            n->b3a = p->b16;
        if (n->b57 & 2)
            n->b56 = p->b17;
    } else {
        n->s108 = unit->s108;
        n->speed_bits = unit->speed_bits;
        n->tail = unit->tail;
        if (n->b1f & 2)
            n->b1e = unit->b1e;
        if (n->b3b & 2)
            n->b3a = unit->b3a;
        if (n->b57 & 2)
            n->b56 = unit->b56;
        DamageUnit(0, unit, 30000, 4, 0);
    }
    ((Unit*)n)->SetStateBits(unit->state, 1);
    ((Unit*)n)->SetStateBits(~unit->state, 0);
}

// A quicksort of a range of item pointers ordered by ComparePlayers through a
// __stdcall function pointer (the same family as 0x488920 and 0x488960). It
// takes the median of the first, middle and last element, partitions the range
// in place around it, then recurses on the smaller half only and loops on the
// larger one. The fourth parameter is never read: both call sites (0x48576d
// and 0x485778, which pass the first level of this sort inline) and the
// recursive calls pass 0.
typedef int (__stdcall* Pred_00488810)(Item_00485940*, Item_00485940*);

// The byte count of a range, rounded down to a whole number of pointers.
// Stays a helper: the left size is only kept in eax through inlined calls.
static inline int RangeSize_00488810(Item_00485940** low, Item_00485940** high)
{
    return (int)((char*)high - (char*)low) & ~3;
}

void __stdcall QuickSort(Item_00485940** first, Item_00485940** last, Pred_00488810 pred, int unused);

// FUNCTION: 0x488810
void __stdcall QuickSort(Item_00485940** first, Item_00485940** last, Pred_00488810 pred, int unused)
{
    while (((int)((char*)last - (char*)first) & ~3) > 0x40) {
        int len = (int)((char*)last - (char*)first);
        Item_00485940* a = *first;
        Item_00485940* m = first[(len >> 2) / 2];
        Item_00485940* c = last[-1];
        Item_00485940* pivot;

        if (pred(a, m)) {
            if (pred(m, c))
                pivot = m;
            else if (pred(a, c))
                pivot = c;
            else
                pivot = a;
        } else if (pred(a, c)) {
            pivot = a;
        } else {
            pivot = pred(m, c) ? c : m;
        }

        Item_00485940** j = last;
        Item_00485940** i = first;
        for (;;) {
            while (pred(*i, pivot))
                ++i;
            while (pred(pivot, *--j))
                ;
            if (j <= i)
                break;
            Item_00485940* t = *i;
            *i = *j;
            *j = t;
            ++i;
        }

        if (RangeSize_00488810(i, last) <= RangeSize_00488810(first, i)) {
            QuickSort(i, last, pred, 0);
            last = i;
        } else {
            QuickSort(first, i, pred, 0);
            first = i;
        }
    }
}

// std::sort's _Unguarded_insert for a vector of item pointers ordered by
// ComparePlayers, from a file compiled with __stdcall as the default
// (compare 0x488960).
// FUNCTION: 0x488920
void __stdcall InsertShift(Item_00485940** last, Item_00485940* value, Pred_00488810 pred)
{
    for (Item_00485940** m = last; pred(value, *--m); last = m)
        *last = *m;
    *last = value;
}

// std::sort's _Unguarded_partition for a vector of item pointers ordered by
// ComparePlayers, from a file compiled with __stdcall as the default.
// FUNCTION: 0x488960
Item_00485940** __stdcall Partition(Item_00485940** first, Item_00485940** last,
                                        Item_00485940* pivot, Pred_00488810 pred)
{
    for (;; ++first) {
        for (; pred(*first, pivot); ++first)
            ;
        for (; pred(pivot, *--last);)
            ;
        if (last <= first)
            return first;
        Item_00485940* t = *first;
        *first = *last;
        *last = t;
    }
}

// A file-local global std::vector of unit categories: the compiler generates
// its initialiser (0x4889d0) and the destructor it registers with atexit
// (0x488a00). The element is 8 bytes and its destructor releases the
// reference-counted string at +0 through ReleaseRef.
class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

// Copy constructor of the reference-counted string handle (0x4c91a0).
class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

// Constructor of the same handle from a C string (0x4c91b0).
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ReleaseRef(); }
};

// Assignment of the same handle (0x4c93b0).
struct Class_004c93b0 {
    char* ptr;

    Class_004c93b0* Assign(Class_004c93b0* param_1);
};

// Assignment of a {string handle, int} record.
class Class_00489240 {
public:
    Class_004c93b0 name;               // +0x0
    int value;                         // +0x4

    Class_00489240* AssignCategory(Class_00489240* other);
};

class UnitCategory {
public:
    Class_004c91a0 name;               // +0x0
    void* value;                       // +0x4

    UnitCategory(const Class_004c91a0& n, void* v) : name(n) { value = v; }
    UnitCategory(const UnitCategory& other);
    UnitCategory& operator=(const UnitCategory& other)
    {
        ((Class_00489240*)this)->AssignCategory((Class_00489240*)&other);
        return *this;
    }
    ~UnitCategory() { name.ReleaseRef(); }
};

// FUNCTION: 0x4889d0 _$E5
// FUNCTION: 0x488a00 _$E3
static std::vector<UnitCategory> s_unitCategories;

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

static inline int Less_00488a50(const char* a, const char* b)
{
    return _strcmpi(a, b) < 0;
}

// Out of line: the original calls it from CreateMissionUnits.
#pragma auto_inline(off)
// Binary search over the 0x249-byte item table at g_game+0x1439b, comparing
// the name at +0x20 case-insensitively, returning the entry whose name matches
// exactly (0 when there is none).
// FUNCTION: 0x488a50
UnitType_00488b10* __stdcall FindUnitType(const char* name)
{
    // `end` before `lo`: the order decides which registers the base loads use.
    UnitType_00488b10* end = g_game->types + g_game->count;
    UnitType_00488b10* lo = g_game->types + 1;
    int n = (int)(end - lo);
    while (n > 0) {
        int half = n / 2;
        UnitType_00488b10* mid = lo + half;
        if (Less_00488a50(mid->name, name)) {
            lo = mid + 1;
            n = n - half - 1;
        } else {
            n = half;
        }
    }
    if (lo != end && _strcmpi(name, lo->name) == 0)
        return lo;
    return 0;
}
#pragma auto_inline(on)

// Binary search of the game's 0x249-byte table at g_game+0x1439b, starting past
// entry 0, for an entry whose name at +0x20 matches case-insensitively. Returns
// the entry's index at +0x21e, or 0 when there is no such entry (callers mask
// the result with 0xffff, so the return type is unsigned short).
// FUNCTION: 0x488b10
unsigned short __stdcall FindUnitTypeId(const char* name)
{
    // `last` before `first`: the order decides the register of the table base.
    UnitType_00488b10* last = g_game->types + g_game->count;
    UnitType_00488b10* first = g_game->types + 1;
    int n = ((char*)last - (char*)first) / 0x249;
    while (n > 0) {
        int mid = n / 2;
        UnitType_00488b10* e = first + mid;
        if (_strcmpi(e->name, name) < 0) {
            first = e + 1;
            n = n - mid - 1;
        } else {
            n = mid;
        }
    }
    if (first == last || _strcmpi(name, first->name) != 0)
        first = 0;
    if (first)
        return first->id;
    return 0;
}

extern int g_mapLoadFlag;

// FUNCTION: 0x488be0
void InitUnitCategories()
{
    g_mapLoadFlag = 1;
}

// Empties the file-local global vector: frees each element's value, then
// clears the vector, which releases each element's reference-counted name
// through ReleaseRef.
// FUNCTION: 0x488bf0
void FreeUnitCategories()
{
    for (std::vector<UnitCategory>::iterator it = s_unitCategories.begin(); it != s_unitCategories.end(); it++)
        delete it->value;
    s_unitCategories.clear();
    g_mapLoadFlag = 0;
}

// 0x40-byte set (512 bits), as in the callers 0x406db0 and 0x406e40.
class UnitTypeSet {
public:
    int bits[16];                      // the object is 0x40 bytes

    UnitTypeSet() { memset(this, 0, sizeof(UnitTypeSet)); }
    void AddTypeOrCategory(char* text, int* out);
};

// Looks a name up in the global vector of unit categories and, if the name is
// not there, allocates a zeroed 0x40-byte object and inserts a (name, object)
// pair at the position the sorted search ended on. The search loop is the same
// shape as 0x438760.cpp.
// FUNCTION: 0x488c50
UnitTypeSet* __stdcall GetCategoryMask(char* name)
{
    UnitCategory* first = s_unitCategories.begin();
    int n = s_unitCategories.end() - first;
    for (; 0 < n; ) {
        int n2 = n / 2;
        UnitCategory* m = first + n2;
        if (_strcmpi(m->name.data, name) < 0)
            first = ++m, n -= n2 + 1;
        else
            n = n2;
    }
    if (first != s_unitCategories.end() && _strcmpi(first->name.data, name) == 0)
        return (UnitTypeSet*)first->value;

    UnitTypeSet* p = new UnitTypeSet;
    s_unitCategories.insert(first, UnitCategory(Class_004c91b0(name), p));
    return p;
}

// Parses a player list: binary searches the sorted item table (g_game->types,
// 0x249 bytes per item, name at +0x20, id at +0x21e) and, on a hit, sets that
// id's bit in the 0x40-byte set; on a miss it merges the mask that GetCategoryMask
// returns for an alias name. *out is 1 for a single item, 0 for an alias.

// Search and final comparison stay one inlined helper: it fixes the register
// use and the opposite strcmpi argument orders.
static inline char* FindByName(char* first, char* last, char* text)
{
    int n = (last - first) / 0x249;
    while (n > 0) {
        int m = n / 2;
        UnitType_00488b10* e = (UnitType_00488b10*)(first + m * 0x249);
        if (_strcmpi(e->name, text) < 0) {
            n = n - m - 1;
            first = (char*)e + 0x249;
        } else {
            n = m;
        }
    }
    if (first == last || _strcmpi(text, ((UnitType_00488b10*)first)->name) != 0)
        return 0;
    return first;
}

// FUNCTION: 0x488d30
void UnitTypeSet::AddTypeOrCategory(char* text, int* out)
{
    char* base = (char*)g_game->types;
    char* last = base + g_game->count * 0x249;
    char* first = FindByName(base + 0x249, last, text);
    unsigned short v = first ? ((UnitType_00488b10*)first)->id : 0;
    if (v != 0) {
        bits[v >> 5] |= 1 << (v & 31);
        *out = 1;
        return;
    }
    UnitTypeSet* other = GetCategoryMask(text);
    for (int i = 0; i < 16; i++)
        bits[i] |= other->bits[i];
    *out = 0;
}

// FUNCTION: 0x488f30
float __stdcall GetEnergyUse(Unit* unit)
{
    if (unit->field_1c6 != 0.0) {
        return unit->field_1c6;
    }
    if (unit->field_1d2 > 0.0f) {
        return -(g_game->field_37ede * unit->field_1d2);
    }
    if (unit->field_1d6 > 0.0f) {
        return -(g_game->field_14267 * unit->field_1d6);
    }
    return 0.0f;
}

// std::vector<UnitCategory>::insert(iterator, size_type, const T&), out of
// line, for the global vector above. The copy constructor is 0x489260 and the
// assignment 0x489240; the destructor releases the handle through 0x4c9390.
// Taking insert's address is what makes the compiler emit the instantiation.
typedef std::vector<UnitCategory> Vec_00488fb0;
typedef void (Vec_00488fb0::*InsertFn_00488fb0)(
    Vec_00488fb0::iterator, Vec_00488fb0::size_type, const UnitCategory&);

// FUNCTION: 0x488fb0 ?insert@?$vector@VUnitCategory@@V?$allocator@VUnitCategory@@@std@@@std@@QAEXPAVUnitCategory@@IABV3@@Z
InsertFn_00488fb0 g_insert_00488fb0 = &Vec_00488fb0::insert;

// Out of line: the instantiation of insert above comes after this definition and would inline it.
#pragma auto_inline(off)
// Assignment of a record holding a reference-counted string handle (assigned
// by 0x4c93b0) and an int.
// FUNCTION: 0x489240
Class_00489240* Class_00489240::AssignCategory(Class_00489240* other)
{
    name.Assign(&other->name);
    value = other->value;
    return this;
}

#pragma auto_inline(on)

// Copy constructor of a {string handle, int} record.
// FUNCTION: 0x489260
UnitCategory::UnitCategory(const UnitCategory& other)
    : name(other.name), value(other.value)
{
}
