// Decompiled by Opus, deepseek-v4.1-flash, DeepSeek V4.1 Flash, Claude Opus
// 5.5, Haiku and Sonnet; the std::map and template members renamed by the
// orchestrator. Names are provisional.
// The rest of the unit synchronisation module: the out-of-line
// std::map<unsigned int, UnitSyncEntry>, std::vector and std::list members the
// module instantiates, and the hand-written _Tree helpers, from 0x46e9b0 to
// 0x4707a0. The /Gi members (unit_sync_46eba0.cpp, unit_sync_46f7a0.cpp) and
// unit_sync_470040.cpp, whose Class_00470250/Class_00470270 views cannot share
// this file's, stay in their own files.
#include <map>
#include <vector>
#include <list>
#include <yvals.h>

struct UnitSyncEntry {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

// --- the map's find and lower_bound (0x46e9b0, 0x46fe60) ---------------------

struct Node_0046e9b0 {
    Node_0046e9b0* left;               // +0x0
    Node_0046e9b0* parent;             // +0x4
    Node_0046e9b0* right;              // +0x8
    unsigned int key;                  // +0xc
};

class Iter_0046e9b0 {
public:
    Node_0046e9b0* ptr;

    Iter_0046e9b0() {}
    Iter_0046e9b0(Node_0046e9b0* p) : ptr(p) {}
    bool operator==(const Iter_0046e9b0& other) const { return ptr == other.ptr; }
};

struct Less_0046e9b0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046e9b0 {
public:
    Less_0046e9b0 compare;
    Node_0046e9b0* head;               // +0x4

