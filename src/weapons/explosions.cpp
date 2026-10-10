// Decompiled by Opus, GPT-6, deepseek-v4.1, deepseek-v4.1-flash, DeepSeek V4.1 Flash, Claude Opus 5.5, space-bunny-free, GPT-6.1-sol, Sonnet 5.5, mimo-v2.6-pro, xiaomi/mimo-v2.6-pro, fledge-alpha-free and GPT-6. Names are provisional.
// Explosions: the CalcedExplosion frames built at start-up, the 300-entry
// debris table and the 100-slot pool of exploded pieces, all reached through
// g_game.
#include <windows.h>
#include <string.h>
// <minmax.h> and <setjmp.h> are here for their symbols only (docs/c2-regalloc.md).
#include <minmax.h>
#include <setjmp.h>

struct Vec3 {
    int x;
    int y;
    int z;

    Vec3 operator+(const Vec3& o) const
    {
        Vec3 r;
        r.x = x + o.x;
        r.y = y + o.y;
        r.z = z + o.z;
        return r;
    }
};

struct Vec3f {
    float x;
    float y;
    float z;
};

struct Point {
    int x;
    int y;
};

struct Point16 {
    short x;
    short z;
};

struct Fixed {
    union {
        int value;
        struct {
            unsigned short fraction;
            short whole;
        } parts;
    };
};

struct Position {
    Fixed x, y, z;
};

struct ImageFrame {
    void* image;                       // +0x0
    short duration;                    // +0x4
    short unknown;
};

struct Explosion {
    unsigned short count;            // +0x0
    char flags;                        // +0x2
    char unknown_3[0x25];
    ImageFrame frames[1];              // +0x28
};

// The explosions as InitGafSequence reads them.
struct GafSequence {
    unsigned short count;              // +0x0
    unsigned char kind;                // +0x2
    char unknown_3[0x2c - 3];
    struct { unsigned short value; char unknown_2[6]; } entries[1]; // +0x2c
};

struct GafRef {
    union {
        void* pic;
        int tex;
        struct {
            unsigned short index;
            unsigned short value;
        };
    };                                 // +0x0
    unsigned char kind;                // +0x4
    char unknown_5[3];
    GafSequence* src;                  // +0x8
};

// Bitfield union: gives the original's shr/test chain for the flag tests.
struct FaceFlags {
    union {
        unsigned int raw;
        struct {
            unsigned int a : 1;
            unsigned int b : 1;
            unsigned int c : 1;
            unsigned int rest : 29;
        } bits;
    };
};

struct Face {
    int color;                         // +0x00
    int nverts;                        // +0x04
    int unknown_8;
    unsigned short* vindex;            // +0x0c
    GafRef tex;                        // +0x10
    FaceFlags flags;                   // +0x1c
};

// A model: the 300 "explodepiece" objects in g_game and the piece models
// of the units are the same thing.
struct Object3D {
    signed char state;                 // +0x00 (-1 = free)
    char unknown_1[3];
    int vertexCount;                   // +0x04
    int faceCount;                     // +0x08
    int firstFace;                     // +0x0c
    char unknown_10[12];
    const char* name;                  // +0x1c
    int active;                        // +0x20
    Vec3* verts;                       // +0x24
    Face* faces;                       // +0x28
    char unknown_2c[8];
};

struct Size {
    char unknown_0[8];
    int x;                             // +0x08
    int y;                             // +0x0c
    int z;                             // +0x10
};

struct PlayerInfoView {
    char unknown_0[0x96];
    unsigned char color;               // +0x96
};

struct PieceList {
    int count;                         // +0x0
};

struct Cell {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned char offsetY;             // +0xa
    unsigned char offsetX;             // +0xb
    char unknown_c;
};

struct Feature {
    char unknown_0[0x94];
    Point16 footprint;                 // +0x94
    char unknown_98[0x100 - 0x98];
};

struct Net {
    char unknown_0[0xd44];
    int f_d44;                         // +0xd44
    int f_d48;                         // +0xd48
};

// Unused here: the symbol ids these declarations take keep UpdateExplosions
// and BreakPieceIntoDebris in the register windows they match in now that
// CMemoryCache comes from a shared header (docs/c2-regalloc.md).
void WriteScreenshot(char*, char*, int, int, int, int);
void SetCameraPosition(int, int, int);
void StartScreenShake(int, int, int);
void AccumulateScreenShake(int, int, int);
void CenterCameraOnPoint(int, int, int);
void SetMissionStatus(int, int, int);

