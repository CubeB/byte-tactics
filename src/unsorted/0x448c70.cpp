// Decompiled by deepseek-v4.1-flash, finished by GPT-6, checked by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// deepseek-v4.1-flash pass (issue 4098, 10 minute box shared with 0x447380/0x447b10):
// one check.py run, best stays 47.4% (3902 original / 3948 emitted), first
// mismatch still the +8 stack-slot shift for the loop counter/temp pair; no new
// variant tried, file unchanged.
// deepseek-v4.1-flash pass (issue 3565, 10 minute box): three check.py runs, best
// stays 47.4% (3902 original / 3948 emitted); the box went to 0x447380. First
// mismatch is still the +8 stack-slot shift for the loop counter/temp pair.
// deepseek-v4.1-flash pass (issue 3538, 10 minute box): one check.py run, best
// stays 47.4% (3902 original / 3948 emitted); no new variant was tried.
// deepseek-v4.1-flash pass (issue 3485): re-ran check.py once; best stays 47.4%
// (3902 original / 3948 emitted) with the residual exactly as described in the
// notes below (iVar17 EBX/EBP swap and the [esp+0x34]/[esp+0x30] slot roles).
// File restored to this best version unmodified.
// Retry (GPT-6.1-sol): the best remains 47.4% (3902 original / 3948 emitted).
// Four checker invocations in this pass: baseline 47.4%; a ushort scroll-start
// local scored 47.2%, a byte local_b8 scored 41.0%, and a separate uint
// scroll-start local was unchanged. The two lower-scoring variants were reverted.
// Partial, 47.4%: the frame now matches at 0xd4 (all four buffers at 0x48/0x68/0x7c/0xb0).
// Retry (deepseek-v4.1-flash) tried and reverted these, none changed the score:
//   declaring iVar17 before uVar4; uVar4 as uint (47.3); swapping the two ushort
//   loads; local_b4 as byte* (compile error elsewhere); reordering
//   local_b4/local_c0; folding the local_b4 sum differently. The ebx/ebp swap
//   between the top-loop counter (iVar17) and the 0x2a40 value (uVar4) survives
//   all of them, and everything else is a consequence of that plus the frame
//   slot rotation.
// How: dropping the local_a4/local_a0 temporaries (the PING widget pointer stays in iVar17 and
// the player byte stride is recomputed inline) took 0xdc -> 0xd8, then reusing the existing byte
// local bVar6 for the four iVar17+0x29 byte temps removed the last dedicated slot (0xd8 -> 0xd4).
// Still differs: the scalar slots are rotated by one (our local_c0 sits at 0x28 where the
// original has its first slot at 0x24, and our local_b4 pointer lands at 0x44 instead of the
// original's 0x30 via lea eax,[esi+eax*2+0x1b63]), the top-loop ebx/ebp pair is swapped
// (we zero ebx for iVar17 and use ebp for the 0x2a40 ushort), and the player-loop register
// allocation and the map-refresh region shape still differ.
// Fixes that gained points: FUN_00435c40 declared bool (removed a neg/sbb/neg),
// reading the selected slot pointer before the map-slot local, and clearing
// local_b8 with 0 instead of & 0xffffff00.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
extern char* g_game;
struct Class_004358f0 { int FUN_004358f0(); };
struct Class_00435920 { int FUN_00435920(); };
struct Class_00435a20 { int FUN_00435a20(int); };
struct Class_00435c20 { int FUN_00435c20(); };
struct Class_00435c30 { int FUN_00435c30(); };
struct Class_00435c40 { bool FUN_00435c40(); };
struct Class_004373a0 { int FUN_004373a0(); };
int FUN_00444a20();
int FUN_00445ed0();
int FUN_00446a50();
int FUN_00446c70();
int __stdcall FUN_0044ffd0(int);
int FUN_00450f90();
int FUN_00451180();
int __stdcall FUN_00453010(int,int);
int FUN_00456850();
int FUN_00457a50();
int __stdcall FUN_00463e50(int,int,int,int);
struct Class_0046d040 { int FUN_0046e0b0(int); };
int __stdcall FUN_0049fa90(int);
int __stdcall FUN_0049fdf0(int,int,int);
int __stdcall FUN_0049ff10(int,int);
int __stdcall FUN_0049ff90(int,int);
int __stdcall FUN_004a0180(int,int);
int __stdcall FUN_004a0280(int,int);
int __stdcall FUN_004a0570(int,int,int);
int __stdcall FUN_004a0bf0(int,int,int,int);
int __stdcall FUN_004a1080(int,int,int);
int __stdcall FUN_004a1110(int,int,int);
int __stdcall FUN_004a1450(int,int,int);
int FUN_004a50b0();
int __stdcall FUN_004a5d50(int,int);
int __stdcall FUN_004ab060(int,int);
int FUN_004b6340();
int __stdcall FUN_004b6af0(int,int);
int __stdcall FUN_004c5740(int);
void FUN_00444ba0();
// FUNCTION: 0x448c70
void FUN_00448c70(void)

