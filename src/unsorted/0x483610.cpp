// Decompiled by Claude Sonnet 5.5 (skeleton), regions by DeepSeek V4.1 Flash.
// Names are provisional.
// Map loader: reads a TNT map (version 0x1020 or 0x2000) into the game state
// and allocates the tile map, plot memory, tile set, sort lists and the
// mapped and eyeball memory blocks.
//
// STATUS: MATCH (1975/1975 bytes). `uv run tools/check.py 0x483610` prints
// MATCH. Instruction order was fixed by: moving attr_b ahead of attr_a in the
// 0x1020 case, hoisting the attr_b plot pointer above the count test, moving
// `b.n = a.n` inside the attr_a count test, giving the attr_a second loop its
// own `int n` counter, inverting the zero-size test around operator new, and
// declaring r4's `int cols, rows;` in that order while assigning rows first.
// Two constant multiplies survive only because `(x * 65536.0) * 0.0011111111111111111`
// is parenthesised; without it the compiler folds them into one.

// SHARED begin
#include <string.h>
extern int DAT_00511de8;
extern char DAT_0051e6a0;

struct TntHeader_00483610 {
    unsigned short width;
    unsigned short height;
    unsigned short pad0;
    unsigned short pad1;
    unsigned char flag[4];
    int zero0;
    unsigned short* data;
    int zero1;
};

struct MapInfo_00483610 {
    int version;
    int width;
    int height;
    int flag;
    int sea_a;
    int sea_b;
    int sea_d;
    int tile_count;
    int map_size;
    int* tile_set_src;
    unsigned int tile_set_count;
    int* tile_map_src;
    unsigned char* attr_b;
    unsigned char* attr_a;
    int attr_limit;
    unsigned int feature_flags;
    unsigned short* feature_data;
};

struct Point_00483610 {
    short x;
    short y;
};

union Slot_00483610 {
    int n;
    Point_00483610 p;
};

class Class_004356c0 {
public:
    int* FUN_004356c0(int index);
};

class Class_00433130 {
public:
    void FUN_00433130();
};

int* __stdcall FUN_00429660(int* file);
int __cdecl sprintf(char* buf, const char* fmt, ...);
void __stdcall FUN_004b6290(char* message);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* block);
void* __stdcall FUN_004b8da0(const char* name, int width, int height);
void __stdcall FUN_004b8a80(void* dst, void* src);
void __stdcall FUN_004b7f90(void* surface, void* header, int x, int y);
void __stdcall FUN_00421f20(int* info);
void* __stdcall FUN_00423c50(void* target, unsigned short id, void* pos, void* field_64, unsigned char owner);
void __stdcall FUN_00423160();
void __stdcall FUN_00483210(Point_00483610 pos, Point_00483610 size);
void FUN_00482c20();
void FUN_004833b0();
void FUN_00422040();
void* operator new(unsigned int size);
void operator delete(void* p);
// SHARED end

