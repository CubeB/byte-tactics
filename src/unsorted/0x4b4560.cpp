// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Finds the section named by the argument in the parsed text file, or appends
// a new empty section when there is none; returns 1 when it already existed.
#include <stdio.h>
#include <string.h>

struct Section_004b4560 {            // 0x18 bytes
    char* name;                      // +0x00
    char unknown_4[8];
    int field_c;                     // +0x0c, set to -1 for the current section
    char unknown_10[8];
};

struct File_004b4560 {
    int count;                       // +0x00
    Section_004b4560* sections;      // +0x04
    int current;                     // +0x08
};

void* __cdecl FUN_004d8580(void* ptr, int size);
char* __cdecl FUN_004d8610(char* s);

class Class_004b4560 {
public:
    File_004b4560* file;             // +0x00
    int FUN_004b4560(char* name);
};

// FUNCTION: 0x4b4560
int Class_004b4560::FUN_004b4560(char* name)
{
    for (int i = 0; i < file->count; i++) {
        if (_strcmpi(file->sections[i].name, name) == 0) {
            file->current = i;
            file->sections[i].field_c = -1;
            return 1;
        }
    }
    file->current = file->count;
    file->count++;
    file->sections = (Section_004b4560*)FUN_004d8580(
        file->sections, file->count * sizeof(Section_004b4560));
    memset(&file->sections[file->current], 0, sizeof(Section_004b4560));
    file->sections[file->current].name = FUN_004d8610(name);
    file->sections[file->current].field_c = -1;
    return 0;
}
