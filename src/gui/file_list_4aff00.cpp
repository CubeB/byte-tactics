// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Entry_004a1810;

// The object at obj+0x18; +4 points at the entry array.
struct Obj18_004aff00 {
    char unknown_0[4];
    char* field_4;                     // +0x4
};

#pragma pack(push, 1)
struct Word_004aff00 {                 // 0xa4 bytes
    char text[0x80];                   // +0x0
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

struct Class_004aff00 {
    char unknown_0[0x18];
    Obj18_004aff00* field_18;          // +0x18
    char unknown_1c[0xa6 - 0x1c];
    Word_004aff00* words;              // +0xa6
    int count;                         // +0xaa
    int active;                        // +0xae
};
#pragma pack(pop)

unsigned int GetTicks();
int GetTickRate();
int GetTextKeyColor();
void __stdcall FUN_004a1810(Entry_004a1810* entries, int index);
void __stdcall SetTextColors(int param_1, int param_2);
void __stdcall DrawString(void* surface, const char* text, int x, int y,
                            int maxWidth);

// FUNCTION: 0x4aff00
void __stdcall FUN_004aff00(Class_004aff00* obj)
{
    if (obj->active == 0)
        return;

    int time = GetTicks();

    if (obj->words->value != -1)
        FUN_004a1810((Entry_004a1810*)obj->field_18->field_4,
                     obj->words->value);

    for (int i = 0; i < obj->count; i++) {
        if (obj->words[i].text[0] == 0)
            continue;

        float ft = (float)time;
        if (obj->words[i].field_9c < ft) {
            if (obj->words[i].field_98 != 0) {
                obj->words[i].field_9c =
                    (float)GetTickRate() * obj->words[i].field_8c + ft;
                obj->words[i].field_98 = 0;
            } else {
                obj->words[i].field_9c =
                    (float)GetTickRate() * obj->words[i].field_94 + ft;
                obj->words[i].field_98 = 1;
            }
        }

        if (obj->words[i].field_98 != 0)
            SetTextColors(obj->words[i].field_90, GetTextKeyColor());
        else
            SetTextColors(obj->words[i].field_88, GetTextKeyColor());

        DrawString((void*)*(int*)(obj->field_18->field_4 + 0xbc),
                     obj->words[i].text, obj->words[i].field_80,
                     obj->words[i].field_84, -1);
    }
}
