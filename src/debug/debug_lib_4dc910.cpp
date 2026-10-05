// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::erase(iterator) from MSVC 5's <xtree>, hand-written for the
// game's file-record map: the same tree whose insert is 0x4dc680 and whose
// _Nil node is DAT_00528a50. Unlike 0x46f1e0 (plain std::allocator), this
// tree's allocator is a pool: _Freenode inlines to a push onto the free list
// at DAT_005289e0, not an operator delete call. The value_type is 0x30 bytes
// (key first), so the node's colour flag lands at +0x3c.
//
// Written out by hand rather than through a std::map instantiation because the
// iterator's out-of-line _Inc (0x4dde70) is already recorded in
// data/symbols.csv as Class_004dde70::FUN_004dde70, and check.py rejects any
// other name for that call target. The hand-written operator++ therefore calls
// that exact member, which both matches the bytes and satisfies the reference.
#include <yvals.h>
#include <algorithm>

struct Value_004dc910 { char unknown_0[0x30]; };

struct Node_004dc910 {
    Node_004dc910* _Left;
    Node_004dc910* _Parent;
    Node_004dc910* _Right;
    Value_004dc910 _Value;
    int _Color;
};

extern Node_004dc910* DAT_00528a50;
extern void* DAT_005289e0;

class Class_004dde70 {
public:
    void FUN_004dde70();
};

class Iter_004dc910 {
public:
    Node_004dc910* _Ptr;
    Iter_004dc910() {}
    Iter_004dc910(Node_004dc910* _P) : _Ptr(_P) {}
    Iter_004dc910 operator++(int)
        { Iter_004dc910 _Tmp = *this;
          ((Class_004dde70*)this)->FUN_004dde70();
          return (_Tmp); }
    Node_004dc910* _Mynode() const { return _Ptr; }
};

class Class_004dc910 {
public:
    int unknown_0;
    Node_004dc910* _Head;              // +4
    unsigned char _Multi;              // +8
    int _Size;                         // +0xc

    Node_004dc910*& _Root() { return _Head->_Parent; }
    Node_004dc910*& _Lmost() { return _Head->_Left; }
    Node_004dc910*& _Rmost() { return _Head->_Right; }

    static Node_004dc910* _Min(Node_004dc910* _P)
        { std::_Lockit _Lk;
          while (_P->_Left != DAT_00528a50)
              _P = _P->_Left;
          return _P; }

    static Node_004dc910* _Max(Node_004dc910* _P)
        { std::_Lockit _Lk;
          while (_P->_Right != DAT_00528a50)
              _P = _P->_Right;
          return _P; }

    void _Lrotate(Node_004dc910* _X)
        { std::_Lockit _Lk;
          Node_004dc910* _Y = _X->_Right;
          _X->_Right = _Y->_Left;
          if (_Y->_Left != DAT_00528a50)
              _Y->_Left->_Parent = _X;
          _Y->_Parent = _X->_Parent;
          if (_X == _Root())
              _Root() = _Y;
          else if (_X == _X->_Parent->_Left)
              _X->_Parent->_Left = _Y;
          else
              _X->_Parent->_Right = _Y;
          _Y->_Left = _X;
          _X->_Parent = _Y; }

    void _Rrotate(Node_004dc910* _X)
        { std::_Lockit _Lk;
          Node_004dc910* _Y = _X->_Left;
          _X->_Left = _Y->_Right;
          if (_Y->_Right != DAT_00528a50)
              _Y->_Right->_Parent = _X;
          _Y->_Parent = _X->_Parent;
          if (_X == _Root())
              _Root() = _Y;
          else if (_X == _X->_Parent->_Right)
              _X->_Parent->_Right = _Y;
          else
              _X->_Parent->_Left = _Y;
          _Y->_Right = _X;
          _X->_Parent = _Y; }

    Iter_004dc910 erase(Iter_004dc910 _P);
};

