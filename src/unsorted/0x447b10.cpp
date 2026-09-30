// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
// (earlier passes by deepseek-v4.1-flash and GPT-6)
// Partial, 45.7% (deepseek-v4.1, 2026-09-30): a plausible full dispatcher.
// BIG LEVER FOUND: scalar stack slots follow DECLARATION ORDER. Declaring
// `int iVar8;` first (it is the first local the original stores, at frame+4)
// took the score from 35.8% to 41.6% with no other change. The arrays also
// live in one struct now (see below), which put the buffer below the table.
// SECOND LEVER (this pass, 41.6% -> 45.7%): the two player-pointer locals
// (iVar11 = g_game + i*0x14b, piVar9 = iVar11 + 0x1b63) are ONE variable in the
// original. Writing `piVar9 = (int*)((int)g_game + (uint)bVar1*0x14b + 0x1b63)`
// and using `(int)piVar9 + 0x27` / +0x13f / +4 for the old iVar11+0x1b8a /
// +0x1ca2 / +0x1b67 sites makes the frame exactly the original sub esp,0x13c
// (the prologue, the two array slots and the two epilogues now match), as long
// as the 10-iteration walk loop gets its OWN pointer local (pVar22) so the
// fused pointer survives the loop. Both swap attempts failed: putting
// `iVar5 = FUN_00457a50();` before the pointer assignment is 41.9%, moving
// `int iVar5;` in the declaration list changes nothing (45.7% both).
// Still differs (next steps):
//  - the fused pointer lives in esi for us, in ebx for the original, so the
//    FUN_00457a50 result (iVar5) takes the other register: the original emits
//    `mov esi,eax` after that call and `lea ebx,[edx+eax+0x1b63]` at its first
//    use, we emit `mov ebx,eax` and reach the pointer through esi. MSVC5 homes
//    by first use, so the original pointer's first use must come before iVar5's.
//  - our scalars sit 4 bytes high: iVar8 is stored at frame+8 (original +4) and
//    iVar5 at frame+0xc (original +0x14), while our frame+4 is free; the
//    original's slots are, in store order: frame+0 player pointer, +4 iVar8,
//    +8 bVar1, +0xc iVar6 (player index), +0x10 the FUN_00456850 byte,
//    +0x14 iVar5 (FUN_00457a50 result).
// Tried 2026-09-30 (deepseek-v4.1), all measured with check.py:
//  - fusing iVar11/piVar9 into one `(int)g_game + 0x1b63 + (uint)bVar1*0x14b`
//    expression (the original really does keep one pointer and reach
//    0x108/0x13f/0x27 off it) DOES give sub esp,0x13c, but the score drops to
//    34.6% and the iVar8 store stays at [esp+0x18] (frame+8) where the original
//    has [esp+0x14] (frame+4): the extra slot is at frame+4 in ours, the
//    original's unused hole is at frame+0xc (between bVar1 at +8 and the byte
//    local at +0x10). So the +4 is a short-lived temp, not the second pointer.
// Confirmed 2026-09-30 (deepseek-v4.1): the TWO check outputs for the two
// declaration orders are byte-identical (same md5), so for this function MSVC 5
// places the int[10] and the char[252] deterministically regardless of source
// order: table low, buffer high. Standalone probes (build/scratch/0x447b10/t1b
// to t5: byte[252]+int[10] in either order, int[63]+int[10], struct wrapper) all
// confirm two separate arrays never swap: the 40-byte one always lands low.
// Wrapping both in ONE 292-byte struct (SlotBuf here) DOES give the original's
// internal order: buffer at +0 and int[10] at +0xfc. With the struct AND
// iVar11/piVar9 fused into one player-pointer local (exactly as the original
// prologue computes it), our frame becomes sub esp,0x13c and both arrays land on
// the original's absolute slots ([esp+0x28] buffer, [esp+0x124] table). That
// variant is saved as build/scratch/0x447b10/fused_0x13c.cpp; it scores 33.7%
// against 35.8% for the version kept here, so this file keeps the higher number.
// That saved variant is structurally closer (right frame, right slots) and is
// the place to restart from.
//  - swapping the declaration order of local_124[252] and local_28[10] changes
//    the binary not at all; in our build the int[10] table lands low
//    ([esp+0x28] = frame+0x18) and the 252-byte buffer high, the reverse of the
//    original ([esp+0x28] buffer = frame+0x18, [esp+0x124] table = frame+0x114).
//    So slot placement for the arrays is driven by something other than
//    declaration order (first-use order of the locals is the next thing to try).
// Also differs: the 0x447b9c block re-computes the player pointer instead of
// reusing the [esp+0x10] slot (original keeps it in eax/esi), the sprintf call
// sites do not reuse lea eax,[esp+N] the way the original does, and the inlined
// string copies (0x40/0x519 style block moves) are missing at the tail.
#include <stdio.h>
#include <string.h>
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
extern char* g_game;
extern int DAT_00506dbc;
extern int DAT_00512994;
extern int DAT_00513000;
int __stdcall FUN_004288d0(int,int,int,int);
int FUN_00430f00();
struct Class_004358f0 { int FUN_004358f0(); };
struct Class_00435c40 { int FUN_00435c40(); };
struct Class_004373a0 { int FUN_004373a0(); };
int FUN_00444a20();
int FUN_00444ea0();
int __stdcall FUN_00446080(int);
int FUN_00446310();
int FUN_00446a50();
int FUN_00446c70();
int __stdcall FUN_00446e90(int);
int FUN_0044c7e0();
int FUN_00450f90();
int FUN_00451180();
int __stdcall FUN_00451220(unsigned char,int);
int __stdcall FUN_004526c0(int);
int __stdcall FUN_00452960(int,int,int,int);
int __stdcall FUN_00452bd0(int);
int __stdcall FUN_00453010(int,unsigned char);
byte FUN_00456850();
int FUN_00457a50();
int FUN_00457af0();
int FUN_00457b40();
int FUN_00457b90();
struct Class_004618a0 { int FUN_004618a0(int); };
struct Class_00463c60 { int FUN_00463c60(int); };
int __stdcall FUN_00463ca0(int,int,int,int);
int __stdcall FUN_00463e50(int,int,int,int);
int __stdcall FUN_0046c620(int);
struct Class_0046df40 { int FUN_0046df40(); };
int __stdcall FUN_0047f1a0(int,int);
int __stdcall FUN_0049fb10(int,int);
int __stdcall FUN_0049fd60(int,int);
int __stdcall FUN_0049fdf0(int,int,int);
int __stdcall FUN_004a0010(int,int);
int __stdcall FUN_004a0f30(int,int);
int __stdcall FUN_004a0f60(int,int);
int __stdcall FUN_004a1080(int,int,int);
int __stdcall FUN_004a1110(int,int,int);
int __stdcall FUN_004a5f40(int,int);
int __stdcall FUN_004a7190(int,int);
int __stdcall FUN_004a81e0(int,int);
int __stdcall FUN_004aa8f0(int,int,int);
int __stdcall FUN_004ab0a0(int);
int __stdcall FUN_004abd90(int,int,int,int,int);
int FUN_004b6340();
int __stdcall FUN_004c5740(int);
int __cdecl FUN_004d85a0(int);
void FUN_00444ba0();
// FUNCTION: 0x447b10
void __stdcall FUN_00447b10(byte *param_1)

