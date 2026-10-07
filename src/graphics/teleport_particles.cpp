// Decompiled by Opus, Haiku, Sonnet, DeepSeek V4.1 Flash and Claude Opus 5.5. Names are provisional.
// TeleportParticles (vtable 0x4fd588, 0x44 bytes), derived from ParticleSystem
// (the family is listed in 0x471cc0.cpp): a line of animated sparks from pos1
// to pos2.
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

// The helpers must stay inlined methods: free expressions change the load order.
struct Vec3_004736e0 {
    int x;
    int y;
    int z;
    Vec3_004736e0 operator-(const Vec3_004736e0& o) const
    {
        Vec3_004736e0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    int Length() const
    {
        float fx = x;
        float fy = y;
        float fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
};

union Fix_004736e0 {
    int whole;
    short half[2];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    short field_1431f;                 // +0x1431f
    char unknown_14321[2];
    short field_14323;                 // +0x14323
    char unknown_14325[0x147f3 - 0x14325];
    void* unknown_147f3;               // +0x147f3, the spark animation
    char unknown_147f7[0x38a47 - 0x147f7];
    int field_38a47;                   // +0x38a47, the tick
};
#pragma pack(pop)

extern Game* g_game;

extern "C" int __stdcall GetGafFrameCount(void* ptr);

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FreeSlot(void* p);            // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;

// One spark, 0x34 bytes.
class Class_00473560 {
public:
    void* field_0;                     // +0x0, the animation
    Vec3_004736e0 pos1;                // +0x4
    Vec3_004736e0 pos2;                // +0x10
    Vec3_004736e0 dir;                 // +0x1c
    int field_28;                      // +0x28, the frame count - 1
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30, the tick it expires

    void Step();
    void DrawParticle(void* p, short a, short b);
    int IsExpired(int param_1);
};

// The particle vector seen as its four words, to call its out-of-line insert.
class List_004737c0 {
public:
    char* head;                        // +0x00
    char* first;                       // +0x04
    char* last;                        // +0x08
    char* end;                         // +0x0c
    void FUN_004758c0(char* where, int count, const Class_00473560& val);
};

class ParticleSystem {
public:
    int field_4;                                        // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void FUN_00472e30(void* p) = 0;             // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
    void SetLifetime(int ticks);
};

class TeleportParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8, the next emit tick
    std::vector<Class_00473560> items;                  // +0xc (_First +0x10)
    int field_1c;                                       // +0x1c, the sparks' lifetime
    Vec3_004736e0 pos1;                                 // +0x20
    Vec3_004736e0 pos2;                                 // +0x2c
    Vec3_004736e0 dir;                                  // +0x38

    TeleportParticles() {}
    virtual void Update();                              // slot 1, 0x472d50
    virtual void FUN_00472e30(void* p);                 // slot 2, 0x472e30
    virtual int FUN_00472e70();                         // slot 3, 0x472e70
    virtual void Emit();                                // slot 4, 0x4737c0
    virtual int FUN_00472e00();                         // slot 5, 0x472e00
    virtual void FUN_004736e0(Vec3_004736e0* a, Vec3_004736e0* b, int c);  // slot 6
};

// The base destructor and operator delete, defined again, unannotated, because
// the original file defined them and /Ob2 inlined them into the scalar
// deleting destructor below.
ParticleSystem::~ParticleSystem()
{
}

void __stdcall ParticleSystem::operator delete(void* p)
{
    DAT_0051e610.FreeSlot(p);
}

// The compiler-generated scalar deleting destructor.
// The global below exists only to make the compiler emit the vtable and this COMDAT.
// FUNCTION: 0x471430 ??_GTeleportParticles@@UAEPAXI@Z
static TeleportParticles* s_object = new TeleportParticles;

// Steps every element of the vector, dropping the ones the element's own test
// rejects (erase in place, so the element now at the cursor is tested again),
// then hands over to the virtual at +0x14, which asks the one at +0x10 to
// rebuild. Identical in shape to 0x473170, the same slot of the sibling class.
// FUNCTION: 0x472d50
void TeleportParticles::Update()
{
    std::vector<Class_00473560>::iterator it = items.begin();

    while (it != items.end()) {
        it->Step();
        if (it->IsExpired(g_game->field_38a47)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (FUN_00472e00()) {
        Emit();
    }
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x472e00
int TeleportParticles::FUN_00472e00()
{
    if (field_8 <= field_4) {
        unsigned int game_val = g_game->field_38a47;
        if ((unsigned int)field_8 <= game_val) {
            return 1;
        }
    }
    return 0;
}

// Calls 0x473590 on every element of the std::vector of 52-byte elements
// whose emptiness 0x472e70 tests.
// FUNCTION: 0x472e30
void TeleportParticles::FUN_00472e30(void* p)
{
    for (std::vector<Class_00473560>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle(p, g_game->field_1431f, g_game->field_14323);
    }
}

// Returns whether the particle vector is empty.
// FUNCTION: 0x472e70
int TeleportParticles::FUN_00472e70()
{
    return items.empty();
}

// Sibling of 0x4742c0: the same base call, the same two position copies, the
// same difference of the two and the same trailing virtual call, but this one
// divides the difference by how many 327680-unit segments its length holds
// instead of multiplying it by a reciprocal id.
// FUNCTION: 0x4736e0
void TeleportParticles::FUN_004736e0(Vec3_004736e0* a, Vec3_004736e0* b, int c)
{
    SetLifetime(c);
    pos1 = *a;
    pos2 = *b;
    dir = pos2 - pos1;
    int dist = dir.Length();
    // The quotient goes through a union so it sits in memory; the high half is read back.
    Fix_004736e0 scale;
    scale.whole = (int)(((__int64)dist << 16) / 327680);
    int step = scale.half[1];
    field_1c = step;
    dir.x /= step;
    dir.y /= step;
    dir.z /= step;
    Emit();
}

// Slot 4. It first makes room for however many ten-tick
// units field_4 has fallen behind the current tick with items.reserve(...),
// then appends one element built from the three positions plus a random value,
// and finally sets field_8 ten ticks ahead of the current tick.
// FUNCTION: 0x4737c0
void TeleportParticles::Emit()
{
    int grow = (field_4 - g_game->field_38a47 + 10) / 10;

    // The inlined vector::reserve itself is needed, not a hand-written block.
    if (grow > 0)
        items.reserve(grow + items.size());

    for (int i = 0; i < 1; i++) {
        Class_00473560 e;

        e.pos1 = pos1;
        e.pos2 = pos2;
        e.dir = dir;
        e.field_30 = g_game->field_38a47 + field_1c;
        e.field_0 = g_game->unknown_147f3;
        e.field_28 = GetGafFrameCount(g_game->unknown_147f3) - 1;
        e.field_2c = (int)(((__int64)rand() * e.field_28) / 0x8000);
        // Not push_back: calling through the List layout keeps the insert out of line.
        List_004737c0* v = (List_004737c0*)&items;
        v->FUN_004758c0(v->last, 1, e);
    }

    field_8 = g_game->field_38a47 + 10;
}
