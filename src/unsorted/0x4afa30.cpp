// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free. Names are provisional.
// MATCH. Notes on the shape, since they are not obvious from the disassembly:
//  * The object is passed as the FIRST STACK argument; ecx (`this`) is dead on entry,
//    which is why the prologue saves a register it never reads. The result of
//    FUN_004aa8f0 is the object that gets the vtable slot at +8, not the argument.
//  * `push ecx` is the 4-byte frame local, not a save: it holds the FUN_004aa8f0
//    result, and later the "TITL" entry, whose live ranges do not overlap.
//  * The empty-path test is `strlen(cwd) == 0`. MSVC 5's inlined strlen leaves
//    length+1 in ecx after `not ecx`, so the compare against 0 becomes `dec ecx / jne`.
//    Written as `cwd[0] == 0` it folds to a byte test instead, and as `== 1` it grows
//    a `cmp ecx, 1`.
#include <string.h>

#pragma pack(push, 1)
struct Class_004af5b0 {
    char* gui;                       // +0x00
    void* field_4;                   // +0x04
    char* field_8;                   // +0x08
    char* field_c;                   // +0x0c
    char* field_10;                  // +0x10
    char unknown_14[0x24 - 0x14];    // +0x14
    char cwd[0x100];                 // +0x24
    char drive[0x10];                // +0x124
    char unknown_134[0x100];         // +0x134
    int field_234;                   // +0x234
    int field_238;                   // +0x238
    int field_23c;                   // +0x23c
    int field_240;                   // +0x240
    int field_244;                   // +0x244
};

struct Class_004afa30 {
    char path[0x13];                 // +0x00
    short field_13;                  // +0x13
    char unknown_15[0x18 - 0x15];   // +0x15
    void* field_18;                  // +0x18
    char unknown_1c[0xcca - 0x1c];  // +0x1c
    int field_cca;                   // +0xcca

    // self is really the first stack argument, not `this`.
    Class_004af5b0* FUN_004afa30(Class_004afa30* self, char* arg2, char* arg3, char* arg4);
};

struct Entry_004a0010 {
    char unknown_0[2];
    char name[0x10];
    char unknown_12[0xb6 - 0x12];
    short count;
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

void* FUN_004d83b0(unsigned int param_1, unsigned int param_2);
void __stdcall FUN_004bb120(char* path);
void __stdcall FUN_004bb150(char* path);
void __stdcall FUN_004bc2e0(char* buf);
char* __stdcall FUN_004bc320(char* drive, char* buf, int size);
Entry_004a0010* __stdcall FUN_004a0010(Entry_004a0010* entries, char* name);
Entry_004a0010* __stdcall FUN_004a0180(Entry_004a0010* entries, char* name);
Entry_004a0010* __stdcall FUN_004a0200(Entry_004a0010* entries, char* name);
Entry_004a0010* __stdcall FUN_0049ff10(Entry_004a0010* entries, char* name);
void __stdcall FUN_0049fa90(Class_004afa30* obj);
void __stdcall FUN_004af5b0(Class_004af5b0* obj);
void* __stdcall FUN_004aa8f0(void* param_1, char* param_2, int param_3);
void FUN_004af670();

// FUNCTION: 0x4afa30
Class_004af5b0* Class_004afa30::FUN_004afa30(Class_004afa30* self, char* arg2, char* arg3, char* arg4)
{
    void* gui = FUN_004aa8f0(self, "FILEREQ.GUI", 0);
    if (gui == NULL) {
        return NULL;
    }

    Class_004af5b0* obj = (Class_004af5b0*)FUN_004d83b0((unsigned int)"FILE REQUESTER DATA", 0x24c);
    obj->gui = (char*)self;
    strcpy(obj->cwd, arg2);
    FUN_004bb120(obj->cwd);

    if (strlen(obj->cwd) == 0) {
        strcpy(obj->cwd, "NO PATH");
    }

    ((void**)gui)[2] = (void*)FUN_004af670;
    ((void**)gui)[3] = obj;
    obj->field_244 = 0;

    Entry_004a0010* entries = (Entry_004a0010*)((char**)self->field_18)[1];
    obj->field_8 = (char*)FUN_004a0010(entries, "NAME");
    obj->field_c = (char*)FUN_004a0010(entries, "MASK");
    obj->field_10 = (char*)FUN_0049ff10(entries, "PATH");
    Entry_004a0010* titl = FUN_004a0180(entries, "TITL");
    obj->field_4 = (void*)FUN_004a0200(entries, "SLID");

    short none = -1;
    *(short*)((char*)entries + 0x13) = none;
    *(short*)((char*)entries + 0x15) = none;
    strcpy((char*)titl + 0xb6, arg4);
    *(short*)((char*)titl + 0x13) = none;

    obj->field_234 = (int)FUN_004d83b0((unsigned int)"FILE NAMES", 0x17700);
    memset((void*)obj->field_234, -1, 0x17700);

    obj->field_238 = (int)FUN_004d83b0((unsigned int)"FILE SIZES", 0xea60);
    memset((void*)obj->field_238, -1, 0xea60);

    obj->field_240 = (int)arg3;
    obj->field_23c = (int)arg2;

    FUN_004bb150(arg2);

    strcpy((char*)obj->field_8 + 0xb6, arg2);
    strcpy((char*)obj->field_c + 0xb6, arg3);

    FUN_004bc2e0(obj->drive);
    FUN_004bc320(obj->drive, obj->unknown_134, 0x100);
    FUN_004af5b0(obj);

    FUN_0049fa90(self);

    return obj;
}
