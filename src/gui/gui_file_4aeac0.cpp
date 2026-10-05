// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by opus. Names are provisional.
// MATCH. Two things were missing, and earlier notes blamed the wrong one:
// - The real preceding function, 0x4ae630 (the matching GUI writer, matched
//   in its own file), is defined above this one without an annotation, as the
//   guide's preceding-function rule says. Without it MSVC tail-duplicates the
//   six-instruction loop latch into every switch case (880 bytes, 75.3 %);
//   with it the latch stays shared as in the original. Earlier passes tried
//   made-up preceding functions, which did nothing; only the real one works.
//   No extra latch statement is needed (the old `e->body.nuttin = i;` probe
//   is gone).
// - hotornot is a 1-bit bitfield, read through an int local exactly as the
//   standalone reader 0x4ae410 does. Assigning the call result directly
//   copies the old field into edx; the hand-written xor/and/xor form loads
//   the field before the call.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The real preceding function, 0x4ae630, matched in its own file (see the
// note above). It has no annotation here to avoid a duplicate.
struct Class_004bbbe0;
char* __stdcall ChangeExtension(char*, char*, const char*);
int __stdcall FUN_004bbc40(char*);
void __stdcall FUN_004bbc30(char*);
void __stdcall FUN_004bbc10(char*, char*);
Class_004bbbe0* __stdcall FUN_004bb6a0(char*);
void __stdcall FUN_004bb5d0(Class_004bbbe0*);
unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0*, void*, unsigned int);
void __stdcall WriteTabs(Class_004bbbe0*, int);
void __stdcall WriteKeyValue(Class_004bbbe0*, char*, char*, int);
void __stdcall WriteCommonFields(void*, Class_004bbbe0*, int);
void __stdcall WritePanelFields(void*, Class_004bbbe0*, int);

void __stdcall WriteGuiFile(char* obj, char* name)
{
    int index;
    char button[100];
    char slider[100];
    char header[100];
    char common[100];
    char gadget[100];
    char path[256];
    char backup[256];
    char hot[100];
    char edit[100];
    char empty[100];
    char list[100];
    ChangeExtension(name, path, "GUI");
    if (FUN_004bbc40(path)) {
        ChangeExtension(name, backup, "BGU");
        FUN_004bbc30(backup);
        FUN_004bbc10(path, backup);
    }
    Class_004bbbe0* out = FUN_004bb6a0(path);
    char* p = obj;
    for (index = 0; index < *(short*)(obj + 0xb6) + 1; index++, p += 0x15b) {
        sprintf(gadget, "GADGET%d", index);
        sprintf(header, "[%s]", gadget);
        FUN_004bbbe0(out, header, strlen(header));
        FUN_004bbbe0(out, "\n", 1);
        WriteTabs(out, 1);
        FUN_004bbbe0(out, "{\n", 2);
        sprintf(common, "[%s]", "COMMON");
        {
            char t1 = '\t';
            for (int i = 0; i < 1; i++) FUN_004bbbe0(out, &t1, 1);
        }
        FUN_004bbbe0(out, common, strlen(common));
        FUN_004bbbe0(out, "\n", 1);
        WriteTabs(out, 2);
        FUN_004bbbe0(out, "{\n", 2);
        WriteCommonFields(p, out, 2);
        {
            int j = 2;
            char t2 = '\t';
            do { FUN_004bbbe0(out, &t2, 1); } while (--j);
        }
        FUN_004bbbe0(out, "}\n", 2);
        switch (*(unsigned char*)p) {
        case 0:
            WritePanelFields(p, out, 1);
            break;
        case 1:
            WriteKeyValue(out, "status", _itoa(*(short*)(p + 0x138), button, 10), 1);
            WriteKeyValue(out, "text", p + 0xb6, 1);
            WriteKeyValue(out, "quickkey", _itoa(*(signed char*)(p + 0x13a), button, 10), 1);
            WriteKeyValue(out, "grayedout", _itoa(*(unsigned char*)(p + 0x13c) & 1, button, 10), 1);
            WriteKeyValue(out, "stages", _itoa(*(unsigned char*)(p + 0x136), button, 10), 1);
            break;
        case 2:
            WriteKeyValue(out, "itemheight", _itoa(*(short*)(p + 0xda), list, 10), 1);
            break;
        case 3:
            WriteKeyValue(out, "maxchars", _itoa(*(short*)(p + 0x138), edit, 10), 1);
            WriteKeyValue(out, "text", p + 0xb6, 1);
            break;
        case 4:
            WriteKeyValue(out, "range", _itoa(*(short*)(p + 0x136), slider, 10), 1);
            WriteKeyValue(out, "thick", _itoa(*(int*)(p + 0x13c), slider, 10), 1);
            WriteKeyValue(out, "knobpos", _itoa(*(short*)(p + 0x140), slider, 10), 1);
            WriteKeyValue(out, "knobsize", _itoa(*(short*)(p + 0x142), slider, 10), 1);
            break;
        case 5:
            WriteKeyValue(out, "text", p + 0xb6, 1);
            WriteKeyValue(out, "link", p + 0x136, 1);
            break;
        case 6:
            WriteKeyValue(out, "hotornot", _itoa(*(unsigned int*)(p + 0xc8) & 1, hot, 10), 1);
            break;
        case 7:
            WriteKeyValue(out, "filename", p + 0xb6, 1);
            break;
        case 8:
            WriteKeyValue(out, "filename", p + 0xb6, 1);
            break;
        case 10:
            WriteKeyValue(out, "nuttin", _itoa(*(int*)(p + 0xb6), empty, 10), 1);
            break;
        }
        {
            char t3 = '\t';
            for (int i = 0; i < 1; i++) FUN_004bbbe0(out, &t3, 1);
        }
        FUN_004bbbe0(out, "}\n", 2);
    }
    FUN_004bb5d0(out);
}

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c3e20 {
public:
    void* FUN_004c3e20();
};

