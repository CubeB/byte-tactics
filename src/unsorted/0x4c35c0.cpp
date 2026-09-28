// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::logic_error's constructor from the MSVC 5 <stdexcept>:
//   explicit logic_error(const string& _S) : exception(""), _Str(_S) {}
// The base is built from the empty literal at 0x5119b8, and the string copy
// constructor is inlined at +0xc: the empty-allocator byte, _Tidy zeroing
// _Ptr/_Len/_Res, then assign(_X, 0, npos) with its self-assign, share and
// grow paths. <stdexcept> defines this constructor in-class, and MSVC /Ob2
// always inlines it, so the header alone never emits the COMDAT here. Writing
// the same class with the constructor defined out of line (as a real
// definition, not a __declspec hack) forces the compiler to emit it. The
// definition matches the header text exactly, so the constructor itself is
// not provisional; only its remaining 4-byte codegen difference is.
//
// Remaining difference (partial, 88.1%):
//   original 0x4c3620: mov ecx,eax / sub ecx,ebx / cmp ecx,esi / jae
//   ours:              sub eax,ebx / cmp eax,esi / jae
//   The original keeps _Len in eax and computes (_Len - _P0 - _M) as
//   (_Len - _M) - _P0 in the inlined basic_string::erase; MSVC 5 SP3 (and
//   RTM) always CSE the already-computed _Len - _P0 here (350 bytes vs 354).
//   No source-level wording of the header's erase expression changes it.
// Also, check.py flags the call to exception::exception(0x4e81d0) as BAD:
// data/symbols.csv only names the default constructor 0x4e8190
// "exception::exception", and base_name collapses both overloads to that one
// name, so every call to the char* constructor (0x4e81d0) mismatches. The
// same happens in the already-submitted 0x4c3950.cpp, 0x4c3af0.cpp and
// 0x4c3740.cpp, so it is a symbols.csv naming gap, not a code problem.
#include <exception>
#include <xstring>

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
