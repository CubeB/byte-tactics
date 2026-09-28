// Decompiled by GPT-6-Luna, finished by Space Bunny Free. Names are provisional.
// PARTIAL, 81.9% (406 of 427 bytes; up from 54.0%). Read the caveat below
// before trusting the percentage: the shape that scores 81.9% is one I believe
// is a miscompilation of the source's intent, and I have recorded it as such
// rather than presenting the number as progress toward the original.
//
// WHAT IS SOLVED. The piece array starts at list+0x22, not +0x44, with `info`
// at piece+0, `vertices` at +0x22 and `flags` at +0x28 on a 0x36 stride, under
// `#pragma pack(push,1)`. The `Vec3` really is a local, materialised at
// frame+0x1c and passed by pointer to 0x4584d0, while 0x459200 takes it *by
// value*, and getting that parameter order right is what produces the exact
// `sub esp,0xc / mov edx,esp / push list` interleaving. The two flag conditions
// must be written as inline `owner->flags & 0x20000000` expressions rather than
// through an `int special` local: the local lets MSVC common the two uses, and
// the inline form is what forces the spill to frame+0x18 and the reload for the
// second test, which also fixes the frame at 0x18 and puts the bitmap in ebx and
// the flags in edx (`test dh,0x20`). The prologue then falls out of two changes
// together: moving `int rebuild = 0;` above the two `g_game` reads *and* deleting
// the second `bitmap = list->bitmap;`. That reassignment alone was flipping the
// whole function's allocation, and together they give the original's
// esi=x, ebp=z, edi=list, ecx=owner exactly. `kind` is `unsigned char`, and so
// is the sixth parameter of 0x4584d0, which drops the `xor ecx,ecx` before the
// byte load. The `visible` decision needs `int t = (unsigned char)~owner->field_10e;
// if (t & 1) ... else ...`, which is the only spelling of about sixteen that emits
// the extra `and eax,0xff` in the `not al` sequence.
//
// THE CAVEAT, and the main remaining puzzle. The original reloads `list->bitmap`
// into ecx *after* the 0x458586a0 rebuild call, which frees ebx and forces the
// loop counter into the (by then dead) `this` stack slot rather than ebx. The
// reload family is semantically correct and reproduces the tail exactly, but it
// scores 69.4% because the extra load destroys the prologue's x-before-list
// order. The 81.9% file has no reload, so the pre-call bitmap value is live
// across the call (`test ebx,ebx` in the tail) and MSVC reloads `this` from two
// different stack slots, one of which holds the `special` value. That is a
// miscompilation of this source's intent, and it is what the byte shape at 81.9%
// requires. I would not read 81.9% as "close to the original": it is a
// different shape that scores well, and the semantically right one scores 12
// points lower. Roughly thirty shape knobs could not get the reload and the
// correct prologue at the same time, and the statement-order knobs are all
// exhausted (logs in build/scratch/458810/s3.txt and s5.txt).
//
// The next thing to try is therefore NOT another statement-order knob. It is to
// make the tail use a second `List_458810*` copy, or to move the reload so the
// bitmap's live range ends before the call *without adding a value*, for example
// by re-reading through a pointer-to-field held in a callee-saved register. The
// real source very likely re-reads `list->bitmap` after the rebuild, and the
// prologue's `esi = x` before `edi = list` has to survive that.
//
// Also still different: the duplicated `test eax, eax; jne` after
// `mov eax, [ebx+0x14]`, about 3 bytes, four spellings tried with no result; and
// `coords.y` being read from the arg2 slot at [esp+0x30] rather than its own slot
// at [esp+0x20].
//
// No suspected bug in the original. The duplicated `test eax, eax` is redundant
// and `local_8` (an uninitialised y) is read from a slot the frame never writes,
// but both look like ordinary MSVC artefacts of how the source was written rather
// than mistakes by Cavedog.
extern char* g_game;

struct Vertex_458810 { int x; int y; int z; };