    Iter_0046e9b0 End() { return Iter_0046e9b0(head); }
    Iter_0046e9b0 FUN_0046e9b0(const unsigned int& key);
};

// Shaped like std::_Tree<...>::_Lbound(const _K&) from MSVC 5's <xtree> for a
// tree keyed by unsigned int (the std::map<unsigned int, Rect> of 0x46e330),
// under a lock object; DAT_0051e598 is the tree's _Nil node and head->parent
// is the root.
struct Node_0046fe60 {
    Node_0046fe60* left;            // +0x0
    Node_0046fe60* parent;          // +0x4
    Node_0046fe60* right;           // +0x8
    unsigned int key;               // +0xc
};

struct Less_0046fe60 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_0046fe60 {
public:
    char allocator;                 // +0x0
    Less_0046fe60 key_compare;      // +0x1
    Node_0046fe60* head;            // +0x4
    Node_0046fe60* FUN_0046fe60(const unsigned int* key);
};

// An out-of-line std::map<unsigned int, ...>::find(): FUN_0046fe60 is the
// tree's lower_bound(), and a missing key yields the head node (end()).
// FUNCTION: 0x46e9b0
Iter_0046e9b0 Class_0046e9b0::FUN_0046e9b0(const unsigned int& key)
{
    Iter_0046e9b0 p = Iter_0046e9b0((Node_0046e9b0*)((Class_0046fe60*)this)->FUN_0046fe60(&key));
    return (p == End() || compare(key, p.ptr->key)) ? End() : p;
}

// --- the std::map<unsigned int, UnitSyncEntry> members -----------------------

// FUNCTION: 0x46ea10 ?_Inc@iterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAEXXZ
// FUNCTION: 0x46ef50 ?insert@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAE?AU?$pair@Viterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@_N@2@ABU?$pair@IUUnitSyncEntry@@@2@@Z
// FUNCTION: 0x46f1e0 ?erase@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAE?AViterator@12@V312@@Z
// FUNCTION: 0x46f6d0 ?_Erase@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEXPAU_Node@12@@Z
// FUNCTION: 0x46fb80 ?_Insert@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAE?AViterator@12@PAU_Node@12@0ABU?$pair@IUUnitSyncEntry@@@2@@Z
// FUNCTION: 0x46feb0 ?_Lrotate@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEXPAU_Node@12@@Z
// FUNCTION: 0x46ff10 ?_Rrotate@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEXPAU_Node@12@@Z
// FUNCTION: 0x46ff70 ?_Buynode@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEPAU_Node@12@PAU312@W4_Redbl@12@@Z
// FUNCTION: 0x46ff90 ?_Dec@iterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAEXXZ
// The tree's protected members come out of an explicit instantiation: the
// protected ones (the rotations) cannot be reached by a member pointer.
template class std::_Tree<unsigned int, std::pair<const unsigned int, UnitSyncEntry>, std::map<unsigned int, UnitSyncEntry>::_Kfn, std::less<unsigned int>, std::allocator<UnitSyncEntry> >;

// --- vector<Class_0046eaa0>::_Destroy (0x46eaa0) -----------------------------

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

void __stdcall FUN_00470030(int);

namespace std {
template<> inline void allocator<Elem_0046faf0>::destroy(Elem_0046faf0* p)
{
    // Direct call: an inline std::_Destroy overload would be one inline
    // level too deep.
    FUN_00470030((int)p);
}
}

struct Elem_004702a0 {
    int unknown_0;
};

struct PacketSequencer {               // operator= is 0x470560
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    // Nested struct with its own out-of-line operator=: sets the inline depth
    // that keeps list_c's _Destroy out of line and list_d's inlined.
    std::vector<Elem_0046faf0> list_c; // +0x0c
    std::vector<Elem_0046faf0> list_d; // +0x1c (operator= 0x4707a0)
};

struct Class_0046eaa0 {                // operator= is 0x470040
    int field_0;                       // +0x00
    std::vector<Elem_004702a0> list_a; // +0x04
    std::vector<Elem_004702a0> list_b; // +0x14
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    PacketSequencer sub;               // +0x30
};

typedef std::vector<Class_0046eaa0> Vec_0046eaa0;
typedef void (Vec_0046eaa0::*DestroyFn_0046eaa0)(Vec_0046eaa0::iterator, Vec_0046eaa0::iterator);

struct Access_0046eaa0 : Vec_0046eaa0 {
    static DestroyFn_0046eaa0 fn;
};

// std::vector<Class_0046eaa0>::_Destroy(first, last) from MSVC 5's <vector>,
// called from the inlined erase at 0x46db82 (0x46dad0) with ecx set to the
// vector. It runs each 0x5c-byte element's implicit destructor, which frees
// the element's four std::vector members last-first.
// FUNCTION: 0x46eaa0 ?_Destroy@?$vector@UClass_0046eaa0@@V?$allocator@UClass_0046eaa0@@@std@@@std@@IAEXPAUClass_0046eaa0@@0@Z
DestroyFn_0046eaa0 Access_0046eaa0::fn = &Access_0046eaa0::_Destroy;

// --- std::list<int> members (0x46eb60, 0x46fac0) -----------------------------

typedef std::list<int> List_0046eb60;
typedef List_0046eb60::iterator (List_0046eb60::*EraseFn_0046eb60)(List_0046eb60::iterator);

// std::list<int>::erase(iterator), out of line: its callers (0x46c920,
// 0x46ca60, 0x46d040's neighbours) call it as `erase(_F++)` with the
// iterator post-increment at 0x46fac0, on the list at +0x20 of
// UnitSync.
// FUNCTION: 0x46eb60 ?erase@?$list@HV?$allocator@H@std@@@std@@QAE?AViterator@12@V312@@Z
EraseFn_0046eb60 g_erase_0046eb60 = &List_0046eb60::erase;

// --- the hand-written _Tree::_Init (0x46f720) --------------------------------

// std::_Tree<...>::_Init() from MSVC 5's <xtree>, written out by hand:
// DAT_0051e598 is the tree's shared _Nil node and DAT_0051e59c its
// reference count (_Nilrefs). Nodes are 0x24 bytes (a 0x14-byte value).
struct Node_0046f720 {
    Node_0046f720* left;               // +0x0
    Node_0046f720* parent;             // +0x4
    Node_0046f720* right;              // +0x8
    char value[0x14];                  // +0xc
    int color;                         // +0x20 (0 red, 1 black)
};

extern Node_0046f720* DAT_0051e598;
extern int DAT_0051e59c;

class Class_0046f720 {
public:
    int unknown_0;
    Node_0046f720* head;               // +0x4
    int multi;                         // +0x8
    int size;                          // +0xc

