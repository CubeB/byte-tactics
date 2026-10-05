// Decompiled by Opus. Names are provisional.
// Walks a linked list at +0xa2 and calls slot 0 of each node's object.

class Listener_004897b0 {
public:
    virtual void Notify(int event);
};

struct Node_004897b0 {
    char unknown_0[8];
    Node_004897b0* next;               // +0x8
    Listener_004897b0* listener;       // +0xc
};

#pragma pack(push, 1)
struct Obj_004897b0 {
    char unknown_0[0xa2];
    Node_004897b0* head;               // +0xa2
};
#pragma pack(pop)

// FUNCTION: 0x4897b0
void __stdcall NotifyUnitRefs(Obj_004897b0* obj, int event)
{
    for (Node_004897b0* n = obj->head; n != 0; n = n->next) {
        if (n->listener != 0)
            n->listener->Notify(event);
    }
}