struct Owner_458810 {
    void* relation;               // +0x00
    char unknown_4[0x1c];
    int field_20;                 // +0x20
    char unknown_24[0xff - 0x24];
    unsigned char kind;           // +0xff
    char unknown_100[4];
    float intensity;              // +0x104
    char unknown_108[6];
    unsigned char field_10e;      // +0x10e
    char unknown_10f;
    unsigned int flags;           // +0x110
    unsigned char field_114;      // +0x114
    char unknown_115[3];
};

struct Bitmap_458810 {
    char unknown_0[0x14];
    int field_14;                 // +0x14
};

struct PieceInfo_458810 { char unknown_0[4]; int vertexCount; };

#pragma pack(push, 1)
struct Piece_458810 {
    PieceInfo_458810* info;       // +0x00
    char unknown_4[0x1e];
    Vertex_458810* vertices;      // +0x22
    char unknown_26[2];
    unsigned char flags;          // +0x28
    char unknown_29[0xd];
};

struct List_458810 {
    int pieceCount;               // +0x00
    int frame;                    // +0x04
    char unknown_8[4];
    Owner_458810* owner;          // +0x0c
    Bitmap_458810* bitmap;        // +0x10
    int field_14;                 // +0x14
    char unknown_18[0x22 - 0x18];
    Piece_458810 pieces[1];       // +0x22
};
#pragma pack(pop)

struct Vec3_458810;

class Class_004584d0 {
public:
    void FUN_004584d0(List_458810* list, Vec3_458810* param_2, void* param_3,
        PieceInfo_458810* info, Vertex_458810* vertices, unsigned char kind, int visible);
};

class Class_00459200 {
public:
    void FUN_00459200(void* param_1, List_458810* list, Vec3_458810 coords, int visible);
};

class Class_004581e0 {
public:
    int FUN_004586a0(List_458810* list, int param_2, int param_3);
    void FUN_00458810(List_458810* list, Vec3_458810* result);
};

struct Vec3_458810 { int x; int y; int z; };

// FUNCTION: 0x458810
void Class_004581e0::FUN_00458810(List_458810* list, Vec3_458810* result)
{
    int rebuild = 0;
    int x = *(int*)(g_game + 0x1431f) << 16;
    int z = *(int*)(g_game + 0x14323) << 16;
    int visible;
    Owner_458810* owner = list->owner;
    if (owner->flags & 0x20000000) {
        int t = (unsigned char)~owner->field_10e;
        if (t & 1)
            visible = 1;
        else
            visible = 0;
    } else {
        visible = *(int*)((char*)owner->relation + 0x20) == 0;
    }
    if (list->frame == 0)
        rebuild = 1;
    Bitmap_458810* bitmap = list->bitmap;
    if (bitmap == 0)
        list->field_14 = 0;
    if ((owner->flags & 0x20000000) != 0) {
        if (bitmap == 0
            || (owner->intensity != 0.0f
                && (owner->flags & 0x2000) != 0
                && bitmap->field_14 == 0))
            rebuild = 1;
    }
    if (bitmap == 0 && (owner->flags & 0x20000000) != 0)
        rebuild = 1;
    if ((owner->field_114 & 1) != 0 && bitmap == 0)
        rebuild = 1;
    if (rebuild) {
        list->field_14 = 0;
        FUN_004586a0(list, 0, 1);
    }
    Vec3_458810 coords;
    int local_8;                  // uninitialised, as in the original's stack slot
    coords.x = x;
    coords.y = local_8;
    coords.z = z;
    if (bitmap != 0) {
        ((Class_00459200*)this)->FUN_00459200(result, list, coords, visible);
        list->frame++;
        return;
    }
    for (int i = list->pieceCount - 1; i >= 0; i--) {
        Piece_458810* piece = &list->pieces[i];
        if (piece->flags & 1) {
            unsigned char kind = list->owner->kind;
            ((Class_004584d0*)this)->FUN_004584d0(list, result, &coords, piece->info,
                piece->vertices, kind, visible);
        }
    }
    list->frame++;
}