    Node_0046f720* Buynode(Node_0046f720* parent, int color)
    {
        Node_0046f720* s = (Node_0046f720*)operator new(sizeof(Node_0046f720));
        s->parent = parent;
        s->color = color;
        return s;
    }
    void FUN_0046f720();
};

// FUNCTION: 0x46f720
void Class_0046f720::FUN_0046f720()
{
    std::_Lockit lock;
    if (DAT_0051e598 == 0) {
        DAT_0051e598 = Buynode(0, 1);
        DAT_0051e598->left = 0, DAT_0051e598->right = 0;
    }
    ++DAT_0051e59c;
    head = Buynode(DAT_0051e598, 0), size = 0;
    head->left = head, head->right = head;
}

// --- the iterator's post-increment (0x46fac0) --------------------------------

typedef std::list<int> List_0046fac0;
typedef List_0046fac0::iterator (List_0046fac0::iterator::*PostIncFn_0046fac0)(int);

// std::list<int>::iterator::operator++(int), out of line: return the old
// position and step to the next node. Its callers (e.g. 0x46c920) use it in
// the inlined list::erase(first, last) loop, `erase(_F++)`, on the list at
// +0x20 of the object 0x46d040 builds. The node is 0xc bytes (the constructor
// allocates the head with `new(0xc)`), so the element is a 4-byte value
// compared against an id (0x46d860); int is a guess.
// FUNCTION: 0x46fac0 ??Eiterator@?$list@HV?$allocator@H@std@@@std@@QAE?AV012@H@Z
PostIncFn_0046fac0 g_postinc_0046fac0 = &List_0046fac0::iterator::operator++;

// --- std::pair<map::iterator, bool> (0x46fad0) -------------------------------

// Probably std::pair<map::iterator, bool>::pair(const iterator&, const bool&),
// out of line: its one caller (0x46d2e0) is an inlined map::insert that
// builds the result from the node found (or inserted) and whether it is new.
struct Node_0046fad0;

class Class_0046fad0 {
public:
    Node_0046fad0* first;              // +0x0 (the iterator's node)
    bool second;                       // +0x4

    Class_0046fad0(Node_0046fad0* const& f, const bool& s);
};

// FUNCTION: 0x46fad0
Class_0046fad0::Class_0046fad0(Node_0046fad0* const& f, const bool& s)
    : first(f), second(s)
{
}

// --- vector<Elem_0046faf0> members (0x46faf0, 0x46fb40, 0x470770, 0x4707a0) --

typedef std::vector<Elem_0046faf0> Vec_0046faf0;
typedef Vec_0046faf0::iterator (Vec_0046faf0::*UcopyFn_0046faf0)(
    Vec_0046faf0::const_iterator, Vec_0046faf0::const_iterator, Vec_0046faf0::iterator);

struct Access_0046faf0 : Vec_0046faf0 {
    static UcopyFn_0046faf0 fn;
};

// std::vector<Elem_0046faf0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies.
// A 14-byte element type (its layout is a guess). Its callers (0x46cc10,
// 0x470390, 0x470560, 0x46ca60 and others) call 0x46fb40 (_Ufill), 0x46faf0
// (_Ucopy) and 0x46e870 (_Destroy) with ecx set to the vector.
// FUNCTION: 0x46faf0 ?_Ucopy@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEPAUElem_0046faf0@@PBU3@0PAU3@@Z
UcopyFn_0046faf0 Access_0046faf0::fn = &Access_0046faf0::_Ucopy;

typedef void (Vec_0046faf0::*UfillFn_0046faf0)(
    Vec_0046faf0::iterator, Vec_0046faf0::size_type, const Elem_0046faf0&);

struct Access_0046fb40 : Vec_0046faf0 {
    static UfillFn_0046faf0 fn;
};

// std::vector<Elem_0046faf0>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first.
// FUNCTION: 0x46fb40 ?_Ufill@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@IAEXPAUElem_0046faf0@@IABU3@@Z
UfillFn_0046faf0 Access_0046fb40::fn = &Access_0046fb40::_Ufill;

// --- the map's lower_bound (0x46fe60) ----------------------------------------

// The original calls this from 0x46e9b0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x46fe60
Node_0046fe60* Class_0046fe60::FUN_0046fe60(const unsigned int* key)
{
    std::_Lockit lock;
    Node_0046fe60* x = head->parent;
    Node_0046fe60* y = head;
    while (x != (Node_0046fe60*)DAT_0051e598)
        if (key_compare(x->key, *key))
            x = x->right;
        else
            y = x, x = x->left;
    return y;
}
#pragma auto_inline(on)

// --- vector<Elem_004702a0> members (0x470290, 0x4702a0) ----------------------

typedef std::vector<Elem_004702a0> Vec_00470290;
typedef void (Vec_00470290::*DestroyFn_00470290)(Vec_00470290::iterator, Vec_00470290::iterator);

struct Access_00470290 : Vec_00470290 {
    static DestroyFn_00470290 fn;
};

// std::vector<Elem_004702a0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. Its caller 0x470040 calls
// 0x4702a0 (_Ucopy) and 0x470290 (_Destroy) with ecx set to the vector.
// FUNCTION: 0x470290 ?_Destroy@?$vector@UElem_004702a0@@V?$allocator@UElem_004702a0@@@std@@@std@@IAEXPAUElem_004702a0@@0@Z
DestroyFn_00470290 Access_00470290::fn = &Access_00470290::_Destroy;

typedef std::vector<Elem_004702a0> Vec_004702a0;
typedef Vec_004702a0::iterator (Vec_004702a0::*UcopyFn_004702a0)(
    Vec_004702a0::const_iterator, Vec_004702a0::const_iterator, Vec_004702a0::iterator);

struct Access_004702a0 : Vec_004702a0 {
    static UcopyFn_004702a0 fn;
};

// std::vector<Elem_004702a0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies. Its caller 0x470040 calls 0x4702a0 (_Ucopy) and
// 0x470290 (_Destroy) with ecx set to the vector.
// FUNCTION: 0x4702a0 ?_Ucopy@?$vector@UElem_004702a0@@V?$allocator@UElem_004702a0@@@std@@@std@@IAEPAUElem_004702a0@@PBU3@0PAU3@@Z
UcopyFn_004702a0 Access_004702a0::fn = &Access_004702a0::_Ucopy;

// --- the empty function 0x470030 ---------------------------------------------

// FUNCTION: 0x470030
void __stdcall FUN_00470030(int)
{
}

// --- Class_00470250's and Class_00470270's counts (0x470250, 0x470270) --------

class Class_00470250 {
public:
    char unknown_0[4];
    int first;
    char unknown_8[4];
    int last;

