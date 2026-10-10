// Decompiled by Opus, Haiku, Sonnet, space-bunny-free, Space Bunny Free,
// deepseek-v4.1, deepseek-v4.1-flash, GPT-5.6-Terra, GPT-6.1-sol, GPT-6,
// DeepSeek V4.1 Flash, Claude Opus 5.5, Sonnet 5.5 and claude-sonnet-5-5.
// Names are provisional.
// The unit synchronisation module: the game-event reporting the statistics DLL
// calls, PacketSequencer (the sequenced 0xe-byte command packets), UnitSync
// (the shared unit list every player is checked against) and the out-of-line
// std::map<unsigned int, UnitSyncEntry>, std::vector and std::list members
// with the hand-written _Tree helpers, from 0x46c620 to 0x4707a0. Files that
// stay apart, and the constraint that keeps each there:
// - unit_sync_46ca60.cpp: FinishUnitSync calls the out-of-line _Destroy of the
//   Elem_0046faf0 vector (0x46e870), where the real <vector> inlines its empty
//   body; it needs a hand-written std::vector.
// - unit_sync_46cc10.cpp: SendSequenced inlines vector::insert but calls the
//   out-of-line _Destroy, _Ucopy and _Ufill, which the real <vector> inlines
//   too (832 bytes against 678); it needs a hand-written std::vector.
// - unit_sync_46d1a0.cpp: ~UnitSync needs real std::map, std::list and
//   std::vector members, where UnitSync here holds the hand-written
//   UnitSyncMap and element views.
// - unit_sync_46dad0.cpp: ProcessSync needs UnitSync's player list as a
//   vector of SyncPlayerRecord and the real std::map, which UnitSync here
//   spells as the hand-written views.
// - unit_sync_46e640.cpp, unit_sync_46eba0.cpp and unit_sync_46f7a0.cpp: the
//   out-of-line vector::insert instances are built with /Gi, which this file
//   cannot carry (unit_sync_46f7a0.cpp also needs a hand-written std::vector).
// - unit_sync_player.cpp: the ??_GUnitSyncPlayer COMDAT is emitted with
//   ~UnitSyncPlayer inlined, while 0x46c920 calls that destructor out of line.
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
    int created;                        // +0x4c9
    char unknown_4cd[4];
};

#include "../map/mission.h"

#pragma pack(push, 1)
struct PlayerEntry_0046d6c0 {           // 0x14b bytes, one slot of g_game's players
    int id;                             // +0x0, g_game + 0x1b67
    char unknown_4[0x14b - 4];
};

struct UnitDef {                        // 0x249 bytes, one unit type
    char unknown_0[0x13e];
    unsigned int key;                   // +0x13e
    int y;                              // +0x142
    char unknown_146[0x15a - 0x146];
    int limit;                          // +0x15a
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
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x1438f - 0x2a42 - sizeof(unsigned char)];
    int unitDefCount;                   // +0x1438f
    char unknown_14393[0x1439b - 0x1438f - sizeof(int)];
    UnitDef* unitDefs;                  // +0x1439b
    char unknown_1439f[0x391e9 - 0x1439b - sizeof(UnitDef*)];
    Mission* mapInfo;                   // +0x391e9
    char unknown_391ed[0x39201 - 0x391e9 - sizeof(Mission*)];
    char providerGuid[1];               // +0x39201, the selected service provider's GUID
};
#pragma pack(pop)

extern Game* g_game;

typedef int (__stdcall *SendFn_0046c620)(int, Rect_0046c620*, void*, int, Name_0046c620*,
                                         int, int, int, void*, void*);

extern int g_onlineReportScores;
extern Name_0046c620 g_reportPlayerName;
extern int* g_onlineReportPlayers;
extern void* g_onlineReportScoreBoards;
extern SendFn_0046c620 g_riReport;
extern int g_reporterDll;
extern int g_hapinetOnlineReport;
extern int g_reportFlags;
extern void (*g_riIntervalTimer)(void);
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
    if (!g_hapinetOnlineReport && !g_reporterDll)
        return 4;
    if (!g_onlineReportPlayers || !g_onlineReportScoreBoards || !g_onlineReportScores)
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

    if (g_hapinetOnlineReport) {
        if (msg == 1 || msg == 6 || msg == 7)
            g_reportFlags = QueryOnlineFlags(msg == 1 ? 1 : 2 + (msg != 6));
        if (g_reportFlags & 3) {
            // its own statement, not an argument: the call has to be emitted
            // ahead of the other nine arguments being set up
            int team = (int)g_game->mapInfo->GetMissionName();
            if (RIReport(msg, &rect, (char*)&g_game->providerGuid, thing, &g_reportPlayerName,
                             team, g_game->localPlayer,
                             id, g_onlineReportPlayers, g_onlineReportScoreBoards))
                g_hapinetOnlineReport = 0;
        }
    }

    if (g_reporterDll) {
        int team = (int)g_game->mapInfo->GetMissionName();
        g_riReport(msg, &rect, (char*)&g_game->providerGuid, thing, &g_reportPlayerName,
                     team, g_game->localPlayer,
                     id, g_onlineReportPlayers, g_onlineReportScoreBoards);
    }

    return 0;
}

