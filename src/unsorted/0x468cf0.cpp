// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol. Names are provisional.
// Partial, 40.0%: rendering and overlay logic is present, but the function's
// register allocation, temporary/frame layout and x87 scheduling still differ.
// The 128-set header sweep did not improve the score. Resource fields remain
// packed for the original 33-byte snapshot comparison.
#include <stdio.h>
#include <string.h>
#include <math.h>
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
extern char* g_game;
unsigned long FUN_004b6560();
int __stdcall FUN_00415fa0(int);
int __stdcall FUN_00417f30(int,int);
int __stdcall FUN_00418310(int);
int __stdcall FUN_00420b00(int);
struct Class_00435100 { int FUN_00435100(); };
int __stdcall FUN_0045ac20(int,int);
int __stdcall FUN_0045ffb0(int);
int __stdcall FUN_00464060(int);
float __stdcall FUN_00464ab0(int);
float __stdcall FUN_00464ac0(int);
float __stdcall FUN_00464af0(int);
float __stdcall FUN_00464b00(int);
int __stdcall FUN_004658e0(int,int,int,int,int,int);
int __stdcall FUN_00466b00(int);
int __stdcall FUN_00467a20(int,int,int,int);
int __stdcall FUN_00467c00(int,int,int,int);
int __stdcall FUN_00468380(int);
int __stdcall FUN_004689c0(int);
struct Class_0046a400 { int FUN_0046a400(int); };
int __stdcall FUN_0046a430(int,int,int,int);
int __stdcall FUN_0046a530(int,int);
int __stdcall FUN_0046a610(int,int,int,int);
int __stdcall FUN_0046a860(int);
int __stdcall FUN_0046b900(int,int,int);
int __stdcall FUN_00471f90(int,int);
int __stdcall FUN_00483fa0(int);
int __stdcall FUN_004848e0(int);
int __stdcall FUN_0048c190(int,int);
int __stdcall FUN_0048cc30(int,int);
int __stdcall FUN_004948e0(int);
int __stdcall FUN_0049be60(int);
int __stdcall FUN_004ab170(int,int,int);
int FUN_004b66a0();
int FUN_004b6710();
int __stdcall FUN_004b6720(int,int,int);
int __stdcall FUN_004b7f30(int,int);
int __stdcall FUN_004b7f90(int,int,int,int);
int __stdcall FUN_004be950(int,int,int,int,int,int);
int __stdcall FUN_004bf6f0(int,int,int);
int __stdcall FUN_004bf8c0(int,int,int);
int __stdcall FUN_004c13a0(int,int);
int FUN_004c13f0();
int __stdcall FUN_004c1420(int);
int FUN_004c1450();
int __stdcall FUN_004c1480(int,int);
int __stdcall FUN_004c14f0(int,int,int,int,int);
int __stdcall FUN_004c1b80(int);
int FUN_004c2470();
int FUN_004c2870();
int __stdcall FUN_004c5740(int);
int FUN_004c63a0();
int __stdcall FUN_004c69a0(int);
int __stdcall FUN_004c69c0(int);
struct OverlayRect { int left,top,right,bottom; };
struct Class_004c6b10 { int FUN_004c6b10(OverlayRect); };
typedef unsigned long DWORD;
typedef double float10;
typedef __int64 longlong;
void FUN_00444ba0();
#pragma pack(push,1)
struct OverlayLocals {
  int local_210;
  int local_20c;
  ushort *local_208;
  int local_204;
  uint local_200;
  int local_1fc;
  uint local_1f8;
  int local_1f4;
  uint local_1f0 [12];
  int local_1c0;
  int local_1bc;
  int local_1b8;
  byte local_1b4;
  byte pad_1b3[3];
  byte *local_1b0;
  char local_1ac;
  float local_1ab;
  float local_1a7;
  float local_1a3;
  float local_19f;
  float local_19b;
  float local_197;
  float local_193;
  float local_18f;
  byte resource_pad[3];
  uint local_188;
  undefined4 local_184;
  undefined4 local_180;
  undefined4 local_17c;
  int local_178;
  int local_174;
  byte local_170 [32];
  byte local_150 [80];
  byte local_100 [252];

};
#pragma pack(pop)
// FUNCTION: 0x468cf0
void __stdcall FUN_00468cf0(int param_1,int param_2)

