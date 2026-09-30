// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// Best result 90.2% (2183 bytes against 2173). The GUI suffix loop must be written as a do-while
// whose condition re-reads the FUN_004bbc40 result from a local:
//   more = FUN_004bbc40(path); if (more) { suffix++; found = 1; } while (more);
// That stops /O2 from peeling the first iteration; for(;;), while(1) and a goto loop all score
// 86.2 (2240 bytes) because MSVC duplicates the loop body ahead of a rotated loop, and the peeled
// copy also turns `found = 1` into `mov esi, ebx`. With the do-while the body and the tail match
// the original instruction for instruction except that ours re-tests eax at the bottom
// (`test eax, eax / jne`) where the original jumps back unconditionally.
// The compaction keeps the earlier winning shape: keep bit-23-set elements, scan loop and copy
// loop separate, `*d = *s` (not `*d++`, which re-allocates the surrounding blocks, 88.2).
// Remaining: the compaction copy loop spills d to [esp+0x10] and reloads it around every
// operator= call (the original keeps d in edi and stores it once after the loop, about 6 bytes);
// the unit loop keeps g_game in edi, so idiv reads a register where the original reloads g_game
// into ecx at the loop bottom and uses idiv dword ptr [ecx + 0x1438f] (about 2 bytes); and the
// initial compaction guard is inverted (ours jne, the original je).
#include <string.h>
#include <stdio.h>

#pragma pack(push, 1)

class Class_004c2ea0 {
  public:
    int field_0;
    void* current;
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
  public:
    int FUN_004c2f60(char* file);
};

class Class_004c3e10 {
  public:
    void FUN_004c3e10();
};

class Class_004c3410 {
  public:
    int FUN_004c3410(char* name);
};

class Class_004c3240 {
  public:
    void FUN_004c3240();
};

class Class_004c48c0 {
  public:
    int FUN_004c48c0(char* dst, char* key, int size, char* def);
};

class Class_00458160 {
  public:
    char unknown_0[0x14];
    Class_00458160();
};

class Class_00458180 {
  public:
    void FUN_00458180(int size);
};

union UType_0042b370_flags {
    unsigned int value;
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int canbuild : 1;
        unsigned int b7 : 1;
        unsigned int b8 : 1;
        unsigned int b9 : 1;
        unsigned int b10 : 1;
        unsigned int b11 : 1;
        unsigned int b12 : 1;
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int b16 : 1;
        unsigned int b17 : 1;
        unsigned int b18 : 1;
        unsigned int b19 : 1;
        unsigned int b20 : 1;
        unsigned int b21 : 1;
        unsigned int b22 : 1;
        unsigned int hasgui : 1;
        unsigned int b24 : 1;
        unsigned int b25 : 1;
        unsigned int b26 : 1;
        unsigned int b27 : 1;
        unsigned int b28 : 1;
        unsigned int b29 : 1;
        unsigned int b30 : 1;
        unsigned int gui : 1;
    } bits;
};

class Class_0042b370 {
  public:
    char unknown_0[0x20];
    char name[0x60];
    char model[0x20];
    char unknown_a0[0x152 - 0xa0];
    int field_152;
    void* field_156;
    char unknown_15a[0x162 - 0x15a];
    int field_162;
    char unknown_166[0x16e - 0x166];
    int field_16e;
    char unknown_172[0x17a - 0x172];
    int field_17a;
    char unknown_17e[0x18e - 0x17e];
    void* field_18e;
    char unknown_192[0x21e - 0x192];
    unsigned short field_21e;
    char unknown_220[0x22e - 0x220];
    unsigned char field_22e;
    char unknown_22f[0x241 - 0x22f];
    UType_0042b370_flags flags;
    char unknown_245[0x249 - 0x245];

    Class_0042b370& operator=(const Class_0042b370& other);
};

struct Class_00440320 {
    int* field_0;
    short field_4;
    short field_6;
    short field_8;
    short field_a;
    unsigned char field_c;
    unsigned char field_d;
    unsigned char field_e;
    unsigned char field_f;
    int field_10;
    int field_14;
    void* field_18;
    int field_1c;

    void FUN_00440340(void* parser);
};

struct Class_00440290 {
    Class_00440320 entries[32];

    static Class_00440290 DAT_00512358;
};

