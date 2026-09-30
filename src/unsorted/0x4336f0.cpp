// Decompiled by deepseek-v4.1-flash. Names are provisional.
// GPT-6 retry: remains 69.3% (704 of 708 bytes). Tried allocator destructor
// declarations, a vector specialization, signed element fields and count/index
// representations. None improved the saved version. Register allocation and
// the failed-lookup _Destroy call remain different.
// PARTIAL 69.3%. Class_004336f0 is a std::vector<Elem_00434020> (4-byte
// elements) holding one pair of signed shorts per line entry. It reads the
// "line%d" key from a TDF-style parser via Class_004c48c0, resizes itself to
// the first comma-separated number and fills the elements from later tokens,
// negating the a or b half by mode (the four callers at 0x433484 pass mode
// 0..3). When the parser lookup fails it shrinks the vector to 0.
//
// What is still wrong (all register allocation, no source shape left to try):
//   original: n(short) in edi, its sign-extended int copy in ebp, the loop
//             index in edi, the _First temp in ebx; the failed-lookup tail
//             inlines resize(0) and ends with `call UElem_00434020::_Destroy`;
//   ours:     n(short) in ebx, the int copy in edi, the index in ebx, the
//             _First temp in ebp; the tail has no _Destroy call (MSVC inlines
//             the empty trivial-destructor _Destroy away).
// Tried and no better: no separate int copy (64.8%), int-before-short (65.8%),
// count before resize (68.5%), erase(begin(), end()) (68.5%), clear() (68.5%),
// and all 128 header sets (68.5%). resize(0, x) in the failed path is what got
// 708 -> 704 bytes and 68.5 -> 69.3%. The missing _Destroy call is 4 bytes;
// making Elem non-trivial to force it adds a destructor call at return, which
// the original lacks, so the element type really is trivial.
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
    Elem_00434020 x;
    char name[32];
    char buf[0x200];

    sprintf(name, "line%d", line + 1);
    if (obj->parser->FUN_004c48c0(buf, name, 0x200, DAT_005119b8) != 0) {
        char* tok = strtok(buf, ", ");
        if (tok == 0)
            return;
        short n = atoi(tok);
        int count = n;
        resize(n, x);
        switch (mode) {
        case 0:
            if (n > 0) {
                int i = 0;
                do {
                    (*this)[i].a = atoi(strtok(0, ", "));
                    (*this)[i].b = -atoi(strtok(0, ", "));
                    i++;
                } while (--count);
            }
            return;
        case 1:
            if (n > 0) {
                int i = 0;
                do {
                    (*this)[i].b = atoi(strtok(0, ", "));
                    (*this)[i].a = atoi(strtok(0, ", "));
                    i++;
                } while (--count);
            }
            return;
        case 2:
            if (n > 0) {
                int i = 0;
                do {
                    (*this)[i].a = -atoi(strtok(0, ", "));
                    (*this)[i].b = atoi(strtok(0, ", "));
                    i++;
                } while (--count);
            }
            return;
        case 3:
            if (n > 0) {
                int i = 0;
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
        resize(0, x);
    }
}