// FUNCTION: 0x4dc910
Iter_004dc910 Class_004dc910::erase(Iter_004dc910 _P)
{
    Node_004dc910* _X;
    Node_004dc910* _Y = (_P++)._Mynode();
    Node_004dc910* _Z = _Y;
    std::_Lockit _Lk;
    if (_Y->_Left == DAT_00528a50)
        _X = _Y->_Right;
    else if (_Y->_Right == DAT_00528a50)
        _X = _Y->_Left;
    else
        _Y = _Min(_Y->_Right), _X = _Y->_Right;
    if (_Y != _Z)
        {
        _Z->_Left->_Parent = _Y;
        _Y->_Left = _Z->_Left;
        if (_Y == _Z->_Right)
            _X->_Parent = _Y;
        else
            {
            _X->_Parent = _Y->_Parent;
            _Y->_Parent->_Left = _X;
            _Y->_Right = _Z->_Right;
            _Z->_Right->_Parent = _Y;
            }
        if (_Root() == _Z)
            _Root() = _Y;
        else if (_Z->_Parent->_Left == _Z)
            _Z->_Parent->_Left = _Y;
        else
            _Z->_Parent->_Right = _Y;
        _Y->_Parent = _Z->_Parent;
        std::swap(_Y->_Color, _Z->_Color);
        _Y = _Z;
        }
    else
        {
        _X->_Parent = _Y->_Parent;
        if (_Root() == _Z)
            _Root() = _X;
        else if (_Z->_Parent->_Left == _Z)
            _Z->_Parent->_Left = _X;
        else
            _Z->_Parent->_Right = _X;
        if (_Lmost() != _Z)
            ;
        else if (_Z->_Right == DAT_00528a50)
            _Lmost() = _Z->_Parent;
        else
            _Lmost() = _Min(_X);
        if (_Rmost() != _Z)
            ;
        else if (_Z->_Left == DAT_00528a50)
            _Rmost() = _Z->_Parent;
        else
            _Rmost() = _Max(_X);
        }
    if (_Y->_Color == 1)
        {
        while (_X != _Root() && _X->_Color == 1)
            if (_X == _X->_Parent->_Left)
                {
                Node_004dc910* _W = _X->_Parent->_Right;
                if (_W->_Color == 0)
                    {
                    _W->_Color = 1;
                    _X->_Parent->_Color = 0;
                    _Lrotate(_X->_Parent);
                    _W = _X->_Parent->_Right;
                    }
                if (_W->_Left->_Color == 1 && _W->_Right->_Color == 1)
                    {
                    _W->_Color = 0;
                    _X = _X->_Parent;
                    }
                else
                    {
                    if (_W->_Right->_Color == 1)
                        {
                        _W->_Left->_Color = 1;
                        _W->_Color = 0;
                        _Rrotate(_W);
                        _W = _X->_Parent->_Right;
                        }
                    _W->_Color = _X->_Parent->_Color;
                    _X->_Parent->_Color = 1;
                    _W->_Right->_Color = 1;
                    _Lrotate(_X->_Parent);
                    break;
                    }
                }
            else
                {
                Node_004dc910* _W = _X->_Parent->_Left;
                if (_W->_Color == 0)
                    {
                    _W->_Color = 1;
                    _X->_Parent->_Color = 0;
                    _Rrotate(_X->_Parent);
                    _W = _X->_Parent->_Left;
                    }
                if (_W->_Right->_Color == 1 && _W->_Left->_Color == 1)
                    {
                    _W->_Color = 0;
                    _X = _X->_Parent;
                    }
                else
                    {
                    if (_W->_Left->_Color == 1)
                        {
                        _W->_Right->_Color = 1;
                        _W->_Color = 0;
                        _Lrotate(_W);
                        _W = _X->_Parent->_Left;
                        }
                    _W->_Color = _X->_Parent->_Color;
                    _X->_Parent->_Color = 1;
                    _W->_Left->_Color = 1;
                    _Rrotate(_X->_Parent);
                    break;
                    }
                }
        _X->_Color = 1;
        }
    if (_Y != 0)
        {
        *(void**)_Y = DAT_005289e0;
        DAT_005289e0 = _Y;
        }
    --_Size;
    return (_P);
}
