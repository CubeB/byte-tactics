// Decompiled by Opus. Names are provisional.

struct Flags_0049e010 {
    unsigned int bit0 : 1;
    unsigned int unknown_1 : 3;
    unsigned int bit4 : 1;
    unsigned int unknown_5 : 3;
    unsigned int bit8 : 1;
    unsigned int unknown_9 : 10;
    unsigned int bit19 : 1;
    unsigned int bit20 : 1;
};

struct Info_0049e010;

typedef int (__stdcall* DrawFn_0049e010)(Info_0049e010* info, int a, int b, int c);

#pragma pack(push, 1)
struct Info_0049e010 {
    char unknown_0[0x60];
    DrawFn_0049e010 draw;              // +0x60
    char unknown_64[0x111 - 0x64];
    Flags_0049e010 flags;              // +0x111
};
#pragma pack(pop)

int __stdcall FireTurretWeapon(Info_0049e010* info, int a, int b, int c);
int __stdcall FireVLaunchWeapon(Info_0049e010* info, int a, int b, int c);
int __stdcall FireDroppedWeapon(Info_0049e010* info, int a, int b, int c);
int __stdcall FireLineOfSightWeapon(Info_0049e010* info, int a, int b, int c);

// Picks the routine for the object's flags.
// FUNCTION: 0x49e010
void __stdcall SetWeaponFireHandler(Info_0049e010* info)
{
    if (info->flags.bit19) {
        info->draw = FireTurretWeapon;
        return;
    }
    if (info->flags.bit4) {
        info->draw = FireVLaunchWeapon;
        return;
    }
    if (info->flags.bit0 || info->flags.bit20)
        info->draw = FireLineOfSightWeapon;
    else if (info->flags.bit8)
        info->draw = FireDroppedWeapon;
}
