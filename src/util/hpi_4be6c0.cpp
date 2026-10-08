// Decompiled by Sonnet 5.5. Names are provisional.
// Stays in its own file: hpi.cpp's hand-written std::vector keeps this
// insert out of line at the call sites in ListDirectory and FindFilesRecursive
// (0x4bca30 and 0x4bcb50), which the real <vector> would inline.
// std::vector<StringRef>::insert(iterator, size_type, const T&), out of
// line, for a vector of reference-counted string handles (4 bytes each). The
// copy constructor is 0x4c91a0, the assignment 0x4c93b0 and the destructor
// releases the handle through 0x4c9390.
#include <vector>

// The reference-counted string handle: a pointer to the characters with the
// reference count in the int just before them (see 0x4c91a0, 0x4c9390 and
// 0x4c93b0 for the copy, release and assign views).
class StringRef {
public:
    char* ptr;                         // +0x0

    StringRef(const StringRef& other);
    StringRef(const char* text);
    StringRef(const char* text, int len);
    StringRef& operator=(const StringRef& other)
    {
        Assign((StringRef*)&other);
        return *this;
    }
    ~StringRef() { ReleaseRef(); }
    void ReleaseRef();
    StringRef* Assign(StringRef* param_1);
    StringRef* Append(const StringRef& other);
    StringRef* MakeLower();
    StringRef* MakeUpper();
    StringRef* AssignText(const char* text);
    char* GetUnique();
    int IsEmpty() const;
    StringRef SubString(int start, int end) const;
};

typedef std::vector<StringRef> Vec_004be6c0;
typedef void (Vec_004be6c0::*InsertFn_004be6c0)(
    Vec_004be6c0::iterator, Vec_004be6c0::size_type, const StringRef&);

// FUNCTION: 0x4be6c0 ?insert@?$vector@VStringRef@@V?$allocator@VStringRef@@@std@@@std@@QAEXPAVStringRef@@IABV3@@Z
InsertFn_004be6c0 g_insert_004be6c0 = &Vec_004be6c0::insert;
