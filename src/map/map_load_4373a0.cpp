// Decompiled by space-bunny-free. Names are provisional.
// Map cache lookup for the campaign object (`this` is the campaign at
// g_game+0x391e9, the class 0x435c00.cpp calls Class_00435c00). +0xc1c is
// both the running checksum and the "already loaded" flag, so a second call
// returns straight away. Otherwise name slot 1 is searched in the file-local
// vector of {name, checksum} pairs at 0x5122c0 (its initialiser and atexit
// destructor are 0x434a30.cpp, its out-of-line insert is 0x437580.cpp): a hit
// takes the stored checksum, a miss opens the file, checks the 0x2000 magic of
// its 0x40-byte header and folds the checksums (0x4b6ba0) of the header, of
// the plot data (width * height * 4 bytes at the offset in +0x10) and of the
// feature data (features * 0x84 bytes at the offset in +0x20) into +0xc1c,
// then appends the new pair. The result is +0xc20 xor +0xc1c throughout.
//
// The two vector pointers are declared as globals in their own right so that
// each reference carries the address the original uses: through a single
// std::vector both would be DAT_005122c0 with a displacement, which check.py
// reports as a reference to the wrong address.
//
// SetChecksum is not a real method: it is how the new entry gets +0xc1c. The
// store happens after the entry's copy constructor, not inside it, so giving
// the element a two-argument constructor makes MSVC hoist the load of +0xc1c
// ahead of the call and keeps it in a callee-saved register, which costs the
// match.
#include <string.h>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

// The reference-counted string handle: 0x4c91a0 is its copy constructor and
// 0x4c9390 its release.
class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

// 0x4c91b0 builds one of those handles from a C string. In the game this is a
// second constructor of the same class, which data/symbols.csv cannot hold
// twice, so it is a derived class here.
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
};

class Class_004373a0 {
public:
    int type;                          // +0x0
    char campaign[0x100];              // +0x4
    char names[9][0x100];              // +0x104
    char unknown_a04[0xc18 - 0xa04];
    int missionIndex;                  // +0xc18
    int field_c1c;                     // +0xc1c
    int field_c20;                     // +0xc20

    int ComputeMapChecksum();

    // 0x435320, inlined
    char* GetName(int index)
    {
        char* ptr = (char*)this + index * 0x100 + 0x104;
        if (strlen(ptr) > 0)
            return ptr;
        return 0;
    }
};

class Class_00437820 {
public:
    Class_004c91a0 handle;             // +0x0
    int field_4;                       // +0x4

    Class_00437820(const Class_004c91a0& other) : handle(other) {}

    Class_00437820& SetChecksum(Class_004373a0* self)
    {
        field_4 = self->field_c1c;
        return *this;
    }
};

// The vector at 0x5122c0: MSVC 5 puts the empty allocator byte at +0, so
// _First is DAT_005122c4 and _Last is DAT_005122c8.
extern Class_00437820* DAT_005122c4;
extern Class_00437820* DAT_005122c8;

class Class_00437580 {
public:
    char unknown_0[4];
    void FUN_00437580(Class_00437820* pos, int count, const Class_00437820& val);
};

extern Class_00437580 DAT_005122c0;

struct Header_004373a0 {
    int magic;                         // +0x00
    int width;                         // +0x04
    int height;                        // +0x08
    int unknown_0c;                    // +0x0c
    int plotOffset;                    // +0x10
    int unknown_14;                    // +0x14
    int unknown_18;                    // +0x18
    int features;                      // +0x1c
    int featureOffset;                 // +0x20
    char unknown_24[0x40 - 0x24];
};

void* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);
int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb5d0(void* file);
int __stdcall FUN_004b6ba0(unsigned char* data, int len);
void* __cdecl FUN_004d83b0(const char* tag, int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4373a0
int Class_004373a0::ComputeMapChecksum()
{
    if (field_c1c != 0) {
        return field_c20 ^ field_c1c;
    }
    char* name = GetName(1);
    Class_00437820* it;
    for (it = DAT_005122c4; it != DAT_005122c8; it++) {
        if (_strcmpi(it->handle.data, name) == 0) {
            field_c1c = it->field_4;
            return field_c20 ^ field_c1c;
        }
    }
    void* file = FUN_004bb5b0(name);
    if (file == 0) {
        return 0;
    }
    Header_004373a0 header;
    FUN_004bb7c0(file, &header, 0x40);
    if (header.magic != 0x2000) {
        return 0;
    }
    field_c1c = field_c1c ^ FUN_004b6ba0((unsigned char*)&header, 0x40);
    int size = header.width * header.height * 4;
    void* data = FUN_004d83b0("Raw Plot Data", size);
    FUN_004bb710(file, header.plotOffset);
    FUN_004bb7c0(file, data, size);
    field_c1c = field_c1c ^ FUN_004b6ba0((unsigned char*)data, size);
    FUN_004d85a0(data);
    size = header.features * 0x84;
    if (size > 0) {
        data = FUN_004d83b0("Raw Feature Data", size);
        FUN_004bb710(file, header.featureOffset);
        FUN_004bb7c0(file, data, size);
        field_c1c = field_c1c ^ FUN_004b6ba0((unsigned char*)data, size);
        FUN_004d85a0(data);
    }
    FUN_004bb5d0(file);
    DAT_005122c0.FUN_00437580(DAT_005122c8, 1, Class_00437820(Class_004c91b0(name)).SetChecksum(this));
    return field_c20 ^ field_c1c;
}
