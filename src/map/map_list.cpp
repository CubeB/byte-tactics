// Decompiled by Opus, Space Bunny Free, deepseek-v4.1-flash and deepseek-v4.1. Names are provisional.

#include <string.h>
#include <vector>

extern char DAT_005119b8[];

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

class Class_004c91a0 {
public:
    char* ptr;
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

struct Elem_00434a60 {
    Class_004c9390 name;               // +0x0
    int value;                         // +0x4

    ~Elem_00434a60() { name.ReleaseRef(); }
};

class TdfFile {
public:
    int field_0;
    void* current;
    int field_8;

    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
};

class Mission {
public:
    int owner;                          // +0x0
    char unknown_4[0xa04 - 0x4];
    int field_a04;                      // +0xa04
    TdfFile field_a08;                  // +0xa08
    char text_a14[0x100];               // +0xa14
    char text_b14[0x100];               // +0xb14
    int field_c14;                      // +0xc14
    char unknown_c18[0xd24 - 0xc18];
    int field_d24;                      // +0xd24
    int field_d28;                      // +0xd28
    int field_d2c;                      // +0xd2c
    char unknown_d30[0xdac - 0xd30];
    int field_dac;                      // +0xdac
    int field_db0;                      // +0xdb0
    int field_db4;                      // +0xdb4
    int field_db8;                      // +0xdb8
    int field_dbc;                      // +0xdbc
    int field_dc0;                      // +0xdc0
    char unknown_dc4[0xec4 - 0xdc4];

    Mission(int owner_);
    ~Mission();
    void LoadCampaign(char* name);
    int FUN_00436860(int type, TdfFile* parser, char* schema);
};

inline Mission::Mission(int owner_)
{
    field_db8 = 0;
    field_db0 = 0;
    field_dc0 = 0;
    field_db4 = 0;
    field_dac = 0;
    field_dbc = 0;
    field_a04 = 0;
    field_d24 = 0;
    field_d28 = 0;
    field_d2c = 0;
    field_c14 = 0;
    text_a14[0] = 0;
    text_b14[0] = 0;
    owner = owner_;
    ((Mission*)this)->LoadCampaign(DAT_005119b8);
}

class PacketManager {
public:
    void SendAllQueued(int param);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Mission* field_391e9;              // +0x391e9
};
#pragma pack(pop)

extern char* DAT_005122d4;
extern int DAT_005122d8;
extern int DAT_005122dc;
extern int DAT_005122e0;
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern Game* g_game;

void __cdecl FUN_004d85a0(void* p);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __stdcall FUN_00491c80(int n);
void __stdcall ListDirectory(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
char* __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
char* __stdcall StripExtension(char* name);
char* __stdcall Translate(char* text);
void HandleNetPackets();

// A file-local global std::vector: the compiler generates its initialiser
// (0x434a30) and the destructor it registers with atexit (0x434a60).
//
// The element is 8 bytes and its destructor releases the reference-counted
// string at +0 through ReleaseRef. The vector must be `static`: for an
// external global MSVC reloads _First after the destroy loop on both paths
// (into eax), while for a static it keeps _First in esi on the empty path.
// FUNCTION: 0x434a30 _$E5
// FUNCTION: 0x434a60 _$E3
static std::vector<Elem_00434a60> DAT_005122c0;


// Replaces the singleton at g_game+0x391e9 with a fresh Mission when the
// current one belongs to a different owner, then stores it back (NULL when the
// allocation failed). The constructor body is inlined here.
//
// The original re-reads the global in the delete (the compiler keeps the
// delete's null check because its operand is a load, not the condition's
// value); loading it into a local first drops the check and is 4 bytes short.
// FUNCTION: 0x434ab0
void __stdcall FUN_00434ab0(int owner)
{
    if (g_game->field_391e9 != 0) {
        if (g_game->field_391e9->owner == owner) {
            return;
        }
        delete g_game->field_391e9;
    }
    g_game->field_391e9 = new Mission(owner);
}

// Frees a global buffer, clears three globals and deletes the object at
// g_game+0x391e9.

void __cdecl FUN_004d85a0(void* p);
// FUNCTION: 0x434b90
void FUN_00434b90()
{
    if (DAT_005122d4) {
        FUN_004d85a0(DAT_005122d4);
        DAT_005122d4 = 0;
    }
    DAT_005122d8 = 0;
    DAT_005122dc = 0;
    DAT_005122e0 = 0;
    delete g_game->field_391e9;
    g_game->field_391e9 = 0;
}

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
// FUNCTION: 0x434bf0
int __stdcall LoadMapList(void** param_1, int param_2, int param_3)
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
        if (g_game->field_391e9->owner != 3)
            s.bFlag = 0;
    } else {
        s.bFlag = 0;
    }
    int offset = 0;
    DAT_005122dc = 1;
    DAT_005122d4 = (char*)FUN_004d83b0("MULTI MAPS", 1);
    *(char*)DAT_005122d4 = 0;

    std::vector<Class_004c91a0> files;
    ListDirectory("Maps\\*.ota", 0, &files);
    DAT_005122e0 = 0;

    s.count = files.size();
    for (s.i = 0; s.i < s.count; s.i++) {
        struct A { char name[256]; char lower[256]; char path[256]; } a;
        BuildDataPath(a.path, "Maps", files[s.i].ptr, "OTA");
        TdfFile parser;
        if (((TdfFile*)&parser)->LoadFile(a.path) != 0
            && g_game->field_391e9->FUN_00436860(3, &parser, 0) != 0) {
            strcpy(a.name, files[s.i].ptr);
            StripExtension(a.name);
            strcpy(a.lower, a.name);
            _strlwr(a.lower);
            char* src = Translate(a.lower);
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
    int result = LoadMapList(param_1, param_2, param_3);
    return result;
}
