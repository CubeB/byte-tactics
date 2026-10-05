// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Detaches every node from an owner's list (head at +0xa2): for each node
// calls listener slot 0 with 8, then unlinks it from the list when its owner
// did not change during the notification.

class Listener_00489740 {
public:
    virtual void Notify(int code);
};

class UnitRef;

#pragma pack(push, 2)
struct Owner_00489740 {
    char unknown_0[0xa2];
    UnitRef* head;                     // +0xa2
};
#pragma pack(pop)

class UnitRef {
public:
    char unknown_0[4];
    Owner_00489740* owner;             // +0x4
    UnitRef* next;                     // +0x8
    Listener_00489740* listener;       // +0xc
};

// FUNCTION: 0x489740
void __stdcall ClearUnitRefs(Owner_00489740* obj)
{
    for (UnitRef* n = obj->head; n != 0; n = obj->head) {
        Owner_00489740* saved = n->owner;
        if (n->listener)
            n->listener->Notify(8);
        if (n->owner == saved && n->owner) {
            UnitRef** pp = &n->owner->head;
            while (*pp != n)
                pp = &(*pp)->next;
            *pp = n->next;
            n->owner = 0;
            n->next = 0;
        }
    }
}
