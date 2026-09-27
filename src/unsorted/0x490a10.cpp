// Decompiled by Opus. Names are provisional.

class Class_00415dc0 {                 // bit reader
public:
    int FUN_00415dc0(int bits);
};

struct Owner_00490a10;

class Class_0043d210 {
public:
    void FUN_0043d210(Owner_00490a10* owner, int state);
};

struct Owner_00490a10 {
    Class_0043d210* obj;               // +0x0
};

class Base_00490a10 {
public:
    virtual ~Base_00490a10();
};

#pragma pack(push, 2)
class Class_0044e080 : public Base_00490a10 {
public:
    char unknown_4[0x36 - 0x4];
    Class_0044e080(Owner_00490a10* owner, Class_00415dc0* reader);
};

class Class_0044e9c0 : public Base_00490a10 {
public:
    char unknown_4[0x2c - 0x4];
    Class_0044e9c0(Owner_00490a10* owner, Class_00415dc0* reader);
};
#pragma pack(pop)

class Class_00490a10 {
public:
    int unknown_0;                     // +0x0
    Base_00490a10* current;            // +0x4
    Owner_00490a10* owner;             // +0x8

    void FUN_00490a10(Class_00415dc0* reader);
};

// FUNCTION: 0x490a10
void Class_00490a10::FUN_00490a10(Class_00415dc0* reader)
{
    if (current) {
        delete current;
        current = 0;
    }
    int kind = reader->FUN_00415dc0(2);
    if (kind == 1)
        current = new Class_0044e080(owner, reader);
    else if (kind == 2)
        current = new Class_0044e9c0(owner, reader);
    int state = reader->FUN_00415dc0(2);
    owner->obj->FUN_0043d210(owner, state);
}
