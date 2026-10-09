// UnitRef: a reference to a unit, the 0x10-byte node embedded in an order's
// target (an order keeps one at +0x12) and the node of a unit's own list (head
// at +0xa2). +0x0 is the vtable the objects that store it carry, +0x4 the unit
// it points at, +0x8 the next node of the list and +0xc the listener told when
// the reference changes. The one declaration of the class for the files that
// share it; the types behind the pointers stay private to their own files.
#ifndef UNIT_REF_H
#define UNIT_REF_H

struct Unit;
class Listener_004896f0;

class UnitRef {
public:
    void* table;                   // +0x0, the object's vtable
    Unit* owner;                   // +0x4
    UnitRef* next;                 // +0x8
    Listener_004896f0* listener;   // +0xc

    // The empty do-while is a debug check that compiles to nothing.
    Unit* Get() { do {} while (0); return owner; }

    void ClearRef(void);
    void LinkToUnit(Unit* o);
    void UnlinkFromUnit();
    void Unlink();
};

#endif