{
  DWORD *pDVar1;
  short *psVar2;
  char cVar3;
  ushort uVar4;
  byte bVar5;
  int uVar6;
  DWORD DVar7;
  int iVar8;
  ushort *puVar9;
  int *piVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  int *piVar16;
  uint *puVar17;
  char *pcVar18;
  uint uVar19;
  uint *puVar20;
  int iVar21;
  char *pcVar22;
  int bVar23;
  float10 fVar24;
  int lVar25;
  int lVar26;
  OverlayLocals L;

  L.local_178 = (*(int *)((int)g_game + 0x37e1f) + 0x80) / 2;
  L.local_174 = *(int *)((int)g_game + 0x37e23) / 2;
  FUN_004c69a0((int)(*(undefined4 *)((int)g_game + 0x37e1b)));
  iVar11 = (int)g_game;
  L.local_1b0 = (byte *)((int)g_game + 0xdcb);
  memcpy(L.local_1f0,*(void**)(g_game+0x37e1b),48);
  FUN_004c2470();
  ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(*(OverlayRect*)(g_game+0x37e27));
  iVar12 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[19] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  FUN_00483fa0((int)((ushort *)L.local_1f0));
  FUN_00418310((int)(L.local_1f0));
  iVar21 = (int)*(short *)((int)g_game + 0x2cac) - *(int *)((int)g_game + 0x1431f);
  iVar12 = ((int)*(short *)((int)g_game + 0x2cb4) - ((int)*(short *)((int)g_game + 0x2cb0) >> 1))
           - *(int *)((int)g_game + 0x14323);
  if (*(char *)((int)g_game + 0x14280) == '\x02') {
    FUN_004be950((int)(L.local_1f0),(int)(iVar21 + 0x7e),(int)(iVar12 + 0x20U),(int)(iVar21 + 0x82),(int)(iVar12 + 0x20U),(int)((uint)*(byte *)(iVar11 + 0xdda)));
    FUN_004be950((int)(L.local_1f0),(int)(iVar21 + 0x80U),(int)(iVar12 + 0x1e),(int)(iVar21 + 0x80U),(int)(iVar12 + 0x22),(int)((uint)*(byte *)(iVar11 + 0xdda)));
  }
  FUN_004c69c0((int)((int *)L.local_1f0));
  iVar11 = (int)g_game;
  uVar19 = (uint)*(byte *)((int)g_game + 0x2a43);
  memcpy(&L.local_1ac,g_game+0x37e3f,33);
  cVar3 = *(char*)(g_game+uVar19*0x14b+0x1ca9);
  iVar12 = (int)g_game+uVar19*0x14b+0x1b63;
  L.local_1ac = cVar3;
  lVar25 = (int)L.local_1ab;
  lVar26 = (int)*(float*)(iVar12+0x8c);
  iVar21 = (int)lVar26 - (int)lVar25;
  if (iVar21 < 0) {
    iVar21 = (int)(iVar21 + (iVar21 >> 0x1f & 7U)) >> 3;
    if (iVar21 == 0) {
      iVar21 = -1;
    }
  }
  else if (iVar21 < 1) {
    iVar21 = 0;
  }
  else {
    iVar21 = (int)(iVar21 + (iVar21 >> 0x1f & 7U)) >> 3;
    if (iVar21 == 0) {
      iVar21 = 1;
    }
  }
  L.local_1ab = (float)(iVar21 + (int)lVar25);
  lVar25 = (int)L.local_19f;
  lVar26 = (int)*(float*)(iVar12+0x98);
  iVar21 = (int)lVar26 - (int)lVar25;
  if (iVar21 < 0) {
    iVar21 = (int)(iVar21 + (iVar21 >> 0x1f & 7U)) >> 3;
    if (iVar21 == 0) {
      iVar21 = -1;
    }
  }
  else if (iVar21 < 1) {
    iVar21 = 0;
  }
  else {
    iVar21 = (int)(iVar21 + (iVar21 >> 0x1f & 7U)) >> 3;
    if (iVar21 == 0) {
      iVar21 = 1;
    }
  }
  L.local_193 = *(float *)(iVar12 + 0xa4);
  L.local_18f = *(float *)(iVar12 + 0xa8);
  L.local_19f = (float)(iVar21 + (int)lVar25);
  if (L.local_193 < L.local_1ab) {
    L.local_1ab = L.local_193;
  }
  if (L.local_18f < L.local_19f) {
    L.local_19f = L.local_18f;
  }
  if (*(uint *)(iVar12 + 0xf8) < *(uint *)(iVar11 + 0x38a47)) {
    *(uint *)(iVar12 + 0xf8) = *(uint *)(iVar12 + 0xf8) + 0x1e;
    fVar24 = FUN_00464ab0((int)(iVar12));
    L.local_1a7 = (float)fVar24;
    fVar24 = FUN_00464ac0((int)(iVar12));
    L.local_1a3 = (float)fVar24;
    fVar24 = FUN_00464af0((int)(iVar12));
    L.local_19b = (float)fVar24;
    fVar24 = FUN_00464b00((int)(iVar12));
    L.local_197 = (float)fVar24;
    iVar11 = (int)g_game;
  }
  if (memcmp(g_game+0x37e3f,&L.local_1ac,33) != 0) {
    memcpy(g_game+0x37e3f,&L.local_1ac,33);
    uVar19 = (uint)*(byte *)(*(int *)(iVar12 + 0x27) + 0x95);
    iVar11 = (int)g_game + 0x37f3d + uVar19 * 0x232;
    FUN_004c1420((int)(*(int *)((int)g_game + 0x3816b + uVar19 * 0x232)));
    FUN_004c1450();
    iVar21 = (int)g_game;
    L.local_208 = (ushort *)((int)g_game + 0xdcb);
    iVar8 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)(iVar21 + 0xdda)),(int)(iVar8));
    iVar21 = 0x81;
    do {
      puVar9 = (ushort *)
               FUN_004b7f30((int)(*(ushort **)
                             ((int)g_game + 83999 + (uVar19 + (uint)(0x81 < iVar21) * 5) * 4)),(int)(0));
      FUN_00467a20((int)(L.local_1f0),(int)(puVar9),(int)(iVar21),(int)(0));
      iVar21 = iVar21 + (uint)*puVar9;
    } while (iVar21 < *(int *)((int)g_game + 0x37e1f));
    FUN_00467c00((int)L.local_1f0,iVar12,iVar11+0x42,0);
    L.local_188 = *(uint *)(iVar11 + 0x52);
    L.local_184 = *(undefined4 *)(iVar11 + 0x56);
    L.local_180 = *(undefined4 *)(iVar11 + 0x5a);
    L.local_17c = *(undefined4 *)(iVar11 + 0x5e);
    puVar9 = L.local_208;
    if (0.0 < *(float *)(iVar12 + 0xa4)) {
      lVar25 = (int)(((int)L.local_180-(int)L.local_188)*L.local_1ab / *(float*)(iVar12+0xa4)+(int)L.local_188);
      L.local_180 = (undefined4)lVar25;
      FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_188),(int)((byte)*(undefined4 *)(iVar11 + 0x222)));
      puVar9 = L.local_208;
      if ((0.0 < *(float *)(iVar12 + 0xe8)) &&
         (*(float *)(iVar12 + 0xe8) < *(float *)(iVar12 + 0x8c))) {
        L.local_200 = *(uint *)(iVar11 + 0x52);
        L.local_1fc = *(int *)(iVar11 + 0x56);
        L.local_1f8 = *(undefined4 *)(iVar11 + 0x5a);
        L.local_1f4 = *(int *)(iVar11 + 0x5e);
        lVar25 = (int)(((int)L.local_1f8-(int)L.local_200)* *(float*)(iVar12+0xe8) / *(float*)(iVar12+0xa4)+(int)L.local_200);
        puVar9 = L.local_208;
        L.local_200 = (uint)lVar25;
        L.local_1f8 = L.local_200 + 2;
        FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_200),(int)(*(byte *)((int)L.local_208 + 0xc)));
      }
    }
    sprintf((char*)L.local_170,"%d",(int)L.local_1ab);
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x62)),(int)(*(int *)(iVar11 + 0x66)),(int)(-1));
    FUN_004c14f0((int)(L.local_1f0),(int)("0"),(int)(*(int *)(iVar11 + 0xd2)),(int)(*(int *)(iVar11 + 0xd6)),(int)(-1));
    sprintf((char*)L.local_170,"%d",(int)*(float*)(iVar12+0xa4));
    iVar21 = FUN_004c1480((int)(*(int *)(iVar11 + 0x22e)),(int)(L.local_170));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0xb2) - iVar21),(int)(*(int *)(iVar11 + 0xb6)),(int)(-1));
    if (L.local_1a7 <= 99999.0) sprintf((char*)L.local_170,"%d",(int)L.local_1a7);
    else sprintf((char*)L.local_170,"%dK",(int)L.local_1a7/1000);
    iVar21 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 10)),(int)(iVar21));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0xf2)),(int)(*(int *)(iVar11 + 0xf6)),(int)(-1));
    if (-99999.0 <= L.local_1a3) sprintf((char*)L.local_170,"%d",(int)L.local_1a3);
    else sprintf((char*)L.local_170,"%dK",(int)L.local_1a3/1000);
    iVar21 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 0xc)),(int)(iVar21));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x102)),(int)(*(int *)(iVar11 + 0x106)),(int)(-1));
    L.local_188 = *(uint *)(iVar11 + 0x72);
    L.local_184 = *(undefined4 *)(iVar11 + 0x76);
    L.local_180 = *(undefined4 *)(iVar11 + 0x7a);
    L.local_17c = *(undefined4 *)(iVar11 + 0x7e);
    if (0.0 < *(float *)(iVar12 + 0xa8)) {
      lVar25 = (int)(((int)L.local_180-(int)L.local_188)*L.local_19f / *(float*)(iVar12+0xa8)+(int)L.local_188);
      L.local_180 = (undefined4)lVar25;
      FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_188),(int)((byte)*(undefined4 *)(iVar11 + 0x226)));
      if ((0.0 < *(float *)(iVar12 + 0xe4)) &&
         (*(float *)(iVar12 + 0xe4) < *(float *)(iVar12 + 0x98))) {
        L.local_200 = *(uint *)(iVar11 + 0x72);
        L.local_1fc = *(int *)(iVar11 + 0x76);
        L.local_1f8 = *(undefined4 *)(iVar11 + 0x7a);
        L.local_1f4 = *(int *)(iVar11 + 0x7e);
        lVar25 = (int)(((int)L.local_1f8-(int)L.local_200)* *(float*)(iVar12+0xe4) / *(float*)(iVar12+0xa8)+(int)L.local_200);
        L.local_200 = (uint)lVar25;
        L.local_1f8 = L.local_200 + 2;
        FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_200),(int)(*(byte *)((int)puVar9 + 0xc)));
      }
    }
    iVar12 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 0xf)),(int)(iVar12));
    sprintf((char*)L.local_170,"%d",(int)L.local_19f);
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x82)),(int)(*(int *)(iVar11 + 0x86)),(int)(-1));
    FUN_004c14f0((int)(L.local_1f0),(int)("0"),(int)(*(int *)(iVar11 + 0xe2)),(int)(*(int *)(iVar11 + 0xe6)),(int)(-1));
    sprintf((char*)L.local_170,"%d",(int)*(float*)(iVar12+0xa8));
    iVar12 = FUN_004c1480((int)(*(int *)(iVar11 + 0x22e)),(int)(L.local_170));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0xc2) - iVar12),(int)(*(int *)(iVar11 + 0xc6)),(int)(-1));
    sprintf((char*)L.local_170,"%.1f",L.local_19b);
    iVar12 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 10)),(int)(iVar12));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x112)),(int)(*(int *)(iVar11 + 0x116)),(int)(-1));
    sprintf((char*)L.local_170,"%.1f",fabs(L.local_197));
    iVar12 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 0xc)),(int)(iVar12));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x122)),(int)(*(int *)(iVar11 + 0x126)),(int)(-1));
  }
  FUN_0046a860((int)(L.local_1f0));
  FUN_00466b00((int)(L.local_1f0));
  ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(*(OverlayRect*)(g_game+0x37e27));
  iVar11 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[14] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  iVar8 = (int)g_game;
  piVar10 = (int *)((int)g_game + 0x141fb);
  L.local_1b4 = *(byte *)(g_game+0x2a43);
  iVar12 = (int)g_game + (uint)*(byte *)((int)g_game + 0x2a43) * 0x14b;
  iVar11 = iVar12 + 0x1b63;
  FUN_004c1420((int)(*(int *)((int)g_game + 0x3816b +
                       (uint)*(byte *)(*(int *)(iVar12 + 0x1b8a) + 0x95) * 0x232)));
  iVar12 = FUN_004c13f0();
  FUN_004c13a0((int)((uint)*(byte *)((int)g_game + 0xdda)),(int)(iVar12));
  iVar13 = 0;
  iVar12 = *(int *)((int)g_game + 0x1431f);
  iVar21 = *(int *)((int)g_game + 0x14323);
  if (0 < *(int *)(iVar8 + 0x1424f)) {
    do {
      iVar14 = *(int *)(iVar8 + 0x1424b) * iVar13;
      iVar13 = iVar13 + 1;
      *(int *)(*(int *)(iVar8 + 0x141ff) + -4 + iVar13 * 4) = *piVar10 + iVar14 * 4;
      *(undefined2 *)(*(int *)(iVar8 + 0x14203) + -2 + iVar13 * 2) = 0;
    } while (iVar13 < *(int *)(iVar8 + 0x1424f));
  }
  L.local_210 = *(int *)(iVar8 + 0x1424f);
  L.local_1b8 = ((int)(iVar21 + (iVar21 >> 0x1f & 0xfU)) >> 4) + -0x10;
  if (L.local_1b8 < 0) {
    L.local_1bc = -L.local_1b8;
    L.local_210 = L.local_210 + L.local_1b8;
    L.local_1b8 = 0;
  }
  else {
    L.local_1bc = 0;
  }
  if (*(int *)(iVar8 + 0x14237) + -1 < L.local_210 + L.local_1b8) {
    L.local_210 = (*(int *)(iVar8 + 0x14237) - L.local_1b8) + -1;
  }
  L.local_204 = *(int *)(iVar8 + 0x1424b);
  L.local_208 = (ushort *)(((int)(iVar12 + (iVar12 >> 0x1f & 0xfU)) >> 4) + -10);
  if ((int)L.local_208 < 0) {
    L.local_204 = L.local_204 + (int)L.local_208;
    L.local_208 = (ushort *)0x0;
  }
  if (*(int *)(iVar8 + 0x14233) + -1 < L.local_204 + (int)L.local_208) {
    L.local_204 = (*(int *)(iVar8 + 0x14233) - (int)L.local_208) + -1;
  }
  puVar9 = *(ushort **)((int)g_game + 0x1435f);
  L.local_20c = 0;
  iVar12 = (int)g_game;
  if (0 < *(int *)((int)g_game + 0x14367)) {
    do {
      uVar4 = *puVar9;
      iVar21 = *(int *)(iVar12 + 0x14357);
      iVar13 = (int)*(short *)(iVar21 + 0x74 + (uint)uVar4 * 0x118) - *(int *)(iVar12 + 0x14323);
      iVar13 = ((int)(iVar13 + (iVar13 >> 0x1f & 0xfU)) >> 4) + 0x10;
      if ((-1 < iVar13) && (iVar13 < *(int *)(iVar8 + 0x1424f))) {
        piVar16 = (int *)(*(int *)(iVar8 + 0x141ff) + iVar13 * 4);
        psVar2 = (short *)(*(int *)(iVar8 + 0x14203) + iVar13 * 2);
        *psVar2 = *psVar2 + 1;
        iVar12 = (int)g_game;
        if ((int *)*piVar16 != (int *)0x0) {
          *(int *)*piVar16 = iVar21 + (uint)uVar4 * 0x118;
          *piVar16 = *piVar16 + 4;
          iVar12 = (int)g_game;
        }
      }
      L.local_20c = L.local_20c + 1;
      puVar9 = puVar9 + 1;
    } while (L.local_20c < *(int *)(iVar12 + 0x14367));
  }
  FUN_00471f90((int)(L.local_1f0),(int)(0));
  FUN_00471f90((int)(L.local_1f0),(int)(1));
  FUN_00471f90((int)(L.local_1f0),(int)(2));
  if (0 < L.local_210) {
    L.local_1c0 = L.local_210;
    iVar12 = L.local_1b8;
    do {
      iVar21 = (int)(*(int *)(iVar8 + 0x14233) * iVar12 + (int)L.local_208) * 0xd +
               *(int *)(iVar8 + 0x14287);
      if (0 < L.local_204) {
        L.local_20c = L.local_204;
        puVar9 = L.local_208;
        do {
          *(byte *)(iVar21 + 0xc) = *(byte *)(iVar21 + 0xc) & 0xfb;
          if (*(ushort *)(iVar21 + 8) < 0xfffb) {
            iVar13 = *(int *)((int)g_game + 0x1426f) + (uint)*(ushort *)(iVar21 + 8) * 0x100;
            if (*(byte *)(iVar13 + 0xfa) < 10) {
              if (((*(byte *)(iVar13 + 0xff) & 8) == 0) ||
                 ((*(byte *)(iVar21 + 0xc) >> 3 & 0xf) == L.local_1b4)) {
                FUN_0046a610((int)(L.local_1f0),(int)(iVar21),(int)((int)puVar9),(int)(iVar12));
              }
              else {
                bVar23 = FUN_004658e0((int)(iVar11),(int)((short)puVar9),(int)((short)iVar12),(int)(*(short *)(iVar13 + 0x94)),(int)(*(short *)(iVar13 + 0x96)),(int)((ushort)*(byte *)(iVar21 + 4)));
                if (bVar23 != 0) {
                  FUN_0046a610((int)(L.local_1f0),(int)(iVar21),(int)((int)puVar9),(int)(iVar12));
                }
              }
            }
            else {
              *(byte *)(iVar21 + 0xc) = *(byte *)(iVar21 + 0xc) | 4;
            }
          }
          puVar9 = (ushort *)((int)puVar9 + 1);
          iVar21 = iVar21 + 0xd;
          L.local_20c = L.local_20c + -1;
        } while (L.local_20c != 0);
      }
      iVar12 = iVar12 + 1;
      L.local_1c0 = L.local_1c0 + -1;
    } while (L.local_1c0 != 0);
  }
  FUN_00471f90((int)(L.local_1f0),(int)(3));
  FUN_00471f90((int)(L.local_1f0),(int)(4));
  iVar12 = L.local_210;
  if (0 < L.local_210) {
    L.local_210 = L.local_1b8;
    L.local_1c0 = iVar12;
    L.local_20c = L.local_1bc * 2;
    L.local_1b8 = L.local_1bc - L.local_1b8;
    do {
      iVar12 = 0;
      piVar16 = (int *)(*piVar10 + (L.local_1b8 + L.local_210) * *(int *)(iVar8 + 0x1424b) * 4);
      if (*(short *)(L.local_20c + *(int *)(iVar8 + 0x14203)) != 0) {
        do {
          iVar21 = *piVar16;
          if ((((byte)*(uint *)(iVar21 + 0x110) & 3) == 1) && (param_1 != 0)) {
            if ((*(uint *)(iVar21 + 0x110) >> 4 & 1) != 0) {
              FUN_0046a530((int)(L.local_1f0),(int)(iVar21));
            }
            if (*(int *)(iVar21 + 0x9a) != 0) {
              FUN_0045ac20((int)(L.local_1f0),(int)(iVar21));
            }
          }
          iVar12 = iVar12 + 1;
          piVar16 = piVar16 + 1;
        } while (iVar12 < (int)(uint)*(ushort *)(L.local_20c + *(int *)(iVar8 + 0x14203)));
      }
      iVar12 = (int)(L.local_210 * *(int *)(iVar8 + 0x14233) + (int)L.local_208) * 0xd +
               *(int *)(iVar8 + 0x14287);
      if (0 < L.local_204) {
        L.local_1bc = L.local_204;
        puVar9 = L.local_208;
        do {
          iVar21 = L.local_210;
          if (((*(byte *)(iVar12 + 0xc) & 4) != 0) &&
             (((iVar13 = *(int *)((int)g_game + 0x1426f) + (uint)*(ushort *)(iVar12 + 8) * 0x100,
               (*(byte *)(iVar13 + 0xff) & 8) == 0 ||
               ((*(byte *)(iVar12 + 0xc) >> 3 & 0xf) == L.local_1b4)) ||
              (bVar23 = FUN_004658e0((int)(iVar11),(int)((short)puVar9),(int)((short)L.local_210),(int)(*(short *)(iVar13 + 0x94)),(int)(*(short *)(iVar13 + 0x96)),(int)((ushort)*(byte *)(iVar12 + 4))),
              bVar23 != 0)))) {
            FUN_0046a610((int)(L.local_1f0),(int)(iVar12),(int)((int)puVar9),(int)(iVar21));
          }
          puVar9 = (ushort *)((int)puVar9 + 1);
          iVar12 = iVar12 + 0xd;
          L.local_1bc = L.local_1bc + -1;
        } while (L.local_1bc != 0);
      }
      L.local_20c = L.local_20c + 2;
      L.local_210 = L.local_210 + 1;
      L.local_1c0 = L.local_1c0 + -1;
    } while (L.local_1c0 != 0);
  }
  FUN_00471f90((int)(L.local_1f0),(int)(5));
  if (param_1 != 0) {
    FUN_00471f90((int)(L.local_1f0),(int)(6));
    FUN_0049be60((int)(L.local_1f0));
    FUN_00420b00((int)(L.local_1f0));
    FUN_00471f90((int)(L.local_1f0),(int)(7));
    L.local_204 = 0;
    if (0 < *(int *)(iVar8 + 0x1424f)) {
      do {
        iVar12 = 0;
        piVar16 = (int *)(*piVar10 + *(int *)(iVar8 + 0x1424b) * L.local_204 * 4);
        iVar11 = L.local_204;
        if (*(short *)(*(int *)(iVar8 + 0x14203) + L.local_204 * 2) != 0) {
          do {
            iVar21 = *piVar16;
            if (((byte)*(uint *)(iVar21 + 0x110) & 3) != 1) {
              if ((*(uint *)(iVar21 + 0x110) >> 4 & 1) != 0) {
                FUN_0046a530((int)(L.local_1f0),(int)(iVar21));
                iVar11 = L.local_204;
              }
              if (*(int *)(iVar21 + 0x9a) != 0) {
                FUN_0045ac20((int)(L.local_1f0),(int)(iVar21));
                iVar11 = L.local_204;
              }
            }
            iVar12 = iVar12 + 1;
            piVar16 = piVar16 + 1;
          } while (iVar12 < (int)(uint)*(ushort *)(*(int *)(iVar8 + 0x14203) + iVar11 * 2));
        }
        L.local_204 = iVar11 + 1;
      } while (L.local_204 < *(int *)(iVar8 + 0x1424f));
    }
  }
  FUN_00471f90((int)(L.local_1f0),(int)(8));
  bVar23 = FUN_004c1b80((int)(0xf9));
  if (bVar23 != 0) {
    FUN_0048cc30((int)(L.local_1f0),(int)((uint *)((int)g_game + 0x142f3)));
  }
  if (param_1 != 0) {
    L.local_20c = 0;
    L.local_208 = *(ushort **)((int)g_game + 0x1435f);
    iVar11 = (int)g_game;
    if (0 < *(int *)((int)g_game + 0x14367)) {
      do {
        iVar12 = *(int *)(iVar11 + 0x14357) + (uint)*L.local_208 * 0x118;
        if (((*(byte *)(iVar11 + 0x37f06) & 1) != 0) || (*(int *)(iVar12 + 0xac) != 0)) {
          *(ushort*)&L.local_210 = (ushort)(byte)L.local_210;
          iVar8 = ((int)*(short *)(iVar12 + 0x74) - *(int *)(iVar11 + 0x14323)) -
                  ((int)*(short *)(iVar12 + 0x70) >> 1);
          iVar21 = ((int)*(short *)(iVar12 + 0x6c) - *(int *)(iVar11 + 0x1431f)) + 0x80;
          if ((*(byte *)(iVar11 + 0x37f06) & 1) != 0) {
            bVar5 = L.local_1b4;
            if (*(char *)(*(int *)(iVar12 + 0x96) + 0x146) == L.local_1b4) {
              FUN_0046a430((int)(L.local_1f0),(int)(iVar12),(int)(iVar21),(int)(iVar8 + 0x2a));
              iVar11 = (int)g_game;
            }
            if ((*(char *)(*(int *)(iVar12 + 0x96) + 0x146) == bVar5) &&
               (*(int *)(iVar12 + 0xac) != 0)) {
              *(byte*)&L.local_210 = *(char *)(iVar12+0xac)+'0';
              FUN_004c14f0((int)(L.local_1f0),(int)((byte *)&L.local_210),(int)(iVar21),(int)(iVar8 + 0x2e),(int)(-1));
              iVar11 = (int)g_game;
            }
          }
        }
        L.local_20c = L.local_20c + 1;
        L.local_208 = L.local_208 + 1;
      } while (L.local_20c < *(int *)(iVar11 + 0x14367));
    }
    FUN_00471f90((int)(L.local_1f0),(int)(9));
  }
  iVar11 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[15] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  if (((*(ushort *)((int)g_game + 0x3923b) & 1) != 0) &&
     ((*(ushort *)((int)g_game + 0x3923b) & 2) != 0)) {
    if (param_1 == 0) goto LAB_00469d93;
    piVar10 = (int *)FUN_0048c190((int)(0),(int)(0));
    FUN_00417f30((int)(L.local_1f0),(int)(piVar10));
  }
  if (param_1 != 0) {
    FUN_004848e0((int)(L.local_1f0));
  }
