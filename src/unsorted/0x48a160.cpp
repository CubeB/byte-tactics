// Decompiled by Opus. Names are provisional.

struct Point_0048a160 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Entry_0048a160 {
    Point_0048a160 point;              // +0x0
    char unknown_4[0x1c - 4];
};

struct Class_0048a160 {
    int unknown_0;
    Entry_0048a160 entries[1];         // +0x4
};

// FUNCTION: 0x48a160
void __stdcall FUN_0048a160(Class_0048a160* obj, int index)
{
    Point_0048a160* p = &obj->entries[index].point;
    p->a = 0;
    p->b = 0x8000;
}
