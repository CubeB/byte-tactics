// Decompiled by Sonnet. Names are provisional.
// HandleSyncPacket reads fields of the same object (offsets +0x14, +0x58, +0x5c,
// +0x64) that this function's "this" belongs to; no `mov ecx` appears before
// the call, so ecx (this) flows through unchanged from ReceiveSyncPacket's own
// thiscall "this" into HandleSyncPacket's.

class UnitSync {
public:
    void HandleSyncPacket(void* param_1, int param_2);
};

class Class_0046d500 {
public:
    void ReceiveSyncPacket(void* param_1, int param_2);
};

// FUNCTION: 0x46d500
void Class_0046d500::ReceiveSyncPacket(void* param_1, int param_2)
{
    unsigned char b = *((unsigned char*)param_1 + 1);
    *(int*)((char*)param_1 + 2) = 0;
    if (b < 0x64) {
        ((UnitSync*)this)->HandleSyncPacket(param_1, param_2);
    }
}
