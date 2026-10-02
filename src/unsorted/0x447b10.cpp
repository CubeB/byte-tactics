// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by space-bunny-free. Names are provisional.
// Score history for this statement order (tools/check.py): 46.2% -> 82.8% claude-sonnet-5-5,
// 83.4% base as inherited, 83.6% here (the post-loop flag pair below).
// What the original does, each point verified against its own disassembly:
//  - the player info struct (+0x27) has 16-bit bitfields at +0x9b (ready, b6, watching,
//    b8..b10, cmdr:2, cheat, fixed, closed): reads give "mov cx,[..]; shr; test", writes
//    give "or byte [..],imm", "x ^= 1" gives "not edx; xor edx,eax; and edx,mask; xor". Use
//    PInfo/PLR below instead of raw masks; the 0x2a44 flag is a byte bitfield (GB).
//  - the PLAYER block tests "c != 4 && c != 0" first (cmp 4 / test 0 order), then "if (c == 4)".
//  - FUN_00452960's third parameter must be unsigned char (pushes eax after "mov al,dl").
//  - nested call arguments (FUN_004abd90(gui, FUN_004c5740(msg), 500, 1, 1)) push the
//    constants before the inner call; reusing iVar5/iVar8/iVar20 in the tail made the
//    frame 0x140, giving the tail its own locals gives the original 0x13c.
//  - g_game+0x2bee is written through TWO different lvalues. A byte bitfield (GB2::bflag,
//    "or byte ptr [eax+0x2bee],1") at 0x4482d9 (READY), 0x448347 and 0x448486 (MESSAGE) and
//    0x448bd1 (RES, after its FUN_004ab0a0 call). A 16-bit one
//    ("or word ptr [eax+0x2bee],bp", bp = the ushort local `one`) at 0x448365, 0x4486bc
//    (CHEATING/FIXEDLOC/WATCHING) and 0x448abe (COMMANDER/LOSTYPE/MAPPING/GAMEOPEN).
//    The post-loop pair is "or byte [eax+0x2bee],1" unconditionally at 0x448347, then
//    "mov ebp,1 / cmp ebp,[eax+0x2a3c] / je / or word [eax+0x2bee],bp". `one` is a ushort
//    local: it is also pushed as the argument of FUN_004618a0 at 0x44846e. Writing the
//    post-loop byte-or unconditionally and the word-or inside the if is worth +0.2% here;
//    converting the six tail bodies to the word form as well costs 0.7% (see below).
//  - the else-if bodies converge on two shared blocks: 0x448aaf (call FUN_00450f90, call
//    FUN_00451180, mov eax,[g], or word [eax+0x2bee],bp) for COMMANDER, LOSTYPE, MAPPING
//    and GAMEOPEN, and 0x4486a9 (xor edx,eax, store, call FUN_00450f90, mov eax,[g], or
//    word, ...) for WATCHING, CHEATING and FIXEDLOC, which jump into the middle of the
//    other's code. Our version merges some of these too.
// What still differs (83.6%, 4306 original / 4418 emitted):
//  - BIGGEST ONE, and the whole 112-byte excess: the trailing "FUN_004ab0a0(param_1); return;"
//    is a single block at 0x448b98 in the original, 9 insns / 25 bytes, reached by 6 "jmp",
//    1 "je" and one fall-through. Ours is 8 insns / 19 bytes and MSVC clones it into every
//    predecessor, so this file has 13 "pop edi" epilogues where the original has 7
//    (0x447b5f, 0x4483da, 0x4488c2, 0x448a15, 0x448a55, 0x448ba5, 0x448bce).
//    THE TRIGGER IS ONE INSTRUCTION IN THAT BLOCK: the original's block has one more
//    instruction than ours ("mov edx,[esp+0x150]; push edx" instead of "push ebp"), and any
//    surviving instruction there (a call or a store; the threshold is between 8 and 9
//    instructions, 19 and 24 bytes) stops the cloning and takes the file to 85.1% / 4336
//    bytes with exactly 7 epilogues. Probes: build/scratch/447b10/e24.cpp (a call) 85.1%,
//    e18.cpp and e2.cpp (a store) 85.1%, e27.cpp (a global store) 85.0%, e26.cpp
//    ("one = 1;", a store to a scalar local) 83.6% because the optimiser folds it away,
//    e1.cpp 85.0%. No legitimate extra statement has been found for that spot, so it is
//    left out here, per the rule that a partial should not carry a statement the original
//    does not have. Every jump target after +0x9c6 is shifted by the extra 91 bytes of
//    cloned epilogues.
//  - register split in the tail: the original keeps param_1 in esi (loaded once at 0x44836c,
//    "mov esi,[esp+0x150]") and `one` in ebp ("mov ebp,1" at 0x44835e); we pick ebp for
//    param_1 and esi for `one`, so eight "push esi" sites read "push ebp", the PREVMENU and
//    START-team loops load g_game into eax where the original uses ecx, and the READY block
//    uses edx/edi where the original uses ecx/edx. Moving the declaration of `one` to the
//    top of the function or into the if gives byte-identical code, so this is not reachable
//    from the declaration position.
//  - stack slot order: original (low to high) player ptr, iVar8, bVar1, stride, byte9,
//    iVar5; ours has iVar5 and bVar1 swapped (slot order follows static reference weight).
//    At the loop head the original computes the stride in eax and stores it in the player
//    slot ("lea eax,[ebp+ecx*2]; mov [esp+0x24],eax; lea ebx,[edx+eax+0x1b63]"), we keep it
//    in ebx and store the player pointer into +0x28 instead of +0x24.
//  - START team loop: the original wraps the inner 10-player loop in "if (t != 5)" (entry
//    guard "cmp ebx,5 / jne", count = 0 otherwise) and reads the flag2 bitfield once per
//    team ("mov dl,[ebp+0x2a44]; shr dl,2; and dl,1") before the inner loop. Both were
//    tried on this statement order and each scored lower (80.4%, and 83.6% at 3 bytes more),
//    because the extra branch shifts every later jump target and that costs more than the
//    shape it fixes. Inlining the LAB_notenough body at its goto (no separate tail block)
//    is byte-identical to the goto form and also changes nothing.
//  - tools worth rebuilding: build/scratch/447b10/shapediff.py (address-anchored shape diff,
//    immediates and displacements masked, with --runs for aligned identical runs) and
//    build/scratch/447b10/slots.py (esp slot tracker honouring ret N for __stdcall);
//    build/scratch/447b10/s.sh scores one scratch variant. tools/permute.py over 12 minutes
//    gained nothing here (its own metric 10756 -> 10751, 83.6% -> 83.7%), so the residual
//    is not a meaning-preserving rewrite of these statements.
//  - declared-variable experiments that all give byte-identical code here, so none of them
//    is the lever for the register split or the slot order: giving the loop stride its own
//    local, a tail-local copy of param_1, declaring `one` first / last / inside the if.
#include <stdio.h>
#include <string.h>
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
#pragma pack(push, 1)
struct PInfo {
  char unknown_0[0x94];
  char c94;
  byte c95;
  byte c96;
  byte f97_0 : 1;
  byte f97_rest : 7;
  char unknown_98[3];
  ushort b0_3 : 4;
  ushort b4 : 1;
  ushort ready : 1;
  ushort b6 : 1;
  ushort watching : 1;
  ushort b8 : 1;
  ushort b9 : 1;
  ushort b10 : 1;
  ushort cmdr : 2;
  ushort cheat : 1;
  ushort fixed : 1;
  ushort closed : 1;
  byte f9d_0 : 2;
  byte f9d_2 : 1;
  byte f9d_rest : 5;
  char unknown_9e[0xa7 - 0x9e];
  byte ca7;
  byte ca8;
  uint ia9;
};
struct PLR {
  int active;
  int id;
  uint time;
  char unknown_c[0x27 - 0xc];
  PInfo *info;
  char name[0x73 - 0x2b];
  char type;
  char unknown_74[0x108 - 0x74];
  byte ally[10];
  char unknown_112[0x113 - 0x112];
  byte ally2[10];
  char unknown_11d[0x13f - 0x11d];
  char team;
};
struct GB {
  char unknown_0[0x2a44];
  byte flag0 : 1;
  byte flag1 : 1;
  byte flag2 : 1;
  byte flag_rest : 5;
};
struct GB2 {
  char unknown_0[0x2bee];
  byte bflag : 1;
  byte brest : 7;
};
#pragma pack(pop)
extern char* g_game;
extern int DAT_00506dbc;
extern int DAT_00512994;
extern int DAT_00513000;
int __stdcall FUN_004288d0(int,int,int,int);
int FUN_00430f00();
struct Class_004358f0 { int FUN_004358f0(); };
struct Class_00435c40 { char FUN_00435c40(); };
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
int __stdcall FUN_00452960(int,int,unsigned char,int);
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
  int iVar5;
  int iVar6;
  int iVar8;
  PLR *player;
  uint bVar1;
  uint bVar4;
  char cVar2;
  int bVar3;
  uint uVar7;
  PLR *me;
  int *pVar22;
  byte *pbVar10;
  int *piVar12;
  ushort uVar15;
  int iVar16;
  uint uVar17;
  int iVar20;
  char *pcVar21;
  struct SlotBuf { char text [252]; int used [10]; };
  SlotBuf local_124;

  iVar8 = *(int *)(*(int *)(param_1 + 0x18) + 4);
  if (*(int *)(param_1 + 0x60) == -1) {
    FUN_004d85a0((int)(*(int **)((int)g_game + 0x2a9b)));
    *(int *)((int)g_game + 0x2a9b) = 0;
    DAT_00512994 = 0;
    FUN_00446c70();
    return;
  }
  bVar1 = *(byte *)((int)g_game + 0x2a42);
  me = (PLR *)((int)g_game + (uint)bVar1 * 0x14b + 0x1b63);
  iVar5 = FUN_00457a50();
  uVar17 = 0;
  do {
    iVar6 = uVar17 * 0x14b;
    player = (PLR *)((int)g_game + 0x1b63 + iVar6);
    sprintf(local_124.text,"LOGO%d",uVar17);
    bVar3 = FUN_0049fd60((int)param_1,(int)(local_124.text));
    if (((bVar3 != 0) && (player->active != 0)) &&
       ((player->type == 1 || (player->type == 2)))) {
      FUN_0047f1a0((int)"Multi",0);
      FUN_004526c0((int)player->info->c96 + 1);
      ((GB2 *)g_game)->bflag = 1;
      FUN_00450f90();
    }
    sprintf(local_124.text,"PLAYER%d",uVar17);
    bVar3 = FUN_0049fd60((int)param_1,(int)(local_124.text));
    if ((bVar3 != 0) && (uVar17 != bVar1)) {
      FUN_0047f1a0((int)"Multi",0);
      cVar2 = player->type;
      if ((cVar2 == '\0') && (iVar5 != 0)) {
        ((Class_00463c60*)(player))->FUN_00463c60(4);
        player->id = -1;
        (*(int *)((int)g_game + 0x499))--;
      }
      else {
        if (cVar2 != '\x04' && cVar2 != '\0') {
          if (((player->active == 0) || (cVar2 != '\x02')) ||
             (uVar7 = FUN_004b6340(), uVar7 - player->time <= 0x1e)) {
            if (((iVar5 != 0) && (player->active != 0)) && (player->type == '\x03')) {
              FUN_00446080((int)(uVar17));
            }
          }
          else {
            FUN_00453010(player->id,1);
            ((Class_00463c60*)(player))->FUN_00463c60(0);
          }
          goto LAB_00447e27;
        }
        if (cVar2 == '\x04') {
          ((Class_00463c60*)(player))->FUN_00463c60(0);
          (*(int *)((int)g_game + 0x499))++;
          FUN_00451180();
        }
        bVar4 = FUN_00456850();
        if (((PInfo *)*(int *)((int)g_game + (uint)bVar4 * 0x14b + 0x1b8a))->closed) {
          FUN_004abd90((int)g_game + 0x519,FUN_004c5740((int)"Can't add another player when game is closed."),500,1,1);
          ((Class_00463c60*)(player))->FUN_00463c60(0);
          ((GB2 *)g_game)->bflag = 1;
          break;
        }
        bVar4 = FUN_00456850();
        if ((((PInfo *)*(int *)((int)g_game + (uint)bVar4 * 0x14b + 0x1b8a))->cmdr != 2) && (iVar20 = FUN_00457b90(), iVar20 == 0)) {
          FUN_00451220((byte)uVar17,2);
          piVar12 = local_124.used;
          for (iVar20 = 10; iVar20 != 0; iVar20 = iVar20 + -1) {
            *piVar12 = 0;
            piVar12 = piVar12 + 1;
          }
          iVar16 = 10;
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
          iVar16 = 0;
          for (iVar20 = 0; iVar20 < 10; iVar20++) {
            if (local_124.used[iVar20] == 0) {
              iVar16 = iVar20;
              break;
            }
          }
          player->info->c96 = (byte)iVar16;
        }
      }
LAB_00447e27:
      ((GB2 *)g_game)->bflag = 1;
      FUN_00451180();
      FUN_00450f90();
    }
    sprintf(local_124.text,"SIDE%d",uVar17);
    bVar3 = FUN_0049fd60((int)param_1,(int)(local_124.text));
    if (bVar3 != 0) {
      FUN_0047f1a0((int)"Multi",0);
      if ((player->active != 0) && (player->info->b6)) {
        player->info->b6 = 0;
        player->info->c95 = 0;
      }
      else {
        player->info->c95++;
        if ((int)(uint)player->info->c95 >= *(int *)((int)g_game + 0x37f39)) {
          player->info->c95 = 0;
          bVar4 = FUN_00456850();
          if (((((PInfo *)*(int *)((int)g_game + (uint)bVar4 * 0x14b + 0x1b8a))->watching) &&
               (player->active != 0)) && (player->type == '\x01')) {
            player->info->b6 = 1;
          }
          else {
            FUN_004a1080((int)param_1,(int)(local_124.text),0);
            FUN_004a5f40((int)param_1,*(int *)(param_1 + 0x60));
          }
        }
      }
      ((GB2 *)g_game)->bflag = 1;
      FUN_0046c620(4);
      FUN_00450f90();
    }
    sprintf(local_124.text,"ALLY%d",uVar17);
    bVar3 = FUN_0049fd60((int)param_1,(int)(local_124.text));
    if (bVar3 != 0) {
      me->ally[uVar17] ^= 1;
      FUN_00452960(me->id,player->id,me->ally[uVar17],0);
      char bTeam;
      if (me->team == 5) {
        bTeam = false;
      }
      else {
        bTeam = me->team == player->team;
      }
      if (bTeam) {
        FUN_00446e90((int)me);
        me->team = 5;
        FUN_00452bd0((int)me);
      }
      if (((uint)me->ally2[uVar17] * 2 == 3) | me->ally[uVar17]) {
        FUN_0047f1a0((int)"Ally",0);
      }
      else {
        FUN_0047f1a0((int)"Multi",0);
      }
      sprintf(local_124.text," %s %s",(char*)FUN_004c5740(me->ally[uVar17] ? (int)"allied with" : (int)"broke alliance with"),(char*)((int)g_game + iVar6 + 0x1b8e));
      FUN_00463e50((int)me,(int)(local_124.text),4,0);
      ((GB2 *)g_game)->bflag = 1;
      FUN_00450f90();
    }
    sprintf(local_124.text,"TEAMICONS%d",uVar17);
    bVar3 = FUN_0049fd60((int)param_1,(int)(local_124.text));
    if (bVar3 != 0) {
      FUN_0047f1a0((int)"Ally",0);
      int tm = *(byte *)((int)g_game + 0x1ca2 + iVar6);
      PLR *pl2 = (PLR *)((int)g_game + 0x1b63 + iVar6);
      FUN_00446e90((int)pl2);
      pl2->team = (char)((tm + 1) % 6);
      FUN_00452bd0((int)pl2);
      FUN_00446c70();
      FUN_00446a50();
      FUN_00452bd0((int)player);
    }
    sprintf(local_124.text,"RES%d",uVar17);
    bVar3 = FUN_0049fd60((int)param_1,(int)(local_124.text));
    if (((bVar3 != 0) && (player->active != 0)) &&
       (player->type == '\x01')) {
      FUN_0047f1a0((int)"Multi",0);
      FUN_00446310();
      FUN_004ab0a0((int)param_1);
      ((GB2 *)g_game)->bflag = 1;
      return;
    }
    sprintf(local_124.text,"READY%d",uVar17);
    bVar3 = FUN_0049fd60((int)param_1,(int)(local_124.text));
    if (((bVar3 != 0) && (player->active != 0)) &&
       (player->type == '\x01')) {
      FUN_0047f1a0((int)"Multi",0);
      iVar6 = ((Class_004358f0*)(*(int *)((int)g_game + 0x391e9)))->FUN_004358f0();
      if (iVar6 != 0) {
        byte byte9 = FUN_00456850();
        PInfo *inf = 0;
        bVar3 = false;
        if (byte9 != 10) {
          inf = (PInfo *)*(int *)((int)g_game + (uint)byte9 * 0x14b + 0x1b8a);
          if ((inf->ca7 >= 2) ||
             ((inf->ca7 == 1 && (inf->ca8 >= 2)))) {
            bVar3 = true;
          }
        }
        if ((!bVar3) ||
           (uVar7 = ((Class_004373a0*)(*(int *)((int)g_game + 0x391e9)))->FUN_004373a0(), uVar7 == inf->ia9)) {
          iVar6 = FUN_0049fdf0(iVar8,(int)local_124.text,1);
          uVar7 = FUN_004a0f30((int)g_game + 0x519,iVar6);
          player->info->ready = uVar7 & 1;
          if (player->info->f97_0) {
            strcpy((char*)(iVar8+0xcc),"START");
            iVar6 = FUN_0049fdf0(iVar8,(int)"START",1);
            *(int *)(*(int *)((int)g_game + 0x531) + 0x20) = iVar6;
          }
          iVar20 = 0;
          iVar6 = (int)g_game;
          do {
            if ((*(int *)(iVar6 + 0x1b63 + iVar20) != 0) &&
               ((cVar2 = *(char *)(iVar6 + 0x1bd6 + iVar20), cVar2 == '\x01' || (cVar2 == '\x02'))))
            {
              ((PInfo *)*(int *)(iVar6 + 0x1b8a + iVar20))->ready =
                  ((PInfo *)*(int *)(iVar6 + (uint)*(byte *)(iVar6 + 0x2a42) * 0x14b + 0x1b8a))->ready;
              iVar6 = (int)g_game;
            }
            iVar20 = iVar20 + 0x14b;
          } while (iVar20 < 0xcee);
          *(byte *)(iVar6 + 0x2bee) = *(byte *)(iVar6 + 0x2bee) | 1;
          FUN_00450f90();
          goto LAB_00448300;
        }
      }
      FUN_004a1110((int)g_game + 0x519,(int)local_124.text,0);
    }
LAB_00448300:
    uVar17 = uVar17 + 1;
  } while ((int)uVar17 < 10);
  ushort one = 1;
  ((GB2 *)g_game)->bflag = 1;
  if (uVar17 != *(ushort *)((int)g_game + 0x2a3c)) {
    *(ushort *)((int)g_game + 0x2bee) |= one;
  }
  if (FUN_0049fd60((int)param_1,(int)"PREVMENU")) {
    FUN_0047f1a0((int)"Previous",0);
    iVar5 = 0;
    iVar8 = (int)g_game;
    do {
      if ((*(int *)(iVar8 + 0x1b63 + iVar5) != 0) &&
         ((cVar2 = *(char *)(iVar8 + 0x1bd6 + iVar5), cVar2 == '\x01' || (cVar2 == '\x02')))) {
        FUN_00453010(*(int *)(iVar8 + 0x1b67 + iVar5),2);
        iVar8 = (int)g_game;
      }
      iVar5 = iVar5 + 0x14b;
    } while (iVar5 < 0xcee);
    *(char *)(iVar8 + 0x2bc0) = 3;
    return;
  }
  if (FUN_0049fd60((int)param_1,(int)"MESSAGE")) {
    int msg = FUN_004a0010(iVar8,(int)"MESSAGE");
    pbVar10 = (byte *)(msg + 0xb6);
    if (strlen((char*)pbVar10) != 0) {
      if (_strcmpi((char*)pbVar10,"+syncerr") == 0) {
        pcVar21 = (char*)((Class_0046df40*)(*(int *)((int)g_game + 0x2a30)))->FUN_0046df40();
        if (pcVar21 != (char *)0x0) {
          FUN_00463ca0((int)pcVar21,4,0,'\n');
        }
      }
      else {
        FUN_00463e50((int)me,(int)pbVar10,4,0);
        if (DAT_00506dbc != 0) {
          ((Class_004618a0*)(&DAT_00513000))->FUN_004618a0(one);
        }
      }
      ((GB2 *)g_game)->bflag = 1;
      strcpy((char*)pbVar10,"");
    }
    pcVar21 = (char *)FUN_0049fdf0(*(int *)(*(int *)((int)g_game + 0x531) + 4),(int)"MESSAGE",3);
    FUN_004a7190((int)g_game + 0x519,(int)pcVar21);
  }
  else if (FUN_0049fd60((int)param_1,(int)"COMMANDER")) {
    FUN_0047f1a0((int)"Multi",0);
    me->info->cmdr++;
    if (me->info->cmdr > 2) {
      me->info->cmdr = 0;
    }
    FUN_00450f90();
    FUN_00451180();
    ((GB2 *)g_game)->bflag = 1;
  }
  else if (FUN_0049fd60((int)param_1,(int)"LOSTYPE")) {
    FUN_0047f1a0((int)"Multi",0);
    if (!me->info->b9) {
      me->info->b9 = 1;
      me->info->b10 = 1;
    }
    else if (me->info->b10 == 1) {
      me->info->b10 = 0;
    }
    else {
      me->info->b9 = 0;
    }
    FUN_00450f90();
    FUN_00451180();
    ((GB2 *)g_game)->bflag = 1;
  }
  else if (FUN_0049fd60((int)param_1,(int)"WATCHING")) {
    FUN_0047f1a0((int)"Multi",0);
    me->info->watching ^= 1;
    if (!me->info->watching && me->active != 0 && me->info->b6) {
      me->info->b6 = 0;
    }
    FUN_00450f90();
    FUN_00451180();
    ((GB2 *)g_game)->bflag = 1;
  }
  else if (FUN_0049fd60((int)param_1,(int)"CHEATING")) {
    FUN_0047f1a0((int)"Multi",0);
    me->info->cheat ^= 1;
    FUN_00450f90();
    ((GB2 *)g_game)->bflag = 1;
  }
  else if (FUN_0049fd60((int)param_1,(int)"FIXEDLOC")) {
    FUN_0047f1a0((int)"Multi",0);
    me->info->fixed ^= 1;
    FUN_00450f90();
    ((GB2 *)g_game)->bflag = 1;
  }
  else if (FUN_0049fd60((int)param_1,(int)"MAPPING")) {
    FUN_0047f1a0((int)"Multi",0);
    int m = FUN_004a0f60((int)param_1,(int)"MAPPING");
    me->info->b8 = (m == 0);
    FUN_00450f90();
    FUN_00451180();
    ((GB2 *)g_game)->bflag = 1;
  }
  else if (FUN_0049fd60((int)param_1,(int)"START")) {
    int nReady = 0;
    FUN_0047f1a0((int)"BigButton",0);
    int k10 = 10;
    pVar22 = (int *)((int)g_game + 0x1b8a);
    do {
      if ((((*(int *)((int)pVar22 - 0x27) != 0) && ((char)pVar22[0x13] == '\x01')) ||
           ((*(int *)((int)pVar22 - 0x27) != 0) && ((char)pVar22[0x13] == '\x03') &&
            (((PInfo *)*pVar22)->c94 == '\x01'))) &&
          (((PInfo *)*pVar22)->f9d_2)) {
        nReady = nReady + 1;
      }
      pVar22 = (int *)((int)pVar22 + 0x14b);
      k10 = k10 + -1;
    } while (k10 != 0);
    if (((nReady < 1) || ((nReady < 2 && (FUN_00457af0() > 3))) ||
       ((nReady < 3 && (FUN_00457af0() > 6))))) {
      goto LAB_notenough;
    }
    int sum = FUN_00457b40() + FUN_00457af0();
    int t = 0;
    do {
      int cnt = 0;
      pcVar21 = (char *)((int)g_game + 0x1bd6);
      int k = 10;
      do {
        if (((GB *)g_game)->flag2) {
          if (((((byte)pcVar21[0xcc] == t) && (*(int *)(pcVar21 + -0x73) != 0)) &&
              ((*pcVar21 == '\x01' || (*pcVar21 == '\x02') || (*pcVar21 == '\x03')))) &&
              (pcVar21[0xd3] != '\n') &&
              ((*pcVar21 == '\x01' || (*pcVar21 == '\x02') || (*pcVar21 == '\x03')) &&
               ((*(short *)(pcVar21 + 0xd1) != 0 || (*(int *)(pcVar21 + 0xcd) == 0))))) {
            cnt = cnt + 1;
          }
        }
        else {
          if (((((byte)pcVar21[0xcc] == t) && (*(int *)(pcVar21 + -0x73) != 0)) &&
              ((*pcVar21 == '\x01' || (*pcVar21 == '\x02') || (*pcVar21 == '\x03')))) &&
              (pcVar21[0xd3] != '\n')) {
            cnt = cnt + 1;
          }
        }
        pcVar21 = pcVar21 + 0x14b;
        k = k + -1;
      } while (k != 0);
      if (cnt == sum) {
        FUN_004ab0a0((int)g_game + 0x519);
        FUN_004abd90((int)param_1,FUN_004c5740((int)"Can not start game with all players on the same team."),200,1,1);
        return;
      }
      t = t + 1;
    } while (t < 5);
    if (!((Class_00435c40*)(*(int *)((int)g_game + 0x391e9)))->FUN_00435c40()) {
      FUN_0047f1a0((int)"Multi",0);
      FUN_00444ea0();
    }
    else {
      iVar8 = (int)g_game;
      if (!me->info->watching) {
        iVar5 = 0;
        do {
          if (((*(int *)(iVar8 + 0x1b63 + iVar5) != 0) &&
              (*(char *)(iVar8 + 0x1bd6 + iVar5) == '\x03')) &&
             (((PInfo *)*(int *)(iVar8 + 0x1b8a + iVar5))->b6)) {
            FUN_00453010(*(int *)(iVar8 + 0x1b67 + iVar5),9);
            iVar8 = (int)g_game;
          }
          iVar5 = iVar5 + 0x14b;
        } while (iVar5 < 0xcee);
      }
      *(char *)(iVar8 + 0x2bc0) = 0x11;
      me->info->b4 = 1;
      FUN_00451180();
      *(uint *)((int)g_game + 0x39231) = me->info->b9;
      *(uint *)((int)g_game + 0x39235) = me->info->b10;
      *(uint *)((int)g_game + 0x39229) = me->info->cmdr;
      *(uint *)(*(int *)((int)g_game + 0x29a0) + 0x118) = me->info->fixed;
      *(uint *)((int)g_game + 0x3922d) = me->info->b8;
      FUN_00430f00();
      *(int *)((int)g_game + 0x37eee) = 2;
      return;
    }
  }
  else if (FUN_0049fd60((int)param_1,(int)"GAMEOPEN")) {
    FUN_0047f1a0((int)"Multi",0);
    int m = FUN_004a0f60((int)param_1,(int)"GAMEOPEN");
    me->info->closed = (m == 0);
    FUN_00450f90();
    FUN_00451180();
    ((GB2 *)g_game)->bflag = 1;
  }
  else if (FUN_0049fd60((int)param_1,(int)"RESTRICTIONS")) {
    FUN_0047f1a0((int)"Options",0);
    FUN_0044c7e0();
    FUN_004ab0a0((int)param_1);
  }
  else if (FUN_0049fd60((int)param_1,(int)"MAP") || FUN_0049fd60((int)param_1,(int)"MAPNAME")) {
    FUN_0047f1a0((int)"Multi",0);
    if (me->info->f97_0) {
      FUN_00444ea0();
    }
    else {
      int *pg = (int*)FUN_004aa8f0((int)g_game + 0x519,(int)"VIEWMAP.GUI",0x900);
      pg[2] = (int)FUN_00444ba0;
      FUN_004288d0((int)"DVIEWMAP",0,0,0);
      FUN_00444a20();
      FUN_0049fb10((int)g_game + 0x519,1);
      FUN_004a81e0((int)g_game + 0x519,0x40);
    }
  }
  FUN_004ab0a0((int)param_1);
  return;
LAB_notenough:
  FUN_004ab0a0((int)g_game + 0x519);
  FUN_004abd90((int)param_1,FUN_004c5740((int)"There are not enough game CDs present to play"),200,1,1);
  return;
}