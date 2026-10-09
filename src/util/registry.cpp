// Decompiled by Opus, Haiku, Space Bunny Free, GPT-6.1-sol, mimo-v2.6-pro and Sonnet. Names are provisional.
// The game's values under Software\Cavedog Entertainment, with the rectangle
// tests and the thread, sleep and display helpers that share this unit.
#include <windows.h>
#include <string.h>

struct Rect_004b6750 {
    int x1;                            // +0x0
    int y1;                            // +0x4
    int x2;                            // +0x8
    int y2;                            // +0xc
};

int __stdcall AccessRegistryValue(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                                  DWORD type, DWORD read);

// Returns 1 when rectangle a lies entirely inside rectangle b.
// FUNCTION: 0x4b6750
int __stdcall RectInsideRect(Rect_004b6750* a, Rect_004b6750* b)
{
    if (a->x1 < b->x1) {
        return 0;
    }
    if (a->x1 > b->x2) {
        return 0;
    }
    if (a->x2 < b->x1) {
        return 0;
    }
    if (a->x2 > b->x2) {
        return 0;
    }
    if (a->y1 < b->y1) {
        return 0;
    }
    if (a->y1 > b->y2) {
        return 0;
    }
    if (a->y2 < b->y1) {
        return 0;
    }
    if (a->y2 > b->y2) {
        return 0;
    }
    return 1;
}

// Returns 1 when rectangles a and b overlap.
// FUNCTION: 0x4b67d0
int __stdcall RectsOverlap(Rect_004b6750* a, Rect_004b6750* b)
{
    if (a->x1 > b->x2) {
        return 0;
    }
    if (a->x2 < b->x1) {
        return 0;
    }
    if (a->y1 > b->y2) {
        return 0;
    }
    return a->y2 >= b->y1 ? 1 : 0;
}

// FUNCTION: 0x4b6820
int __stdcall CompareByte1Equal(int param_1, int param_2)
{
    int cl = *(char*)(param_1 + 1);
    int bl = *(char*)(param_2 + 1);
    return cl == bl ? 1 : 0;
}

// FUNCTION: 0x4b6840
void __stdcall SetBytes1To3(unsigned char* buf, unsigned char b2, unsigned char b3, unsigned char b4)
{
    buf[1] = b2;
    buf[2] = b3;
    buf[3] = b4;
}

// Reads a registry value through AccessRegistryValue (see ReadGameRegistryValue).
// FUNCTION: 0x4b6860
int __stdcall ReadRegistryValue(const char* app, const char* key, void* buf, unsigned int* size)
{
    return AccessRegistryValue((char*)app, (char*)key, (LPBYTE)buf, (LPDWORD)size, 0, 1);
}

// The read path accepts ERROR_SUCCESS and ERROR_MORE_DATA (0xea).
// FUNCTION: 0x4b6880
int __stdcall AccessRegistryValue(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                           DWORD type, DWORD read)
{
    // Zeroed in this order: HKEYs with `= 0`, result by a later statement.
    int result;
    HKEY key1 = 0;
    HKEY key2 = 0;
    HKEY key3 = 0;
    REGSAM samDesired = read ? KEY_READ : KEY_WRITE;
    int doRead = read;
    // lpdwDisposition for the three calls. Left uninitialised on purpose: the
    // key is never read back, and initialising it costs a store (301 bytes).
    DWORD disp;
    // Call results go through a LONG before the compare.
    LONG err;
    result = 0;
    err = RegCreateKeyExA(HKEY_CURRENT_USER, "Software", 0, 0, 0, samDesired, 0,
                          &key1, &disp);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                          &key2, &disp);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &disp);
    if (err != 0)
        goto close;
    if (doRead) {
        err = RegQueryValueExA(key3, valueName, 0, 0, data, size);
        if (err != 0 && err != ERROR_MORE_DATA)
            goto close;
    } else {
        err = RegSetValueExA(key3, valueName, 0, type, data, *size);
        if (err != 0)
            goto close;
    }
    result = 1;
