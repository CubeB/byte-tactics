// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Reloads the buffer at +0xc14 from the file named by the text at +0x304:
// frees the old buffer, sizes the file, allocates size+1 bytes and loads it.
// When the text is empty the buffer is cleared instead.
#include <string.h>

int __stdcall FUN_004bbc40(char* path);
void __stdcall FUN_004bbd30(char* filename, void* buffer, int offset, int size);
void* __cdecl FUN_004d83b0(const char* tag, int size);
void __cdecl FUN_004d85a0(void* p);

class Class_00435320 {
public:
    char unknown_0[0x304];
    char text_304[0x910];              // +0x304
    char* field_c14;                   // +0xc14

    void LoadBriefing();
};

// FUNCTION: 0x435320
void Class_00435320::LoadBriefing()
{
    if (field_c14)
        FUN_004d85a0(field_c14);
    char* name = strlen(text_304) > 0 ? text_304 : 0;
    if (name == 0) {
        field_c14 = 0;
        return;
    }
    int size = FUN_004bbc40(name);
    if (size != 0) {
        field_c14 = (char*)FUN_004d83b0("Briefing", size + 1);
        FUN_004bbd30(name, field_c14, 0, size);
        field_c14[size] = 0;
    }
}
