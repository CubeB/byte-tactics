// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
// Claude Opus 5.5 rebuilt the loops from the disassembly and matched it:
// - Each arm is a plain `for (i = 0; i < n; i++)` with a `short i`. MSVC turns
//   it into the original's countdown (ebp = (int)n, shared with resize's
//   conversion) plus a byte offset in edi, guarded by `test di, di`. An `int i`
//   keeps i as a scaled index instead, and the old hand-written
//   `do {} while (--count)` arms got the registers one step round.
// - `atoi(tok = strtok(0, ", "))` evaluates strtok before the element address,
//   so _First is reloaded after the strtok call as in the original. A separate
//   `tok = strtok(...);` statement gives the same order but adds 5 IL per
//   site; with all eight the function's IL passes 556, its /Ob2 budget grows
//   and the failed path's resize(0) inlines vector::_Destroy, which the
//   original calls.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

struct Elem_00434020 {
    unsigned short a;
    unsigned short b;
};

typedef std::vector<Elem_00434020> Vec_004336f0;

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, int size, char* def);
};

class TdfFile {
public:
    char unknown_0[4];
    TdfRecord* parser;
};

extern char DAT_005119b8[];

class LosLine : public Vec_004336f0 {
public:
    void LoadLosLine(TdfFile* obj, short line, short mode);
};

// FUNCTION: 0x4336f0
void LosLine::LoadLosLine(TdfFile* obj, short line, short mode)
{
    char name[32];
    char buf[0x200];

    sprintf(name, "line%d", line + 1);
    if (obj->parser->GetFieldString(buf, name, 0x200, DAT_005119b8) != 0) {
        char* tok = strtok(buf, ", ");
        if (tok == 0)
            return;
        short n = atoi(tok);
        Elem_00434020 x;
        resize(n, x);
        short i;
        switch (mode) {
        case 0:
            for (i = 0; i < n; i++) {
                (*this)[i].a = atoi(tok = strtok(0, ", "));
                (*this)[i].b = -atoi(tok = strtok(0, ", "));
            }
            break;
        case 1:
            for (i = 0; i < n; i++) {
                (*this)[i].b = atoi(tok = strtok(0, ", "));
                (*this)[i].a = atoi(tok = strtok(0, ", "));
            }
            break;
        case 2:
            for (i = 0; i < n; i++) {
                (*this)[i].a = -atoi(tok = strtok(0, ", "));
                (*this)[i].b = atoi(tok = strtok(0, ", "));
            }
            break;
        case 3:
            for (i = 0; i < n; i++) {
                (*this)[i].b = -atoi(tok = strtok(0, ", "));
                (*this)[i].a = -atoi(tok = strtok(0, ", "));
            }
            break;
        }
    } else {
        Elem_00434020 x;
        resize(0, x);
    }
}
