// Decompiled by deepseek-v4.1-flash, finished by GPT-6, space-bunny-free,
// mimo-v2.6-pro and Space Bunny Free. Names are provisional.
// Partial: 70.3%, not MATCH, original 1115 bytes, ours 1133.
// Space Bunny Free pass. What moved the score up from 66.8%:
//  * the two '}' / end-of-file tails are written as if/else
//    ("if (text <= end) unknown_25 = FUN_004b6ba0(...); else unknown_25 = 0;")
//    so MSVC 5 keeps one FUN_004b6ba0 + return sequence per case instead of
//    merging the second case into the first (the shared "unknown_25 = 0" tail
//    at 0x4c4285 is still shared, as in the original), which restores the
//    22 bytes the merged version was missing;
//  * tools/permute.py (build/permute/0x4c3e40/best_ratio.cpp) supplied the
//    loop form "if (1) do { ... } while (1);", the "if (x) { } else { ... }"
//    phrasings and the temporaries around the two tail cases.
// Still different, by address (all verified against the bytes, not the
// Ghidra listing; the listing's own esp offsets are one push-count off in
// places, so the frame slots below come from the /Fa listing and from
// tracking esp by hand):
//  0x4c3e46: the original hoists the first load of the uninitialised
//    allocator byte to just after "sub esp, 0x7fc" (mov al, [esp+7]) and
//    uses al for children._A, so eax is the zero register (xor eax,eax) for
//    all six pointer stores; ours loads both bytes after the pushes, uses cl
//    for children._A and dl for entries._A, and zeroes ecx instead.
//    Tried: allocator with a real byte member, allocator passed by value,
//    _A assigned in the ctor body, explicit member-init lists, a base class
//    holding children: none changed the prologue.
//    STRONG LEAD: the hoist comes back as soon as the lookup stops taking
//    the vector by reference (build/scratch/0x4c3e40/y3.cpp and z5.cpp):
//    with LowerBound(entries._First, entries._Last, key.ptr) called from the
//    body and the insert written against entries, the prologue becomes
//    "sub esp, 0x7f4; mov al, [esp+7]; push ebx; push ebp; mov ebx, ecx;
//    mov cl, [esp+0xf]; xor ebp, ebp; ..." and children._A is stored from al,
//    exactly as in the original, and the whole function drops to 1106 bytes
//    (original 1115). What is then left is that MSVC puts this in ebx and
//    ebp becomes the zero register, so the frame is 0x7f4 instead of 0x7fc
//    (no this-home slot: mid goes to edi, not ebp) and every [esp+N] offset
//    shifts by 8. That single register choice is what keeps that shape at
//    55% while the version in this file scores 70%.
//    The mirror image (build/scratch/0x4c3e40/z8.cpp: the search inline in
//    the body, the equality test plus insert in a helper taking vector&)
//    gets this back into ebp, but then needs one frame slot too many
//    (0x800) and the byte load is not hoisted. Neither shape has both yet.
//  0x4c3e53: the original caches &children in ebx for the whole loop and
//    reaches the entries vector through this (ebp+0x19/0x1d/0x15), creating
//    &entries only late ("add ebp, 0x15" inside the insert branch), and it
//    hoists key.ptr into ebx before the binary search. Ours precomputes
//    lea eax,[ebp+4] and lea ebx,[ebp+0x15] and keeps &entries in ebx, so the
//    whole binary search, the insert and the strcmp use the wrong base and
//    the key is reloaded every iteration. Tried: Lookup taking
//    Class_004c3e40* instead of vector&, Lookup as an in-class member, the
//    lookup written inline with entries.begin()/end(), a Class_004c3e40*
//    self local, an AddChild helper taking the vector by reference, and
//    passing key.ptr to LowerBound: each either moves this into ebx or scores
//    worse (the vector& form is what keeps this in ebp).
//  0x4c3e96: this-home is at frame +0x24 and &children at +0x28 in the
//    original, +0x1c and +0x28 in ours.
//  0x4c3eab: the original loads text after the FUN_004d8610 call and stores
//    current = text there; ours loads both arguments before the call.
//  0x4c3f0f/0x4c407d: the original reloads current into eax before the first
//    0x4c4340 call of each case (ecx holds it); ours keeps it in ecx.
//  0x4c3f5d: binary search registers: original lo=edi, hi=esi, mid=ebp with
//    key.ptr cached in ebx; ours lo=esi, hi=ebp, mid=edi and reloads the key.
//  0x4c3fe0: the strcmp loop's second operand: original "mov bl, [esi]"
//    (bl free because ebx was &children), ours "cmp dl, [edi]".
//  0x4c4175/0x4c4264: the original ends both tails with "dec ecx; cmp edi,
//    ecx; ja; sub ecx, edi; dec ecx", ours with "lea eax, [ecx-1]; cmp edi,
//    eax; ja; sub eax, edi; dec eax". MSVC 5 only modifies the register
//    copy of current in place when a second expression keeps it live across
//    the compare (build/scratch/0x4c3e40/u10.cpp and u13.cpp produce "dec
//    ecx" but then compute the length in the wrong order or in eax), so the
//    two forms could not be combined yet.
// Also still open: "current = SkipSpace(close + 1)" in the '[' case
// reproduces the original's "mov al, [esi+1]; lea ecx, [esi+1]" but costs 3
// bytes elsewhere, and the register choice in the recursive constructor call.
// Tried and rejected: inlining the lookup straight into the body (46.3%),
// moving char* current = text after the FUN_004d8610 call (58.6%).
class Class_004c91a0;
#include <memory.h>
#include <windows.h>
#include <math.h>
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

