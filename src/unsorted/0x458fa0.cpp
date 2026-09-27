// Decompiled by space-bunny-free. Names are provisional.
// GAVE UP at 78.2% (452 of 455 bytes). The frame size (0x5efc, with the
// `mov eax, 0x5efc; call __chkstk`), the local slots (faceno at E+0, piece at
// E+4, info at E+8, count at E+0xc, tmp[25] and verts[2000]), the backwards
// piece walk, the two `dec ecx; js` / `dec ecx; jne` loop shapes, the face
// start, the copy into tmp, `tmp[k] = tmp[0]`, the FUN_004c0820 call, the
// unaligned bit-30 flag at +0x241 and the `? 125 : 50` shade all match byte
// for byte. What is left is the register allocation of the vertex loop: the
// original keeps the view pointer in ebp (loaded between `push ebp` and
// `push ebx` in the preheader), the piece counter in ecx and the piece
// pointer in eax, and it batches the three vertex loads `[ecx]`, `[ecx+4]`,
// `[ecx+8]` before `add eax, 0xc`. This file puts the view pointer in ebx, the
// counter in eax and the piece pointer in esi, and sinks the z load. The
// ordering `x=; y=; y+=field_6; z=; x+=field_4;` gets the preheader right but
// then MSVC hoists `movsx edi, word [ebp+6]` and spills `bright` to
// [esp+0x10], which the original does not.
// Note for whoever takes this next: two scratch variants (v_O_zlast.cpp and
// v_Q2.cpp) score a *higher* 80.3% but are wrong. They lay the two vertex
// arrays out 4 bytes high, at [esp+0x150] where the original uses
// [esp+0x14c] (check with `objdump -d` on the .o). v_P6 is the best variant
// with the original's real local layout, so that is what is committed here.
struct Vertex_0045a610 {
    int x;
    int y;
    int z;
};

struct View_0045a610 {
    short field_0;                   // width
    short field_2;                   // height
    short field_4;                   // x origin, added to every vertex x
    short field_6;                   // y origin, added to every vertex y
};

struct Face_00458fa0 {
    int unknown_0;
    int count;                       // +0x4 number of points in the face
    int unknown_8;
    unsigned short* indices;         // +0xc indexes into the point array
    char unknown_10[0x20 - 0x10];
};

struct PieceInfo_00458fa0 {
    char unknown_0[4];
    int vertexCount;                 // +0x4
    int faceCount;                   // +0x8
    int field_c;                     // +0xc -1 skips the first face
    char unknown_10[0x28 - 0x10];
    Face_00458fa0* faces;            // +0x28
};

#pragma pack(push, 1)
struct Piece_00458310 {
    PieceInfo_00458fa0* info;         // +0x0
    char unknown_4[0x22 - 0x4];
    Vertex_0045a610* vertices;        // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;              // +0x28
    char unknown_29[0x36 - 0x29];
};

struct Map_00458fa0 {
    char unknown_0[0x241];
    unsigned int unknown_241 : 30;
    unsigned int brightFaces : 1;   // +0x241 bit 30, the wider field
    unsigned int unknown_242 : 1;
};

struct Owner_00458fa0 {
    char unknown_0[0x92];
    Map_00458fa0* map;               // +0x92
};

struct Model_00458fa0 {
    int pieceCount;                   // +0x0
    char unknown_4[0xc - 0x4];
    Owner_00458fa0* owner;            // +0xc
    char unknown_10[0x22 - 0x10];
    Piece_00458310 pieces[1];         // +0x22
};
#pragma pack(pop)

void __stdcall FUN_004c0820(View_0045a610* view, Vertex_0045a610* points, int count, int param_4);

// A method that ignores `this`: its one caller (0x458dd0) passes its own
// `this` through in ecx.
class Class_00458fa0 {
public:
    void FUN_00458fa0(View_0045a610* view, Model_00458fa0* model, int param_3);
};

// FUNCTION: 0x458fa0
void Class_00458fa0::FUN_00458fa0(View_0045a610* view, Model_00458fa0* model, int param_3)
{
    Vertex_0045a610 verts[2000];
    Vertex_0045a610 tmp[25];
    int faceno = 0;
    int i = model->pieceCount - 1;
    if (i >= 0) {
        Piece_00458310* piece = &model->pieces[i];
        while (i >= 0) {
            if (piece->flags & 1) {
                PieceInfo_00458fa0* info = piece->info;
                Vertex_0045a610* v = piece->vertices;
                for (int j = 0; j < info->vertexCount; j++) {
                    int bright = model->owner->map->brightFaces ? 125 : 50;
                    int x = (short)(v->x >> 16);
                    int y = (short)(v->y >> 16);
                    int z = (short)(-v->z >> 16);
                    verts[j].x = x;
                    verts[j].y = z - (y >> 1);
                    verts[j].y += view->field_6;
                    verts[j].z = y + bright;
                    verts[j].x += view->field_4;
                    v++;
                }
                Face_00458fa0* f = info->faces;
                if (info->field_c != -1) {
                    f++;
                    faceno = 1;
                } else {
                    faceno = 0;
                }
                for (; faceno < info->faceCount; faceno++, f++) {
                    unsigned short* ip = f->indices;
                    int k = 0;
                    for (; k < f->count; k++, ip++) {
                        tmp[k] = verts[*ip];
                    }
                    tmp[k] = tmp[0];
                    FUN_004c0820(view, tmp, f->count + 1, param_3);
                }
            }
            piece--;
            i--;
        }
    }
}
