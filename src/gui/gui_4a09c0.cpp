// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a09c0 {                // 0x15b bytes
    unsigned char state;               // +0x00
    char unknown_1[0x10 - 1];          // name at +0x02
    char unknown_11[0xb6 - 0x10];
    char text[0x80];                   // +0xb6
    unsigned char count;               // +0x136
    char unknown_137;
    short value;                       // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Data_004a09c0 {
    int unknown_0;
    Entry_004a09c0* entries;           // +0x04
};

struct Context_004a09c0 {
    char unknown_0[0x18];
    Data_004a09c0* data;               // +0x18
    char unknown_1c[0x64 - 0x1c];
    int current;                       // +0x64
    char unknown_68[0x74 - 0x68];
    int length;                        // +0x74
    char unknown_78[0xcca - 0x78];
    int changed;                       // +0xcca
};
#pragma pack(pop)

char* __stdcall Translate(void*);
void __stdcall FUN_004a05e0(Context_004a09c0*, int);

// FUNCTION: 0x4a09c0
void __stdcall FUN_004a09c0(Context_004a09c0* context, int index, char* source, int value)
{
    if (index == -1 || context->data == 0)
        return;
    Entry_004a09c0* entries = context->data->entries;
    char* text = Translate(source);

    switch (entries[index].state) {
    case 5:
        strncpy(entries[index].text, text, 0x80);
        if (entries[index].count != 0)
            FUN_004a05e0(context, index);
        break;
    case 3:
        if (text != 0) {
            strcpy(context->data->entries[index].text, text);
            if (context->current == index)
                context->length = strlen(text);
        }
        if (value != 0)
            entries[index].value = (short)value;
        break;
    case 1:
        strncpy(entries[index].text, text, 0x80);
        FUN_004a05e0(context, index);
        if (entries[index].count != 0) {
            char* p = entries[index].text;
            while (*p != 0) {
                if (*p == '|')
                    *p = 0;
                p++;
            }
            Entry_004a09c0* entry = &context->data->entries[index];
            char temp[0x80];
            char* dst = temp;
            char* src = entry->text;
            int i = 0;
            while (i < entry->count) {
                strcpy(dst, Translate(src));
                dst += strlen(dst) + 1;
                src += strlen(src) + 1;
                i++;
            }
            memcpy(entry->text, temp, 0x80);
        }
        break;
    }
    context->changed = 1;
}
