// Decompiled by Opus, Haiku, Sonnet, space-bunny-free, Space Bunny Free,
// deepseek-v4.1, deepseek-v4.1-flash, GPT-5.6-Terra, GPT-6.1-sol, GPT-6,
// DeepSeek V4.1 Flash, Claude Opus 5.5, Sonnet 5.5 and claude-sonnet-5-5.
// Names are provisional.
// The unit synchronisation module: the game-event reporting the statistics DLL
// calls, PacketSequencer (the sequenced 0xe-byte command packets), UnitSync
// (the shared unit list every player is checked against) and the out-of-line
// std::map<unsigned int, UnitSyncEntry>, std::vector and std::list members
// with the hand-written _Tree helpers, from 0x46c620 to 0x4707a0. The files
// that need a hand-written std::vector, the /Gi insert, the UnitSyncPlayer
// COMDAT and the conflicting _Tree models stay in their own files
// (unit_sync_46ca60.cpp, unit_sync_46cc10.cpp, unit_sync_46d1a0.cpp,
// unit_sync_46d2e0.cpp, unit_sync_46dad0.cpp, unit_sync_46e640.cpp,
// unit_sync_46eba0.cpp, unit_sync_46f7a0.cpp, unit_sync_470040.cpp and
// unit_sync_player.cpp).
#include <stdio.h>
#include <utility>

// This block must stay: it stands in for <xutility> with __stdcall templates,
// and _XUTILITY_ keeps out the header's __cdecl ones.
#define _XUTILITY_
namespace std {
template <class _II, class _OI>
inline _OI __stdcall copy(_II _F, _II _L, _OI _X)
{
    for (; _F != _L; ++_X, ++_F)
        *_X = *_F;
    return (_X);
}
template <class _BI1, class _BI2>
inline _BI2 __stdcall copy_backward(_BI1 _F, _BI1 _L, _BI2 _X)
{
    while (_F != _L)
        *--_X = *--_L;
    return (_X);
}
template <class _II1, class _II2>
inline bool __stdcall equal(_II1 _F, _II1 _L, _II2 _X)
{
    return (mismatch(_F, _L, _X).first == _L);
}
template <class _II1, class _II2, class _Pr>
inline bool __stdcall equal(_II1 _F, _II1 _L, _II2 _X, _Pr _P)
{
    return (mismatch(_F, _L, _X, _P).first == _L);
}
template <class _FI, class _Ty>
inline void __stdcall fill(_FI _F, _FI _L, const _Ty& _X)
{
    for (; _F != _L; ++_F)
        *_F = _X;
}
template <class _OI, class _Sz, class _Ty>
inline void __stdcall fill_n(_OI _F, _Sz _N, const _Ty& _X)
{
    for (; 0 < _N; --_N, ++_F)
        *_F = _X;
}
template <class _II1, class _II2>
inline bool __stdcall lexicographical_compare(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2)
{
    for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
        if (*_F1 < *_F2)
            return (true);
        else if (*_F2 < *_F1)
            return (false);
    return (_F1 == _L1 && _F2 != _L2);
}
template <class _II1, class _II2, class _Pr>
inline bool __stdcall lexicographical_compare(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _Pr _P)
{
    for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
        if (_P(*_F1, *_F2))
            return (true);
        else if (_P(*_F2, *_F1))
            return (false);
    return (_F1 == _L1 && _F2 != _L2);
}
#define _MAX _cpp_max
#define _MIN _cpp_min
template <class _Ty>
inline const _Ty& __stdcall _cpp_max(const _Ty& _X, const _Ty& _Y)
{
    return (_X < _Y ? _Y : _X);
}
template <class _Ty, class _Pr>
inline const _Ty& __stdcall _cpp_max(const _Ty& _X, const _Ty& _Y, _Pr _P)
{
    return (_P(_X, _Y) ? _Y : _X);
}
template <class _Ty>
inline const _Ty& __stdcall _cpp_min(const _Ty& _X, const _Ty& _Y)
{
    return (_Y < _X ? _Y : _X);
}
template <class _Ty, class _Pr>
inline const _Ty& __stdcall _cpp_min(const _Ty& _X, const _Ty& _Y, _Pr _P)
{
    return (_P(_Y, _X) ? _Y : _X);
}
template <class _II1, class _II2>
inline pair<_II1, _II2> __stdcall mismatch(_II1 _F, _II1 _L, _II2 _X)
{
    for (; _F != _L && *_F == *_X; ++_F, ++_X)
        ;
    return (pair<_II1, _II2>(_F, _X));
}
template <class _II1, class _II2, class _Pr>
inline pair<_II1, _II2> __stdcall mismatch(_II1 _F, _II1 _L, _II2 _X, _Pr _P)
{
    for (; _F != _L && _P(*_F, *_X); ++_F, ++_X)
        ;
    return (pair<_II1, _II2>(_F, _X));
}
template <class _Ty>
inline void __stdcall swap(_Ty& _X, _Ty& _Y)
{
    _Ty _Tmp = _X;
    _X = _Y, _Y = _Tmp;
}
}

#include <vector>
#include <map>
#include <list>
#include <yvals.h>

// --- the game-event reporting the statistics DLL calls -----------------------

struct Rect_0046c620 {                  // 16 bytes, the rect at net+0x465
    int x0;
    int y0;
    int x1;
    int y1;
};

struct Name_0046c620 {                  // 17 bytes, the player's name
    char text[16];
    char flag;
};

struct Net_4c97b0 {                     // the object at g_game+0x14
    Name_0046c620 name;                 // +0x00
    char unknown_11[0x4c9 - 0x11];
    int field_4c9;                      // +0x4c9
    char unknown_4cd[4];
};

#include "../map/mission.h"

#pragma pack(push, 1)
struct PlayerEntry_0046d6c0 {           // 0x14b bytes, one slot of g_game's players
    int id;                             // +0x0, g_game + 0x1b67
    char unknown_4[0x14b - 4];
};

struct Def_0046d040 {                   // 0x249 bytes, one unit type
    char unknown_0[0x13e];
    unsigned int key;                   // +0x13e
    int y;                              // +0x142
    char unknown_146[0x15a - 0x146];
    int field_15a;                      // +0x15a
    char unknown_15e[0x241 - 0x15e];
    union {
        unsigned int flags241;          // +0x241
        struct {
            unsigned int bits_0 : 23;
            unsigned int flag_23 : 1;   // bit 23
            unsigned int bits_24 : 8;
        } flags_241;
    };
    unsigned int flags;                 // +0x245
};
#pragma pack(pop)

struct UnitSync;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14];
    Net_4c97b0 net;                     // +0x14
    char unknown_25[0x1b67 - 0x14 - sizeof(Net_4c97b0)];
    PlayerEntry_0046d6c0 players[10];   // +0x1b67
    char unknown_283d[0x2a30 - 0x1b67 - sizeof(PlayerEntry_0046d6c0) * 10];
    UnitSync* sync;                     // +0x2a30
    char unknown_2a34[0x2a42 - 0x2a30 - sizeof(UnitSync*)];
    unsigned char player;               // +0x2a42
    char unknown_2a43[0x1438f - 0x2a42 - sizeof(unsigned char)];
    int count;                          // +0x1438f
    char unknown_14393[0x1439b - 0x1438f - sizeof(int)];
    Def_0046d040* defs;                 // +0x1439b
    char unknown_1439f[0x391e9 - 0x1439b - sizeof(Def_0046d040*)];
    Mission* mapInfo;                   // +0x391e9
    char unknown_391ed[0x39201 - 0x391e9 - sizeof(Mission*)];
    char field_39201[1];                // +0x39201
};
#pragma pack(pop)

extern Game* g_game;

typedef int (__stdcall *SendFn_0046c620)(int, Rect_0046c620*, void*, int, Name_0046c620*,
                                         int, int, int, void*, void*);

