// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// BEST 75.2% (532 of 708 bytes, our size now equals the original's 708).
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
                do {
                    (*this)[i].b = atoi(strtok(0, ", "));
                    (*this)[i].a = atoi(strtok(0, ", "));
                    i++;
                } while (--count);
            }
            return;
        case 2:
            if (n > 0) {
                do {
                    (*this)[i].a = -atoi(strtok(0, ", "));
                    (*this)[i].b = atoi(strtok(0, ", "));
                    i++;
                } while (--count);
            }
            return;
        case 3:
            if (n > 0) {
                do {
                    (*this)[i].b = -atoi(strtok(0, ", "));
                    (*this)[i].a = atoi(strtok(0, ", "));
                    i++;
                } while (--count);
            }
            return;
        }
    }
    else {
        Elem_00434020 x;
        resize(0, x);
    }
}
