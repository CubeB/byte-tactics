// Decompiled by Opus. Names are provisional.
// Empties the file-local global vector (see 0x4889d0.cpp and 0x488a00.cpp):
// frees each element's value, then clears the vector, which releases each
// element's reference-counted name through FUN_004c9390.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

struct Elem_00488a00 {
    Class_004c9390 name;               // +0x0
    void* value;                       // +0x4

    ~Elem_00488a00() { name.FUN_004c9390(); }
};

static std::vector<Elem_00488a00> DAT_0051e6b0;
extern int DAT_0051e6c0;

// FUNCTION: 0x488bf0
void FUN_00488bf0()
{
    for (std::vector<Elem_00488a00>::iterator it = DAT_0051e6b0.begin(); it != DAT_0051e6b0.end(); it++)
        delete it->value;
    DAT_0051e6b0.clear();
    DAT_0051e6c0 = 0;
}
