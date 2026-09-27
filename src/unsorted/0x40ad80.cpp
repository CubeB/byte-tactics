// Decompiled by GPT-6. Names are provisional.
// Bytes match; existing STL names conflict for _Ucopy (0x406c10),
// _Ufill (0x406c40), and size (0x40c560). The same output vector calls
// Unit_00407560* insert at 0x408f30, identifying its element type.
#include <vector>
struct Vec { int x,y,z; };
#pragma pack(push,1)
struct Unit_00407560 { char pad[0x6a]; Vec pos; char pad76[0x110-0x76]; unsigned flags; int Ready() const { return (flags&0x10000000) && !(flags&0x4000); } };
struct Owner { char pad[5]; std::vector<Unit_00407560*> visible,known; char pad25[0x79-0x25]; int hasSpecial; };
#pragma pack(pop)
extern Owner* DAT_005119c0[];
// FUNCTION: 0x40ad80
void __stdcall FUN_0040ad80(int player,const Vec* pos,int radius,int flags,std::vector<Unit_00407560*>* out)
{
    int radius2=radius*radius;
    std::vector<Unit_00407560*>& visible=DAT_005119c0[player]->visible;
    std::vector<Unit_00407560*>::iterator it;
    for(it=visible.begin();it!=visible.end();++it) {
        Unit_00407560* unit=*it;
        int dz=pos->z-unit->pos.z;
        int dx=pos->x-unit->pos.x;
        int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
        if(d<=radius2 && unit->Ready()) out->push_back(unit);
    }
    Owner* owner=DAT_005119c0[player];
    if(owner->hasSpecial && out->empty()) {
        for(it=owner->known.begin();it!=owner->known.end();++it) {
            Unit_00407560* unit=*it;
            int dz=pos->z-unit->pos.z;
        int dx=pos->x-unit->pos.x;
        int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
        if(d<=radius2 && unit->Ready()) out->push_back(unit);
        }
    }
}
