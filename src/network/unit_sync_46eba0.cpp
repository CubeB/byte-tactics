// Decompiled by longcat-2.5-preview-free, finished by space-bunny-free and deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<Packet_0046cef0>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, emitted out of line. Its one caller, 0x46cef0, appends a
// packet to the queue at +0x1c of Class_0046cef0 with push_back, whose inlined
// insert(end(), x) calls this with a count of 1. The element is the 14-byte
// packed packet of 0x46cef0 (copied as three dwords and a word).
//
// Built with /Gi like the original's translation unit (the same recipe as the
// 0x4758c0 to 0x476490 inserts, #5035): the real header plus one push_back on
// the same vector type matches. Without /Gi this source is 62.7%. Under /Gi,
// operator= or resize in place of push_back match too, while no other use,
// reserve, the copy constructor or erase give 63.3%. Under the default
// flags the best was a hand-written clone of the template at 86.1% (the third
// copy walking _P with _Last in a local), because there the grow arm's third
// copy derives its source start dest first, where the original has the
// _P-first `sub esi, edx; add esi, ecx; sub esi, eax`; /Gi's per-function
// symbol numbering is what reverses that order.
#include <vector>

#pragma pack(push, 1)
struct Packet_0046cef0 {               // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    unsigned int id;                   // +0x2
    int field_6;                       // +0x6
    int field_a;                       // +0xa
};
#pragma pack(pop)

typedef std::vector<Packet_0046cef0> Vec_0046eba0;
typedef void (Vec_0046eba0::*InsertFn_0046eba0)(
    Vec_0046eba0::iterator, Vec_0046eba0::size_type,
    const Packet_0046cef0&);

// The push_back of 0x46cef0's queue, the use that instantiates this insert.
void __stdcall Push_0046eba0(Vec_0046eba0* v, const Packet_0046cef0& x)
{
    v->push_back(x);
}

// FUNCTION: 0x46eba0 ?insert@?$vector@UPacket_0046cef0@@V?$allocator@UPacket_0046cef0@@@std@@@std@@QAEXPAUPacket_0046cef0@@IABU3@@Z
InsertFn_0046eba0 g_insert_0046eba0 = &Vec_0046eba0::insert;
