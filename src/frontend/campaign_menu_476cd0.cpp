// Decompiled by space-bunny-free. Names are provisional.
#include <string.h>

void* __cdecl FUN_004d83b0(const char* tag, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x476cd0
char* __stdcall FUN_00476cd0(char* s)
{
    char* buf = (char*)FUN_004d83b0("TempStorage", strlen(s) + 0x32);
    int inquote = 0;
    memset(buf, 0, strlen(s) + 0x32);
    unsigned char* p = (unsigned char*)s;
    if (*p) {
        char* d = buf;
        while (1) {
            char quote;
            if (*p == 0xff)
                break;
            *d = *p;
            if (*p == '&') {
                quote = p[1];
                inquote ^= 1;
            }
            if (*p == '\n' && inquote == 1) {
                d[-1] = '&';
                *d++ = '\r';
                *d++ = '\n';
                *d++ = '&';
                *d = quote;
            }
            d++;
            p++;
            if (*p == 0)
                break;
        }
    }
    FUN_004d85a0(s);
    return buf;
}
