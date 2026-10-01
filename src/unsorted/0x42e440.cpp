// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash break-through (issue #3820): 84.4 -> 84.7 percent (3924 bytes, was 3918),
// 13 -> 10 hunks. The binary search no longer materialises a `middle` pointer local: it uses an
// `unsigned int half = (unsigned int)(last - first) / 2;` and writes the two updates as
// `first = first + half + 1;` / `last = first + half;`. That single change freed the register the
// old `middle` local was spilling and fixed the long-standing count-slot diff: the search count now
// sits at [esp+0x10] exactly like the original (shared there with the later w->sub spill), so the
// four [esp+0x10]/[esp+0x14] hunks are gone. What still differs: the five bitfield-arm hunks
// (push 0 / or destination rotation, pure scheduler placement), three 1-byte jump offsets (we are
// 1 byte long overall, 3924 vs 3923) and the 240-line inlined vector-insert region, where our
// insert keeps the entries base in edi while the original keeps it in ebx and reloads w->sub from
// [esp+0x10]. The `Damage_0042e440* sub = w->sub;` spelling for that region was already tried by an
// earlier pass and regresses to 82.8 percent, so the remaining gap looks allocator-bound.
// deepseek-v4.1-flash retry (issue #3775): re-confirmed 84.4% / 3918 bytes, 13 hunks.
// No source shape was changed: the five bitfield-arm hunks are `or` operand rotation plus
// `push 0` placement (the original picks the value register as the OR destination in the
// bit-1 and bit-8 arms but the loaded-dword register in the bit-13/19/24 arms, so the choice
// follows which side the scheduler computes first, not the assignment spelling), the three
// [esp+0x10] vs [esp+0x14] hunks are the model-search count slot (shared with the w->sub
// spill in the original), the four jump-offset hunks are the 5-byte shortfall, and the
// 160-line region is the inlined three-arm vector insert.
// deepseek-v4.1-flash retry (issue #3855): re-confirmed 84.7% / 3924 bytes, 10 hunks.
// The five bitfield-arm hunks are not one tie: the original stores after `push 0` for
// tracks/bit13 and stockpile/bit28, after both pushes for dropped/bit8, and already
// matches us for bit19 (only the `and eax,1` / mask order differs there), so no single
// source spelling can align all five arms. Remaining work: the 1-byte overage (all three
// late je offsets read +1) and the 240-line inlined vector::insert region.
// Retry by GPT-6.1-sol: best remains 84.3% (3918/3923), not MATCH. Reordering
// model-loop locals and hoisting path to model scope both compiled identically.
// Partial, deepseek-v4.1 retry: 84.3%, not MATCH. Original 3923 bytes, ours 3918.
// deepseek-v4.1-flash retry (issue #3951): best unchanged at 84.7 percent / 3924 bytes. Respelling
// the lower_bound `half` as the explicit signed byte-difference divide
// (`int half = (int)((char*)last - (char*)first) / 8 / 2;`) keeps 84.7 percent but grows us to
// 3937 bytes, so no divide spelling recovers the original sar-3-then-signed-/2 sequence. Still
// open: the 1-byte overage (all four late branch offsets read +1), the five bitfield `push 0`
// placement arms, and the inlined vector-insert region where our entries base lands in edi (with
// the `inc edi` off w->sub) instead of the original eax with [eax+5]/[eax+9] access.
// Fixed earlier: the model/explosion/sound/damage arms now match the original's
// layout (the big arm inline, the small arm out of line), the lava check is the
// double deref *(int*)(*(int*)(g_game+0x391e9)+0xd44), the model search uses a count
// local for the weapon index, and the search is a `for` loop (a do/while made MSVC
// peel the first iteration).
// Fixed this round: soundstart/soundhit/soundwater are `unsigned short`, so the 0xffff
// default is the tracked value 0xffff and MSVC hoists ONE `mov esi,0xffff` before the
// first test (a signed short folds it to `or esi,-1` rematerialised at every site).
// deepseek-v4.1-flash timebox retry: a named `float mba` for the minbarrelangle
// expression (then `w->minbarrelangle = mba;`) forces the original's fstp sink into the
// next FUN_004c46c0("firestarter",0) argument setup, 84.3 -> 84.4 (first hunk gone);
// the mba store folds away, only the schedule changes.
// deepseek-v4.1-flash timebox retry (issue #3908): re-confirmed 84.7% / 3924 bytes,
// 10 hunks, no source change kept. New observation on the +1 byte: all three late
// jump-displacement hunks point into the 0x42f2f8-0x42f340 range, so the single extra
// byte sits before that range, and the one place in the diff where our code is one
// byte bigger than the original is the end of the inlined insert, where the original
// folds the value offset into the index lea (lea esi,[ebx+ecx*8+4] then mov [esi],edx)
// while we form a plain element pointer and store through it (lea esi,[edi+ecx*8] then
// mov [esi+4],edx). The lower_bound re-entry is byte-neutral in total: our `inc edi`
// plus [edi+4]/[edi+8] reads are numerically the original's [eax+5]/[eax+9] off
// w->sub, and the unsigned `shr ebx,1` is 3 bytes cheaper than the original's signed
// cdq/sub/sar divide, which the loop-back copy (mov edx,[esp+0x14];
// mov [esp+0x10],edx) gives back.
// Still differs: the five bitfield-arm hunks (push 0 / or destination rotation), the
// +1 by reading above, and the 240-line inlined vector-insert region.
// Earlier note kept for reference (the count slot it mentions has since moved to
// [esp+0x10], see the #3820 block above): (1) the search count byte lived at [esp+0x14]
// where the original has [esp+0x10]; (2) four bitfield assignments schedule `push 0`,
// the `or` and the store one slot differently; (3) the fstp of minbarrelangle is sunk
// into the next call's argument pushes in the original, not here; (4) the inlined
// vector insert swaps esi/edi (ours first=edi last=esi, original first=esi last=ebx,
// vector base cached on the stack at [esp+0x10]) and its inlined _strcmpi uses the
// `setl cl` shape where the original builds the byte through sbb/sete; a FindEntry
// helper and direct w->sub->entries access both scored worse (80.7%, 80.4%), so this
// looks like an allocator tie in the original's exact <vector> instantiation.
// Allocator construction stays out of line, vector destruction calls
// FUN_00432c20, and shifting/filling calls FUN_00432cb0/FUN_00432c80.
// deepseek-v4.1-flash retry: swapping the declaration order of the search count and
// index, and splitting `count = w->id;` from its declaration, are both byte-identical
// (3918 bytes, 84.3%), so the count slot and the push-0/or/store scheduling of the
// five bitfield arms are allocator-bound stays, not source shape.
// deepseek-v4.1-flash timebox retry (issue #3547): declaring the model block's
// path/h before i/count is byte-identical (3918 bytes, 84.4%), so the count
// byte's [esp+0x14] slot and the push-0/or/store order are allocator-bound.
// deepseek-v4.1-flash retry (issue #3577): hoisting `char model[0x100]` to the top
// of the function, and hoisting the `unsigned char i`/`count` pair out of the model
// `if` block, both compile byte-identically again (3918 bytes, 84.4%, 13 diff hunks,
// the count byte still lands at [esp+0x14]), so the search-local slot truly tracks
// the later vector-insert allocation, which is still the large hunk.
// deepseek-v4.1-flash retry (issue #3706): declaring every `unsigned int x : 1`
// flag as signed `int x : 1` is byte-identical at 84.4% (3918 bytes), and
// replacing the `std::vector<Entry>& entries = w->sub->entries;` reference with a
// `Damage_0042e440* sub = w->sub;` pointer local used as sub->entries regresses
// to 82.8% (3917 bytes), so the vector-insert hunk's ebx/esi/edi split and the
// stack slot of the search count stay allocator-bound.
// deepseek-v4.1-flash timebox retry (issue #3746): still 84.4% / 3918 bytes, 13 hunks.
// Byte-identical experiments this pass: declaring `last` before `first`, and rewriting
// the found-test as !(first != entries.end() && strcmp(...) == 0). Both compile to the
// same 3918 bytes, so the two-pointer register split (ours first=edi/end=esi, original
// first=esi/end=ebx) and the materialised sbb/sete byte on the second strcmp are not
// reachable from the condition spelling. `entries.insert(first, T(...))` (the
// single-element overload) does not exist in this <vector>, so the count-1 fill insert
// stays. Remaining diff: 4 hunks are the search count byte slot ([esp+0x14] here,
// [esp+0x10] original, shared there with the vector-base spill) and 8 hunks are pure
// scheduler placement of `push 0` / the [ebp+0x111] bitfield store and which register
// (eax vs ecx/edx) accumulates the read-modify-write; the 160-line vector insert
// region is the allocator-bound body of this gap.
// construct does not use an allocator receiver in the original, so its
// declaration uses the equivalent two-argument stdcall ABI.

