// Decompiled by GPT-6. Names are provisional.
#include <windows.h>
union Fixed { int value; struct { unsigned short fraction; short whole; } parts; };
struct Position { Fixed x, y, z; };
struct Handle { unsigned short index; char pad[6]; void* table; };
struct Effect { void* model; Handle image; Handle shadow; Position position; char pad[0x4c-0x28]; short rotation[4]; };
struct Effects { int count; Effect entries[300]; };
#pragma pack(push, 1)
struct Game {
 char pad[0x1431f]; int cameraX, cameraZ; char pad2[0x1491b-0x14327];
 Effects effects; char pad3[0x37e27-0x1491b-sizeof(Effects)]; int viewport[4];
};
#pragma pack(pop)
extern Game* g_game;
extern void* DAT_00511df0[100];
extern int __stdcall FUN_00421550(void*, void*);
extern int __stdcall FUN_004b6720(void*, int, int);
extern void* __stdcall FUN_004b7ee0(Handle*);
extern void __stdcall FUN_004b8ec0(void*, void*, int, int);
extern void __stdcall FUN_004b7f90(void*, void*, int, int);
extern void __stdcall FUN_0046bae0(void*, Position*, void*, short*);
// FUNCTION: 0x420b00
void __stdcall FUN_00420b00(void* surface) {
 for(int i=0; i<100; ++i)
  if(DAT_00511df0[i] && !FUN_00421550(surface,DAT_00511df0[i])) DAT_00511df0[i]=0;
 Effects* effects=&g_game->effects;
 Effect* entries=effects->entries;
 Position pos;
 for(i=0; i<effects->count; ++i) {
  pos.x.value=entries[i].position.x.value-(g_game->cameraX<<16);
  pos.y=entries[i].position.y;
  pos.z.value=entries[i].position.z.value-(g_game->cameraZ<<16);
  int x=pos.x.parts.whole+128;
  int y=pos.z.parts.whole-(pos.y.parts.whole>>1)+32;
  if(FUN_004b6720(g_game->viewport,x,y) && entries[i].shadow.table)
   FUN_004b8ec0(surface,FUN_004b7ee0(&entries[i].shadow),x,y);
 }
 for(i=0; i<effects->count; ++i) {
  pos.x.value=entries[i].position.x.value-(g_game->cameraX<<16);
  pos.y=entries[i].position.y;
  pos.z.value=entries[i].position.z.value-(g_game->cameraZ<<16);
  int x=pos.x.parts.whole+128;
  int y=pos.z.parts.whole-(pos.y.parts.whole>>1)+32;
  if(FUN_004b6720(g_game->viewport,x,y)) {
   if(entries[i].model) FUN_0046bae0(surface,&pos,entries[i].model,entries[i].rotation);
   if(entries[i].image.table) FUN_004b7f90(surface,FUN_004b7ee0(&entries[i].image),x,y);
  }
 }
}
