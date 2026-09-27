// Decompiled by Opus. Names are provisional.
struct BlinkWord_004afc60 {
    char text[0xa0];                   // +0x0
    int value;                         // +0xa0
};

#pragma pack(push, 2)
struct Class_004afc60 {
    char unknown_0[0xa6];
    BlinkWord_004afc60* words;         // +0xa6
    int count;                         // +0xaa
    int active;                        // +0xae
};
#pragma pack(pop)

void* FUN_004d83b0(char* name, unsigned int size);
void FUN_004d85a0(void* p);

// FUNCTION: 0x4afc60
void __stdcall FUN_004afc60(Class_004afc60* obj, int count)
{
    if (obj->words) {
        FUN_004d85a0(obj->words);
        obj->words = 0;
    }
    obj->words = (BlinkWord_004afc60*)FUN_004d83b0("BlinkWords", count * sizeof(BlinkWord_004afc60));
    for (int i = 0; i < count; i++)
        obj->words[i].text[0] = 0;
    obj->count = count;
    obj->active = 1;
    obj->words[0].value = -1;
}
