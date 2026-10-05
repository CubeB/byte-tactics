// Decompiled by Opus. Names are provisional.
#include <string.h>
#include <vector>

struct Named_004c4470 {
    char* name;                         // +0x0
};

class TdfRecord {
public:
    int unknown_0;
    std::vector<Named_004c4470*> entries;   // +0x4 (_First at +0x8)

    Named_004c4470* FindSubRecord(const char* name);
};

// FUNCTION: 0x4c4470
Named_004c4470* TdfRecord::FindSubRecord(const char* name)
{
    for (std::vector<Named_004c4470*>::iterator p = entries.begin(); p < entries.end(); p++) {
        if (_strcmpi((*p)->name, name) == 0)
            return *p;
    }
    return 0;
}