#include <string.h>
#include <vector>

class Class_004c4440 {
  public:
    char* FUN_004c4440();
};

class Class_004c46c0 {
  public:
    int FUN_004c46c0(const char* key, int def);
};

class Class_004c4760 {
  public:
    double FUN_004c4760(const char* key, double def);
};

class Class_004c48c0 {
  public:
    int FUN_004c48c0(char* dst, const char* key, int size, char* def);
};

class Class_004c4470 {
  public:
    void* FUN_004c4470(const char* key);
};

class Class_004c45e0 {
  public:
    char* FUN_004c45e0(int index);
};

class Class_004c9390 {
  public:
    void FUN_004c9390();
};
class Class_004c91a0 {
  public:
    char* ptr;
    Class_004c91a0(const Class_004c91a0&);
    ~Class_004c91a0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};
class Class_004c91b0 {
  public:
    char* ptr;
    Class_004c91b0(const char*);
    ~Class_004c91b0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};
class Class_004c93b0 {
  public:
    void* FUN_00432d20(int*);
};
struct Entry_00432cf0 {
    Class_004c91a0 name;
    int value;
    Entry_00432cf0(const Class_004c91a0& n, int v) : name(n), value(v) {}
    Entry_00432cf0& operator=(const Entry_00432cf0& v) {
        ((Class_004c93b0*)this)->FUN_00432d20((int*)&v);
        return *this;
    }
};
class Class_00432c20 {
  public:
    void* FUN_00432c20(unsigned char);
};
// Model the VC5 allocator and vector operations used in this caller.
namespace std {
template <> class allocator<Entry_00432cf0> {
  public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef Entry_00432cf0* pointer;
    typedef const Entry_00432cf0* const_pointer;
    typedef Entry_00432cf0& reference;
    typedef const Entry_00432cf0& const_reference;
    typedef Entry_00432cf0 value_type;
    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }
    pointer allocate(size_type n, const void*) {
        int count = (int)n;
        if (count < 0)
            count = 0;
        return (pointer)::operator new(count * sizeof(value_type));
    }
    char* _Charalloc(size_type n) { return (char*)::operator new(n); }
    void deallocate(void* p, size_type) { ::operator delete(p); }
    static void __stdcall construct(pointer p, const value_type& x);
    void destroy(pointer p) { ((Class_00432c20*)p)->FUN_00432c20(0); }
    size_type max_size() const {
        size_type n = (size_type)-1 / sizeof(value_type);
        return n > 0 ? n : 1;
    }
};
}