// FUNCTION: 0x46c810
int __stdcall ReportGameChat(int msg)
{
    if ((g_hapinetOnlineReport != 0 && (g_reportFlags & 8)) || g_reporterDll != 0) {
        FillScoreTables();
        if (g_hapinetOnlineReport != 0 && (g_reportFlags & 8)) {
            if (RIReportGameChat(g_onlineReportPlayers[g_game->localPlayer], msg))
                g_hapinetOnlineReport = 0;
        }
        if (g_reporterDll != 0)
            return g_riReportGameChat(g_onlineReportPlayers[g_game->localPlayer], msg);
        return 0;
    }
    return 4;
}

// FUNCTION: 0x46c8b0
void ReportIntervalTimer(void)
{
    if (g_reporterDll) {
        g_riIntervalTimer();
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
    int limit;                         // +0xc
};

// A node of the std::map<unsigned int, UnitSyncEntry> tree (MSVC 5 xtree's
// _Node): the links, the pair (key at +0xc, entry at +0x10) and the colour.
struct UnitSyncNode {
    UnitSyncNode* left;                // +0x0
    UnitSyncNode* parent;              // +0x4
    UnitSyncNode* right;               // +0x8
    unsigned int key;                  // +0xc
    UnitSyncEntry value;               // +0x10
    int color;                         // +0x20 (0 red, 1 black)
};

class UnitSyncIter {
public:
    UnitSyncNode* ptr;
    UnitSyncIter() {}
    UnitSyncIter(UnitSyncNode* p) : ptr(p) {}
    bool operator==(const UnitSyncIter& other) const { return ptr == other.ptr; }
};

#pragma pack(push, 1)
struct Unit_0046e330 {
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
};

struct Data_0046e0b0 {
    char unknown_0[0x94];
    unsigned char kind;                  // +0x94
    char unknown_95[0xa7 - 0x95];
    unsigned char versionMajor;          // +0xa7
    unsigned char versionMinor;          // +0xa8
};

struct Player_0046e0b0 {                // 0x14b bytes
    int active;                         // +0x0
    char unknown_4[0x27 - 0x4];
    Data_0046e0b0* data;                 // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                  // +0x73
    char unknown_74[0x14b - 0x74];
};

#pragma pack(pop)

#pragma pack(push, 1)
// The 0xe-byte command packet of type 0x1a that UnitSync sends and
// PacketSequencer queues: a sub-type, a sequence number (0 when it is sent
// without one), the key of the unit it is about, and one payload word or the
// unit's permission bytes.
struct UnitSyncPacket {                // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    unsigned int id;                   // +0x2
    int key;                           // +0x6
    union {
        int value;                     // +0xa
        struct {
            unsigned char enabled;     // +0xa
            unsigned char match;       // +0xb
            short limit;               // +0xc
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
    int key;                           // +0x0
    char unknown_4[4];
    unsigned char enabled;             // +0x8
    char unknown_9;
    unsigned char match;               // +0xa
    char unknown_b;
    short limit;                       // +0xc
};

#pragma pack(pop)

struct Cmp_0046d040 {                  // the map's empty key_compare
    char x;
};

struct Alloc_0046d040 {                // the map's empty allocator
    char x;
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
    unsigned int size() { return first == 0 ? 0 : last - first; }
    void insert(int* pos, int n, int const& val);
};

// One player's record in the vector at UnitSync+0x10: the ids of the units the
// player listed, their pairs (parallel to ids), and the packet counters.
struct PlayerSync_0046e0b0 {            // 0x5c bytes
    unsigned int id;                     // +0x0
    Vec_0046d6c0 ids;                    // +0x4
    Vec_0046d6c0 pairs;                  // +0x14
    int expected;                        // +0x24
    int sent;                            // +0x28
    unsigned int ackd;                   // +0x2c
    char unknown_30[0x5c - 0x30];
};

// The tree behind a std::map<unsigned int, UnitSyncEntry> (MSVC 5's <xtree>
// layout: empty allocator and key_compare at +0 and +1, _Head, _Multi, _Size).
// It is declared by hand, as a template with <xtree>'s own name and first two
// arguments, because the real tree's insert is 0x46ef50 and <xtree>'s inline
// insert would otherwise be expanded here (the real map gives 700 bytes).
struct Kfn_0046d6c0 {};
struct Alloc_0046d6c0 {};

struct Less_0046d6c0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

template<class _K, class _Ty, class _Kfn, class _Pr, class _A>
class _Tree {
public:
    typedef UnitSyncNode _Node;
    struct _Pairib {
        _Node* first;                 // the iterator
        bool second;
        _Pairib() {}
    };

    char _Alloc;                      // +0x0
    union {
        char _Pred;                   // +0x1
        _Pr key_compare;
    };
    _Node* _Head;                     // +0x4
    bool _Multi;                      // +0x8
    int _Size;                        // +0xc

    _Pairib insert(const _Ty& _V);    // 0x46ef50
};

typedef std::pair<const unsigned int, UnitSyncEntry> Value_0046d6c0;

// std::_Tree<...>::_Init() from MSVC 5's <xtree>, written out by hand:
// DAT_0051e598 is the tree's shared _Nil node and DAT_0051e59c its
// reference count (_Nilrefs). Nodes are 0x24 bytes (a 0x14-byte value).
extern UnitSyncNode* DAT_0051e598;
extern int DAT_0051e59c;

// The tree header of UnitSync::map, the std::map<unsigned int, UnitSyncEntry>
// at +0x00, with the members the file calls out of line: _Init (0x46f720),
// begin (0x46e880), find (0x46e9b0), insert (0x46ef50) and lower_bound
// (0x46fe60).
class UnitSyncMap : public _Tree<unsigned int, Value_0046d6c0, Kfn_0046d6c0,
                                  Less_0046d6c0, Alloc_0046d6c0> {
public:
    // Empty classes taken by value: the prologue copies them from the param slot.
    UnitSyncMap(Cmp_0046d040 c, Alloc_0046d040 a)
    {
        _Alloc = c.x;
        _Pred = a.x;
        _Multi = 0;
        Init();
    }

    UnitSyncIter End() { return UnitSyncIter(_Head); }
    UnitSyncIter FindExact(const unsigned int& key);
    UnitSyncNode* LowerBound(const unsigned int* key);
    int* Begin(int* param_1);

    UnitSyncNode* Buynode(UnitSyncNode* parent, int color)
    {
        UnitSyncNode* s = (UnitSyncNode*)operator new(sizeof(UnitSyncNode));
        s->parent = parent;
        s->color = color;
        return s;
    }

    void Init();

    // std::map<unsigned int, UnitSyncEntry>::operator[] from MSVC 5's <map>:
    // insert a default value if the key is new, then hand back the mapped
    // value. The default `UnitSyncEntry()` is an uninitialised temporary in
    // this compiler, which is where the copies of uninitialised stack words in
    // the original come from.
    UnitSyncEntry& operator[](const unsigned int& k)
    {
        _Pairib p = insert(Value_0046d6c0(k, UnitSyncEntry()));
        return p.first->value;
    }
};

class Sub_0046d040 {                   // the object at +0x2c
public:
    int seqSent;                       // +0x0
    int seqCur;                        // +0x4
    int seqMax;                        // +0x8

    void SendUnsequenced(void* packet, int to);
};

class Sub2_0046d040 {                  // the object at +0x58
public:
    int direct;                        // +0x0
    int* pendingPlayerCount;           // +0x4
    int* checksumProgress;             // +0x8
    int* disabled;                     // +0xc
};

// UnitSyncPlayer (0x5c bytes): one player's unit-sync state, with four
// std::vector members.
#pragma pack(push, 2)
struct Elem_0046faf0 {                 // 14 bytes, a queued sync packet
    union {
        struct {
            int a;                     // +0x0
            int b;                     // +0x4
            int c;                     // +0x8
            short d;                   // +0xc
        };
        UnitSyncPacket packet;
    };
};
#pragma pack(pop)

class UnitSyncPlayer {                 // the vector's element, 0x5c bytes
public:
    char unknown_0[0x5c];

    ~UnitSyncPlayer();
};

class SyncChecksumVector {
public:
    std::vector<int> vec;
    ~SyncChecksumVector();
};

int __cdecl GetLocalHumanDpid();
unsigned int GetHostDpid();
void __stdcall SendPacketToPlayer(int a, unsigned int b, void* c, int d);
Player_0046e0b0* __stdcall FindPlayerByDpid(int id);
int __stdcall ComputeUnitScriptChecksum(UnitDef* def);
void ProtectUnitDefsReadWrite();
void ProtectUnitDefsReadOnly();
extern char g_unitSyncStatusText[];

// Inlined copy of PacketSequencer::SendUnsequenced (a method that ignores this).
static inline void SendPacket(unsigned int to, void* packet)
{
    *(int*)((char*)packet + 2) = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), to, packet, 0xe);
}

class UnitSync {
public:
    UnitSyncMap map;                           // +0x00
    std::vector<PlayerSync_0046e0b0> players;  // +0x10
    std::list<int> ids;                        // +0x20
    Sub_0046d040 sub;                          // +0x2c
    std::vector<int> seqSentQueue;             // +0x38
    std::vector<int> seqHeldQueue;             // +0x48
    union {
        Sub2_0046d040 sub2;                    // +0x58
        struct {
            int direct;                        // +0x58
            union {
                int* first;                    // +0x5c
                int pendingPlayerCount;
            };
            int* checksumProgress;             // +0x60
            int disabled;                      // +0x64
        };
    };

    UnitSync(int param);

    UnitSyncIter End() { return UnitSyncIter(map._Head); }
    UnitSyncIter Find(const unsigned int* key)
    {
        UnitSyncIter p = UnitSyncIter(map.LowerBound(key));
        return (p == End() || map.key_compare(*key, p.ptr->key)) ? End() : p;
    }
    void Send(Target_0046d530* target, void* packet)
    {
        if (direct != 0) {
            SendPacket(target->id, packet);
        } else {
            SendPacket(GetHostDpid(), packet);
        }
    }

    void SendSyncPacket(unsigned int* param_1, UnitSyncPacket* param_2, int unused);
    void SendSyncMessage(unsigned char arg, int a, int b, int unused);
    void SendSyncMessageTo(Target_0046d530* target, unsigned char arg, int a, int b, int unused);
    void SendEntryTo(Target_0046d530* target, unsigned char arg, Source_0046d630* src, int unused);
    void HandleSyncPacket(UnitSyncPacket* packet, unsigned char player);
    void ReceiveSyncPacket(void* param_1, int param_2);
    void NotifyEntryChanged(unsigned int param_1);
    void CheckUnitAvailable(unsigned int key, int y);
    char* GetSyncStatusText();
    int AllPlayersSynced();
    int IsPlayerSynced(int id);
    int PopChangedEntry(UnitSyncEntry* out);
    int GetUnitEntry(Unit_0046e330* unit, UnitSyncEntry* out);
    int ToggleUnitAllowed(Unit_0046e330* unit);
    int DisallowUnit(Unit_0046e330* unit);
    int AllowUnit(Unit_0046e330* unit);
    void SetUnitLimit(Unit_0046e330* unit, int value);
    void ApplyToUnitTypes();
    void ResetEntries();
};

static inline UnitDef* Defs_0046d040()
{
    return g_game->unitDefs;
}

// The flag expression in its own small inline helper.
static inline bool FlagOf_0046d040(UnitDef* d)
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
    Sub_0046d040 sub;                              // +0x2c
    SyncChecksumVector seqSentQueue;               // +0x38
    std::vector<int> seqHeldQueue;                 // +0x48
    int direct;                                    // +0x58
    int pendingPlayerCount;                        // +0x5c
    int checksumProgress;                          // +0x60
    int disabled;                                  // +0x64

    ~UnitSyncDel_0046c920() {}
};