{
  ushort *puVar1;
  int *piVar2;
  char cVar3;
  ushort uVar4;
  bool bVar5;
  byte bVar6;
  int uVar7;
  int iVar8;
  char *pcVar9;
  byte *pbVar10;
  byte *pbVar11;
  byte *pbVar12;
  int iVar13;
  uint uVar15;
  uint uVar16;
  int iVar17;
  uint uVar18;
  char *pcVar19;
  char *pcVar20;
  char local_d4 [20];
  uint local_c0;
  int local_bc;
  uint local_b8;
  int local_b4;
  uint local_b0;
  byte *local_ac;
  int *local_a8;
  byte local_9c [32];
  char local_7c [20];
  byte local_68 [52];
  char local_34 [52];

  uVar18 = (uint)*(byte *)((int)g_game + 0x2a42);
  local_b0 = 0xffffffff;
  iVar17 = 0;
  local_c0 = (*(byte *)(*(int *)((int)g_game + uVar18 + 0x1b8a + uVar18 * 0x14a) + 0x9b) & 0x20) >>
             5;
  local_b4 = (int)g_game + uVar18 + 0x1b63 + uVar18 * 0x14a;
  iVar8 = FUN_0049ff90((int)(*(int *)(*(int *)((int)g_game + 0x531) + 4)),(int)("OUTPUT"));
  uint scrollStart = (uint)*(ushort *)((int)g_game + 0x2a3e);
  uVar4 = *(ushort *)((int)g_game + 0x2a40);
  if ((int)scrollStart < (int)uVar4) {
    scrollStart = scrollStart + 0x1e;
  }
  local_bc = iVar8;
  uVar7 = FUN_004a50b0();
  if (((int)(scrollStart - uVar4) > (int)*(short *)(iVar8 + 0x19) / (uVar7 + 2))
     && (*(short *)((int)g_game + 0x2a40) = *(short *)((int)g_game + 0x2a40) + 1,
        *(ushort *)((int)g_game + 0x2a40) >= 0x1e)) {
    *(undefined2 *)((int)g_game + 0x2a40) = 0;
  }
  if ((*(ushort *)((int)g_game + 0x2a3e) != *(ushort *)((int)g_game + 0x2a40)) &&
     (uVar18 = (uint)*(ushort *)((int)g_game + 0x2a40),
     *(ushort *)((int)g_game + 0x2a3e) != uVar18)) {
    do {
      pcVar9 = (char *)FUN_004b6af0((int)(*(int *)((int)g_game + 0x2a9b)),(int)(iVar17));
      strcpy(pcVar9,g_game+0x12ef+uVar18*0x48);
      ++iVar17;
      ++uVar18;
      if (uVar18 == 0x1e) {
        uVar18 = 0;
      }
      iVar8 = local_bc;
    } while (*(ushort *)((int)g_game + 0x2a3e) != uVar18);
  }
  FUN_00445ed0();
  *(short *)(iVar8 + 0xc0) = (short)iVar17;
  iVar8 = FUN_004a0180((int)(*(int *)(*(int *)((int)g_game + 0x531) + 4)),(int)("MAPNAME"));
  pbVar10 = (byte *)((Class_00435c30*)(*(int *)((int)g_game + 0x391e9)))->FUN_00435c30();
  bVar5 = ((Class_00435c40*)(*(int *)((int)g_game + 0x391e9)))->FUN_00435c40();
  if (!bVar5) {
    *(undefined4 *)(iVar8 + 0x23) = 0xc;
    FUN_004a0bf0((int)((int)g_game + 0x519),(int)("MAPNAME"),(int)((byte *)"NOT SELECTED"),(int)(0));
    goto LAB_0044904e;
  }
  pbVar11 = (byte *)((Class_00435c20*)(*(int *)((int)g_game + 0x391e9)))->FUN_00435c20();
  local_ac = (byte *)(iVar8 + 0xb6);
  iVar17 = strcmp((char*)local_ac,(char*)pbVar11);
  if (iVar17 != 0) {
    iVar13 = FUN_004ab060((int)((int)g_game + 0x519),(int)("viewmap.gui"));
    if (iVar13 == 0) {
      ((Class_00435a20*)(*(void **)((int)g_game + 0x391e9)))->FUN_00435a20((int)(pbVar10));
    }
    else {
      FUN_00444a20();
    }
  }
  iVar13 = ((Class_004358f0*)(*(int *)((int)g_game + 0x391e9)))->FUN_004358f0();
  if (iVar13 == 0) {
LAB_00448f38:
    uVar18 = FUN_004b6340();
    *(uint *)(iVar8 + 0x23) = ((int)uVar18 / 30 & 1) ? 0xc : 0;
    if (iVar17 != 0) {
      bVar6 = 4;
      pbVar10 = (byte*)FUN_004c5740((int)((byte *)"does not have this map"));
      iVar8 = local_b4;
      FUN_00463e50((int)(local_b4),(int)(pbVar10),(int)(bVar6),(int)(0));
      puVar1 = (ushort *)(*(int *)(iVar8 + 0x27) + 0x9b);
      *puVar1 = *puVar1 & 0xffdf;
      sprintf(local_d4,"READY%d",*(byte*)(g_game+0x2a42));
      FUN_004a1110((int)((int)g_game + 0x519),(int)(local_d4),(int)(0));
      FUN_00450f90();
    }
    if ((*(byte *)(*(int *)((int)g_game + (uint)*(byte *)((int)g_game + 0x2a42) * 0x14b + 0x1b8a)
                  + 0x97) & 1) == 0) {
      uVar18 = 1;
      goto LAB_00449010;
    }
  }
  else {
    bVar6 = FUN_00456850();
    iVar13 = 0;
    bVar5 = false;
    local_bc = ((local_bc & 0xffffff00) | bVar6);
    if (bVar6 != 10) {
      iVar13 = *(int *)((int)g_game + (uint)bVar6 * 0x14b + 0x1b8a);
      if ((1 < *(byte *)(iVar13 + 0xa7)) ||
         ((*(byte *)(iVar13 + 0xa7) == 1 && (1 < *(byte *)(iVar13 + 0xa8))))) {
        bVar5 = true;
      }
    }
    if ((bVar5) &&
       (uVar18 = ((Class_004373a0*)(*(int *)((int)g_game + 0x391e9)))->FUN_004373a0(), uVar18 != *(uint *)(iVar13 + 0xa9))
       ) goto LAB_00448f38;
    *(undefined4 *)(iVar8 + 0x23) = 0;
    uVar18 = 0;
LAB_00449010:
    FUN_004a1450((int)((int)g_game + 0x519),(int)("MAP"),(int)(uVar18));
  }
  pcVar9 = (char *)((Class_00435c20*)(*(int *)((int)g_game + 0x391e9)))->FUN_00435c20();
  strcpy((char*)local_ac, pcVar9);
LAB_0044904e:
  iVar17 = 0;
  iVar8 = (int)g_game;
  do {
    if ((*(int *)(iVar8 + 0x1b63 + iVar17) != 0) &&
       ((cVar3 = *(char *)(iVar8 + 0x1bd6 + iVar17), cVar3 == '\x01' || (cVar3 == '\x02')))) {
      iVar13 = *(int *)(iVar8 + 0x1b8a + iVar17);
      uVar4 = *(ushort *)(iVar13 + 0x9b);
      *(ushort *)(iVar13 + 0x9b) =
           (byte)(*(byte *)(*(int *)(iVar8 + (uint)*(byte *)(iVar8 + 0x2a42) * 0x14b + 0x1b8a) +
                           0x9b) ^ (byte)uVar4) & 0x20 ^ uVar4;
      iVar8 = (int)g_game;
    }
    iVar17 = iVar17 + 0x14b;
  } while (iVar17 < 0xcee);
  iVar17 = 0;
  do {
    piVar2 = (int *)(iVar8 + 0x1b63 + iVar17);
    if ((((*(ushort *)(*(int *)(iVar8 + (uint)*(byte *)(iVar8 + 0x2a42) * 0x14b + 0x1b8a) + 0x9b) &
          0x1800) == 0x1000) && (*piVar2 != 0)) &&
       ((*(char *)((int)piVar2 + 0x73) == '\x02' ||
        (((*piVar2 != 0 && (*(char *)((int)piVar2 + 0x73) == '\x03')) &&
         (*(char *)(*(int *)((int)piVar2 + 0x27) + 0x94) == '\x02')))))) {
      FUN_00453010((int)((int *)piVar2[1]),(int)(0xb));
      FUN_00450f90();
    }
    bVar6 = FUN_00456850();
    if (((bVar6 != 10) &&
        (bVar6 = FUN_00456850(),
        (*(byte *)(*(int *)((int)g_game + (uint)bVar6 * 0x14b + 0x1b8a) + 0x9b) & 0x80) == 0)) &&
       (*piVar2 != 0)) {
      uVar4 = *(ushort *)(*(int *)((int)piVar2 + 0x27) + 0x9b);
      if ((uVar4 & 0x40) != 0) {
        *(ushort *)(*(int *)((int)piVar2 + 0x27) + 0x9b) = uVar4 & 0xffbf;
        *(undefined1 *)(*(int *)((int)piVar2 + 0x27) + 0x95) = 0;
        FUN_00450f90();
      }
    }
    iVar17 = iVar17 + 0x14b;
    iVar8 = (int)g_game;
  } while (iVar17 < 0xcee);
  FUN_00446c70();
  FUN_00446a50();
  local_b8 = 0;
  iVar8 = *(int *)(*(int *)((int)g_game + 0x531) + 4);
  local_a8 = (int *)((int)g_game + (uint)*(byte *)((int)g_game + 0x2a42) * 0x14b + 0x1b63);
  local_bc = iVar8;
  do {
    uVar18 = local_b8 & 0xff;
    piVar2 = (int *)((int)g_game + 0x1b63 + uVar18 * 0x14b);
    if ((*piVar2 == 0) ||
       (((((cVar3 = *(char *)((int)piVar2 + 0x73), cVar3 != '\x01' && (cVar3 != '\x02')) &&
          (cVar3 != '\x03')) || (*(char *)((int)piVar2 + 0x146) == '\n')) &&
        ((*piVar2 == 0 || ((*(byte *)(*(int *)((int)piVar2 + 0x27) + 0x9b) & 0x40) == 0)))))) {
      sprintf(local_d4,"CD%d",(byte)local_b8);
      FUN_004a0570((int)((int)g_game + 0x519),(int)(local_d4),(int)(0));
      FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(0));
      sprintf(local_d4,"PLAYER%d",(byte)local_b8);
      pcVar9 = "UNUSED";
      if (*(char *)((int)piVar2 + 0x73) == '\x04') {
        sprintf(local_34,"[%s]",(char*)FUN_004c5740((int)"BLOCKED"));
        pcVar9 = local_34;
      }
      strncpy((char *)local_9c,pcVar9,0x1e);
      FUN_004a0bf0((int)((int)g_game + 0x519),(int)(local_d4),(int)(local_9c),(int)(0));
      iVar17 = FUN_0049fdf0((int)(iVar8),(int)(local_d4),(int)(0xe));
      FUN_004a5d50((int)((int)g_game + 0x519),(int)(iVar17));
      FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(local_c0));
      sprintf(local_d4,"LOGO%d",(byte)local_b8);
      iVar17 = FUN_004a0280((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        *(undefined1 *)(iVar17 + 0x29) = 0;
      }
      sprintf(local_d4,"SIDE%d",(byte)local_b8);
      iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        *(undefined1 *)(iVar17 + 0x29) = 0;
      }
      if ((char)local_b8 != *(char *)((int)g_game + 0x2a42)) {
        sprintf(local_d4,"ALLY%d",(byte)local_b8);
        iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
        if (iVar17 != 0) {
          *(undefined1 *)(iVar17 + 0x29) = 0;
        }
      }
      sprintf(local_d4,"TEAMICONS%d",(byte)local_b8);
      iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        *(undefined1 *)(iVar17 + 0x29) = 0;
      }
      sprintf(local_d4,"RES%d",(byte)local_b8);
      iVar17 = FUN_004a0180((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        *(undefined1 *)(iVar17 + 0x29) = 0;
      }
      sprintf(local_d4,"PING%d",(byte)local_b8);
      iVar17 = FUN_004a0180((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        *(undefined1 *)(iVar17 + 0x29) = 0;
      }
      sprintf(local_d4,"MEM%d",(byte)local_b8);
      iVar17 = FUN_004a0180((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        *(undefined1 *)(iVar17 + 0x29) = 0;
      }
      sprintf(local_d4,"READY%d",(byte)local_b8);
      iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        *(byte *)(iVar17 + 0x13c) = *(byte *)(iVar17 + 0x13c) | 1;
        *(undefined2 *)(iVar17 + 0x138) = 0;
        *(undefined1 *)(iVar17 + 0x29) = 0;
      }
    }
    else {
      sprintf(local_d4,"CD%d",(byte)local_b8);
      if ((*piVar2 == 0) ||
         (((*(char *)((int)piVar2 + 0x73) != '\x01' &&
           (((*piVar2 == 0 || (*(char *)((int)piVar2 + 0x73) != '\x03')) ||
            (*(char *)(*(int *)((int)piVar2 + 0x27) + 0x94) != '\x01')))) ||
          ((*(byte *)(*(int *)((int)piVar2 + 0x27) + 0x9d) & 4) == 0)))) {
        iVar17 = 0;
      }
      else {
        iVar17 = 1;
      }
      FUN_004a0570((int)((int)g_game + 0x519),(int)(local_d4),(int)(iVar17));
      FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(0));
      sprintf(local_d4,"LOGO%d",(byte)local_b8);
      iVar17 = FUN_004a0280((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        if ((*(char *)(*(int *)((int)piVar2 + 0x27) + 0x96) == -1) && (local_c0 == 0)) {
          bVar6 = 0;
        }
        else {
          bVar6 = 1;
        }
        *(undefined1 *)(iVar17 + 0x29) = bVar6;
        *(uint *)(iVar17 + 200) = (uint)(local_c0 == 0) | *(uint *)(iVar17 + 200) & 0xfffffffe;
        *(undefined4 *)(iVar17 + 0xbe) = *(undefined4 *)((int)g_game + 0x148db);
        *(ushort *)(iVar17 + 0xc6) = (ushort)*(byte *)(*(int *)((int)piVar2 + 0x27) + 0x96);
      }
      sprintf(local_d4,"PLAYER%d",(byte)local_b8);
      strncpy((char *)local_9c,(char *)((int)piVar2 + 0x2b),0x1e);
      FUN_004a0bf0((int)((int)g_game + 0x519),(int)(local_d4),(int)(local_9c),(int)(0));
      iVar17 = FUN_0049fdf0((int)(iVar8),(int)(local_d4),(int)(0xe));
      FUN_004a5d50((int)((int)g_game + 0x519),(int)(iVar17));
      FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(local_c0));
      sprintf(local_d4,"SIDE%d",(byte)local_b8);
      iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        iVar8 = (int)g_game + uVar18 * 0x14b;
        sprintf(local_7c,"SIDE%d",uVar18);
        if ((*(int *)(iVar8 + 0x1b63) == 0) ||
           ((*(byte *)(*(int *)(iVar8 + 0x1b8a) + 0x9b) & 0x40) == 0)) {
          bVar6 = *(undefined1 *)(*(int *)(iVar8 + 0x1b8a) + 0x95);
        }
        else {
          bVar6 = 2;
        }
        FUN_004a1080((int)((int)g_game + 0x519),(int)(local_7c),(int)(bVar6));
        *(undefined1 *)(iVar17 + 0x29) = 1;
        if ((*piVar2 == 0) ||
           (((*(char *)((int)piVar2 + 0x73) != '\x01' && (*(char *)((int)piVar2 + 0x73) != '\x02'))
            || (local_c0 != 0)))) {
          uVar15 = 1;
        }
        else {
          uVar15 = 0;
        }
        FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(uVar15));
        iVar8 = local_bc;
      }
      sprintf(local_d4,"ALLY%d",(byte)local_b8);
      iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        FUN_004a1080((int)((int)g_game + 0x519),(int)(local_d4),(int)(*(char *)(local_b4 + 0x113 + uVar18) << 1 |
                     *(byte *)(local_b4 + 0x108 + uVar18)));
        iVar13 = *piVar2;
        if (((iVar13 == 0) ||
            ((*(char *)((int)piVar2 + 0x73) != '\x01' &&
             ((iVar13 == 0 ||
              (((*(byte *)(*(int *)((int)piVar2 + 0x27) + 0x9b) & 0x40) == 0 &&
               ((iVar13 == 0 ||
                ((*(char *)((int)piVar2 + 0x73) != '\x02' &&
                 (((iVar13 == 0 || (*(char *)((int)piVar2 + 0x73) != '\x03')) ||
                  (*(char *)(*(int *)((int)piVar2 + 0x27) + 0x94) != '\x02')))))))))))))) &&
           ((*local_a8 == 0 || ((*(byte *)(*(int *)((int)local_a8 + 0x27) + 0x9b) & 0x40) == 0)))) {
          bVar6 = 1;
        }
        else {
          bVar6 = 0;
        }
        *(undefined1 *)(iVar17 + 0x29) = bVar6;
        FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(local_c0));
      }
      sprintf(local_d4,"TEAMICONS%d",(byte)local_b8);
      iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        if ((*piVar2 == 0) || ((*(byte *)(*(int *)((int)piVar2 + 0x27) + 0x9b) & 0x40) == 0)) {
          bVar6 = 1;
        }
        else {
          bVar6 = 0;
        }
        *(undefined1 *)(iVar17 + 0x29) = bVar6;
        if ((*piVar2 == 0) ||
           (((*(char *)((int)piVar2 + 0x73) != '\x01' && (*(char *)((int)piVar2 + 0x73) != '\x02'))
            || (local_c0 != 0)))) {
          uVar18 = 1;
        }
        else {
          uVar18 = 0;
        }
        FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(uVar18));
      }
      sprintf(local_d4,"RES%d",(byte)local_b8);
      if ((*piVar2 == 0) ||
         ((*(char *)((int)piVar2 + 0x73) != '\x01' &&
          (((*piVar2 == 0 || (*(char *)((int)piVar2 + 0x73) != '\x03')) ||
           (*(char *)(*(int *)((int)piVar2 + 0x27) + 0x94) != '\x01')))))) {
        sprintf((char*)local_68,"%s","n/a");
      }
      else {
        sprintf((char*)local_68,"%dx%d",*(ushort*)(*(int*)((int)piVar2+0x27)+0x8b),*(ushort*)(*(int*)((int)piVar2+0x27)+0x8d));
      }
      FUN_004a0bf0((int)((int)g_game + 0x519),(int)(local_d4),(int)(local_68),(int)(0));
      if ((*piVar2 == 0) || (*(char *)((int)piVar2 + 0x73) != '\x01')) {
        iVar17 = FUN_004a0180((int)(iVar8),(int)(local_d4));
        if (iVar17 != 0) {
          *(undefined1 *)(iVar17 + 0x29) = 1;
        }
      }
      else {
        FUN_004a1450((int)((int)g_game + 0x519),(int)(local_d4),(int)(local_c0));
      }
      sprintf(local_d4,"PING%d",(byte)local_b8);
      iVar17 = FUN_004a0180((int)(iVar8),(int)(local_d4));
      if (((*piVar2 == 0) || (*(char *)((int)piVar2 + 0x73) != '\x03')) ||
         (*(char *)(*(int *)((int)piVar2 + 0x27) + 0x94) != '\x01')) {
        FUN_004a0bf0((int)((int)g_game + 0x519),(int)(local_d4),(int)("n/a"),(int)(0));
        if (iVar17 != 0) goto LAB_00449a19;
      }
      else if (iVar17 != 0) {
        local_ac = (byte *)(iVar17 + 0xb6);
        _itoa(piVar2[5],(char *)local_ac,10);
        iVar13 = FUN_00457a50();
        if (iVar13 != 0) {
          uVar18 = FUN_0044ffd0((int)((byte)local_b8));
          iVar8 = ((Class_0046d040*)(*(void **)((int)g_game + 0x2a30)))->FUN_0046e0b0((int)(uVar18));
          pcVar9 = ":s";
          if (iVar8 == 0) {
            pcVar9 = "";
          }
          strcat((char*)local_ac,pcVar9);
          iVar8=local_bc;
        }
        if ((uint)piVar2[5] <= local_b0) {
          local_b0 = piVar2[5];
        }
LAB_00449a19:
        *(undefined1 *)(iVar17 + 0x29) = 1;
      }
      sprintf(local_d4,"MEM%d",(byte)local_b8);
      iVar17 = FUN_004a0180((int)(iVar8),(int)(local_d4));
      if ((*piVar2 == 0) ||
         ((*(char *)((int)piVar2 + 0x73) != '\x01' &&
          (((*piVar2 == 0 || (*(char *)((int)piVar2 + 0x73) != '\x03')) ||
           (*(char *)(*(int *)((int)piVar2 + 0x27) + 0x94) != '\x01')))))) {
        sprintf((char *)(iVar17 + 0xb6),"%s","n/a");
        iVar13 = *(int *)((int)g_game + 0x391e9);
      }
      else {
        sprintf((char *)(iVar17 + 0xb6),"%d",*(ushort*)(*(int*)((int)piVar2+0x27)+0x99));
        iVar13 = *(int *)((int)g_game + 0x391e9);
      }
      iVar13 = ((Class_00435920*)(iVar13))->FUN_00435920();
      uVar4 = *(ushort *)(*(int *)((int)piVar2 + 0x27) + 0x99);
      *(undefined1 *)(iVar17 + 0x29) = 1;
      *(uint *)(iVar17 + 0x23) = (iVar13 <= (int)(uint)uVar4) - 1 & 0xc;
      sprintf(local_d4,"READY%d",(byte)local_b8);
      iVar17 = FUN_0049ff10((int)(iVar8),(int)(local_d4));
      if (iVar17 != 0) {
        bVar6 = *(byte *)(*(int *)((int)g_game + 0x1b8a + (uint)(byte)local_b8 * 0x14b) + 0x9b);
        *(undefined1 *)(iVar17 + 0x29) = 1;
        *(ushort *)(iVar17 + 0x138) = bVar6 >> 5 & 1;
        if ((*piVar2 == 0) || (*(char *)((int)piVar2 + 0x73) != '\x01')) {
          bVar5 = false;
        }
        else {
          bVar5 = true;
        }
        *(ushort *)(iVar17 + 0x13c) =
             (byte)(!bVar5 ^ (byte)*(ushort *)(iVar17 + 0x13c)) & 1 ^ *(ushort *)(iVar17 + 0x13c);
      }
    }
    bVar6 = (char)local_b8 + 1;
    local_b8 = ((local_b8 & 0xffffff00) | bVar6);
    if (9 < bVar6) {
      FUN_0049fa90((int)((int)g_game + 0x519));
      iVar8 = *(int *)(local_b4 + 0x27);
      if (((*(byte *)(iVar8 + 0x97) & 1) != 0) && (local_b0 < *(ushort *)(iVar8 + 0x9f))) {
        *(short *)(iVar8 + 0x9f) = (short)local_b0;
        FUN_00451180();
      }
      return;
    }
  } while( true );
}
