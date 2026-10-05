// Decompiled by deepseek-v4.1-flash. Names are provisional.
struct Data_004b2450 {
    int unknown_0;    // +0x00
    int count_1;      // +0x04
    int count_2;      // +0x08
    int unknown_c;    // +0x0c
    int checksum;     // +0x10
    int count_3;      // +0x14
    int offset_18;    // +0x18
    int offset_1c;    // +0x1c
    int offset_20;    // +0x20
    int offset_24;    // +0x24
    int offset_28;    // +0x28
};

struct Pair8_004b2450 {
    int unknown_0;    // +0x00
    int offset;       // +0x04
};

// std::map<int, int>'s tree, written by hand so that _Tree::insert (0x4b2850,
// which has its own file) stays an out-of-line call under its real name.
namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template<class T> struct less {};
template<class T> class allocator {};
template<class K, class T, class Pr = less<K>, class A = allocator<T> > class map {
public:
    struct _Kfn {};
};
template<class K, class Ty, class Kfn, class Pr, class A> class _Tree {
public:
    class iterator {
    public:
        int _Ptr;
    };
    pair<iterator, bool> insert(const Ty& v);
};
}
typedef std::_Tree<int, std::pair<int, int>, std::map<int, int>::_Kfn,
                   std::less<int>, std::allocator<int> > Tree_004b2450;

extern char DAT_0051fbc0;

extern Data_004b2450* __stdcall HAPI_LoadFile(char* name, int reserved);
extern int __stdcall HAPI_FileLengthByName(char* name);
extern int __stdcall ComputeChecksum(unsigned char* data, int len);

// The original's map lookup (`DAT_0051fbc0[(int)data] = sum`) inlined
// map::operator[] but left _Tree::insert (0x4b2850) out of line, so the
// insert is called explicitly here instead of through <map>.
// FUNCTION: 0x4b2450
Data_004b2450* __stdcall LoadCobScript(char* name)
{
    Data_004b2450* data = HAPI_LoadFile(name, 0);
    if (data == 0)
        return 0;
    int sum = ComputeChecksum((unsigned char*)data, HAPI_FileLengthByName(name));
    std::pair<int, int> value;
    value.first = (int)data;
    value.second = 0;
    std::pair<Tree_004b2450::iterator, bool> out = ((Tree_004b2450*)&DAT_0051fbc0)->insert(value);
    ((int*)out.first._Ptr)[4] = sum;
    data->offset_18 += (int)data;
    data->offset_1c += (int)data;
    for (int i = 0; i < data->count_1; i++)
        ((int*)data->offset_1c)[i] += (int)data;
    data->offset_20 += (int)data;
    for (int j = 0; j < data->count_2; j++)
        ((int*)data->offset_20)[j] += (int)data;
    data->offset_24 += (int)data;
    data->offset_28 += (int)data;
    for (int k = 0; k < data->count_3; k++)
        ((Pair8_004b2450*)data->offset_28)[k].offset += (int)data;
    return data;
}
