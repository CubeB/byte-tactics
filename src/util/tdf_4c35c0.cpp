// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::logic_error's constructor from the MSVC 5 <stdexcept>:
//   explicit logic_error(const string& _S) : exception(""), _Str(_S) {}
#include <xstring>

namespace std {
typedef basic_string<char, char_traits<char>, allocator<char> > _Str_base;

// XSTRING's erase with the move count written as (_Len - _M) - _P0 through a
// named local, which is what the original binary's inlined copy does.
template<>
_Str_base& _Str_base::erase(_Str_base::size_type _P0, _Str_base::size_type _M)
{
    if (_Len < _P0)
        _Xran();
    _Split();
    if (_Len - _P0 < _M)
        _M = _Len - _P0;
    if (0 < _M)
        {size_type _T = _Len - _M;
        char_traits<char>::move(_Ptr + _P0, _Ptr + _P0 + _M,
            _T - _P0);
        size_type _N = _Len - _M;
        if (_Grow(_N))
            _Eos(_N); }
    return (*this);
}
}

#include <exception>

namespace std {
typedef basic_string<char, char_traits<char>, allocator<char> > string;

class logic_error : public exception {
public:
    // Defined out of line: in class it would always be inlined and never emitted.
    explicit logic_error(const string& _S);
    virtual ~logic_error();
    virtual const char* what() const;
protected:
    virtual void _Doraise() const;
private:
    string _Str;
};
}

// FUNCTION: 0x4c35c0 ??0logic_error@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
std::logic_error::logic_error(const string& _S)
    : exception(""), _Str(_S) {}
