// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// Partial, GPT-6 retry: 60.0%, not MATCH, original 1115 bytes, ours 1090.
// The allocator byte copies and 0x7fc frame are now present. The compiler
// caches the entries vector in ebx rather than the children vector, changing
// constructor stores, lookup registers, stack homes and error-path allocation.
// ctx.py names this Class_004c42a0::FUN_004c3e40, but the original initializes
// both vector members and returns this, consistent with a node constructor.
class Class_004c91a0;
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Keep insert out of line while reproducing the VC5 vector layout.
namespace std {

template <class T> class allocator {};

template <class T, class A = allocator<T> > class vector {
  public:
    typedef T* iterator;
    typedef unsigned int size_type;

    A _A;
    T* _First;
    T* _Last;
    T* _End;

    vector(const A& al = A()) : _A(al), _First(0), _Last(0), _End(0) {}
    iterator begin() { return _First; }
    iterator end() { return _Last; }
    T& operator[](size_type i) { return _First[i]; }
    void insert(iterator where, size_type n, const T& value);
    Class_004c91a0* Lookup(const Class_004c91a0& key);
};

}

class Class_004c3e40;

class Class_004c91a0 {
  public:
    char* ptr;

    Class_004c91a0() {}
    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c9180 {
  public:
    char* ptr;

    Class_004c9180();
};

class Class_004c54a0 {
  public:
    Class_004c91a0 first;
    Class_004c91a0 second;

    Class_004c54a0(const Class_004c54a0& other);
};

class Class_004c54d0 {
  public:
    Class_004c91a0 first;
    Class_004c91a0 second;

    Class_004c54d0(const Class_004c91a0& a, const Class_004c91a0& b);
};

class Class_004c4340 {
  public:
    Class_004c91a0* FUN_004c4340(Class_004c91a0* out, char* start, char* end);
};

class Class_004c9390 {
  public:
    char* data;

    void FUN_004c9390();
};

class Class_004c93b0 {
  public:
    char* ptr;

    Class_004c93b0* FUN_004c93b0(Class_004c91a0* param_1);
};

static inline bool KeyEqual(char* a, char* b) { return strcmp(a, b) == 0; }
static inline bool KeyDifferent(char* a, char* b) { return !KeyEqual(a, b); }
template <class T> static inline T* LowerBound(T* first, T* last, const Class_004c91a0& key) {
    T* lo = first;
    T* hi = last;
    while (lo != hi) {
        T* mid = lo + (hi - lo) / 2;
        bool less = _strcmpi(mid->first.ptr, key.ptr) < 0;
        if (less)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

template <class T, class A>
inline Class_004c91a0* std::vector<T, A>::Lookup(const Class_004c91a0& key) {
    T* lo = LowerBound(begin(), end(), key);
    Class_004c91a0* dst;
    if (lo != end() && !KeyDifferent(lo->first.ptr, key.ptr)) {
        dst = &lo->second;
    } else {
        Class_004c9180 empty;
        Class_004c54d0 pair(key, *(Class_004c91a0*)&empty);
        unsigned int idx = (unsigned int)(lo - begin());
        insert(lo, 1, *(T*)&pair);
        dst = &(*this)[idx].second;
        ((Class_004c9390*)((char*)&pair + 4))->FUN_004c9390();
        ((Class_004c9390*)&pair)->FUN_004c9390();
        ((Class_004c9390*)&empty)->FUN_004c9390();
    }
    return dst;
}

#pragma pack(push, 1)
class Class_004c3e40 {
  public:
    char* name;                            // +0x0
    std::vector<Class_004c3e40*> children; // +0x4
    char unknown_14;                       // +0x14
    std::vector<Class_004c54a0> entries;   // +0x15
    char* unknown_25;                      // +0x25

    Class_004c3e40(char* name, char* text, int* nextblock, char* filename);
};
#pragma pack(pop)

char* FUN_004d8610(char* text);
char* FUN_004b6ba0(char* text, int len);
void FUN_004b6290(char* text);

static inline void SkipSpaceInPlace(char*& p) {
    char c = *p;
    while (c && (c == ' ' || c == '\t' || c == '\r' || c == '\n')) {
        c = p[1];
        ++p;
    }
}

// FUNCTION: 0x4c3e40
Class_004c3e40::Class_004c3e40(char* name, char* text, int* nextblock, char* filename) {
    char error[0x7d0] = "Parse error in .TDF File! ";
    char* current = text;

    std::vector<Class_004c3e40*>& kids = children;
    this->name = FUN_004d8610(name);

    while (1) {
        SkipSpaceInPlace(current);

        switch (*current) {
        case '[': {
            char* close = strchr(current, ']');
            if (close == 0)
                goto close_error;
            Class_004c91a0 subname;
            ((Class_004c4340*)this)->FUN_004c4340(&subname, current + 1, close);
            current = close + 1;
            SkipSpaceInPlace(current);
            if (*current != '{') {
                strcat(error, "Sub-record - opening '{' not found");
                ((Class_004c9390*)&subname)->FUN_004c9390();
                goto report;
            }
            Class_004c3e40* child =
                new Class_004c3e40(subname.ptr, current + 1, (int*)&current, filename);
            kids.insert(kids.end(), 1, child);
            ((Class_004c9390*)&subname)->FUN_004c9390();
            continue;
        }
        case '}': {
            if (nextblock)
                *nextblock = (int)(current + 1);
            if (text > current - 1) {
                unknown_25 = 0;
                return;
            }
            unknown_25 = FUN_004b6ba0(text, (int)(current - 1) - (int)text - 1);
            return;
        }
        case 0: {
            if (nextblock == 0) {
                if (text > current - 1) {
                    unknown_25 = 0;
                    return;
                }
                unknown_25 = FUN_004b6ba0(text, (int)(current - 1) - (int)text - 1);
                return;
            }
            goto eof_error;
        }
        default: {
            char* eq = strchr(current, '=');
            if (eq == 0) {
                strcat(error, "Data field - '=' not found");
                goto report;
            }
            Class_004c91a0 key;
            ((Class_004c4340*)this)->FUN_004c4340(&key, current, eq);
            current = eq + 1;
            char* semi = strchr(current, ';');
            if (semi == 0) {
                strcat(error, "Data field - ';' not found");
                ((Class_004c9390*)&key)->FUN_004c9390();
                goto report;
            }
            Class_004c91a0 value;
            ((Class_004c4340*)this)->FUN_004c4340(&value, current, semi);
            current = semi + 1;

            Class_004c91a0* dst = entries.Lookup(key);
            ((Class_004c93b0*)dst)->FUN_004c93b0(&value);
            ((Class_004c9390*)&value)->FUN_004c9390();
            ((Class_004c9390*)&key)->FUN_004c9390();
            continue;
        }
        }

    close_error:
        strcat(error, "Sub-record - closing ']' not found");
        goto report;
    eof_error:
        strcat(error, "End of file - nextblock not zero");
    report:
        if (name)
            sprintf(error + strlen(error), " - name = '%s' from file %s", name, filename);
        FUN_004b6290(error);
        return;
    }
}
