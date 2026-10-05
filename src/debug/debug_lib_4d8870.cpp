// Decompiled by Claude Opus 5.5. Names are provisional.
#include <string.h>

void __cdecl WalkFrameChain(int* frame, int* stack, int eip, int skip, int* out, int max, int* count,
                          int* field_3c, int size, int* field_40);

// A stack trace of up to 14 return addresses (0x4d9c60.cpp has a bigger one).
struct Trace_004d8870 {
    int entries[14];                    // +0x0
    int count;                          // +0x38
    int field_3c;                       // +0x3c
    int field_40;                       // +0x40
    int* stack;                         // +0x44, the stack pointer it was taken at

    // Walks the stack from here, `skip` frames up. Inlined into each caller,
    // so the frame, stack pointer and address it reads are the caller's.
    void Capture(int skip)
    {
        int* frame;
        int* top;
        int eip;
        __asm {
            mov frame, ebp
            mov top, esp
        here:
            lea eax, here
            mov eip, eax
        }
        stack = top;
        WalkFrameChain(frame, top, eip, skip, entries, 14, &count, &field_3c, 1, &field_40);
    }
};

// A named record of where it was made: the name, an id and the stack trace
// of the code that made it.
class TraceRecord {
public:
    char name[0x40];                    // +0x0
    int id;                             // +0x40
    Trace_004d8870 trace;               // +0x44

    TraceRecord(void);
    TraceRecord(const char* name_, int id_, int skip);
};

// FUNCTION: 0x4d8870 ??0TraceRecord@@QAE@XZ
TraceRecord::TraceRecord(void)
{
    trace.Capture(0);
    name[0] = 0;
    id = -1;
}

// The name keeps its last 63 characters.
// FUNCTION: 0x4d88d0 ??0TraceRecord@@QAE@PBDHH@Z
TraceRecord::TraceRecord(const char* name_, int id_, int skip)
{
    trace.Capture(skip + 1);
    if (name_) {
        if (strlen(name_) >= 0x40)
            strcpy(name, name_ + strlen(name_) - 0x3f);
        else
            strcpy(name, name_);
    } else
        name[0] = 0;
    id = id_;
}
