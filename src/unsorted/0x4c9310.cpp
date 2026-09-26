// Decompiled by Opus. Names are provisional.
#include <string.h>
#include <stdlib.h>

class Class_004c9310 {
public:
    char* data;              // refcount lives in the dword before data

    char* GetUnique()
    {
        int len = (int)strlen(data);
        if (*(int*)(data - 4) != 1) {
            int* block = (int*)malloc(len + 5);
            *block = 1;
            char* copy = (char*)(block + 1);
            strcpy(copy, data);
            (*(int*)(data - 4))--;
            if (*(int*)(data - 4) == 0) {
                free(data - 4);
            }
            data = copy;
            return copy;
        }
        return data;
    }

    Class_004c9310* FUN_004c9310();
};

// FUNCTION: 0x4c9310
Class_004c9310* Class_004c9310::FUN_004c9310()
{
    _strupr(GetUnique());
    return this;
}
