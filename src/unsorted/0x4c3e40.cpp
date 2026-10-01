// Decompiled by deepseek-v4.1-flash, finished by GPT-6, space-bunny-free
// and mimo-v2.6-pro. Names are provisional.
// Partial: 66.8%, not MATCH, original 1115 bytes, ours 1093.
// mimo-v2.6-pro (second pass): tail merge is MSVC 5 merging identical block
// endings before scheduling (guide line 1374). Confirmed in a toy
// (build/scratch/0x4c3e40/toy*.cpp) that the second of two identical
// "if (text > end) goto set_zero; unknown_25 = FUN_004b6ba0(...); return;"
// blocks always jumps into the first block's body, so the original's two
// separate dec ecx copies need a pre-scheduling difference the scheduler
// erases. Tried and still merged: char* r = call; unknown_25 = r; an int len
// local in one case, endB - 1 - text in one case, char* endB = current;
// endB--. Also tried "current = SkipSpace(close + 1)" for the '[' case: it
// reproduces the original's mov al,[esi+1]; lea ecx,[esi+1] and removes our
// early current store (that region matches then), but the 3 byte size change
// shifts every later jump target and the score falls to 64.8%, so the merged
// file keeps "current = close + 1; current = SkipSpace(current);".
// mimo-v2.6-pro: phrasing both end cases with "current--" instead of
// "char* end = current - 1" gave the original dec ecx form and 66.2%.
// The two FUN_004b6ba0 call tails are still merged into one block (the
// original keeps one copy per case), and each merged copy stores current
// back to its frame slot after the dec, which the original never does.
// What is already right: the 0x7fc frame, the 27 byte string copy plus the
// 0x315 byte tail zeroing, all eight vector member stores and their offsets,
// the 0x4340 calls, the four inlined strcat error paths, the sprintf tail.
// Remaining hunks, by address:
//  0x4c3e46/0x4c3e4e: two loads of one uninitialised frame byte (offset 7),
//    the first before the pushes, the second after; ours loads both late.
//  0x4c3e53: the original caches &children in ebx for the whole loop, ours
//    precomputes both lea eax,[ebp+4] and lea ebx,[ebp+0x15] and keeps the
//    entries vector in ebx, so 0x4c3f4a, 0x4c3f56, 0x4c4014 and 0x4c4058
//    read the wrong base.
//  0x4c3e62: the original emits xor eax,eax and interleaves mov ecx,6 with
//    the two init blocks (second allocator byte before its three pointers);
//    ours emits xor ecx,ecx, then all six pointers, then the second byte.
//  0x4c3e96/0x4c3e9a: homes for this and &children sit at frame 0x14/0x28 in
//    the original and 0x1c/0x28 in ours, which shifts every later slot.
//  0x4c3ed5: the whitespace skip stores the new pointer one instruction
//    later than the original (scheduling only).
//  0x4c3f0f: argument registers for the first 0x4340 call (ecx vs edx).
//  0x4c3f5d: binary search uses lo=edi, hi=esi, mid=ebp and a cached key.ptr
//    in ebx, ours uses lo=esi, hi=ebp, mid=edi and reloads the key.
//  0x4c3fe0: original frame (esp+0x10 == S): key S+8, subname S+0xc,
//    value S+0x10, this-home S+0x14, child S+0x18, empty S+0x1c, pair
//    S+0x20, kids-home S+0x28, error S+0x2c; ours swaps the this-home and
//    empty slots (0x1c/0x14). From the /Fa listing: $T1358/$T1359 (the
//    allocator byte) sits at S+7 in both builds, only its load timing
//    differs.
//  0x4c4165 and 0x4c41de: the original shares one unknown_25 = 0 tail at
//    0x4c4285 reached by ja; ours emits lea eax,[ecx-1]; cmp edi,eax; jbe
//    inline and never builds the shared block. The blocks below are just
//    the register/size form of ours; the byte loss is the second dec ecx
//    copy (see the second-pass note at the top).
// Tried and rejected: inlining the lookup straight into the body (46.3%),
// moving char* current = text after the FUN_004d8610 call (58.6%).
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
static inline Class_004c91a0* Lookup(std::vector<T, A>& v, const Class_004c91a0& key) {
    T* lo = LowerBound(v.begin(), v.end(), key);
    Class_004c91a0* dst;
    if (lo != v.end() && !KeyDifferent(lo->first.ptr, key.ptr)) {
        dst = &lo->second;
    } else {
        Class_004c9180 empty;
        Class_004c54d0 pair(key, *(Class_004c91a0*)&empty);
        unsigned int idx = (unsigned int)(lo - v.begin());
        v.insert(lo, 1, *(T*)&pair);
        dst = &v[idx].second;
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

char* __cdecl FUN_004d8610(char* text);
char* FUN_004b6ba0(char* text, int len);
void FUN_004b6290(char* text);

static inline char* SkipSpace(char* p) {
    char c = *p;
    while (c && (c == ' ' || c == '\t' || c == '\r' || c == '\n')) {
        c = p[1];
        ++p;
    }
    return p;
}

// FUNCTION: 0x4c3e40
Class_004c3e40::Class_004c3e40(char* name, char* text, int* nextblock, char* filename) {
    char error[0x7d0] = "Parse error in .TDF File! ";
    char* current = text;

    std::vector<Class_004c3e40*>& kids = children;
    this->name = FUN_004d8610(name);

    while (1) {
        current = SkipSpace(current);

        switch (*current) {
        case '[': {
            char* close = strchr(current, ']');
            if (close == 0)
                goto close_error;
            Class_004c91a0 subname;
            ((Class_004c4340*)this)->FUN_004c4340(&subname, current + 1, close);
            current = close + 1;
            current = SkipSpace(current);
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
            char* endA = current - 1;
            if (text > endA)
                goto set_zero;
            unknown_25 = FUN_004b6ba0(text, endA - text - 1);
            return;
        }
        case 0: {
            if (nextblock)
                goto eof_error;
            char* endB = current - 1;
            if (text > endB)
                goto set_zero;
            unknown_25 = FUN_004b6ba0(text, endB - text - 1);
            return;
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

            Class_004c91a0* dst = Lookup(entries, key);
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
    set_zero:
        unknown_25 = 0;
        return;
    }
}
