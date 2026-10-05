// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Index-taking core of 0x4b0bc0: claims a channel for `index` (0x4b08c0),
// pushes the four pointed-to values (0 when the pointer is null) onto the
// channel's value list, processes the channel with 0x4b0da0, then writes the
// four processed values back through the pointers. Compare 0x4b0bc0, its
// name-taking wrapper.
//
// The channel value list is at +0x40 with its count at +0x24, relative to
// `this + index * 0xa4`; the compiler folds the channel array's own +0x1c base
// into those offsets, so the struct below keeps the 0x1c preamble in Channel.
// Binding a reference to the count (`int& cnt = c->count;`) is what keeps the
// count store in the fourth push, which a plain `++c->count` drops.

struct Channel_004b0c40 {
    char unknown_0[0x24];
    int count;                         // +0x24
    char unknown_28[0x3c - 0x28];
    int unknown_3c;                    // +0x3c
    int values[5];                     // +0x40
    char unknown_54[0xa4 - 0x54];
};

class CobScript {
public:
    Channel_004b0c40 channels[8];
    int activeCount;
    int QueryScriptByIndex(int index, int* p2, int* p3, int* p4, int* p5);
    int StartThread(int id);
    void RunThread(int channel, int param_2);
};

// FUNCTION: 0x4b0c40
int CobScript::QueryScriptByIndex(int index, int* p2, int* p3, int* p4, int* p5)
{
    int i = ((CobScript*)this)->StartThread(index);
    if (i < 0)
        return 0;
    Channel_004b0c40* c = &channels[i];
    c->unknown_3c = 0;
    int& cnt = c->count;
    c->values[++cnt] = p2 ? *p2 : 0;
    c->values[++cnt] = p3 ? *p3 : 0;
    c->values[++cnt] = p4 ? *p4 : 0;
    c->values[++cnt] = p5 ? *p5 : 0;
    c->count = 3;
    ((CobScript*)this)->RunThread(i, 0);
    if (p2) *p2 = c->values[0];
    if (p3) *p3 = c->values[1];
    if (p4) *p4 = c->values[2];
    if (p5) *p5 = c->values[3];
    return 1;
}
