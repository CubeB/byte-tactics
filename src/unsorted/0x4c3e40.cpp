// Decompiled by deepseek-v4.1-flash, finished by GPT-6, space-bunny-free,
// mimo-v2.6-pro and Space Bunny Free, finished by DeepSeek V4.1 Flash. Names
// are provisional.
// Partial: 70.6%, not MATCH, original 1115 bytes, ours 1133.
// DeepSeek V4.1 Flash pass: +0.3 (70.3 -> 70.6) from one spelling in the '='
// case. `if (0 == (eq != 0))` stops MSVC 5 from folding `!eq` into a direct
// `test esi,esi; je`, emitting `setne dl; test edx,edx; je` instead. The three
// extra instructions are the SAME total size as the folded form (both 1133),
// but the trailing `je 0x4c411b` now matches the original line verbatim, so the
// difflib ratio rises. It is a diff-alignment gain, not a step toward MATCH:
// the natural `if (!eq)` is 1126 bytes and 69.1%, and every other natural
// spelling of this test (`eq == 0`, `!(eq != 0)`, `(eq != 0) == 0`, an
// if/else, a named bool) is 68.6 to 69.1. The `0 == (eq != 0)` form is kept
// because it is the checker's best; replace it with `!eq` if the extra
// materialisation ever gets in the way of a real match. A 3-minute permuter
// run from this file and one from the previous 70.3 file both plateaued here,
// and the earlier 8-hour cleanup (build/scratch/4c3e40/p_best_min.cpp) reached
// the same 70.6 with the same single lever plus removable scaffolding.
// Space Bunny Free pass 2 (14 checks, no gain; everything below is re-derived
// against this file's own object code, not the Ghidra listing):
//  * HOW THE SCORE IS LOST, and why a shrink does not automatically help.
//    check.py scores with difflib.SequenceMatcher over the normalised
//    instruction text, and normalise() only masks hex values inside the
//    original image, so an INTERNAL branch target is compared verbatim. A
//    variant of the right shape at the wrong length therefore loses every
//    jump it shifts. 25 diff pairs of the current 70.3% differ ONLY in the
//    jump target: build/scratch/4c3e40/addr.py prints them with the drift
//    (ours - original) at each, and build/scratch/4c3e40/drift.py prints the
//    whole profile (it aligns the two listings by normalised text). The 25
//    pairs' drifts, in order, are +4 x6 (the six SkipSpace jumps), then +9,
//    -5, +3, +9, -2 x5, +9, -2 x2, +4, +9 x6. So the six SkipSpace pairs
//    (12 lines, worth about +4%) come back the moment anything in the
//    prologue plus setup block, that is 0x4c3e40 to 0x4c3eb5, loses 4 bytes.
//    Our prologue is 74 bytes to the original's 71 over the same span
//    (0x4c3e40 to the rep movsd), the setup block 49 to 47, so +5 there is
//    where the +4 comes from. The whole-file 18 bytes over is only two of
//    those. Two of the three groups cannot both be won: dropping those 4
//    bytes turns the last +9 group into +5 and gives the six SkipSpace pairs
//    back, a wash; the -2 group wants +2 between the '=' test and the strcmp
//    loop, and the only flexible length there is the 7 bytes the permuter's
//    `0 != ((int)(!eq))` spends, which lands the drift on +5, not 0.
//  * the prologue excess is 5 bytes and all of it is the register swap the
//    older notes describe: we spend `lea eax,[ebp+4]` + `lea ebx,[ebp+0x15]`
//    + `mov byte [eax],cl` + `mov byte [ebx],dl` + `mov [esp+0x38],eax`
//    where the original spends `lea ebx,[ebp+4]` + `mov byte [ebx],al` +
//    `mov [esp+0x3c],ebx`, and our entries zeroing is 9 bytes against the
//    original's 18 because ours goes through ebx ([ebx+4],[ebx+8],[ebx+0xc])
//    where the original goes through ebp ([ebp+0x19],[ebp+0x1d],[ebp+0x21]).
//    We also need two zero registers (xor ecx,ecx for the vector stores plus
//    xor eax,eax for the stosd) where the original reuses eax for both, which
//    is the other 2 bytes.
//  * what this session tried, all worse, all in build/scratch/4c3e40/:
//    the '=' test as `eq == 0`, `!eq` and `0 == eq` (all three identical,
//    1126 bytes, 69.1%: they drop the three extra instructions the permuter's
//    `0 != ((int)(!eq))` emits, but the seven bytes that saves move every
//    later jump off its target); `char* current` assigned after the
//    FUN_004d8610 call instead of in the declaration (1126, 69.4, and it
//    does reproduce the original's `mov edi,[...]; add esp,4; mov [ebp],eax;
//    mov [esp+0x10],edi` order); the lookup fully inlined in the body
//    (1108, 55.5); the search inline with the insert in a helper taking
//    `Class_004c3e40* self` (1108, 58.2); the search inline with a helper
//    taking `vector&` for equality plus insert (the older z8, still 58);
//    dropping the `kids` reference for plain `children.insert(children.end(),
//    1, child)` and making `kids` a pointer (both give byte-identical code to
//    the current file, 1133, 70.3, so the reference is not what keeps
//    &children alive); an `AddChild(Class_004c3e40*, child)` helper so
//    &children is never a loop-live value (1127, 69.1, and its 6 bytes are
//    all after the SkipSpace loop, so the +4 drift there is untouched);
//    the two tails written without a named end, `text <= current - 1` and
//    `FUN_004b6ba0(text, current - text - 2)` (1143, 54.2);
//    KeyEqual hoisted into a named bool, and the whole condition inverted to
//    `v.end() == lo || KeyEqual(...)` with the found branch as the else,
//    which is the branch sense the original's `test cl,cl; jne` has
//    (1126, 65.3, and still no `neg cl; sbb ecx,ecx; inc ecx`).
//  * BEST NEW SHAPE, kept here as a lead: build/scratch/4c3e40/y1.cpp writes
//    the '[' case as `current = SkipSpace(1 + close);` (one call on the
//    expression) instead of assigning `current = 1 + close;` and then
//    SkipSpace(current). That reproduces the original's
//    `mov al,[esi+1]; lea ecx,[esi+1]` and turns the whole '[' case into an
//    address-only diff: every instruction in it is already the right shape,
//    and only the jump targets are off. It is 1130 bytes and 69.9%, and it
//    has 29 aligned pairs instead of 25, but it loses more elsewhere, so it
//    does not replace the current file. In it the '[' case's own jumps sit at
//    drift -5 and want 5 more bytes between 0x4c3fc2 (drift +3) and
//    0x4c4068 (drift -5), which is the insert path, where ours is short by
//    exactly those 8 because of the ebx/ebp split described above. Fix that
//    split and those 7 pairs (14 lines) come back at once.
//  * tools/permute.py was started and died without a single candidate, so
//    this file is unpermuted by this session. `LowerBound`/`Lookup` taking the
//    key object by pointer instead of by reference (`key->ptr`) compiles to
//    byte-identical code (1133, 70.3), so the key's indirection level is not
//    what makes MSVC reload it inside the search loop.
//  * BUG, and the lead it gives: at 0x4c3fcb the original computes
//    `xor ecx,ecx; test eax,eax; sete cl; neg cl; sbb ecx,ecx; inc ecx;
//    test cl,cl; jne 0x4c3fe0`, so with eax the strcmp result it jumps to
//    the insert when the keys ARE equal and falls through to
//    `lea ecx,[edi+4]` (`dst = &lo->second`) when they are not. Ours spells
//    it the sane way round (`!KeyDifferent` -> use the found entry). If the
//    original really is inverted, the TDF parser inserts a duplicate entry
//    for every key that is already present and returns a neighbouring entry
//    for every key that is not, which is worth checking against the game's
//    behaviour before spending more time matching it byte for byte.
// Space Bunny Free pass. What moved the score up from 66.8%:
//  * the two '}' / end-of-file tails are written as if/else
//    ("if (text <= end) unknown_25 = FUN_004b6ba0(...); else unknown_25 = 0;")
//    so MSVC 5 keeps one FUN_004b6ba0 + return sequence per case instead of
//    merging the second case into the first (the shared "unknown_25 = 0" tail
//    at 0x4c4285 is still shared, as in the original), which restores the
//    22 bytes the merged version was missing;
//  * tools/permute.py (build/permute/0x4c3e40/best_ratio.cpp) supplied the
//    loop form "if (1) do { ... } while (1);" and the "if (x) { } else { ... }"
//    phrasings.  Its remaining scaffolding (the ret0/before temporaries, the
//    redundant casts and the "!= 0" comparisons) has been removed where it
//    was free, but a few of those rewrites are load bearing: build/scratch/
//    0x4c3e40/s4.cpp and s10.cpp take the rest out and fall back to 65%.
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

