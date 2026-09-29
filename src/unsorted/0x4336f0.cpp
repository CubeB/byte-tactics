// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 68.5%. Class_004336f0 is a std::vector<Elem_00434020> (4-byte
// elements) holding one pair of signed shorts per line entry. It reads the
// "line%d" key from a TDF-style parser via Class_004c48c0, resizes itself to
// the first comma-separated number and fills the elements from later
// tokens, negating the a or b half by mode (the four callers at 0x433484
// pass mode 0..3 for a 16-byte element array).
//
// Still differs: (1) the compiler puts the short count in ebx and the
// int countdown in edi, while the original uses edi for the short and ebp
// for the countdown (_First temp in ebx there, ebp here); (2) the original's
// inlined erase in the lookup-failure path keeps a call to
// UElem_00434020::_Destroy (0x433d90, an empty ret 8), which MSVC removes
// here. Both are register/inline-choice differences; the source shape is
// otherwise right (64.8% without the int count local, 57.3% with a for loop).
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
        resize(n, x);
        int count = n;
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
        erase(begin(), end());
    }
}
