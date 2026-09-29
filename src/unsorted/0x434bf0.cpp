// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial (92.0%). Everything matches except the bFlag block at 0x434c83:
// the original does `mov eax,1; cmp [esp+0x344],ebp; jne ...; mov ecx,[g_game];
// mov [esp+0x18],eax; ...; je ...; mov [esp+0x18],ebp`, i.e. it stores 1 only
// inside the param_2==0 branch. `int bFlag; if (param_2==0 && cond==3) bFlag=1;
// else bFlag=0;` puts the store/loads in a better order (scratch x4) but was
// not re-checked before the timebox. Two-byte size excess (888 vs 886) from
// this block; all relative jumps are shifted accordingly.
#include <string.h>
#include <vector>

class Class_004c2ea0;

class Class_004c9390 {
public:
    char* data;
    void FUN_004c9390();
};

class Class_004c91a0 {
public:
    char* ptr;
    ~Class_004c91a0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

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

class Class_00435c00 {
public:
    int FUN_00436860(int type, Class_004c2ea0* parser, char* schema);
};

class Class_004618a0 {
public:
    void FUN_004618a0(int param);
};

extern char* DAT_005122d4;
extern int DAT_005122d8;
extern int DAT_005122dc;
extern int DAT_005122e0;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;
extern char* g_game;

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __stdcall FUN_00491c80(int n);
void __stdcall FUN_004bca30(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
char* __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
char* __stdcall FUN_004bb0f0(char* name);
char* __stdcall FUN_004c5740(char* text);
void FUN_00453d40();

// FUNCTION: 0x434bf0
int __stdcall FUN_00434bf0(void** param_1, int param_2, int param_3)
{
    if (DAT_005122d4 != 0) {
        if (param_1 != 0) {
            if (param_3 != 0 && DAT_005122d8 == 0) {
                *param_1 = DAT_005122d4;
                DAT_005122d4 = 0;
            } else {
                char* p = (char*)FUN_004d83b0("MULTI MAPS", DAT_005122dc);
                *param_1 = p;
                memcpy(p, DAT_005122d4, DAT_005122dc);
            }
        }
        return DAT_005122e0;
    }

    FUN_00491c80(0x14);
    int bFlag = 0;
    if (param_2 == 0 && *(int*)(*(int*)(g_game + 0x391e9)) == 3)
        bFlag = 1;
    int offset = 0;
    DAT_005122dc = 1;
    DAT_005122d4 = (char*)FUN_004d83b0("MULTI MAPS", 1);
    *(char*)DAT_005122d4 = 0;

    std::vector<Class_004c91a0> files;
    FUN_004bca30("Maps\\*.ota", 0, &files);
    DAT_005122e0 = 0;

    int count = files.size();
    for (int i = 0; i < count; i++) {
        char path[256];
        FUN_004290f0(path, "Maps", files[i].ptr, "OTA");
        Class_004c2ea0 parser;
        if (((Class_004c2f60*)&parser)->FUN_004c2f60(path) != 0
            && ((Class_00435c00*)(*(int*)(g_game + 0x391e9)))
                   ->FUN_00436860(3, &parser, 0) != 0) {
            char name[256];
            char lower[256];
            strcpy(name, files[i].ptr);
            FUN_004bb0f0(name);
            strcpy(lower, name);
            _strlwr(lower);
            char* src = FUN_004c5740(lower);
            if (_strcmpi(src, lower) == 0)
                src = name;
            int len = strlen(src) + 1;
            DAT_005122d4 = (char*)FUN_004d84a0(DAT_005122d4, "MULTI MAPS",
                                               len + DAT_005122dc);
            strcpy(DAT_005122d4 + offset, src);
            DAT_005122d4[offset + len] = 0;
            offset += len;
            DAT_005122dc += len;
            DAT_005122e0++;
            if (param_2 != 0)
                break;
        }
        if (bFlag != 0) {
            FUN_00453d40();
            if (DAT_00506dbc != 0)
                DAT_00513000.FUN_004618a0(0);
        }
    }
    FUN_00491c80(0x13);
    if (DAT_005122d8 == 0)
        DAT_005122d8 = (param_2 == 0);
    int result = FUN_00434bf0(param_1, param_2, param_3);
    return result;
}
