// Decompiled by Space Bunny Free, space-bunny-free, GPT-6.1-sol, deepseek-v4.1-flash, mimo-v2.6-pro, Claude Opus 5.5, Opus, DeepSeek V4.1 Flash, Sonnet and Haiku. Names are provisional.
//
// The PCX and image file code and what sits beside it in the executable: the
// PCX reader and writer, the screenshot and BMP writers, the helpers for
// the 3DO object trees, and the exception filter that restores the display.
#include <string.h>

struct PCX_004caa40 {
    unsigned char* data;     // +0x0 the decoded 8-bit pixels
    unsigned char* palette;  // +0x4 the 0x300 byte colour map
    int width;               // +0x8
    int height;              // +0xc
};

int __stdcall HAPI_readfromfile(void* file, void* buf, int size);
void __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall HAPI_FileLength(void* file);
void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);

// Static inline: the only shape that puts x, b, run, then p in the first four
// stack slots.
static inline void FUN_row(void* file, unsigned char* p, int x)
{
    unsigned char b;
    unsigned char run;
    while (x > 0) {
        HAPI_readfromfile(file, &b, 1);
        if ((b & 0xc0) == 0xc0) {
            run = b & 0x3f;
            x -= run;
            if (x < 0)
                run += x;             // a run longer than the row is cut short
            HAPI_readfromfile(file, &b, 1);
            if (run == 1) {
                *p = b;
                p++;
            } else {
                memset(p, b, run);
                p += run;
            }
        } else {
            *p = b;
            p++;
            x--;
        }
    }
}

// Reads the body of a PCX file (the 0x80-byte header starts with 0x0a 0x05) into
// the caller's struct: the 8-bit RLE decoded pixels, the 256-entry packed RGB
// colour map that sits in the last 0x300 bytes of the file, and the width and
// height from the header's Xmin/Ymin/Xmax/Ymax words. The four output fields
// are cleared first, and a header that does not match leaves them all zero.
// The colour map is read by seeking back 0x300 from the end of the file (that
// is ftell, not a size), so the body is decoded from the 0x80 byte header on.
// FUNCTION: 0x4caa40
int __stdcall DecodePcx(void* file, PCX_004caa40* pcx)
{
    unsigned char header[0x80];
    memset(pcx, 0, 16);
    if (HAPI_readfromfile(file, header, 0x80) == 0x80 && header[0] == 0x0a && header[1] == 5) {
        pcx->width = *(unsigned short*)(header + 8) - *(unsigned short*)(header + 4) + 1;
        pcx->height = *(unsigned short*)(header + 10) - *(unsigned short*)(header + 6) + 1;
        pcx->data = (unsigned char*)GameAllocIgnoreTag("PCX BODY", pcx->width * pcx->height);
        pcx->palette = (unsigned char*)GameAllocIgnoreTag("COLOR MAP", 0x300);
        HAPI_SeekFile(file, HAPI_FileLength(file) - 0x300);
        HAPI_readfromfile(file, pcx->palette, 0x300);
        HAPI_SeekFile(file, 0x80);
        {
            unsigned char* p = pcx->data;
            int rows;
            int w;
            w = pcx->width;
            // Counts down from height - 1 to 0, not from height to 1.
            for (rows = pcx->height - 1; rows >= 0; rows--) {
                FUN_row(file, p, w);
                p += w;
            }
        }
        return 1;
    }
    return 0;
}

#include <stdio.h>

struct FileHandle {
    FILE* file;
    int error;
};

extern void* __stdcall HAPI_OpenFileAppend(void* thing);
extern void __stdcall HAPI_CloseFile(void* file);
extern unsigned int __stdcall HAPI_WriteFile(void* file, void* buf, unsigned int size);

// PCX file header (128 bytes).
struct Header_004cac40 {
    unsigned char a;            // manufacturer
    unsigned char b;            // version
    unsigned char c;            // encoding
    unsigned char d;            // bits per pixel
    unsigned short e;           // xmin
    unsigned short f;           // ymin
    short g;                    // xmax
    short h;                    // ymax
    short i;                    // horizontal resolution
    short j;                    // vertical resolution
    unsigned char palette[0x30];
    unsigned char k;            // reserved
    unsigned char l;            // planes
    unsigned short m;           // bytes per line
    unsigned short n;           // palette info
    unsigned char rest[0x3a];
};

