// Decompiled by Opus. Names are provisional.
// Unlinks the link from its owner's list (head at +0xa2); the counterpart of
// 0x489540, which links it in.

class Class_00489540;

#pragma pack(push, 2)
struct Owner_00489540 {
    char unknown_0[0xa2];
    Class_00489540* head;  // +0xa2
    short flag;            // +0xa6
};
#pragma pack(pop)

class Class_00489540 {
public:
    char unknown_0[4];
    Owner_00489540* owner;  // +4
    Class_00489540* next;   // +8

    void FUN_00489580();
};

// FUNCTION: 0x489580
void Class_00489540::FUN_00489580()
{
    if (owner != 0) {
        Class_00489540** link = &owner->head;
        while (*link != this)
            link = &(*link)->next;
        *link = next;
        owner = 0;
        next = 0;
    }
}
