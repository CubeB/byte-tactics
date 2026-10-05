// Decompiled by Opus. Names are provisional.
// std::vector<TdfField>::_Destroy(first, last) from MSVC 5's <vector>:
// destroys each element (two reference-counted string handles, released in
// reverse order, see 0x4c5190.cpp). _Destroy is protected, so a derived
// class takes its address to make the compiler emit it out of line.
#include <vector>

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

struct TdfField {
    Class_004c9390 a;                  // +0x0
    Class_004c9390 b;                  // +0x4

    ~TdfField()
    {
        b.ReleaseRef();
        a.ReleaseRef();
    }
};

typedef std::vector<TdfField> Vec_004c5b70;
typedef void (Vec_004c5b70::*DestroyFn_004c5b70)(Vec_004c5b70::iterator, Vec_004c5b70::iterator);

struct Access_004c5b70 : Vec_004c5b70 {
    static DestroyFn_004c5b70 fn;
};

// FUNCTION: 0x4c5b70 ?_Destroy@?$vector@UTdfField@@V?$allocator@UTdfField@@@std@@@std@@IAEXPAUTdfField@@0@Z
DestroyFn_004c5b70 Access_004c5b70::fn = &Access_004c5b70::_Destroy;