Entry_00432cf0* __stdcall FUN_00432cb0(Entry_00432cf0*, Entry_00432cf0*, Entry_00432cf0*);
void __stdcall FUN_00432c80(Entry_00432cf0*, Entry_00432cf0*, Entry_00432cf0*);
static inline void FillEntries(Entry_00432cf0* first, Entry_00432cf0* last,
                               const Entry_00432cf0& value) {
    FUN_00432c80(first, last, (Entry_00432cf0*)&value);
}
namespace std {
template <> class vector<Entry_00432cf0, allocator<Entry_00432cf0> > {
  public:
    typedef Entry_00432cf0* iterator;
    typedef const Entry_00432cf0* const_iterator;
    typedef unsigned int size_type;
    typedef std::allocator<Entry_00432cf0> Alloc;
    vector(const Alloc& a = Alloc()) : allocator(a), _First(0), _Last(0), _End(0) {}
    iterator begin() { return _First; }
    iterator end() { return _Last; }
    unsigned int size() { return _First == 0 ? 0 : _Last - _First; }
    iterator _Ucopy(const_iterator first, const_iterator last, iterator dest);
    iterator CopyEntries(const_iterator first, const_iterator last, iterator dest) {
        for (; first != last; ++first, ++dest)
            allocator.construct(dest, *first);
        return dest;
    }
    void _Ufill(iterator first, size_type n, const Entry_00432cf0& value) {
        for (; n > 0; --n, ++first)
            allocator.construct(first, value);
    }
    void _Destroy(iterator first, iterator last) {
        for (; first != last; ++first)
            allocator.destroy(first);
    }
    void insert(iterator _P, size_type _M, const Entry_00432cf0& _X) {
        if (_End - _Last < _M) {
            size_type _N = size() + (_M < size() ? size() : _M);
            iterator _S = allocator.allocate(_N, (void*)0);
            iterator _Q = CopyEntries(_First, _P, _S);
            _Ufill(_Q, _M, _X);
            CopyEntries(_P, _Last, _Q + _M);
            _Destroy(_First, _Last);
            allocator.deallocate(_First, _End - _First);
            _End = _S + _N;
            _Last = _S + size() + _M;
            _First = _S;
        } else if (_Last - _P < _M) {
            CopyEntries(_P, _Last, _P + _M);
            _Ufill(_Last, _M - (_Last - _P), _X);
            FillEntries(_P, _Last, _X);
            _Last += _M;
        } else if (0 < _M) {
            CopyEntries(_Last - _M, _Last, _Last);
            FUN_00432cb0(_P, _Last - _M, _Last);
            FillEntries(_P, _P + _M, _X);
            _Last += _M;
        }
    }

  private:
    Alloc allocator;
    iterator _First, _Last, _End;
};
}

