// Decompiled by Space Bunny Free. Names are provisional.
#include <ctype.h>
#include <string.h>

class Class_004b7440 {
public:
    char* field_0[0x14];               // +0x00  token pointers
    char field_50[0x7e];               // +0x50  token text buffer
    char unknown_ce;                   // +0xce
    int field_d0;                      // +0xd0  token count
    void Tokenize(char* text, char* end);
};

// FUNCTION: 0x4b7440
void Class_004b7440::Tokenize(char* text, char* end)
{
    char* p = field_50;

    if (end == 0)
        end = text + strlen(text);

    field_d0 = 0;

    while (1) {
        while (text != end && isspace(*text))
            text++;

        if (text == end)
            return;

        if (*text == '#')
            return;

        if (field_d0 < 0x14) {
            field_0[field_d0] = p;
            field_d0++;
        }

        while (text != end) {
            if (isspace(*text))
                break;
            if (*text == '#')
                break;
            if (p >= &field_50[0x7e])
                break;
            *p = *text;
            p++;
            text++;
        }

        *p = 0;
        p++;
    }
}
