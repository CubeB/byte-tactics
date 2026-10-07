// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Loads every features/*.tdf through FindFilesRecursive into a local
// std::vector<Elem_004222e0>, then parses each file name into a new
// TdfFile and appends it to the global vector DAT_00511fb4.
//
#include <vector>

#include "../util/tdf.h"

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct Elem_004222e0 {
    Class_004c9390 name;               // +0x0

    ~Elem_004222e0() { name.ReleaseRef(); }
};

extern std::vector<TdfFile*>* DAT_00511fb4;

void __stdcall FindFilesRecursive(char* dir, char* pattern, void* list, int flags, char recurse);

// Must not be __cdecl: the calling convention changes the loop entry order.
// FUNCTION: 0x4222e0
void __stdcall LoadFeatureFileList()
{
    DAT_00511fb4 = new std::vector<TdfFile*>;
    std::vector<Elem_004222e0> list;
    FindFilesRecursive("features", "*.tdf", &list, -1, 1);
    for (std::vector<Elem_004222e0>::iterator it = list.begin(); it < list.end(); it++) {
        TdfFile* obj = new TdfFile;
        if (obj->LoadFile(it->name.data)) {
            DAT_00511fb4->push_back(obj);
        } else {
            delete obj;
        }
    }
}
