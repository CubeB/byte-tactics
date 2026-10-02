// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash; further tried by GPT-6.1-sol, edited by deepseek-v4.1, further tried by Space Bunny Free. Names are provisional.
//
// Space Bunny Free, issue 4579, second pass (started from 78.6 percent, 558 of
// 583 bytes). Still no MATCH. THIS PASS: 78.6 -> 80.4 percent (565 bytes), two
// changes, both in the code rather than in the layout. The four changes that
// took the previous pass from 70.2 to 78.6 are all still in place and are
// described at the end of this file.
//
// 1. THE INSERT TEST'S BOOL IS AN ASSIGNMENT INSIDE THE CONDITION, and that is
//    worth 0.9 points on its own:
//        if (!(atEnd || (keysEqual = (strcmp(e->key.ptr, key.ptr) == 0))))
//    It gives the exe-style `xor ecx,ecx; cmp eax,ebx; sete cl; test cl,cl;
//    jne <insert>` where the plain inline comparison gave a bare
//    `cmp eax,ebx; je <insert>`, and it is the only spelling found that keeps
//    BOTH the strcmp short-circuited behind `atEnd` and a materialised bool.
//    With the assignment in the condition the polarity no longer matters: the
//    same code comes out of `if (atEnd || (keysEqual = ...)) { insert } else
//    { assign }` (compared instruction for instruction: identical), so the
//    previous pass's rule about putting the assign in the then-branch is
//    obsolete now that the bool is materialised.
//    Measured on this base, all with the same body:
//      * `if (!(atEnd || (keysEqual = (strcmp(...) == 0))))`        80.4 (kept)
//      * the same with `(strcmp(...) == 0) != 0` in place             80.4
//      * `int insert = atEnd || (strcmp(...) == 0); if (!insert)`     79.5
//      * `bool doins = atEnd || (strcmp(...) == 0); if (!doins)`       79.5
//      * `bool k; k = (strcmp(...) == 0); if (!(atEnd || k))`         80.4 but
//        it hoists the strcmp above the atEnd test, losing the short circuit,
//        so the assignment has to stay INSIDE the condition,
//      * `int same = (strcmp(...) == 0); if (!(atEnd || same))`       76.4,
//        and `char` instead of `bool`                                 78.8,
//      * `atEnd || (keysEqual && atEnd == 0)`                        77.7 (the
//        `&&` restores the extra test), `(atEnd || (t = ..., t != 0))` 79.5.
//    What is still missing here is the original's `neg cl; sbb ecx,ecx;
//    inc cl`, the 0/1 int form of the same comparison that the exe computes
//    beside the byte bool and then never uses. No spelling found produces it:
//    an `int` local gets it (`xor ecx,ecx; cmp; sete cl; mov eax,ecx` in
//    ecx but hoisting the strcmp), and a DEAD `int t = keysEqual;` is folded
//    away by the copy propagation in every position (before the if, inside
//    the else, doubled, through a `(char)` round trip). Nor do the arithmetic
//    spellings bring it out: `atEnd + (keysEqual = ...)` 60.8, the same
//    compared with 0 60.8, `atEnd ? 0 : (int)(keysEqual = ...)` 62.2,
//    `(keysEqual = ...) ? 1 : 0` bound to an int 74.9, the operands swapped
//    75.5, while `(int)(keysEqual = ...)` and `... + 0` are free but do not
//    change a byte (80.4). The int form of the comparison wants a dead use
//    that MSVC 5's copy propagation cannot see, and nothing tried here
//    provides one.
//
// 2. THE DESTRUCTION BLOCK'S VECTOR POINTER IS PINNED BY A SELF-CONDITIONAL,
//    the loop bounds are read through it, and the free goes through a
//    `static inline void FreeVec(Vec_004c54f0*)` helper:
//        Vec_004c54f0* w = &s->v;
//        w = w ? w : w;               // 79.5 with this, 64.3 without it
//        Elem_004c5bc0* e = w->last;  // 78.6 with `s->v.last` instead
//        Elem_004c5bc0* p = w->first;
//    `w = w ? w : w;` emits no code at all; it exists only to give w a phi so
//    the allocator keeps a register of its own for the vector instead of
//    folding it onto the object base, and it is worth 15.2 points in this
//    shape. It must not be "tidied" away.
//
// WHAT THE BYTE DIFF SHOWS NOW: 148 of the original's 188 instructions are
// byte-identical, in 29 regions, and the file is 565 bytes against 583.
// Seventeen of those regions are pure jump-target shifts, because the file is
// 18 bytes shorter. The structural differences that are left:
//   A. the destruction block, the largest single one:
//      * the original tests the global with `test eax,eax; je` where ours
//        materialises a bool out of MapIsLoaded (`xor ecx,ecx; test eax,eax;
//        setne cl; test cl,cl; je`), because MapIsLoaded must return bool.
//        Re-measured on this base: `bool` 79.5 (kept), `int` 73.5, a pointer
//        73.5, `void*` 73.5, and taking the pointer as a parameter 79.5, so
//        the bool materialisation is the price of the 6 points MapIsLoaded is
//        worth everywhere else;
//      * `mov ebx, eax` (the object copy) sits before the two loads here and
//        after the `cmp esi, ebp` in the original, so the loads read [ebx+9]
//        and [ebx+5] where the original reads [eax+9] and [eax+5];
//      * the free is a `push edi; call FreeVec` where the original inlines
//        eight instructions. THIS IS THE PART THAT IS STILL A COMPILER WALL,
//        and it is now bounded from every side:
//          - MSVC 5 DOES inline a function containing `::operator delete`, one
//            and two levels deep (probed directly with tools/wcl and an /Fa
//            listing: a static inline free function, and a static inline free
//            function called from another static inline free function, both
//            inline into the caller),
//          - but it refuses the THIRD level: FreeVec called from an in-class
//            destructor that is itself inlined into FUN_004c54f0 stays a call,
//          - and every shape that DOES get the delete into FUN_004c54f0's own
//            body folds the vector back onto the object ([edi+5], [edi+9],
//            [edi+0xd]), drops to three callee-saved registers, hoists
//            `section` into ebx before _strcmpi (`push ebx; mov ebx,[esp+0x22c]`)
//            and shrinks the frame to 0x220. Measured 64.3 for: the free
//            written out in an in-class destructor, the whole destruction
//            written out in the destructor, the whole destruction written out
//            in FUN_004c54f0's body (with and without the pin, with the bounds
//            through s->v or through w, both zeroing orders), a static inline
//            DestroyMap(Class_004c5840*) called at the call site, the vector
//            obtained from a static inline VecOf(s) helper, from an in-class
//            `Vec()` method, from `((Vec*)((char*)s + 1))`, from
//            `((Vec*)(1 + (char*)s))`, and from `s->v` with the bounds read
//            through `s->v`. The only spelling that stops the fold AND keeps a
//            register for the vector is a real call (`FreeVec(w)`) or a load
//            through `(*(Vec**)((char*)s + 1))`, and the load scores 72.2
//            because it is not source any programmer would write.
//          So the original's shape (inline free, vector in edi, object in ebx,
//          four callee-saved registers) is not reachable from this file with
//          this compiler. It needs the free to be in the body AND the vector
//          pointer to survive MSVC 5's member-address folding, and those two
//          are mutually exclusive here.
//   B. the insert branch still merges into ONE shared FUN_004c93f0 call in the
//      original (`mov esi,eax; add esi,4` after 0x4c59d0, then `mov ecx,esi`)
//      where this file has two call sites. Re-measured with the new condition:
//      a `Class_004c93f0* target` set in both branches 78.9, an
//      `Elem_004c5bc0* slot` with `&slot->value` after the if 78.4, the same
//      with `(char*)slot + 4` 78.4, and a `receiver` set from the insert
//      expression itself 79.8, all below 80.4. The merged shape is
//      structurally closer (it does emit `lea esi,[edi+4]; jmp` and
//      `mov ecx,esi`) but it loses more elsewhere than it gains.
//   C. one register tie left: the original pushes the address of the empty
//      handle in eax (`lea ecx,[esp+0x28]; call 0x4c9180; lea edx,[esp+0x10];
//      push eax; push edx`) with no `lea` for it, so its own TU must have been
//      able to see that Class_004c9180's constructor returns `this` in eax
//      (0x4c9180 is `mov eax,ecx; ...; ret`, and the empty object is the
//      constructor's own argument, so eax is exactly `&empty` after it).
//      DO NOT read this as a bug: it is the missing `lea`, not a garbage
//      argument, and `eax` is not the strcmp result. MSVC 5 does not assume
//      the this-return for a declared-but-not-defined constructor (probed with
//      tools/wcl and an /Fa listing), so this file emits `lea ecx,[esp+0x28]`
//      a second time and `push ecx`. Seven spellings tried (swapping the two
//      arguments, `*(Class*)&empty`, a named reference, a named pointer,
//      pointer parameters, both pointers cast): all 79.5 or worse, none
//      changes the two instructions. It can only be fixed by defining the
//      constructor, which the rules forbid.
//
// Everything else that is not in the list above is a jump-target shift and
// costs nothing once the function is 583 bytes long.
//
// (An older measurement in this file said the constructor's flag byte wanted
// `mov cl` against the exe's `mov dl`. That is no longer true: the store
// order `v.first = 0; v.count = count; v.last = 0; v.end = 0;` gives the
// exe's exact `mov dl,[esp+0x17]; mov [eax+5],ebx; mov [eax+1],dl;
// mov [eax+9],ebx; mov [eax+0xd],ebx`, and the strcpy expansion matches too.)
#include <string.h>
#include <memory.h>
#include <stdio.h>