// Writes a PCX file: opens `filename` through HAPI_OpenFileAppend, writes the 128
// byte header (manufacturer 10, version 5, RLE encoding, 8 bits per pixel,
// the first 0x30 bytes of `block` as the 16-colour palette, one plane),
// run length compresses `height` rows of `width` bytes from `data`, then
// writes the 0x0c marker byte and the 0x300 byte VGA palette `block`.
// Returns 1 only if the header and the palette went out in full.
// FUNCTION: 0x4cac40
int __stdcall WritePcx(void* filename, unsigned char* data, int width, int height, unsigned char* block)
{
    Header_004cac40 hdr;
    FileHandle* file = (FileHandle*)HAPI_OpenFileAppend(filename);
    int total;
    int rows;
    int n;
    unsigned char* row;
    unsigned char* p;
    int run;
    int wrote;
    // One cur variable, no separate register and memory copies.
    unsigned char cur;
    unsigned char next;
    unsigned char rep;
    unsigned char lit;
    unsigned char cnt;

    if (file == 0)
        goto out;

    memset(&hdr, 0, 0x80);
    hdr.a = 10;
    hdr.b = 5;
    hdr.c = 1;
    hdr.d = 8;
    hdr.e = 0;
    hdr.f = 0;
    hdr.g = (short)(width - 1);
    hdr.h = (short)(height - 1);
    hdr.i = (short)width;
    hdr.j = (short)height;
    memcpy(hdr.palette, block, 0x30);
    hdr.n = 0;
    hdr.l = 1;
    hdr.m = (unsigned short)width;
    if (HAPI_WriteFile(file, &hdr, 0x80) != 0x80)
        goto out;

    rows = height - 1;
    row = data;
    if (rows >= 0) {
        ++rows;
        do {
            // t is declared in the row loop, not at function scope: shares its slot.
            unsigned char t;
            // total = 0 comes before cur = *row: keeps cur's spill store after it.
            total = 0;
            cur = *row;
            run = 1;
            p = row + 1;
            if (width > 1) {
                n = width - 1;
                do {
                    next = *p++;
                    if (cur == next) {
                        run++;
                    } else {
                        wrote = 0;
                        if (run == 1 && (cur & 0xc0) != 0xc0) {
                            lit = cur;
                            HAPI_WriteFile(file, &lit, 1);
                            wrote = run;
                        } else {
                            while (run > 0) {
                                int chunk = run > 0x3f ? 0x3f : run;
                                cnt = (unsigned char)(chunk | 0xc0);
                                HAPI_WriteFile(file, &cnt, 1);
                                rep = cur;
                                HAPI_WriteFile(file, &rep, 1);
                                run -= chunk;
                                wrote += 2;
                            }
                        }
                        total += wrote;
                        cur = next;
                        run = 1;
                    }
                } while (--n);
            }
            if (run == 1 && (cur & 0xc0) != 0xc0) {
                t = cur;
                HAPI_WriteFile(file, &t, 1);
            } else {
                // Declared in this else block: shares a slot with next.
                unsigned char ocnt;
                while (run > 0) {
                    int chunk = run > 0x3f ? 0x3f : run;
                    ocnt = (unsigned char)(chunk | 0xc0);
                    HAPI_WriteFile(file, &ocnt, 1);
                    rep = cur;
                    HAPI_WriteFile(file, &rep, 1);
                    run -= chunk;
                }
            }
            row += width;
        } while (--rows);
    }

    {
        // Own variable in its own block: shares a slot with t.
        unsigned char marker = 0x0c;
        HAPI_WriteFile(file, &marker, 1);
    }
    if (HAPI_WriteFile(file, block, 0x300) != 0x300)
        goto out;
    HAPI_CloseFile(file);
    return 1;

out:
    if (file)
        HAPI_CloseFile(file);
    return 0;
}

#include <windows.h>

// An image: the surface header that LoadPcx fills and the display hands out.
struct Bitmap_004caec0 {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
    unsigned char* data;               // +0xc
};

// The display object: its window, the surface in use and the palette.
#pragma pack(push, 1)
struct Game_004caec0 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    char unknown_44[0xbc - 0x44];
    Bitmap_004caec0* bitmap;           // +0xbc
    char unknown_c0[0x1c];
    int field_dc;                      // +0xdc
    char unknown_e0[0x1b2 - 0xe0];
    int field_1b2;                     // +0x1b2
    char unknown_1b6[0x214 - 0x1b6];
    PALETTEENTRY palette[256];         // +0x214
};
#pragma pack(pop)

