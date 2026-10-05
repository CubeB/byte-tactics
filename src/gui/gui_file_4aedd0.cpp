// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads the GAF named by `name` (prefixed with obj->dir) and stores the loaded
// GAF in obj->items[index], after shifting every frame by the offset of the
// glyph/entry 0x49. obj->field_14 receives the same GAF pointer. When the file
// does not exist, obj->items[index] is zeroed.
#include <string.h>

struct Table_004aedd0 {
    unsigned short count;              // +0x0
    char unknown_2[0x28 - 0x2];
    void* entries[1];                  // +0x28, 8-byte stride
};

struct Object_004aedd0 {
    char unknown_0[8];
    void* items[1];                    // +0x8
    char unknown_c[0x14 - 0xc];
    void* field_14;                    // +0x14
    char unknown_18[0xab6 - 0x18];
    char dir[0x100];                   // +0xab6
};

void __stdcall ChangeExtension(char* out, char* in, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void* __stdcall LoadGaf(char* path);
void* __stdcall GetGafFrame(void* table, int index);

// FUNCTION: 0x4aedd0
void __stdcall LoadGafIntoSlot(Object_004aedd0* obj, char* name, int index)
{
    // Suspected original bug: this local is never assigned, and the original
    // reads it at 0x4aee78 (mov ebx,[esp+0x10]) when the table has no entry
    // 0x49. Reproducing the read is required for a byte match.
    int unknown;
    char path[256];
    strncpy(path, obj->dir, 0x100);
    strcat(path, name);
    ChangeExtension(path, path, "GAF");
    if (HAPI_FileLengthByName(path)) {
        void* gaf = LoadGaf(path);
        obj->items[index] = gaf;
        Table_004aedd0* table = *(Table_004aedd0**)((char*)gaf + 0xc);
        char* e = (char*)GetGafFrame(table, 0x49);
        int d;
        if (e)
            d = *(unsigned short*)(e + 2);
        else
            d = unknown;
        for (int i = 0; i < table->count; i++) {
            char* f = (char*)GetGafFrame(table, i);
            if (f)
                *(short*)(f + 6) -= d;
        }
        obj->field_14 = obj->items[index];
    } else {
        obj->items[index] = 0;
        return;
    }
}
