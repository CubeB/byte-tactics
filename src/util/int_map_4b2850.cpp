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
// _Size at +0xc.
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
