// Decompiled by Sonnet. Names are provisional.
#include <string.h>

extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
extern "C" char* __cdecl _strlwr(char* str);

class Class_004c9290 {
public:
    char* data;

    Class_004c9290* FUN_004c9290();
};

// FUNCTION: 0x4c9290
Class_004c9290* Class_004c9290::FUN_004c9290()
{
    char* s = data;
    int len = (int)strlen(s);
    if (*(int*)(s - 4) != 1) {
        int* buf = (int*)malloc(len + 5);
        *buf = 1;
        char* newData = (char*)buf + 4;
        strcpy(newData, data);
        (*(int*)(data - 4))--;
        if (*(int*)(data - 4) == 0) {
            free(data - 4);
        }
        data = newData;
    }
    _strlwr(data);
    return this;
}
