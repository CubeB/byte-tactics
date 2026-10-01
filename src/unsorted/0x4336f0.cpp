// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// GPT-6.1-sol (#3157 retry): baseline rechecked at 86.1% (712/708), one scored run. Two helper variants failed to compile or resolve; best unchanged, no MATCH.
// space-bunny-free pass: kept the 86.1% file unchanged (it is the best known)
// and mapped what is left with a byte-exact diff (relocation fields masked),
// because check.py's difflib score hides one whole class of difference.
//
// Remaining difference 1: the three extra `test ebp,ebp / jle` pairs. Confirmed
// still present with every loop spelling I tried in the 3-top-tested arms
// (`while (count > 0)`, `for (; count > 0; count--)`, `for (; count; --count)`),
// so it is not the spelling of the top test, only its existence. Also confirmed
// the allocation flip is NOT sensitive to any declaration shape: 7 decl sets
// (count before n, `int count = (short)atoi(tok)`, resize(count) vs resize(n)
// vs resize((int)n), unsigned count, long count, i declared before count) all
// give byte-identical code, 85.9% on the difflib scale, same 712 bytes.
// So the edi/ebp/ebx rotation really is bought only by the extra top tests.
//
// Remaining difference 2, which NO score has shown before: in all four arms the
// original emits `call strtok; mov ebx,[esi+4]; add esp,8` (the reload of
// _First lands immediately AFTER the call) where every variant here, including
// the 4-do-while one, emits `mov ebx,[esi+4]` BEFORE the two pushes, i.e. it is
// hoisted above the strtok call. Both slots are legal (a load cannot cross a
// call, and in the original neither load is hoisted past its own strtok), so
// this is a scheduler tie-break, not a missing instruction: difflib matches the
// two `mov ebx, dword ptr [esi+4]` texts to each other and scores them equal,
// but the bytes are 6 out of place in each of the 8 loads. That is why the
// text score never reached 100% even where the instruction multiset matches.
// Writing the store through a local pointer (`Elem* p = &(*this)[i]; p->a = ...`)
// puts the load even earlier, drops the function to 696 bytes and makes the
// allocation flip back (68.1% / 79.1%), so the pointer form is not it either.
//
// deepseek-v4.1-flash pass 2: swept all subsets of arms switched to
// top-tested loops and all combinations of which arms test n vs count in a
// do-while. BEST 86.1% (708 bytes, s01): case 0 keeps the exact-shape
// `if (n > 0) do {} while (--count)`, case 1 is `if (n > 0) while
// (count > 0) {}` (adds one redundant `test ebp,ebp / jle` pair), cases 2 and
// 3 drop the n-guard and use `while (count > 0)` (their one `test ebp,ebp`
// replaces the original `test di,di`). That is three top-tested arms, the
// minimum that flips MSVC 5 to the original edi/ebp/ebx allocation. Using
// count-guards with do-while arms instead of while arms does NOT flip it
// (best 81.4%), so the top-tested-loop tree, not the mere count test, is the
// trigger. Remaining diffs are those three arm tests plus the move of
// `mov ebx,[esi+4]` before the first strtok call and the resulting address
// shifts.
// BEST 80.1% (724 bytes). Builds on the 79.3% pass (all four loop arms as
// `while (count > 0) { body; i++; count--; }`, 732 bytes) and the exact-shape
// 75.2% attempt (all four arms as `do { body; i++; } while (--count)`, 708
// bytes, register rotation only).
//
// What this pass fixed: the register allocation flips to the original's
// (short n and the byte index in edi, the int count in ebp, the reloaded
// _First in ebx) only when at least three of the four switch arms are written
// as top-tested `while (count > 0)` loops. Writing just one or two arms that
// way leaves the rotation wrong (74-75%); writing three arms gets the correct
// allocation with only three redundant `test ebp,ebp / jle` pairs (724 bytes)
// instead of four (732 bytes). The best three-arm choice is to keep case 0 as
// the exact-shape `do {} while (--count)` arm (mask 0b1110: cases 1, 2 and 3
// top-tested), because leaving the first arm in the original shape delays the
// first address shift furthest down the function, so the most bytes line up.
//
// What still differs: the three redundant arm tests (`test ebp,ebp / jle`
// before `xor edi,edi`, one in each of cases 1, 2, 3) and the resulting later
// address shifts. The instruction shapes and every operand are otherwise the
// original's. Removing those three tests while keeping this allocation is the
// whole job that is left.
//
// Earlier notes follow.
// BEST 75.2% (708 bytes, source size matches the original).
// GPT-6.1-sol refinement: tried all 24 switch case orderings and several
// count/default variants; none beat this source. Remaining mismatch is the
// earlier-noted register rotation: original uses edi for index and ebp for
// count; MSVC assigns ebx for index and edi for count.
//
// What this pass fixed: declaring the loop index ONCE before the switch
// (`int i = 0;` beside `short n` and `int count`) instead of once inside each
// of the four case bodies. MSVC 5 then copies the `i = 0` into each case arm
// anyway (it can: only one arm runs) and, more importantly, spends one inline
// expansion less, so `vector<Elem_00434020>::_Destroy` in the failed-lookup
// tail stays an out-of-line CALL (as the original has) instead of being
// inlined to nothing. That restored the 5 missing bytes:
// 0x433985 mov eax,[esi+8] / mov ecx,esi / push eax / push edi /
// call 0x433d90 (_Destroy) / mov [esi+8],edi, and with it the whole tail
// register choice (edi holds _First there, exactly as the original) instead of
// ecx plus a dead spill to [esp+0x10].
//
// What still differs (register rotation only, every instruction is the right
// one with the right operands):
//   original: loop index and the short n in edi, the int count in ebp, the
//             reloaded _First in ebx;
//   ours:     index and short n in ebx, count in edi, _First in ebp.
// MSVC 5 hands the same three callee-saved registers out one step round, so the
// only way on is a source change that shifts its scratch-register weights
// (the order in which the four case bodies are written, how `n - size()` is
// spelled, and so on). Declaration order of n/count/i does NOT change it
// (tried i before n, and count+i before n: all three give the same 75.2%).
// Tried and no better: a single-use static helper around the failed-path
// resize (unchanged, 69.3%), the failed path written first with an early
// return (668 bytes, 42.9%), unsigned int for count (unchanged), and all 128
// header sets. Refinement pass: moving count/i declarations and initializers
// across resize, then swapping their initialization order, all stayed 75.2%.
// deepseek-v4.1-flash pass: swept all 15 non-empty subsets of the four arms
// converted to top-tested loops; 1- and 2-arm subsets stay at the wrong
// allocation (74-75%), 3- and 4-arm subsets flip it (79.7-80.1%). Also
// re-ran headers.py --cpp over the exact-shape base: all 768 header sets stay
// at 75.2%, confirming the compiler-state tie.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

