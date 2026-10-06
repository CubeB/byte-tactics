// Decompiled by GPT-6. Names are provisional.
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
#pragma pack(push, 2)
class Class_0043a1f0 { public: char data[0x56]; Class_0043a1f0(Class_00438760, int, void*, int, int, int); };
#pragma pack(pop)
class Class_00439e80 { public: void FUN_00439e80(int); };
#pragma pack(push, 1)
struct Unit { char pad[0x110]; union { unsigned flags; struct { unsigned mode:2; unsigned rest:30; }; }; void ReleaseWeapons(int); };
struct Order { char pad[5]; unsigned char state; unsigned flags; };
#pragma pack(pop)
Unit* __stdcall FUN_0043b700(Unit*);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
int __stdcall RandomInt(int);
// FUNCTION: 0x406090
int __stdcall StandbyMineOrder(Unit* unit, Order* order, int unused)
{
    switch (order->state) {
    case 0:
        if (!(unit->flags & 0x20000000)) return 7;
        unit->ReleaseWeapons(3);
        order->flags |= 0x10000;
        ((Class_00439e80*)order)->FUN_00439e80(1);
        return 1;
    case 1:
        {
            Unit* other = FUN_0043b700(unit);
            if (other && other->mode == 1 && (unit->flags & 0x300000)) {
                AppendOrder(unit, new Class_0043a1f0("SELFDESTRUCT", 0, 0, 1, 0, 0));
                return 5;
            }
            order->flags |= 0x10000;
            ((Class_00439e80*)order)->FUN_00439e80(RandomInt(30) + 30);
            return 2;
        }
    default: return 7;
    }
}
