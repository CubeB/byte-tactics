// Decompiled by Haiku. Names are provisional.

class CommandArgs {
public:
    char unknown_0[0xd0];
    int field_d0;

    CommandArgs* InitArgs();
};

// FUNCTION: 0x4b73b0
CommandArgs* CommandArgs::InitArgs()
{
    field_d0 = 0;
    return this;
}