struct Game_0042d2e0 {
    char unknown_0[0xc];
    void* field_c;
    char unknown_10[0x14377 - 0x10];
    void** field_14377;
    Class_00458160* field_1437b;
    char unknown_1437f[0x1438f - 0x1437f];
    int field_1438f;
    int field_14393;
    int field_14397;
    Class_0042b370* field_1439b;
    char unknown_1439f[0x37e1f - 0x1439f];
    int field_37e1f;
    int field_37e23;
    char unknown_37e27[0x38d71 - 0x37e27];
    unsigned char field_38d71;
};
#pragma pack(pop)

extern Game_0042d2e0* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_0042bf40(char* path, Class_0042b370* type);
void __stdcall FUN_0042a140(void* obj, char* name);
int __stdcall FUN_004bbc40(char* path);
void* __stdcall FUN_004cb560(char* path);
void __stdcall FUN_004cb590(void* obj);
int __stdcall FUN_004cb5f0(void* obj);
void __stdcall FUN_004b6290(const char* msg);
void* __stdcall FUN_004b2450(char* path);
void __stdcall FUN_004bb0f0(char* text);
short __stdcall FUN_00488b10(char* text);
int __cdecl FUN_004d8610(char* name);
void* __cdecl FUN_004d83b0(const char* name, int size);
void __cdecl FUN_004d85a0(void* p);
void __cdecl FUN_004d8780(void* p);
void __cdecl FUN_004d8710(void* p);
void __stdcall FUN_00432fb0(void* start, void* end, void* cmp, int param);
void __stdcall FUN_00432d40(void* start, void* end, void* cmp, int param);
int __stdcall FUN_0042db60(const char* a, const char* b);

