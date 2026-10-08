// Decompiled by longcat-2.5-preview-free, finished by space-bunny-free and deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// Stays in its own file: it is built with /Gi, which unit_sync.cpp
// cannot carry.
// std::vector<UnitSyncPacket>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, emitted out of line. Its one caller, 0x46cef0, appends a
// packet to the queue at +0x1c of PacketSequencer with push_back, whose inlined
// insert(end(), x) calls this with a count of 1. The element is the 14-byte
// packed packet of 0x46cef0 (copied as three dwords and a word).
#include <vector>

#pragma pack(push, 1)
struct UnitSyncPacket {                // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    unsigned int id;                   // +0x2
    int key;                           // +0x6
    int value;                         // +0xa
};
#pragma pack(pop)

typedef std::vector<UnitSyncPacket> Vec_0046eba0;
typedef void (Vec_0046eba0::*InsertFn_0046eba0)(
    Vec_0046eba0::iterator, Vec_0046eba0::size_type,
    const UnitSyncPacket&);

// The push_back of 0x46cef0's queue, the use that instantiates this insert.
void __stdcall Push_0046eba0(Vec_0046eba0* v, const UnitSyncPacket& x)
{
    v->push_back(x);
}

// FUNCTION: 0x46eba0 ?insert@?$vector@UUnitSyncPacket@@V?$allocator@UUnitSyncPacket@@@std@@@std@@QAEXPAUUnitSyncPacket@@IABU3@@Z
InsertFn_0046eba0 g_insert_0046eba0 = &Vec_0046eba0::insert;
