// Decompiled by Opus. Names are provisional.
#include <string.h>
#include <vector>

struct Pair_004c5840 {
    int value;                      // +0x0
    char* name;                     // +0x4
};

#pragma pack(push, 1)
class TranslationTable {
public:
    char unknown_0[0x1];
    std::vector<Pair_004c5840> pairs;   // +0x1 (_First at +0x5)
};
#pragma pack(pop)

extern TranslationTable* g_translations;

// FUNCTION: 0x4c5840
int __stdcall FindTranslation(char* name)
{
    if (name == 0 || g_translations == 0)
        return 0;
    for (std::vector<Pair_004c5840>::iterator p = g_translations->pairs.begin(); p < g_translations->pairs.end(); p++) {
        if (_strcmpi(p->name, name) == 0)
            return p->value;
    }
    return 0;
}
