// Decompiled by Opus. Names are provisional.

struct Entry_0048a440 {
    short id;                          // +0x0
    short kind;                        // +0x2
    char unknown_4[0x1c - 4];
};

struct Class_0048a440 {
    char unknown_0[4];
    Entry_0048a440 entries[3];         // +0x4
};

struct Object_0048a440 {
    char unknown_0[0xa8];
    unsigned short id;                 // +0xa8
};

// FUNCTION: 0x48a440
int __stdcall FUN_0048a440(Class_0048a440* list, Object_0048a440* obj)
{
    for (int i = 0; i < 3; i++) {
        if (list->entries[i].kind == -0x8000 && list->entries[i].id == obj->id) {
            return 1;
        }
    }
    return 0;
}
