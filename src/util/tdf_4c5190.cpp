// Decompiled by Opus. Names are provisional.
// Out-of-line destructor of the element of std::vector<TdfField> (two
// reference-counted string handles, see 0x4c5bc0.cpp): called on each
// element in the vectors' destroy loops, and on the local passed to insert
// (0x4c59d0) at the end of its scope. The handles are released in reverse
// order with ReleaseRef.

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

struct TdfField {
    Class_004c9390 a;                  // +0x0
    Class_004c9390 b;                  // +0x4

    ~TdfField();
};

// FUNCTION: 0x4c5190
TdfField::~TdfField()
{
    b.ReleaseRef();
    a.ReleaseRef();
}
