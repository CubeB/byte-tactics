// Decompiled by space-bunny-free, Opus and Space Bunny Free. Names are provisional.
// PacketSequencer::operator=: copies three ints, then assigns two vectors of a
// 14-byte element type (Elem_0046faf0, packed to 2 bytes, so the three ints and
// the short give 14 bytes and the copy loops move three dwords and a word).
// The first vector's operator= is inlined by /Ob2, the second stays a call to
// its out-of-line copy (0x4707a0), and the same inline budget is why three of
// the seven size() calls are calls to 0x470770 while the other four are
// expanded inline. The element type's _Ucopy (0x46faf0) and _Destroy
// (0x46e870) are called, and the 14-byte std::copy of the "enough room, but
// longer" branch is 0x470a40.
// The file of the original was compiled with /Gz, so <xutility>'s templates
// are __stdcall there and the std::copy call cleans up its own arguments.
// Defining _XUTILITY_ keeps out the header's __cdecl ones, which <vector>
// would otherwise include; the block below stands in for <xutility> (the same
// stand-in 0x424c00.cpp uses, with the C4666 warnings it gives).
//
// Every byte matches, but check.py still reports two references as wrong, both
// of them names in data/ that this file cannot use and should not work around:
//
//  0. The three calls to 0x470770 come out as `Elem_0046faf0::?$vector::size`,
//     which is what they are: taking the address of the real
//     `std::vector<Elem_0046faf0>::size` compiles to 0x470770 byte for byte
//     (checked with `?size@?$vector@UElem_0046faf0@@...@QBEIXZ`). data/symbols.csv
//     gives 0x470770 the placeholder name `Class_00470770::FUN_00470770` from
//     0x470770.cpp, a hand-written stand-in for the same body. Renaming that
//     row the way 0x40c5b0.cpp names vector<Elem_0040cc40>::size would make
//     these three references right.
//  1. The std::copy at 0x470a40 is the second `std::copy` instantiation the
//     checker sees (0x4256a0 is the first, for unsigned short). Overloads and
//     duplicate instantiations share one name in base_name, which is the case
//     docs/consolidation.md predicts needs a row in data/aliases.csv:
//     `std::copy,0x470a40,...`.
#include <utility>

#define _XUTILITY_
namespace std {
template <class _II, class _OI>
inline _OI __stdcall copy(_II _F, _II _L, _OI _X)
{
    for (; _F != _L; ++_X, ++_F)
        *_X = *_F;
    return (_X);
}
template <class _BI1, class _BI2>
inline _BI2 __stdcall copy_backward(_BI1 _F, _BI1 _L, _BI2 _X)
{
    while (_F != _L)
        *--_X = *--_L;
    return (_X);
}
template <class _II1, class _II2>
inline bool __stdcall equal(_II1 _F, _II1 _L, _II2 _X)
{
    return (mismatch(_F, _L, _X).first == _L);
}
template <class _II1, class _II2, class _Pr>
inline bool __stdcall equal(_II1 _F, _II1 _L, _II2 _X, _Pr _P)
{
    return (mismatch(_F, _L, _X, _P).first == _L);
}
template <class _FI, class _Ty>
inline void __stdcall fill(_FI _F, _FI _L, const _Ty& _X)
{
    for (; _F != _L; ++_F)
        *_F = _X;
}
template <class _OI, class _Sz, class _Ty>
inline void __stdcall fill_n(_OI _F, _Sz _N, const _Ty& _X)
{
    for (; 0 < _N; --_N, ++_F)
        *_F = _X;
}
template <class _II1, class _II2>
inline bool __stdcall lexicographical_compare(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2)
{
    for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
        if (*_F1 < *_F2)
            return (true);
        else if (*_F2 < *_F1)
            return (false);
    return (_F1 == _L1 && _F2 != _L2);
}
template <class _II1, class _II2, class _Pr>
inline bool __stdcall lexicographical_compare(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _Pr _P)
{
    for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
        if (_P(*_F1, *_F2))
            return (true);
        else if (_P(*_F2, *_F1))
            return (false);
    return (_F1 == _L1 && _F2 != _L2);
}
#define _MAX _cpp_max
#define _MIN _cpp_min
template <class _Ty>
inline const _Ty& __stdcall _cpp_max(const _Ty& _X, const _Ty& _Y)
{
    return (_X < _Y ? _Y : _X);
}
template <class _Ty, class _Pr>
inline const _Ty& __stdcall _cpp_max(const _Ty& _X, const _Ty& _Y, _Pr _P)
{
    return (_P(_X, _Y) ? _Y : _X);
}
template <class _Ty>
inline const _Ty& __stdcall _cpp_min(const _Ty& _X, const _Ty& _Y)
{
    return (_Y < _X ? _Y : _X);
}
template <class _Ty, class _Pr>
inline const _Ty& __stdcall _cpp_min(const _Ty& _X, const _Ty& _Y, _Pr _P)
{
    return (_P(_Y, _X) ? _Y : _X);
}
template <class _II1, class _II2>
inline pair<_II1, _II2> __stdcall mismatch(_II1 _F, _II1 _L, _II2 _X)
{
    for (; _F != _L && *_F == *_X; ++_F, ++_X)
        ;
    return (pair<_II1, _II2>(_F, _X));
}
template <class _II1, class _II2, class _Pr>
inline pair<_II1, _II2> __stdcall mismatch(_II1 _F, _II1 _L, _II2 _X, _Pr _P)
{
    for (; _F != _L && _P(*_F, *_X); ++_F, ++_X)
        ;
    return (pair<_II1, _II2>(_F, _X));
}
template <class _Ty>
inline void __stdcall swap(_Ty& _X, _Ty& _Y)
{
    _Ty _Tmp = _X;
    _X = _Y, _Y = _Tmp;
}
}

