// Decompiled by Opus. Names are provisional.
#include <stdlib.h>

// Command arguments: replaces each "%N" argument with argument N of another list.
class Class_004b74f0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0

    // Inline copy of the range-checked getter at 0x4b73c0; its redundant
    // range check folds away but the extra use of `other` decides registers.
    char* GetArg(int index, char* def)
    {
        if (index < 0 || index >= count) return def;
        return args[index];
    }

    void SubstituteArgs(Class_004b74f0* other);
};

// FUNCTION: 0x4b74f0
void Class_004b74f0::SubstituteArgs(Class_004b74f0* other)
{
    for (int i = 0; i < count; i++) {
        if (*args[i] == '%') {
            int n = atoi(args[i] + 1);
            if (n >= 0 && n < other->count) {
                args[i] = other->GetArg(n, 0);
            }
        }
    }
}
