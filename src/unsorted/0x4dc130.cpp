// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::erase(iterator) from MSVC 5's <xtree>, written out by hand
// rather than via <map> so that the iterator increment is the call the original
// makes: 0x4dd340, which data/symbols.csv names
// Class_004dd340::FUN_004dd340. The real std::map template compiles this body
// to byte-identical code too, but it emits the callee as iterator::_Inc and the
// checker then refuses the reference. The tree's _Nil node is DAT_00528a54 and
// its node allocator is the pool at 0x4ddd70 (free list DAT_00528a10, node size
// 0x18, _Color at +0x14). _Min, _Max, _Lrotate and _Rrotate are inlined, each
// dragging in its own std::_Lockit.
#include <yvals.h>
#include <map>

struct Node_004dc130 {
    Node_004dc130* left;               // +0x0
    Node_004dc130* parent;             // +0x4
    Node_004dc130* right;              // +0x8
    unsigned int key;                  // +0xc (_Value.first)
    unsigned int value;                // +0x10 (_Value.second)
    int color;                         // +0x14 (_Color)
};

extern Node_004dc130* DAT_00528a54;    // _Nil
extern void* DAT_00528a10;             // pool free list

class Class_004dd340 {
public:
    Node_004dc130* ptr;                // +0x0

    void FUN_004dd340();               // _Inc, out of line at 0x4dd340

    Class_004dd340& operator++()
    {
        FUN_004dd340();
        return *this;
    }

    Class_004dd340 operator++(int)
    {
        Class_004dd340 _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    Node_004dc130* _Mynode() const { return ptr; }
};

struct Alloc_004dc130 { };             // empty, at the tree's +0
struct Less_004dc130 { };              // empty, at the tree's +1

class Class_004dc130 {
public:
    enum Redbl { _Red, _Black };

    Alloc_004dc130 allocator;          // +0x0
    Less_004dc130 key_compare;         // +0x1
    Node_004dc130* head;               // +0x4
    bool multi;                        // +0x8
    unsigned int size;                 // +0xc

    static Node_004dc130*& Left(Node_004dc130* p) { return p->left; }
    static Node_004dc130*& Parent(Node_004dc130* p) { return p->parent; }
    static Node_004dc130*& Right(Node_004dc130* p) { return p->right; }
    static int& Color(Node_004dc130* p) { return p->color; }
    static unsigned int* _Value(Node_004dc130* p) { return &p->key; }

    Node_004dc130*& Root() { return Parent(head); }
    Node_004dc130*& Lmost() { return Left(head); }
    Node_004dc130*& Rmost() { return Right(head); }

    static Node_004dc130* Min(Node_004dc130* p)
    {
        std::_Lockit _Lk;
        while (Left(p) != DAT_00528a54)
            p = Left(p);
        return p;
    }

    static Node_004dc130* Max(Node_004dc130* p)
    {
        std::_Lockit _Lk;
        while (Right(p) != DAT_00528a54)
            p = Right(p);
        return p;
    }

    void Lrotate(Node_004dc130* x)
    {
        std::_Lockit _Lk;
        Node_004dc130* y = Right(x);
        Right(x) = Left(y);
        if (Left(y) != DAT_00528a54)
            Parent(Left(y)) = x;
        Parent(y) = Parent(x);
        if (x == Root())
            Root() = y;
        else if (x == Left(Parent(x)))
            Left(Parent(x)) = y;
        else
            Right(Parent(x)) = y;
        Left(y) = x;
        Parent(x) = y;
    }

    void Rrotate(Node_004dc130* x)
    {
        std::_Lockit _Lk;
        Node_004dc130* y = Left(x);
        Left(x) = Right(y);
        if (Right(y) != DAT_00528a54)
            Parent(Right(y)) = x;
        Parent(y) = Parent(x);
        if (x == Root())
            Root() = y;
        else if (x == Right(Parent(x)))
            Right(Parent(x)) = y;
        else
            Left(Parent(x)) = y;
        Right(y) = x;
        Parent(x) = y;
    }

    static void Destval(unsigned int*) { }

    static void Freenode(Node_004dc130* p)
    {
        if (p != 0) {
            *(void**)p = DAT_00528a10;
            DAT_00528a10 = p;
        }
    }

