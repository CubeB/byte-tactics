// Decompiled by Opus. Names are provisional.
// Walks a buffer of variable-sized records {id, size, ...} and clears the id
// of every record matching `id`.

struct Record_00437c90 {
    int id;                            // +0x0
    int size;                          // +0x4
};

class CMemoryCache {
public:
    int length;                        // +0x0
    Record_00437c90* records;          // +0x4

    void ReleaseHandle(int id);
};

// FUNCTION: 0x437c90
void CMemoryCache::ReleaseHandle(int id)
{
    Record_00437c90* r = records;
    for (int offset = 0; offset < length; ) {
        offset += r->size;
        if (r->id == id) {
            r->id = 0;
        }
        r = (Record_00437c90*)((char*)r + r->size);
    }
}
