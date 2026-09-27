// Decompiled by Opus. Names are provisional.
// Getter counterpart of 0x49fb10: reads field +0x18 of the object at +0x18.

struct Inner_0049fb30 {
    char unknown_0[0x18];
    int field_18;                      // +0x18
};

struct Outer_0049fb30 {
    char unknown_0[0x18];
    Inner_0049fb30* inner;             // +0x18
};

// FUNCTION: 0x49fb30
int __stdcall FUN_0049fb30(Outer_0049fb30* obj)
{
    if (obj->inner != 0) {
        return obj->inner->field_18;
    }
    return 0;
}
