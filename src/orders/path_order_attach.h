// PathOrderAttach: the 0x10-byte link at +0x12 of an order, the node the
// PathGoal family and the order handlers hang off the order's target. Its
// constructor puts it in its owner's list (the head is the unit's +0xa2) and
// its destructor unlinks it again; +0xc holds the object the link belongs to.
// The one declaration of the class for the files that share it; the types
// behind the pointers stay private to their own files. order_list.cpp and
// order_queue_4384a0.cpp keep their own views because they need the inline
// SetValue, unit_targets_489280.cpp because it defines the methods with the
// destructor inline, and unit_orders.cpp because it needs Get(); in unit.cpp
// and vtol_orders_414380.cpp the class decides a register allocation.
#ifndef PATH_ORDER_ATTACH_H
#define PATH_ORDER_ATTACH_H

struct Unit;

class PathOrderAttach {
public:
    Unit* owner;                   // +0x4
    PathOrderAttach* next;         // +0x8
    void* value;                   // +0xc, the object the link belongs to

    PathOrderAttach(Unit* o, int v);
    virtual ~PathOrderAttach();
    void SetUnit(Unit* o);
};

#endif