// FUNCTION: 0x42d2e0
void FUN_0042d2e0() {
    char namebuf[32];
    char section[32];
    char path[256];
    char classbuf[100];
    char objpath[256];
    char valbuf[256];

    {
        Class_004c2ea0 parser;
        FUN_004290f0(path, "gamedata", "moveinfo", "TDF");
        if (!((Class_004c2f60*)&parser)->FUN_004c2f60(path))
            FUN_004b6290("Can't load MOVEINFO.TDF");

        int i = 0;
        Class_00440320* cls = Class_00440290::DAT_00512358.entries;
        Class_00440320* cls_end = &Class_00440290::DAT_00512358.entries[32];
        do {
            sprintf(classbuf, "CLASS%d", i);
            ((Class_004c3e10*)&parser)->FUN_004c3e10();
            if (((Class_004c3410*)&parser)->FUN_004c3410(classbuf)) {
                ((Class_004c48c0*)parser.current)
                    ->FUN_004c48c0(classbuf, "name", 100, DAT_005119b8);
                cls->field_0 = (int*)FUN_004d8610(classbuf);
                cls->FUN_00440340(&parser);
            }
            cls++;
            i++;
        } while ((int)cls < (int)cls_end);
        ((Class_004c3240*)&parser)->FUN_004c3240();
    }

    Class_00458160* obj = new Class_00458160;
    g_game->field_1437b = obj;

    int t = g_game->field_37e23 * g_game->field_37e1f * 2;
    int v = (int)(t * 1.3);

    int n = *(int*)((char*)g_game->field_c + 0x620) / 0x100000 + 1;
    float scale = 1.0f;
    if (n > 0x10) {
        double d = (double)n * 0.0625;
        if (d > 5.0)
            scale = 5.0f;
        else
            scale = (float)d;
    }
    int size = (int)(v * scale);
    ((Class_00458180*)g_game->field_1437b)->FUN_00458180((size + 0xfff) & 0xfffff000);

    FUN_004d8780(g_game->field_1439b);

    Class_0042b370* end = g_game->field_1439b + g_game->field_1438f;
    Class_0042b370* start = g_game->field_1439b + 1;

    Class_0042b370* p = start;
    Class_0042b370* d;
    if (p == end) {
        d = p;
    } else {
        while (p != end && !(~(p->flags.value) & 0x800000))
            p++;
        d = p;
        if (p != end) {
            for (Class_0042b370* s = p + 1; s != end; s++) {
                if (!(~(s->flags.value) & 0x800000)) {
                    *d = *s;
                    d++;
                }
            }
        }
    }
    g_game->field_1438f = (int)(d - g_game->field_1439b);

    start = g_game->field_1439b + 1;
    Class_0042b370* last = d;
    if (last - start <= 0x10) {
        FUN_00432fb0(start, last, (void*)FUN_0042db60, 0);
    } else {
        FUN_00432d40(start, last, (void*)FUN_0042db60, 0);
        Class_0042b370* q = start + 0x10;
        FUN_00432fb0(start, q, (void*)FUN_0042db60, 0);
        for (; q != last; q++) {
            Class_0042b370 tmp = *q;
            Class_0042b370* r = q - 1;
            Class_0042b370* w = q;
            while (_strcmpi(tmp.name, r->name) < 0) {
                *w = *r;
                w = r;
                r--;
            }
            *w = tmp;
        }
    }

    {
        unsigned short index = 0;
        if (g_game->field_1438f > 0) {
            do {
                g_game->field_1439b[index].field_21e = index;
                index++;
            } while ((int)index < g_game->field_1438f);
        }
    }
    int c = g_game->field_1438f;
    g_game->field_14393 = 0;
    if (c) {
        do {
            c >>= 1;
            g_game->field_14393++;
        } while (c);
    }

    g_game->field_14377 = (void**)FUN_004d83b0("MODEL PTRS", g_game->field_1438f * 4);

    for (unsigned short u = 1; u < g_game->field_1438f; u++) {
        Class_0042b370* type = &g_game->field_1439b[u];
        g_game->field_38d71 = (unsigned char)((u * 100) / g_game->field_1438f);
        type->field_21e = u;
        FUN_004290f0(path, "units", type->name, "FBI");
        if (FUN_004bbc40(path))
            FUN_0042bf40(path, type);

        strncpy(namebuf, type->model, 0x20);
        namebuf[0x1f] = 0;
        FUN_004290f0(objpath, "objects3d", namebuf, "3DO");
        void* model = FUN_004cb560(objpath);
        if (model == 0)
            FUN_004b6290(objpath);
        FUN_004cb590(model);
        FUN_0042a140(model, namebuf);
        g_game->field_14377[u] = model;
        type->field_162 = 0;
        type->field_16e = FUN_004cb5f0(g_game->field_14377[u]);
        type->field_17a = type->field_16e - type->field_162;

        strcpy(namebuf, type->name);
        FUN_004bb0f0(namebuf);
        sprintf(section, "%s0", namebuf);
        FUN_004290f0(path, "guis", section, "GUI");
        if (FUN_004bbc40(path))
            type->flags.bits.gui = 1;
        else
            type->flags.bits.gui = 0;

        int suffix = 1;
        int found = 0;
        int more;
        do {
            sprintf(section, "%s%d", namebuf, suffix);
            FUN_004290f0(path, "guis", section, "GUI");
            more = FUN_004bbc40(path);
            if (more) {
                suffix++;
                found = 1;
            }
        } while (more);
        if (found)
            type->field_22e = suffix;
        else if (type->flags.bits.gui)
            type->field_22e = 1;
        else
            type->field_22e = 0;

        FUN_004290f0(path, "scripts", type->name, "COB");
        type->field_18e = FUN_004b2450(path);
    }

    FUN_004d8710(g_game->field_14377);

    Class_004c2ea0 parser2;
    FUN_004290f0(path, "gamedata", "sidedata", "TDF");
    if (!((Class_004c2f60*)&parser2)->FUN_004c2f60(path)) {
        FUN_004b6290("Can't load GAMEDATA.TDF");
    } else {
        short* list = (short*)FUN_004d83b0("TEMP UTYPE LIST", 0x3c);
        for (unsigned short s = 1; s < g_game->field_1438f; s++) {
            Class_0042b370* type = &g_game->field_1439b[s];
            type->field_152 = 0;
            type->field_156 = 0;
            if (type->flags.bits.canbuild) {
                ((Class_004c3e10*)&parser2)->FUN_004c3e10();
                if (((Class_004c3410*)&parser2)->FUN_004c3410("CANBUILD") &&
                    ((Class_004c3410*)&parser2)->FUN_004c3410(type->name)) {
                    int count = 0;
                    short* out = list;
                    int k = 1;
                    sprintf(objpath, "canbuild%d", k);
                    while (((Class_004c48c0*)parser2.current)
                               ->FUN_004c48c0(valbuf, objpath, 0x20, DAT_005119b8)) {
                        short val = FUN_00488b10(valbuf);
                        if (val != 0) {
                            *out = val;
                            count++;
                            out++;
                        }
                        k++;
                        sprintf(objpath, "canbuild%d", k);
                    }
                    type->field_152 = count;
                }
                sprintf(objpath, "CANBUILD %s", type->name);
                type->field_156 = FUN_004d83b0(objpath, 0x3c);
                memcpy(type->field_156, list, 0x3c);
            }
        }
        FUN_004d85a0(list);
        ((Class_004c3240*)&parser2)->FUN_004c3240();
    }

    g_game->field_38d71 = 100;
    g_game->field_14397 = 1;
    FUN_004d8710(g_game->field_1439b);
}
