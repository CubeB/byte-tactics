// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::erase(iterator) for the 500-byte-value name map that lives
// at NameTable+0x21c (tree at 0x5294ec, _Nil at 0x5292c4). The 0x1f8-byte
// _Value setting _Color at +0x204 is the only thing the erased node's value
// type contributes (its destructor is trivial, so _Destval emits nothing).
#include <yvals.h>
#include <algorithm>

struct Node_004dfea0 {
    Node_004dfea0* _Left;              // +0x0
    Node_004dfea0* _Parent;            // +0x4
    Node_004dfea0* _Right;             // +0x8
    char _Value[0x1f8];                // +0xc
    int _Color;                        // +0x204  (0 = _Red, 1 = _Black)
};

// The tree iterator; passed by value and returned by value, so the caller
// supplies a hidden return buffer.
// Hand-written <xtree>, not <map>: only classes with the literal callee names
// (Class_004e0450, Class_004dfea0) produce those symbols.
class Class_004e0450 {
public:
    Node_004dfea0* _Ptr;               // +0x0

    void FUN_004e0450();               // _Inc
    Class_004e0450& operator++() { FUN_004e0450(); return *this; }
    Class_004e0450 operator++(int)
    {
        Class_004e0450 _Tmp = *this;
        ++*this;
        return (_Tmp);
    }
    Node_004dfea0* _Mynode() const { return _Ptr; }
    bool operator==(const Class_004e0450& _X) const { return _Ptr == _X._Ptr; }
    bool operator!=(const Class_004e0450& _X) const { return !(*this == _X); }
};

extern Node_004dfea0* DAT_005292c4;    // tree _Nil
extern void* DAT_00529e58;             // node free list head

class Class_004dfea0 {
public:
    char _Alnod[4];                    // +0x0
    Node_004dfea0* _Head;              // +0x4
    char _Multi;                       // +0x8
    char pad_9[3];
    unsigned int _Size;                // +0xc

    static int& _Color(Node_004dfea0* _P) { return _P->_Color; }
    static Node_004dfea0*& _Left(Node_004dfea0* _P) { return _P->_Left; }
    static Node_004dfea0*& _Parent(Node_004dfea0* _P) { return _P->_Parent; }
    static Node_004dfea0*& _Right(Node_004dfea0* _P) { return _P->_Right; }
    Node_004dfea0*& _Root() const { return _Head->_Parent; }
    Node_004dfea0*& _Lmost() const { return _Head->_Left; }
    Node_004dfea0*& _Rmost() const { return _Head->_Right; }

    static Node_004dfea0* _Min(Node_004dfea0* _P)
    {
        std::_Lockit _Lk;
        while (_Left(_P) != DAT_005292c4)
            _P = _Left(_P);
        return (_P);
    }
    static Node_004dfea0* _Max(Node_004dfea0* _P)
    {
        std::_Lockit _Lk;
        while (_Right(_P) != DAT_005292c4)
            _P = _Right(_P);
        return (_P);
    }
    void _Lrotate(Node_004dfea0* _X)
    {
        std::_Lockit _Lk;
        Node_004dfea0* _Y = _Right(_X);
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
        _Parent(_X) = _Y;
    }
    void _Rrotate(Node_004dfea0* _X)
    {
        std::_Lockit _Lk;
        Node_004dfea0* _Y = _Left(_X);
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
        _Parent(_X) = _Y;
    }
    static void _Freenode(Node_004dfea0* _P)
    {
        if (_P != 0) {
            *(void**)_P = DAT_00529e58;
            DAT_00529e58 = _P;
        }
    }

    Class_004e0450 FUN_004dfea0(Class_004e0450 _P);
};

