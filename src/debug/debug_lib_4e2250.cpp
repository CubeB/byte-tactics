// Decompiled by space-bunny-free, finished by Claude Opus 5.5. Names are provisional.
// std::_Tree<...>::insert(const value_type&) from MSVC 5's <xtree>, for the
// name map whose key is a C-string class and whose value is a 500-byte
// buffer (pair<const key, value> is 0x1f8 bytes, the node 0x208). The <map>
// instantiation, std::map<NameKey, Value_004e2250,
// std::less<NameKey>, pooled allocator>, compiles to these 801 bytes
// exactly; this file transcribes it member for member so that every
// out-of-line callee keeps the name it already has in data/symbols.csv.
// Each _Tree member lives on the class that names its address, chained by
// inheritance: _Rrotate 0x4e29b0, _Lrotate 0x4e2950, _Buynode 0x4e2a30,
// _Insert 0x4e2620, iterator::_Dec 0x4e2ab0, the key's operator< 0x4e1a30.
// The /Ob2 budget decides the shape, so the transcription keeps the XTREE
// bodies and accessors as they are (c2prio.py --inline shows the same
// decisions as the real template): the key compare inlines only in the
// search loop (it must be the one-expression `return a != b && strcmp(a, b)
// < 0;`; the if/return form is 60 IL, over the loop's share of 55), _Insert
// (788 IL) inlines only on the _Multi path, and the budget left after the
// second pair<iterator, bool> constructor (41 IL) is 39, so the last two
// call the constructor out of line at 0x4e2a10. That constructor is matched
// as the method Class_004e2a10::FUN_004e2a10 in 0x4e2a30.cpp, so
// data/aliases.csv gives it its constructor name as well.
// The iterator returned by the out-of-line _Insert at 0x4e24c3 is built in
// the _V argument slot, which is dead once _V has been pushed; that is the
// compiler reusing the slot, not a bug.
#include <string.h>
#include <map>

// The key: a C string ordered by strcmp. Its operator< is the out-of-line
// 0x4e1a30.
class NameKey {
public:
    char* name;                        // +0x0

    bool FUN_004e1a30(const NameKey& o) const
    {
        return name != o.name && strcmp(name, o.name) < 0;
    }
};

struct Value_004e2250 {
    char text[500];
};

typedef std::pair<const NameKey, Value_004e2250> Pair_004e2250;

// std::less<key>.
struct Less_004e2250 : public std::binary_function<NameKey, NameKey, bool> {
    bool operator()(const NameKey& _X, const NameKey& _Y) const
    {
        return (_X.FUN_004e1a30(_Y));
    }
};

// std::map<...>::_Kfn.
struct Kfn_004e2250 : public std::unary_function<Pair_004e2250, NameKey> {
    const NameKey& operator()(const Pair_004e2250& _X) const
    {
        return (_X.first);
    }
};

class Alloc_004e2b60 {
public:
    char* FUN_004e2b60(unsigned int n);
};

enum _Redbl_004e2250 { _Red, _Black };

struct Node_004e2250 {
    void* _Left;                       // +0x0
    void* _Parent;                     // +0x4
    void* _Right;                      // +0x8
    Pair_004e2250 _Value;              // +0xc
    _Redbl_004e2250 _Color;            // +0x204
};

typedef Node_004e2250* _Nodeptr;

extern _Nodeptr DAT_005292c4;          // the tree's _Nil

// The members and static accessors of std::_Tree.
class Tree_004e2250 {
public:
    static _Redbl_004e2250& _Color(_Nodeptr _P)
        {return ((_Redbl_004e2250&)(*_P)._Color); }
    static const NameKey& _Key(_Nodeptr _P)
        {return (Kfn_004e2250()(_Value(_P))); }
    static _Nodeptr& _Left(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _P)
        {return ((_Nodeptr&)(*_P)._Right); }
    static Pair_004e2250& _Value(_Nodeptr _P)
        {return ((Pair_004e2250&)(*_P)._Value); }
    static _Nodeptr _Max(_Nodeptr _P)
        {std::_Lockit _Lk;
        while (_Right(_P) != DAT_005292c4)
            _P = _Right(_P);
        return (_P); }
    _Nodeptr& _Lmost()
        {return (_Left(_Head)); }
    _Nodeptr& _Rmost()
        {return (_Right(_Head)); }
    _Nodeptr& _Root()
        {return (_Parent(_Head)); }

    Alloc_004e2b60 allocator;          // +0x0
    Less_004e2250 key_compare;         // +0x1
    _Nodeptr _Head;                    // +0x4
    bool _Multi;                       // +0x8
    unsigned int _Size;                // +0xc
};

// std::_Tree<...>::iterator; _Dec is 0x4e2ab0.
class Class_004e2ab0 : public std::_Bidit<Pair_004e2250, int> {
public:
    Class_004e2ab0()
        {}
    Class_004e2ab0(_Nodeptr _P)
        : _Ptr(_P) {}
    Class_004e2ab0& operator--()
        {FUN_004e2ab0();
        return (*this); }
    bool operator==(const Class_004e2ab0& _X) const
        {return (_Ptr == _X._Ptr); }
    void FUN_004e2ab0()
        {std::_Lockit _Lk;
        if (Tree_004e2250::_Color(_Ptr) == _Red
            && Tree_004e2250::_Parent(Tree_004e2250::_Parent(_Ptr)) == _Ptr)
            _Ptr = Tree_004e2250::_Right(_Ptr);
        else if (Tree_004e2250::_Left(_Ptr) != DAT_005292c4)
            _Ptr = Tree_004e2250::_Max(Tree_004e2250::_Left(_Ptr));
        else
            {_Nodeptr _P;
            while (_Ptr == Tree_004e2250::_Left(_P = Tree_004e2250::_Parent(_Ptr)))
                _Ptr = _P;
            _Ptr = _P; }}
    _Nodeptr _Mynode() const
        {return (_Ptr); }
protected:
    _Nodeptr _Ptr;
};

