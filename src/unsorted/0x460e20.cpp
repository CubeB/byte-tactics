// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// GAVE UP: best 53.7%. The compiler-generated dynamic initialiser (_$E4) of the
// global DAT_00513000 (vtable 0x4fd514): 11 per-player channel records of
// 0x1044 bytes at +0x8 (head of 14 dwords then a Class_00460f40 at +0x38 and
// FUN_00462860(4000)/FUN_004628a0(200)), followed at +0xB300 by a
// Class_00462d30 sub-object with 10 Class_004635b0 entries and a trailing
// {0,0,0,-1,-1}. Two structural problems remain, both documented by the worker:
//   1. the channel head must be a plain member of the global so its reset stores
//      inline (naming it a class makes MSVC emit a ??0 call), and
//   2. the +0xB528 tail belongs to a distinct sub-object of Class_00462d30 that
//      I could not place so that it inlines at the right offsets.
// The full notes are in the PR body; this file is the best model.
class Class_00462860 {
public:
    void FUN_00462860(int x);
};
class Class_004628a0 {
public:
    void FUN_004628a0(int x);
};
class Class_00460f40 {
public:
    Class_00460f40();
};

struct Head_00460e20 {
    int f0, f4, f8, fc, f10, f14, f18, f1c, f20, f24, f28, f2c, f30, f34;

    Head_00460e20()
    {
        f0 = -1; f4 = 0; f8 = 0; fc = 0; f10 = -2; f14 = -1; f18 = 0; f1c = -1;
        f20 = 0; f24 = 0; f28 = 0; f2c = 0; f30 = 0; f34 = 0;
    }
};

struct Chan_00460e20 {
    Head_00460e20 head;                // +0x0
    Class_00460f40 body;               // +0x38
    char pad[0x1044 - 0x38 - sizeof(Class_00460f40)];

    Chan_00460e20()
    {
        ((Class_00462860*)this)->FUN_00462860(4000);
        ((Class_004628a0*)this)->FUN_004628a0(200);
    }
};

class Class_00460f60 {
public:
    virtual ~Class_00460f60();
    unsigned long pacing;              // +0x4
    Chan_00460e20 channels[11];        // +0x8
};

// FUNCTION: 0x460e20 _$E4
Class_00460f60 DAT_00513000;