// FUNCTION: 0x4dfea0
Class_004e0450 Class_004dfea0::FUN_004dfea0(Class_004e0450 _P)
{
    Node_004dfea0* _X;
    Node_004dfea0* _Y = (_P++)._Mynode();
    Node_004dfea0* _Z = _Y;
    std::_Lockit _Lk;
    if (_Left(_Y) == DAT_005292c4)
        _X = _Right(_Y);
    else if (_Right(_Y) == DAT_005292c4)
        _X = _Left(_Y);
    else
        _Y = _Min(_Right(_Y)), _X = _Right(_Y);
    if (_Y != _Z) {
        _Parent(_Left(_Z)) = _Y;
        _Left(_Y) = _Left(_Z);
        if (_Y == _Right(_Z))
            _Parent(_X) = _Y;
        else {
            _Parent(_X) = _Parent(_Y);
            _Left(_Parent(_Y)) = _X;
            _Right(_Y) = _Right(_Z);
            _Parent(_Right(_Z)) = _Y;
        }
        if (_Root() == _Z)
            _Root() = _Y;
        else if (_Left(_Parent(_Z)) == _Z)
            _Left(_Parent(_Z)) = _Y;
        else
            _Right(_Parent(_Z)) = _Y;
        _Parent(_Y) = _Parent(_Z);
        std::swap(_Color(_Y), _Color(_Z));
        _Y = _Z;
    } else {
        _Parent(_X) = _Parent(_Y);
        if (_Root() == _Z)
            _Root() = _X;
        else if (_Left(_Parent(_Z)) == _Z)
            _Left(_Parent(_Z)) = _X;
        else
            _Right(_Parent(_Z)) = _X;
        if (_Lmost() != _Z)
            ;
        else if (_Right(_Z) == DAT_005292c4)
            _Lmost() = _Parent(_Z);
        else
            _Lmost() = _Min(_X);
        if (_Rmost() != _Z)
            ;
        else if (_Left(_Z) == DAT_005292c4)
            _Rmost() = _Parent(_Z);
        else
            _Rmost() = _Max(_X);
    }
    if (_Color(_Y) == 1) {
        while (_X != _Root() && _Color(_X) == 1)
            if (_X == _Left(_Parent(_X))) {
                Node_004dfea0* _W = _Right(_Parent(_X));
                if (_Color(_W) == 0) {
                    _Color(_W) = 1;
                    _Color(_Parent(_X)) = 0;
                    _Lrotate(_Parent(_X));
                    _W = _Right(_Parent(_X));
                }
                if (_Color(_Left(_W)) == 1
                    && _Color(_Right(_W)) == 1) {
                    _Color(_W) = 0;
                    _X = _Parent(_X);
                } else {
                    if (_Color(_Right(_W)) == 1) {
                        _Color(_Left(_W)) = 1;
                        _Color(_W) = 0;
                        _Rrotate(_W);
                        _W = _Right(_Parent(_X));
                    }
                    _Color(_W) = _Color(_Parent(_X));
                    _Color(_Parent(_X)) = 1;
                    _Color(_Right(_W)) = 1;
                    _Lrotate(_Parent(_X));
                    break;
                }
            } else {
                Node_004dfea0* _W = _Left(_Parent(_X));
                if (_Color(_W) == 0) {
                    _Color(_W) = 1;
                    _Color(_Parent(_X)) = 0;
                    _Rrotate(_Parent(_X));
                    _W = _Left(_Parent(_X));
                }
                if (_Color(_Right(_W)) == 1
                    && _Color(_Left(_W)) == 1) {
                    _Color(_W) = 0;
                    _X = _Parent(_X);
                } else {
                    if (_Color(_Left(_W)) == 1) {
                        _Color(_Right(_W)) = 1;
                        _Color(_W) = 0;
                        _Lrotate(_W);
                        _W = _Left(_Parent(_X));
                    }
                    _Color(_W) = _Color(_Parent(_X));
                    _Color(_Parent(_X)) = 1;
                    _Color(_Left(_W)) = 1;
                    _Rrotate(_Parent(_X));
                    break;
                }
            }
        _Color(_X) = 1;
    }
    _Freenode(_Y);
    --_Size;
    return (_P);
}
