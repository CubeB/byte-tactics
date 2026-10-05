// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// MSVC 5 <xtree> _Tree::_Insert(_X, _Y, _V). The body below mirrors the
// original XTREE source line for line so the allocator sees the same live
// ranges.
#include <string.h>
#include <yvals.h>

class Class_004e1a30 {
public:
    char* name;                        // +0x0
    bool FUN_004e1a30(const Class_004e1a30& other) const;
};

struct Value_004e2620 {
    Class_004e1a30 key;                // +0x0
    char text[500];                    // +0x4

    Value_004e2620& operator=(const Value_004e2620& v)
    {
        if (this)
            memcpy(this, &v, sizeof(Value_004e2620));
        return *this;
    }
};

struct Node_004e2620 {
    Node_004e2620* left;               // +0x0
    Node_004e2620* parent;             // +0x4
    Node_004e2620* right;              // +0x8
    Value_004e2620 value;              // +0xc
    int color;                         // +0x204
};

extern Node_004e2620* DAT_005292c4;    // the tree's _Nil

class Class_004e2b60 {
public:
    void* FUN_004e2b60(unsigned int n);
};

struct Less_004e2620 {
    bool operator()(const Class_004e1a30& a, const Class_004e1a30& b) const
    {
        return a.FUN_004e1a30(b);
    }
};

class Iter_004e2620 {
public:
    Node_004e2620* ptr;

    Iter_004e2620() {}
    Iter_004e2620(Node_004e2620* p) : ptr(p) {}
};

class Class_004e2620 {
public:
    Less_004e2620 compare;             // +0x0
    Node_004e2620* head;               // +0x4
    int nilref;                        // +0x8
    unsigned int size;                 // +0xc

    Node_004e2620*& _Root() { return head->parent; }
    Node_004e2620*& _Lmost() { return head->left; }
    Node_004e2620*& _Rmost() { return head->right; }

    static Node_004e2620*& _Left(Node_004e2620* p) { return p->left; }
    static Node_004e2620*& _Right(Node_004e2620* p) { return p->right; }
    static Node_004e2620*& _Parent(Node_004e2620* p) { return p->parent; }
    static int& _Color(Node_004e2620* p) { return p->color; }

    void _Lrotate(Node_004e2620* _X)
    {
        std::_Lockit _Lk;
        Node_004e2620* _Y = _Right(_X);
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
    void _Rrotate(Node_004e2620* _X)
    {
        std::_Lockit _Lk;
        Node_004e2620* _Y = _Left(_X);
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

    Iter_004e2620 FUN_004e2620(Node_004e2620* _X, Node_004e2620* _Y, const Value_004e2620& _V);
};

// FUNCTION: 0x4e2620
Iter_004e2620 Class_004e2620::FUN_004e2620(Node_004e2620* _X, Node_004e2620* _Y, const Value_004e2620& _V)
{
    std::_Lockit _Lk;
    Node_004e2620* _Z = (Node_004e2620*)((Class_004e2b60*)this)->FUN_004e2b60(0x208);
    _Parent(_Z) = _Y;
    _Color(_Z) = 0;
    _Left(_Z) = DAT_005292c4;
    _Right(_Z) = DAT_005292c4;
    _Z->value = _V;
    ++size;
    if (_Y == head || _X != DAT_005292c4 || compare(_V.key, _Y->value.key)) {
        _Left(_Y) = _Z;
        if (_Y == head) {
            _Root() = _Z;
            _Rmost() = _Z;
        } else if (_Y == _Lmost())
            _Lmost() = _Z;
    } else {
        _Right(_Y) = _Z;
        if (_Y == _Rmost())
            _Rmost() = _Z;
    }
    for (_X = _Z; _X != _Root() && _Color(_Parent(_X)) == 0; ) {
        if (_Parent(_X) == _Left(_Parent(_Parent(_X)))) {
            _Y = _Right(_Parent(_Parent(_X)));
            if (_Color(_Y) == 0) {
                _Color(_Parent(_X)) = 1;
                _Color(_Y) = 1;
                _Color(_Parent(_Parent(_X))) = 0;
                _X = _Parent(_Parent(_X));
            } else {
                if (_X == _Right(_Parent(_X))) {
                    _X = _Parent(_X);
                    _Lrotate(_X);
                }
                _Color(_Parent(_X)) = 1;
                _Color(_Parent(_Parent(_X))) = 0;
                _Rrotate(_Parent(_Parent(_X)));
            }
        } else {
            _Y = _Left(_Parent(_Parent(_X)));
            if (_Color(_Y) == 0) {
                _Color(_Parent(_X)) = 1;
                _Color(_Y) = 1;
                _Color(_Parent(_Parent(_X))) = 0;
                _X = _Parent(_Parent(_X));
            } else {
                if (_X == _Left(_Parent(_X))) {
                    _X = _Parent(_X);
                    _Rrotate(_X);
                }
                _Color(_Parent(_X)) = 1;
                _Color(_Parent(_Parent(_X))) = 0;
                _Lrotate(_Parent(_Parent(_X)));
            }
        }
    }
    _Color(_Root()) = 1;
    return Iter_004e2620(_Z);
}
