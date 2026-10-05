// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

extern int __cdecl _strcmpi(const char*, const char*);

struct Value_004b4910 {                // 0x10 bytes
    char* name;                        // +0x00
    int type;                          // +0x04
    int unknown_8;                     // +0x08
    int unknown_c;                     // +0x0c
};

struct Section_004b4910 {              // 0x18 bytes
    char unknown_0[4];
    int count;                         // +0x04
    char unknown_8[8];
    Value_004b4910* values;            // +0x10
    char unknown_14[4];
};

struct File_004b4910 {
    char unknown_0[4];
    Section_004b4910* sections;        // +0x04
    int current;                       // +0x08
};

void* __cdecl FUN_004d8580(void* ptr, int size);
char* __cdecl FUN_004d8610(char* s);

class Class_004b48f0 {
public:
    File_004b4910* file;               // +0x00
    int FUN_004b4910(char* name, int flag);
};

// FUNCTION: 0x4b4910
int Class_004b48f0::FUN_004b4910(char* name, int flag)
{
    File_004b4910* f = file;
    if (!f || f->current < 0)
        return -1;
    Section_004b4910* s = &f->sections[f->current];
    for (int i = 0; i < s->count; i++) {
        if (_strcmpi(s->values[i].name, name) == 0)
            return i;
    }
    if (!flag)
        return -1;
    int n = s->count;
    s->count = n + 1;
    s->values = (Value_004b4910*)FUN_004d8580(s->values, (n + 1) * sizeof(Value_004b4910));
    memset(&s->values[n], 0, sizeof(Value_004b4910));
    s->values[n].name = FUN_004d8610(name);
    return n;
}
