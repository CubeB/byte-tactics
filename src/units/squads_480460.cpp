// Decompiled by Opus. Names are provisional.
// For every active unit in the array owned by `owner` whose field at +0xac
// equals `key`, asks FUN_0043f0e0 for an order kind and hands it, with the
// remaining arguments, to FUN_0043adc0.

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xa6];
    short active;                      // +0xa6
    char unknown_a8[0xac - 0xa8];
    int key;                           // +0xac
    char unknown_b0[0x118 - 0xb0];
};

struct Owner_00480460 {
    char unknown_0[0x67];
    Unit* first;                       // +0x67
    Unit* last;                        // +0x6b
};
#pragma pack(pop)

Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit* unit,
                                       Unit* target, int flags);
void __stdcall FUN_0043adc0(Class_00438760 kind, int remove, Unit* owner, Unit* id, int flags, int param_6, int param_7);

// FUNCTION: 0x480460
void __stdcall OrderSquad(Owner_00480460* owner, int key, unsigned char mode, int remove,
                            Unit* target, int flags, int param_7, int param_8)
{
    for (Unit* u = owner->first; u <= owner->last; u++) {
        if (u->active != 0 && u->key == key) {
            Class_00438760 kind = FUN_0043f0e0(mode, u, target, flags);
            FUN_0043adc0(kind, remove, u, target, flags, param_7, param_8);
        }
    }
}