#pragma pack(push, 1)
struct Damage_0042e440 {
    char pad;
    std::vector<Entry_00432cf0> entries;
};
#pragma pack(pop)
void __stdcall FUN_0049e010(void*);

#pragma pack(push, 1)
struct Weapon_0042e440 {
    char name[0x20];               // +0x000
    char name2[0x40];              // +0x020
    char pad_60[4];                // +0x060
    Damage_0042e440* sub;          // +0x064
    int weaponvelocity;            // +0x068
    int startvelocity;             // +0x06c
    int weaponacceleration;        // +0x070
    void* text;                    // +0x074
    void* anim1;                   // +0x078
    void* anim2;                   // +0x07c
    char model[0x40];              // +0x080
    float energypershot;           // +0x0c0
    float metalpershot;            // +0x0c4
    float minbarrelangle;          // +0x0c8
    int shakemagnitude;            // +0x0cc
    int shakeduration;             // +0x0d0
    short damage;                  // +0x0d4
    short areaofeffect;            // +0x0d6
    float edgeeffectiveness;       // +0x0d8
    int range;                     // +0x0dc
    int coverage;                  // +0x0e0
    short reloadtime;              // +0x0e4
    short weapontimer;             // +0x0e6
    short turnrate;                // +0x0e8
    short burst;                   // +0x0ea
    short burstrate;               // +0x0ec
    short sprayangle;              // +0x0ee
    short duration;                // +0x0f0
    short randomdecay;             // +0x0f2
    unsigned short soundstart;     // +0x0f4
    unsigned short soundhit;       // +0x0f6
    unsigned short soundwater;     // +0x0f8
    short smokedelay;              // +0x0fa
    short flighttime;              // +0x0fc
    short holdtime;                // +0x0fe
    char pad_100[4];               // +0x100
    short accuracy;                // +0x104
    short tolerance;               // +0x106
    short pitchtolerance;          // +0x108
    unsigned char id;              // +0x10a
    unsigned char firestarter;     // +0x10b
    unsigned char rendertype;      // +0x10c
    unsigned char color;           // +0x10d
    unsigned char color2;          // +0x10e
    unsigned char pad_10f[2];      // +0x10f
    unsigned int lineofsight : 1;  // +0x111 bit 0
    unsigned int ballistic : 1;    // bit 1
    unsigned int shellweapon : 1;  // bit 2
    unsigned int beamweapon : 1;   // bit 3
    unsigned int vlaunch : 1;      // bit 4
    unsigned int meteor : 1;       // bit 5
    unsigned int noradar : 1;      // bit 6
    unsigned int paralyzer : 1;    // bit 7
    unsigned int dropped : 1;      // bit 8
    unsigned int startsmoke : 1;   // bit 9
    unsigned int endsmoke : 1;     // bit 10
    unsigned int soundtrigger : 1; // bit 11
    unsigned int guidance : 1;     // bit 12
    unsigned int tracks : 1;       // bit 13
    unsigned int unitsonly : 1;    // bit 14
    unsigned int groundbounce : 1; // bit 15
    unsigned int waterweapon : 1;  // bit 16
    unsigned int toairweapon : 1;  // bit 17
    unsigned int smoketrail : 1;   // bit 18
    unsigned int turret : 1;       // bit 19
    unsigned int selfprop : 1;     // bit 20
    unsigned int propeller : 1;    // bit 21
    unsigned int noexplode : 1;    // bit 22
    unsigned int burnblow : 1;     // bit 23
    unsigned int twophase : 1;     // bit 24
    unsigned int cruise : 1;       // bit 25
    unsigned int commandfire : 1;  // bit 26
    unsigned int noautorange : 1;  // bit 27
    unsigned int stockpile : 1;    // bit 28
    unsigned int targetable : 1;   // bit 29
    unsigned int interceptor : 1;  // bit 30
    unsigned int : 1;              // bit 31
};

