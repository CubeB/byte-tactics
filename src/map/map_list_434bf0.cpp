// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Aggregate grouping pins the frame order. The original's scalar locals were
// members of one small local struct and its three 256-byte buffers were members
// of another, so their frame offsets follow member order instead of MSVC5's
// free-list order. With `struct { int count; int bFlag; int i; } s;` declared at
// the top of the slow path and `struct { char name[256]; char lower[256];
// char path[256]; } a;` at the top of the loop body the frame comes out:
// allocator temp 0x10, s.count 0x14, s.bFlag 0x18, s.i 0x1c, files 0x20,
// parser 0x30, a.name 0x3c, a.lower 0x13c, a.path 0x23c, byte-exact (886).
// Earlier attempts (declaration/statement reordering, explicit allocator,
// unsigned/size_t counts, header sets, N-declaration sweeps) all stalled at
// 92.9% because MSVC5 numbers these slots by free-list order, which no scalar
// declaration order moves. Grouping the two 256-byte pairs in the base version
// was already right; grouping the scalars is what fixed the five small homes,
// and grouping the buffers with name before lower fixes the last two.
// The odd `mov al, [esp+0x13]` / `mov [files], al` pair is the inlined vector
// default constructor copying the empty allocator temporary; no local for it.

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

class PacketManager {
public:
    void SendAllQueued(int param);
};

extern char* DAT_005122d4;
extern int DAT_005122d8;
extern int DAT_005122dc;
extern int DAT_005122e0;
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern char* g_game;

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __stdcall FUN_00491c80(int n);
void __stdcall FUN_004bca30(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
char* __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
char* __stdcall FUN_004bb0f0(char* name);
char* __stdcall FUN_004c5740(char* text);
void HandleNetPackets();

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
    struct S { int count; int bFlag; int i; } s;
    
    if (param_2 == 0) {
        s.bFlag = 1;
        if (*(int*)(*(int*)(g_game + 0x391e9)) != 3)
            s.bFlag = 0;
    } else {
        s.bFlag = 0;
    }
    int offset = 0;
    DAT_005122dc = 1;
    DAT_005122d4 = (char*)FUN_004d83b0("MULTI MAPS", 1);
    *(char*)DAT_005122d4 = 0;

    std::vector<Class_004c91a0> files;
    FUN_004bca30("Maps\\*.ota", 0, &files);
    DAT_005122e0 = 0;

    s.count = files.size();
    for (s.i = 0; s.i < s.count; s.i++) {
        struct A { char name[256]; char lower[256]; char path[256]; } a;
        FUN_004290f0(a.path, "Maps", files[s.i].ptr, "OTA");
        Class_004c2ea0 parser;
        if (((Class_004c2f60*)&parser)->FUN_004c2f60(a.path) != 0
            && ((Class_00435c00*)(*(int*)(g_game + 0x391e9)))
                   ->FUN_00436860(3, &parser, 0) != 0) {
            strcpy(a.name, files[s.i].ptr);
            FUN_004bb0f0(a.name);
            strcpy(a.lower, a.name);
            _strlwr(a.lower);
            char* src = FUN_004c5740(a.lower);
            if (_strcmpi(src, a.lower) == 0)
                src = a.name;
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
        if (s.bFlag != 0) {
            HandleNetPackets();
            if (g_usePacketManager != 0)
                g_packetManager.SendAllQueued(0);
        }
    }
    FUN_00491c80(0x13);
    if (DAT_005122d8 == 0)
        DAT_005122d8 = (param_2 == 0);
    int result = FUN_00434bf0(param_1, param_2, param_3);
    return result;
}
