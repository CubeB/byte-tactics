// Decompiled by Opus and space-bunny-free. Names are provisional.
// UnitSyncPlayer (0x5c bytes): one player's unit-sync state, with four
// std::vector members.
// Stays in its own file: the ??_GUnitSyncPlayer COMDAT below is emitted with
// ~UnitSyncPlayer inlined, while 0x46c920 in unit_sync.cpp calls that
// destructor out of line; one file cannot have it both ways.
#include <map>
#include <list>
#include <vector>

#pragma pack(push, 2)
struct Elem_0046faf0 {                 // 14 bytes
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

struct Rect_0046e330 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int limit;                         // +0xc
};

// The two 14-byte-element vectors, a sub-struct with a copy constructor of its
// own: that is what spends the copy constructor's inline budget so the fourth
// _Ucopy stays a call (see there).
struct Pair_0046faf0 {                 // 32 bytes
    std::vector<Elem_0046faf0> sentQueue;
    std::vector<Elem_0046faf0> heldQueue;
    Pair_0046faf0(const Pair_0046faf0& other)
        : sentQueue(other.sentQueue), heldQueue(other.heldQueue)
    {
    }
};

class UnitSyncPlayer {
public:
    int id;                                    // +0x00
    std::vector<int> ids;                      // +0x04
    std::vector<int> pairs;                    // +0x14
    int expected;                              // +0x24
    int sent;                                  // +0x28
    int ackd;                                  // +0x2c
    int lastSent;                              // +0x30
    int cur;                                   // +0x34
    int max;                                   // +0x38
    Pair_0046faf0 pair;                        // +0x3c

    UnitSyncPlayer(const UnitSyncPlayer& other);
    ~UnitSyncPlayer();
};

// The destructor: each inlined ~vector frees its buffer and zeroes
// _First/_Last/_End, in reverse order.
// FUNCTION: 0x46ded0
UnitSyncPlayer::~UnitSyncPlayer()
{
}

// The compiler-generated scalar deleting destructor, with the destructor
// inlined.
// The class has no virtual destructor, so MSVC only emits this ??_G where an
// inlined vector<UnitSyncPlayer> destroy loop runs out of inline depth and
// calls it with flag 0: its one caller, 0x46ca60, which deletes the object
// held at g_game+0x2a30 (UnitSync, constructor 0x46d040).
//
// That caller is rebuilt below, unannotated and only approximately (about
// 63%), to emit this COMDAT. Its sibling 0x46c920 deletes the same object
// one inline level shallower and calls ~UnitSyncPlayer instead.
class SyncChecksumVector {
public:
    std::vector<int> vec;
};

class UnitSync {
public:
    std::map<unsigned int, Rect_0046e330> rects;   // +0x00
    std::vector<UnitSyncPlayer> elems;             // +0x10
    std::list<int> ids;                            // +0x20
    int seqSent;                                   // +0x2c
    int seqCur;                                    // +0x30
    int seqMax;                                    // +0x34
    SyncChecksumVector seqSentQueue;               // +0x38
    std::vector<int> seqHeldQueue;                 // +0x48
    int direct;                                    // +0x58
    int pendingPlayerCount;                        // +0x5c
    int checksumProgress;                          // +0x60
    int disabled;                                  // +0x64

    void ApplyToUnitTypes();
};

struct Game {
    char unknown_0[0x2a30];
    UnitSync* sync;                                // +0x2a30
};

extern Game* g_game;

// FUNCTION: 0x470300 ??_GUnitSyncPlayer@@QAEPAXI@Z
void FinishUnitSync()
{
    g_game->sync->ApplyToUnitTypes();
    if (g_game->sync)
        delete g_game->sync;
    g_game->sync = 0;
}

// The copy constructor. Its one caller copies a 0x5c-byte array of these.
// A std::vector in this build is 16 bytes: the empty allocator member is its
// first dword and _First, _Last and _End follow it, so the single byte copy
// in front of each vector is the allocator's own initializer, and the vector
// at +0x04 has its _First at +0x08, the one at +0x14 at +0x18, the one at
// +0x3c at +0x40 and the one at +0x4c at +0x50.
// Each vector member is copied by the copy constructor of MSVC 5's <vector>:
// size(), allocator.allocate() (??2@YAPAXI@Z, the array new) and _Ucopy.
// FUNCTION: 0x470390
UnitSyncPlayer::UnitSyncPlayer(const UnitSyncPlayer& other)
    : id(other.id), ids(other.ids), pairs(other.pairs),
      expected(other.expected), sent(other.sent), ackd(other.ackd),
      lastSent(other.lastSent), cur(other.cur), max(other.max),
      pair(other.pair)
{
}
