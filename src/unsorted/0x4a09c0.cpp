// Decompiled by GPT-5.6-Terra. Names are provisional.
// Partial: 76.1%. The original keeps the entry-array base and byte offset in
// separate registers through the string-building loop; this version materializes
// the entry pointer, changing the later stack and register allocation.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a09c0 {
    unsigned char state;
    char unknown_1[0xb6 - 1];
    char text[0x80];
    unsigned char count;
    char unknown_137;
    short value;
    char unknown_13a[0x15b - 0x13a];
};

struct List_004a09c0 {
    char unknown_0[4];
    Entry_004a09c0* entries;
};

struct Context_004a09c0 {
    char unknown_0[0x18];
    List_004a09c0* list;
    char unknown_1c[0x64 - 0x1c];
    int selected;
    char unknown_68[0x74 - 0x68];
    int text_length;
    char unknown_78[0xcca - 0x78];
    int changed;
};
#pragma pack(pop)

char* __stdcall FUN_004c5740(void*);
void __stdcall FUN_004a05e0(Context_004a09c0*, int);

// FUNCTION: 0x4a09c0
void __stdcall FUN_004a09c0(Context_004a09c0* context, int index, void* source, int value)
{
    if (index != -1 && context->list != 0) {
        char* entries = (char*)context->list->entries;
        char* text = FUN_004c5740(source);
        int offset = index * sizeof(Entry_004a09c0);
#define ENTRY ((Entry_004a09c0*)(entries + offset))
        switch (ENTRY->state) {
        case 5:
            strncpy(ENTRY->text, text, 0x80);
            if (ENTRY->count != 0)
                FUN_004a05e0(context, index);
            break;
        case 3:
            if (text != 0) {
                strcpy(ENTRY->text, text);
                if (context->selected == index)
                    context->text_length = strlen(text);
            }
            if (value != 0)
                ENTRY->value = (short)value;
            break;
        case 1:
            strncpy(ENTRY->text, text, 0x80);
            FUN_004a05e0((Context_004a09c0*)value, index);
            if (ENTRY->count != 0) {
                for (char* p = ENTRY->text; *p != 0; p++) {
                    if (*p == '|')
                        *p = 0;
                }
                char buffer[0x80];
                char* dst = buffer;
                char* src = ENTRY->text;
                for (int i = 0; i < ENTRY->count; i++) {
                    char* part = FUN_004c5740(src);
                    strcpy(dst, part);
                    dst += strlen(dst) + 1;
                    src += strlen(src) + 1;
                }
                memcpy(ENTRY->text, buffer, 0x80);
            }
            break;
        }
#undef ENTRY
        context->changed = 1;
    }
}