struct Game_0042e440 {
    char unknown_0[0x2cf3];
    Weapon_0042e440 weapons[0x100]; // +0x2cf3, stride 0x115
};
#pragma pack(pop)

extern Game_0042e440* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_004b6290(char* path);
void* __stdcall FUN_004cb560(char* path);
void __stdcall FUN_004cb590(void* p);
void __stdcall FUN_0042a140(void* a, char* b);
void* __stdcall FUN_00429700(char* name);
void* __stdcall FUN_004b8d40(void* a, char* b);
int __stdcall FUN_00429470(void* a, char* b);

// FUNCTION: 0x42e440
void __stdcall FUN_0042e440(Class_004c4440* parser) {
    char* id = parser->FUN_004c4440();
    Weapon_0042e440* w = &g_game->weapons[((Class_004c46c0*)parser)->FUN_004c46c0("ID", -1)];
    strcpy(w->name, id);
    ((Class_004c48c0*)parser)->FUN_004c48c0(w->name2, "name", 0x40, DAT_005119b8);

    w->weaponvelocity =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("weaponvelocity", 0.0) * 2184.5333333333333);
    w->startvelocity =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("startvelocity", 0.0) * 2184.5333333333333);
    w->weaponacceleration =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("weaponacceleration", 0.0) *
              72.81777777777778);
    w->range = ((Class_004c46c0*)parser)->FUN_004c46c0("range", 0x7fff);
    w->coverage = ((Class_004c46c0*)parser)->FUN_004c46c0("coverage", 0);
    w->reloadtime = (short)(((Class_004c4760*)parser)->FUN_004c4760("reloadtime", 0.0) * 30.0);
    w->energypershot = (float)((Class_004c4760*)parser)->FUN_004c4760("energypershot", 0.0);
    w->metalpershot = (float)((Class_004c4760*)parser)->FUN_004c4760("metalpershot", 0.0);
    w->areaofeffect = (short)((Class_004c46c0*)parser)->FUN_004c46c0("areaofeffect", 0);
    w->edgeeffectiveness = (float)((Class_004c4760*)parser)->FUN_004c4760("edgeeffectiveness", 0.0);
    w->weapontimer = (short)(((Class_004c4760*)parser)->FUN_004c4760("weapontimer", 0.0) * 30.0);
    w->noautorange = ((Class_004c46c0*)parser)->FUN_004c46c0("noautorange", 0);
    w->turnrate =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("turnrate", 0.0) * 0.03333333333333333);
    w->burst = (short)((Class_004c46c0*)parser)->FUN_004c46c0("burst", 0);
    w->burstrate = (short)(((Class_004c4760*)parser)->FUN_004c4760("burstrate", 0.0) * 30.0);
    w->sprayangle = (short)((Class_004c46c0*)parser)->FUN_004c46c0("sprayangle", 0);
    w->duration = (short)(((Class_004c4760*)parser)->FUN_004c4760("duration", 0.0) * 30.0);
    w->randomdecay = (short)(((Class_004c4760*)parser)->FUN_004c4760("randomdecay", 0.0) * 30.0);
    w->smokedelay = (short)(((Class_004c4760*)parser)->FUN_004c4760("smokedelay", 0.0) * 30.0);
    w->flighttime = (short)(((Class_004c4760*)parser)->FUN_004c4760("flighttime", 0.0) * 30.0);
    w->holdtime = (short)(((Class_004c4760*)parser)->FUN_004c4760("holdtime", 0.0) * 30.0);
    float mba = (float)(((Class_004c4760*)parser)->FUN_004c4760("minbarrelangle", -11.25) *
                        0.017453292519943278);
    w->minbarrelangle = mba;
    w->firestarter = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("firestarter", 0);
    w->rendertype = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("rendertype", 0);
    w->color = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("color", 0);
    w->color2 = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("color2", 0);
    w->soundtrigger = ((Class_004c46c0*)parser)->FUN_004c46c0("soundtrigger", 0);
    w->guidance = ((Class_004c46c0*)parser)->FUN_004c46c0("guidance", 0);
    w->tracks = ((Class_004c46c0*)parser)->FUN_004c46c0("tracks", 0);
    w->lineofsight = ((Class_004c46c0*)parser)->FUN_004c46c0("lineofsight", 0);
    w->ballistic = ((Class_004c46c0*)parser)->FUN_004c46c0("ballistic", 0);
    w->unitsonly = ((Class_004c46c0*)parser)->FUN_004c46c0("unitsonly", 0);
    w->groundbounce = ((Class_004c46c0*)parser)->FUN_004c46c0("groundbounce", 0);
    w->waterweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("waterweapon", 0);
    w->toairweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("toairweapon", 0);
    w->smoketrail = ((Class_004c46c0*)parser)->FUN_004c46c0("smoketrail", 0);
    w->turret = ((Class_004c46c0*)parser)->FUN_004c46c0("turret", 0);
    w->selfprop = ((Class_004c46c0*)parser)->FUN_004c46c0("selfprop", 0);
    w->propeller = ((Class_004c46c0*)parser)->FUN_004c46c0("propeller", 0);
    w->noexplode = ((Class_004c46c0*)parser)->FUN_004c46c0("noexplode", 0);
    w->burnblow = ((Class_004c46c0*)parser)->FUN_004c46c0("burnblow", 0);
    w->twophase = ((Class_004c46c0*)parser)->FUN_004c46c0("twophase", 0);
    w->cruise = ((Class_004c46c0*)parser)->FUN_004c46c0("cruise", 0);
    w->commandfire = ((Class_004c46c0*)parser)->FUN_004c46c0("commandfire", 0);
    w->stockpile = ((Class_004c46c0*)parser)->FUN_004c46c0("stockpile", 0);
    w->targetable = ((Class_004c46c0*)parser)->FUN_004c46c0("targetable", 0);
    w->interceptor = ((Class_004c46c0*)parser)->FUN_004c46c0("interceptor", 0);
    w->beamweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("beamweapon", 0);
    w->shellweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("shellweapon", 0);
    w->dropped = ((Class_004c46c0*)parser)->FUN_004c46c0("dropped", 0);
    w->vlaunch = ((Class_004c46c0*)parser)->FUN_004c46c0("vlaunch", 0);
    w->meteor = ((Class_004c46c0*)parser)->FUN_004c46c0("meteor", 0);
    w->noradar = ((Class_004c46c0*)parser)->FUN_004c46c0("noradar", 0);
    w->paralyzer = ((Class_004c46c0*)parser)->FUN_004c46c0("paralyzer", 0);
    w->startsmoke = ((Class_004c46c0*)parser)->FUN_004c46c0("startsmoke", 0);
    w->endsmoke = ((Class_004c46c0*)parser)->FUN_004c46c0("endsmoke", 0);
    w->accuracy = (short)((Class_004c46c0*)parser)->FUN_004c46c0("accuracy", 0);
    w->tolerance = (short)((Class_004c46c0*)parser)->FUN_004c46c0("tolerance", 0);
    w->pitchtolerance = (short)((Class_004c46c0*)parser)->FUN_004c46c0("pitchtolerance", 0);
    w->shakemagnitude = ((Class_004c46c0*)parser)->FUN_004c46c0("shakemagnitude", 0);
    w->shakeduration = (int)(((Class_004c4760*)parser)->FUN_004c4760("shakeduration", 0.0) * 30.0);

    char model[0x100];
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "model", 0x100, DAT_005119b8) != 0) {
        unsigned char i;
        unsigned char count = w->id;
        for (i = 0; i < count; i++) {
            if (_strcmpi(model, g_game->weapons[i].model) == 0) {
                g_game->weapons[count].model[0] = 0;
                g_game->weapons[count].text = g_game->weapons[i].text;
                goto model_done;
            }
        }
        char path[0x100];
        FUN_004290f0(path, "objects3d", model, "3DO");
        void* h = FUN_004cb560(path);
        if (h == 0)
            FUN_004b6290(path);
        FUN_004cb590(h);
        FUN_0042a140(h, model);
        g_game->weapons[count].text = h;
        strcpy(g_game->weapons[count].model, model);
    } else {
        w->text = 0;
    }
