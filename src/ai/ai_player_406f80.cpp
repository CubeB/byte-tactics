// Decompiled by GPT-6 Astra. Names are provisional.
#pragma pack(push, 1)
struct AI { char pad0[13]; int nextAction; };
struct Owner {
    int active; char pad4[0x73-4]; unsigned char control; AI* ai;
    char pad78[0x108-0x78]; unsigned char allied[0x3e]; unsigned char index;
};
struct WeaponDef { char pad0[0x111]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
struct UnitDef {
    char pad0[0x231]; unsigned int* weaponCategories[3]; unsigned int* categories;
    unsigned int flags, flags2;
};
struct Order { char pad0[0x42]; unsigned int capabilities; };
struct Unit {
    char pad0[8]; Weapon weapons[3]; Order* order;
    char pad60[0x92-0x60]; UnitDef* def; Owner* owner;
    char pad9a[12]; unsigned short category; char pada8[0xf4-0xa8];
    unsigned char player, state; char padf6[9]; unsigned char ownerIndex;
    char pad100[4]; float progress; char pad108[8]; unsigned int flags;
};
struct Game { char pad0[0x38a47]; int tick; };
#pragma pack(pop)
extern Game* g_game;
void __stdcall NotifyUnitRefs(Unit*, int);
int __stdcall RandomInt(int);
void __stdcall DeleteOrders(Unit*, int);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, unsigned char);
int __stdcall FUN_0043b1f0(Unit*, Unit*, int);
Unit* __stdcall GetWeaponTargetUnit(Unit*, int);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
unsigned int __stdcall FUN_00438be0(Unit*);
void __stdcall FUN_0047f850(Unit*, int, int);
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }
// FUNCTION: 0x406f80
void __stdcall ReactToAttack(Unit* attacker, Unit* unit, int unused)
{
    NotifyUnitRefs(unit,16);
    if (attacker && !attacker->category) attacker=0;
    if ((unit->def->flags2&0x1000) && unit->owner->active && unit->owner->control==2) {
        unit->owner->ai->nextAction=RandomInt(300)+g_game->tick+30;
        DeleteOrders(unit,0);
    }
    if (attacker && unit->owner->active && (unit->owner->control==1 || unit->owner->control==2) &&
        (unit->def->flags&0x10010000) && unit->progress==0.0f && !unit->owner->allied[attacker->owner->index]) {
        int ordered=0;
        if ((!unit->order || (unit->order->capabilities&0x20000)) &&
            !Contains(unit->def->categories,attacker->category) &&
            !Contains(unit->def->weaponCategories[0],attacker->category) && WeaponCanReachUnit(unit,attacker,0))
            ordered=FUN_0043b1f0(unit,attacker,0);
        if (!ordered && (unit->flags&0x300000)) {
            for (unsigned char i=0;i<3;++i) {
                Weapon* weapon=&unit->weapons[i];
                if ((weapon->flags&2) && (weapon->flags&0x10) && WeaponCanReachUnit(unit,attacker,i) &&
                    !((unsigned char)(weapon->def->flags>>26)&1)) {
                    Unit* target=GetWeaponTargetUnit(unit,i);
                    if (!target || !WeaponCanReachUnit(unit,target,i) || Contains(unit->def->weaponCategories[i],target->category))
                        SetWeaponTargetUnit(unit,attacker,i);
                }
            }
        }
    }
    if (!(FUN_00438be0(unit)&0x80) && (unit->player!=unit->ownerIndex || unit->state==1))
        FUN_0047f850(unit,2,0);
}
