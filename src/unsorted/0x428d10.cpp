// Decompiled by Space Bunny Free. Names are provisional.
// AI profile tokenizer. The number scan is written as
// `char* q = field_88; field_88 = q + 1;` rather than `*++field_88` or a bare
// `field_88++` statement: the two-register form (increment in edx, then a
// `mov eax, edx` copy for the load) is what MSVC emits for every pre-increment
// spelling, while going through a pointer local that is stored back to the
// member makes it increment and store in eax directly, as the original does.
#include <ctype.h>

class Class_00428d10 {
public:
    char token[0x80];                   // +0
    char* field_80;                     // +0x80
    char* field_84;                     // +0x84
    char* field_88;                     // +0x88
    int field_8c;                       // +0x8c
    int field_90;                       // +0x90
    int field_94;                       // +0x94
    int FUN_00428d10();
};

// FUNCTION: 0x428d10
int Class_00428d10::FUN_00428d10()
{
    if (field_8c != 0)
        return 0x102;

    while (isspace(*field_88) && field_88 != field_84)
        field_88++;

    if (field_88 == field_84 || *field_88 == '\0')
        return 0;

    if (ispunct(*field_88))
        return *field_88++;

    int len = 0;
    if (!isdigit(*field_88) && *field_88 != '-' && *field_88 != '.') {
        while (isalnum(*field_88) && field_88 != field_84 && len < 0x7f) {
            token[len] = *field_88;
            len++;
            field_88++;
        }
        token[len] = 0;
        if (len != 0 && len != 0x7f)
            return 0x100;
        return 0x102;
    }

    token[0] = *field_88;
    int i = 1;
    for (;;) {
        char* q = field_88;
        field_88 = q + 1;
        if (!(isdigit(*field_88) || *field_88 == '.'))
            break;
        token[i++] = *field_88;
    }
    token[i] = 0;
    return 0x101;
}
