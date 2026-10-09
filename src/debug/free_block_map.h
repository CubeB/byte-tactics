// FreeBlockMap: the debug allocator's set of free address-space blocks, a
// hand-walked std::map<unsigned int, unsigned int> from a block's base offset
// to its length, and FreeBlockIter, the node-pointer iterator that walks it.
// The one declaration of the two types for debug_lib_4dacf0.cpp and
// debug_lib_4db7d0.cpp. free_block_map.cpp, debug_lib.cpp and
// debug_lib_4db610.cpp keep their own view: each inlines a different tree
// helper (free_block_map.cpp's operator++ walks with FindLeftmost,
// debug_lib.cpp's with _Min and 4db610.cpp is a std::map member) against its
// own symbol context, and the header can carry only one. The header includes
// nothing.
#ifndef FREE_BLOCK_MAP_H
#define FREE_BLOCK_MAP_H

struct Pair_004db000 {
    unsigned int offset;               // +0x0
    unsigned int length;               // +0x4
    Pair_004db000() {}
    Pair_004db000(unsigned int o, unsigned int l) : offset(o), length(l) {}
};

struct Node_004db000 {
    Node_004db000* left;               // +0x0
    Node_004db000* parent;             // +0x4
    Node_004db000* right;              // +0x8
    Pair_004db000 value;               // +0xc
    int color;                         // +0x14
};

// The free-block tree's iterator: one pointer. PrevNode is its _Dec; Previous
// and Next are its out-of-line operator--(int) and operator++(int).
class FreeBlockIter {
public:
    Node_004db000* ptr;

    FreeBlockIter() {}
    FreeBlockIter(Node_004db000* q) : ptr(q) {}
    bool operator==(const FreeBlockIter& o) const { return ptr == o.ptr; }
    bool operator!=(const FreeBlockIter& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    FreeBlockIter Previous(int); // operator--(int)
    FreeBlockIter Next(int); // operator++(int)
};

// The (iterator, inserted) pair the tree's insert functions return.
class MapInsertResult {
public:
    FreeBlockIter first;
    unsigned char second;
    MapInsertResult() {}
};

class FreeBlockMap {
public:
    char unknown_0[4];     // +0x0
    Node_004db000* head;   // +0x4
    unsigned char rebuild; // +0x8
    unsigned int count;    // +0xc
    unsigned int total;    // +0x10

    FreeBlockIter begin() { return Begin(); }
    FreeBlockIter end() { return FreeBlockIter(head); }
    unsigned int size() const { return count; }

    FreeBlockIter Begin();
    FreeBlockIter EraseCopyIter(FreeBlockIter it);
    MapInsertResult InsertOrFind(const Pair_004db000& v);
    FreeBlockIter lower_bound(const Pair_004db000& k);
    FreeBlockIter UpperBound(const unsigned int& k);
    void AddFreeBlock(Pair_004db000 p);
    unsigned int TakeFreeBlock(unsigned int bytes);
    bool GrowReservation(unsigned int size);
};

#endif