    Class_004dd340 FUN_004dc130(Class_004dd340 _P);
};

// FUNCTION: 0x4dc130 ?FUN_004dc130@Class_004dc130@@QAE?AVClass_004dd340@@V2@@Z
Class_004dd340 Class_004dc130::FUN_004dc130(Class_004dd340 _P)
{
    Node_004dc130* _X;
    Node_004dc130* _Y = (_P++)._Mynode();
    Node_004dc130* _Z = _Y;
    std::_Lockit _Lk;
    if (Left(_Y) == DAT_00528a54)
        _X = Right(_Y);
    else if (Right(_Y) == DAT_00528a54)
        _X = Left(_Y);
    else
        _Y = Min(Right(_Y)), _X = Right(_Y);
    if (_Y != _Z) {
        Parent(Left(_Z)) = _Y;
        Left(_Y) = Left(_Z);
        if (_Y == Right(_Z))
            Parent(_X) = _Y;
        else {
            Parent(_X) = Parent(_Y);
            Left(Parent(_Y)) = _X;
            Right(_Y) = Right(_Z);
            Parent(Right(_Z)) = _Y;
        }
        if (Root() == _Z)
            Root() = _Y;
        else if (Left(Parent(_Z)) == _Z)
            Left(Parent(_Z)) = _Y;
        else
            Right(Parent(_Z)) = _Y;
        Parent(_Y) = Parent(_Z);
        std::swap(Color(_Y), Color(_Z));
        _Y = _Z;
    } else {
        Parent(_X) = Parent(_Y);
        if (Root() == _Z)
            Root() = _X;
        else if (Left(Parent(_Z)) == _Z)
            Left(Parent(_Z)) = _X;
        else
            Right(Parent(_Z)) = _X;
        if (Lmost() != _Z)
            ;
        else if (Right(_Z) == DAT_00528a54)
            Lmost() = Parent(_Z);
        else
            Lmost() = Min(_X);
        if (Rmost() != _Z)
            ;
        else if (Left(_Z) == DAT_00528a54)
            Rmost() = Parent(_Z);
        else
            Rmost() = Max(_X);
    }
    if (Color(_Y) == _Black) {
        while (_X != Root() && Color(_X) == _Black)
            if (_X == Left(Parent(_X))) {
                Node_004dc130* _W = Right(Parent(_X));
                if (Color(_W) == _Red) {
                    Color(_W) = _Black;
                    Color(Parent(_X)) = _Red;
                    Lrotate(Parent(_X));
                    _W = Right(Parent(_X));
                }
                if (Color(Left(_W)) == _Black && Color(Right(_W)) == _Black) {
                    Color(_W) = _Red;
                    _X = Parent(_X);
                } else {
                    if (Color(Right(_W)) == _Black) {
                        Color(Left(_W)) = _Black;
                        Color(_W) = _Red;
                        Rrotate(_W);
                        _W = Right(Parent(_X));
                    }
                    Color(_W) = Color(Parent(_X));
                    Color(Parent(_X)) = _Black;
                    Color(Right(_W)) = _Black;
                    Lrotate(Parent(_X));
                    break;
                }
            } else {
                Node_004dc130* _W = Left(Parent(_X));
                if (Color(_W) == _Red) {
                    Color(_W) = _Black;
                    Color(Parent(_X)) = _Red;
                    Rrotate(Parent(_X));
                    _W = Left(Parent(_X));
                }
                if (Color(Right(_W)) == _Black && Color(Left(_W)) == _Black) {
                    Color(_W) = _Red;
                    _X = Parent(_X);
                } else {
                    if (Color(Left(_W)) == _Black) {
                        Color(Right(_W)) = _Black;
                        Color(_W) = _Red;
                        Lrotate(_W);
                        _W = Left(Parent(_X));
                    }
                    Color(_W) = Color(Parent(_X));
                    Color(Parent(_X)) = _Black;
                    Color(Left(_W)) = _Black;
                    Rrotate(Parent(_X));
                    break;
                }
            }
        Color(_X) = _Black;
    }
    Destval(_Value(_Y));
    Freenode(_Y);
    --size;
    return (_P);
}