extern int g_onlineReportScores;
extern Name_0046c620 g_reportPlayerName;
extern int* g_onlineReportPlayers;
extern void* DAT_0051e57c;
extern SendFn_0046c620 DAT_0051e584;
extern int DAT_0051e58c;
extern int DAT_0051e590;
extern int g_reportFlags;
extern void (*DAT_0051e580)(void);
extern int (__stdcall* g_riReportGameChat)(int, int);

Rect_0046c620* __stdcall GetSessionGuidInstance(Net_4c97b0* p);
int __stdcall GetCreatedLobbyInterface(Net_4c97b0* p);
int __stdcall QueryOnlineFlags(int mode);
int __stdcall RIReport(int, Rect_0046c620*, void*, int, Name_0046c620*, int,
                           int, int, void*, void*);
int FillScoreTables();
int __stdcall RIReportGameChat(int arg1, int arg2);

// FUNCTION: 0x46c620
int __stdcall ReportGameEvent(int msg)
{
    if (!DAT_0051e590 && !DAT_0051e58c)
        return 4;
    if (!g_onlineReportPlayers || !DAT_0051e57c || !g_onlineReportScores)
        return 1;

    Rect_0046c620 rect = *GetSessionGuidInstance(&g_game->net);
    int thing = GetCreatedLobbyInterface(&g_game->net);

    if (msg == 1) {
        g_reportPlayerName = g_game->net.name;
        char* p = g_reportPlayerName.text + 15;
        while (p > g_reportPlayerName.text && *p == ' ')
            *p-- = 0;
    }

    int id = FillScoreTables();

    if (DAT_0051e590) {
        if (msg == 1 || msg == 6 || msg == 7)
            g_reportFlags = QueryOnlineFlags(msg == 1 ? 1 : 2 + (msg != 6));
        if (g_reportFlags & 3) {
            // its own statement, not an argument: the call has to be emitted
            // ahead of the other nine arguments being set up
            int team = (int)g_game->mapInfo->GetMissionName();
            if (RIReport(msg, &rect, (char*)&g_game->field_39201, thing, &g_reportPlayerName,
                             team, g_game->player,
                             id, g_onlineReportPlayers, DAT_0051e57c))
                DAT_0051e590 = 0;
        }
    }

    if (DAT_0051e58c) {
        int team = (int)g_game->mapInfo->GetMissionName();
        DAT_0051e584(msg, &rect, (char*)&g_game->field_39201, thing, &g_reportPlayerName,
                     team, g_game->player,
                     id, g_onlineReportPlayers, DAT_0051e57c);
    }

    return 0;
}

// FUNCTION: 0x46c810
int __stdcall ReportGameChat(int msg)
{
    if ((DAT_0051e590 != 0 && (g_reportFlags & 8)) || DAT_0051e58c != 0) {
        FillScoreTables();
        if (DAT_0051e590 != 0 && (g_reportFlags & 8)) {
            if (RIReportGameChat(g_onlineReportPlayers[g_game->player], msg))
                DAT_0051e590 = 0;
        }
        if (DAT_0051e58c != 0)
            return g_riReportGameChat(g_onlineReportPlayers[g_game->player], msg);
        return 0;
    }
    return 4;
}

// FUNCTION: 0x46c8b0
void ReportIntervalTimer(void)
{
    if (DAT_0051e58c) {
        DAT_0051e580();
    }
}

// FUNCTION: 0x46c8c0
void __stdcall FUN_0046c8c0(int)
{
}

// FUNCTION: 0x46c8d0
void FUN_0046c8d0(void)
{
}

// --- UnitSync: the shared unit list ------------------------------------------

struct UnitSyncEntry {                 // the map's mapped type, 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

struct Node_0046e330 {
    Node_0046e330* left;               // +0x0
    Node_0046e330* parent;             // +0x4
    Node_0046e330* right;              // +0x8
    unsigned int key;                  // +0xc
    UnitSyncEntry value;               // +0x10
};

class Iter_0046e330 {
public:
    Node_0046e330* ptr;
    Iter_0046e330() {}
    Iter_0046e330(Node_0046e330* p) : ptr(p) {}
    bool operator==(const Iter_0046e330& other) const { return ptr == other.ptr; }
};

#pragma pack(push, 1)
struct Unit_0046e330 {
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
};

struct Data_0046e0b0 {
    char unknown_0[0x94];
    unsigned char field_94;              // +0x94
    char unknown_95[0xa7 - 0x95];
    unsigned char count_0;               // +0xa7
    unsigned char count_1;               // +0xa8
};

struct Player_0046e0b0 {                // 0x14b bytes
    int field_0;                         // +0x0
    char unknown_4[0x27 - 0x4];
    Data_0046e0b0* data;                 // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                  // +0x73
    char unknown_74[0x14b - 0x74];
};

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

struct Ids_0046e000 {
    int* begin;                        // +0x0
    int* end;                          // +0x4
    int* capacity;                     // +0x8
};

struct Sub_0046e000 {
    Ids_0046e000 ids;                 // +0x0
    char unknown_c[0x1c - 0xc];
    int count;                        // +0x1c
    int field_20;                     // +0x20
    int field_24;                     // +0x24
    char unknown_28[0x5c - 0x28];
};

struct Entry_0046e000 {                // 0x5c bytes
    int id;                            // +0x0
    char unknown_4[0x5c - 0x4];
};
#pragma pack(pop)

struct Unit_0046e0b0 { char unknown_0[0x5c]; };

struct PlayerSync_0046e0b0 {            // 0x5c bytes
    int id;                              // +0x0
    std::vector<Unit_0046e0b0*> units;   // +0x4
    char unknown_14[0x24 - 0x14];
    int expected;                        // +0x24
    int sent;                            // +0x28
    int ackd;                            // +0x2c
    char unknown_30[0x5c - 0x30];
};

struct Less_0046e330 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

// The map's out-of-line find() (0x46e9b0) walks the tree through
// Class_0046fe60's LowerBound, the tree's lower_bound().
struct Node_0046e9b0 {
    Node_0046e9b0* left;               // +0x0
    Node_0046e9b0* parent;             // +0x4
    Node_0046e9b0* right;              // +0x8
    unsigned int key;                  // +0xc
    UnitSyncEntry value;               // +0x10
};

class Iter_0046e9b0 {
public:
    Node_0046e9b0* ptr;

    Iter_0046e9b0() {}
    Iter_0046e9b0(Node_0046e9b0* p) : ptr(p) {}
    bool operator==(const Iter_0046e9b0& other) const { return ptr == other.ptr; }
};

struct Less_0046e9b0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046e9b0 {
public:
    Less_0046e9b0 compare;
    Node_0046e9b0* head;               // +0x4

    Iter_0046e9b0 End() { return Iter_0046e9b0(head); }
    Iter_0046e9b0 FindExact(const unsigned int& key);
};

// Shaped like std::_Tree<...>::_Lbound(const _K&) from MSVC 5's <xtree> for a
// tree keyed by unsigned int (the std::map<unsigned int, Rect> of 0x46e330),
// under a lock object; DAT_0051e598 is the tree's _Nil node and head->parent
// is the root.
struct Node_0046fe60 {
    Node_0046fe60* left;            // +0x0
    Node_0046fe60* parent;          // +0x4
    Node_0046fe60* right;           // +0x8
    unsigned int key;               // +0xc
};

struct Less_0046fe60 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_0046fe60 {
public:
    char allocator;                 // +0x0
    Less_0046fe60 key_compare;      // +0x1
    Node_0046fe60* head;            // +0x4
    Node_0046fe60* LowerBound(const unsigned int* key);
};

#pragma pack(push, 1)
struct Packet_0046d530 {               // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    union {
        int field_a;                   // +0xa
        struct {
            unsigned char entry_a;     // +0xa
            unsigned char entry_b;     // +0xb
            short entry_c;             // +0xc
        };
    };
};

// A player's slot in the sync: its id and how many packets were sent to it.
struct Target_0046d530 {
    unsigned int id;                   // +0x0
    char unknown_4[0x24];
    int sent;                          // +0x28
};

