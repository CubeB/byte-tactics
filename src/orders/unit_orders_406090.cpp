// Decompiled by GPT-6. Names are provisional.
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
#pragma pack(push, 2)
class Class_0043a1f0 { public: char data[0x56]; Class_0043a1f0(Class_00438760, int, void*, int, int, int); };
#pragma pack(pop)
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_00489800 { public: void ReleaseWeapons(int); };
#pragma pack(push, 1)
struct Unit { char pad[0x110]; union { unsigned flags; struct { unsigned mode:2; unsigned rest:30; }; }; };
struct Order { char pad[5]; unsigned char state; unsigned flags; };
#pragma pack(pop)
Unit* __stdcall FUN_0043b700(Unit*);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
int __stdcall FUN_004b6c30(int);
// FUNCTION: 0x406090
int __stdcall FUN_00406090(Unit* unit, Order* order, int unused)
{
    switch (order->state) {
    case 0:
        if (!(unit->flags & 0x20000000)) return 7;
        ((Class_00489800*)unit)->ReleaseWeapons(3);
        order->flags |= 0x10000;
        ((Class_00439e80*)order)->FUN_00439e80(1);
        return 1;
    case 1:
        {
            Unit* other = FUN_0043b700(unit);
            if (other && other->mode == 1 && (unit->flags & 0x300000)) {
                FUN_0043acb0(unit, new Class_0043a1f0("SELFDESTRUCT", 0, 0, 1, 0, 0));
                return 5;
            }
            order->flags |= 0x10000;
            ((Class_00439e80*)order)->FUN_00439e80(FUN_004b6c30(30) + 30);
            return 2;
        }
    default: return 7;
    }
}