Game_004caec0* GetDisplay();

// Saves a bitmap as an image file with the current palette: the game's
// PALETTEENTRY table at +0x214 is converted to packed RGB triples first.
// FUNCTION: 0x4caec0
void __stdcall SaveSurfacePcx(char* name, Bitmap_004caec0* bitmap)
{
    unsigned char pal[256 * 3];
    Game_004caec0* game = GetDisplay();
    for (int i = 0; i < 256; i++) {
        pal[i * 3] = game->palette[i].peRed;
        pal[i * 3 + 1] = game->palette[i].peGreen;
        pal[i * 3 + 2] = game->palette[i].peBlue;
    }
    WritePcx(name, bitmap->data, bitmap->width, bitmap->height, pal);
}

void* __stdcall HAPI_OpenFileRead(char* path);
Bitmap_004caec0* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(void* p);
void __cdecl GameFreeThunk(void* p);

// Loads a PCX file whose header starts with 0x0a 0x05, allocates an image of
// the header's width x height, decodes the body through DecodePcx and copies
// the pixels into the image. When the caller passes a palette buffer it is
// filled with 256 PALETTEENTRY quads (peFlags = 0) expanded from the file's
// 0x300-byte colour map. Returns the image, or 0 when the file is missing, is
// not the expected PCX variant, or the image allocation fails.
// FUNCTION: 0x4caf30
Bitmap_004caec0* __stdcall LoadPcx(char* path, unsigned char* outPalette)
{
    void* file = HAPI_OpenFileRead(path);
    if (file == 0)
        return 0;
    int ok;
    PCX_004caa40 pcx;
    unsigned char header[0x80];
    if (HAPI_readfromfile(file, header, 0x80) != 0x80 || header[0] != 0x0a || header[1] != 5) {
        HAPI_CloseFile(file);
        return 0;
    }
    int width = *(unsigned short*)(header + 8) - *(unsigned short*)(header + 4) + 1;
    int height = *(unsigned short*)(header + 10) - *(unsigned short*)(header + 6) + 1;
    Bitmap_004caec0* image = AllocSurface(path, width, height);
    if (image == 0) {
        HAPI_CloseFile(file);
        return 0;
    }
    HAPI_SeekFile(file, 0);
    ok = DecodePcx(file, &pcx);
    HAPI_CloseFile(file);
    if (ok) {
        memcpy(image->data, pcx.data, height * width);
    }
    if (outPalette != 0) {
        unsigned char* s = pcx.palette;
        for (int i = 0; i < 0x100; i++) {
            outPalette[0] = s[0];
            outPalette[1] = s[1];
            outPalette[2] = s[2];
            outPalette[3] = 0;
            outPalette += 4;
            s += 3;
        }
    }
    GameFreeThunk(pcx.palette);
    GameFreeThunk(pcx.data);
    if (!ok) {
        FreeSurface(image);
    }
    return image;
}

// Loads a PCX file whose header starts with 0x0a 0x05, opens its body through
// DecodePcx (which allocates the pixel data and the 256-entry packed RGB
// palette), expands the palette to 256 PALETTEENTRY quads (peFlags = 0), copies
// them into the caller's buffer and frees the PCX buffers. Returns 1 on
// success, 0 when the file is missing or is not the expected PCX variant.
// FUNCTION: 0x4cb080
int __stdcall LoadPcxPalette(char* path, unsigned int* out)
{
    void* file = HAPI_OpenFileRead(path);
    if (file == 0)
        return 0;
    PCX_004caa40 pcx;
    unsigned char header[0x80];
    PALETTEENTRY pal[0x100];
    if (HAPI_readfromfile(file, header, 0x80) != 0x80) {
        HAPI_CloseFile(file);
        return 0;
    }
    if (header[0] != 0x0a || header[1] != 5) {
        HAPI_CloseFile(file);
        return 0;
    }
    HAPI_SeekFile(file, 0);
    DecodePcx(file, &pcx);
    HAPI_CloseFile(file);
    unsigned char* s = pcx.palette;
    for (int i = 0; i < 0x100; i++) {
        pal[i].peRed = s[0];
        pal[i].peGreen = s[1];
        pal[i].peBlue = s[2];
        pal[i].peFlags = 0;
        s += 3;
    }
    memcpy(out, pal, 0x400);
    GameFreeThunk(pcx.palette);
    GameFreeThunk(pcx.data);
    return 1;
}

