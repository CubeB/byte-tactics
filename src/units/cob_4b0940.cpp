// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Looks a name up in the table at +8 (the lookup at 0x4b07c0, inlined here),
// claims a channel slot for its index with 0x4b08c0, stores the value and
// refreshes the channels when asked. Compare 0x4b0a10, its index-taking twin.
#include <string.h>

struct NameTable_004b0940 {
    char unknown_0[4];
    int count;                         // +0x4
    char unknown_8[0x1c - 0x8];
    char** names;                      // +0x1c
};

struct Channel_004b0940 {              // 0xa4 bytes
    int used;                          // +0x0
    int id;                            // +0x4
    char unknown_8[0x20 - 0x8];
    int value;                         // +0x20
    char unknown_24[0xa4 - 0x24];
};

class CobScript {
public:
    char unknown_0[8];
    NameTable_004b0940* table;         // +0x8
    char unknown_c[0x1c - 0xc];
    Channel_004b0940 channels[8];      // +0x1c
    int activeCount;                   // +0x53c

    int StartScript(const char* name, int value, int update);
    int FindScript(const char* name);
    int StartThread(int id);
    void RunThread(int channel, int param_2);
    void AnimatePieces(int param_1);
};

// The name lookup just before this function in the original file, defined here
// so /Ob2 inlines it as the original did (the same helper as in 0x4b0830.cpp).
int CobScript::FindScript(const char* name)
{
    for (int i = 0; i < table->count; i++) {
        if (strcmp(name, table->names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4b0940
int CobScript::StartScript(const char* name, int value, int update)
{
    int i = ((CobScript*)this)->StartThread(((CobScript*)this)->FindScript(name));
    if (i < 0)
        return 0;
    channels[i].value = value;
    if (update) {
        if (activeCount) {
            for (int j = 0; j < 8; j++)
                ((CobScript*)this)->RunThread(j, 0);
        }
        ((CobScript*)this)->AnimatePieces(0);
    }
    return 1;
}
