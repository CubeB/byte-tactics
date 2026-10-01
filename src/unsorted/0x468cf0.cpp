// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// (Earlier partials: deepseek-v4.1-flash, then GPT-6, then GPT-6.1-sol.)
// deepseek-v4.1-flash retry 4 (timeboxed): one variant tried, no gain, reverted.
// Correction to the note further down: the local_1a3 99999.0 site is the ONLY
// one whose compare idiom still differs (base diff, check.py output line ~679):
//   original: test ah,1 / je 0x46933e     ours: test ah,1 / jne 0x469340
// (all the other sites already emit test ah,0x41 and match, so the older note
// claiming 0x41-vs-0x1 is stale). Rewriting that site as
//   if (L.local_1a3 < -99999.0f) sprintf "%dK" /1000; else sprintf "%d";
// does give the original's je polarity but scores 53.4 (6031 bytes): the
// surrounding register allocation has to line up first, so the plain
// `>= -99999.0f` form stays. The whole remaining diff is 1831 lines of
// register/scheduling noise, so attack it block by block against the saved
// base diff (build/scratch/0x468cf0/base.txt in the issue-3534 worktree)
// rather than by dyadic rewrites of one statement.
// deepseek-v4.1-flash retry 3 (timeboxed): 53.1 -> 53.8, 6029 -> 6023 bytes.
// Three fixes, all statement/evaluation order rather than structure:
// 1. The 33-byte memcpy of g_game+0x37e3f must come BEFORE the cVar3 load in
//    the source: the original's mov cl,[eax+edx*2+0x1ca9] is scheduled between
//    rep movsd and movsb, so the load is emitted mid-memcpy only when the
//    memcpy statement precedes it. 53.1 -> 53.5.
// 2. The FUN_004c1420 index at 0x3816b is read through the iVar11 (= iVar12 +
//    0x1b63) pointer with +0x27, not through iVar12 + 0x1b8a: the original
//    folds +0x1b63 into the lea and reads [eax+0x27]. 53.5 -> 53.7.
// 3. The FUN_004c13a0 byte arg near the first FUN_004c1420 is read through the
//    just-stored L.local_208 (= g_game+0xdcb) pointer with +0xf: the original
//    emits lea edi,[eax+0xdcb]; mov [esp+0x1c],edi; mov cl,[edi+0xf]. 53.7 ->
//    53.8. Same trick did NOT help in the prologue (be950 args via
//    L.local_1b0[0xf], already known to give 51.9).
// Neutral (compiles to identical bytes): dropping the parens in the
// 0x2cb4 - (0x2cb0>>1) - 0x14323 expression, assigning local_19f before
// local_18f, a named delta temp in the pDVar1[19] tick update, and inlining
// FUN_004c13f0() into the FUN_004c13a0 argument list at either site.
// deepseek-v4.1-flash retry 2 (timeboxed, 3 variants, no gain): routing the two
// FUN_004be950 byte args through L.local_1b0[0xf] instead of *(byte*)(iVar11+0xdda)
// gives 51.9 (it perturbs far more than the 2 [ebx+0xf] sites); swapping the
// 0x2cac/0x2cb4 coordinate assignments gives 52.8; splitting the >>1 of 0x2cb0
// into its own leading statement is byte-neutral (53.1, same 6029 bytes).
// STATUS AT THAT TIME: partial 53.1% (deepseek-v4.1-flash retry). Two fixes that pass:
// 1. The `/8` remainder middle branch: the original emits `test eax,eax; jle`
//    (the trailing zero-case is the forward jump target), so the middle branch
//    must read `else if (iVar21 > 0) { divide; if==0 -> 1 } else { iVar21=0 }`
//    with the zero case LAST, not `else if (iVar21 <= 0) { 0 } else { divide }`
//    (which gives `cmp eax,1; jge` / `jg` fall-through). 52.9 -> 53.0 -> 53.1.
// WHAT STILL DIFFERS (large, register/scheduling, not source-controllable here):
// - Prologue: original does `lea ebx,[eax+0xdcb]` and keeps that pointer in ebx
//   for the whole function; ours does `add eax,0xdcb` and uses eax, reloading
//   g_game. Also `[ebx+0xf]` vs our `[ebx+0xdda]` (base reg is g_game not the
//   g_game+0xdcb pointer).
// - The add that forms `iVar21 + lVar25` for the local_1ab/local_19f updates:
//   original `add eax,esi` / `mov [esp+0x10],eax`; ours `add esi,eax` /
//   `mov [esp+0x10],esi` (commuted result register). Swapping the operand order
//   `(int)lVar25 + iVar21` did NOT change it (tied at 53.1).
// - movsx/lea ordering in the cVar3/iVar12 address arithmetic (ours splits
//   `lea eax,[eax+edx*2]`/`lea ebp,[eax+0x1b63]`, original folds
//   `mov cl,[eax+edx*2+0x1ca9]`/`lea ebp,[eax+edx*2+0x1b63]`).
// - The 99999.0 compare flag idiom (`test ah,0x41; jne` vs our `test ah,1; je`).
// - The pDVar1[14] += DVar7 - *pDVar1 tick update and the OverlayRect argument
//   copy at the first FUN_004c6b10 (ours uses eax as destination).
// No obvious original bug spotted in this pass.
// Partial, 48.1% (deepseek-v4.1: 46.2% partial then three fixes below).
// The big local struct sits at the TOP of the locals region, so its base is
// 0x10 + (bytes of scalar slots below it); the original has exactly one scalar
// slot (the fild/fistp conversion scratch at [esp+0x10]), giving base 0x14 and
// local_178 at [esp+0xac]. Ours had two (the piVar10 home was the extra one);
// computing g_game+0x141fb inline at each use freed that slot, which moved
// every struct field back by 4 and took the file 40.0 -> 45.3. Assigning
// local_1b0 after the 48-byte memcpy instead of before it (as the original's
// instruction order shows: mov ecx,0xc / lea edi / mov esi / lea ebx /
// rep movsd / mov [esp+0x74],ebx) gave 45.3 -> 46.2.
// deepseek-v4.1 additions, 46.2 -> 48.1:
// 1. The two FUN_004be950 calls in the 0x14280 == 2 branch: the original folds
//    +0x80 into iVar21 and +0x20 into iVar12 in the definitions (its code does
//    add edi,0x80 / add esi,0x20 right after the loads and then passes
//    edi+2, esi, edi-2, esi and edi, esi-2, edi, esi+2), not into the call
//    arguments. Moving the constants there took 46.2 -> 48.0.
// 2. The 33-byte memcpy of g_game+0x37e3f into local_1ac comes after the
//    cVar3/iVar12 address arithmetic in the original (mov cl,[eax+edx*2+0x1ca9]
//    after the mov ecx,8 setup, movsb last). Moving it after those two
//    statements took 48.0 -> 48.1.
// deepseek-v4.1, 48.1 -> 51.6 (five independent fixes):
// 1. The /8 and /16 remainders: the original's bodies use the cdq form
//    (cdq / and edx,7 / add eax,edx / sar eax,3), i.e. plain `x / 8`, not the
//    hand-written `(x + (x >> 0x1f & 7)) >> 3` shift form (49.3).
// 2. The four 99999.0 comparisons are against a FLOAT constant in the original
//    (fcomp dword ptr), so 99999.0f / -99999.0f (49.3).
// 3. The big struct is 4 bytes too small: local_100 is 256 bytes in the
//    original, not 252, which makes the frame 0x214 and moves the param homes
//    to [esp+0x228]/[esp+0x22c], matching (49.4).
// 4. Float compare operand order: the original loads local_1ab (not local_193)
//    first, so the source reads `local_1ab > local_193`, and likewise the
//    `> 0.0f` guards and `local_1a3 >= -99999.0f` put the memory operand on
//    the left (50.0, 51.1).
// 5. The two FUN_004658e0 calls pass full 32-bit values in the original (it
//    does mov ebx,[esp+0x14] and pushes ebp, with no movsx), so drop the
//    (short) narrowing casts on puVar9 / iVar12 / L.local_210 (51.6).
// Also worth trying next: the original keeps the row and column loop counters
// in the struct fields L.local_210 (row, inc per row) and L.local_208 (column),
// reading them back with mov/movsx, rather than in the registers our version
// uses; the original also reads the spilled local_1b4 byte back for the
// *0x14b index (mov eax,[esp+0x70] / and eax,0xff) instead of re-reading
// g_game+0x2a43.
// deepseek-v4.1-flash retry, 51.6 -> 52.9:
// 1. Re-read of the player index: the original reads the spilled local_1b4
//    back for the g_game + idx*0x14b base (`mov eax,[esp+0x70]; and eax,0xff`
//    right after `mov byte ptr [esp+0x70], al`), so use L.local_1b4 in the
//    iVar12 = g_game + (uint)L.local_1b4*0x14b line instead of re-reading
//    g_game+0x2a43. 51.6 -> 52.9.
// Tried and reverted (worse): (a) dropping the early `iVar11 = (int)g_game`
// and reading `L.local_1b0[0xf]` for the FUN_004be950 args to try to get the
// original's `lea ebx,[eax+0xdcb]` (50.4); (b) making the two
// `*(char*)(...[0x96]+0x146) == local_1b4` comparisons byte (the original
// does `cmp byte ptr [ecx+0x146], bl`, ours sign-extends to int via
// `movsx`/`and 0xff`/`cmp ecx,ebx`) (52.8, size 12 smaller but a hair lower).
// WHAT STILL DIFFERS: the same large register/scheduling issues as before plus
// the earlier `lea ebx,[eax+0xdcb]` vs `add eax,0xdcb`, the address CSE in the
// local_1ac region, and the 99999.0 compare flag idiom (`test ah,0x41; jne`
// vs our `test ah,1; je`, same semantics, different emitted form).
// NEXT: the byte compare above is likely right semantically but needs the
// surrounding register picks (edi/esi swap, ebp/ebx) to also line up before it
// scores; the `[esp+0x70]` byte reads should use bVar5 (byte) on both sides
// only once the block's base registers match.
// OLD NOTES (kept):
// The original keeps the local_1b0 pointer (= g_game+0xdcb) in ebx for the
// whole function (lea ebx,[eax+0xdcb] at 0x468d49, [ebx+0xf] at both
// FUN_004be950 calls) and reloads param_1 into ebx at 0x469b02/0x469d31/
// 0x469f29; ours keeps g_game in ebx, writes [ebx+0xdda], and reloads param_1
// into eax/edi/esi. Also outstanding: the pDVar1[14] += DVar7 - *pDVar1 tick
// updates (ours hoists the *pDVar1 store), the OverlayRect argument copy at the
// first FUN_004c6b10 call (ours uses eax as destination), and the address CSE
// in the local_1ac region.
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
  byte local_100 [256];

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
  memcpy(L.local_1f0,*(void**)(g_game+0x37e1b),48);
  L.local_1b0 = (byte *)((int)g_game + 0xdcb);
  FUN_004c2470();
  ((Class_004c6b10*)L.local_1f0)->FUN_004c6b10(*(OverlayRect*)(g_game+0x37e27));
  iVar12 = (int)g_game;
  pDVar1 = (DWORD *)((int)g_game + 0x38d85);
  DVar7 = FUN_004b6560();
  pDVar1[19] += DVar7 - *pDVar1;
  *pDVar1 = DVar7;
  FUN_00483fa0((int)((ushort *)L.local_1f0));
  FUN_00418310((int)(L.local_1f0));
  iVar21 = (int)*(short *)((int)g_game + 0x2cac) - *(int *)((int)g_game + 0x1431f) + 0x80;
  iVar12 = (int)*(short *)((int)g_game + 0x2cb4) - ((int)*(short *)((int)g_game + 0x2cb0) >> 1)
           - *(int *)((int)g_game + 0x14323) + 0x20;
  if (*(char *)((int)g_game + 0x14280) == '\x02') {
    FUN_004be950((int)(L.local_1f0),(int)(iVar21 + -2),(int)(iVar12),(int)(iVar21 + 2),(int)(iVar12),(int)((uint)*(byte *)(iVar11 + 0xdda)));
    FUN_004be950((int)(L.local_1f0),(int)(iVar21),(int)(iVar12 + -2),(int)(iVar21),(int)(iVar12 + 2),(int)((uint)*(byte *)(iVar11 + 0xdda)));
  }
  FUN_004c69c0((int)((int *)L.local_1f0));
  iVar11 = (int)g_game;
  uVar19 = (uint)*(byte *)((int)g_game + 0x2a43);
  iVar12 = (int)g_game+uVar19*0x14b+0x1b63;
  memcpy(&L.local_1ac,g_game+0x37e3f,33);
  cVar3 = *(char*)(g_game+uVar19*0x14b+0x1ca9);
  L.local_1ac = cVar3;
  lVar25 = (int)L.local_1ab;
  lVar26 = (int)*(float*)(iVar12+0x8c);
  iVar21 = (int)lVar26 - (int)lVar25;
  if (iVar21 < 0) {
    iVar21 = iVar21 / 8;
    if (iVar21 == 0) {
      iVar21 = -1;
    }
  }
  else if (iVar21 > 0) {
    iVar21 = iVar21 / 8;
    if (iVar21 == 0) {
      iVar21 = 1;
    }
  }
  else {
    iVar21 = 0;
  }
  L.local_1ab = (float)(iVar21 + (int)lVar25);
  lVar25 = (int)L.local_19f;
  lVar26 = (int)*(float*)(iVar12+0x98);
  iVar21 = (int)lVar26 - (int)lVar25;
  if (iVar21 < 0) {
    iVar21 = iVar21 / 8;
    if (iVar21 == 0) {
      iVar21 = -1;
    }
  }
  else if (iVar21 > 0) {
    iVar21 = iVar21 / 8;
    if (iVar21 == 0) {
      iVar21 = 1;
    }
  }
  else {
    iVar21 = 0;
  }
  L.local_193 = *(float *)(iVar12 + 0xa4);
  L.local_18f = *(float *)(iVar12 + 0xa8);
  L.local_19f = (float)(iVar21 + (int)lVar25);
  if (L.local_1ab > L.local_193) {
    L.local_1ab = L.local_193;
  }
  if (L.local_19f > L.local_18f) {
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
    L.local_208 = (ushort *)((int)g_game + 0xdcb);
    iVar8 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)((byte *)L.local_208)[0xf]),(int)(iVar8));
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
    if (*(float *)(iVar12 + 0xa4) > 0.0f) {
      lVar25 = (int)(((int)L.local_180-(int)L.local_188)*L.local_1ab / *(float*)(iVar12+0xa4)+(int)L.local_188);
      L.local_180 = (undefined4)lVar25;
      FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_188),(int)((byte)*(undefined4 *)(iVar11 + 0x222)));
      puVar9 = L.local_208;
      if ((*(float *)(iVar12 + 0xe8) > 0.0f) &&
         (*(float *)(iVar12 + 0x8c) > *(float *)(iVar12 + 0xe8))) {
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
    if (L.local_1a7 <= 99999.0f) sprintf((char*)L.local_170,"%d",(int)L.local_1a7);
    else sprintf((char*)L.local_170,"%dK",(int)L.local_1a7/1000);
    iVar21 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 10)),(int)(iVar21));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0xf2)),(int)(*(int *)(iVar11 + 0xf6)),(int)(-1));
    if (L.local_1a3 >= -99999.0f) sprintf((char*)L.local_170,"%d",(int)L.local_1a3);
    else sprintf((char*)L.local_170,"%dK",(int)L.local_1a3/1000);
    iVar21 = FUN_004c13f0();
    FUN_004c13a0((int)((uint)*(byte *)((int)puVar9 + 0xc)),(int)(iVar21));
    FUN_004c14f0((int)(L.local_1f0),(int)(L.local_170),(int)(*(int *)(iVar11 + 0x102)),(int)(*(int *)(iVar11 + 0x106)),(int)(-1));
    L.local_188 = *(uint *)(iVar11 + 0x72);
    L.local_184 = *(undefined4 *)(iVar11 + 0x76);
    L.local_180 = *(undefined4 *)(iVar11 + 0x7a);
    L.local_17c = *(undefined4 *)(iVar11 + 0x7e);
    if (*(float *)(iVar12 + 0xa8) > 0.0f) {
      lVar25 = (int)(((int)L.local_180-(int)L.local_188)*L.local_19f / *(float*)(iVar12+0xa8)+(int)L.local_188);
      L.local_180 = (undefined4)lVar25;
      FUN_004bf6f0((int)(L.local_1f0),(int)(&L.local_188),(int)((byte)*(undefined4 *)(iVar11 + 0x226)));
      if ((*(float *)(iVar12 + 0xe4) > 0.0f) &&
         (*(float *)(iVar12 + 0x98) > *(float *)(iVar12 + 0xe4))) {
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
  L.local_1b4 = *(byte *)(g_game+0x2a43);
  iVar12 = (int)g_game + (uint)L.local_1b4 * 0x14b;
  iVar11 = iVar12 + 0x1b63;
  FUN_004c1420((int)(*(int *)((int)g_game + 0x3816b +
                       (uint)*(byte *)(*(int *)(iVar11 + 0x27) + 0x95) * 0x232)));
  FUN_004c13a0((int)((uint)*(byte *)((int)g_game + 0xdda)),(int)FUN_004c13f0());
  iVar13 = 0;
  iVar12 = *(int *)((int)g_game + 0x1431f);
  iVar21 = *(int *)((int)g_game + 0x14323);
  if (0 < *(int *)(iVar8 + 0x1424f)) {
    do {
      iVar14 = *(int *)(iVar8 + 0x1424b) * iVar13;
      iVar13 = iVar13 + 1;
      *(int *)(*(int *)(iVar8 + 0x141ff) + -4 + iVar13 * 4) = *(int *)((int)g_game + 0x141fb) + iVar14 * 4;
      *(undefined2 *)(*(int *)(iVar8 + 0x14203) + -2 + iVar13 * 2) = 0;
    } while (iVar13 < *(int *)(iVar8 + 0x1424f));
  }
  L.local_210 = *(int *)(iVar8 + 0x1424f);
  L.local_1b8 = iVar21 / 16 - 0x10;
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
  L.local_208 = (ushort *)(iVar12 / 16 - 10);
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
      iVar13 = iVar13 / 16 + 0x10;
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
                bVar23 = FUN_004658e0((int)(iVar11),(int)puVar9,(int)iVar12,(int)(*(short *)(iVar13 + 0x94)),(int)(*(short *)(iVar13 + 0x96)),(int)((ushort)*(byte *)(iVar21 + 4)));
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
      piVar16 = (int *)(*(int *)((int)g_game + 0x141fb) + (L.local_1b8 + L.local_210) * *(int *)(iVar8 + 0x1424b) * 4);
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
              (bVar23 = FUN_004658e0((int)(iVar11),(int)puVar9,(int)L.local_210,(int)(*(short *)(iVar13 + 0x94)),(int)(*(short *)(iVar13 + 0x96)),(int)((ushort)*(byte *)(iVar12 + 4))),
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
        piVar16 = (int *)(*(int *)((int)g_game + 0x141fb) + *(int *)(iVar8 + 0x1424b) * L.local_204 * 4);
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
