// Decompiled by GPT-6. Names are provisional.
#include <stdlib.h>
#pragma pack(push,1)
struct Def { char pad[0x156]; int flag; char pad15a[0x22f-0x15a]; char mobile; char pad230[0x249-0x230]; };
struct Game { char pad[0x1438f]; int count; char pad14393[8]; Def* defs; };
class Class_00409470 { public:
    char pad[0x81]; short* a; char pad85[0xa1-0x85]; unsigned char* b;
    char pada5[12]; unsigned char* c; char padb5[12]; int* d; char padc5[12]; int* e; char padd5[12]; int* f;
    void InitUnitTables();
};
#pragma pack(pop)
extern Game* g_game;
// FUNCTION: 0x409470
void Class_00409470::InitUnitTables()
{
    int n=g_game->count;
    for(int i=0;i<n;++i) {
        Def* def=&g_game->defs[i];
        b[i]=0;
        if(!def->mobile) b[i]+=40;
        if(def->flag) b[i]+=20;
        a[i]=0; c[i]=100; d[i]=0; e[i]=-1; f[i]=0;
    }
}