LAB_00469d93:
  iVar11 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[16] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  if (param_1 == 0) {
    bVar23 = false;
  }
  else if ((*(byte *)((int)g_game + 0x2cc6) & 8) == 0) {
    if (*(char *)((int)g_game + 0x2cc3) == '\x0e') {
      iVar11 = FUN_004b6720((int)((int *)((int)g_game + 0x37e27)),(int)(*(int *)((int)g_game + 0x2c76)),(int)(*(int *)((int)g_game + 0x2c7a)));
      bVar23 = iVar11 != 0;
    }
    else {
      bVar23 = false;
    }
  }
  else {
    bVar23 = true;
  }
  if (bVar23) {
    uVar19 = (*(int *)((int)g_game + 0x2c92) - *(int *)((int)g_game + 0x1431f)) + 0x80;
    iVar11 = ((*(int *)((int)g_game + 0x2c9a) - (*(int *)((int)g_game + 0x2c96) >> 1)) -
             *(int *)((int)g_game + 0x14323)) + 0x20;
    L.local_1f8 = (*(int *)((int)g_game + 0x2c9e) - *(int *)((int)g_game + 0x1431f)) + 0x80;
    L.local_1f4 = ((*(int *)((int)g_game + 0x2ca6) - (*(int *)((int)g_game + 0x2ca2) >> 1)) -
                *(int *)((int)g_game + 0x14323)) + 0x20;
    if (*(char *)((int)g_game + 0x2cc3) == '\x0e') {
      iVar12 = (-(uint)((*(byte *)((int)g_game + 0x2cc6) & 0x40) != 0) & 6) + 4;
    }
    else {
      iVar12 = 0xf;
    }
    uVar15 = (uint)L.local_1b0[iVar12];
    L.local_200 = uVar19;
    if ((int)L.local_1f8 < (int)uVar19) {
      L.local_200 = L.local_1f8;
      L.local_1f8 = uVar19;
    }
    L.local_1fc = iVar11;
    if (L.local_1f4 < iVar11) {
      L.local_1fc = L.local_1f4;
      L.local_1f4 = iVar11;
    }
    FUN_004bf8c0((int)(L.local_1f0),(int)(&L.local_200),(int)(uVar15));
    L.local_200 = L.local_200 + 1;
    L.local_1fc = L.local_1fc + 1;
    L.local_1f8 = L.local_1f8 - 1;
    L.local_1f4 = L.local_1f4 + -1;
    if (*(char *)((int)g_game + 0x2cc3) != '\x0e') {
      uVar15 = (uint)*L.local_1b0;
    }
    FUN_004bf8c0((int)(L.local_1f0),(int)(&L.local_200),(int)(uVar15));
  }
  iVar11 = ((Class_00435100*)(*(undefined4 **)((int)g_game + 0x391e9)))->FUN_00435100();
  if ((iVar11 == 3) ||
     (iVar11 = ((Class_00435100*)(*(undefined4 **)((int)g_game + 0x391e9)))->FUN_00435100(), iVar11 == 2)) {
    FUN_004c69c0((int)((int *)L.local_1f0));
    FUN_004948e0((int)((int *)L.local_1f0));
    ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(*(OverlayRect*)(g_game+0x37e27));
  }
  FUN_004689c0((int)(L.local_1f0));
  if (*(int *)((int)g_game + 0x391c3) != 0) {
    FUN_00468380((int)(L.local_1f0));
  }
  if (param_1 != 0) {
    FUN_00464060((int)(L.local_1f0));
  }
  if (((*(byte *)((int)g_game + 0x3923b) & 2) != 0) && (param_1 != 0)) {
    iVar11 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)L.local_1b0[0xf]),(int)(iVar11));
    FUN_004c1420((int)(*(int *)((int)g_game + 0x391f9)));
    uVar6 = FUN_004c1450();
    iVar11 = uVar6 * 3 + -10;
    sprintf((char*)L.local_150,"FRATE: %d\n",FUN_004b66a0());
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_150),(int)(0x83),(int)(iVar11),(int)(-1));
    FUN_004c14f0((int)(L.local_1f0),(int)((byte *)"[Release]"),(int)(0xbc),(int)(iVar11),(int)(-1));
    sprintf((char*)L.local_150,"MODE %s INFO %s",(*(byte*)(g_game+0x3923b)&2)?"DEBUG":"NORMAL",(*(byte*)(g_game+0x3923b)&1)?"ON":"OFF");
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_150),(int)(0x1ee),(int)(iVar11),(int)(-1));
    uVar6 = FUN_004c1450();
    if ((*(byte *)((int)g_game + 0x2a44) & 1) != 0) {
      FUN_00415fa0((int)(L.local_150));
      FUN_004c14f0((int)(L.local_1f0),(int)(L.local_150),(int)(0xbc),(int)(iVar11 + uVar6),(int)(-1));
    }
  }
  iVar12 = L.local_174;
  iVar11 = L.local_178;
  if ((*(byte *)((int)g_game + 0x38a51) & 1) != 0) {
    iVar21 = L.local_178;
    iVar8 = L.local_174;
    puVar9 = (ushort *)FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x1481b)),(int)(0));
    FUN_004b7f90((int)(L.local_1f0),(int)(puVar9),(int)(iVar21),(int)(iVar8));
  }
  if ((*(byte *)(*(int *)((int)g_game + (uint)*(byte *)((int)g_game + 0x2a42) * 0x14b + 0x1b8a) +
                0x9b) & 0x40) == 0) {
    if ((*(byte *)((int)g_game + 0x3923b) >> 5 & 1) != 0) {
      iVar21 = iVar11;
      iVar8 = iVar12;
      puVar9 = (ushort *)FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x14813)),(int)(0));
      FUN_004b7f90((int)(L.local_1f0),(int)(puVar9),(int)(iVar21),(int)(iVar8));
    }
    if ((*(byte *)((int)g_game + 0x3923b) >> 6 & 1) != 0) {
      puVar9 = (ushort *)FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x14817)),(int)(0));
      FUN_004b7f90((int)(L.local_1f0),(int)(puVar9),(int)(iVar11),(int)(iVar12));
    }
  }
  if ((*(byte *)((int)g_game + 0x37f2f) >> 6 & 1) != 0) {
    uint ticks=*(uint*)(g_game+0x38a47);
    uint hours=ticks/108000;
    int rest=ticks-hours*108000;
    int minutes=rest/1800;
    int seconds=(rest-minutes*1800)/30;
    sprintf((char*)L.local_100,"%s : %02d:%02d:%02d",(char*)FUN_004c5740((int)"Game Time"),hours,minutes,seconds);
    iVar11 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)L.local_1b0[0xf]),(int)(iVar11));
    iVar12 = -1;
    uVar6 = FUN_004c1450();
    iVar11 = FUN_004b6710();
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_100),(int)(0x82),(int)((-0x22 - uVar6) + iVar11),(int)(iVar12))
    ;
  }
  if ((*(byte *)((int)g_game + 0x38a51) >> 1 & 1) != 0) {
    iVar12 = *(int *)((int)g_game + 0x37e23) + -0x50;
    iVar11 = *(int *)((int)g_game + 0x37e1f) + -0x10;
    puVar9 = (ushort *)FUN_004b7f30((int)(*(ushort **)((int)g_game + 0x148cf)),(int)(0));
    FUN_004b7f90((int)(L.local_1f0),(int)(puVar9),(int)(iVar11),(int)(iVar12));
  }
  FUN_004c69c0((int)((int *)L.local_1f0));
  FUN_004ab170((int)((int)g_game + 0x519),(int)(L.local_1f0),(int)((int *)((int)g_game + 0x37e27)));
  if ((*(int *)((int)g_game + 0x38dd5) != 0) && (param_1 != 0)) {
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Network"),(int)(0));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Units"),(int)(1));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Logic"),(int)(2));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Render Static"),(int)(3));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Render Stuff"),(int)(4));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Render Fog"),(int)(5));
    FUN_0046b900((int)(L.local_1f0),(int)("SFX"),(int)(6));
    FUN_0046b900((int)(L.local_1f0),(int)((byte *)"Weapon"),(int)(7));
    FUN_0046b900((int)(L.local_1f0),(int)("Misc"),(int)(8));
  }
  FUN_0045ffb0((int)L.local_1f0);
  FUN_004c2870();
  if ((param_1 != 0) && (param_2 != 0)) {
    FUN_004c63a0();
  }
  ((Class_0046a400*)((void *)((int)g_game + 0x38d85)))->FUN_0046a400((int)(3));
  return;
}
