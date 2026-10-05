// Decompiled by Haiku. Names are provisional.

struct CommandArgs {
    int array[0x34];  // +0x0 to +0xcf
    int size;         // +0xd0

    int GetArg(int index, int default_val);
};

// FUNCTION: 0x4b73c0
int CommandArgs::GetArg(int index, int default_val)
{
    if (index < 0 || index >= size) {
        return default_val;
    }
    return array[index];
}
