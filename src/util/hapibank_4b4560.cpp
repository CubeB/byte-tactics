// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Finds the section named by the argument in the parsed text file, or appends
// a new empty section when there is none; returns 1 when it already existed.
#include <stdio.h>
#include <string.h>

struct BankAccount {                 // 0x18 bytes
    char* name;                      // +0x00
    char unknown_4[8];
    int field_c;                     // +0x0c, set to -1 for the current section
    char unknown_10[8];
};

struct AccountList {
    int count;                       // +0x00
    BankAccount* sections;           // +0x04
    int current;                     // +0x08
};

void* __cdecl FUN_004d8580(void* ptr, int size);
char* __cdecl GameStrdup(char* s);

class HapiBank {
public:
    AccountList* file;               // +0x00
    int OpenAccount(char* name);
};

// FUNCTION: 0x4b4560
int HapiBank::OpenAccount(char* name)
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
    file->sections = (BankAccount*)FUN_004d8580(
        file->sections, file->count * sizeof(BankAccount));
    memset(&file->sections[file->current], 0, sizeof(BankAccount));
    file->sections[file->current].name = GameStrdup(name);
    file->sections[file->current].field_c = -1;
    return 0;
}