#include "../graphics/memory_cache.h"

#pragma pack(push, 1)
struct PlayerView {
    char unknown_0[0x27];
    PlayerInfoView* info;              // +0x27
};

struct Unit {
    Size* motion;                      // +0x00
    char unknown_4[0x6a - 4];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x96 - 0x76];
    PlayerView* player;                // +0x96
    char unknown_9a[4];
    PieceList* state;                  // +0x9e
};

// A piece of a unit's model, 0x36 bytes each, 0x22 bytes after the count in
// PieceList.
struct PieceRec {
    Object3D* desc;                    // +0x00
    char unknown_4[0xc];
    short angle_10;                    // +0x10
    short angle_12;                    // +0x12
    short angle_14;                    // +0x14
    Vec3 pos;                          // +0x16
    Vec3* verts;                       // +0x22
    char unknown_26[2];
    unsigned short flags;              // +0x28
    char unknown_2a[0x36 - 0x2a];
};

// An exploding piece: the 0x30-byte header each pool slot starts with.
struct ExplodedPiece {
    Unit* unit;                        // +0x00
    int index;                         // +0x04
    int angVelZ;                       // +0x08
    int angVelX;                       // +0x0c
    int angVelY;                       // +0x10
    int vel_x;                         // +0x14
    int vel_y;                         // +0x18
    int vel_z;                         // +0x1c
    int ticks;                         // +0x20
    int scale;                         // +0x24
    unsigned int b0 : 1;               // +0x28
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int rest : 26;
    PieceRec* rec;                     // +0x2c
};

struct ExplodedBlock {
    ExplodedPiece header;              // +0x00
    PieceRec rec;                      // +0x30
};

struct Debris {
    Object3D* obj;                     // +0x00
    GafRef ref1;                       // +0x04
    GafRef ref2;                       // +0x10
    Vec3 pos;                          // +0x1c
    Vec3 size;                         // +0x28
    Vec3 vel;                          // +0x34
    Vec3 spin;                         // +0x40
    short angle_x;                     // +0x4c
    short angle_y;                     // +0x4e
    short angle_z;                     // +0x50
    unsigned short flag : 1;           // +0x52
};

struct Game {
    char unknown_0[0x14263];
    int gravity;                       // +0x14263
    char unknown_14267[0x1426f - 0x14267];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x1431f - 0x14280];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x147eb - 0x14327];
    GafSequence* waterSplashSeq;       // +0x147eb
    GafSequence* lavaSplashSeq;        // +0x147ef
    char unknown_147f3[0x147f7 - 0x147f3];
    GafSequence* explosionSeq;         // +0x147f7
    char unknown_147fb[0x1491b - 0x147fb];
    int count;                         // +0x1491b
    Debris debris[300];                // +0x1491f
    Explosion* explosions[3];          // +0x1ab8f
    void* explosionLensFrame;          // +0x1ab9b
    Object3D pieces[300];              // +0x1ab9f
    Vec3 vertices[300][8];
    Face frames[300][6];
    char unknown_33a0f[0x37e27 - 0x33a0f];
    int viewport[4];                   // +0x37e27
    char unknown_37e37[0x38d74 - 0x37e37];
    unsigned char shade;               // +0x38d74
    char unknown_38d75[0x391e9 - 0x38d75];
    Net* mapInfo;                      // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;
extern CMemoryCache g_debrisMemCache;
extern ExplodedPiece* g_explodedPieces[100];
extern int g_explosion0Duration, g_explosion0StartSize, g_explosion0EndSize, g_explosion1Duration, g_explosion1StartSize, g_explosion1EndSize, g_explosion2Duration, g_explosion2StartSize, g_explosion2EndSize;
extern unsigned short g_faceVertexIndices[6][4];