// FUNCTION: 0x483610
void FUN_00483610()
{
    int* tmp0 = (int*)(DAT_00511de8 + 0x141fb);
    MapInfo_00483610 info;
    TntHeader_00483610 pic;
    char text[64];
    Slot_00483610 a;
    Slot_00483610 b;
    int* tnt;

    // REGION r1 begin
    tnt = ((Class_004356c0*)*(void**)(DAT_00511de8 + 0x391e9))->FUN_004356c0(1);
    tnt = FUN_00429660(tnt);
    info.version = *tnt;
    switch (info.version) {
    case 0x1020:
        info.width = tnt[1];
        info.height = tnt[2];
        info.flag = tnt[9];
        info.sea_d = tnt[0xd];
        info.sea_a = tnt[10];
        info.sea_b = tnt[0xb];
        info.tile_count = tnt[7];
        info.map_size = tnt[8] + (int)tnt;
        info.tile_set_count = tnt[6];
        info.tile_set_src = (int*)(tnt[5] + (int)tnt);
        info.tile_map_src = (int*)(tnt[3] + (int)tnt);
        info.attr_b = (unsigned char*)(tnt[4] + (int)tnt);
        info.attr_a = 0;
        info.attr_limit = 0xfc;
        info.feature_flags = info.feature_flags ^ ((tnt[0xf] ^ info.feature_flags) & 1);
        info.feature_data = (unsigned short*)(tnt[0xe] + (int)tnt);
        break;
    case 0x2000:
        info.width = tnt[1];
        info.height = tnt[2];
        info.flag = tnt[9];
        info.sea_d = 0;
        info.sea_a = 100;
        info.sea_b = 2000;
        info.tile_count = tnt[7];
        info.map_size = tnt[8] + (int)tnt;
        info.tile_set_count = tnt[6];
        info.tile_set_src = (int*)(tnt[5] + (int)tnt);
        info.tile_map_src = (int*)(tnt[3] + (int)tnt);
        info.attr_a = (unsigned char*)(tnt[4] + (int)tnt);
        info.attr_b = 0;
        info.attr_limit = 0xfffb;
        info.feature_flags = info.feature_flags ^ ((tnt[0xb] ^ info.feature_flags) & 1);
        info.feature_data = (unsigned short*)(tnt[10] + (int)tnt);
        break;
    default:
        sprintf(text, "Unknown TNT version:  0x%08x", info.version);
        FUN_004b6290(text);
        break;
    }
    // REGION r1 end

    // REGION r2 begin
    a.n = *(int*)(*(int*)(DAT_00511de8 + 0x391e9) + 0xd34);
    if (a.n >= 0 && info.version >= 0x2000)
        *(int*)((char*)tmp0 + 0x60) = a.n;
    else
        *(int*)((char*)tmp0 + 0x60) = info.sea_a;
    a.n = *(int*)(*(int*)(DAT_00511de8 + 0x391e9) + 0xd38);
    if (a.n >= 0 && info.version >= 0x2000)
        *(int*)((char*)tmp0 + 0x64) = a.n;
    else
        *(int*)((char*)tmp0 + 0x64) = info.sea_b;
    a.n = *(int*)(*(int*)(DAT_00511de8 + 0x391e9) + 0xd3c);
    if (a.n >= 0 && info.version >= 0x2000)
        *(int*)((char*)tmp0 + 0x68) = (int)((a.n * 65536.0) * 0.0011111111111111111);
    else if (info.sea_d != 0)
        *(int*)((char*)tmp0 + 0x68) = (int)((info.sea_d * 65536.0) * 0.0011111111111111111);
    else
        *(int*)((char*)tmp0 + 0x68) = 0x1fdb;
    if (*(float*)(*(int*)(DAT_00511de8 + 0x391e9) + 0xd40) >= 0.0f)
        *(int*)((char*)tmp0 + 0x6c) = *(int*)(*(int*)(DAT_00511de8 + 0x391e9) + 0xd40);
    else
        *(int*)((char*)tmp0 + 0x6c) = 0x3f000000;
    *(unsigned char*)((char*)tmp0 + 0x84) = (unsigned char)info.flag;
    *(int*)((char*)tmp0 + 0x38) = info.width;
    *(int*)((char*)tmp0 + 0x3c) = info.height;
    tmp0[10] = tmp0[14] << 4;
    tmp0[11] = tmp0[15] << 4;
    if (info.feature_flags & 1) {
        pic.width = *info.feature_data;
        pic.height = info.feature_data[2];
        pic.pad0 = 0;
        pic.pad1 = 0;
        pic.flag[0] = 0;
        pic.flag[1] = 0;
        pic.flag[2] = 0;
        pic.flag[3] = 0;
        pic.zero0 = 0;
        pic.data = info.feature_data + 4;
        pic.zero1 = 0;
        *(void**)(DAT_00511de8 + 0x1426b) = FUN_004b8da0("TED GENERATED PIC", *(int*)info.feature_data, *(int*)(info.feature_data + 2));
        FUN_004b8a80(text, *(void**)(DAT_00511de8 + 0x1426b));
        FUN_004b7f90(text, &pic, 0, 0);
    } else {
        *(int*)(DAT_00511de8 + 0x1426b) = 0;
    }
    // REGION r2 end

    // REGION r3 begin
    a.n = (tmp0[10] / 32) * (tmp0[11] / 32);
    int* dst = (int*)FUN_004d83b0("TILE MAP", a.n * 2);
    tmp0[36] = (int)dst;
    memcpy(dst, info.tile_map_src, a.n * 2);
    a.n = tmp0[14] * tmp0[15];
    unsigned char* plot = (unsigned char*)FUN_004d83b0("PLOT MEMORY", a.n * 0xd);
    tmp0[35] = (int)plot;
    int fill = *(int*)(*(int*)(DAT_00511de8 + 0x391e9) + 0xd30);
    if (fill < 0 || info.version < 0x2000)
        fill = 0;
    for (int i = a.n; i > 0; i--) {
        plot[0xc] &= 0xfc;
        *(unsigned short*)plot = 0;
        *(unsigned short*)(plot + 2) = 0;
        *(unsigned short*)(plot + 8) = 0xffff;
        plot[7] = (unsigned char)fill;
        plot += 0xd;
    }
    FUN_00421f20(&info.version);
    if (info.attr_b != 0) {
        unsigned char* q = *(unsigned char**)&tmp0[35];
        if (a.n > 0) {
            unsigned char* src = info.attr_b;
            for (int i = a.n; i > 0; i--) {
                q[4] = *src;
                q[7] = src[6];
                q[0xc] = (q[0xc] & 0xd7) | 0x50;
                q += 0xd;
                src += 8;
            }
        }
        if (*(int*)(DAT_00511de8 + 0x38d6b) == 0) {
            q = *(unsigned char**)&tmp0[35];
            if (a.n > 0) {
                unsigned char* src = info.attr_b + 2;
                do {
                    if (*src < info.attr_limit)
                        FUN_00423c50(q, *src, 0, 0, 10);
                    q += 0xd;
                    src += 8;
                    a.n--;
                } while (a.n != 0);
            }
        }
    } else {
        if (info.attr_a != 0) {
            unsigned char* q = *(unsigned char**)&tmp0[35];
            unsigned char* src = info.attr_a;
            if (a.n > 0) {
                b.n = a.n;
                do {
                    q[4] = *src;
                    q[0xc] = (q[0xc] & 0xd7) | 0x50;
                    if (*(unsigned short*)(src + 1) == 0xfffc)
                        FUN_00423c50(q, 0xfffc, 0, 0, 10);
                    q += 0xd;
                    src += 4;
                    b.n--;
                } while (b.n != 0);
            }
            if (*(int*)(DAT_00511de8 + 0x38d6b) == 0) {
                q = *(unsigned char**)&tmp0[35];
                if (a.n > 0) {
                    unsigned short* sp = (unsigned short*)(info.attr_a + 1);
                    int n = a.n;
                    do {
                        if ((int)*sp < info.attr_limit)
                            FUN_00423c50(q, *sp, 0, 0, 10);
                        q += 0xd;
                        sp += 2;
                        n--;
                    } while (n != 0);
                }
                FUN_00423160();
            }
        }
    }
    // REGION r3 end

    // REGION r4 begin
    unsigned int* set = (unsigned int*)FUN_004d83b0("TILE SET", info.tile_set_count * 0x400 + 8);
    *(unsigned int**)((char*)tmp0 + 0x88) = set;
    *set = info.tile_set_count;
    *(int*)(*(int*)((char*)tmp0 + 0x88) + 4) = *(int*)((char*)tmp0 + 0x88) + 8;
    memcpy(*(void**)(*(int*)((char*)tmp0 + 0x88) + 4), info.tile_set_src, info.tile_set_count * 0x400);
    FUN_004d85a0(tnt);
    ((Class_00433130*)&DAT_0051e6a0)->FUN_00433130();
    int mw = *(int*)(DAT_00511de8 + 0x37e37);
    int mh = *(int*)(DAT_00511de8 + 0x37e3b);
    *(int*)((char*)tmp0 + 0x40) = mw / 16;
    *(int*)((char*)tmp0 + 0x44) = mh / 16;
    *(int*)((char*)tmp0 + 0x48) = mw / 32;
    *(int*)((char*)tmp0 + 0x4c) = mh / 32;
    int* list = (int*)operator new(0x10);
    int* obj = 0;
    if (list != 0) {
        list[1] = 0;
        list[2] = 0;
        list[3] = 0;
        list[0] = 0;
        obj = list;
    }
    int rb = 2;
    *(int**)((char*)tmp0 + 0x24) = obj;
    a.n = 2;
    if (mw % 32 != 0)
        a.n = 3;
    if (mh % 32 != 0)
        rb = 3;
    int cols, rows;
    rows = *(int*)((char*)tmp0 + 0x44) / 2 + rb;
    cols = *(int*)((char*)tmp0 + 0x40) / 2 + a.n;
    obj[1] = cols;
    obj[2] = rows;
    operator delete((void*)obj[0]);
    unsigned int total = (rows * cols + 7U) & 0xfffffff8;
    obj[3] = total;
    int buf;
    if (total != 0)
        buf = (int)operator new(total * 2);
    else
        buf = 0;
    obj[0] = buf;
    *(unsigned short*)(DAT_00511de8 + 0x14281) &= 0xfff7;
    b.p.x = 0;
    b.p.y = 0;
    a.p.x = *(short*)(DAT_00511de8 + 0x14233);
    a.p.y = *(short*)(DAT_00511de8 + 0x14237);
    FUN_00483210(b.p, a.p);
    // REGION r4 end

    // REGION r5 begin
    FUN_00482c20();
    FUN_004833b0();
    unsigned int total2 = (unsigned int)(tmp0[14] * tmp0[15]) * 2;
    unsigned int half = total2 / 4;
    int* mapped = (int*)FUN_004d83b0("MAPPED MEMORY", half);
    tmp0[30] = (int)mapped;
    memset(mapped, 0, half);
    int sx = tmp0[16] + 0xc;
    int sy = tmp0[17] + 0x20;
    tmp0[21] = sy;
    tmp0[20] = sx;
    *tmp0 = (int)FUN_004d83b0("SORT UNIT LIST", sy * sx * 4);
    tmp0[1] = (int)FUN_004d83b0("SORT INDICES", tmp0[21] << 2);
    tmp0[2] = (int)FUN_004d83b0("SORT LINE COUNT", tmp0[21] << 1);
    FUN_00422040();
    *(int*)(DAT_00511de8 + 0x14277) = 0;
    *(int*)(DAT_00511de8 + 0x1427b) = (int)FUN_004d83b0("EYEBALL MEMORY", 0x2d0);
    tmp0[23] = 0;
    *(unsigned char*)(DAT_00511de8 + 0x38d70) = 100;
    // REGION r5 end
}
