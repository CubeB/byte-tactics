// Decompiled by Space Bunny Free. Names are provisional.
// std::vector<Elem>::insert(iterator, size_type, const T&) for a vector of
// 4-byte elements: the exe's only caller (0x4c40b2, the .TDF parser) passes
// end(), 1 and the address of the element, so this is the append used after a
// new object is built (0x4c3e40). The fast branch shifts the tail right and the
// slow branch allocates with operator new[] and copies the three runs.
//
// The element type is a guess: the caller stores the pointer a new TdfRecord
// returned, and 0x4c3e40 itself has a vector at +0x4, so TdfRecord* is the
// natural pick.
#include <vector>

class TdfRecord {                      // the element, only its size is used
public:
    int field_0;
};

typedef std::vector<TdfRecord*> Vec_004c4d70;
// Returns void, or the two-argument insert overload is resolved instead.
// Taking the address is what emits the instantiation out of line.
typedef void (Vec_004c4d70::*InsertFn_004c4d70)(Vec_004c4d70::iterator, Vec_004c4d70::size_type, TdfRecord* const&);

// FUNCTION: 0x4c4d70 ?insert@?$vector@PAVTdfRecord@@V?$allocator@PAVTdfRecord@@@std@@@std@@QAEXPAPAVTdfRecord@@IABQAV3@@Z
InsertFn_004c4d70 g_insert_004c4d70 = &Vec_004c4d70::insert;
