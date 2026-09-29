// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads MOVEINFO.TDF (the movement class list), builds the model table,
// then loads GAMEDATA.TDF/sidedata and the per-unit CANBUILD lists.
#include <string.h>
#include <stdio.h>

#pragma pack(push, 1)

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                      // +0x4
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
    Class_00458160();
};

class Class_00458180 {
public:
    void FUN_00458180(int size);
};

class Class_0042b370 {
public:
    char unknown_0[0x20];
    char name[0x60];                    // +0x20
    char model[0x20];                   // +0x80
    char unknown_a0[0x152 - 0xa0];
    int field_152;                      // +0x152
    void* field_156;                    // +0x156
    char unknown_15a[0x162 - 0x15a];
    int field_162;                      // +0x162
    char unknown_166[0x16e - 0x166];
    int field_16e;                      // +0x16e
    char unknown_172[0x17a - 0x172];
    int field_17a;                      // +0x17a
    char unknown_17e[0x18e - 0x17e];
    void* field_18e;                    // +0x18e
    char unknown_192[0x21e - 0x192];
    unsigned short field_21e;           // +0x21e
    char unknown_220[0x22e - 0x220];
    unsigned char field_22e;            // +0x22e
    char unknown_22f[0x241 - 0x22f];
    unsigned int flags;                 // +0x241
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
    void* field_c;                      // +0xc
    char unknown_10[0x14377 - 0x10];
    void** field_14377;                 // +0x14377
    Class_00458160* field_1437b;        // +0x1437b
    char unknown_1437f[0x1438f - 0x1437f];
    int field_1438f;                    // +0x1438f
    int field_14393;                    // +0x14393
    int field_14397;                    // +0x14397
    Class_0042b370* field_1439b;        // +0x1439b
    char unknown_1439f[0x37e1f - 0x1439f];
    int field_37e1f;                    // +0x37e1f
    int field_37e23;                    // +0x37e23
    char unknown_37e27[0x38d71 - 0x37e27];
    unsigned char field_38d71;          // +0x38d71
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
void FUN_0042d2e0()
{
    char path[256];
    char section[100];
    char namebuf[100];

    {
        Class_004c2ea0 parser;
        FUN_004290f0(path, "gamedata", "moveinfo", "TDF");
        if (!((Class_004c2f60*)&parser)->FUN_004c2f60(path))
            FUN_004b6290("Can't load MOVEINFO.TDF");

        Class_00440320* entry = Class_00440290::DAT_00512358.entries;
        int i = 0;
        while (entry < Class_00440290::DAT_00512358.entries + 32) {
            sprintf(section, "CLASS%d", i);
            ((Class_004c3e10*)&parser)->FUN_004c3e10();
            if (((Class_004c3410*)&parser)->FUN_004c3410(section)) {
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(namebuf, "name", 100, DAT_005119b8);
                entry->field_0 = (int*)FUN_004d8610(namebuf);
                entry->FUN_00440340(&parser);
            }
            entry++;
            i++;
        }
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
    size = (size + 0xfff) & 0xfffff000;
    ((Class_00458180*)g_game->field_1437b)->FUN_00458180(size);

    FUN_004d8780(g_game->field_1439b);

    Class_0042b370* base = g_game->field_1439b;
    Class_0042b370* end = base + g_game->field_1438f;

    Class_0042b370* p = base + 1;
    Class_0042b370* d = end;
    if (p != end) {
        while ((p->flags & 0x800000) != 0) {
            if (++p == end)
                break;
        }
        d = p;
        if (p != end) {
            Class_0042b370* s = p + 1;
            while (s != end) {
                if ((s->flags & 0x800000) != 0) {
                    *d = *s;
                    d++;
                }
                s++;
            }
        }
    }
    g_game->field_1438f = (int)(d - base) / 585;

    Class_0042b370* last = base + g_game->field_1438f;
    if (g_game->field_1438f - 1 < 0x11) {
        FUN_00432fb0(base + 1, last, (void*)FUN_0042db60, 0);
    } else {
        FUN_00432d40(base + 1, last, (void*)FUN_0042db60, 0);
        Class_0042b370* q = base + 0x11;
        FUN_00432fb0(base + 1, q, (void*)FUN_0042db60, 0);
        for (; q != last; q++) {
            Class_0042b370 tmp = *q;
            Class_0042b370* r = q - 1;
            while (_strcmpi(tmp.name, r->name) < 0) {
                *(r + 1) = *r;
                r--;
            }
            *(r + 1) = tmp;
        }
    }

    unsigned short index = 0;
    if (g_game->field_1438f > 0) {
        unsigned int k = 0;
        do {
            base[k].field_21e = index;
            index++;
            k = index;
        } while ((int)k < g_game->field_1438f);
    }

    int c = g_game->field_1438f;
    g_game->field_14393 = 0;
    while (c != 0) {
        g_game->field_14393++;
        c >>= 1;
    }

    g_game->field_14377 = (void**)FUN_004d83b0("MODEL PTRS", g_game->field_1438f * 4);

    for (unsigned short u = 1; u < g_game->field_1438f; u++) {
        Class_0042b370* type = &g_game->field_1439b[u];
        g_game->field_38d71 = (unsigned char)((int)(u * 100) / g_game->field_1438f);
        type->field_21e = u;
        FUN_004290f0(path, "units", type->name, "FBI");
        if (FUN_004bbc40(path))
            FUN_0042bf40(path, type);

        strncpy(namebuf, type->model, 0x20);
        namebuf[0x1f] = 0;
        FUN_004290f0(path, "objects3d", namebuf, "3DO");
        void* model = FUN_004cb560(path);
        if (model == 0)
            FUN_004b6290(path);
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
            type->flags |= 0x80000000;
        else
            type->flags &= 0x7fffffff;

        char suffix = 1;
        bool found = false;
        for (;;) {
            sprintf(section, "%s%d", namebuf, suffix);
            FUN_004290f0(path, "guis", section, "GUI");
            if (!FUN_004bbc40(path))
                break;
            suffix++;
            found = true;
        }
        if (found)
            type->field_22e = suffix;
        else if ((int)type->flags < 0)
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
            if ((type->flags >> 6) & 1) {
                ((Class_004c3e10*)&parser2)->FUN_004c3e10();
                if (((Class_004c3410*)&parser2)->FUN_004c3410("CANBUILD")
                    && ((Class_004c3410*)&parser2)->FUN_004c3410(type->name)) {
                    int count = 0;
                    short* out = list;
                    int k = 1;
                    sprintf(namebuf, "canbuild%d", k);
                    while (((Class_004c48c0*)parser2.current)
                               ->FUN_004c48c0(section, namebuf, 0x20, DAT_005119b8)) {
                        short val = FUN_00488b10(section);
                        if (val != 0) {
                            *out = val;
                            count++;
                            out++;
                        }
                        k++;
                        sprintf(namebuf, "canbuild%d", k);
                    }
                    type->field_152 = count;
                }
                sprintf(section, "CANBUILD %s", type->name);
                type->field_156 = FUN_004d83b0(section, 0x3c);
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