struct Source_0046d630 {
    int field_0;                       // +0x0
    char unknown_4[4];
    unsigned char field_8;             // +0x8
    char unknown_9;
    unsigned char field_a;             // +0xa
    char unknown_b;
    short field_c;                     // +0xc
};

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

struct Event_0046e280 {                // 0x10 bytes
    unsigned int key;                  // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
};

struct Cmp_0046d040 {                  // the map's empty key_compare
    char x;
};

struct Alloc_0046d040 {                // the map's empty allocator
    char x;
};

// std::_Tree<...>::_Init() from MSVC 5's <xtree>, written out by hand:
// DAT_0051e598 is the tree's shared _Nil node and DAT_0051e59c its
// reference count (_Nilrefs). Nodes are 0x24 bytes (a 0x14-byte value).
struct Node_0046f720 {
    Node_0046f720* left;               // +0x0
    Node_0046f720* parent;             // +0x4
    Node_0046f720* right;              // +0x8
    char value[0x14];                  // +0xc
    int color;                         // +0x20 (0 red, 1 black)
};

extern Node_0046f720* DAT_0051e598;
extern int DAT_0051e59c;

class Class_0046f720 {                 // the std::map's tree header at +0x00
public:
    char field_0;                      // +0x0
    char field_1;                      // +0x1
    char unknown_2[2];
    Node_0046f720* head;               // +0x4
    char multi;                        // +0x8
    char unknown_9[3];
    int size;                          // +0xc

    // Empty classes taken by value: the prologue copies them from the param slot.
    Class_0046f720(Cmp_0046d040 c, Alloc_0046d040 a)
        : field_0(c.x), field_1(a.x), multi(0)
    {
        Init();
    }

    Node_0046f720* Buynode(Node_0046f720* parent, int color)
    {
        Node_0046f720* s = (Node_0046f720*)operator new(sizeof(Node_0046f720));
        s->parent = parent;
        s->color = color;
        return s;
    }

    void Init();

    UnitSyncEntry& operator[](unsigned int key)
    {
        return ((std::map<unsigned int, UnitSyncEntry>*)this)->operator[](key);
    }
};

class Sub_0046d040 {                   // the object at +0x2c
public:
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8

    void SendUnsequenced(void* packet, int to);
};

class Sub2_0046d040 {                  // the object at +0x58
public:
    int flag;                          // +0x0
    int* first;                        // +0x4
    int* last;                         // +0x8
    int* end;                          // +0xc
};

// UnitSyncPlayer (0x5c bytes): one player's unit-sync state, with four
// std::vector members.
#pragma pack(push, 2)
struct Elem_0046faf0 {                 // 14 bytes
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

class UnitSyncPlayer {                 // the vector's element, 0x5c bytes
public:
    char unknown_0[0x5c];

    ~UnitSyncPlayer();
};

class Class_0046e610 {
public:
    std::vector<int> vec;
    ~Class_0046e610();
};

class Class_0046e160 {
public:
    char allocator;                    // +0x0
    Less_0046e330 compare;             // +0x1
    char unknown_2[2];
    Node_0046e330* head;               // +0x4
    char unknown_8[0x64 - 0x8];
    int field_64;                      // +0x64

    Iter_0046e330 End() { return Iter_0046e330(head); }
    Iter_0046e330 Find(const unsigned int* key)
    {
        Iter_0046e330 p = Iter_0046e330((Node_0046e330*)((Class_0046fe60*)this)->LowerBound(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
    void ApplyToUnitTypes();
};

int __cdecl GetLocalHumanDpid();
unsigned int GetHostDpid();
void __stdcall SendPacketToPlayer(int a, unsigned int b, void* c, int d);
Player_0046e0b0* __stdcall FindPlayerByDpid(int id);
int __stdcall ComputeUnitScriptChecksum(Def_0046d040* def);
void ProtectUnitDefsReadWrite();
void ProtectUnitDefsReadOnly();
extern char g_unitSyncStatusText[];

// Inlined copy of Class_0046cec0::SendUnsequenced (a method that ignores this).
static inline void SendPacket(unsigned int to, void* packet)
{
    *(int*)((char*)packet + 2) = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), to, packet, 0xe);
}

class UnitSync {
public:
    Class_0046f720 map;                        // +0x00
    std::vector<PlayerSync_0046e0b0> players;  // +0x10
    std::list<int> ids;                        // +0x20
    Sub_0046d040 sub;                          // +0x2c
    std::vector<int> list_a;                   // +0x38
    std::vector<int> list_b;                   // +0x48
    union {
        Sub2_0046d040 sub2;                    // +0x58
        struct {
            int direct;                        // +0x58
            union {
                int* first;                    // +0x5c
                int pendingPlayerCount;
            };
            int* last;                         // +0x60
            int disabled;                      // +0x64
        };
    };

    UnitSync(int param);

    Iter_0046e330 End() { return Iter_0046e330((Node_0046e330*)((Class_0046e9b0*)this)->head); }
    Iter_0046e330 Find(const unsigned int* key)
    {
        Iter_0046e330 p = Iter_0046e330((Node_0046e330*)((Class_0046fe60*)this)->LowerBound(key));
        return (p == End() || ((Class_0046e9b0*)this)->compare(*key, p.ptr->key)) ? End() : p;
    }
    void Send(Target_0046d530* target, void* packet)
    {
        if (direct != 0) {
            SendPacket(target->id, packet);
        } else {
            SendPacket(GetHostDpid(), packet);
        }
    }

    void SendSyncPacket(unsigned int* param_1, Packet_0046d530* param_2, int unused);
    void SendSyncMessage(unsigned char arg, int a, int b, int unused);
    void SendSyncMessageTo(Target_0046d530* target, unsigned char arg, int a, int b, int unused);
    void SendEntryTo(Target_0046d530* target, unsigned char arg, Source_0046d630* src, int unused);
    void HandleSyncPacket(Packet_0046d6c0* packet, unsigned char player);
    void ReceiveSyncPacket(void* param_1, int param_2);
    void NotifyEntryChanged(unsigned int param_1);
    void CheckUnitAvailable(unsigned int key, int y);
    char* GetSyncStatusText();
    int AllPlayersSynced();
    int IsPlayerSynced(int id);
    int PopChangedEntry(Event_0046e280* out);
    int GetUnitEntry(Unit_0046e330* unit, UnitSyncEntry* out);
    int ToggleUnitAllowed(Unit_0046e330* unit);
    int DisallowUnit(Unit_0046e330* unit);
    int AllowUnit(Unit_0046e330* unit);
    void SetUnitLimit(Unit_0046e330* unit, int value);
};

static inline Def_0046d040* Defs_0046d040()
{
    return g_game->defs;
}

// The flag expression in its own small inline helper.
static inline bool FlagOf_0046d040(Def_0046d040* d)
{
    return (d->flags >> 16) & 1;
}

// The delete in 0x46c920 expands this view of the object, whose members are
// the real std::map, std::vector and std::list instantiations.
class UnitSyncDel_0046c920 {
public:
    std::map<unsigned int, UnitSyncEntry> map;     // +0x00
    std::vector<UnitSyncPlayer> elems;             // +0x10
    std::list<int> ids;                            // +0x20
    int field_2c;                                  // +0x2c
    int field_30;                                  // +0x30
    int field_34;                                  // +0x34
    Class_0046e610 field_38;                       // +0x38
    std::vector<int> field_48;                     // +0x48
    short field_58;                                // +0x58
    int field_5c;                                  // +0x5c
    int field_60;                                  // +0x60
    int field_64;                                  // +0x64

