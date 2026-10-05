// Decompiled by Sonnet. Names are provisional.

class Listener_00471120 {
public:
    virtual void unk0();
    virtual void unk1();
    virtual void Notify(void* param);
};

struct Slot_00471120 {
    char unknown_0[4];
    Listener_00471120** first;         // +0x4
    Listener_00471120** last;          // +0x8
    char unknown_c[4];
};

class ParticleLists {
public:
    Slot_00471120 slots[1];
    void DrawList(void* param, short index);
};

// FUNCTION: 0x471120
void ParticleLists::DrawList(void* param, short index)
{
    Slot_00471120* s = &slots[index];
    for (Listener_00471120** p = s->first; p != s->last; p++)
        (*p)->Notify(param);
}