// Included only for its symbol ids: AddObjectBounds matches only with them.
#include <io.h>
#include <stdlib.h>

struct FindData_004cb170 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

extern char g_pathSepBackslash[];
extern char DAT_005119b8[];

int __stdcall HAPI_FindFirst(const char* path, FindData_004cb170* fd, int a, int b);
int __stdcall HAPI_FindNext(int handle, FindData_004cb170* fd);
void __stdcall HAPI_FindClose(int handle);

// FUNCTION: 0x4cb170
int __stdcall SaveScreenshot(char* param_1, char* param_2)
{
    Game_004caec0* game = GetDisplay();
    int flag = 0;
    char filename[260];
    FindData_004cb170 fd;
    unsigned char pal[768];
    int best = 0;

    char c = param_1[0];
    if (c != '\0') {
        int len = strlen(param_1);
        if (param_1[len - 1] != '\\')
            flag = 1;
    }
    if (game->field_dc == 0)
        return 0;

    const char* sep = flag ? g_pathSepBackslash : DAT_005119b8;
    sprintf(filename, "%s%s%s*.pcx", param_1, sep, param_2);

    int handle = HAPI_FindFirst(filename, &fd, -1, 1);
    if (handle >= 0) {
        do {
            int n = atoi(fd.name + strlen(param_2));
            if (n > best)
                best = n;
        } while (HAPI_FindNext(handle, &fd) == 0);
        HAPI_FindClose(handle);
    }

    sep = flag ? g_pathSepBackslash : DAT_005119b8;
    sprintf(filename, "%s%s%s%04i.pcx", param_1, sep, param_2, best + 1);

    Bitmap_004caec0* bitmap = game->bitmap;
    Game_004caec0* g = GetDisplay();
    for (int i = 0; i < 256; i++) {
        pal[i * 3] = g->palette[i].peRed;
        pal[i * 3 + 1] = g->palette[i].peGreen;
        pal[i * 3 + 2] = g->palette[i].peBlue;
    }
    return WritePcx(filename, bitmap->data, bitmap->width, bitmap->height, pal);
}

// A 3DO object (0x34 bytes) with its vertices, its primitives and the links to
// the sibling and child objects.
struct Vec3_004cb2f0 {
    int x;
    int y;
    int z;
};

struct Primitive_004cb2f0 {            // 0x20 bytes
    int unknown_0;
    int count;                         // +0x4
    int ptr_8;                         // +0x8
    union {
        int ptr_c;                     // +0xc
        unsigned short* indices;       // +0xc
    };
    int ptr_10;                        // +0x10
    int unknown_14[3];
};

struct Object_004cb2f0 {
    int unknown_0;
    int vertexCount;                   // +0x4
    int count;                         // +0x8 primitives
    int index;                         // +0xc
    int x;                             // +0x10
    int y;                             // +0x14
    int z;                             // +0x18
    int ptr_1c;                        // +0x1c
    int ptr_20;                        // +0x20
    Vec3_004cb2f0* vertices;           // +0x24
    Primitive_004cb2f0* elems;         // +0x28
    Object_004cb2f0* sibling;          // +0x2c
    Object_004cb2f0* child;            // +0x30
};

// Average of the y of the vertices selected by a list of 16-bit indices.
// FUNCTION: 0x4cb2f0
int __stdcall AveragePrimitiveY(Object_004cb2f0* table, Primitive_004cb2f0* list)
{
    int sum = 0;
    int n = list->count;
    unsigned short* p = list->indices;
    for (int i = 0; i < n; i++) {
        sum += table->vertices[*p++].y;
    }
    return sum / n;
}

// FUNCTION: 0x4cb330
void __stdcall SwapPrimitives(Primitive_004cb2f0* param_1, Primitive_004cb2f0* param_2)
{
    Primitive_004cb2f0 tmp = *param_1;
    *param_1 = *param_2;
    *param_2 = tmp;
}

