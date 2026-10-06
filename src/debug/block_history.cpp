// Decompiled by Haiku, deepseek-v4.1 and Sonnet. Names are provisional.

#include <string.h>

// Memory block description, 0x30 bytes, passed by value to FormatBlockInfo.
class BlockInfo {
public:
    int address;                     // +0x00
    int size;                        // +0x04
    int allocNumber;                 // +0x08
    char name[0x24];                 // +0x0c

    BlockInfo(void);
};

// One allocation or free site, 0x8c bytes.
class TraceRecord {
public:
    char data[0x8c];
    TraceRecord(void);
};

// The same record as TraceRecord, under the name its formatter has.
class CallSite {
public:
    char unknown_0[0x40];
    int line;                        // +0x40
    void FormatCallSite(char* buf, int size);
};

extern void __cdecl FormatBlockInfo(BlockInfo info, char* buf, int unused);

class BlockHistory : public BlockInfo {
public:
    TraceRecord allocSite;           // +0x30
    TraceRecord freeSite;            // +0xbc
    char field_148;                  // +0x148

    BlockHistory(void);
    void FormatBlockHistory(char* buf, int size, char freed);
    void FUN_004d8d40(char freed);
};

// FUNCTION: 0x4d8bd0
BlockHistory::BlockHistory(void) :
    BlockInfo(),
    allocSite(),
    freeSite()
{
    field_148 = 0;
}

// The original calls this from 0x4d8d40 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4d8c50
void BlockHistory::FormatBlockHistory(char* buf, int size, char freed)
{
    FormatBlockInfo(*this, buf, size);
    strcat(buf, ", allocated from:");
    char* p = buf + strlen(buf);
    ((CallSite*)&allocSite)->FormatCallSite(p, size - strlen(buf));
    if (freed) {
        strcat(buf, "\n\tfreed from:");
        char* q = buf + strlen(buf);
        ((CallSite*)&freeSite)->FormatCallSite(q, size - strlen(buf));
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4d8d40
void BlockHistory::FUN_004d8d40(char freed)
{
    char buf[2000];
    FormatBlockHistory(buf, 2000, freed);
}
