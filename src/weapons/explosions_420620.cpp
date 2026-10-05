// Decompiled by GPT-6. Names are provisional.
#include <windows.h>
#include <string.h>
struct ImageFrame { void* image; short duration; short unknown; };
struct Explosion { short count; char flags; char pad[37]; ImageFrame frames[1]; };
struct Shape { int a, b; };
struct PieceFrame { int duration, count, unknown; Shape* shape; int rest[3]; unsigned flags; };
struct Piece { signed char state; char pad[3]; int count, frames, index; char unknown[12]; const char* name; int active; void* vertices; PieceFrame* frameData; char rest[8]; };
#pragma pack(push, 1)
struct Game {
 char pad[0x1491b]; int active; char pad2[0x1ab8f-0x1491f];
 Explosion* smallExplosion; Explosion* mediumExplosion; Explosion* largeExplosion; void* image;
 Piece pieces[300]; char vertices[300][0x60]; PieceFrame frames[300][6];
 char pad3[0x38d74-0x33a0f]; unsigned char shade;
};
#pragma pack(pop)
extern Game* g_game;
extern int DAT_00511f90, DAT_00511f94, DAT_00511f98, DAT_00511f9c, DAT_00511fa0, DAT_00511fa4, DAT_00511fa8, DAT_00511fac, DAT_00511fb0;
extern Shape DAT_00502bf8[6];
extern int DAT_00511df0[100];
class CMemoryCache { public: void InitCache(int); };
extern CMemoryCache DAT_00511f80;
extern void* __stdcall BuildLensFrame(int, int, int);
extern void* __cdecl FUN_004d83b0(const char*, unsigned);
extern void* __stdcall BuildExplosionFrame(int);
inline Explosion* MakeExplosion(int count, int start, int end) {
 Explosion* result = (Explosion*)FUN_004d83b0("CalcedExplosion", 0x28 + count * 8);
 int radius = start;
 int step = (end - start) / count;
 result->count = count;
 result->flags = 0;
 for (int i=0; i<count; ++i) {
  result->frames[i].image = BuildExplosionFrame(radius);
  result->frames[i].duration = 2;
  radius += step;
 }
 return result;
}
// FUNCTION: 0x420620
void InitExplosions() {
 g_game->active = 0;
 g_game->image = BuildLensFrame(22,22,8);
 DAT_00511f90=24; DAT_00511f94=64; DAT_00511f98=8;
 DAT_00511f9c=30; DAT_00511fa0=128; DAT_00511fa4=16;
 DAT_00511fa8=30; DAT_00511fac=200; DAT_00511fb0=32;
 g_game->smallExplosion=MakeExplosion(DAT_00511f90/2,DAT_00511f94,DAT_00511f98);
 g_game->shade=20;
 g_game->mediumExplosion=MakeExplosion(DAT_00511f9c/2,DAT_00511fa0,DAT_00511fa4);
 g_game->shade=50;
 g_game->largeExplosion=MakeExplosion(DAT_00511fa8/2,DAT_00511fac,DAT_00511fb0);
 g_game->shade=100;
 for(int i=0;i<300;++i) {
  g_game->pieces[i].state=-1;
  g_game->pieces[i].count=8;
  g_game->pieces[i].frames=6;
  g_game->pieces[i].index=-1;
  g_game->pieces[i].name="explodepiece";
  g_game->pieces[i].active=0;
  g_game->pieces[i].vertices=g_game->vertices[i];
  g_game->pieces[i].frameData=g_game->frames[i];
 }
 for(i=0;i<300;++i) for(int j=0;j<6;++j) {
  g_game->frames[i][j].flags |= 1;
  g_game->frames[i][j].duration=200;
  g_game->frames[i][j].count=4;
  g_game->frames[i][j].shape=&DAT_00502bf8[j];
 }
 DAT_00511f80.InitCache(100000);
 memset(DAT_00511df0,0,sizeof(DAT_00511df0));
}
