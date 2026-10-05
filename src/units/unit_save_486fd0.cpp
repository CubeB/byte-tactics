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
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

class Class_004b4b50 {
public:
    int OpenNumberedBox(int a);
};

class Class_004b4c10 {
public:
    void SeekBox(int pos);
};

class Class_004b4c80 {
public:
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
        if (((Class_004b4800*)file)->GetIntegerItem("Version", 0) == 0x11) {
            int n = ((Class_004b4800*)file)->GetIntegerItem("Number of Units", 0);
            for (int i = 0; i < n; i++) {
                if (((Class_004b4b50*)file)->OpenNumberedBox(i)) {
                    ((Class_004b4c10*)file)->SeekBox(0);
                    UnitRecord_00486fd0 rec;
                    int len = ((Class_004b4c80*)file)->ReadBox(&rec, 0xb8);
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
