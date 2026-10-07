// Decompiled by Opus. Names are provisional.
// std::vector<TdfField>::_Ufill(first, n, value): copy-constructs n copies of
// value into raw storage at first.
// The element holds two reference-counted handles whose copy constructor is
// 0x4c91a0 (see 0x4c5bc0 for _Ucopy and 0x4c5b70 for _Destroy).
#include <vector>

class Class_004c91a0 {
public:
    char* p;
    Class_004c91a0();
    Class_004c91a0(const Class_004c91a0& other);
};

struct TdfField {
    Class_004c91a0 a;                  // +0x0
    Class_004c91a0 b;                  // +0x4
};

typedef std::vector<TdfField> Vec_004c5c20;
typedef void (Vec_004c5c20::*UfillFn_004c5c20)(
    Vec_004c5c20::iterator, Vec_004c5c20::size_type, const TdfField&);

// _Ufill is protected: a derived class takes its address to emit it out of line.
struct Access_004c5c20 : Vec_004c5c20 {
    static UfillFn_004c5c20 fn;
};

// FUNCTION: 0x4c5c20 ?_Ufill@?$vector@UTdfField@@V?$allocator@UTdfField@@@std@@@std@@IAEXPAUTdfField@@IABU3@@Z
UfillFn_004c5c20 Access_004c5c20::fn = &Access_004c5c20::_Ufill;