{
  int iVar8;
  int *player;
  uint bVar1;
  int iVar6;
  uint bVar4;
  int iVar5;
  char cVar2;
  int bVar3;
  uint uVar7;
  int *piVar9;
  int *pVar22;
  byte *pbVar10;
  int iVar11;
  int *piVar12;
  uint uVar13;
  ushort uVar14;
  ushort uVar15;
  int iVar16;
  uint uVar17;
  byte *pbVar18;
  char *pcVar19;
  int iVar20;
  char *pcVar21;
  struct SlotBuf { char text [252]; int used [10]; };
  SlotBuf local_124;

  iVar8 = *(int *)(*(int *)(param_1 + 0x18) + 4);
  if (*(int *)(param_1 + 0x60) == -1) {
    FUN_004d85a0((int)(*(int **)((int)g_game + 0x2a9b)));
    *(undefined4 *)((int)g_game + 0x2a9b) = 0;
    DAT_00512994 = 0;
    FUN_00446c70();
    return;
  }
  bVar1 = *(byte *)((int)g_game + 0x2a42);
  piVar9 = (int *)((int)g_game + (uint)bVar1 * 0x14b + 0x1b63);
  iVar5 = FUN_00457a50();
  uVar17 = 0;
  do {
    iVar6 = uVar17 * 0x14b;
    player = (int *)((int)g_game + 0x1b63 + iVar6);
    sprintf(local_124.text,"LOGO%d",uVar17);
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)(local_124.text));
    if (((bVar3 != 0) && (*player != 0)) &&
       ((*(char *)((int)player + 0x73) == '\x01' || (*(char *)((int)player + 0x73) == '\x02')))) {
      FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
      FUN_004526c0((int)(*(byte *)(*(int *)((int)player + 0x27) + 0x96) + 1));
      *(byte *)((int)g_game + 0x2bee) = *(byte *)((int)g_game + 0x2bee) | 1;
      FUN_00450f90();
    }
    sprintf(local_124.text,"PLAYER%d",uVar17);
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)(local_124.text));
    if ((bVar3 != 0) && (uVar17 != bVar1)) {
      FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
      cVar2 = *(char *)((int)player + 0x73);
      if ((cVar2 == '\0') && (iVar5 != 0)) {
        ((Class_00463c60*)(player))->FUN_00463c60((int)(4));
        player[1] = -1;
        *(int *)((int)g_game + 0x499) = *(int *)((int)g_game + 0x499) + -1;
      }
      else {
        if (cVar2 == '\x04') {
          ((Class_00463c60*)(player))->FUN_00463c60((int)(0));
          *(int *)((int)g_game + 0x499) = *(int *)((int)g_game + 0x499) + 1;
          FUN_00451180();
        }
        else if (cVar2 != '\0') {
          if (((*player == 0) || (cVar2 != '\x02')) ||
             (uVar7 = FUN_004b6340(), uVar7 - player[2] < 0x1f)) {
            if (((iVar5 != 0) && (*player != 0)) && (*(char *)((int)player + 0x73) == '\x03')) {
              FUN_00446080((int)(uVar17));
            }
          }
          else {
            FUN_00453010((int)((int *)player[1]),(int)(1));
            ((Class_00463c60*)(player))->FUN_00463c60((int)(0));
          }
          goto LAB_00447e27;
        }
        bVar4 = FUN_00456850();
        if (*(short *)(*(int *)((int)g_game + (uint)bVar4 * 0x14b + 0x1b8a) + 0x9b) < 0) {
          iVar20 = 1;
          iVar6 = 1;
          iVar5 = 500;
          pbVar10 = (byte*)FUN_004c5740((int)((byte *)"Can't add another player when game is closed."));
          FUN_004abd90((int)((byte *)((int)g_game + 0x519)),(int)(pbVar10),(int)(iVar5),(int)(iVar6),(int)(iVar20));
          ((Class_00463c60*)(player))->FUN_00463c60((int)(0));
          *(byte *)((int)g_game + 0x2bee) = *(byte *)((int)g_game + 0x2bee) | 1;
          break;
        }
        bVar4 = FUN_00456850();
        if (((*(ushort *)(*(int *)((int)g_game + (uint)bVar4 * 0x14b + 0x1b8a) + 0x9b) & 0x1800) !=
             0x1000) && (iVar20 = FUN_00457b90(), iVar20 == 0)) {
          FUN_00451220((int)((byte)uVar17),(int)(2));
          iVar16 = 10;
          piVar12 = local_124.used;
          for (iVar20 = 10; iVar20 != 0; iVar20 = iVar20 + -1) {
            *piVar12 = 0;
            piVar12 = piVar12 + 1;
          }
          pcVar21 = (char *)((int)g_game + 0x1bd6);
          do {
            if (((*(int *)(pcVar21 + -0x73) != 0) &&
                (((cVar2 = *pcVar21, cVar2 == '\x01' || (cVar2 == '\x02')) || (cVar2 == '\x03'))))
               && (pcVar21[0xd3] != '\n')) {
              if (*(byte *)(*(int *)(pcVar21 + -0x4c) + 0x96) < 9) {
                uVar7 = (uint)*(byte *)(*(int *)(pcVar21 + -0x4c) + 0x96);
              }
              else {
                uVar7 = 9;
              }
              local_124.used[uVar7] = 1;
            }
            pcVar21 = pcVar21 + 0x14b;
            iVar16 = iVar16 + -1;
          } while (iVar16 != 0);
          iVar20 = 0;
          piVar12 = local_124.used;
          do {
            iVar16 = iVar20;
            if (*piVar12 == 0) break;
            iVar20 = iVar20 + 1;
            piVar12 = piVar12 + 1;
            iVar16 = 0;
          } while (iVar20 < 10);
          *(char *)(*(int *)((int)player + 0x27) + 0x96) = (char)iVar16;
        }
      }
LAB_00447e27:
      *(byte *)((int)g_game + 0x2bee) = *(byte *)((int)g_game + 0x2bee) | 1;
      FUN_00451180();
      FUN_00450f90();
    }
    sprintf(local_124.text,"SIDE%d",uVar17);
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)(local_124.text));
    if (bVar3 != 0) {
      FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
      if (*player == 0) {
LAB_00447ea4:
        *(char *)(*(int *)((int)player + 0x27) + 0x95) =
             *(char *)(*(int *)((int)player + 0x27) + 0x95) + '\x01';
        if (*(int *)((int)g_game + 0x37f39) <=
            (int)(uint)*(byte *)(*(int *)((int)player + 0x27) + 0x95)) {
          *(undefined1 *)(*(int *)((int)player + 0x27) + 0x95) = 0;
          bVar4 = FUN_00456850();
          if ((((*(byte *)(*(int *)((int)g_game + (uint)bVar4 * 0x14b + 0x1b8a) + 0x9b) & 0x80) ==
                0) || (*player == 0)) || (*(char *)((int)player + 0x73) != '\x01')) {
            FUN_004a1080((int)((int)param_1),(int)((char *)local_124.text),(int)(0));
            FUN_004a5f40((int)((int)param_1),(int)(*(int *)(param_1 + 0x60)));
          }
          else {
            pbVar10 = (byte *)(*(int *)((int)player + 0x27) + 0x9b);
            *pbVar10 = *pbVar10 | 0x40;
          }
        }
      }
      else {
        uVar15 = *(ushort *)(*(int *)((int)player + 0x27) + 0x9b);
        if ((uVar15 & 0x40) == 0) goto LAB_00447ea4;
        *(ushort *)(*(int *)((int)player + 0x27) + 0x9b) = uVar15 & 0xffbf;
        *(undefined1 *)(*(int *)((int)player + 0x27) + 0x95) = 0;
      }
      *(byte *)((int)g_game + 0x2bee) = *(byte *)((int)g_game + 0x2bee) | 1;
      FUN_0046c620((int)(4));
      FUN_00450f90();
    }
    sprintf(local_124.text,"ALLY%d",uVar17);
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)(local_124.text));
    if (bVar3 != 0) {
      bVar4 = *(byte *)(uVar17 + 0x108 + (int)piVar9);
      *(byte *)(uVar17 + 0x108 + (int)piVar9) = bVar4 ^ 1;
      FUN_00452960((int)(*(uint *)((int)piVar9 + 4)),(int)(player[1]),(int)(bVar4 ^ 1),(int)(0));
      if (*(char *)((int)piVar9 + 0x13f) == '\x05') {
        bVar3 = false;
      }
      else {
        bVar3 = *(char *)((int)piVar9 + 0x13f) == *(char *)((int)player + 0x13f);
      }
      if (bVar3) {
        FUN_00446e90((int)((int)piVar9));
        *(undefined1 *)((int)piVar9 + 0x13f) = 5;
        FUN_00452bd0((int)(piVar9));
      }
      if ((uint)*(byte *)(uVar17 + 0x113 + (int)piVar9) * 2 == 3 ||
          *(char *)(uVar17 + 0x108 + (int)piVar9) != '\0') {
        pcVar21 = "Ally";
      }
      else {
        pcVar21 = "Multi";
      }
      FUN_0047f1a0((int)((byte *)pcVar21),(int)(0));
      pcVar21 = "allied with";
      if (*(char *)(uVar17 + 0x108 + (int)piVar9) == '\0') {
        pcVar21 = "broke alliance with";
      }
      sprintf(local_124.text," %s %s",(char*)FUN_004c5740((int)((int)pcVar21)),(char*)player+0x2b);
      FUN_00463e50((int)(piVar9),(int)(local_124.text),(int)(4),(int)(0));
      *(byte *)((int)g_game + 0x2bee) = *(byte *)((int)g_game + 0x2bee) | 1;
      FUN_00450f90();
    }
    sprintf(local_124.text,"TEAMICONS%d",uVar17);
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)(local_124.text));
    if (bVar3 != 0) {
      FUN_0047f1a0((int)("Ally"),(int)(0));
      bVar4 = *(byte *)((int)g_game + 0x1ca2 + iVar6);
      piVar12 = (int *)((int)g_game + 0x1b63 + iVar6);
      FUN_00446e90((int)((int)piVar12));
      *(char *)((int)piVar12 + 0x13f) = (char)((bVar4 + 1) % 6);
      FUN_00452bd0((int)(piVar12));
      FUN_00446c70();
      FUN_00446a50();
      FUN_00452bd0((int)(player));
    }
    sprintf(local_124.text,"RES%d",uVar17);
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)(local_124.text));
    if (((bVar3 != 0) && (*player != 0)) &&
       (*(char *)((int)player + 0x73) == '\x01')) {
      FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
      FUN_00446310();
      FUN_004ab0a0((int)((int)param_1));
      *(byte *)((int)g_game + 0x2bee) = *(byte *)((int)g_game + 0x2bee) | 1;
      return;
    }
    sprintf(local_124.text,"READY%d",uVar17);
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)(local_124.text));
    if (((bVar3 != 0) && (*player != 0)) &&
       (*(char *)((int)player + 0x73) == '\x01')) {
      FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
      iVar6 = ((Class_004358f0*)(*(int *)((int)g_game + 0x391e9)))->FUN_004358f0();
      if (iVar6 != 0) {
        bVar4 = FUN_00456850();
        iVar6 = 0;
        bVar3 = false;
        if (bVar4 != 10) {
          iVar6 = *(int *)((int)g_game + (uint)bVar4 * 0x14b + 0x1b8a);
          if ((1 < *(byte *)(iVar6 + 0xa7)) ||
             ((*(byte *)(iVar6 + 0xa7) == 1 && (1 < *(byte *)(iVar6 + 0xa8))))) {
            bVar3 = true;
          }
        }
        if ((!bVar3) ||
           (uVar7 = ((Class_004373a0*)(*(int *)((int)g_game + 0x391e9)))->FUN_004373a0(), uVar7 == *(uint *)(iVar6 + 0xa9)
           )) {
          iVar6 = FUN_0049fdf0((int)(iVar8),(int)((char *)local_124.text),(int)(1));
          uVar7 = FUN_004a0f30((int)((int)g_game + 0x519),(int)(iVar6));
          *(ushort *)(*(int *)((int)player + 0x27) + 0x9b) =
               *(ushort *)(*(int *)((int)player + 0x27) + 0x9b) & 0xffdf | (ushort)((uVar7 & 1) << 5);
          if ((*(byte *)(*(int *)((int)player + 0x27) + 0x97) & 1) != 0) {
            strcpy((char*)(iVar8+0xcc),"START");
            iVar6 = FUN_0049fdf0((int)(iVar8),(int)("START"),(int)(1));
            *(int *)(*(int *)((int)g_game + 0x531) + 0x20) = iVar6;
          }
          iVar20 = 0;
          iVar6 = (int)g_game;
          do {
            if ((*(int *)(iVar6 + 0x1b63 + iVar20) != 0) &&
               ((cVar2 = *(char *)(iVar6 + 0x1bd6 + iVar20), cVar2 == '\x01' || (cVar2 == '\x02'))))
            {
              iVar16 = *(int *)(iVar6 + 0x1b8a + iVar20);
              uVar15 = *(ushort *)(iVar16 + 0x9b);
              *(ushort *)(iVar16 + 0x9b) =
                   (byte)(*(byte *)(*(int *)(iVar6 + (uint)*(byte *)(iVar6 + 0x2a42) * 0x14b +
                                            0x1b8a) + 0x9b) ^ (byte)uVar15) & 0x20 ^ uVar15;
              iVar6 = (int)g_game;
            }
            iVar20 = iVar20 + 0x14b;
          } while (iVar20 < 0xcee);
          *(byte *)(iVar6 + 0x2bee) = *(byte *)(iVar6 + 0x2bee) | 1;
          FUN_00450f90();
          goto LAB_00448300;
        }
      }
      FUN_004a1110((int)((int)g_game + 0x519),(int)((char *)local_124.text),(int)(0));
    }
