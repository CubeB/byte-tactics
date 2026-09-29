// Decompiled by space-bunny-free. Names are provisional.
//
// 0x4b2850 is the out-of-line copy MSVC 5 emitted of
// _Tree<_K, _Ty, _Kfn, _Pr, _A>::insert(const value_type&) from the toolchain's
// INCLUDE/XTREE (lines 211-232), instantiated for the std::map<int,int> whose
// _Nil sentinel is std::HH::HU?$pair::?$_Tree::_Nil (DAT_0051fbbc) and whose
// siblings are 0x4b2840 (begin), 0x4b3020 (_Insert), 0x4b3310 (_Lrotate),
// 0x4b33b0 (_Rrotate), 0x4b3410 (_Buynode), 0x4b34f0 (iterator::_Dec) and
// 0x4b2ac0 / 0x4b2fb0 (erase / _Erase).
//
// The object layout is XTREE's own (XTREE lines 593-597): allocator at +0,
// key_compare at +1, the _Head node pointer at +4, the _Multi flag at +8 and
// _Size at +0xc. That flag is why insert has two arms: with _Multi set it goes
// straight to _Insert, whose body (node creation, the _Head-linking step and
// the whole rebalance loop, XTREE lines 459-508) /Ob2 inlines here, and with
// _Multi clear it steps back one node with iterator::_Dec to look for an equal
// key first, the duplicate check only a map needs, and then calls the out-of-line
// _Insert.
//
// Two source shapes below are load bearing, and both were measured with
// tools/check.py on this function:
//   * The copy of insert has to come from a real call in a caller too big for
//     /Ob2 to inline it. Forcing the copy into the object with
//     `&Map::insert` instead (a member-pointer initialiser) also emits it, but
//     in that state /Ob2 does NOT inline _Insert at the _Multi site: the
//     function then comes out 283 bytes, all four _Insert calls out of line.
//     A real call from a big caller gives the original's 624 bytes.
//   * The first _Lockit, the one in the search block (XTREE lines 215-220),
//     gets its own stack slot and is destroyed before the _Multi test, and the
//     second one (the _Insert's own _Lockit) gets the next slot, which is why
//     both must stay in their own braced blocks.
//
// check.py reports 100.0% (all 624 bytes identical) but still marks five
// references BAD, and only for the three helpers data/symbols.csv files under
// provisional placeholder names: 0x4b3310 is Class_004b3310::FUN_004b3310, and
// 0x4b33b0 is Class_004b33b0::FUN_004b33b0, where the real template members
// are _Tree<...>::_Lrotate and _Rrotate, and 0x4b3410 is
// Class_004b3410::FUN_004b3410 where the real member is _Tree<...>::_Buynode.
// All three are COMDAT template instantiations, the kind of symbol the linker
// discards, which is presumably why they never got a real name. Every other
// reference here checks out: _Insert at 0x4b3020, iterator::_Dec at 0x4b34f0,
// _Nil at 0x51fbbc and both std::_Lockit members. Reaching those three
// through the placeholder names would mean hand-writing the tree class, and
// that drops the byte match to 38% (see 0x4b26f0.cpp for the same trade-off),
// so the real <map> source is kept.
#include <map>

typedef std::map<int,int> Map_004b2850;
typedef Map_004b2850::value_type Vt_004b2850;

extern Map_004b2850 DAT_0051fbc0;

static int mix_004b2850(int a, int b)
{
    if (a > b)
        return a - b;
    if (a < b)
        return b - a;
    a += 3;
    b *= 2;
    a ^= b;
    b ^= a;
    a ^= b;
    return a + b + a * b;
}

// Only the call to insert matters here: it is what puts the out-of-line copy
// of insert in the object file.
Map_004b2850::_Pairib __stdcall use_004b2850(const Vt_004b2850& v, int a, int b,
    int c, int d, int e, int f, int g, int h)
{
    int r = mix_004b2850(a, b) + mix_004b2850(c, d) + mix_004b2850(e, f)
        + mix_004b2850(g, h) + mix_004b2850(a, e) + mix_004b2850(b, f)
        + mix_004b2850(c, g) + mix_004b2850(d, h);
    if (r == 12345)
        r = mix_004b2850(r, a);
    if (r == 999)
        r = mix_004b2850(r, b);
    if (r < 0)
        r = -r;
    if (r > 1000000)
        r = 0;
    return DAT_0051fbc0.insert(v);
}

// FUNCTION: 0x4b2850 ?insert@?$_Tree@HU?$pair@HH@std@@U_Kfn@?$map@HHU?$less@H@std@@V?$allocator@H@2@@2@U?$less@H@2@V?$allocator@H@2@@std@@QAE?AU?$pair@Viterator@?$_Tree@HU?$pair@HH@std@@U_Kfn@?$map@HHU?$less@H@std@@V?$allocator@H@2@@2@U?$less@H@2@V?$allocator@H@2@@std@@_N@2@ABU?$pair@HH@2@@Z
