// Decompiled by space-bunny-free. Names are provisional.
// Walks the whitespace separated names in the argument string. Each name is
// looked up in the name to mask table (FUN_00488c50) and this object's team bit
// is set in the mask that name maps to, then the same is done for "ALL", so the
// team always ends up in the ALL mask.
// The first test sits outside the loop (a do/while): that is what puts the
// loop's register save between the test and the body, and it is also what wins
// ebx for `this` instead of edi.
#include <stdio.h>

struct Class_00488e70 {
    char unknown_0[0x21e];
    unsigned short team;              // +0x21e

    void FUN_00488e70(char* names);
};

void* __stdcall FUN_00488c50(char* name);

// FUNCTION: 0x488e70
void Class_00488e70::FUN_00488e70(char* names)
{
    int n;
    char buf[256];
    if (sscanf(names, " %s %n", buf, &n) == 1) {
        do {
            names += n;
            unsigned short team = this->team;
            unsigned int* mask = (unsigned int*)FUN_00488c50(buf);
            mask[team >> 5] |= 1 << (team & 0x1f);
        } while (sscanf(names, " %s %n", buf, &n) == 1);
    }
    unsigned short team = this->team;
    unsigned int* all = (unsigned int*)FUN_00488c50("ALL");
    all[team >> 5] |= 1 << (team & 0x1f);
}