    ~UnitSyncDel_0046c920() {}
};

// --- the unit-sync object's lifecycle ----------------------------------------

// FUNCTION: 0x46c8e0
void __stdcall CreateUnitSync(int param_1)
{
    *(UnitSync**)((char*)g_game + 0x2a30) = new UnitSync(param_1);
}

// Releases the overlay object at g_game+0x2a30 (UnitSync, built by 0x46c8e0
// and its constructor 0x46d040): `if (obj) delete obj;` with the whole
// ~UnitSync inlined here.
// FUNCTION: 0x46c920
// __fastcall: keeps the erase loop comparing the iterator slot directly.
void __fastcall DeleteUnitSync()
{
    if (g_game->sync)
        delete (UnitSyncDel_0046c920*)g_game->sync;
    g_game->sync = 0;
}

// --- PacketSequencer ---------------------------------------------------------

#pragma pack(push, 1)
struct Packet_0046cef0 {         // 0xe bytes
    unsigned char type;          // +0x0
    unsigned char arg;           // +0x1
    unsigned int id;             // +0x2
    int field_6;                 // +0x6
    int field_a;                 // +0xa
};
#pragma pack(pop)

// A list seen as a std::vector of packets (allocator byte, _First, _Last,
// _End): ReceiveSequenced reads the lists through it, and InsertPacket is that
// vector's insert(end(), count, val), out of line.
class Class_0046eba0 {
public:
    char unknown_0[4];
    Packet_0046cef0* first;      // +0x4
    Packet_0046cef0* last;       // +0x8
    Packet_0046cef0* end;        // +0xc
    void InsertPacket(Packet_0046cef0* where, int count, Packet_0046cef0* val);
};

struct PacketSequencer {
    int field_0;                       // +0x00
    unsigned int cur;                  // +0x04
    unsigned int max;                  // +0x08
    // Nested struct with its own out-of-line operator=: sets the inline depth
    // that keeps list_c's _Destroy out of line and list_d's inlined.
    std::vector<Elem_0046faf0> list_c; // +0x0c (16 bytes, _First at +0x10)
    std::vector<Elem_0046faf0> list_d; // +0x1c (operator= is 0x4707a0)
    PacketSequencer();
    PacketSequencer& operator=(const PacketSequencer& rhs);
    // In unit_sync_46cc10.cpp: it needs a hand-written std::vector, which
    // cannot share a file with <vector>.
    void SendSequenced(Elem_0046faf0* param_1, Elem_0046faf0* param_2);
    void ReceiveSequenced(Packet_0046cef0* packet, int param_2, void* param_3, unsigned int target);
};

// The constructor: two empty vectors (the allocator bytes are copied from an
// uninitialised temporary) and three dwords zeroed in the body.
// FUNCTION: 0x46cbe0
PacketSequencer::PacketSequencer()
{
    field_0 = 0;
    cur = 0;
    max = 0;
}

// A method that ignores `this`: its callers (0x46dad0) set ecx to the object
// at +0x2c before each call. Clears the packet's +2 field and sends the
// 0xe-byte packet. 0x46d530, 0x46d5b0 and 0x46d630 inline a copy of it.
class Class_0046cec0 {
public:
    void SendUnsequenced(unsigned int param_1, void* param_2);
};

// FUNCTION: 0x46cec0
void Class_0046cec0::SendUnsequenced(unsigned int param_1, void* param_2)
{
    *(int*)((char*)param_2 + 2) = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), param_1, param_2, 0xe);
}

// Dispatches a 0xe-byte command packet. arg 0x65 re-sends the stored copy that
// has the same id; any other id widens the range and, when it is the one right
// after cur, runs cur and then every queued entry that follows it,
// asking the sender for the next one (packet 0x1a 0x65) whenever the queue runs
// dry.
// FUNCTION: 0x46cef0
void PacketSequencer::ReceiveSequenced(Packet_0046cef0* packet, int param_2, void* param_3, unsigned int target)
{
    if (packet->arg == 0x65) {
        for (Packet_0046cef0* p = ((Class_0046eba0*)&list_c)->first; p != ((Class_0046eba0*)&list_c)->last; p++) {
            if (p->id == packet->id) {
                SendPacketToPlayer(GetLocalHumanDpid(), target, p, 0xe);
                break;
            }
        }
        return;
    }
    if (packet->id > max) {
        max = packet->id;
    }
    if (packet->id == cur + 1) {
        cur = packet->id;
        ((UnitSync*)param_3)->HandleSyncPacket((Packet_0046d6c0*)packet, param_2);
        for (unsigned int i = cur + 1; i <= max; i++) {
            Packet_0046cef0* p;
            for (p = ((Class_0046eba0*)&list_d)->first; p != ((Class_0046eba0*)&list_d)->last; p++) {
                if (p->id == i) break;
            }
            if (p == ((Class_0046eba0*)&list_d)->last) break;
            cur = i;
            ((UnitSync*)param_3)->HandleSyncPacket((Packet_0046d6c0*)p, param_2);
        }
        return;
    }
    if (packet->id <= cur) {
        return;
    }
    // Through a reference, so the call sets up ecx before evaluating its
    // arguments, as the original does.
    Class_0046eba0& v = *(Class_0046eba0*)&list_d;
    v.InsertPacket(v.last, 1, packet);
    for (unsigned int i = cur + 1; i <= max; i++) {
        Packet_0046cef0* p;
        for (p = ((Class_0046eba0*)&list_d)->first; p != ((Class_0046eba0*)&list_d)->last; p++) {
            if (p->id == i) break;
        }
        if (p == ((Class_0046eba0*)&list_d)->last) {
            Packet_0046cef0 msg;
            msg.type = 0x1a;
            msg.arg = 0x65;
            msg.id = i;
            SendPacketToPlayer(GetLocalHumanDpid(), target, &msg, 0xe);
        }
    }
}

// FUNCTION: 0x470560 ??4PacketSequencer@@QAEAAU0@ABU0@@Z
PacketSequencer& PacketSequencer::operator=(const PacketSequencer& rhs)
{
    field_0 = rhs.field_0;
    cur = rhs.cur;
    max = rhs.max;
    list_c = rhs.list_c;
    list_d = rhs.list_d;
    return *this;
}

// The constructor of the 0x68-byte object held at g_game+0x2a30: the map
// header at +0x00 (Class_0046f720), the player vector, the id list and the
// three sub-objects. The map at +0x00 is the std::map<unsigned int,
// UnitSyncEntry> whose tree header lives at 0x46f720.
// FUNCTION: 0x46d040
UnitSync::UnitSync(int param)
    : map(Cmp_0046d040(), Alloc_0046d040())
{
    sub.a = 0;
    sub.b = 0;
    sub.c = 0;
    sub2.end = 0;
    sub2.flag = param;
    sub2.first = 0;
    sub2.last = 0;
    {
        UnitSyncEntry v;
        for (unsigned short i = 1; i < g_game->count; i++) {
            unsigned int key = g_game->defs[i].key;
            v.x = key;
            v.y = 0;
            v.w = 1;
            v.h = (short)sub2.flag;
            v.unknown_c = FlagOf_0046d040(&g_game->defs[i]) ? 0 : -1;
            map[key] = v;
        }
    }
}

// --- the sync packets the object sends ---------------------------------------

// Sends a sync packet to the given player in direct mode, else to the host.
// The original calls this from 0x46d860 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46d4c0
void UnitSync::SendSyncPacket(unsigned int* param_1, Packet_0046d530* param_2, int unused)
{
    unsigned int val;
    if (direct != 0)
        val = *param_1;
    else
        val = GetHostDpid();

    param_2->field_2 = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), val, param_2, 14);
}
#pragma auto_inline(on)

// HandleSyncPacket reads fields of the same object (offsets +0x14, +0x58, +0x5c,
// +0x64) that this function's "this" belongs to; no `mov ecx` appears before
// the call, so ecx (this) flows through unchanged from ReceiveSyncPacket's own
// thiscall "this" into HandleSyncPacket's.
// FUNCTION: 0x46d500
void UnitSync::ReceiveSyncPacket(void* param_1, int param_2)
{
    unsigned char b = *((unsigned char*)param_1 + 1);
    *(int*)((char*)param_1 + 2) = 0;
    if (b < 0x64) {
        HandleSyncPacket((Packet_0046d6c0*)param_1, param_2);
    }
}