static inline bool KeyEqual(char* a, char* b) {
    bool equal = 0 == strcmp(a, b);
    return equal;
}
static inline bool KeyDifferent(char* a, char* b) { return !KeyEqual(a, b); }
template <class T> static inline T* LowerBound(T* first, T* last, const Class_004c91a0& key) {
    T* lo = first, * hi = last;
    if (lo != hi) do {
        T* mid = lo + (hi - ((T*)lo)) / 2;
        bool before = _strcmpi(mid->first.ptr, key.ptr) < 0;
        if (before)
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

// FUNCTION: 0x4c3e40
Class_004c3e40::Class_004c3e40(char* name, char* text, int* nextblock, char* filename) {
    Class_004c3e40* child;
    char* close;
    char* eq, * current = text, error[0x7d0] = "Parse error in .TDF File! ";

    std::vector<Class_004c3e40*>& kids = children;
    this->name = FUN_004d8610(name);

    if (1) do {
        current = SkipSpace(current);

        switch (*current) {
        case '[': {
            close = strchr(current, ']');
            if (!close)
                goto close_error;
            Class_004c91a0 subname, * sub = ((Class_004c4340*)this)->FUN_004c4340(&subname, 1 + current, ((char*)close));
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
            if (0 == (eq != 0)) {
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
        if (name != 0) { sprintf(strlen(error) + error, " - name = '%s' from file %s", name, filename); }
        FUN_004b6290(error);
        return;
    set_zero:
        unknown_25 = 0;
        return;
    } while (1);
}
