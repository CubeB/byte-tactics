// Decompiled by GPT-6 Astra. Names are provisional.
#include <stdio.h>
struct Vec3 { int x,y,z; };
#pragma pack(push,1)
struct UnitDef { char pad0[0x20]; char name[0x15e-0x20]; Vec3 min,max; char pad176[0x249-0x176]; };
struct Game { char pad0[0x2caa]; Vec3 pos; char pad2cb6[0x1422b-0x2cb6]; int width; char pad1422f[0x1438f-0x1422f]; int count; char pad14393[8]; UnitDef* defs; };
#pragma pack(pop)
extern Game* g_game;
extern char DAT_005119b8[];
class CommandArgs { public: int GetIntArg(int,int); char* GetArg(int,char*); };
int __stdcall MatchWildcard(const char*,const char*);
void __stdcall FUN_0047ddc0(UnitDef*,Vec3*);
void* __stdcall CreateUnit(unsigned char,short,Vec3,int,int,int);
void* __stdcall HAPI_OpenFileRead(char*);
void* __stdcall HAPI_LoadOpenFile(char*,void*,int*);
unsigned int __stdcall ExecuteCommandText(char*,int,CommandArgs*,unsigned int);
void __cdecl FUN_004d85a0(void*);
int __stdcall HAPI_CloseFile(void*);
// FUNCTION: 0x417890
void __stdcall FUN_00417890(CommandArgs* args)
{
    Vec3 pos=g_game->pos;
    int count=0;
    for (unsigned short i=1;i<g_game->count;++i) {
        UnitDef* def=&g_game->defs[i];
        if (MatchWildcard(def->name,args->GetArg(0,DAT_005119b8))) {
            if (count) pos.x-=def->min.x;
            FUN_0047ddc0(def,&pos);
            CreateUnit(((CommandArgs*)args)->GetIntArg(1,0),i,pos,1,1,0);
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
        sprintf(path,"debugdat\\%s.txt",args->GetArg(0,DAT_005119b8));
        void* file=HAPI_OpenFileRead(path);
        if (file) {
            saved=g_game->pos;
            void* data=HAPI_LoadOpenFile(path,file,&size);
            if (data) {
                ExecuteCommandText((char*)data,size,args,0xffffffff);
                FUN_004d85a0(data);
            }
            HAPI_CloseFile(file);
            g_game->pos=saved;
        }
    }
}
