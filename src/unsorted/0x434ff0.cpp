// Decompiled by Space Bunny Free. Names are provisional.
// Destructor of Class_00434f70 (constructor 0x434f70, sibling 0x437280).
// Deletes the object at g_game+0x391ed, frees the two-dword buffers at
// +0xdac/+0xdb4/+0xdbc and the pointers at +0xc14 and +0xd24, then destroys
// the sub-object at +0xa08. The second round of buffer frees is dead (the
// first round already zeroed the pointers) but the original emits it anyway,
// so keep it.

#pragma pack(push, 1)
struct Game_00434ff0 {
    char unknown_0[0x391ed];
    void* field_391ed;                 // +0x391ed
};
#pragma pack(pop)

extern Game_00434ff0* g_game;

void __cdecl FUN_004d85a0(int* param_1);

class Class_0048dfb0 {
public:
    void FUN_0048dfb0();
};

class Class_004c2ea0 {
public:
    ~Class_004c2ea0();
};

struct Buffer_00434ff0 {
    int* data;                         // +0x0
    int size;                          // +0x4
};

class Class_00434f70 {
public:
    char unknown_0[0xa08];
    char subobject_a08[0xc14 - 0xa08]; // Class_004c2ea0, destroyed explicitly
    int* field_c14;                    // +0xc14
    char unknown_c18[0xd24 - 0xc18];
    int* field_d24;                    // +0xd24
    char unknown_d28[0xdac - 0xd28];
    Buffer_00434ff0 buffer0;           // +0xdac
    Buffer_00434ff0 buffer1;           // +0xdb4
    Buffer_00434ff0 buffer2;           // +0xdbc

    ~Class_00434f70();
};

// FUNCTION: 0x434ff0
Class_00434f70::~Class_00434f70()
{
    Class_0048dfb0* obj = (Class_0048dfb0*)g_game->field_391ed;
    if (obj) {
        obj->FUN_0048dfb0();
        operator delete(obj);
        g_game->field_391ed = 0;
    }
    if (buffer0.data)
        FUN_004d85a0(buffer0.data);
    buffer0.data = 0;
    buffer0.size = 0;
    if (buffer1.data)
        FUN_004d85a0(buffer1.data);
    buffer1.data = 0;
    buffer1.size = 0;
    if (buffer2.data)
        FUN_004d85a0(buffer2.data);
    buffer2.data = 0;
    buffer2.size = 0;
    if (field_c14) {
        FUN_004d85a0(field_c14);
        field_c14 = 0;
    }
    if (field_d24)
        FUN_004d85a0(field_d24);
    if (buffer2.data)
        FUN_004d85a0(buffer2.data);
    if (buffer1.data)
        FUN_004d85a0(buffer1.data);
    if (buffer0.data)
        FUN_004d85a0(buffer0.data);
    ((Class_004c2ea0*)((char*)this + 0xa08))->~Class_004c2ea0();
}