extern char DAT_0051fdc0[256];
extern char DAT_005119b8[];

void __cdecl FUN_004d83a0(int);
void __cdecl operator delete(void* p);

// A reference counted string handle. 0x4c91b0 builds one from a C string,
// 0x4c9180 makes an empty one, 0x4c93f0 assigns a C string and 0x4c9390 is
// the release.
class Class_004c9390 {
public:
    char* ptr;

    void FUN_004c9390();
};

class Class_004c93f0 {
public:
    char* ptr;

    Class_004c93f0* FUN_004c93f0(const char* text);
};

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 {
public:
    char* ptr;

    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

class Class_004c9180 {
public:
    char* ptr;

    Class_004c9180();
    ~Class_004c9180() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

// One entry of the global map: a key and a value, both string handles.
struct Elem_004c5bc0 {
    Class_004c91a0 key;                  // +0x0
    Class_004c91a0 value;                // +0x4

    ~Elem_004c5bc0();
};

class Class_004c54d0 : public Elem_004c5bc0 {
public:
    Class_004c54d0(const Class_004c91a0& a, const Class_004c91a0& b);
};

// The map itself, keyed by the string the handle points at.
#pragma pack(push, 1)
class Class_004c5c60 {
public:
    char unknown_0[5];
    Elem_004c5bc0* first;               // +0x5
    Elem_004c5bc0* last;                // +0x9

    Elem_004c5bc0* FUN_004c5c60(const char* key);
};

// The same vector seen from the insert helper, which is handed the object
// plus one, so its pointers sit four bytes lower.
class Vec_004c54f0 {
public:
    char count;                          // +0x0
    char pad[3];
    Elem_004c5bc0* first;                // +0x4
    Elem_004c5bc0* last;                 // +0x8
    Elem_004c5bc0* end;                  // +0xc

    Elem_004c5bc0* FUN_004c59d0(Elem_004c5bc0* pos, Elem_004c5bc0* val);
};



// Free the buffer and empty the vector, in the exe's order (first, last, end).
static inline void FreeVec(Vec_004c54f0* w)
{
    ::operator delete(w->first);
    w->first = 0;
    w->last = 0;
    w->end = 0;
}

class Class_004c5840 {
public:
    char unknown_0;                      // +0x0
    Vec_004c54f0 v;                       // +0x1 (_First at +0x5)

    Class_004c5840(char count);
    ~Class_004c5840()
    {
        Class_004c5840* s = this;
        Vec_004c54f0* w = &s->v;
        // Pinning w with a self-conditional emits no code; it only gives the
        // allocator a phi so the vector keeps a register of its own.
        w = w ? w : w;
        Elem_004c5bc0* e = w->last;
        Elem_004c5bc0* p = w->first;
        while (p != e) {
            p->~Elem_004c5bc0();
            p++;
        }
        FreeVec(w);
        ::operator delete(s);
    }
};
#pragma pack(pop)

extern Class_004c5840* DAT_0051fdb8;

Class_004c5840::Class_004c5840(char count)
{
    v.first = 0;
    v.count = count;
    v.last = 0;
    v.end = 0;
}

// A TDF section: its name and the entries under it.
class Class_004c4420 {
public:
    const char* name;                    // +0x0

    void FUN_004c4420(char* dest, size_t count);
};

class Class_004c48c0 {
public:
    char unknown_0[0x19];

    int FUN_004c48c0(char* dst, const char* key, size_t size, const char* def);
};

// The open TDF file.
class Class_004c2ea0 {
public:
    int root;                            // +0x0
    Class_004c4420* current;             // +0x4
    int file;                            // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    char unknown_0[4];
    int field_4;

    int FUN_004c2f60(char* filename);
};

class Class_004c3490 {
public:
    char unknown_0[4];
    int field_4;

    Class_004c4420* FUN_004c3490(int index);
};

class Class_004c3e10 {
public:
    char unknown_0[4];
    int field_4;

    void FUN_004c3e10();
};

class Class_004c3240 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    void FUN_004c3240();
};

// Building the new map and remembering the section name. Written as its own
// inline function because writing it out in the caller changes the register
// allocation and costs 2.2 percent (71.4 -> 69.2).
static inline void LoadMap(char flag, char* section)
{
    DAT_0051fdb8 = new Class_004c5840(flag);
    FUN_004d83a0((int)DAT_0051fdb8);
    strcpy(DAT_0051fdc0, section);
}

// Also load-bearing as a separate function, for the same reason as LoadMap.
static inline bool MapIsLoaded()
{
    return DAT_0051fdb8 != 0;
}

// FUNCTION: 0x4c54f0
void __stdcall FUN_004c54f0(char* filename, char* section)
{
    int atEnd;
    bool keysEqual;
    char flag;
    Class_004c5840* s;

    if (_strcmpi(section, DAT_0051fdc0) == 0)
        return;
    if (MapIsLoaded())
        DAT_0051fdb8->~Class_004c5840();
    LoadMap(flag, section);
    {
        Class_004c2ea0 f;
        char value[256];
        char name[256];
        if (((Class_004c2f60*)&f)->FUN_004c2f60(filename)) {
            int idx[1];
            idx[0] = 0;
            while (((Class_004c3490*)&f)->FUN_004c3490(idx[0])) {
                f.current->FUN_004c4420(name, 0xff);
                ((Class_004c48c0*)f.current)->FUN_004c48c0(value, DAT_0051fdc0, 0xff, DAT_005119b8);
                if (strlen(value) != 0) {
                    Class_004c91b0 key(name);
                    Elem_004c5bc0* e;
                    s = DAT_0051fdb8;
                    e = ((Class_004c5c60*)DAT_0051fdb8)->FUN_004c5c60(key.ptr);
                    atEnd = (Elem_004c5bc0*)e == ((Class_004c5c60*)s)->last;
                    // The assignment inside the condition is what materialises the
                    // bool the exe keeps in cl; see the note at the top of this file.
                    if (!(atEnd || (keysEqual = (strcmp(e->key.ptr, key.ptr) == 0)))) {
                        ((Class_004c93f0*)(&e->value))->FUN_004c93f0(value);
                    } else {
                        Class_004c9180 empty;
                        Class_004c54d0 tmp((Class_004c91a0&)key, (Class_004c91a0&)empty);
                        e = ((Vec_004c54f0*)(1 + (char*)s))->FUN_004c59d0((Elem_004c5bc0*)e, (Elem_004c5bc0*)&tmp);
                    }
                }
                ((Class_004c3e10*)&f)->FUN_004c3e10();
                idx[0]++;
            }
            ((Class_004c3240*)&f)->FUN_004c3240();
        }
    }
}
