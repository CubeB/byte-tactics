// Decompiled by Opus. Names are provisional.
// Asks the unit's script which piece the weapon (0..2) aims from
// ("QueryPrimary", "QuerySecondary", "QueryTertiary") and returns it.

class Class_004b0bc0 {
public:
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 1)
struct Object {
    char unknown_0[0x9a];
    Class_004b0bc0* script;            // +0x9a
};
#pragma pack(pop)

// FUNCTION: 0x43e1e0
int __stdcall QueryWeaponPiece(Object* obj, unsigned char weapon)
{
    char* names[3] = { "QueryPrimary", "QuerySecondary", "QueryTertiary" };
    int piece = 0;
    obj->script->QueryScript(names[weapon], &piece, 0, 0, 0);
    return piece;
}