// std::pair<iterator, bool>; its constructor is 0x4e2a10.
class Class_004e2a10 {
public:
    Class_004e2a10(const Class_004e2ab0& _V1, const bool& _V2)
        : first(_V1), second(_V2) {}
    Class_004e2ab0 first;
    bool second;
};

// _Rrotate (0x4e29b0).
class Class_004e29b0 : public Tree_004e2250 {
public:
    void FUN_004e29b0(_Nodeptr _X)
        {std::_Lockit _Lk;
        _Nodeptr _Y = _Left(_X);
        _Left(_X) = _Right(_Y);
        if (_Right(_Y) != DAT_005292c4)
            _Parent(_Right(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Right(_Parent(_X)))
            _Right(_Parent(_X)) = _Y;
        else
            _Left(_Parent(_X)) = _Y;
        _Right(_Y) = _X;
        _Parent(_X) = _Y; }
};

// _Lrotate (0x4e2950).
class Class_004e2950 : public Class_004e29b0 {
public:
    void FUN_004e2950(_Nodeptr _X)
        {std::_Lockit _Lk;
        _Nodeptr _Y = _Right(_X);
        _Right(_X) = _Left(_Y);
        if (_Left(_Y) != DAT_005292c4)
            _Parent(_Left(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Left(_Parent(_X)))
            _Left(_Parent(_X)) = _Y;
        else
            _Right(_Parent(_X)) = _Y;
        _Left(_Y) = _X;
        _Parent(_X) = _Y; }
};

// _Buynode (0x4e2a30).
class Class_004e2a30 : public Class_004e2950 {
public:
    _Nodeptr FUN_004e2a30(_Nodeptr _Parg, _Redbl_004e2250 _Carg)
        {_Nodeptr _S = (_Nodeptr)allocator.FUN_004e2b60(
            1 * sizeof (Node_004e2250));
        _Parent(_S) = _Parg;
        _Color(_S) = _Carg;
        return (_S); }
    void _Consval(Pair_004e2250* _P, const Pair_004e2250& _V)
        {std::_Construct(&*_P, _V); }
};

// _Insert (0x4e2620).
class Class_004e2620 : public Class_004e2a30 {
public:
    Class_004e2ab0 FUN_004e2620(_Nodeptr _X, _Nodeptr _Y, const Pair_004e2250& _V)
        {std::_Lockit _Lk;
        _Nodeptr _Z = FUN_004e2a30(_Y, _Red);
        _Left(_Z) = DAT_005292c4, _Right(_Z) = DAT_005292c4;
        _Consval(&_Value(_Z), _V);
        ++_Size;
        if (_Y == _Head || _X != DAT_005292c4
            || key_compare(Kfn_004e2250()(_V), _Key(_Y)))
            {_Left(_Y) = _Z;
            if (_Y == _Head)
                {_Root() = _Z;
                _Rmost() = _Z; }
            else if (_Y == _Lmost())
                _Lmost() = _Z; }
        else
            {_Right(_Y) = _Z;
            if (_Y == _Rmost())
                _Rmost() = _Z; }
        for (_X = _Z; _X != _Root()
            && _Color(_Parent(_X)) == _Red; )
            if (_Parent(_X) == _Left(_Parent(_Parent(_X))))
                {_Y = _Right(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                    {_Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X)); }
                else
                    {if (_X == _Right(_Parent(_X)))
                        {_X = _Parent(_X);
                        FUN_004e2950(_X); }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    FUN_004e29b0(_Parent(_Parent(_X))); }}
            else
                {_Y = _Left(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                    {_Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X)); }
                else
                    {if (_X == _Left(_Parent(_X)))
                        {_X = _Parent(_X);
                        FUN_004e29b0(_X); }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    FUN_004e2950(_Parent(_Parent(_X))); }}
        _Color(_Root()) = _Black;
        return (Class_004e2ab0(_Z)); }
};

// std::_Tree<...>::insert(const value_type&).
class Class_004e2250 : public Class_004e2620 {
public:
    Class_004e2ab0 begin()
        {return (Class_004e2ab0(_Lmost())); }
    Class_004e2a10 FUN_004e2250(const Pair_004e2250& _V);
};

// FUNCTION: 0x4e2250
Class_004e2a10 Class_004e2250::FUN_004e2250(const Pair_004e2250& _V)
{
    _Nodeptr _X = _Root();
    _Nodeptr _Y = _Head;
    bool _Ans = true;
    {
        std::_Lockit Lk;
        while (_X != DAT_005292c4) {
            _Y = _X;
            _Ans = key_compare(Kfn_004e2250()(_V), _Key(_X));
            _X = _Ans ? _Left(_X) : _Right(_X);
        }
    }
    if (_Multi)
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    Class_004e2ab0 _P = Class_004e2ab0(_Y);
    if (!_Ans)
        ;
    else if (_P == begin())
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    else
        --_P;
    if (key_compare(_Key(_P._Mynode()), Kfn_004e2250()(_V)))
        return (Class_004e2a10(FUN_004e2620(_X, _Y, _V), true));
    return (Class_004e2a10(_P, false));
}
