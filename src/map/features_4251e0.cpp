// Decompiled by Haiku. Names are provisional.
// std::vector<TdfFile*>::_Destroy(first, last): empty, since the element
// type is trivial.
// Its caller, 0x424c00, inlines FreeFeatureFileList (delete every feature in the
// global vector DAT_00511fb4, then the vector) and calls this with ecx set
// to that vector.
// The same vector's insert is 0x425480 (called from 0x4222e0).
#include <vector>

#include "../util/tdf.h"

typedef std::vector<TdfFile*> Vec_004251e0;
typedef void (Vec_004251e0::*DestroyFn_004251e0)(Vec_004251e0::iterator, Vec_004251e0::iterator);

// _Destroy is protected: the derived class takes its address to emit it out of line.
struct Access_004251e0 : Vec_004251e0 {
    static DestroyFn_004251e0 fn;
};

// FUNCTION: 0x4251e0 ?_Destroy@?$vector@PAVTdfFile@@V?$allocator@PAVTdfFile@@@std@@@std@@IAEXPAPAVTdfFile@@0@Z
DestroyFn_004251e0 Access_004251e0::fn = &Access_004251e0::_Destroy;
