// Decompiled by Haiku. Names are provisional.

struct FileHandle
{
public:
    char unknown_0[4];
    int field_4;
};

// FUNCTION: 0x4bb650
bool __stdcall HAPI_IsInArchive(FileHandle* obj)
{
    return obj->field_4 != 0;
}