// Builds a 0xe-byte packet of type 0x1a and sends it, unless sending is
// disabled. Sibling of SendEntryTo (same object, same packet type).

// FUNCTION: 0x46d530
void UnitSync::SendSyncMessage(unsigned char arg, int a, int b, int unused)
{
    if (disabled == 0) {
        Packet_0046d530 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = a;
        packet.field_a = b;
        // Send takes a target and is called with none: direct mode reads the id through it.
        Send(0, &packet);
    }
}

// Builds a 0xe-byte packet of type 0x1a and sends it to the target, unless
// sending is disabled, then counts it on the target. Sibling of 0x46d530
// (same object and packet) and 0x46d630.

// FUNCTION: 0x46d5b0
void UnitSync::SendSyncMessageTo(Target_0046d530* target, unsigned char arg, int a, int b, int unused)
{
    if (disabled == 0) {
        Packet_0046d530 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = a;
        packet.field_a = b;
        if (direct != 0) {
            SendPacket(target->id, &packet);
        } else {
            SendPacket(GetHostDpid(), &packet);
        }
        target->sent++;
    }
}

// Sends an entry of the unit list, like SendSyncMessageTo.
// FUNCTION: 0x46d630
void UnitSync::SendEntryTo(Target_0046d530* target, unsigned char arg, Source_0046d630* src, int unused)
{
    if (disabled == 0) {
        Packet_0046d530 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = src->field_0;
        packet.entry_a = src->field_8;
        packet.entry_b = src->field_a;
        packet.entry_c = src->field_c;
        if (direct != 0) {
            SendPacket(target->id, &packet);
        } else {
            SendPacket(GetHostDpid(), &packet);
        }
        target->sent++;
    }
}

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

struct Entry_0046d6c0 {               // 0x5c bytes
    int id;                           // +0x0
    Vec_0046d6c0 ids;                 // +0x4
    Vec_0046d6c0 pairs;               // +0x14
    int field_24;                     // +0x24
    char unknown_28[0x2c - 0x28];
    unsigned int field_2c;            // +0x2c
    char unknown_30[0x5c - 0x30];
};

// The tree behind a std::map<unsigned int, UnitSyncEntry> (MSVC 5's <xtree>
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

typedef std::pair<const unsigned int, UnitSyncEntry> Value_0046d6c0;

// std::map<unsigned int, UnitSyncEntry>::operator[] from MSVC 5's <map>: insert
// a default value if the key is new, then hand back the mapped value. The
// default `UnitSyncEntry()` is an uninitialised temporary in this compiler,
// which is where the copies of uninitialised stack words in the original come
// from.
class Map_0046d6c0 : public _Tree<unsigned int, Value_0046d6c0, Kfn_0046d6c0,
                                   Less_0046d6c0, Alloc_0046d6c0> {
public:
    UnitSyncEntry& operator[](const unsigned int& k)
    {
        _Pairib p = insert(Value_0046d6c0(k, UnitSyncEntry()));
        return p.first->_Value.second;
    }
};