static inline bool KeyEqual(char* a, char* b) { bool ret0 = 0 == strcmp(a, b);
return ret0; }
static inline bool KeyDifferent(char* a, char* b) { return !KeyEqual(a, b); }
template <class T> static inline T* LowerBound(T* first, T* last, const Class_004c91a0& key) {
    T* lo = first, * hi = last;
    T* same2 = lo;
    lo = same2;
    if (lo != hi) do {
        T* mid = lo + (hi - ((T*)lo)) / 2;
        bool tmp5 = _strcmpi(mid->first.ptr, key.ptr) < 0;
        if (tmp5)
            lo = ((T*)mid) + 1;
        else
            hi = mid;
    } while (hi != lo);
    return lo;
}

template <class T, class A>
static inline Class_004c91a0* Lookup(std::vector<T, A>& v, const Class_004c91a0& key) {
    unsigned int idx;
    T* lo = LowerBound(v.begin(), v.end(), key);
    Class_004c91a0* dst;
    if (v.end() != lo && !KeyDifferent(lo->first.ptr, key.ptr)) {
        dst = &lo->second;
    } else {
        Class_004c9180 empty;
        Class_004c54d0 pair(key, *(Class_004c91a0*)&empty);
        idx = (unsigned int)(lo - v.begin());
        v.insert(lo, 1, *(T*)&pair);
        dst = &v[idx].second;
        Class_004c91a0* same1;
        same1 = dst;
        dst = same1;
        ((Class_004c9390*)(4 + (char*)&pair))->FUN_004c9390();
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
    while (c && (c == ' ' || c == '\t' || c == '\r' || '\n' == c)) {
        c = p[1];
        p += 1;
    }
    return p;
}

static inline Class_004c4340* inl1(Class_004c3e40* self) { return (Class_004c4340*)self; }

// FUNCTION: 0x4c3e40
Class_004c3e40::Class_004c3e40(char* name, char* text, int* nextblock, char* filename) {
    Class_004c3e40* child;
    char* close;
    int tmp1;
    char* eq, * current = text, error[0x7d0] = "Parse error in .TDF File! ";

    std::vector<Class_004c3e40*>& kids = children;
    this->name = FUN_004d8610(name);

    if (1) do {
        current = SkipSpace(current);

        switch (*current) {
        case '[': {
            close = strchr(current, ']');
            tmp1 = !close;
            if (tmp1)
                goto close_error;
            Class_004c91a0 subname, * tmp2 = ((Class_004c4340*)this)->FUN_004c4340(&subname, 1 + current, ((char*)close));
            tmp2;
            current = 1 + close;
            current = SkipSpace(((char*)current));
            if ((*current) != '{') {
                strcat(error, "Sub-record - opening '{' not found");
                ((Class_004c9390*)&subname)->FUN_004c9390();
                goto report;
            }
            child = new Class_004c3e40(subname.ptr, ((char*)current) + 1, ((int*)&current), filename);
            kids.insert(kids.end(), 1, child);
            ((Class_004c9390*)&subname)->FUN_004c9390();
            continue;
        }
        case '}': {
            if ((int*)nextblock) *nextblock = (int)(1 + ((char*)current));
            char* endA = current - 1;
            if (text <= endA)
                unknown_25 = FUN_004b6ba0(text, endA - text - 1);
            else
                unknown_25 = 0;
            return;
        }
        case 0: {
            if (nextblock)
                goto eof_error;
            char* endB = current - 1;
            if (text <= endB)
                unknown_25 = FUN_004b6ba0(text, endB - text - 1);
            else
                unknown_25 = 0;
            return;
        }
        default: {
            eq = strchr(current, '=');
            if (0 != ((int)(!eq))) {
                strcat(error, "Data field - '=' not found");
                goto report;
            }
            Class_004c91a0 key;
            ((Class_004c4340*)this)->FUN_004c4340(&key, ((char*)current), eq);
            current = eq + 1;
            char* semi;
            semi = strchr(current, ';');
            if (semi != 0) {
            } else {
                strcat(error, "Data field - ';' not found");
                ((Class_004c9390*)&key)->FUN_004c9390();
                goto report;
            }
            Class_004c91a0 value;
            (inl1(this))->FUN_004c4340(&value, current, semi);
            current = semi + 1;

            Class_004c91a0* tmp4 = Lookup(entries, key), * dst = tmp4;
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
        if (name != 0) { sprintf(strlen(error) + error, " - name = '%s' from file %s", name, filename); }
        FUN_004b6290(error);
        return;
    set_zero:
        unknown_25 = 0;
        return;
    } while (1);
}
