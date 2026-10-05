// Decompiled by Opus. Names are provisional.
// HAPINET_initmultiplaydefaults: sets two network defaults (0x10 and 1500).

#pragma pack(push, 1)
struct Net_4c9c20 {
    char unknown_0[0x4dd];
    int field_4dd;                     // +0x4dd
    int field_4e1;                     // +0x4e1
};
#pragma pack(pop)

void __cdecl HapinetTrace(int);

// FUNCTION: 0x4c9c20
void __stdcall HAPINET_initmultiplaydefaults(Net_4c9c20* net)
{
    HapinetTrace((int)"HAPINET_initmultiplaydefaults\n");
    net->field_4dd = 0x10;
    net->field_4e1 = 0x5dc;
}
