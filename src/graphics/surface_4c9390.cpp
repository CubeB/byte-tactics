// Decompiled by Sonnet. Names are provisional.
// Reference-count decrement and free for a reference-counted string handle
// (see the copy constructor at 0x4c91a0 and assignment at 0x4c93b0, which
// share the same shape). data/symbols.csv already names this address and
// class independently (Class_004c9390::ReleaseRef), and every existing
// caller (0x434a30.cpp, 0x432c00.cpp, 0x488a00.cpp, 0x4b75d0.cpp) already
// calls it that way as a plain method, so that established name is kept
// here rather than renamed to Class_004c91a0::~Class_004c91a0.

extern "C" void __cdecl free(void*);

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

// FUNCTION: 0x4c9390
void Class_004c9390::ReleaseRef()
{
    ((int*)data)[-1]--;
    int* p = (int*)data - 1;
    if (((int*)data)[-1] == 0) {
        free(p);
    }
}
