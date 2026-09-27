// Decompiled by Opus. Names are provisional.
// Second constructor of the Class_0044ce20 subclass with vtable 0x4fd388
// (compare 0x44d930): the rectangle comes from a position, made relative to
// the owner's view origin, and a size.

struct View_0044d8a0 {
    char unknown_0[0x7e];
    short x;                           // +0x7e
    short y;                           // +0x80
};

#pragma pack(push, 2)
struct Owner_0044d8a0 {
    char unknown_0[0xe];
    View_0044d8a0* view;               // +0xe
};
#pragma pack(pop)

struct Point_0044d8a0 {
    short x;
    short y;
};

// The vtables are stored by hand, as in the other Class_0044ce20 subclasses.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd388[];

class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Owner_0044d8a0* owner;             // +0x4

    Class_0044ce20(Owner_0044d8a0* param_1)
    {
        vtable = DAT_004fd2f8;
        owner = param_1;
    }
};

class Class_0044d8a0 : public Class_0044ce20 {
public:
    int a;                             // +0x8
    int b;                             // +0xc
    int c;                             // +0x10
    int d;                             // +0x14

    Class_0044d8a0(Owner_0044d8a0* owner, Point_0044d8a0 pos, Point_0044d8a0 size);
};

// FUNCTION: 0x44d8a0
Class_0044d8a0::Class_0044d8a0(Owner_0044d8a0* owner, Point_0044d8a0 pos, Point_0044d8a0 size)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd388;
    a = pos.x - owner->view->x;
    c = pos.y - owner->view->y;
    b = size.x + pos.x;
    d = size.y + pos.y;
}
