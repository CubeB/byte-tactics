// Decompiled by space-bunny-free. Names are provisional.

#pragma pack(push, 1)
struct UnitRecord_00486fd0 {            // 0xb8 bytes, one saved unit
    char unknown_0[0x21];
    short id;                           // +0x21
    char unknown_23[0xb8 - 0x23];
};
#pragma pack(pop)

class HapiBank {
public:
    int OpenAccount(char* name);
    int GetIntegerItem(char* name, int def);
    int OpenNumberedBox(int a);
    void SeekBox(int pos);
    int ReadBox(void* buf, int len);
};

void __stdcall LoadUnit(short id, HapiBank* file);

// Reads the "Units" section of a saved game: one 0xb8 byte record per unit,
// handed to LoadUnit. A record 2 bytes short comes from an older save, and
// its unit id is not present, so it is set to 0 first.
// FUNCTION: 0x486fd0
void __stdcall LoadUnits(HapiBank* file)
{
    if (file->OpenAccount("Units")) {
        if (((HapiBank*)file)->GetIntegerItem("Version", 0) == 0x11) {
            int n = ((HapiBank*)file)->GetIntegerItem("Number of Units", 0);
            for (int i = 0; i < n; i++) {
                if (((HapiBank*)file)->OpenNumberedBox(i)) {
                    ((HapiBank*)file)->SeekBox(0);
                    UnitRecord_00486fd0 rec;
                    int len = ((HapiBank*)file)->ReadBox(&rec, 0xb8);
                    if (len != 0xb8) {
                        if (len + 2 != 0xb8)
                            continue;
                        rec.id = 0;
                    }
                    LoadUnit(rec.id, file);
                }
            }
        }
    }
}