// Handles one player-list packet. With `direct` set, the packet updates the
// player's entry in the vector at +0x10 (arg 1 stores a word, arg 2 appends to
// the entry's two std::vector<int> members through the out-of-line
// vector::insert at 0x46e640 and then calls 0x46d970, arg 4 raises a
// maximum). Otherwise an arg 3 packet stores a rectangle in the
// std::map<unsigned int, UnitSyncEntry> at +0x0 and calls 0x46d860.
//
// The map store is `map[key] = r` with the tree header's operator[] (see
// Class_0046f720): it inserts a default-constructed value, then the locally
// built rectangle is copied over the mapped value.
// FUNCTION: 0x46d6c0
void UnitSync::HandleSyncPacket(Packet_0046d6c0* packet, unsigned char player)
{
    if (disabled != 0) {
        return;
    }
    pendingPlayerCount++;
    if (direct != 0) {
        std::vector<Entry_0046d6c0>::iterator i = ((std::vector<Entry_0046d6c0>*)&players)->begin();
        if (i != ((std::vector<Entry_0046d6c0>*)&players)->end()) {
            unsigned int id = g_game->players[player].id;
            for (; i != ((std::vector<Entry_0046d6c0>*)&players)->end(); i++) {
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
            ((UnitSync*)this)->CheckUnitAvailable(packet->field_6, packet->field_a.all);
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
            UnitSyncEntry r;
            r.x = packet->field_6;
            r.y = 0;
            r.w = packet->field_a.part.lo;
            r.h = packet->field_a.part.hi;
            r.unknown_c = packet->field_a.part.top;
            (*(Map_0046d6c0*)&map)[packet->field_6] = r;
            ((UnitSync*)this)->NotifyEntryChanged(packet->field_6);
        }
    }
}

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

// Sends the "units expected" (packet type 0x1a, sub-type 3) notice to every
// player whose sync record this object holds, then queues the player's id on
// the insertion-ordered list at +0x20 unless it is already there.
// FUNCTION: 0x46d860
void UnitSync::NotifyEntryChanged(unsigned int param_1)
{
    std::vector<Player_0046d860>& ps = *(std::vector<Player_0046d860>*)&players;
    if (disabled != 0) {
        return;
    }
    if (direct != 0) {
        Iter_0046e9b0 it = ((Class_0046e9b0*)this)->FindExact(param_1);
        for (std::vector<Player_0046d860>::iterator i = ps.begin(); i != ps.end(); ++i) {
            // v is taken before the disabled check: MSVC then keeps it.ptr in eax
            // across the loop. The check stays a positive block, not a continue.
            Event_0046d860* v = &((Node_0046d860*)it.ptr)->value;
            if (disabled == 0) {
                Packet_0046d860 packet;
                packet.type = 0x1a;
                packet.arg = 3;
                packet.field_6 = v->field_0;
                packet.field_a = v->field_8;
                packet.field_b = v->field_a;
                packet.field_c = v->field_c;
                ((UnitSync*)this)->SendSyncPacket((unsigned int*)&*i, (Packet_0046d530*)&packet, 1);
                i->sent++;
            }
        }
    }
    for (std::list<unsigned int>::iterator it = ((std::list<unsigned int>*)&ids)->begin(); it != ((std::list<unsigned int>*)&ids)->end(); ++it) {
        if (*it == param_1) {
            return;
        }
    }
    ((std::list<unsigned int>*)&ids)->push_back(param_1);
}

// Given the unit's key and a y value, makes sure the entry has that y (looking
// the unit type up in g_game when it has none), checks that every player
// lists the key, and then sets the entry's height to whether all of that held
// before letting NotifyEntryChanged recompute the entry.

// FUNCTION: 0x46d970
void UnitSync::CheckUnitAvailable(unsigned int key, int y)
{
    if (disabled != 0)
        return;

    Iter_0046e9b0 it = ((Class_0046e9b0*)this)->FindExact(key);
    if (it == ((Class_0046e9b0*)this)->End())
        return;

    int h = 1;
    if (y != 0) {
        if (it.ptr->value.y == 0) {
            int n = g_game->count;
            for (int i = 1; i < n; i++) {
                Def_0046d040* def = &g_game->defs[i];
                if (def->key == key) {
                    ComputeUnitScriptChecksum(def);
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
        for (Entry_0046d970* e = (Entry_0046d970*)players.begin(); e != (Entry_0046d970*)players.end(); e++) {
            int flag;
            if (y != 0) {
                Player_0046e0b0* pl = FindPlayerByDpid(e->id);
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

// --- UnitSyncPlayer ----------------------------------------------------------

// The sync status line: the first player whose units or packets are not
// all accounted for, or "OK".
// FUNCTION: 0x46df40
char* UnitSync::GetSyncStatusText()
{
    if (direct == 0) {
        return 0;
    }
    for (std::vector<PlayerSync_0046e0b0>::iterator it = players.begin(); it != players.end(); ++it) {
        if (it->expected == 0) {
            return "No units_expected sent from player";
        }
        if (it->units.size() != it->expected) {
            sprintf(g_unitSyncStatusText, "expected %d units, got %d", it->expected, it->units.size());
            return g_unitSyncStatusText;
        }
        if (it->sent != it->ackd) {
            sprintf(g_unitSyncStatusText, "packets sent=%d  ackd=%d", it->sent, it->ackd);
            return g_unitSyncStatusText;
        }
    }
    return "OK";
}

// Checks every 0x5c-byte entry of the player vector at +0x10. An entry passes when its
// owner is still a live player of a type that needs no bookkeeping (type 2, or
// type 3 whose team data->field_94 is 2) and, otherwise, when its cached count
// is not zero, matches the size of its id vector, and its two counters agree.
// Returns 1 when nothing needs checking (disabled set, direct clear, no
// entries) or when every entry passes, 0 on the first entry that does not.

// FUNCTION: 0x46e000
int UnitSync::AllPlayersSynced()
{
    if (disabled != 0)
        return 1;
    if (direct == 0)
        return 1;
    Entry_0046e000* p = (Entry_0046e000*)players.begin();
    if (p == (Entry_0046e000*)players.end())
        return 1;
    // The second half is walked through its own pointer stepping with the entry stride.
    Sub_0046e000* s = (Sub_0046e000*)((char*)p + 8);
    for (; p != (Entry_0046e000*)players.end(); p++, s++) {
        Player_0046e0b0* pl = FindPlayerByDpid(p->id);
        if (pl != 0) {
            // pl->field_0 is tested again in the second test: the original
            // reloads it rather than reusing the first test's result.
            if (pl->field_0 != 0 && pl->type == 3 && pl->data->field_94 == 2)
                continue;
            if (pl->field_0 != 0 && pl->type == 2)
                continue;
            if (s->count == 0)
                return 0;
            // The count comes from the byte distance between the list's ends,
            // which keeps it a plain arithmetic shift.
            if ((s->ids.begin == 0 ? 0 : ((char*)s->ids.end - (char*)s->ids.begin) >> 2)
                != s->count)
                return 0;
            if (s->field_20 != s->field_24)
                return 0;
        }
    }
    return 1;
}

// Reports whether one player's copy of the shared unit list has caught up. The
// player whose id matches is looked up with FindPlayerByDpid, and the entry's
// std::vector of 0x5c-byte per-player records (the same records 0x46df40
// reports on) is scanned for the id.
// FUNCTION: 0x46e0b0
int UnitSync::IsPlayerSynced(int id)
{
    if (direct == 0)
        return 0;
    Player_0046e0b0* player;
    if (disabled != 0
        || (player = FindPlayerByDpid(id)) == 0
        || (player->field_0 != 0 && player->type == 3 && player->data->field_94 == 2)
        || (player->field_0 != 0 && player->type == 2))
        return 1;
    for (std::vector<PlayerSync_0046e0b0>::iterator it = players.begin(); it != players.end(); ++it) {
        if (it->id != id)
            continue;
        if (it->expected == 0 || it->units.size() != it->expected)
            return 0;
        // Written as return 1; break;: keeps the shared return-0 block as the fallthrough.
        if (it->sent == it->ackd)
            return 1;
        break;
    }
    return 0;
}

// Refreshes the map entry of every unit type: the 0x249-byte defs at
// g_game+0x1439b are looked up in the same std::map<unsigned int, Rect> that
// 0x46e330 uses (its find() is inlined here), the entry's bit 23 becomes "the
// rect has a non-empty size", and def+0x15a takes the rect's last field.
// FUNCTION: 0x46e160
void Class_0046e160::ApplyToUnitTypes()
{
    if (field_64 != 0)
        return;
    for (unsigned short i = 1; i < g_game->count; i++) {
        Def_0046d040* def = &g_game->defs[i];
        Iter_0046e330 it = Find(&def->key);
        if (it == End()) {
            ProtectUnitDefsReadWrite();
            def->field_15a = 0;
            def->flags_241.flag_23 = 0;
        } else {
            ProtectUnitDefsReadWrite();
            def->flags_241.flag_23 = (it.ptr->value.w != 0 && it.ptr->value.h != 0);
            def->field_15a = it.ptr->value.unknown_c;
        }
        ProtectUnitDefsReadOnly();
    }
}

struct Less_0046e280 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

struct Node_0046e280 {
    Node_0046e280* left;               // +0x0
    Node_0046e280* parent;             // +0x4
    Node_0046e280* right;              // +0x8
    unsigned int key;                  // +0xc
    Event_0046e280 value;              // +0x10
};

class Iter_0046e280 {
public:
    Node_0046e280* ptr;

    Iter_0046e280() {}
    Iter_0046e280(Node_0046e280* p) : ptr(p) {}
    bool operator==(const Iter_0046e280& other) const { return ptr == other.ptr; }
};

class Map_0046e280 {
public:
    Less_0046e280 compare;
    Node_0046e280* head;               // +0x4

    Iter_0046e280 End() { return Iter_0046e280(head); }
    Iter_0046e280 Find(const unsigned int* key)
    {
        Iter_0046e280 p = Iter_0046e280((Node_0046e280*)((Class_0046fe60*)this)->LowerBound(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
};

// Dequeues the front event of an insertion-ordered queue: the class holds a
// std::map<unsigned int, Event> (key at map node +0xc, Event at +0x10) for
// lookup and a std::list<Event> at +0x20 (_Head +0x24, _Size +0x28) for the
// order. The list front's first field is the map key. The inlined find() is
// copied from 0x46e330 (LowerBound is the tree's lower_bound(); a missing
// key yields the head node, end()).
// FUNCTION: 0x46e280
int UnitSync::PopChangedEntry(Event_0046e280* out)
{
    std::list<Event_0046e280>& q = *(std::list<Event_0046e280>*)&ids;
    if (q.empty())
        return 0;
    Iter_0046e280 p = ((Map_0046e280*)this)->Find(&q.front().key);
    *out = p.ptr->value;
    q.pop_front();
    return 1;
}

// Copies out the unit's entry and returns whether it has a non-empty size.
// FUNCTION: 0x46e330
int UnitSync::GetUnitEntry(Unit_0046e330* unit, UnitSyncEntry* out)
{
    *out = Find(&unit->key).ptr->value;
    return out->w != 0 && out->h != 0;
}

// Toggles the entry's width; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e3c0
int UnitSync::ToggleUnitAllowed(Unit_0046e330* unit)
{
    Node_0046e330* n = Find(&unit->key).ptr;
    n->value.w = (n->value.w == 0);
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Clears the entry's width; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e450
int UnitSync::DisallowUnit(Unit_0046e330* unit)
{
    Node_0046e330* n = Find(&unit->key).ptr;
    n->value.w = 0;
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Sets the entry's width to 1; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e4d0
int UnitSync::AllowUnit(Unit_0046e330* unit)
{
    Node_0046e330* n = Find(&unit->key).ptr;
    n->value.w = 1;
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Sets the last field of the unit's entry and has NotifyEntryChanged look at it.
// FUNCTION: 0x46e550
void UnitSync::SetUnitLimit(Unit_0046e330* unit, int value)
{
    Iter_0046e330 it = Find(&unit->key);
    if (!(it == End())) {
        it.ptr->value.unknown_c = value;
        NotifyEntryChanged(unit->key);
    }
}

struct Class_0046e5c0 {
    char field_0x0;
    char unknown_1[3];
    int field_0x4;
    int field_0x8;
    int field_0xc;

    Class_0046e5c0* InitTaggedVector(char* param_1);
};

// FUNCTION: 0x46e5c0
Class_0046e5c0* Class_0046e5c0::InitTaggedVector(char* param_1)
{
    field_0x0 = *param_1;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    return this;
}

// Out-of-line destructor of a class whose only member is a std::vector of a
// trivial type: the storage is freed and {_First,_Last,_End} zeroed.
// Same as 0x46e610, which is called on other locals of the same caller
// (0x46dxxx).
struct Elem_0046e5e0 {
    int value;                         // +0x0
};

class Class_0046e5e0 {
public:
    std::vector<Elem_0046e5e0> vec;

    ~Class_0046e5e0();
};

// FUNCTION: 0x46e5e0
Class_0046e5e0::~Class_0046e5e0()
{
}

// A std::vector<int> member's destructor: frees the storage and zeroes the
// {_First,_Last,_End} triple.
// The original calls this from 0x46c920 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46e610
Class_0046e610::~Class_0046e610()
{
}
#pragma auto_inline(on)

// std::vector<Elem_0046faf0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial.
// A 14-byte element type (its layout is a guess). Its callers (0x46cc10,
// 0x470390, 0x470560, 0x46ca60 and others) call 0x46fb40 (_Ufill), 0x46faf0
// (_Ucopy) and 0x46e870 (_Destroy) with ecx set to the vector.
typedef std::vector<Elem_0046faf0> Vec_0046e870;
typedef void (Vec_0046e870::*DestroyFn_0046e870)(Vec_0046e870::iterator, Vec_0046e870::iterator);

// _Destroy is protected: a derived class takes its address to emit it out of line.
struct Access_0046e870 : Vec_0046e870 {
    static DestroyFn_0046e870 fn;
};

// FUNCTION: 0x46e870 ?_Destroy@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEXPAUElem_0046faf0@@0@Z
DestroyFn_0046e870 Access_0046e870::fn = &Access_0046e870::_Destroy;

class Class_0046e880 {
public:
    char unknown_0[4];
    int* field_4;

    int* Begin(int* param_1);
};

// FUNCTION: 0x46e880
int* Class_0046e880::Begin(int* param_1)
{
    *param_1 = *field_4;
    return param_1;
}

// std::_Tree<...>::erase(iterator first, iterator last) from MSVC 5's <xtree>
// for the std::map<unsigned int, UnitSyncEntry> tree (_Nil is DAT_0051e598,
// _Nilrefs DAT_0051e59c), emitted out of line. Its three callers (0x46c920,
// 0x46ca60, 0x46d1a0) inline ~_Tree(): erase(begin(), end()), free the head,
// then drop _Nilrefs under a lock. The real template reproduces every call:
// _Erase() is inlined once and its recursion calls 0x46f6d0, erase(_F++)
// calls iterator::_Inc() (0x46ea10) and then erase(iterator) (0x46f1e0).
// The reference to erase(iterator) needs an entry in data/aliases.csv.
typedef std::map<unsigned int, UnitSyncEntry>::_Imp Tree_0046e890;
typedef Tree_0046e890::iterator (Tree_0046e890::*EraseFn_0046e890)(
    Tree_0046e890::iterator, Tree_0046e890::iterator);

// FUNCTION: 0x46e890 ?erase@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAE?AViterator@12@V312@0@Z
EraseFn_0046e890 g_erase_0046e890 = &Tree_0046e890::erase;

// An out-of-line std::map<unsigned int, ...>::find(): LowerBound is the
// tree's lower_bound(), and a missing key yields the head node (end()).
// The original calls this from 0x46d860 and 0x46d970 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46e9b0
Iter_0046e9b0 Class_0046e9b0::FindExact(const unsigned int& key)
{
    Iter_0046e9b0 p = Iter_0046e9b0((Node_0046e9b0*)((Class_0046fe60*)this)->LowerBound(&key));
    return (p == End() || compare(key, p.ptr->key)) ? End() : p;
}
#pragma auto_inline(on)

// --- the std::map<unsigned int, UnitSyncEntry> members -----------------------

// FUNCTION: 0x46ea10 ?_Inc@iterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAEXXZ
// FUNCTION: 0x46ef50 ?insert@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAE?AU?$pair@Viterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@_N@2@ABU?$pair@IUUnitSyncEntry@@@2@@Z
// FUNCTION: 0x46f1e0 ?erase@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAE?AViterator@12@V312@@Z
// FUNCTION: 0x46f6d0 ?_Erase@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEXPAU_Node@12@@Z
// FUNCTION: 0x46fb80 ?_Insert@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAE?AViterator@12@PAU_Node@12@0ABU?$pair@IUUnitSyncEntry@@@2@@Z
// FUNCTION: 0x46feb0 ?_Lrotate@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEXPAU_Node@12@@Z
// FUNCTION: 0x46ff10 ?_Rrotate@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEXPAU_Node@12@@Z
// FUNCTION: 0x46ff70 ?_Buynode@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEPAU_Node@12@PAU312@W4_Redbl@12@@Z
// FUNCTION: 0x46ff90 ?_Dec@iterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAEXXZ
// The tree's protected members come out of an explicit instantiation: the
// protected ones (the rotations) cannot be reached by a member pointer.
template class std::_Tree<unsigned int, std::pair<const unsigned int, UnitSyncEntry>, std::map<unsigned int, UnitSyncEntry>::_Kfn, std::less<unsigned int>, std::allocator<UnitSyncEntry> >;

// --- vector<Class_0046eaa0>::_Destroy (0x46eaa0) -----------------------------

void __stdcall NopChecksumEntryDtor(int);

namespace std {
template<> inline void allocator<Elem_0046faf0>::destroy(Elem_0046faf0* p)
{
    // Direct call: an inline std::_Destroy overload would be one inline
    // level too deep.
    NopChecksumEntryDtor((int)p);
}
}

struct Elem_004702a0 {
    int unknown_0;
};

struct Class_0046eaa0 {                // operator= is 0x470040
    int field_0;                       // +0x00
    std::vector<Elem_004702a0> list_a; // +0x04
    std::vector<Elem_004702a0> list_b; // +0x14
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    PacketSequencer sub;               // +0x30
};

typedef std::vector<Class_0046eaa0> Vec_0046eaa0;
typedef void (Vec_0046eaa0::*DestroyFn_0046eaa0)(Vec_0046eaa0::iterator, Vec_0046eaa0::iterator);

struct Access_0046eaa0 : Vec_0046eaa0 {
    static DestroyFn_0046eaa0 fn;
};

// std::vector<Class_0046eaa0>::_Destroy(first, last) from MSVC 5's <vector>,
// called from the inlined erase at 0x46db82 (0x46dad0) with ecx set to the
// vector. It runs each 0x5c-byte element's implicit destructor, which frees
// the element's four std::vector members last-first.
// FUNCTION: 0x46eaa0 ?_Destroy@?$vector@UClass_0046eaa0@@V?$allocator@UClass_0046eaa0@@@std@@@std@@IAEXPAUClass_0046eaa0@@0@Z
DestroyFn_0046eaa0 Access_0046eaa0::fn = &Access_0046eaa0::_Destroy;

// --- std::list<int> members (0x46eb60, 0x46fac0) -----------------------------

typedef std::list<int> List_0046eb60;
typedef List_0046eb60::iterator (List_0046eb60::*EraseFn_0046eb60)(List_0046eb60::iterator);

// std::list<int>::erase(iterator), out of line: its callers (0x46c920,
// 0x46ca60, 0x46d040's neighbours) call it as `erase(_F++)` with the
// iterator post-increment at 0x46fac0, on the list at +0x20 of
// UnitSync.
// FUNCTION: 0x46eb60 ?erase@?$list@HV?$allocator@H@std@@@std@@QAE?AViterator@12@V312@@Z
EraseFn_0046eb60 g_erase_0046eb60 = &List_0046eb60::erase;

// The tree's _Init: DAT_0051e598 is the shared _Nil node and DAT_0051e59c its
// reference count (_Nilrefs).
// FUNCTION: 0x46f720
void Class_0046f720::Init()
{
    std::_Lockit lock;
    if (DAT_0051e598 == 0) {
        DAT_0051e598 = Buynode(0, 1);
        DAT_0051e598->left = 0, DAT_0051e598->right = 0;
    }
    ++DAT_0051e59c;
    head = Buynode(DAT_0051e598, 0), size = 0;
    head->left = head, head->right = head;
}

// --- the iterator's post-increment (0x46fac0) --------------------------------

typedef std::list<int> List_0046fac0;
typedef List_0046fac0::iterator (List_0046fac0::iterator::*PostIncFn_0046fac0)(int);

// std::list<int>::iterator::operator++(int), out of line: return the old
// position and step to the next node. Its callers (e.g. 0x46c920) use it in
// the inlined list::erase(first, last) loop, `erase(_F++)`, on the list at
// +0x20 of the object 0x46d040 builds. The node is 0xc bytes (the constructor
// allocates the head with `new(0xc)`), so the element is a 4-byte value
// compared against an id (0x46d860); int is a guess.
// FUNCTION: 0x46fac0 ??Eiterator@?$list@HV?$allocator@H@std@@@std@@QAE?AV012@H@Z
PostIncFn_0046fac0 g_postinc_0046fac0 = &List_0046fac0::iterator::operator++;

// --- std::pair<map::iterator, bool> (0x46fad0) -------------------------------

// Probably std::pair<map::iterator, bool>::pair(const iterator&, const bool&),
// out of line: its one caller (0x46d2e0) is an inlined map::insert that
// builds the result from the node found (or inserted) and whether it is new.
struct Node_0046fad0;

class Class_0046fad0 {
public:
    Node_0046fad0* first;              // +0x0 (the iterator's node)
    bool second;                       // +0x4

    Class_0046fad0(Node_0046fad0* const& f, const bool& s);
};

// FUNCTION: 0x46fad0
Class_0046fad0::Class_0046fad0(Node_0046fad0* const& f, const bool& s)
    : first(f), second(s)
{
}

// --- vector<Elem_0046faf0> members (0x46faf0, 0x46fb40, 0x470770, 0x4707a0) --

typedef std::vector<Elem_0046faf0> Vec_0046faf0;
typedef Vec_0046faf0::iterator (Vec_0046faf0::*UcopyFn_0046faf0)(
    Vec_0046faf0::const_iterator, Vec_0046faf0::const_iterator, Vec_0046faf0::iterator);

struct Access_0046faf0 : Vec_0046faf0 {
    static UcopyFn_0046faf0 fn;
};

// std::vector<Elem_0046faf0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies.
// A 14-byte element type (its layout is a guess). Its callers (0x46cc10,
// 0x470390, 0x470560, 0x46ca60 and others) call 0x46fb40 (_Ufill), 0x46faf0
// (_Ucopy) and 0x46e870 (_Destroy) with ecx set to the vector.
// FUNCTION: 0x46faf0 ?_Ucopy@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEPAUElem_0046faf0@@PBU3@0PAU3@@Z
UcopyFn_0046faf0 Access_0046faf0::fn = &Access_0046faf0::_Ucopy;

typedef void (Vec_0046faf0::*UfillFn_0046faf0)(
    Vec_0046faf0::iterator, Vec_0046faf0::size_type, const Elem_0046faf0&);

struct Access_0046fb40 : Vec_0046faf0 {
    static UfillFn_0046faf0 fn;
};

// std::vector<Elem_0046faf0>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first.
// FUNCTION: 0x46fb40 ?_Ufill@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEXPAUElem_0046faf0@@IABU3@@Z
UfillFn_0046faf0 Access_0046fb40::fn = &Access_0046fb40::_Ufill;

// --- the map's lower_bound (0x46fe60) ----------------------------------------

// The original calls this from 0x46e9b0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46fe60
Node_0046fe60* Class_0046fe60::LowerBound(const unsigned int* key)
{
    std::_Lockit lock;
    Node_0046fe60* x = head->parent;
    Node_0046fe60* y = head;
    while (x != (Node_0046fe60*)DAT_0051e598)
        if (key_compare(x->key, *key))
            x = x->right;
        else
            y = x, x = x->left;
    return y;
}
#pragma auto_inline(on)

// --- vector<Elem_004702a0> members (0x470290, 0x4702a0) ----------------------

typedef std::vector<Elem_004702a0> Vec_00470290;
typedef void (Vec_00470290::*DestroyFn_00470290)(Vec_00470290::iterator, Vec_00470290::iterator);

struct Access_00470290 : Vec_00470290 {
    static DestroyFn_00470290 fn;
};

// std::vector<Elem_004702a0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. Its caller 0x470040 calls
// 0x4702a0 (_Ucopy) and 0x470290 (_Destroy) with ecx set to the vector.
// FUNCTION: 0x470290 ?_Destroy@?$vector@UElem_004702a0@@V?$allocator@UElem_004702a0@@@std@@@std@@IAEXPAUElem_004702a0@@0@Z
DestroyFn_00470290 Access_00470290::fn = &Access_00470290::_Destroy;

typedef std::vector<Elem_004702a0> Vec_004702a0;
typedef Vec_004702a0::iterator (Vec_004702a0::*UcopyFn_004702a0)(
    Vec_004702a0::const_iterator, Vec_004702a0::const_iterator, Vec_004702a0::iterator);

struct Access_004702a0 : Vec_004702a0 {
    static UcopyFn_004702a0 fn;
};

// std::vector<Elem_004702a0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies. Its caller 0x470040 calls 0x4702a0 (_Ucopy) and
// 0x470290 (_Destroy) with ecx set to the vector.
// FUNCTION: 0x4702a0 ?_Ucopy@?$vector@UElem_004702a0@@V?$allocator@UElem_004702a0@@@std@@@std@@IAEPAUElem_004702a0@@PBU3@0PAU3@@Z
UcopyFn_004702a0 Access_004702a0::fn = &Access_004702a0::_Ucopy;

// --- the empty function 0x470030 ---------------------------------------------

// FUNCTION: 0x470030
void __stdcall NopChecksumEntryDtor(int)
{
}

// --- Class_00470250's and Class_00470270's counts (0x470250, 0x470270) --------

class Class_00470250 {
public:
    char unknown_0[4];
    int first;
    char unknown_8[4];
    int last;

    int Capacity();
};

// FUNCTION: 0x470250
int Class_00470250::Capacity()
{
    if (!first) {
        return 0;
    }
    return (last - first) >> 2;
}

class Class_00470270 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int Size();
};

// FUNCTION: 0x470270
int Class_00470270::Size() {
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 2;
}

// --- std::copy for 4-byte elements (0x4702d0) --------------------------------

// std::copy for 4-byte elements, compiled with __stdcall as the default
// convention (same shape as 0x44eef0). Its one caller copies one vector's
// elements into another.
// FUNCTION: 0x4702d0
int* __stdcall CopyDwordRangeOverwrite(int* first, int* last, int* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}

// --- vector<Elem_0046faf0>::size and operator= (0x470770, 0x4707a0) ----------

typedef Vec_0046faf0::size_type (Vec_0046faf0::*SizeFn_00470770)() const;

// std::vector<Elem_0046faf0>::size() for the 14-byte element (three ints and
// a short, packed to 2 bytes), the same vector as _Ucopy (0x46faf0) and the
// operator= at 0x4707a0. Its callers are in 0x470560.
// FUNCTION: 0x470770 ?size@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@QBEIXZ
SizeFn_00470770 g_size_00470770 = &Vec_0046faf0::size;

typedef Vec_0046faf0& (Vec_0046faf0::*AssignFn_0046faf0)(const Vec_0046faf0&);

// std::vector<Elem_0046faf0>::operator= from MSVC 5's <vector>. The element
// type is 14 bytes (three ints and a short, packed to 2), the same vector as
// _Ucopy (0x46faf0), _Destroy (0x46e870), _Ufill (0x46fb40) and size (0x470770).
// Its one caller is PacketSequencer::operator= (0x470560).
// FUNCTION: 0x4707a0 ??4?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@QAEAAV01@ABV01@@Z
AssignFn_0046faf0 g_assign_0046faf0 = &Vec_0046faf0::operator=;
