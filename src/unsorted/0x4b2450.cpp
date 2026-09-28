// Decompiled by deepseek-v4.1-flash. Names are provisional.
struct Data_004b2450 {
    int unknown_0;    // +0x00
    int count_1;      // +0x04
    int count_2;      // +0x08
    int unknown_c;    // +0x0c
    int checksum;     // +0x10
    int count_3;      // +0x14
    int offset_18;    // +0x18
    int offset_1c;    // +0x1c
    int offset_20;    // +0x20
    int offset_24;    // +0x24
    int offset_28;    // +0x28
};

struct Pair8_004b2450 {
    int unknown_0;    // +0x00
    int offset;       // +0x04
};

struct Pair_004b2450 {
    int key;          // +0x00
    int value;        // +0x04
};

struct Pairib_004b2450 {
    int iterator;     // +0x00
    int inserted;     // +0x04
};

class Class_004b2850 {
public:
    void FUN_004b2850(Pairib_004b2450* out, Pair_004b2450* value);
};

extern char DAT_0051fbc0;

extern Data_004b2450* __stdcall FUN_004bbe50(char* name, int reserved);
extern int __stdcall FUN_004bbc40(char* name);
extern int __stdcall FUN_004b6ba0(unsigned char* data, int len);

// The original's map lookup (`DAT_0051fbc0[(int)data] = sum`) inlined
// map::operator[] but left _Tree::insert (0x4b2850) out of line, so the
// insert is called explicitly here instead of through <map>.
// FUNCTION: 0x4b2450
Data_004b2450* __stdcall FUN_004b2450(char* name)
{
    Data_004b2450* data = FUN_004bbe50(name, 0);
    if (data == 0)
        return 0;
    int sum = FUN_004b6ba0((unsigned char*)data, FUN_004bbc40(name));
    Pair_004b2450 value;
    Pairib_004b2450 out;
    value.key = (int)data;
    value.value = 0;
    ((Class_004b2850*)&DAT_0051fbc0)->FUN_004b2850(&out, &value);
    ((int*)out.iterator)[4] = sum;
    data->offset_18 += (int)data;
    data->offset_1c += (int)data;
    for (int i = 0; i < data->count_1; i++)
        ((int*)data->offset_1c)[i] += (int)data;
    data->offset_20 += (int)data;
    for (int j = 0; j < data->count_2; j++)
        ((int*)data->offset_20)[j] += (int)data;
    data->offset_24 += (int)data;
    data->offset_28 += (int)data;
    for (int k = 0; k < data->count_3; k++)
        ((Pair8_004b2450*)data->offset_28)[k].offset += (int)data;
    return data;
}