void* __cdecl GameAllocIgnoreTag(const char* name, unsigned size);
void __cdecl GameFreeThunk(void* p);
void* __stdcall BuildLensFrame(int, int, int);
void __stdcall InitGafSequence(GafRef* ref, GafSequence* src, int index);
void __stdcall EmitSmoke(int* pos, int a, int b, int c);
void __stdcall EmitWhiteSmoke(void* buf, int arg);
void __stdcall EmitJitteredThrustParticles(void* buf, int arg);
int __stdcall PointInRect(void* rect, int x, int y);
void* __stdcall GetGafSequenceFrame(GafRef* ref);
void* __stdcall GetGafFrame(GafSequence* table, int index);
void __stdcall DrawFrameShadow(void* surface, void* frame, int x, int y);
void __stdcall DrawFrame(void* surface, void* frame, int x, int y);
void __stdcall DrawModel3doProjected(void* surface, Position* pos, void* model, short* rotation);
void __stdcall FillPolygon(void* surface, Point* points, int count, int flags);
void __stdcall DrawFrameQuad(void* surface, void* pic, Point* points, void* src);
int __stdcall RandomInt(int range);
int __stdcall GetCellMeanHeight(Vec3* pos);
int __stdcall StepGafSequence(GafRef* ref);
void __stdcall RotateByAngles(Vec3* in, Vec3* out, short* angles);
Vec3f __stdcall VectorFromTo(Vec3f from, Vec3f to);
Vec3f __stdcall CrossProduct(Vec3f a, Vec3f b);
Vec3f __stdcall NormalizeVector(Vec3f v);
Cell* __stdcall GetMapCell(int x, int y);
void __stdcall AddExplosionEffect(Vec3* pos, GafSequence* src, int index, int flag);
Explosion* __stdcall BuildCalcedExplosion(int n, int from, int to);
void* __stdcall BuildExplosionFrame(int size);
int __stdcall UpdateExplodedPiece(ExplodedPiece* obj);
int __stdcall DrawExplodedPiece(void* surface, ExplodedPiece* obj);
void __stdcall StartExplodePiece(ExplodedPiece* spawn);
void __stdcall BreakPieceIntoDebris(ExplodedPiece* param);

// BuildCalcedExplosion (0x420cb0) again: InitExplosions inlines this copy.
inline Explosion* MakeExplosion(int count, int start, int end)
{
    Explosion* result = (Explosion*)GameAllocIgnoreTag("CalcedExplosion", 0x28 + count * 8);
    int radius = start;
    int step = (end - start) / count;
    result->count = count;
    result->flags = 0;
    for (int i = 0; i < count; ++i) {
        result->frames[i].image = BuildExplosionFrame(radius);
        result->frames[i].duration = 2;
        radius += step;
    }
    return result;
}

// FUNCTION: 0x420620
void InitExplosions()
{
    g_game->count = 0;
    g_game->explosionLensFrame = BuildLensFrame(22, 22, 8);
    g_explosion0Duration = 24; g_explosion0StartSize = 64; g_explosion0EndSize = 8;
    g_explosion1Duration = 30; g_explosion1StartSize = 128; g_explosion1EndSize = 16;
    g_explosion2Duration = 30; g_explosion2StartSize = 200; g_explosion2EndSize = 32;
    g_game->explosions[0] = MakeExplosion(g_explosion0Duration / 2, g_explosion0StartSize, g_explosion0EndSize);
    g_game->shade = 20;
    g_game->explosions[1] = MakeExplosion(g_explosion1Duration / 2, g_explosion1StartSize, g_explosion1EndSize);
    g_game->shade = 50;
    g_game->explosions[2] = MakeExplosion(g_explosion2Duration / 2, g_explosion2StartSize, g_explosion2EndSize);
    g_game->shade = 100;
    for (int i = 0; i < 300; ++i) {
        g_game->pieces[i].state = -1;
        g_game->pieces[i].vertexCount = 8;
        g_game->pieces[i].faceCount = 6;
        g_game->pieces[i].firstFace = -1;
        g_game->pieces[i].name = "explodepiece";
        g_game->pieces[i].active = 0;
        g_game->pieces[i].verts = g_game->vertices[i];
        g_game->pieces[i].faces = g_game->frames[i];
    }
    for (i = 0; i < 300; ++i)
        for (int j = 0; j < 6; ++j) {
            g_game->frames[i][j].flags.raw |= 1;
            g_game->frames[i][j].color = 200;
            g_game->frames[i][j].nverts = 4;
            g_game->frames[i][j].vindex = g_faceVertexIndices[j];
        }
    g_debrisMemCache.InitCache(100000);
    memset(g_explodedPieces, 0, sizeof(g_explodedPieces));
}

// Claims a free object (first byte -1), marks it used and returns it, or
// returns 0 when all 300 are taken.
// FUNCTION: 0x420920
Object3D* AllocExplodePieceObject()
{
    for (int i = 0; i < 300; i++) {
        if (g_game->pieces[i].state == -1) {
            g_game->pieces[i].state = 0;
            return &g_game->pieces[i];
        }
    }
    return 0;
}