struct Elem_00434020 {
    unsigned short a;
    unsigned short b;
};

typedef std::vector<Elem_00434020> Vec_004336f0;

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, int size, char* def);
};

class Class_004c3e10 {
public:
    char unknown_0[4];
    Class_004c48c0* parser;
};

extern char DAT_005119b8[];

class Class_004336f0 : public Vec_004336f0 {
public:
    void FUN_004336f0(Class_004c3e10* obj, short line, short mode);
};

// FUNCTION: 0x4336f0
void Class_004336f0::FUN_004336f0(Class_004c3e10* obj, short line, short mode)
{
    char name[32];
    char buf[0x200];

    sprintf(name, "line%d", line + 1);
    if (obj->parser->FUN_004c48c0(buf, name, 0x200, DAT_005119b8) != 0) {
        char* tok = strtok(buf, ", ");
        if (tok == 0)
            return;
        short n = atoi(tok);
        int count = n;
        int i = 0;
        Elem_00434020 x;
        resize(n, x);
        switch (mode) {
            case 0:
                    if (n > 0) {
    do {
                            (*this)[i].a = atoi(strtok(0, ", "));
                            (*this)[i].b = -atoi(strtok(0, ", "));
                            i++;
                        } while (--count);
                    }
                return;
            case 1:
                    if (n > 0) {
    while (count > 0) {
                            (*this)[i].b = atoi(strtok(0, ", "));
                            (*this)[i].a = atoi(strtok(0, ", "));
                            i++;
                            count--;
                        }
                    }
                return;
            case 2:
while (count > 0) {
                        (*this)[i].a = -atoi(strtok(0, ", "));
                        (*this)[i].b = atoi(strtok(0, ", "));
                        i++;
                        count--;
                    }
                return;
            case 3:
while (count > 0) {
                        (*this)[i].b = -atoi(strtok(0, ", "));
                        (*this)[i].a = atoi(strtok(0, ", "));
                        i++;
                        count--;
                    }
                return;
        }
    }
    else {
        Elem_00434020 x;
        resize(0, x);
    }
}
