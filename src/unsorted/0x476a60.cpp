// Decompiled by space-bunny-free. Names are provisional.
// Builds the packed list of campaign names whose [HEADER] "campaignside"
// matches the side name at g_game+0x37f3d + side*0x232, or the literal "ALL",
// and returns how many matched. Two buffers come out of FUN_004d83b0: the
// packed name list from the camps\*.TDF directory (FUN_004af320) and the
// result list handed back through *out. The side name is the second parameter,
// not a loop counter, so the loop over the file list has no induction variable
// of its own and MSVC rotates it into a countdown.
// The append is written through a static inline helper and `q` is declared
// before `p`: that order is what puts the append cursor in ebx and the walking
// name pointer in a stack slot, which is the original's allocation.
#include <string.h>

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

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, int size, char* def);
};

#pragma pack(push, 1)
struct Game_476a60 {
    char unknown_0[0x37f3d];
    char names[1][0x232];               // +0x37f3d
};
#pragma pack(pop)

extern Game_476a60* g_game;
extern char DAT_005119b8[];             // ""

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bc930(const char* path, int flag);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* data);
int __stdcall FUN_004af320(char* path, char* buffer, char* p3, int p4, int p5, int p6);

// Copies one name onto the end of the result list and steps the cursor past it.
static inline char* AppendName_00476a60(char* p, char* s)
{
    strcpy(p, s);
    return p + strlen(p) + 1;
}

// FUNCTION: 0x476a60
int __stdcall FUN_00476a60(char** out, int side)
{
    int found = 0;
    Class_004c2ea0 parser;
    char name[0x40];
    char path[0x100];
    name[0] = '0';
    FUN_004290f0(path, "camps", "*", "TDF");
    int n = FUN_004bc930(path, 0);
    char* names = (char*)FUN_004d83b0("CAMPAIGN NAMES1", n << 8);
    *out = (char*)FUN_004d83b0("CAMPAIGN NAMES2", n << 8);
    FUN_004af320(path, names, 0, 0, 1, 2);
    char* q = names;
    char* p = *out;
    for (int i = 0; i < n; i++) {
        FUN_004290f0(path, "camps", q, "tdf");
        if (((Class_004c2f60*)&parser)->FUN_004c2f60(path)) {
            if (((Class_004c3410*)&parser)->FUN_004c3410("HEADER")) {
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(name, "campaignside", 0x40, DAT_005119b8);
                if (strcmp(g_game->names[side], name) == 0 || strcmp("ALL", name) == 0) {
                    found++;
                    p = AppendName_00476a60(p, q);
                }
            }
            q += strlen(q) + 1;
        }
    }
    FUN_004d85a0(names);
    return found;
}