// FUNCTION: 0x420960
void FreeExplosions(void)
{
    if (g_game->explosionLensFrame != 0) {
        g_debrisMemCache.FreeCache();
        for (int i = 0; i < 3; i++) {
            if (g_game->explosions[i] != 0) {
                for (int j = 0; j < g_game->explosions[i]->count; j++) {
                    GameFreeThunk(g_game->explosions[i]->frames[j].image);
                    g_game->explosions[i]->frames[j].image = 0;
                }
                GameFreeThunk(g_game->explosions[i]);
                g_game->explosions[i] = 0;
            }
        }
        GameFreeThunk(g_game->explosionLensFrame);
        g_game->explosionLensFrame = 0;
    }
}

// Appends one entry (up to 300) to the table at g_game+0x1491b: a position and
// two references built by InitGafSequence.
// FUNCTION: 0x420a30
void __stdcall AddExplosionEffect(Vec3* pos, GafSequence* src, int index, int flag)
{
    int* pCount = &g_game->count;
    if (*pCount < 300) {
        Debris* e = (Debris*)(pCount + 1) + (*pCount)++;
        e->pos = *pos;
        if (src != 0)
            InitGafSequence(&e->ref1, src, 0);
        else
            e->ref1.src = 0;
        if (index >= 0)
            InitGafSequence(&e->ref2, *(GafSequence**)((char*)pCount + index * 4 + 0x6274), 0);
        else
            e->ref2.src = 0;
        if (flag == 0 && *(short*)((char*)&pos->y + 2) > (short)g_game->seaLevel)
            EmitSmoke((int*)pos, 7, 0xf, 9);
        e->obj = 0;
    }
}

// FUNCTION: 0x420b00
void __stdcall DrawExplosions(void* surface)
{
    int i;
    for (i = 0; i < 100; ++i)
        if (g_explodedPieces[i] && !DrawExplodedPiece(surface, g_explodedPieces[i])) g_explodedPieces[i] = 0;
    int* pCount = &g_game->count;
    Debris* d = (Debris*)(pCount + 1);
    Position pos;
    for (i = 0; i < *pCount; ++i, d++) {
        pos.x.value = d->pos.x - (g_game->scrollX << 16);
        pos.y.value = d->pos.y;
        pos.z.value = d->pos.z - (g_game->scrollY << 16);
        int x = pos.x.parts.whole + 128;
        int y = pos.z.parts.whole - (pos.y.parts.whole >> 1) + 32;
        if (PointInRect(g_game->viewport, x, y) && d->ref2.src)
            DrawFrameShadow(surface, GetGafSequenceFrame(&d->ref2), x, y);
    }
    d = (Debris*)(pCount + 1);
    for (i = 0; i < *pCount; ++i, d++) {
        pos.x.value = d->pos.x - (g_game->scrollX << 16);
        pos.y.value = d->pos.y;
        pos.z.value = d->pos.z - (g_game->scrollY << 16);
        int x = pos.x.parts.whole + 128;
        int y = pos.z.parts.whole - (pos.y.parts.whole >> 1) + 32;
        if (PointInRect(g_game->viewport, x, y)) {
            if (d->obj) DrawModel3doProjected(surface, &pos, d->obj, &d->angle_x);
            if (d->ref1.src) DrawFrame(surface, GetGafSequenceFrame(&d->ref1), x, y);
        }
    }
}

// Builds a "CalcedExplosion": a header followed by one entry per frame, each
// frame generated by 0x420d20 (an "ExplosionFrame" of the given size), with
// sizes stepping evenly from `from` towards `to`.
// FUNCTION: 0x420cb0
Explosion* __stdcall BuildCalcedExplosion(int n, int from, int to)
{
    Explosion* e = (Explosion*)GameAllocIgnoreTag("CalcedExplosion", 0x28 + n * 8);
    int radius = from;
    int step = (to - from) / n;
    e->count = n;
    e->flags = 0;
    for (int i = 0; i < n; ++i) {
        e->frames[i].image = BuildExplosionFrame(radius);
        e->frames[i].duration = 2;
        radius += step;
    }
    return e;
}

