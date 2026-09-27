// Decompiled by space-bunny-free. Names are provisional.
// Appends an entry to the std::vector<Elem_00473500> that the short index
// picks out of the array: 0x10 bytes per entry (MSVC 5's vector has its empty
// allocator at +0, then _First +4, _Last +8, _End +0xc), so the index is
// multiplied by 0x10. When the list already holds more than 400 entries its
// oldest element is deleted and erased first, exactly as the sibling
// Class_00471340::Add in 0x471340.cpp does for std::vector<Class_00471cc0*>.
// The append is MSVC 5's inlined vector::insert(end(), 1, x) (push_back is
// insert(end(), _X) in this <vector>), which is why the three out-of-line
// helpers of that vector are called: 0x473500 (_Ucopy), 0x473530 (_Ufill) and
// 0x4732d0 (_Destroy). 0x471820 and 0x471a50 inline the same method.
//
// STILL DIFFERS: nothing in the bytes. check.py reports "bytes match, but a
// reference is wrong" on one of the ten references:
//   +0x129  ?size@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@QBEIXZ
// That symbol is the out-of-line copy of this vector's size() const, which
// MSVC 5 emits as a COMDAT in every object whose inliner declines to expand it
// (here the last of the four size() uses, the `_Last = _S + size() + _M` of
// the growth branch). Compiling this file emits exactly that COMDAT, with the
// same 19 bytes as the exe's 0x472d30 (mov edx,[ecx+4]; test; jne; xor eax,eax;
// ret; mov eax,[ecx+8]; sub eax,edx; sar eax,2; ret), and it is called from
// exactly the three functions in the exe where the inliner declined:
// 0x471160, 0x471820, 0x471a50. So 0x472d30 is that size(), and the name
// data/symbols.csv gives it, Class_00472d30::FUN_00472d30, is a provisional
// name from matching 0x472d30 against a hand-rolled {begin, end} struct. It
// only blocks the reference check here; rewriting 0x472d30.cpp the way
// 0x473500.cpp does (a struct that takes &std::vector<Elem_00473500>::size
// forces the out-of-line copy out, and that also matches 0x472d30 in 19
// bytes) would fix the name for this function, for 0x471820, for 0x471a50 and
// for 0x472d30 itself. With the name corrected, this file is a full MATCH:
// 476 of 476 bytes and all ten references.
#include <vector>

class Listener_00471120 {             // what the elements point at
public:
    virtual ~Listener_00471120();
    virtual void Notify(void* param);
};

struct Elem_00473500 {                // one entry, a pointer to a listener
    Listener_00471120* p;             // +0x0
};

class Class_00471120 {
public:
    std::vector<Elem_00473500> lists[1];

    void FUN_00471120(void* param, short index);
    void FUN_00471160(Elem_00473500 x, short index);
};

// FUNCTION: 0x471160
void Class_00471120::FUN_00471160(Elem_00473500 x, short index)
{
    if (lists[index].size() > 400) {
        delete lists[index][0].p;
        lists[index].erase(lists[index].begin());
    }
    lists[index].push_back(x);
}