    int FUN_00470250();
};

// FUNCTION: 0x470250
int Class_00470250::FUN_00470250()
{
    if (!first) {
        return 0;
    }
    return (last - first) >> 2;
}

class Class_00470270 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_00470270();
};

// FUNCTION: 0x470270
int Class_00470270::FUN_00470270() {
    if (field_4 == 0) {
        return 0;
    }
    return (field_8 - field_4) >> 2;
}

// --- std::copy for 4-byte elements (0x4702d0) --------------------------------

// std::copy for 4-byte elements, compiled with __stdcall as the default
// convention (same shape as 0x44eef0). Its one caller copies one vector's
// elements into another.
// FUNCTION: 0x4702d0
int* __stdcall FUN_004702d0(int* first, int* last, int* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}

// --- vector<Elem_0046faf0>::size and operator= (0x470770, 0x4707a0) ----------

typedef Vec_0046faf0::size_type (Vec_0046faf0::*SizeFn_00470770)() const;

// std::vector<Elem_0046faf0>::size() for the 14-byte element (three ints and
// a short, packed to 2 bytes), the same vector as _Ucopy (0x46faf0) and the
// operator= at 0x4707a0. Its callers are in 0x470560.
// FUNCTION: 0x470770 ?size@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@QBEIXZ
SizeFn_00470770 g_size_00470770 = &Vec_0046faf0::size;

typedef Vec_0046faf0& (Vec_0046faf0::*AssignFn_0046faf0)(const Vec_0046faf0&);

// std::vector<Elem_0046faf0>::operator= from MSVC 5's <vector>. The element
// type is 14 bytes (three ints and a short, packed to 2), the same vector as
// _Ucopy (0x46faf0), _Destroy (0x46e870), _Ufill (0x46fb40) and size (0x470770).
// Its one caller is PacketSequencer::operator= (0x470560).
// FUNCTION: 0x4707a0 ??4?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@QAEAAV01@ABV01@@Z
AssignFn_0046faf0 g_assign_0046faf0 = &Vec_0046faf0::operator=;
