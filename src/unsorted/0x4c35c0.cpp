// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::logic_error's constructor from the MSVC 5 <stdexcept>:
//   explicit logic_error(const string& _S) : exception(""), _Str(_S) {}
// The body is the empty-string exception base (the literal at 0x5119b8 passed
// by address through the stack temporary that exception(const char* const&)
// takes) plus the inlined copy construction of the message string _Str at
// +0xc: the empty allocator byte, _Tidy zeroing _Ptr/_Len/_Res, then
// assign(_X, 0, npos) with its self-assign, share and grow paths.
//
// <stdexcept> defines this constructor in class, and MSVC /Ob2 always inlines
// it, so the header alone never emits a COMDAT for it. Writing the class with
// the constructor defined out of line (a real definition) forces the
// emission, as in the original TU (its caller is 0x4c3490, where the extra
// inline level of GetChild keeps the constructor out of line).
//
// The inlined erase needed one source-level reproduction to match. The
// original computes the move count as (_Len - _M) - _P0: it keeps _Len in eax
// and does two subtractions in a row, while the test keeps a separate
// _Len - _P0 in ecx. XSTRING line 200 writes the count as _Len - _P0 - _M, and
// MSVC 5 reassociates that to reuse the test's _Len - _P0, which loses 4 bytes
// (350 against the original 354). Writing _Len - _M - _P0 alone does not help,
// the optimizer reassociates it right back; splitting the first difference
// into a named local (size_type _T = _Len - _M; ... _T - _P0) stops the
// reassociation and reproduces the original instruction for instruction. The
// explicit specialization below is that one change; the rest is the XSTRING
// text verbatim.
//
// This is a full byte match (354 bytes, 100.0%). check.py still reports one
// BAD reference, the call to exception::exception at 0x4e81d0, the
// const char* constructor. data/symbols.csv names only the default constructor
// 0x4e8190 "exception::exception", and base_name() collapses every overload to
// that one name, so the reloc to 0x4e81d0 is compared against 0x4e8190. The
// copy constructor 0x4e8230 already has an overload row in data/aliases.csv;
// this one needs the same:
//   exception::exception,0x4e81d0,overload: the const char* constructor
//     ??0exception@@QAE@ABQBD@Z (already named in data/functions.csv); the
//     default constructor is 0x4e8190 and the copy constructor is 0x4e8230
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