close:
    if (key3 != 0) {
        RegCloseKey(key3);
    }
    if (key2 != 0) {
        RegCloseKey(key2);
    }
    if (key1 != 0) {
        RegCloseKey(key1);
    }
    return result;
}

// FUNCTION: 0x4b69b0
int __stdcall ReadRegistryData(void* param_1, void* param_2, void* param_3, void* param_4)
{
    return AccessRegistryValue((char*)param_1, (char*)param_2, (LPBYTE)param_3, (LPDWORD)param_4, 0, 1);
}

// Reads a 4-byte value through AccessRegistryValue (same family as 0x4b69b0).
// FUNCTION: 0x4b69d0
int __stdcall ReadRegistryDword(void* param_1, void* param_2, void* param_3)
{
    int size = 4;
    return AccessRegistryValue((char*)param_1, (char*)param_2, (LPBYTE)param_3, (LPDWORD)&size, 0, 1);
}

// FUNCTION: 0x4b6a00
void __stdcall WriteRegistryBinary(void* param_1, void* param_2, void* param_3, int unused)
{
    AccessRegistryValue((char*)param_1, (char*)param_2, (LPBYTE)param_3, (LPDWORD)&unused, 3, 0);
}

// Stores a string value (type 1) through AccessRegistryValue, passing its length
// including the terminator.
// FUNCTION: 0x4b6a20
void __stdcall WriteRegistryString(void* param_1, void* param_2, char* value)
{
    unsigned int size = strlen(value) + 1;
    AccessRegistryValue((char*)param_1, (char*)param_2, (LPBYTE)value, (LPDWORD)&size, 1, 0);
}

// Stores a DWORD value (type 4) through AccessRegistryValue, compare 0x4b6a20.
// FUNCTION: 0x4b6a50
void __stdcall WriteRegistryDword(char* key, char* name, int value)
{
    unsigned int size = 4;
    AccessRegistryValue(key, name, (LPBYTE)&value, (LPDWORD)&size, 4, 0);
}

// FUNCTION: 0x4b6a80
int __stdcall GetWindowsUserName(char* out)
{
    DWORD size;
    char name[256];
    size = 256;
    if (GetUserNameA(name, &size) && name[0] != 0) {
        strcpy(out, name);
        return 1;
    }
    return 0;
}

// The loop test was an inlined helper with one return per outcome. Its
// multi-block body stops MSVC from rotating the loop (moving the test to the
// bottom) and from merging the two identical "next line" branches; written
// as a plain `while (n != lines)` the loop is rotated and the branches merged.
static inline int NotDone(int wanted, int current)
{
    if (wanted == current) {
        return 0;
    }
    return 1;
}

// Returns a pointer to the start of line `n` of `text`; lines end at '\n' or '\0'.
// FUNCTION: 0x4b6af0
char* __stdcall SkipTextLines(char* text, int n)
{
    int i = 0;
    int lines = 0;
    while (NotDone(n, lines)) {
        if (text[i] == 0) {
            lines++;
            i++;
        } else if (text[i] == '\n') {
            lines++;
            i++;
        } else {
            i++;
        }
    }
    return text + i;
}

extern unsigned int __cdecl _beginthread(void* start, unsigned int stack, void* param);

// FUNCTION: 0x4b6b20
int __stdcall StartThread(void* param_1, unsigned int param_2, void* param_3)
{
    unsigned int result = _beginthread(param_1, param_2, param_3);
    if (result != (unsigned int)-1) {
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4b6b50
void __stdcall SleepMilliseconds(unsigned int param_1)
{
    Sleep(param_1);
}

extern void* g_display;

// FUNCTION: 0x4b6b60
void __stdcall SetMediaNotifyCallback(int param_1)
{
    *(int*)((char*)g_display + 0x80) = param_1;
}
