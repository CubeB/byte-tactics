// Decompiled by Haiku, rewritten by Opus. Names are provisional.
// Copy constructor of a {string handle, int, int} record.

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class CommandEntry {
public:
    Class_004c91a0 handle;
    int field_4;
    int field_8;

    CommandEntry(const CommandEntry& other);
};

// FUNCTION: 0x4b7e30
CommandEntry::CommandEntry(const CommandEntry& other)
    : handle(other.handle), field_4(other.field_4), field_8(other.field_8)
{
}
