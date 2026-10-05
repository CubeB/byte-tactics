// Decompiled by Opus. Names are provisional.

class BitReader {                      // bit reader
public:
    int ReadBits(int bits);
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
    Class_0044e080(Owner_00490a10* owner, BitReader* reader);
};

class Class_0044e9c0 : public Base_00490a10 {
public:
    char unknown_4[0x2c - 0x4];
    Class_0044e9c0(Owner_00490a10* owner, BitReader* reader);
};
#pragma pack(pop)

// Class_00490880's override of slot 9 (vtable 0x4fd9e0, see 0x44ef60.cpp for
// the family): rebuilds the object at +0x4 from the bit stream (a 2-bit kind:
// 1 and 2 pick its class, anything else leaves none), then passes a 2-bit
// state read after it to the owner.
class Class_00490880 {
public:
    Base_00490a10* current;            // +0x4
    Owner_00490a10* owner;             // +0x8

    virtual void FUN_0044efd0(BitReader* reader);       // slot 9
};

// FUNCTION: 0x490a10
void Class_00490880::FUN_0044efd0(BitReader* reader)
{
    if (current) {
        delete current;
        current = 0;
    }
    int kind = reader->ReadBits(2);
    if (kind == 1)
        current = new Class_0044e080(owner, reader);
    else if (kind == 2)
        current = new Class_0044e9c0(owner, reader);
    int state = reader->ReadBits(2);
    owner->obj->FUN_0043d210(owner, state);
}