model_done:
    w->anim1 = 0;
    char gaf[0x100];
    if (((Class_004c48c0*)parser)->FUN_004c48c0(gaf, "explosiongaf", 0x100, DAT_005119b8) != 0 &&
        ((Class_004c48c0*)parser)->FUN_004c48c0(model, "explosionart", 0x100, DAT_005119b8) != 0) {
        void* a = FUN_00429700(gaf);
        void* r = FUN_004b8d40(a, model);
        *(unsigned char*)((char*)r + 2) = 0;
        w->anim1 = r;
    }
    w->anim2 = 0;
    if (*(int*)(*(char**)((char*)g_game + 0x391e9) + 0xd44) != 0) {
        if (((Class_004c48c0*)parser)->FUN_004c48c0(gaf, "lavaexplosiongaf", 0x100, DAT_005119b8) !=
                0 &&
            ((Class_004c48c0*)parser)
                    ->FUN_004c48c0(model, "lavaexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = FUN_00429700(gaf);
            void* r = FUN_004b8d40(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    } else {
        if (((Class_004c48c0*)parser)
                    ->FUN_004c48c0(gaf, "waterexplosiongaf", 0x100, DAT_005119b8) != 0 &&
            ((Class_004c48c0*)parser)
                    ->FUN_004c48c0(model, "waterexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = FUN_00429700(gaf);
            void* r = FUN_004b8d40(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundstart", 0x100, DAT_005119b8) != 0) {
        w->soundstart = (unsigned short)FUN_00429470(0, model);
    } else {
        w->soundstart = 0xffff;
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundhit", 0x100, DAT_005119b8) != 0) {
        w->soundhit = (unsigned short)FUN_00429470(0, model);
    } else {
        w->soundhit = 0xffff;
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundwater", 0x100, DAT_005119b8) != 0) {
        w->soundwater = (unsigned short)FUN_00429470(0, model);
    } else {
        w->soundwater = 0xffff;
    }
    void* damage = ((Class_004c4470*)parser)->FUN_004c4470("DAMAGE");
    if (damage != 0) {
        w->damage = (short)((Class_004c46c0*)damage)->FUN_004c46c0("default", 0);
        int index = 0;
        char* key = ((Class_004c45e0*)damage)->FUN_004c45e0(index);
        while (key) {
            if (_strcmpi(key, "default") != 0) {
                int value = ((Class_004c46c0*)damage)->FUN_004c46c0(key, 0);
                if (!w->sub)
                    w->sub = new Damage_0042e440;
                Class_004c91b0 name(key);
                std::vector<Entry_00432cf0>& entries = w->sub->entries;
                Entry_00432cf0* first = entries.begin();
                Entry_00432cf0* last = entries.end();
                while (first != last) {
                    unsigned int half = (unsigned int)(last - first) / 2;
                    if ((unsigned char)(_strcmpi((first + half)->name.ptr, name.ptr) < 0))
                        first = first + half + 1;
                    else
                        last = first + half;
                }
                if (first == entries.end() || strcmp(first->name.ptr, name.ptr) != 0) {
                    unsigned int offset = first - entries.begin();
                    entries.insert(first, 1, Entry_00432cf0(*(Class_004c91a0*)&name, 0));
                    first = entries.begin() + offset;
                }
                first->value = value;
            }
            key = ((Class_004c45e0*)damage)->FUN_004c45e0(++index);
        }
    } else {
        w->damage = 0;
    }
    FUN_0049e010(w);
}
