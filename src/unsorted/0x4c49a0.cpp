// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The reference-counted std::basic_string<char> copy constructor:
// allocator copy, _Tidy(), then the inlined assign(_X, 0, npos).
#include <string>
#include <string.h>

class Class_004c49a0 {
public:
    typedef std::allocator<char> _A;
    typedef std::char_traits<char> _Tr;
    typedef _A::size_type size_type;

    enum { _FROZEN = 255 };
    static const size_type npos;

    explicit Class_004c49a0(const _A& _Al = _A())
        : allocator(_Al) {_Tidy(); }
    Class_004c49a0(const Class_004c49a0& _X);
    ~Class_004c49a0()
        {_Tidy(true); }

    Class_004c49a0& operator=(const Class_004c49a0& _X)
        {return (assign(_X)); }

    const char* c_str() const
        {return (_Ptr == 0 ? _Nullstr() : _Ptr); }
    size_type size() const
        {return (_Len); }
    size_type capacity() const
        {return (_Res); }

    Class_004c49a0& assign(const Class_004c49a0& _X)
        {return (assign(_X, 0, npos)); }
    Class_004c49a0& assign(const Class_004c49a0& _X, size_type _P, size_type _M)
        {if (_X.size() < _P)
            std::_Xran();
        size_type _N = _X.size() - _P;
        if (_M < _N)
            _N = _M;
        if (this == &_X)
            erase((size_type)(_P + _N)), erase(0, _P);
        else if (0 < _N && _N == _X.size()
            && _Refcnt(_X.c_str()) < _FROZEN - 1
            && allocator == _X.allocator)
            {_Tidy(true);
            _Ptr = (char *)_X.c_str();
            _Len = _X.size();
            _Res = _X.capacity();
            ++_Refcnt(_Ptr); }
        else if (_Grow(_N, true))
            {_Tr::copy(_Ptr, &_X.c_str()[_P], _N);
            _Eos(_N); }
        return (*this); }

    Class_004c49a0& erase(size_type _P0 = 0, size_type _M = npos)
        {if (_Len < _P0)
            std::_Xran();
        _Split();
        if (_Len - _P0 < _M)
            _M = _Len - _P0;
        if (0 < _M)
            {_Tr::move(_Ptr + _P0, _Ptr + _P0 + _M, _Len - _P0 - _M);
            size_type _N = _Len - _M;
            if (_Grow(_N))
                _Eos(_N); }
        return (*this); }

private:
    _A allocator;                      // +0x0
    char* _Ptr;                        // +0x4
    size_type _Len;                    // +0x8
    size_type _Res;                    // +0xc

    bool _Grow(size_type _N, bool _Trim = false);
    void _Eos(size_type _N);
    void _Split();

    unsigned char& _Refcnt(const char* _U)
        {return (((unsigned char *)_U)[-1]); }
    static const char* _Nullstr()
        {static const char _C = char(0);
        return (&_C); }
    void _Tidy(bool _Built = false)
        {if (!_Built || _Ptr == 0)
            ;
        else if (_Refcnt(_Ptr) == 0 || _Refcnt(_Ptr) == _FROZEN)
            allocator.deallocate(_Ptr - 1, _Res + 2);
        else
            --_Refcnt(_Ptr);
        _Ptr = 0, _Len = 0, _Res = 0; }
};

// FUNCTION: 0x4c49a0
Class_004c49a0::Class_004c49a0(const Class_004c49a0& _X)
    : allocator(_X.allocator) {_Tidy(), assign(_X, 0, npos); }