// FUNCTION: 0x420e50
void __stdcall ExplodeUnitPieces(Unit* unit)
{
    ExplodedPiece s;
    s.b4 = 0;
    s.b5 = 0;
    s.unit = unit;
    s.ticks = 900;
    s.scale = 1;
    for (int i = 0; i < unit->state->count; i++) {
        s.b1 = RandomInt(100) & 1;
        s.b2 = 1;
        s.b3 = 1;
        s.angVelZ = RandomInt(3000);
        s.angVelX = RandomInt(3000);
        s.angVelY = RandomInt(3000);
        s.vel_x = (20 - RandomInt(40)) << 14;
        s.vel_y = RandomInt(10) << 16;
        s.vel_z = (20 - RandomInt(40)) << 14;
        s.index = i;
        StartExplodePiece(&s);
    }
}

// MATCH: use a local int pointer for the gravity subtraction so MSVC reloads vel.y after the position updates.
// FUNCTION: 0x420f30
void UpdateExplosions()
{
    for (int i = 0; i < 100; i++) {
        if (g_explodedPieces[i] != 0 && UpdateExplodedPiece(g_explodedPieces[i]) == 0)
            g_explodedPieces[i] = 0;
    }

    int* pCount = &g_game->count;
    Debris* d = (Debris*)(pCount + 1);
    Debris* base = d;
    int n;
    for (n = 0; n < *pCount; n++, d++) {
        if (d->obj != 0) {
            Vec3 old = d->pos;
            d->pos.x += d->size.x + d->vel.x;
            d->pos.y += d->size.y + d->vel.y;
            d->pos.z += d->size.z + d->vel.z;
            int* velocityY = &d->vel.y;
            *velocityY = *velocityY - g_game->gravity;
            d->angle_x += d->spin.x;
            d->angle_y += d->spin.y;
            d->angle_z += d->spin.z;
            int r = GetCellMeanHeight(&d->pos);
            if (d->pos.y > (int)((unsigned)g_game->seaLevel << 16) || r >= (int)g_game->seaLevel) {
                if (*(short*)((char*)&d->pos.y + 2) <= r) {
                    d->pos = old;
                    d->vel.y = -(d->vel.y / 2);
                    if (*(short*)((char*)&d->vel.y + 2) <= 0) {
                        if (d->flag)
                            AddExplosionEffect(&d->pos, g_game->explosionSeq, 0, 0);
                        d->obj->state = 0xff;
                        d->obj = 0;
                    }
                }
            } else {
                if (d->flag) {
                    Net* net = g_game->mapInfo;
                    if (net->f_d48 == 0) {
                        GafSequence* src = net->f_d44 != 0 ? g_game->lavaSplashSeq : g_game->waterSplashSeq;
                        AddExplosionEffect(&d->pos, src, -1, 1);
                    }
                }
                d->obj->state = 0xff;
                d->obj = 0;
            }
        }
        if (d->ref1.src != 0)
            StepGafSequence(&d->ref1);
        if (d->ref2.src != 0)
            StepGafSequence(&d->ref2);
    }

    int removed = 1;
    while (removed) {
        removed = 0;
        int count = *pCount;
        Debris* e = base;
        int k;
        for (k = 0; k < count; k++, e++) {
            if (e->obj == 0 && e->ref1.src == 0 && e->ref2.src == 0) {
                int j;
                for (j = k; j < *pCount - 1; j++, e++)
                    *e = e[1];
                (*pCount)--;
                removed = 1;
                break;
            }
        }
    }
}

// Returns the index of the first free (zero) slot of the 100-entry table of
// exploded pieces, or -1 when it is full.
// FUNCTION: 0x421150
int FindFreeExplodedPieceSlot()
{
    for (int i = 0; i < 100; i++) {
        if (g_explodedPieces[i] == 0) {
            return i;
        }
    }
    return -1;
}

// Updates every entry of the 100-slot table (see FindFreeExplodedPieceSlot)
// with UpdateExplodedPiece and clears the slots it reports finished.
// FUNCTION: 0x421170
void UpdateExplodedPieces()
{
    for (int i = 0; i < 100; i++) {
        if (g_explodedPieces[i] != 0 && UpdateExplodedPiece(g_explodedPieces[i]) == 0) {
            g_explodedPieces[i] = 0;
        }
    }
}

