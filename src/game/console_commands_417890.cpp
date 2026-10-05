// Decompiled by GPT-6 Astra. Names are provisional.
#include <stdio.h>
struct Vec3 { int x,y,z; };
#pragma pack(push,1)
struct UnitDef { char pad0[0x20]; char name[0x15e-0x20]; Vec3 min,max; char pad176[0x249-0x176]; };
struct Game { char pad0[0x2caa]; Vec3 pos; char pad2cb6[0x1422b-0x2cb6]; int width; char pad1422f[0x1438f-0x1422f]; int count; char pad14393[8]; UnitDef* defs; };
#pragma pack(pop)
extern Game* g_game;
extern char DAT_005119b8[];
class Class_004b73c0 { public: char* FUN_004b73c0(int,char*); };
class Class_004b73e0 { public: int FUN_004b73e0(int,int); };
int __stdcall FUN_004bc370(const char*,const char*);
void __stdcall FUN_0047ddc0(UnitDef*,Vec3*);
void* __stdcall FUN_00485f50(unsigned char,short,Vec3,int,int,int);
void* __stdcall FUN_004bb5b0(char*);
void* __stdcall FUN_004bbff0(char*,void*,int*);
unsigned int __stdcall FUN_004b7a30(char*,int,Class_004b73c0*,unsigned int);
void __cdecl FUN_004d85a0(void*);
int __stdcall FUN_004bb5d0(void*);
// FUNCTION: 0x417890
void __stdcall FUN_00417890(Class_004b73c0* args)
{
    Vec3 pos=g_game->pos;
    int count=0;
    for (unsigned short i=1;i<g_game->count;++i) {
        UnitDef* def=&g_game->defs[i];
        if (FUN_004bc370(def->name,args->FUN_004b73c0(0,DAT_005119b8))) {
            if (count) pos.x-=def->min.x;
            FUN_0047ddc0(def,&pos);
            FUN_00485f50(((Class_004b73e0*)args)->FUN_004b73e0(1,0),i,pos,1,1,0);
            pos.x+=def->max.x+0x200000;
            if (pos.x >= (g_game->width<<16)) { pos.x=0xa00000; pos.z+=0xa00000; }
            ++count;
        }
    }
    if (!count) {
        Vec3 saved;
        int size;
        char path[60];
        // The original unbounded formatting can overflow path for a long argument.
        sprintf(path,"debugdat\\%s.txt",args->FUN_004b73c0(0,DAT_005119b8));
        void* file=FUN_004bb5b0(path);
        if (file) {
            saved=g_game->pos;
            void* data=FUN_004bbff0(path,file,&size);
            if (data) {
                FUN_004b7a30((char*)data,size,args,0xffffffff);
                FUN_004d85a0(data);
            }
            FUN_004bb5d0(file);
            g_game->pos=saved;
        }
    }
}
