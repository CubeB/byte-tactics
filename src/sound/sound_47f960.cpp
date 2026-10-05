// Decompiled by Sonnet. Names are provisional.
// Copies ecx (this) into eax up front and returns it: a constructor.

#pragma pack(push, 1)
class SpeechQueue {
public:
    char unknown_0[0x99];
    int field_99;
    int field_9d;
    int field_a1;
    int field_a5;

    SpeechQueue(int param_1, int param_2);
};
#pragma pack(pop)

// FUNCTION: 0x47f960
SpeechQueue::SpeechQueue(int param_1, int param_2)
{
    field_99 = 0;
    field_9d = 0;
    field_a1 = param_1;
    field_a5 = param_2;
}
