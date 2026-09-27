// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Entry_004a7ee0 {                 // 0x15b bytes
    char unknown_0[0x15b];
};
#pragma pack(pop)

// FUNCTION: 0x4a7ee0
void __stdcall FUN_004a7ee0(Entry_004a7ee0* entries, int index)
{
    Entry_004a7ee0 temp;
    if (index != -1) {
        temp = entries[index];
        for (int i = index; i > 1; i--) {
            entries[i] = entries[i - 1];
        }
        entries[1] = temp;
    }
}
