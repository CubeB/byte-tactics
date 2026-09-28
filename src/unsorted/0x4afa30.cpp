// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: Register allocation differences throughout. Original saves `this` on stack
// and reloads into ebp/edi/ecx/edx at different points, while mine keeps it in a register.
// Prologue push order differs (original: ecx,esi then edi,ebp,ebx; mine: ecx,esi,edi then ebp,ebx).
// Result of FUN_004aa8f0 saved at different stack offset. 80.3% match.
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

    Class_004af5b0* FUN_004afa30(char* gui_name, char* arg2, char* arg3, int arg4);
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
void* FUN_004aa8f0(void* param_1, char* param_2, int param_3);

// FUNCTION: 0x4afa30
Class_004af5b0* Class_004afa30::FUN_004afa30(char* gui_name, char* arg2, char* arg3, int arg4)
{
    void* result = FUN_004aa8f0(gui_name, "FILEREQ.GUI", 0);
    if (result == NULL) {
        return NULL;
    }

    Class_004af5b0* obj = (Class_004af5b0*)FUN_004d83b0((unsigned int)"FILE REQUESTER DATA", 0x24c);
    obj->gui = gui_name;
    strcpy(obj->cwd, this->path);
    FUN_004bb120(obj->cwd);

    if (obj->cwd[0] == '\0') {
        strcpy(obj->cwd, "NO PATH");
    }

    ((void**)result)[2] = (void*)0x4af670;
    ((void**)result)[3] = obj;
    obj->field_244 = 0;

    Entry_004a0010* entries = (Entry_004a0010*)((char**)this->field_18)[1];
    obj->field_8 = (char*)FUN_004a0010(entries, "NAME");
    obj->field_c = (char*)FUN_004a0010(entries, "MASK");
    obj->field_10 = (char*)FUN_0049ff10(entries, "PATH");
    Entry_004a0010* titl = FUN_004a0180(entries, "TITL");
    obj->field_4 = (void*)FUN_004a0200(entries, "SLID");

    *(short*)((char*)entries + 0x13) = -1;
    *(short*)((char*)entries + 0x15) = -1;
    strcpy((char*)titl + 0xb6, arg2);

    this->field_13 = -1;

    obj->field_234 = (int)FUN_004d83b0((unsigned int)"FILE NAMES", 0x17700);
    memset((void*)obj->field_234, -1, 0x17700);

    obj->field_238 = (int)FUN_004d83b0((unsigned int)"FILE SIZES", 0xea60);
    memset((void*)obj->field_238, -1, 0xea60);

    obj->field_240 = (int)arg3;
    obj->field_23c = (int)arg2;

    FUN_004bb150(arg2);

    strcpy((char*)obj->field_8 + 0xb6, arg2);
    strcpy((char*)obj->field_c + 0xb6, arg2);

    FUN_004bc2e0(obj->drive);
    FUN_004bc320(obj->drive, obj->unknown_134, 0x100);
    FUN_004af5b0(obj);

    FUN_0049fa90(this);

    return obj;
}
