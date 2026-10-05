// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_00480020 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

class SpeechQueue {
public:
    Entry_00480020 entries[9];         // +0x0
    int count;                         // +0x99

    void RemoveSpeechOfId(int id);
};
#pragma pack(pop)

// FUNCTION: 0x480020
void SpeechQueue::RemoveSpeechOfId(int id)
{
    int i = 0;
    while (i < count) {
        if (entries[i].field_8 == id) {
            if (entries[i].data) {
                FUN_004d85a0(entries[i].data);
                entries[i].data = 0;
            }
            for (int j = i; j < count; j++)
                entries[j] = entries[j + 1];
            count--;
        } else {
            i++;
        }
    }
}
