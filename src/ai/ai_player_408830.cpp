// Decompiled by GPT-6. Names are provisional.
#pragma pack(push,1)
struct Def {
    char pad[0x1c0]; short height; char pad1c2[0x241-0x1c2];
    unsigned unused:6; unsigned builder:1; unsigned unused7:4; unsigned flying:1; unsigned unused12:20;
    unsigned unused245:12; unsigned special:1; unsigned unused13:19;
};
struct Unit { char pad[0x92]; Def* def; char pad96[0xac-0x96]; int group; char padb0[0x110-0xb0]; unsigned flags; int pad114; };
struct Player { char pad[0x67]; Unit* first; Unit* last; };
#pragma pack(pop)
void __stdcall SetUnitSquad(Unit*,int);
class Class_00408830 { public: Player* player; void FUN_00408830(); };
// FUNCTION: 0x408830
void Class_00408830::FUN_00408830()
{
    for(Unit* u=player->first;u<=player->last;++u) {
        if(u->flags&0x20) {
            if(u->def->special) u->flags=(u->flags&~0x80000)|0x40000;
            else u->flags=(u->flags&~0x40000)|0x80000;
            u->flags=(u->flags&~0x100000)|0x200000;
            if(!u->group) {
                if(u->flags&0x20000000) {
                    if(u->flags&0x80000000) SetUnitSquad(u,5);
                    else SetUnitSquad(u,1);
                } else if(u->def->builder) SetUnitSquad(u,4);
                else if(u->def->flying) SetUnitSquad(u,8);
                else if(u->def->height>0) SetUnitSquad(u,7);
                else if(u->flags&0x80000000) SetUnitSquad(u,3);
            }
        }
    }
}