// Moves the element selected by node->index to the front (if any), then bubble
// sorts the remaining elements ascending by the integer average of the y of
// the vertices each element's `count` ushort indices select.
// FUNCTION: 0x4cb370
void __stdcall SortPrimitives(Object_004cb2f0* node)
{
    if (node->index != -1 && node->count > 0) {
        Primitive_004cb2f0* elems = node->elems;
        Primitive_004cb2f0 temp = elems[node->index];
        elems[node->index] = elems[0];
        elems[0] = temp;
        node->index = 0;
    }
    Primitive_004cb2f0* e;
    int i;
    int swapped;
    do {
        swapped = 0;
        e = node->elems + 1;
        for (i = 1; i < node->count - 1; i++, e++) {
            // Pointer cached in a local, counting down: keeps node in ebp.
            unsigned short* q = e->indices;
            int a = 0;
            for (int k = e->count; k > 0; k--)
                a += node->vertices[*q++].y;
            a /= e->count;
            Primitive_004cb2f0* p = e + 1;
            unsigned short* r = p->indices;
            int b = 0;
            for (int j = p->count; j > 0; j--)
                b += node->vertices[*r++].y;
            b /= p->count;
            if (a > b) {
                Primitive_004cb2f0 t = *e;
                *e = *p;
                *p = t;
                swapped = 1;
            }
        }
    } while (swapped);
}

// Relocates a tree of nodes loaded from a file: adds `delta` to each
// (non-null) stored pointer of the node, its element array and, recursively,
// its sibling and child nodes, then hands the node to SortPrimitives.
// Any common header must be included: it makes ptr_20 compile to lea.
// FUNCTION: 0x4cb4c0
void __stdcall RelocateObject(int delta, Object_004cb2f0* node)
{
    if (node->ptr_1c)
        node->ptr_1c += delta;
    if (node->ptr_20)
        node->ptr_20 += delta;
    node->vertices = (Vec3_004cb2f0*)((char*)node->vertices + delta);
    node->elems = (Primitive_004cb2f0*)((char*)node->elems + delta);
    if (node->sibling) {
        node->sibling = (Object_004cb2f0*)((char*)node->sibling + delta);
        RelocateObject(delta, node->sibling);
    }
    if (node->child) {
        node->child = (Object_004cb2f0*)((char*)node->child + delta);
        RelocateObject(delta, node->child);
    }
    // Pointer walking the element array: the loop needs it.
    Primitive_004cb2f0* e = node->elems;
    for (int i = 0; i < node->count; i++, e++) {
        if (e->ptr_8)
            e->ptr_8 += delta;
        e->ptr_c += delta;
        if (e->ptr_10)
            e->ptr_10 += delta;
    }
    SortPrimitives(node);
}

extern void* __stdcall HAPI_LoadFile(void*, int);

// FUNCTION: 0x4cb560
void* __stdcall Load3do(char* param)
{
    void* result = HAPI_LoadFile(param, 0);
    if (result == 0) {
        return 0;
    }
    RelocateObject((int)result, (Object_004cb2f0*)result);
    return result;
}

// Mirrors an object tree: negates x and z of every vertex and of the node's
// own offset, recursing into the sibling and then the child (the child call
// became a loop).
// FUNCTION: 0x4cb590
void __stdcall MirrorObject(Object_004cb2f0* obj)
{
    for (int i = 0; i < obj->vertexCount; i++) {
        obj->vertices[i].x = -obj->vertices[i].x;
        obj->vertices[i].z = -obj->vertices[i].z;
    }
    obj->x = -obj->x;
    obj->z = -obj->z;
    if (obj->sibling)
        MirrorObject(obj->sibling);
    if (obj->child)
        MirrorObject(obj->child);
}

// Walks a list of nodes (and, recursively, their children) and returns the
// largest vertex y plus the owning node's y offset, or 0 if none.
// FUNCTION: 0x4cb5f0
int __stdcall GetObjectHeight(Object_004cb2f0* node)
{
    int best = 0;
    for (; node; node = node->sibling) {
        for (int i = 0; i < node->vertexCount; i++) {
            int v = node->vertices[i].y + node->y;
            if (v > best)
                best = v;
        }
        if (node->child) {
            int v = GetObjectHeight(node->child) + node->y;
            if (v > best)
                best = v;
        }
    }
    return best;
}

void __stdcall AddObjectBounds(Object_004cb2f0* obj, Vec3_004cb2f0* offset,
                            Vec3_004cb2f0* lo, Vec3_004cb2f0* hi, int arg);

// Computes the bounding box of an object tree: clears the two output corners
// and walks the tree from a zero offset.
// FUNCTION: 0x4cb650
void __stdcall GetObjectBounds(Object_004cb2f0* obj, Vec3_004cb2f0* lo,
                            Vec3_004cb2f0* hi, int arg)
{
    Vec3_004cb2f0 offset;
    lo->x = 0;
    lo->y = 0;
    lo->z = 0;
    hi->x = 0;
    hi->y = 0;
    hi->z = 0;
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    AddObjectBounds(obj, &offset, lo, hi, arg);
}