class Class_004c3e30 {
public:
    void FUN_004c3e30(void* p);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_004c3490 {
public:
    int FUN_004c3490(int index);
};

class Class_004c2ea0 {
public:
    void* field_0;
    Class_004c46c0* current;            // +0x4
    void* field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

extern char DAT_005119b8[];

char* __stdcall ChangeExtension(char* name, char* out, const char* ext);
char* __stdcall FUN_004c5740(char* text);

#pragma pack(push, 1)
struct Sub2_004aeac0 {
    char pad0[0xce - 0xb6];
    int field_ce;                      // +0xce
    char pad1[0xd6 - 0xce - 4];
    int field_d6;                      // +0xd6
    short itemheight;                  // +0xda
    char pad2[0x136 - 0xdc];
};

struct Sub6_004aeac0 {
    char pad0[0xc8 - 0xb6];
    unsigned int hotornot : 1;         // +0xc8, bit 0
    char pad1[0x136 - 0xcc];
};

union Body_004aeac0 {
    char text[0x80];                   // +0xb6
    int nuttin;                        // +0xb6
    short total;                       // +0xb6
    Sub2_004aeac0 s2;
    Sub6_004aeac0 s6;
};

struct Sub34_004aeac0 {
    short range;                       // +0x136
    short maxchars;                    // +0x138
    char pad0[0x13c - 0x13a];
    int thick;                         // +0x13c
    short knobpos;                     // +0x140
    short knobsize;                    // +0x142
    int field_144;                     // +0x144
    char pad1[0x15b - 0x148];
};

struct Sub5_004aeac0 {
    char link[0x11];                   // +0x136
    char field_147;                    // +0x147
    char pad0[0x15b - 0x148];
};

union Tail_004aeac0 {
    Sub34_004aeac0 s34;
    Sub5_004aeac0 s5;
};

struct Elem_004aeac0 {
    unsigned char type;                // +0x000
    char pad0[0xb6 - 0x001];
    Body_004aeac0 body;                // +0xb6
    Tail_004aeac0 tail;                // +0x136
};
#pragma pack(pop)

void __stdcall ReadCommonSection(Elem_004aeac0* obj, Class_004c2ea0* tree);
void __stdcall ReadPanelFields(Elem_004aeac0* obj, Class_004c2ea0* tree);
void __stdcall ReadButtonFields(Elem_004aeac0* obj, Class_004c2ea0* tree);

// FUNCTION: 0x4aeac0
int __stdcall ReadGuiFile(Elem_004aeac0* obj, char* name)
{
    Class_004c2ea0 parser;
    int i;
    int ret = 0;
    char path[256];
    ChangeExtension(name, path, "GUI");
    if (((Class_004c2f60*)&parser)->FUN_004c2f60(path) == 1) {
        ret = 1;
        i = 0;
        while (1) {
            ((Class_004c3e10*)&parser)->FUN_004c3e10();
            if (!((Class_004c3490*)&parser)->FUN_004c3490(i))
                break;
            void* cur = ((Class_004c3e20*)&parser)->FUN_004c3e20();
            Elem_004aeac0* e = obj + i;
            ReadCommonSection(e, &parser);
            ((Class_004c3e30*)&parser)->FUN_004c3e30(cur);
            switch (e->type) {
            case 0:
                ReadPanelFields(e, &parser);
                break;
            case 1:
                ReadButtonFields(e, &parser);
                break;
            case 2:
                e->body.s2.field_ce = 0;
                e->body.s2.field_d6 = 0;
                e->body.s2.itemheight = (short)parser.current->FUN_004c46c0("itemheight", 0);
                break;
            case 3:
                e->tail.s34.maxchars = (short)parser.current->FUN_004c46c0("maxchars", 0);
                if (e->tail.s34.maxchars > 0x80)
                    e->tail.s34.maxchars = 0x80;
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "text", 0x80, DAT_005119b8);
                strcpy(e->body.text, FUN_004c5740(e->body.text));
                break;
            case 4:
                e->tail.s34.range = (short)parser.current->FUN_004c46c0("range", 0);
                e->tail.s34.thick = (short)parser.current->FUN_004c46c0("thick", 0);
                e->tail.s34.knobpos = (short)parser.current->FUN_004c46c0("knobpos", 0);
                e->tail.s34.knobsize = (short)parser.current->FUN_004c46c0("knobsize", 0);
                e->tail.s34.field_144 = 0;
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "text", 0x80, DAT_005119b8);
                strcpy(e->body.text, FUN_004c5740(e->body.text));
                break;
            case 5:
                e->tail.s5.link[0] = 0;
                e->tail.s5.field_147 = 0;
                memset(e->body.text, 0, sizeof(e->body.text));
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "text", 0x80, DAT_005119b8);
                strncpy(e->body.text, FUN_004c5740(e->body.text), 0x7f);
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->tail.s5.link, "link", 0x10, DAT_005119b8);
                break;
            case 6:
                {
                    int value = parser.current->FUN_004c46c0("hotornot", 0);
                    e->body.s6.hotornot = value;
                }
                break;
            case 7:
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "filename", 0x20, DAT_005119b8);
                break;
            case 8:
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "filename", 0x20, DAT_005119b8);
                break;
            case 10:
                e->body.nuttin = parser.current->FUN_004c46c0("nuttin", 0);
                break;
            }
            i++;
        }
        obj->body.total = (short)(i - 1);
        ((Class_004c3240*)&parser)->FUN_004c3240();
    }
    return ret;
}
