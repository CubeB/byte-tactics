// Decompiled by space-bunny-free. Names are provisional.
#include <string.h>
#include <vector>

extern void __cdecl operator delete(void*);

class Class_004c9390 {
public:
    char* data;
    void ReleaseRef();
};

void __cdecl FUN_004d85a0(void* p);

struct Elem_0042f3a0 {
    Class_004c9390 handle;
    int unknown_4;
    Elem_0042f3a0() { }
    ~Elem_0042f3a0() { handle.ReleaseRef(); }
};

#pragma pack(push, 1)
class VecOwner_0042f3a0 {
public:
    char unknown_0;
    std::vector<Elem_0042f3a0> vec;
};

struct Obj_0042f3a0 {
    char unknown_0[0x64];
    VecOwner_0042f3a0* sub;      // +0x64
    char unknown_68[0xc];
    void* text;                  // +0x74
    char unknown_78[8];
    char name[0x95];             // +0x80
};

struct Game {
    char unknown_0[0x2cf3];
    Obj_0042f3a0 objs[256];      // +0x2cf3, stride 0x115
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x42f3a0
void FreeWeaponTypes()
{
    int i = 0;
    while (i < 256) {
        Obj_0042f3a0* o = &g_game->objs[i];
        if (strlen(o->name) != 0) {
            FUN_004d85a0(o->text);
            o->text = 0;
            o->name[0] = 0;
        }
        if (o->sub) {
            delete o->sub;
            o->sub = 0;
        }
        i++;
    }
}
