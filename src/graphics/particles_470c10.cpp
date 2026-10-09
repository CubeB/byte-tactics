// Decompiled by space-bunny-free. Names are provisional.
// Stays in its own file: its Grow needs the hand-written <vector> view of the
// pool, which cannot share a file with the real <vector> (particles.cpp).
// Grows the arena ObjectPool (vtable 0x4fd580, see 0x470a90.cpp and
// 0x470ae0.cpp) to param_1 slots of param_2 bytes. The table of slot
// pointers is reallocated with GameReallocIgnoreTag, the raw memory for the new
// slots comes from GameAllocShared, and the base of that block is pushed on
// the vector of blocks that the destructor frees one by one (0x470e50).
#include <stddef.h>

void* __cdecl GameAllocShared(int size);
void* __cdecl GameReallocIgnoreTag(void* table, int size);
void __stdcall CopyPointer(void* dest, void* src);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

struct Elem_00470f00 {
    void* p;                            // +0x0, the base of one memory block
};

namespace std {
    template<class _Ty> inline
    _Ty* _Allocate(ptrdiff_t _N, _Ty*)
        {if (_N < 0)
            _N = 0;
        return ((_Ty*)operator new((size_t)_N * sizeof(_Ty))); }

    template<class _Ty> class allocator {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef _Ty* pointer;
        typedef const _Ty* const_pointer;
        typedef _Ty& reference;
        typedef const _Ty& const_reference;
        typedef _Ty value_type;
        pointer allocate(size_type _N, const void*)
            {return ((pointer)_Allocate((ptrdiff_t)_N, (pointer)0)); }
        void deallocate(void* _P, size_type)
            {operator delete(_P); }
    };

    template<class _FI, class _Ty> inline
    void fill(_FI _F, _FI _L, const _Ty& _X)
        {for (; _F != _L; ++_F)
        *_F = _X; }

    template<class _BI1, class _BI2> inline
    _BI2 copy_backward(_BI1 _F, _BI1 _L, _BI2 _X)
        {while (_F != _L)
        *--_X = *--_L;
    return (_X); }

    template<class _Ty, class _A = allocator<_Ty> > class vector {
    public:
        typedef _A::size_type size_type;
        typedef _Ty* iterator;
        typedef const _Ty* const_iterator;
        _A alloc;
        iterator _First;
        iterator _Last;
        iterator _End;
        explicit vector(const _A& _Al = _A())
            : alloc(_Al), _First(0), _Last(0), _End(0) {}
        iterator begin()
            {return (_First); }
        iterator end()
            {return (_Last); }
        size_type size() const
            {return (_First == 0 ? 0 : _Last - _First); }
        void push_back(const _Ty& _X)
            {insert(end(), 1, _X); }
        void insert(iterator _P, size_type _M, const _Ty& _X)
            {if (_End - _Last < _M)
                {size_type _N = size() + (_M < size() ? size() : _M);
                iterator _S = alloc.allocate(_N, (void*)0);
                iterator _Q = _S;
                for (iterator _F = _First; _F != _P; ++_F, ++_Q)
                    CopyPointer(_Q, _F);
                _Ufill(_Q, _M, _X);
                _Ucopy(_P, _Last, _Q + _M);
                _Destroy(_First, _Last);
                alloc.deallocate(_First, _End - _First);
                _End = _S + _N;
                _Last = _S + size() + _M;
                _First = _S; }
            else if (_Last - _P < _M)
                {_Ucopy(_P, _Last, _P + _M);
                _Ufill(_Last, _M - (_Last - _P), _X);
                fill(_P, _Last, _X);
                _Last += _M; }
            else if (0 < _M)
                {_Ucopy(_Last - _M, _Last, _Last);
                copy_backward(_P, _Last - _M, _Last);
                fill(_P, _P + _M, _X);
                _Last += _M; }}
    protected:
        iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P);
        void _Ufill(iterator _P, size_type _N, const _Ty& _X);
        void _Destroy(iterator _F, iterator _L);
    };
}

class ObjectPool {
public:
    int unknown_0;                             // +0x0, the vtable pointer
    std::vector<Elem_00470f00> items;          // +0x4, the blocks in use
    Elem_00470f00** slots;                     // +0x14, the slot table
    int slotSize;                              // +0x18, the slot size
    int capacity;                              // +0x1c, the slot count
    int used;                                  // +0x20, slots handed out

    int Grow(int param_1, int param_2);
};

// FUNCTION: 0x470c10
int ObjectPool::Grow(int param_1, int param_2)
{
    int result = 0;
    if (param_1 > capacity) {
        Elem_00470f00** table = (Elem_00470f00**)GameReallocIgnoreTag(slots, param_1 * 4);
        if (table != 0) {
            Elem_00470f00 block;
            block.p = GameAllocShared((param_1 - capacity) * param_2);
            slots = table;
            if (block.p != 0) {
                int i = capacity;
                if (i < param_1) {
                    int offset = 0;
                    do {
                        slots[i] = (Elem_00470f00*)((char*)block.p + offset);
                        offset += param_2;
                        i++;
                    } while (i < param_1);
                }
                items.push_back(block);
                capacity = param_1;
                slotSize = param_2;
                result = 1;
            }
        }
    }
    else {
        result = 1;
    }
    return result;
}
