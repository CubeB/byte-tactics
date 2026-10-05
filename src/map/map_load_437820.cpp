// Decompiled by Haiku, rewritten by Opus. Names are provisional.
// Copy constructor of a {string handle, int} pair.

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class MapCacheEntry {
public:
    Class_004c91a0 handle;
    int field_4;

    MapCacheEntry(const MapCacheEntry& other);
};

// FUNCTION: 0x437820
MapCacheEntry::MapCacheEntry(const MapCacheEntry& other)
    : handle(other.handle), field_4(other.field_4)
{
}