// Passes every entry of the 100-slot table to DrawExplodedPiece with the
// argument and clears the slots it reports finished (compare 0x421170).
// FUNCTION: 0x4211a0
void __stdcall DrawExplodedPieces(void* surface)
{
    for (int i = 0; i < 100; i++) {
        if (g_explodedPieces[i] != 0 && DrawExplodedPiece(surface, g_explodedPieces[i]) == 0) {
            g_explodedPieces[i] = 0;
        }
    }
}

// FUNCTION: 0x4211d0
void __stdcall DrawExplodedPieceFaces(void* surface, ExplodedPiece* obj, PieceRec* inner)
{
    Point projected[2000];
    Point poly[25];
    int i;

    Object3D* arr = inner->desc;
    struct Off_004211d0 { int a; int y; int b; };
    Off_004211d0 off;
    off.a = inner->pos.x - (g_game->scrollX << 16);
    off.y = inner->pos.y;
    off.b = inner->pos.z - (g_game->scrollY << 16);

    // Address of off taken once, high words read as hp[n]: forces the original's spill.
    short* hp = (short*)&off;
    int sy = hp[5] - (hp[3] >> 1) + 0x20;
    int sx = hp[1] + 0x80;
    if (!PointInRect(&g_game->viewport[0], sx, sy)) {
        return;
    }

    Vec3* v = inner->verts;
    // Indexed destination with a walked source pointer u: the byte-exact form.
    Vec3* u = v;
    for (i = 0; i < arr->vertexCount; i++, u++) {
        projected[i].x = (short)((u->x + off.a) >> 16) + 0x80;
        projected[i].y = (short)((off.b - u->z) >> 16)
            - ((short)((u->y + off.y) >> 16) >> 1) + 0x20;
    }

    int j;
    Face* face;
    // face and i assigned in both arms: keeps arr in a register, i stored once.
    if (arr->firstFace != -1) {
        face = arr->faces + 1;
        i = 1;
    } else {
        face = arr->faces;
        i = 0;
    }
    for (; i < arr->faceCount; i++, face++) {
        unsigned short* idx = face->vindex;
        j = 0;
        // Guarded do/while with the load, ++j and ++idx as separate statements.
        if (face->nverts > 0) {
            do {
                poly[j] = projected[*idx];
                ++j;
                ++idx;
            } while (j < face->nverts);
        }
        FaceFlags flags = face->flags;
        if (!flags.bits.a) {
            if (face->nverts == 4) {
                void* pic;
                if (flags.bits.b) {
                    if (flags.bits.c) {
                        pic = GetGafFrame(face->tex.src, obj->unit->player->info->color);
                    } else {
                        pic = GetGafSequenceFrame(&face->tex);
                    }
                } else {
                    pic = face->tex.pic;
                }
                DrawFrameQuad(surface, pic, poly, 0);
            }
        } else {
            FillPolygon(surface, poly, face->nverts, face->color);
        }
    }
}

// One tick of an exploded piece (a slot of g_explodedPieces, see 0x421170 and
// 0x420f30). obj is the 0x30-byte header 0x481140 builds; its rec holds the
// position and the three rotation shorts. The timer counts down. Above sea
// level the piece moves and GetCellMeanHeight (map height under the point)
// decides whether it hit the ground: on a hit it damps the velocity and, when
// the ground is above +0x20000, stops.
// FUNCTION: 0x4213b0
int __stdcall UpdateExplodedPiece(ExplodedPiece* obj)
{
    int n = obj->ticks;
    PieceRec* inner = obj->rec;
    obj->ticks = n - 1;
    if (n == 0)
        return 0;

    Vec3 pos;
    if (inner->pos.y <= (int)((unsigned)g_game->seaLevel << 16)) {
        if (obj->b4 && g_game->mapInfo->f_d48 == 0) {
            GafSequence* src;
            if (g_game->mapInfo->f_d44 != 0)
                src = g_game->lavaSplashSeq;
            else
                src = g_game->waterSplashSeq;
            pos.x = inner->pos.x;
            pos.y = inner->pos.y;
            pos.z = inner->pos.z;
            AddExplosionEffect(&pos, src, -1, 1);
            return 0;
        }
        return 0;
    }

    int vy = obj->vel_y;
    pos.x = inner->pos.x;
    pos.y = inner->pos.y;
    pos.z = inner->pos.z;
    int limit = GetCellMeanHeight(&pos) << 16;
    if (vy + inner->pos.y <= limit) {
        int ny = -(vy >> 1);
        int nx = obj->vel_x >> 1;
        int nz = obj->vel_z >> 1;
        obj->vel_y = ny;
        obj->vel_x = nx;
        obj->vel_z = nz;
        if (ny < 0x20000) {
            if (obj->b4) {
                pos.x = inner->pos.x;
                pos.y = inner->pos.y;
                pos.z = inner->pos.z;
                AddExplosionEffect(&pos, g_game->explosionSeq, 0, 0);
            }
            return 0;
        }
    }

    inner->pos.x += obj->vel_x;
    inner->pos.y += obj->vel_y;
    inner->pos.z += obj->vel_z;
    inner->angle_10 += (short)obj->angVelX;
    inner->angle_12 += (short)obj->angVelY;
    inner->angle_14 += (short)obj->angVelZ;
    if (obj->b3)
        obj->vel_y -= g_game->gravity;
    return 1;
}

