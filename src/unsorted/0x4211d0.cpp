// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 54.4%. Frame size (0x3f58), array layout (poly[25] at +0x20,
// projected[2000] at +0xe8) and the face/draw loop skeleton are right. What
// still differs: (1) the prologue does not spill off.a/off.y/off.b to the
// same homes and re-read their high words as `movsx r, word ptr [esp+0x16]`
// (mine keeps off.a in esi and off.b in edi and reads the low halves); the
// original stores the aggregate then reads the high words from memory.
// (2) The vertex loop destination: original biases ecx to projected+4 and
// writes [ecx-4]/[ecx-8]; mine starts at projected and writes [ecx]/[ecx-4].
// (3) The copy-loop scratch register: original uses ebx for the point value
// and reloads surface (ebx) after the loop; mine uses ebp and keeps surface
// in ebx, so the reload order differs. (4) After the copy loop the original
// reloads edi (i) then ebx (surface); mine reloads ebx then ebp. The flags
// test is written `test al,2`/`test al,4` instead of the original's
// shr/test form. Reference near-copy: 0x4584d0 (same callees, same frame,
// matched at 79.6% by another model).

struct Point_004211d0 { int x; int y; };
struct Vec3_004b6cc0 { int x; int y; int z; };

struct Pic_004211d0 { void* pic; int unknown_4; };

struct Face_004211d0 {
    int unknown_0;                   // +0x00
    int count;                       // +0x04
    int unknown_8;                   // +0x08
    unsigned short* indices;         // +0x0c
    Pic_004211d0 pic;                // +0x10
    unsigned short* color;           // +0x18
    unsigned int flags;              // +0x1c
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
struct Game_004211d0 {
    char unknown_0[0x1431f];
    int cameraX;                     // +0x1431f
    int cameraZ;                     // +0x14323
    char unknown_14327[0x37e27 - 0x14327];
    int viewport[4];                 // +0x37e27
};
#pragma pack(pop)

extern Game_004211d0* g_game;

int __stdcall FUN_004b6720(void* rect, int x, int y);
void* __stdcall FUN_004b7ee0(Pic_004211d0* pic);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004c0310(void* surface, Point_004211d0* points, int count, int flags);
void __stdcall FUN_004c7580(void* surface, void* pic, Point_004211d0* points, void* src);

// FUNCTION: 0x4211d0
void __stdcall FUN_004211d0(void* surface, Obj_00421170* obj, Inner_00421550* inner)
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

    int sy = (short)(off.b >> 16) - ((short)(off.y >> 16) >> 1) + 0x20;
    int sx = (short)(off.a >> 16) + 0x80;
    if (!FUN_004b6720(&g_game->viewport[0], sx, sy)) {
        return;
    }

    Vec3_004b6cc0* v = inner->f22;
    Point_004211d0* q = projected;
    for (i = 0; i < arr->count; i++, q++) {
        q->x = (short)((v[i].x + off.a) >> 16) + 0x80;
        q->y = (short)((off.b - v[i].z) >> 16)
            - ((short)((v[i].y + off.y) >> 16) >> 1) + 0x20;
    }

    int j;
    Face_004211d0* face = arr->faces;
    if (arr->firstFace != -1) {
        face++;
        i = 1;
    } else {
        i = 0;
    }
    for (; i < arr->faceCount; i++, face++) {
        unsigned short* idx = face->indices;
        for (j = 0; j < face->count; j++) {
            poly[j] = projected[*idx++];
        }
        unsigned int flags = face->flags;
        if (!(flags & 1)) {
            if (face->count == 4) {
                void* pic;
                if (flags & 2) {
                    if (flags & 4) {
                        int player = *(int*)(*(int*)(obj->f0 + 0x96) + 0x27);
                        pic = FUN_004b7f30(face->color,
                            *(unsigned char*)(player + 0x96));
                    } else {
                        pic = FUN_004b7ee0(&face->pic);
                    }
                } else {
                    pic = face->pic.pic;
                }
                FUN_004c7580(surface, pic, poly, 0);
            }
        } else {
            FUN_004c0310(surface, poly, face->count, face->unknown_0);
        }
    }
}
