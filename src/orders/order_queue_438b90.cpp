// Decompiled by Opus. Names are provisional.
// Changes the object's kind: stores the new kind and reloads its flags from
// the kind table (DAT_00512344, 0x19-byte entries, default flags at +0x11),
// keeping the object's own bits 9 and 10 (0x600; the constructor 0x43a0c0
// clears them individually). The layout (kind at +0x4, flags at +0x42)
// matches Class_0043a1f0, which this probably is.
// The original reads the table entry twice with separately computed
// addresses: once indexed by the parameter masked to a byte, once by the
// kind field just stored. Any pair of identical index expressions (or an
// unsigned char parameter) lets MSVC share one load.

#pragma pack(push, 1)
struct Entry_00438b90 {                // 0x19 bytes, table at DAT_00512344
    char unknown_0[0x11];
    unsigned int flags;                // +0x11, default flags of the kind
    char unknown_15[4];
};
#pragma pack(pop)

extern Entry_00438b90* DAT_00512344;

#pragma pack(push, 1)
class Class_00438b90 {
public:
    char unknown_0[4];
    unsigned char kind;                // +0x4
    char unknown_5[0x42 - 5];
    unsigned int flags;                // +0x42

    void FUN_00438b90(int k);
};
#pragma pack(pop)

// FUNCTION: 0x438b90
void Class_00438b90::FUN_00438b90(int k)
{
    kind = k;
    flags = ((DAT_00512344[k & 0xff].flags ^ flags) & 0x600) ^ DAT_00512344[kind].flags;
}