// FUNCTION: 0x421550
int __stdcall DrawExplodedPiece(void* surface, ExplodedPiece* obj)
{
    PieceRec* inner = obj->rec;
    short angles[3];
    int buf[3];

    if (obj->b1) {
        buf[0] = inner->pos.x;
        buf[1] = inner->pos.y;
        buf[2] = inner->pos.z;
        EmitWhiteSmoke(buf, 9);
    }
    if (obj->b0) {
        buf[0] = inner->pos.x;
        buf[1] = inner->pos.y;
        buf[2] = inner->pos.z;
        EmitJitteredThrustParticles(buf, 9);
    }

    angles[0] = inner->angle_14;
    angles[2] = inner->angle_10;
    angles[1] = inner->angle_12;

    int n = inner->desc->vertexCount;
    for (int i = n - 1; i >= 0; i--) {
        RotateByAngles(&inner->desc->verts[i], &inner->verts[i], angles);
    }

    DrawExplodedPieceFaces(surface, obj, inner);
    return 1;
}

// Takes a free slot from the 100-entry pool at g_explodedPieces and copies the
// argument header plus the per-unit record into the freshly allocated block.
// FUNCTION: 0x421620
void __stdcall StartExplodePiece(ExplodedPiece* param_1)
{
    Unit* obj = param_1->unit;
    PieceRec* rec = (PieceRec*)((char*)obj->state + 0x22 + param_1->index * 0x36);
    rec->flags &= 0xfffe;
    if (param_1->b2) {
        BreakPieceIntoDebris(param_1);
        return;
    }
    int index = FindFreeExplodedPieceSlot();
    if (index < 0) {
        return;
    }
    int num = rec->desc->vertexCount;
    if (g_debrisMemCache.AllocHandle((void**)&g_explodedPieces[index], num * 12 + 0x66) == 0) {
        return;
    }
    ExplodedBlock* block = (ExplodedBlock*)g_explodedPieces[index];
    block->header = *param_1;
    PieceRec* dst = (PieceRec*)((char*)block + 0x30);
    block->header.rec = dst;
    *dst = *rec;
    block->header.rec->verts = (Vec3*)((char*)block->header.rec + 0x36);
    block->header.rec->pos.x += obj->pos.x;
    block->header.rec->pos.y += obj->pos.y;
    block->header.rec->pos.z += obj->pos.z;
}

// For its symbols only: it sets the registers of 0x421700.
#include <stddef.h>

// Cast and whole expression each parenthesised: keeps the a, b, c stores
// list-scheduled.
#define FIX2F(x) (((float)(x)) / 65535.0f)

