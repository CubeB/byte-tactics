// Decompiled by GPT-6. Names are provisional.
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438930 { public: void FUN_00438930(void*, int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class CobScript { public: int StartScriptWithArgs(char*, void*, int, int, int, int, int, int); };
#pragma pack(push, 1)
struct Def { char pad[0x22a]; unsigned char capacity; char pad22b[0x245-0x22b]; unsigned flags; };
struct Unit {
    int valid; char pad4[0x6a-4]; int pos[3]; char pad76[8]; short size;
    char pad80[6]; int owner; char pad8a[8]; Def* def; int pad96; CobScript* script;
    char pad9e[0xa8-0x9e]; unsigned short id;
};
struct Order { char pad[5]; unsigned char state; unsigned flags; char pada[12]; Unit* target; char pad1a[0x36-0x1a]; int attempts; };
#pragma pack(pop)
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
int __stdcall FUN_00438730(Unit*, Order*, int);
// Keep cases 1 and 3 separate: MSVC merges their identical bodies.
// FUNCTION: 0x406780
int __stdcall GroundPickupOrder(Unit* unit, Order* order, unsigned char flags)
{
    Unit* target = order->target;
    if (target && !(flags & 8)) {
        switch(order->state) {
        case 0:
            if (!unit->valid) break;
            if (!(unit->def->flags & 0x100)) break;
            if (target->size > (short)unit->def->capacity) {
                QueueUnitSpeech(unit, 7, "Unit is too large to transport"); return 8;
            }
            ((Class_00438880*)order)->FUN_00438880("Loading unit"); return 1;
        case 1: return FUN_00438730(unit, order, 8);
        case 2:
            {
            int id = target->id;
            unit->script->StartScriptWithArgs("TransportPickup", 0, 1, 1, id, 0, 0, 0);
            QueueUnitSpeech(unit, 12, 0);
            ++order->attempts;
            ((Class_00439e80*)order)->FUN_00439e80(15); return 1;
            }
        case 3: return FUN_00438730(unit, order, 8);
        case 4:
            if (target->owner) return 5;
            if (order->attempts >= 3) return 9;
            ((Class_00438930*)order)->FUN_00438930(target->pos, 0);
            order->flags = 0xe8; return 1;
        case 5: ((Class_004388d0*)order)->FUN_004388d0(0); return 0;
        default: break;
        }
        return 7;
    }
    QueueUnitSpeech(unit, 7, "Transport mission failed"); return 8;
}
