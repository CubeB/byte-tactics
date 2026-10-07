// Decompiled by deepseek-v4.1-flash, space-bunny-free, deepseek-v4.1, mimo-v2.6-pro, Claude Opus 5.5 and Haiku. Names are provisional.

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
    int GetLosLineStepCount();
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
        // short counter: gives the original's countdown loop.
        short i;
        switch (mode) {
        case 0:
            // strtok called inside atoi: the element address is loaded after the call.
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

// FUNCTION: 0x4339c0
int LosLine::GetLosLineStepCount()
{
    return size();
}