// Breaks a unit piece into debris: for each quad face of the piece, takes a
// free debris slot and a free 3D object (AllocExplodePieceObject, inlined),
// gives it a random velocity and spin, copies the face's four vertices into the
// object
// (twice, as a box), pushes the back four out along the face normal by
// param->scale, centres the box on its own origin and copies the face's
// texture and colour into the object's faces.
// FUNCTION: 0x421700
void __stdcall BreakPieceIntoDebris(ExplodedPiece* param)
{
    Unit* unit = param->unit;
    // piece is computed from unit, and desc is read before verts: the order of
    // first uses sets the frame slots.
    PieceRec* piece =
        (PieceRec*)((char*)unit->state + 0x22 + param->index * 0x36);
    int* count = &g_game->count;
    Object3D* desc = piece->desc;
    Vec3* verts = piece->verts;
    if (param->scale == 0)
        param->scale = 1;
    for (int i = 0; i < desc->faceCount; i++) {
        if (*count >= 300)
            return;
        if (desc->faces[i].nverts == 4 && !(desc->faces[i].flags.raw & 1) && desc->firstFace != i) {
            Debris* d = (Debris*)(count + 1) + (*count)++;
            d->pos = piece->pos + unit->pos;
            d->obj = AllocExplodePieceObject();
            if (d->obj == 0)
                return;
            d->flag = param->b5;
            if (unit->motion) {
                d->size.x = unit->motion->x >> 1;
                d->size.z = unit->motion->z >> 1;
                d->size.y = unit->motion->y >> 1;
            }
            d->vel.x = (0x50 - RandomInt(0xa0)) << 9;
            d->vel.z = (0x50 - RandomInt(0xa0)) << 9;
            d->vel.y = ((0x50 - RandomInt(0xa0)) << 9) + g_game->gravity * 30;
            d->spin.x = 800 - RandomInt(0x640);
            d->spin.y = 800 - RandomInt(0x640);
            d->spin.z = 800 - RandomInt(0x640);
            d->angle_x = 0;
            d->angle_y = 0;
            d->angle_z = 0;
            d->ref1.src = 0;
            d->ref2.src = 0;
            Object3D* o = d->obj;
            int k;
            for (k = 0; k < desc->faces[i].nverts; k++) {
                o->verts[k] = verts[desc->faces[i].vindex[k]];
                o->verts[7 - k] = verts[desc->faces[i].vindex[k]];
            }
            Vec3* v = o->verts;
            Vec3f a, b, c;
            a.x = FIX2F(v[0].x);
            a.y = FIX2F(v[0].y);
            a.z = FIX2F(v[0].z);
            b.x = FIX2F(v[1].x);
            b.y = FIX2F(v[1].y);
            b.z = FIX2F(v[1].z);
            c.x = FIX2F(v[2].x);
            c.y = FIX2F(v[2].y);
            c.z = FIX2F(v[2].z);
            Vec3f ab, n;
            ab = VectorFromTo(b, a);
            // Second difference goes into n itself, not nested in CrossProduct.
            n = VectorFromTo(b, c);
            n = NormalizeVector(CrossProduct(n, ab));
            d->vel.x += RandomInt(200) * (short)(n.x * 512.0f);
            d->vel.z -= RandomInt(200) * (short)(n.z * 512.0f);
            int nx = (int)(n.x * 65535.0f);
            int ny = (int)(n.y * 65535.0f);
            int nz = (int)(n.z * 65535.0f);
            for (k = 0; k < 4; k++) {
                o->verts[k + 4].x -= nx * param->scale;
                o->verts[k + 4].y -= ny * param->scale;
                o->verts[k + 4].z -= nz * param->scale;
            }
            int sx = 0;
            int sy = 0;
            int sz = 0;
            for (k = 0; k < 8; k++) {
                sx += o->verts[k].x;
                sy += o->verts[k].y;
                sz += o->verts[k].z;
            }
            sx /= 8;
            sy /= 8;
            sz /= 8;
            for (k = 0; k < 8; k++) {
                o->verts[k].x -= sx;
                o->verts[k].y -= sy;
                o->verts[k].z -= sz;
            }
            for (int m = 0; m < o->faceCount; m++) {
                o->faces[m].tex = desc->faces[i].tex;
                o->faces[m].flags = desc->faces[i].flags;
                o->faces[m].color = desc->faces[i].color;
                int flags = o->faces[m].flags.raw;
                if (!(flags & 1) && (flags & 2) && (flags & 4)) {
                    o->faces[m].tex.pic = GetGafFrame(o->faces[m].tex.src, unit->player->info->color);
                    o->faces[m].flags.raw &= ~2;
                }
            }
        }
    }
}

// FUNCTION: 0x421da0
unsigned short __stdcall FindFeatureAtPos(Vec3* pos, Point16* cell, Point16* size)
{
    Point16 c;
    c.x = (short)(pos->x >> 20);
    c.z = (short)(pos->z >> 20);
    Cell* p = GetMapCell(c.x, c.z);
    if (p == 0)
        return 0xffff;
    if (p->feature == 0xfffe) {
        c.x -= p->offsetX;
        c.z -= p->offsetY;
        p = GetMapCell(c.x, c.z);
    }
    if (p->feature >= 0xfffb)
        return 0xffff;
    if (cell)
        *cell = c;
    if (size)
        *size = g_game->features[p->feature].footprint;
    return p->feature;
}