#include <vector>

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Packet_0046cef0 {         // 0xe bytes
    unsigned char type;          // +0x0
    unsigned char arg;           // +0x1
    unsigned int id;             // +0x2
    int field_6;                 // +0x6
    int field_a;                 // +0xa
};
#pragma pack(pop)

int __cdecl GetLocalHumanDpid();
void __stdcall SendPacketToPlayer(int a, unsigned int b, void* c, int d);

// A list seen as a std::vector of packets (allocator byte, _First, _Last,
// _End): ReceiveSequenced reads the lists through it, and FUN_0046eba0 is that
// vector's insert(end(), count, val), out of line.
class Class_0046eba0 {
public:
    char unknown_0[4];
    Packet_0046cef0* first;      // +0x4
    Packet_0046cef0* last;       // +0x8
    Packet_0046cef0* end;        // +0xc
    void FUN_0046eba0(Packet_0046cef0* where, int count, Packet_0046cef0* val);
};

class UnitSync {
public:
    void HandleSyncPacket(void* param_1, int param_2);
};

struct PacketSequencer {
    int field_0;                       // +0x00
    unsigned int cur;                  // +0x04
    unsigned int max;                  // +0x08
    std::vector<Elem_0046faf0> list_c; // +0x0c (16 bytes, _First at +0x10)
    std::vector<Elem_0046faf0> list_d; // +0x1c (operator= is 0x4707a0)
    PacketSequencer();
    PacketSequencer& operator=(const PacketSequencer& rhs);
    // In unit_sync_46cc10.cpp: it needs a hand-written std::vector, which
    // cannot share a file with <vector>.
    void SendSequenced(Elem_0046faf0* param_1, Elem_0046faf0* param_2);
    void ReceiveSequenced(Packet_0046cef0* packet, int param_2, void* param_3, unsigned int target);
};

// The constructor: two empty vectors (the allocator bytes are copied from an
// uninitialised temporary) and three dwords zeroed in the body.
// FUNCTION: 0x46cbe0
PacketSequencer::PacketSequencer()
{
    field_0 = 0;
    cur = 0;
    max = 0;
}

// Dispatches a 0xe-byte command packet. arg 0x65 re-sends the stored copy that
// has the same id; any other id widens the range and, when it is the one right
// after cur, runs cur and then every queued entry that follows it,
// asking the sender for the next one (packet 0x1a 0x65) whenever the queue runs
// dry.
// FUNCTION: 0x46cef0
void PacketSequencer::ReceiveSequenced(Packet_0046cef0* packet, int param_2, void* param_3, unsigned int target)
{
    if (packet->arg == 0x65) {
        for (Packet_0046cef0* p = ((Class_0046eba0*)&list_c)->first; p != ((Class_0046eba0*)&list_c)->last; p++) {
            if (p->id == packet->id) {
                SendPacketToPlayer(GetLocalHumanDpid(), target, p, 0xe);
                break;
            }
        }
        return;
    }
    if (packet->id > max) {
        max = packet->id;
    }
    if (packet->id == cur + 1) {
        cur = packet->id;
        ((UnitSync*)param_3)->HandleSyncPacket(packet, param_2);
        for (unsigned int i = cur + 1; i <= max; i++) {
            Packet_0046cef0* p;
            for (p = ((Class_0046eba0*)&list_d)->first; p != ((Class_0046eba0*)&list_d)->last; p++) {
                if (p->id == i) break;
            }
            if (p == ((Class_0046eba0*)&list_d)->last) break;
            cur = i;
            ((UnitSync*)param_3)->HandleSyncPacket(p, param_2);
        }
        return;
    }
    if (packet->id <= cur) {
        return;
    }
    // Through a reference, so the call sets up ecx before evaluating its
    // arguments, as the original does.
    Class_0046eba0& v = *(Class_0046eba0*)&list_d;
    v.FUN_0046eba0(v.last, 1, packet);
    for (unsigned int i = cur + 1; i <= max; i++) {
        Packet_0046cef0* p;
        for (p = ((Class_0046eba0*)&list_d)->first; p != ((Class_0046eba0*)&list_d)->last; p++) {
            if (p->id == i) break;
        }
        if (p == ((Class_0046eba0*)&list_d)->last) {
            Packet_0046cef0 msg;
            msg.type = 0x1a;
            msg.arg = 0x65;
            msg.id = i;
            SendPacketToPlayer(GetLocalHumanDpid(), target, &msg, 0xe);
        }
    }
}

// FUNCTION: 0x470560 ??4PacketSequencer@@QAEAAU0@ABU0@@Z
PacketSequencer& PacketSequencer::operator=(const PacketSequencer& rhs)
{
    field_0 = rhs.field_0;
    cur = rhs.cur;
    max = rhs.max;
    list_c = rhs.list_c;
    list_d = rhs.list_d;
    return *this;
}
