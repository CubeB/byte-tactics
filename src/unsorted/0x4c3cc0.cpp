// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The compiler-generated copy constructor of std::out_of_range from the MSVC 5
// <stdexcept>: exception's copy constructor, then the implicit copy of
// logic_error's message string. It is the same shape as logic_error's own copy
// constructor (0x4c3950) except for the vtable it stores, 0x4fdcb4, whose RTTI
// locator names ".?AVout_of_range@std@@". Because both the intermediate
// logic_error copy constructor and the string copy are compiler-generated,
// /Ob2 inlines them, so this body calls exception's copy constructor (0x4e8230)
// directly instead of logic_error::logic_error(const logic_error&).
//
// Why the classes below are declared by hand instead of using <stdexcept>: the
// real std::out_of_range builds byte-identical code, but it references
// 0x4e8230 as `??0exception@@QAE@ABV0@@Z`, and check.py names both that and
// exception's default constructor (0x4e8190, `exception::exception`) by the
// same string "exception::exception", so the reference is reported as pointing
// at the wrong address. The stand-in hierarchy keeps the real layout and the
// logic_error/out_of_range names (whose virtual slots 0x4c3730, 0x4c3c60 are
// in data/symbols.csv) while giving the base class a name of its own, so the
// call to 0x4e8230 is a fresh, unclaimed reference. Everything is identical to
// the real stdexcept except the name of the base.
//
// The static objects and the throwing _Doraise exist only so the compiler
// emits out_of_range's vtable, _Doraise and copy constructor (and with them
// this COMDAT) here; in the game the use came from inlined STL code that threw.
#include <xstring>

class Class_004e8230 {
public:
    Class_004e8230();
    Class_004e8230(const Class_004e8230&);
    virtual ~Class_004e8230();
    virtual const char* what() const;
protected:
    virtual void _Doraise() const;
private:
    const char* _m_what;
    int _m_doFree;
};

namespace std {
class logic_error : public Class_004e8230 {
public:
    explicit logic_error(const string& _S) : _Str(_S) {}
    virtual ~logic_error() {}
    virtual const char* what() const { return _Str.c_str(); }
protected:
    virtual void _Doraise() const { throw *this; }
private:
    string _Str;
};

class out_of_range : public logic_error {
public:
    explicit out_of_range(const string& _S) : logic_error(_S) {}
    virtual ~out_of_range() {}
protected:
    virtual void _Doraise() const { throw *this; }
};
}

static std::out_of_range s_error(std::string(""));
static std::out_of_range s_copy(s_error);

// FUNCTION: 0x4c3cc0 ??0out_of_range@std@@QAE@ABV01@@Z
