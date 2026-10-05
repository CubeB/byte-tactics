// Decompiled by space-bunny-free. Names are provisional.
//
// Looks up (or creates) the PlayerFrameInfo entry for an id in a table of ten
// of them, the layout of the class built by the constructor 0x462c00: an entry
// is 0x34 bytes at this+0x20, with a three-int head (the debug string gives
// PlayerFrameInfo away) and a tail holding three zeroed ints, a buffer pointer
// and two -1s, the same tail as the sibling 0x463610 (see 0x4635b0.cpp and
// 0x4636b0.cpp for the buffer object).
//
// Three ways an entry is found: one already holding the id, an unused one
// (-1) after the first hole, or, when the table is full, one whose id no
// player in g_game's table of ten 0x14b-byte players still refers to (that one
// is recycled, and the old id keeps its g_game player). Returns 0 when all ten
// are still in use.
//
// Init and Reset are inline methods of the entry: the compiler tail-merged the
// two copies of Reset (and of the three head stores after each print), which is
// why both call sites jump to one shared block. The reset stores and the buffer
// test only come out in the original order (the buffer load between the last
// head store and the tail stores) with the tail as a member of its own with an
// inline Init, and the two loops need separate pointer variables for the test
// and for the initialise, or MSVC strength-reduces one and misses the
// recomputed address at the second call site.

#pragma pack(push, 1)

struct GameEntry_00462d90 {
    int id;                                // +0x00 (g_game + 0x1b67)
    char unknown_4[0x14b - 4];
};

struct Game {
    char unknown_0[0x1b67];
    GameEntry_00462d90 players[10];
};

#pragma pack(pop)

// The object the entry's buffer field points at.
struct Obj_00462d90 {
    int field_0;                           // +0x0
    int field_4;                           // +0x4
    int field_8;                           // +0x8
    char unknown_c[0x180c - 0xc];

    Obj_00462d90() { field_0 = 0; field_4 = 0; field_8 = -1; }
};

void __cdecl PacketTrace(const char* fmt, ...);

struct Tail_00462d90 {
    int field_0;                           // +0x18
    int field_4;                           // +0x1c
    int field_8;                           // +0x20
    int field_c;                           // +0x24
    Obj_00462d90* buffer;                  // +0x28
    int field_14;                          // +0x2c
    int field_18;                          // +0x30

    void Init()
    {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_c = 0;
        field_14 = -1;
        field_18 = -1;
        if (buffer == 0)
            buffer = new Obj_00462d90;
    }
};

struct PlayerFrameInfo {
    int field_0;                           // +0x00, the id
    int field_4;                           // +0x04
    int field_8;                           // +0x08
    int field_c;                           // +0x0c
    int field_10;                          // +0x10
    char* field_14;                        // +0x14
    Tail_00462d90 tail;                    // +0x18

    void Init(long id)
    {
        PacketTrace("PlayerFrameInfo::Initialize: %ld", id);
        field_0 = id;
        field_4 = -1;
        field_8 = -1;
    }

    void Reset()
    {
        field_c = 0;
        tail.Init();
    }
};

class Class_00462d30 {
public:
    char unknown_0[4];
    int field_4;                           // +0x04
    int field_8;                           // +0x08
    int field_c;                           // +0x0c
    int field_10;                          // +0x10
    int field_14;                          // +0x14
    void* field_18;                        // +0x18
    void* field_1c;                        // +0x1c
    PlayerFrameInfo entries[10];           // +0x20

    PlayerFrameInfo* FindPlayerFrameInfo(long id);
};

extern Game* g_game;

// FUNCTION: 0x462d90
PlayerFrameInfo* Class_00462d30::FindPlayerFrameInfo(long id)
{
    unsigned int i;
    int j;
    PlayerFrameInfo* e;

    for (i = 0; i < 10; i++) {
        e = &entries[i];
        if (e->field_0 == id)
            return e;
        if (e->field_0 == -1)
            break;
    }
    if (i < 10) {
        for (; i < 10; i++) {
            PlayerFrameInfo* p = &entries[i];
            if (p->field_0 == -1) {
                entries[i].Init(id);
                e = &entries[i];
                e->Reset();
                return e;
            }
        }
    }
    for (i = 0; i < 10; i++) {
        int used = 0;
        for (j = 0; j < 10; j++) {
            if (g_game->players[j].id == entries[i].field_0) {
                used = 1;
                break;
            }
        }
        if (used)
            continue;
        e = &entries[i];
        e->Init(id);
        e->Reset();
        return e;
    }
    return 0;
}
