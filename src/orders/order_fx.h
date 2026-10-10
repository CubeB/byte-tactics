// OrderFx: the base of the objects an order carries (vtable 0x4fd2f8, 0x8
// bytes: the vpointer and, at +0x4, the order the object was built for). Its
// subclasses are the approach areas, the path orders and the air maneuver.
// The polymorphic view, for the files that delete the object through its
// virtual destructor or call its slots (order_queue_4384a0.cpp and
// victory.cpp). order_targets.cpp keeps its own view of the base: the
// constructors it matches store the vtable by hand (`vtable = g_orderFxVtable`),
// which a class with virtual functions cannot spell.
#ifndef ORDER_FX_H
#define ORDER_FX_H

struct Vec3;
struct Unit;
struct Order;
class HapiBank;
class BitWriter;

class OrderFx {
public:
    int source;                        // +0x4

    virtual ~OrderFx();                                                     // slot 0
    virtual int SerializeToSave(Order* order, HapiBank* file, char* name);  // slot 1
    virtual int GetType();                                                  // slot 2
    virtual int IsFxStyle();                                                // slot 3
    virtual int ContainsUnit(Unit* unit);                                   // slot 4
    virtual int ContainsCell(int x, int y);                                 // slot 5
    virtual void FillGoalCells(void* cells);                                // slot 6
    virtual int ApproxDist(int x, int y);                                   // slot 7
    virtual int FillWorldPos(Vec3* out);                                    // slot 8
    virtual int GetDesiredHeading(unsigned short* out);                     // slot 9
    virtual void SerializeToBits(BitWriter* stream);                        // slot 10
    virtual int KeepAfterComplete();                                        // slot 11

    void AddFlags(int flags);
};

#endif
