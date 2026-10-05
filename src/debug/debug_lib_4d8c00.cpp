// Decompiled by Opus. Names are provisional.
// A constructor: three member objects are constructed, then the first one is
// overwritten with a copy of the header passed in and a flag is cleared.

class BlockInfo {
public:
    char unknown_0[0x30];

    BlockInfo();
};

class Class_004d88d0 {
public:
    char unknown_0[0x8c];

    Class_004d88d0(const char* name, int id, int count);
};

class TraceRecord {
public:
    char unknown_0[0x8c];

    TraceRecord();
};

class Class_004d8c00 {
public:
    BlockInfo header;                  // +0x0
    Class_004d88d0 field_30;           // +0x30
    TraceRecord field_bc;              // +0xbc
    char field_148;                    // +0x148

    Class_004d8c00(const BlockInfo& h, const char* name, int id, int count);
};

// FUNCTION: 0x4d8c00
Class_004d8c00::Class_004d8c00(const BlockInfo& h, const char* name, int id, int count)
    : field_30(name, id, count + 1)
{
    header = h;
    field_148 = 0;
}
