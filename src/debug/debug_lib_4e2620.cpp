// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// MSVC 5 <xtree> _Tree::_Insert(_X, _Y, _V). The body below mirrors the
// original XTREE source line for line so the allocator sees the same live
// ranges.
#include <string.h>
#include <yvals.h>
#include "name_map_tree.h"

// Declared in the header's class; defined here so the tree insert below
// inlines it, as the original's memcpy copy does.
inline Value_004e2620& Value_004e2620::operator=(const Value_004e2620& v)
{
    if (this)
        memcpy(this, &v, sizeof(Value_004e2620));
    return *this;
}

// FUNCTION: 0x4e2620
Iter_004e2620 NameMapTree::Insert(Node_004e2620* _X, Node_004e2620* _Y, const Value_004e2620& _V)
{
    std::_Lockit _Lk;
    Node_004e2620* _Z = (Node_004e2620*)((NameMapAllocator*)this)->Allocate(0x208);
    _Parent(_Z) = _Y;
    _Color(_Z) = 0;
    _Left(_Z) = DAT_005292c4;
    _Right(_Z) = DAT_005292c4;
    _Z->value = _V;
    ++_Size;
    if (_Y == _Head || _X != DAT_005292c4 || compare(_V.key, _Y->value.key)) {
        _Left(_Y) = _Z;
        if (_Y == _Head) {
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
