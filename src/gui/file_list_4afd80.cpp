// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Word_004afd80 {                 // 0xa4 bytes
    char text[0x80];                   // +0x00
    int field_80;                      // +0x80
    int field_84;                      // +0x84
    int field_88;                      // +0x88
    float field_8c;                    // +0x8c
    int field_90;                      // +0x90
    float field_94;                    // +0x94
    int field_98;                      // +0x98
    float field_9c;                    // +0x9c
    int value;                         // +0xa0
};

struct Dialog {
    char unknown_0[0xa6];
    Word_004afd80* words;              // +0xa6
    int count;                         // +0xaa
    int active;                        // +0xae
};
#pragma pack(pop)

int GetTickRate();
unsigned int GetTicks();

// FUNCTION: 0x4afd80
int __stdcall FUN_004afd80(Dialog* obj, const char* text, int p2, int p3,
                           int p4, int p5, float f6, float f7)
{
    Word_004afd80* p = obj->words;
    if (p == 0)
        return 0;

    int count = obj->count;
    int i = 0;
    for (i = 0; i < count; i++, p++) {
        if (p->text[0] == 0)
            break;
    }
    if (i == count)
        return 0;

    int time = GetTicks();

    strncpy(obj->words[i].text, text, 0x80);
    obj->words[i].field_80 = p2;
    obj->words[i].field_84 = p3;
    obj->words[i].field_88 = p4;
    obj->words[i].field_90 = p5;
    obj->words[i].field_8c = f6;
    obj->words[i].field_94 = f7;

    int rate = GetTickRate();
    obj->words[i].field_9c = (float)rate * f6 + (float)time;
    obj->words[i].field_98 = 0;
    return 1;
}