// Expands a bounding box (lo, hi) with the vertices of an object and, if arg is
// set, of its two children, translated by offset.
// FUNCTION: 0x4cb6a0
void __stdcall AddObjectBounds(Object_004cb2f0* obj, Vec3_004cb2f0* offset,
                            Vec3_004cb2f0* lo, Vec3_004cb2f0* hi, int arg)
{
    Vec3_004cb2f0 local;
    local.x = obj->x + offset->x;
    local.y = offset->y + obj->y;
    local.z = obj->z + offset->z;
    if (obj->vertexCount > 2) {
        for (int i = 0; i < obj->vertexCount; i++) {
            Vec3_004cb2f0* v = &obj->vertices[i];
            if (v->x + local.x > hi->x) hi->x = v->x + local.x;
            if (v->x + local.x < lo->x) lo->x = v->x + local.x;
            if (v->y + local.y > hi->y) hi->y = v->y + local.y;
            if (v->y + local.y < lo->y) lo->y = v->y + local.y;
            if (v->z + local.z > hi->z) hi->z = v->z + local.z;
            if (v->z + local.z < lo->z) lo->z = v->z + local.z;
        }
    }
    if (arg != 0) {
        if (obj->child != 0)
            AddObjectBounds(obj->child, &local, lo, hi, arg);
        if (obj->sibling != 0)
            AddObjectBounds(obj->sibling, offset, lo, hi, arg);
    }
}

// The BMP writer: the image size, the offset of the pixel data and the file.
class BmpWriter {
public:
    int width;                  // +0x0
    int height;                 // +0x4
    int dataOffset;             // +0x8
    FILE* file;                 // +0xc

    BmpWriter* Init();
    void Close();
    bool Open(const char* name, int w, int h);
    bool WriteRows(Bitmap_004caec0* image, int x, int rows, int unused_4, int y,
                      int unused_6, int srcY);
};

// FUNCTION: 0x4cb7c0
BmpWriter* BmpWriter::Init()
{
    file = 0;
    return this;
}

// FUNCTION: 0x4cb7d0
void BmpWriter::Close()
{
    if (file != NULL) {
        fclose(file);
    }
}

#pragma pack(push, 2)
struct BmpFileHeader {
    unsigned short bfType;      // +0x0
    unsigned int bfSize;        // +0x2
    unsigned short bfReserved1; // +0x6
    unsigned short bfReserved2; // +0x8
    unsigned int bfOffBits;     // +0xa
};
#pragma pack(pop)

struct BmpInfoHeader {
    unsigned int biSize;        // +0x0
    int biWidth;                // +0x4
    int biHeight;               // +0x8
    unsigned short biPlanes;    // +0xc
    unsigned short biBitCount;  // +0xe
    unsigned int biCompression; // +0x10
    unsigned int biSizeImage;   // +0x14
    int biXPelsPerMeter;        // +0x18
    int biYPelsPerMeter;        // +0x1c
    unsigned int biClrUsed;     // +0x20
    unsigned int biClrImportant;// +0x24
};

struct RgbQuad {
    unsigned char blue;         // +0x0
    unsigned char green;        // +0x1
    unsigned char red;          // +0x2
    unsigned char reserved;     // +0x3
};

// Header plus all 256 palette entries (0x28 + 256 * 4 = 0x428 bytes).
struct BmpInfo {
    BmpInfoHeader header;
    RgbQuad colors[256];
};

