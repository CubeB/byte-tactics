// Decompiled by Opus. Names are provisional.
class Class_004896f0;

class Listener_004896f0 {
public:
    virtual void Notify(int code);
};

#pragma pack(push, 2)
struct Owner_004896f0 {
    char unknown_0[0xa2];
    Class_004896f0* head;   // +0xa2
};
#pragma pack(pop)

class Class_004896f0 {
public:
    char unknown_0[4];
    Owner_004896f0* owner;          // +0x4
    Class_004896f0* next;           // +0x8
    Listener_004896f0* listener;    // +0xc
    void FUN_004896f0(void);
};

// FUNCTION: 0x4896f0
void Class_004896f0::FUN_004896f0(void)
{
    Owner_004896f0* saved = owner;
    if (listener)
        listener->Notify(8);
    if (owner == saved && owner) {
        Class_004896f0** pp = &owner->head;
        while (*pp != this)
            pp = &(*pp)->next;
        *pp = next;
        owner = 0;
        next = 0;
    }
}
