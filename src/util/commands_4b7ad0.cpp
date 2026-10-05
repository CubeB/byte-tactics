// Decompiled by Opus. Names are provisional.
// Empties the file-local global vector created by 0x4b75a0 (destroyed by
// 0x4b75d0): the inlined vector::clear() runs each element's destructor.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct CommandEntry {
    Class_004c9390 name;               // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    ~CommandEntry() { name.ReleaseRef(); }
};

static std::vector<CommandEntry> s_commandTable;

// FUNCTION: 0x4b7ad0
void ClearCommandTable()
{
    s_commandTable.clear();
}
