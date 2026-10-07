// Decompiled by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by
// space-bunny-free, retried by Sonnet 5.5, finished by deepseek-v4.1-flash,
// finished by mimo-v2.6-pro. Names are provisional.
struct Point_004211d0 { int x; int y; };
struct Vec3_004b6cc0 { int x; int y; int z; };

struct Pic_004211d0 { void* pic; int unknown_4; };

// Bitfield union: gives the original's shr/test chain for the flag tests.
struct Flags_004211d0 {
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

struct Face_004211d0 {
    int unknown_0;                   // +0x00
    int count;                       // +0x04
    int unknown_8;                   // +0x08
    unsigned short* indices;         // +0x0c
    Pic_004211d0 pic;                // +0x10
    unsigned short* color;           // +0x18
    Flags_004211d0 flags;            // +0x1c
};

struct Arr_00421550 {
    char unknown_0[4];
    int count;                       // +0x04
    int faceCount;                   // +0x08
    int firstFace;                   // +0x0c
    char unknown_10[0x14];
    Vec3_004b6cc0* items;            // +0x24
    Face_004211d0* faces;            // +0x28
};

#pragma pack(push, 1)
struct Inner_00421550 {
    Arr_00421550* f0;                // +0x00
    char unknown_4[0xc];
    short f10;                       // +0x10
    short f12;                       // +0x12
    short f14;                       // +0x14
    int f16;                         // +0x16
    int f1a;                         // +0x1a
    int f1e;                         // +0x1e
    Vec3_004b6cc0* f22;              // +0x22
};
#pragma pack(pop)

struct Obj_00421170 {
    int f0;                          // +0x00
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    int cameraX;                     // +0x1431f
    int cameraZ;                     // +0x14323
    char unknown_14327[0x37e27 - 0x14327];
    int viewport[4];                 // +0x37e27
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall PointInRect(void* rect, int x, int y);
void* __stdcall GetGafSequenceFrame(Pic_004211d0* pic);
void* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall FillPolygon(void* surface, Point_004211d0* points, int count, int flags);
void __stdcall DrawFrameQuad(void* surface, void* pic, Point_004211d0* points, void* src);

// FUNCTION: 0x4211d0
void __stdcall DrawExplodedPieceFaces(void* surface, Obj_00421170* obj, Inner_00421550* inner)
{
    Point_004211d0 projected[2000];
    Point_004211d0 poly[25];
    int i;

    Arr_00421550* arr = inner->f0;
    struct Off_004211d0 { int a; int y; int b; };
    Off_004211d0 off;
    off.a = inner->f16 - (g_game->cameraX << 16);
    off.y = inner->f1a;
    off.b = inner->f1e - (g_game->cameraZ << 16);

    // Address of off taken once, high words read as hp[n]: forces the original's spill.
    short* hp = (short*)&off;
    int sy = hp[5] - (hp[3] >> 1) + 0x20;
    int sx = hp[1] + 0x80;
    if (!PointInRect(&g_game->viewport[0], sx, sy)) {
        return;
    }

    Vec3_004b6cc0* v = inner->f22;
    // Indexed destination with a walked source pointer u: the byte-exact form.
    Vec3_004b6cc0* u = v;
    for (i = 0; i < arr->count; i++, u++) {
        projected[i].x = (short)((u->x + off.a) >> 16) + 0x80;
        projected[i].y = (short)((off.b - u->z) >> 16)
            - ((short)((u->y + off.y) >> 16) >> 1) + 0x20;
    }

    int j;
    Face_004211d0* face;
    // face and i assigned in both arms: keeps arr in a register, i stored once.
    if (arr->firstFace != -1) {
        face = arr->faces + 1;
        i = 1;
    } else {
        face = arr->faces;
        i = 0;
    }
    for (; i < arr->faceCount; i++, face++) {
        unsigned short* idx = face->indices;
        j = 0;
        // Guarded do/while with the load, ++j and ++idx as separate statements.
        if (face->count > 0) {
            do {
                poly[j] = projected[*idx];
                ++j;
                ++idx;
            } while (j < face->count);
        }
        Flags_004211d0 flags = face->flags;
        if (!flags.bits.a) {
            if (face->count == 4) {
                void* pic;
                if (flags.bits.b) {
                    if (flags.bits.c) {
                        int player = *(int*)(*(int*)(obj->f0 + 0x96) + 0x27);
                        pic = GetGafFrame(face->color,
                            *(unsigned char*)(player + 0x96));
                    } else {
                        pic = GetGafSequenceFrame(&face->pic);
                    }
                } else {
                    pic = face->pic.pic;
                }
                DrawFrameQuad(surface, pic, poly, 0);
            }
        } else {
            FillPolygon(surface, poly, face->count, face->unknown_0);
        }
    }
}