LAB_00448300:
    uVar17 = uVar17 + 1;
  } while ((int)uVar17 < 10);
  if (uVar17 != *(ushort *)((int)g_game + 0x2a3c)) {
    *(ushort *)((int)g_game + 0x2bee) = *(ushort *)((int)g_game + 0x2bee) | 1;
  }
  bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"PREVMENU"));
  if (bVar3 != 0) {
    FUN_0047f1a0((int)((byte *)"Previous"),(int)(0));
    iVar5 = 0;
    iVar8 = (int)g_game;
    do {
      if ((*(int *)(iVar8 + 0x1b63 + iVar5) != 0) &&
         ((cVar2 = *(char *)(iVar8 + 0x1bd6 + iVar5), cVar2 == '\x01' || (cVar2 == '\x02')))) {
        FUN_00453010((int)(*(int **)(iVar8 + 0x1b67 + iVar5)),(int)(2));
        iVar8 = (int)g_game;
      }
      iVar5 = iVar5 + 0x14b;
    } while (iVar5 < 0xcee);
    *(undefined1 *)(iVar8 + 0x2bc0) = 3;
    return;
  }
  bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"MESSAGE"));
  if (bVar3 != 0) {
    iVar8 = FUN_004a0010((int)(iVar8),(int)("MESSAGE"));
    pbVar10 = (byte *)(iVar8 + 0xb6);
    if (strlen((char*)pbVar10) != 0) {
      uVar17 = _strcmpi((char*)pbVar10,(char *)"+syncerr");
      if (uVar17 == 0) {
        pcVar21 = (char*)((Class_0046df40*)(*(int *)((int)g_game + 0x2a30)))->FUN_0046df40();
        if (pcVar21 != (char *)0x0) {
          FUN_00463ca0((int)(pcVar21),(int)(4),(int)(0),(int)('\n'));
        }
      }
      else {
        FUN_00463e50((int)(piVar9),(int)(pbVar10),(int)(4),(int)(0));
        if (DAT_00506dbc != 0) {
          ((Class_004618a0*)(&DAT_00513000))->FUN_004618a0((int)(1));
        }
      }
      uVar17 = 0xffffffff;
      *(byte *)((int)g_game + 0x2bee) = *(byte *)((int)g_game + 0x2bee) | 1;
      pcVar21 = "";
      do {
        pcVar19 = pcVar21;
        if (uVar17 == 0) break;
        uVar17 = uVar17 - 1;
        pcVar19 = pcVar21 + 1;
        cVar2 = *pcVar21;
        pcVar21 = pcVar19;
      } while (cVar2 != '\0');
      uVar17 = ~uVar17;
      pbVar18 = (byte *)(pcVar19 + -uVar17);
      for (uVar7 = uVar17 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pbVar10 = *(undefined4 *)pbVar18;
        pbVar18 = pbVar18 + 4;
        pbVar10 = pbVar10 + 4;
      }
      for (uVar17 = uVar17 & 3; uVar17 != 0; uVar17 = uVar17 - 1) {
        *pbVar10 = *pbVar18;
        pbVar18 = pbVar18 + 1;
        pbVar10 = pbVar10 + 1;
      }
    }
    pcVar21 = (char *)FUN_0049fdf0((int)(*(int *)(*(int *)((int)g_game + 0x531) + 4)),(int)("MESSAGE"),(int)(3));
    FUN_004a7190((int)((int)g_game + 0x519),(int)(pcVar21));
    goto LAB_00448b98;
  }
  bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"COMMANDER"));
  if (bVar3 == 0) {
    bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"LOSTYPE"));
    if (bVar3 == 0) {
      bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"WATCHING"));
      if (bVar3 == 0) {
        bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"CHEATING"));
        if (bVar3 == 0) {
          bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"FIXEDLOC"));
          if (bVar3 == 0) {
            bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"MAPPING"));
            if (bVar3 == 0) {
              bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"START"));
              if (bVar3 != 0) {
                iVar5 = 0;
                FUN_0047f1a0((int)((byte *)"BigButton"),(int)(0));
                iVar8 = 10;
                pVar22 = (int *)((int)g_game + 0x1b8a);
                do {
                  if ((*(int *)((int)pVar22 - 0x27) != 0) &&
                     ((((char)pVar22[0x13] == '\x01' ||
                       (((*(int *)((int)pVar22 - 0x27) != 0 && ((char)pVar22[0x13] == '\x03')) &&
                        (*(char *)(*pVar22 + 0x94) == '\x01')))) &&
                      ((*(byte *)(*pVar22 + 0x9d) & 4) != 0)))) {
                    iVar5 = iVar5 + 1;
                  }
                  pVar22 = (int *)((int)pVar22 + 0x14b);
                  iVar8 = iVar8 + -1;
                } while (iVar8 != 0);
                if (((iVar5 < 1) || ((iVar5 < 2 && (iVar8 = FUN_00457af0(), 3 < iVar8)))) ||
                   ((iVar5 < 3 && (iVar8 = FUN_00457af0(), 6 < iVar8)))) {
                  FUN_004ab0a0((int)((int)g_game + 0x519));
                  iVar11 = 1;
                  iVar5 = 1;
                  iVar8 = 200;
                  pbVar10 = (byte*)FUN_004c5740((int)((byte *)"There are not enough game CDs present to play"));
                  FUN_004abd90((int)(param_1),(int)(pbVar10),(int)(iVar8),(int)(iVar5),(int)(iVar11));
                  return;
                }
                iVar8 = FUN_00457b40();
                iVar5 = FUN_00457af0();
                uVar17 = 0;
                do {
                  iVar20 = 0;
                  pcVar21 = (char *)((int)g_game + 0x1bd6);
                  iVar6 = 10;
                  do {
                    if ((*(byte *)((int)g_game + 0x2a44) >> 2 & 1) == 0) {
                      if (((((byte)pcVar21[0xcc] == uVar17) && (*(int *)(pcVar21 + -0x73) != 0)) &&
                          ((cVar2 = *pcVar21, cVar2 == '\x01' ||
                           ((cVar2 == '\x02' || (cVar2 == '\x03')))))) && (pcVar21[0xd3] != '\n'))
                      goto LAB_00448872;
                    }
                    else if ((((((byte)pcVar21[0xcc] == uVar17) && (*(int *)(pcVar21 + -0x73) != 0))
                              && ((cVar2 = *pcVar21, cVar2 == '\x01' ||
                                  ((cVar2 == '\x02' || (cVar2 == '\x03')))))) &&
                             (pcVar21[0xd3] != '\n')) &&
                            ((((cVar2 == '\x01' || (cVar2 == '\x02')) || (cVar2 == '\x03')) &&
                             ((*(short *)(pcVar21 + 0xd1) != 0 || (*(int *)(pcVar21 + 0xcd) == 0))))
                            )) {
LAB_00448872:
                      iVar20 = iVar20 + 1;
                    }
                    pcVar21 = pcVar21 + 0x14b;
                    iVar6 = iVar6 + -1;
                  } while (iVar6 != 0);
                  if (iVar20 == iVar8 + iVar5) {
                    FUN_004ab0a0((int)((int)g_game + 0x519));
                    iVar11 = 1;
                    iVar5 = 1;
                    iVar8 = 200;
                    pbVar10 = (byte*)FUN_004c5740((int)((byte *)"Can not start game with all players on the same team."));
                    FUN_004abd90((int)(param_1),(int)(pbVar10),(int)(iVar8),(int)(iVar5),(int)(iVar11));
                    return;
                  }
                  uVar17 = uVar17 + 1;
                } while ((int)uVar17 < 5);
                bVar3 = ((Class_00435c40*)(*(int *)((int)g_game + 0x391e9)))->FUN_00435c40();
                if (bVar3) {
                  iVar8 = (int)g_game;
                  if ((*(byte *)(*(int *)((int)piVar9 + 0x27) + 0x9b) & 0x80) == 0) {
                    iVar5 = 0;
                    do {
                      if (((*(int *)(iVar8 + 0x1b63 + iVar5) != 0) &&
                          (*(char *)(iVar8 + 0x1bd6 + iVar5) == '\x03')) &&
                         ((*(byte *)(*(int *)(iVar8 + 0x1b8a + iVar5) + 0x9b) & 0x40) != 0)) {
                        FUN_00453010((int)(*(int **)(iVar8 + 0x1b67 + iVar5)),(int)(9));
                        iVar8 = (int)g_game;
                      }
                      iVar5 = iVar5 + 0x14b;
                    } while (iVar5 < 0xcee);
                  }
                  *(undefined1 *)(iVar8 + 0x2bc0) = 0x11;
                  pbVar10 = (byte *)(*(int *)((int)piVar9 + 0x27) + 0x9b);
                  *pbVar10 = *pbVar10 | 0x10;
                  FUN_00451180();
                  *(uint *)((int)g_game + 0x39231) =
                       (*(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) & 0x200) >> 9;
                  *(uint *)((int)g_game + 0x39235) =
                       (*(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) & 0x400) >> 10;
                  *(uint *)((int)g_game + 0x39229) =
                       (*(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) & 0x1800) >> 0xb;
                  *(uint *)(*(int *)((int)g_game + 0x29a0) + 0x118) =
                       (*(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) & 0x4000) >> 0xe;
                  *(uint *)((int)g_game + 0x3922d) =
                       (*(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) & 0x100) >> 8;
                  FUN_00430f00();
                  *(undefined4 *)((int)g_game + 0x37eee) = 2;
                  return;
                }
                FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
                FUN_00444ea0();
                goto LAB_00448b98;
              }
              bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"GAMEOPEN"));
              if (bVar3 == 0) {
                bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"RESTRICTIONS"));
                if (bVar3 == 0) {
                  bVar3 = FUN_0049fd60((int)((int)param_1),(int)("MAP"));
                  if ((bVar3 != 0) ||
                     (bVar3 = FUN_0049fd60((int)((int)param_1),(int)((byte *)"MAPNAME")),
                     bVar3 != 0)) {
                    FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
                    if ((*(byte *)(*(int *)((int)piVar9 + 0x27) + 0x97) & 1) == 0) {
                      piVar9 = (int*)FUN_004aa8f0((int)((int)g_game + 0x519),(int)("VIEWMAP.GUI"),(int)(0x900));
                      piVar9[2] = (int)FUN_00444ba0;
                      FUN_004288d0((int)((byte *)"DVIEWMAP"),(int)(0),(int)(0),(int)(0));
                      FUN_00444a20();
                      FUN_0049fb10((int)((int)g_game + 0x519),(int)(1));
                      FUN_004a81e0((int)((int)g_game + 0x519),(int)(0x40));
                    }
                    else {
                      FUN_00444ea0();
                    }
                  }
                }
                else {
                  FUN_0047f1a0((int)((byte *)"Options"),(int)(0));
                  FUN_0044c7e0();
                  FUN_004ab0a0((int)((int)param_1));
                }
                goto LAB_00448b98;
              }
              FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
              uVar17 = FUN_004a0f60((int)((int)param_1),(int)("GAMEOPEN"));
              iVar8 = *(int *)((int)piVar9 + 0x27);
              uVar15 = *(ushort *)(iVar8 + 0x9b) & 0x7fff | (ushort)(uVar17 == 0) << 0xf;
            }
            else {
              FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
              uVar17 = FUN_004a0f60((int)((int)param_1),(int)("MAPPING"));
              iVar8 = *(int *)((int)piVar9 + 0x27);
              uVar15 = *(ushort *)(iVar8 + 0x9b) & 0xfeff | (ushort)(uVar17 == 0) << 8;
            }
            *(ushort *)(iVar8 + 0x9b) = uVar15;
            goto LAB_00448aaf;
          }
          FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
          iVar8 = *(int *)((int)piVar9 + 0x27);
          uVar15 = *(ushort *)(iVar8 + 0x9b);
          uVar14 = 0x4000;
        }
        else {
          FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
          iVar8 = *(int *)((int)piVar9 + 0x27);
          uVar15 = *(ushort *)(iVar8 + 0x9b);
          uVar14 = 0x2000;
        }
        *(ushort *)(iVar8 + 0x9b) = uVar14 ^ uVar15;
        FUN_00450f90();
        *(ushort *)((int)g_game + 0x2bee) = *(ushort *)((int)g_game + 0x2bee) | 1;
        goto LAB_00448b98;
      }
      FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
      *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) =
           *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) ^ 0x80;
      uVar15 = *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b);
      if ((((uVar15 & 0x80) == 0) && (*piVar9 != 0)) && ((uVar15 & 0x40) != 0)) {
        *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) = uVar15 & 0xffbf;
      }
    }
    else {
      FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
      iVar8 = *(int *)((int)piVar9 + 0x27);
      uVar15 = *(ushort *)(iVar8 + 0x9b);
      if ((uVar15 & 0x200) == 0) {
        *(ushort *)(iVar8 + 0x9b) = uVar15 | 0x200;
        pbVar10 = (byte *)(*(int *)((int)piVar9 + 0x27) + 0x9c);
        *pbVar10 = *pbVar10 | 4;
      }
      else if ((uVar15 & 0x400) == 0x400) {
        *(ushort *)(iVar8 + 0x9b) = uVar15 & 0xfbff;
      }
      else {
        *(ushort *)(iVar8 + 0x9b) = uVar15 & 0xfdff;
      }
    }
  }
  else {
    FUN_0047f1a0((int)((byte *)"Multi"),(int)(0));
    uVar15 = *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b);
    *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) =
         ((uVar15 & 0xf800) + 0x800 ^ uVar15) & 0x1800 ^ uVar15;
    uVar15 = *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b);
    if (0x1000 < (uVar15 & 0x1800)) {
      *(ushort *)(*(int *)((int)piVar9 + 0x27) + 0x9b) = uVar15 & 0xe7ff;
    }
  }
LAB_00448aaf:
  FUN_00450f90();
  FUN_00451180();
  *(ushort *)((int)g_game + 0x2bee) = *(ushort *)((int)g_game + 0x2bee) | 1;
LAB_00448b98:
  FUN_004ab0a0((int)((int)param_1));
  return;
}