// --- the unit-sync object's lifecycle ----------------------------------------

// FUNCTION: 0x46c8e0
void __stdcall CreateUnitSync(int param_1)
{
    g_game->sync = new UnitSync(param_1);
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

// A list seen as a std::vector of packets (allocator byte, _First, _Last,
// _End): ReceiveSequenced reads the lists through it, and InsertPacket is that
// vector's insert(end(), count, val), out of line.
class Class_0046eba0 {
public:
    char unknown_0[4];
    UnitSyncPacket* first;       // +0x4
    UnitSyncPacket* last;        // +0x8
    UnitSyncPacket* end;         // +0xc
    void InsertPacket(UnitSyncPacket* where, int count, UnitSyncPacket* val);
};

struct PacketSequencer {
    int lastSent;                      // +0x00
    unsigned int cur;                  // +0x04
    unsigned int max;                  // +0x08
    // Nested struct with its own out-of-line operator=: sets the inline depth
    // that keeps sentQueue's _Destroy out of line and heldQueue's inlined.
    std::vector<Elem_0046faf0> sentQueue; // +0x0c (16 bytes, _First at +0x10)
    std::vector<Elem_0046faf0> heldQueue; // +0x1c (operator= is 0x4707a0)
    PacketSequencer();
    PacketSequencer& operator=(const PacketSequencer& rhs);
    // A method that ignores `this`: see its definition.
    void SendUnsequenced(unsigned int param_1, void* param_2);
    // In unit_sync_46cc10.cpp: it needs a hand-written std::vector, which
    // cannot share a file with <vector>.
    void SendSequenced(Elem_0046faf0* param_1, Elem_0046faf0* param_2);
    void ReceiveSequenced(UnitSyncPacket* packet, int param_2, void* param_3, unsigned int target);
};

// The constructor: two empty vectors (the allocator bytes are copied from an
// uninitialised temporary) and three dwords zeroed in the body.
// FUNCTION: 0x46cbe0
PacketSequencer::PacketSequencer()
{
    lastSent = 0;
    cur = 0;
    max = 0;
}

// A method that ignores `this`: its callers (0x46dad0) set ecx to the
// PacketSequencer at +0x2c of UnitSync before each call. Clears the packet's
// +2 field and sends the 0xe-byte packet. 0x46d530, 0x46d5b0 and 0x46d630
// inline a copy of it.
// FUNCTION: 0x46cec0
void PacketSequencer::SendUnsequenced(unsigned int param_1, void* param_2)
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
void PacketSequencer::ReceiveSequenced(UnitSyncPacket* packet, int param_2, void* param_3, unsigned int target)
{
    if (packet->arg == 0x65) {
        for (Elem_0046faf0* p = sentQueue.begin(); p != sentQueue.end(); p++) {
            if (p->packet.id == packet->id) {
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
        ((UnitSync*)param_3)->HandleSyncPacket(packet, param_2);
        for (unsigned int i = cur + 1; i <= max; i++) {
            Elem_0046faf0* p;
            for (p = heldQueue.begin(); p != heldQueue.end(); p++) {
                if (p->packet.id == i) break;
            }
            if (p == heldQueue.end()) break;
            cur = i;
            ((UnitSync*)param_3)->HandleSyncPacket(&p->packet, param_2);
        }
        return;
    }
    if (packet->id <= cur) {
        return;
    }
    // Through a reference, so the call sets up ecx before evaluating its
    // arguments, as the original does.
    Class_0046eba0& v = *(Class_0046eba0*)&heldQueue;
    v.InsertPacket(v.last, 1, packet);
    for (unsigned int i = cur + 1; i <= max; i++) {
        Elem_0046faf0* p;
        for (p = heldQueue.begin(); p != heldQueue.end(); p++) {
            if (p->packet.id == i) break;
        }
        if (p == heldQueue.end()) {
            UnitSyncPacket msg;
            msg.type = 0x1a;
            msg.arg = 0x65;
            msg.id = i;
            SendPacketToPlayer(GetLocalHumanDpid(), target, &msg, 0xe);
        }
    }
}

// The original calls this out of line from 0x470040.
#pragma auto_inline(off)
// FUNCTION: 0x470560 ??4PacketSequencer@@QAEAAU0@ABU0@@Z
PacketSequencer& PacketSequencer::operator=(const PacketSequencer& rhs)
{
    lastSent = rhs.lastSent;
    cur = rhs.cur;
    max = rhs.max;
    sentQueue = rhs.sentQueue;
    heldQueue = rhs.heldQueue;
    return *this;
}
#pragma auto_inline(on)

// The constructor of the 0x68-byte object held at g_game+0x2a30: the map
// header at +0x00 (UnitSyncMap), the player vector, the id list and the
// three sub-objects. The map at +0x00 is the std::map<unsigned int,
// UnitSyncEntry> whose tree header lives at 0x46f720.
// FUNCTION: 0x46d040
UnitSync::UnitSync(int param)
    : map(Cmp_0046d040(), Alloc_0046d040())
{
    sub.seqSent = 0;
    sub.seqCur = 0;
    sub.seqMax = 0;
    sub2.disabled = 0;
    sub2.direct = param;
    sub2.pendingPlayerCount = 0;
    sub2.checksumProgress = 0;
    {
        UnitSyncEntry v;
        for (unsigned short i = 1; i < g_game->unitDefCount; i++) {
            unsigned int key = g_game->unitDefs[i].key;
            v.x = key;
            v.y = 0;
            v.w = 1;
            v.h = (short)sub2.direct;
            v.limit = FlagOf_0046d040(&g_game->unitDefs[i]) ? 0 : -1;
            map[key] = v;
        }
    }
}

// Resets every unit type's entry in the map.
// FUNCTION: 0x46d2e0
void UnitSync::ResetEntries()
{
    UnitSyncEntry v;
    for (unsigned short i = 1; i < g_game->unitDefCount; i++) {
        v.x = g_game->unitDefs[i].key;
        // Dead store that must stay: the uninitialised slot is what the insert copies.
        v.y = 0;
        v.w = 1;
        v.h = (short)direct;
        v.limit = FlagOf_0046d040(&g_game->unitDefs[i]) ? 0 : -1;
        ((std::map<unsigned int, UnitSyncEntry>&)map)[v.x] = v;
    }
}

// --- the sync packets the object sends ---------------------------------------

// Sends a sync packet to the given player in direct mode, else to the host.
// The original calls this from 0x46d860 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46d4c0
void UnitSync::SendSyncPacket(unsigned int* param_1, UnitSyncPacket* param_2, int unused)
{
    unsigned int val;
    if (direct != 0)
        val = *param_1;
    else
        val = GetHostDpid();

    param_2->id = 0;
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
        HandleSyncPacket((UnitSyncPacket*)param_1, param_2);
    }
}

// Builds a 0xe-byte packet of type 0x1a and sends it, unless sending is
// disabled. Sibling of SendEntryTo (same object, same packet type).

// FUNCTION: 0x46d530
void UnitSync::SendSyncMessage(unsigned char arg, int a, int b, int unused)
{
    if (disabled == 0) {
        UnitSyncPacket packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.key = a;
        packet.value = b;
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
        UnitSyncPacket packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.key = a;
        packet.value = b;
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
        UnitSyncPacket packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.key = src->key;
        packet.enabled = src->enabled;
        packet.match = src->match;
        packet.limit = src->limit;
        if (direct != 0) {
            SendPacket(target->id, &packet);
        } else {
            SendPacket(GetHostDpid(), &packet);
        }
        target->sent++;
    }
}

// Handles one player-list packet. With `direct` set, the packet updates the
// player's entry in the vector at +0x10 (arg 1 stores a word, arg 2 appends to
// the entry's two std::vector<int> members through the out-of-line
// vector::insert at 0x46e640 and then calls 0x46d970, arg 4 raises a
// maximum). Otherwise an arg 3 packet stores a rectangle in the
// std::map<unsigned int, UnitSyncEntry> at +0x0 and calls 0x46d860.
//
// The map store is `map[key] = r` with the tree header's operator[] (see
// UnitSyncMap): it inserts a default-constructed value, then the locally
// built rectangle is copied over the mapped value.
// FUNCTION: 0x46d6c0
void UnitSync::HandleSyncPacket(UnitSyncPacket* packet, unsigned char player)
{
    if (disabled != 0) {
        return;
    }
    pendingPlayerCount++;
    if (direct != 0) {
        std::vector<PlayerSync_0046e0b0>::iterator i = players.begin();
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
            i->expected = packet->value;
            break;

        case 2:
            {
                int* j = i->ids.begin();
                while (j != i->ids.end()) {
                    if (*j == packet->key) {
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
                v.insert(v.end(), 1, packet->key);
            }
            {
                Vec_0046d6c0& w = i->pairs;
                w.insert(w.end(), 1, packet->value);
            }
            this->CheckUnitAvailable(packet->key, packet->value);
            break;

        case 3:
            break;

        case 4:
            if (i->ackd < (unsigned int)packet->value) {
                i->ackd = packet->value;
            }
            break;
        }
    } else {
        if (packet->arg != 0 && packet->arg == 3) {
            UnitSyncEntry r;
            r.x = packet->key;
            r.y = 0;
            r.w = packet->enabled;
            r.h = packet->match;
            r.limit = packet->limit;
            map[packet->key] = r;
            this->NotifyEntryChanged(packet->key);
        }
    }
}

// Sends the "units expected" (packet type 0x1a, sub-type 3) notice to every
// player whose sync record this object holds, then queues the player's id on
// the insertion-ordered list at +0x20 unless it is already there.
// FUNCTION: 0x46d860
void UnitSync::NotifyEntryChanged(unsigned int param_1)
{
    std::vector<PlayerSync_0046e0b0>& ps = players;
    if (disabled != 0) {
        return;
    }
    if (direct != 0) {
        UnitSyncIter it = map.FindExact(param_1);
        for (std::vector<PlayerSync_0046e0b0>::iterator i = ps.begin(); i != ps.end(); ++i) {
            // v is taken before the disabled check: MSVC then keeps it.ptr in eax
            // across the loop. The check stays a positive block, not a continue.
            UnitSyncEntry* v = &it.ptr->value;
            if (disabled == 0) {
                UnitSyncPacket packet;
                packet.type = 0x1a;
                packet.arg = 3;
                packet.key = v->x;
                packet.enabled = v->w;
                packet.match = v->h;
                packet.limit = v->limit;
                this->SendSyncPacket(&i->id, &packet, 1);
                i->sent++;
            }
        }
    }
    for (std::list<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
        if (*it == param_1) {
            return;
        }
    }
    // push_back takes the parameter itself, not a temporary int copy of it.
    ids.push_back((int&)param_1);
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

    UnitSyncIter it = map.FindExact(key);
    if (it == map.End())
        return;

    int h = 1;
    if (y != 0) {
        if (it.ptr->value.y == 0) {
            int n = g_game->unitDefCount;
            for (int i = 1; i < n; i++) {
                UnitDef* def = &g_game->unitDefs[i];
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
        for (PlayerSync_0046e0b0* e = players.begin(); e != players.end(); e++) {
            int flag;
            if (y != 0) {
                Player_0046e0b0* pl = FindPlayerByDpid(e->id);
                if (pl == 0)
                    break;
                flag = pl->data->versionMajor >= 2 ? 1 : (pl->data->versionMajor == 1 && pl->data->versionMinor >= 2 ? 1 : 0);
            } else {
                flag = 0;
            }
            int* p2 = e->pairs.first;
            int* p1 = e->ids.first;
            int* p3 = e->ids.last;
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
        if (it->ids.size() != it->expected) {
            sprintf(g_unitSyncStatusText, "expected %d units, got %d", it->expected, it->ids.size());
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
// type 3 whose team data->kind is 2) and, otherwise, when its cached count
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
    PlayerSync_0046e0b0* p = players.begin();
    if (p == players.end())
        return 1;
    for (; p != players.end(); p++) {
        Player_0046e0b0* pl = FindPlayerByDpid(p->id);
        if (pl != 0) {
            // pl->active is tested again in the second test: the original
            // reloads it rather than reusing the first test's result.
            if (pl->active != 0 && pl->type == 3 && pl->data->kind == 2)
                continue;
            if (pl->active != 0 && pl->type == 2)
                continue;
            if (p->expected == 0)
                return 0;
            if (p->ids.size() != p->expected)
                return 0;
            if (p->sent != p->ackd)
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
        || (player->active != 0 && player->type == 3 && player->data->kind == 2)
        || (player->active != 0 && player->type == 2))
        return 1;
    for (std::vector<PlayerSync_0046e0b0>::iterator it = players.begin(); it != players.end(); ++it) {
        if (it->id != id)
            continue;
        if (it->expected == 0 || it->ids.size() != it->expected)
            return 0;
        // Written as return 1; break;: keeps the shared return-0 block as the fallthrough.
        if (it->sent == it->ackd)
            return 1;
        break;
    }
    return 0;
}

// Refreshes the map entry of every unit type: the 0x249-byte unitDefs at
// g_game+0x1439b are looked up in the same std::map<unsigned int, Rect> that
// 0x46e330 uses (its find() is inlined here), the entry's bit 23 becomes "the
// rect has a non-empty size", and def+0x15a takes the rect's last field.
// FUNCTION: 0x46e160
void UnitSync::ApplyToUnitTypes()
{
    if (disabled != 0)
        return;
    for (unsigned short i = 1; i < g_game->unitDefCount; i++) {
        UnitDef* def = &g_game->unitDefs[i];
        UnitSyncIter it = Find(&def->key);
        if (it == End()) {
            ProtectUnitDefsReadWrite();
            def->limit = 0;
            def->flags_241.flag_23 = 0;
        } else {
            ProtectUnitDefsReadWrite();
            def->flags_241.flag_23 = (it.ptr->value.w != 0 && it.ptr->value.h != 0);
            def->limit = it.ptr->value.limit;
        }
        ProtectUnitDefsReadOnly();
    }
}

// Dequeues the front event of an insertion-ordered queue: the class holds a
// std::map<unsigned int, UnitSyncEntry> (key at map node +0xc, entry at +0x10)
// for lookup and a std::list at +0x20 (_Head +0x24, _Size +0x28) for the
// order. The list front is the map key. The inlined find() is copied from
// 0x46e330 (LowerBound is the tree's lower_bound(); a missing key yields the
// head node, end()).
// FUNCTION: 0x46e280
int UnitSync::PopChangedEntry(UnitSyncEntry* out)
{
    if (ids.empty())
        return 0;
    // The queue holds the keys as ints; find() takes them as unsigned.
    UnitSyncIter p = Find((unsigned int*)&ids.front());
    *out = p.ptr->value;
    ids.pop_front();
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
    UnitSyncNode* n = Find(&unit->key).ptr;
    n->value.w = (n->value.w == 0);
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Clears the entry's width; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e450
int UnitSync::DisallowUnit(Unit_0046e330* unit)
{
    UnitSyncNode* n = Find(&unit->key).ptr;
    n->value.w = 0;
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Sets the entry's width to 1; returns whether the entry now has a non-empty
// size.
// FUNCTION: 0x46e4d0
int UnitSync::AllowUnit(Unit_0046e330* unit)
{
    UnitSyncNode* n = Find(&unit->key).ptr;
    n->value.w = 1;
    NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}

// Sets the last field of the unit's entry and has NotifyEntryChanged look at it.
// FUNCTION: 0x46e550
void UnitSync::SetUnitLimit(Unit_0046e330* unit, int value)
{
    UnitSyncIter it = Find(&unit->key);
    if (!(it == End())) {
        it.ptr->value.limit = value;
        NotifyEntryChanged(unit->key);
    }
}

struct SyncTaggedVector {
    char field_0x0;
    char unknown_1[3];
    int field_0x4;
    int field_0x8;
    int field_0xc;

    SyncTaggedVector* InitTaggedVector(char* param_1);
};

// FUNCTION: 0x46e5c0
SyncTaggedVector* SyncTaggedVector::InitTaggedVector(char* param_1)
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

class SyncTempTaggedVector {
public:
    std::vector<Elem_0046e5e0> vec;

    ~SyncTempTaggedVector();
};

// FUNCTION: 0x46e5e0
SyncTempTaggedVector::~SyncTempTaggedVector()
{
}

// A std::vector<int> member's destructor: frees the storage and zeroes the
// {_First,_Last,_End} triple.
// The original calls this from 0x46c920 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46e610
SyncChecksumVector::~SyncChecksumVector()
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

// The tree's begin(): stores the leftmost node (the head's left link) through the
// out parameter.
// FUNCTION: 0x46e880
int* UnitSyncMap::Begin(int* param_1)
{
    *param_1 = (int)_Head->left;
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
UnitSyncIter UnitSyncMap::FindExact(const unsigned int& key)
{
    UnitSyncIter p = UnitSyncIter(LowerBound(&key));
    return (p == End() || key_compare(key, p.ptr->key)) ? End() : p;
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

// --- vector<SyncPlayerRecord>::_Destroy (0x46eaa0) -----------------------------

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

// Unused here: real functions declared to keep the file's symbol count (docs/c2-regalloc.md).
void RegisterUnitOrders(void);

int* __stdcall CopyDwordRangeOverwrite(int* first, int* last, int* dest);

class Class_00470250 : public std::vector<Elem_004702a0> {
public:
    unsigned int Capacity() const;

    unsigned int __inline count() const
    {
        if (!_First)
            return 0;
        return (int)((char *)_Last - (char *)_First) >> 2;
    }
};

class Class_00470270 : public Class_00470250 {
public:
    unsigned int Size() const;

    __inline Elem_004702a0* begin() { return _First; }
    __inline Elem_004702a0* end() { return _Last; }
    __inline const Elem_004702a0* begin() const { return _First; }
    __inline const Elem_004702a0* end() const { return _Last; }

    // Three dead one-statement calls standing in for whatever the original
    // spent its inline budget on: /Ob2 inlines _Ucopy and _Destroy here only
    // as deep as this budget allows.
    static __inline void burn(int a)
    {
        int t = a;
        t = t * 3 + 1;
        (void)t;
    }

    // Separate from assign_second: ids inlines size(), pairs does not.
    // the first list
    void __inline assign_first(Class_00470270* d, const Class_00470270* s)
    {
        if (d == s)
            ;
        else if (s->count() <= d->count()) {
            int* p = (int*)s->_First;
            int* e = (int*)s->_Last;
            int* q = (int*)d->_First;
            for (; p != e; ++p, ++q)
                *q = *p;
            d->_Last = d->_First + s->Size();
        } else {
            unsigned int room = d->Capacity();
            if (s->Size() <= room) {
                Elem_004702a0* mid = s->_First;
                mid += d->Size();
                CopyDwordRangeOverwrite((int*)s->begin(), (int*)mid, (int*)d->_First);
                d->_Ucopy(mid, s->_Last, d->_Last);

                d->_Last = d->_First + s->Size();
            } else {
                d->_Destroy(d->_First, d->_Last);
                // begin()/end() here, raw fields in assign_second: sets the register split.
                operator delete(d->begin());
                int n = (int)s->Size();
                if (n < 0)
                    n = 0;
                Elem_004702a0* p = (Elem_004702a0*)operator new(n * 4);
                d->_First = p;
                Elem_004702a0* q = d->_Ucopy(s->begin(), s->end(), p);
                d->_Last = q;
                d->_End = q;
            }
        }
    }

    // the second list
    void __inline assign_second(Class_00470270* d, const Class_00470270* s)
    {
        if (d == s)
            ;
        else if (s->Size() <= d->Size()) {
            Elem_004702a0* r = (Elem_004702a0*)CopyDwordRangeOverwrite(
                (int*)s->_First, (int*)s->_Last, (int*)d->_First);
            d->_Destroy(r, d->_Last);
            d->_Last = d->_First + s->Size();
        } else {
            unsigned int room = d->Capacity();
            if (s->Size() <= room) {
                Elem_004702a0* mid = s->_First;
                mid += d->Size();
                CopyDwordRangeOverwrite((int*)s->_First, (int*)mid, (int*)d->_First);
                d->_Ucopy(mid, s->_Last, d->_Last);

                d->_Last = d->_First + s->Size();
            } else {
                d->_Destroy(d->_First, d->_Last);
                operator delete((void*)d->_First);
                int n = (int)s->Size();
                if (n < 0)
                    n = 0;
                Elem_004702a0* p = (Elem_004702a0*)operator new(n * 4);
                d->_First = p;
                Elem_004702a0* q = d->_Ucopy(s->_First, s->_Last, p);
                d->_Last = q;
                d->_End = q;
            }
        }
    }
};

struct SyncPlayerRecord {              // 0x5c bytes, one vector element
    int id;                            // +0x00
    std::vector<Elem_004702a0> ids;    // +0x04
    std::vector<Elem_004702a0> pairs;  // +0x14
    int expected;                      // +0x24
    int sent;                          // +0x28
    int ackd;                          // +0x2c
    PacketSequencer sub;               // +0x30

    SyncPlayerRecord& operator=(const SyncPlayerRecord& src);
};

typedef std::vector<SyncPlayerRecord> Vec_0046eaa0;
typedef void (Vec_0046eaa0::*DestroyFn_0046eaa0)(Vec_0046eaa0::iterator, Vec_0046eaa0::iterator);

struct Access_0046eaa0 : Vec_0046eaa0 {
    static DestroyFn_0046eaa0 fn;
};

// std::vector<SyncPlayerRecord>::_Destroy(first, last) from MSVC 5's <vector>,
// called from the inlined erase at 0x46db82 (0x46dad0) with ecx set to the
// vector. It runs each 0x5c-byte element's implicit destructor, which frees
// the element's four std::vector members last-first.
// FUNCTION: 0x46eaa0 ?_Destroy@?$vector@USyncPlayerRecord@@V?$allocator@USyncPlayerRecord@@@std@@@std@@IAEXPAUSyncPlayerRecord@@0@Z
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
void UnitSyncMap::Init()
{
    std::_Lockit lock;
    if (DAT_0051e598 == 0) {
        DAT_0051e598 = Buynode(0, 1);
        DAT_0051e598->left = 0, DAT_0051e598->right = 0;
    }
    ++DAT_0051e59c;
    _Head = Buynode(DAT_0051e598, 0), _Size = 0;
    _Head->left = _Head, _Head->right = _Head;
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

class InsertResult {
public:
    Node_0046fad0* first;              // +0x0 (the iterator's node)
    bool second;                       // +0x4

    InsertResult(Node_0046fad0* const& f, const bool& s);
};

// FUNCTION: 0x46fad0
InsertResult::InsertResult(Node_0046fad0* const& f, const bool& s)
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

// Shaped like std::_Tree<...>::_Lbound(const _K&) from MSVC 5's <xtree> for a
// tree keyed by unsigned int, under a lock object; DAT_0051e598 is the tree's
// _Nil node and _Head->parent is the root.
// The original calls this from 0x46e9b0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46fe60
UnitSyncNode* UnitSyncMap::LowerBound(const unsigned int* key)
{
    std::_Lockit lock;
    UnitSyncNode* x = _Head->parent;
    UnitSyncNode* y = _Head;
    while (x != DAT_0051e598)
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

// SyncPlayerRecord::operator=. ids and pairs are two vectors of 4-byte
// elements, each with an out-of-line capacity() (0x470250) and size()
// (0x470270) that both return the element count, i.e. a byte difference
// shifted right by 2. The first list inlines its size() for the first
// comparison, the second does not.
// The members stay plain std::vector for the element destructor (0x46eaa0): a
// vector subclass as the member type changes how deep that inlines, so the
// assign helpers are reached through casts.
// FUNCTION: 0x470040
SyncPlayerRecord& SyncPlayerRecord::operator=(const SyncPlayerRecord& src)
{
    id = src.id;

    ((Class_00470270*)&ids)->assign_first((Class_00470270*)&ids, (const Class_00470270*)&src.ids);
    ((Class_00470270*)&pairs)->assign_second((Class_00470270*)&pairs, (const Class_00470270*)&src.pairs);

    Class_00470270::burn(1);
    Class_00470270::burn(2);
    Class_00470270::burn(3);

    expected = src.expected;
    sent = src.sent;
    ackd = src.ackd;
    sub = src.sub;
    return *this;
}

// --- Class_00470250's and Class_00470270's counts (0x470250, 0x470270) --------

// FUNCTION: 0x470250
unsigned int Class_00470250::Capacity() const
{
    if (!_First) {
        return 0;
    }
    return _End - _First;
}

// FUNCTION: 0x470270
unsigned int Class_00470270::Size() const
{
    if (_First == 0) {
        return 0;
    }
    return _Last - _First;
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
