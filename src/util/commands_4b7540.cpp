// Decompiled by Opus. Names are provisional.

// Command arguments (see 0x4b74f0): drops the first n arguments.
class Class_004b7540 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0

    void FUN_004b7540(int n);
};

// FUNCTION: 0x4b7540
void Class_004b7540::FUN_004b7540(int n)
{
    if (count >= n) {
        count = 0;
        return;
    }
    char** end = &args[count];
    char** p = &args[n];
    char** dst = args;
    while (p != end)
        *dst++ = *p++;
    count -= n;
}