// Opens a 256-colour BMP file for writing and emits the BMP file header,
// the BITMAPINFOHEADER and the palette (the game's RGB palette bytes are
// stored red, green, blue and written out reversed as blue, green, red).
// width/height are kept in the object, plus the offset of the pixel data
// (ftell right after the header) and the FILE*. Returns false if the file
// cannot be opened or either header write is short.
// FUNCTION: 0x4cb7f0
bool BmpWriter::Open(const char* name, int w, int h)
{
    void* palbase = GetDisplay();

    width = w;
    height = h;
    file = fopen(name, "wb");
    if (file == NULL) {
        return false;
    }
    BmpFileHeader fh;
    fh.bfType = 0x4d42;
    fh.bfSize = 0;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = 0x436;
    if (fwrite(&fh, sizeof(fh), 1, file) != 1) {
        return false;
    }
    BmpInfo info;
    info.header.biSize = 0x28;
    info.header.biWidth = w;
    info.header.biHeight = h;
    info.header.biPlanes = 1;
    info.header.biBitCount = 8;
    info.header.biCompression = 0;
    info.header.biSizeImage = 0;
    info.header.biXPelsPerMeter = 3000;
    info.header.biYPelsPerMeter = 3000;
    info.header.biClrUsed = 0x100;
    info.header.biClrImportant = 0x100;
    unsigned char* dp = (unsigned char*)info.colors + 1;
    unsigned char* sp = (unsigned char*)palbase + 0x215;
    for (int i = 0; i < 256; i++) {
        dp[1] = sp[-1];
        dp[0] = sp[0];
        dp[-1] = sp[1];
        dp += 4;
        sp += 4;
    }
    if (fwrite(&info, sizeof(info), 1, file) != 1) {
        return false;
    }
    dataOffset = ftell(file);
    return true;
}

// Bitmap file writer: seeks to the file row for `y` and writes `height` rows
// of the image, bottom-up, each padded to a multiple of 4 bytes. Returns
// false when the seek or a write fails.
// FUNCTION: 0x4cb940
bool BmpWriter::WriteRows(Bitmap_004caec0* image, int x, int rows, int unused_4,
                                  int y, int unused_6, int srcY)
{
    int stride = (image->width + 3) & ~3;
    if (fseek(file, (height - rows - y) * stride + dataOffset, SEEK_SET) != 0) {
        return false;
    }
    for (int i = rows - 1; i >= 0; i--) {
        if (fwrite(image->data + image->pitch * (i + srcY), stride, 1, file) != 1) {
            return false;
        }
    }
    return true;
}

// Writes the pixel rows of a bitmap into the file opened by
// BmpWriter::Open, bottom-up, each row padded to 4 bytes.
// Suspected original bug: the seek offset subtracts `n` (the image height the
// caller just passed to Open, which stores it in bmp.height), so
// `bmp.height - n` is always 0 and the seek always lands on dataOffset.
// FUNCTION: 0x4cb9e0
int __stdcall SaveBmp(char* name, Bitmap_004caec0* image)
{
    BmpWriter bmp;
    bool ok;
    bmp.file = 0;
    if (!bmp.Open(name, image->width, image->height)) {
        if (bmp.file) fclose(bmp.file);
        return 0;
    }
    {
        int stride = (image->width + 3) & ~3;
        int n = image->height;
        if (fseek(bmp.file, (bmp.height - n) * stride + bmp.dataOffset, SEEK_SET) != 0) {
            ok = false;
            goto done;
        }
        for (int i = n - 1; i >= 0; i--) {
            if (fwrite(image->data + i * image->pitch, stride, 1, bmp.file) != 1) {
                ok = false;
                goto done;
            }
        }
        ok = true;
    }
done:
    if (!ok) {
        if (bmp.file) fclose(bmp.file);
        return 0;
    }
    if (bmp.file) fclose(bmp.file);
    return 1;
}

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;

void __cdecl ReportException(int param_1, int param_2);
void __stdcall ReleaseDirectDraw(Game_004caec0* app);
void UnlockAllScreens(void);

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (r == 0) {
            DAT_0052a4ec = 0x4d41494e;
            return 0;
        }
        if (DAT_0052a4ec == 0x4d41494e)
            return r;
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
}

static inline void Unlock(LONG held)
{
    if (held == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
}

// Takes the 'MAIN' spin lock (DAT_0052a4e8, owner tag DAT_0052a4ec, event
// DAT_0052a4f0), clears field_1b2 of the display object, pops the screen lock
// stack and releases the display, then restores the window to non-topmost.
// FUNCTION: 0x4cbab0
int __stdcall ExceptionFilter(int param_1, int param_2)
{
    ReportException(param_1, param_2);
    Game_004caec0* app = GetDisplay();
    if (app != 0) {
        LONG held = Lock();
        app->field_1b2 = 0;
        UnlockAllScreens();
        Unlock(held);
        ReleaseDirectDraw(app);
        if (app->hwnd != 0) {
            SetWindowPos(app->hwnd, (HWND)-2, 0, 0, 0, 0, 0x13);
        }
    }
    return 0;
}
