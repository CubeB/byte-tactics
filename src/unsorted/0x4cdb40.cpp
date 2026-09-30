// Decompiled by deepseek-v4.1-flash, finished by GPT-6, continued by GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// Partial (75.4%, 1268 bytes against the original 1248). Everything still
// missing has one cause: the case 0 zero has to land in EBX. The original keeps
// it there across the strcmp, so EAX is free and the DAT_0050b540 = -1
// materialises in EAX, which makes the release tail of case 0 and of the mode 4
// block identical, so the two merge into the single copy at 0x4cdcaa. MSVC 5
// gives our case 0 zero ESI instead and re-materialises it, EAX is then free
// for the -1 but EBX is taken by it, the two release tails stop being
// identical and nothing merges: 20 bytes too long. Also still open: the
// mciSendStringA(...) == 0 tests fold to test eax,eax where the original has
// cmp eax, edi, which wants the zero to be a real variable, not a constant.
// The zero in EDI and the one in EBX do match now.
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>

extern int DAT_0050b540;
extern int DAT_0050b544;

extern int __stdcall FUN_004b64d0(int);

class Class_004ceb60 {
public:
    int FUN_004ceb60(int index, int flag);
};

class Class_004d00d0 {
public:
    int FUN_004d00d0(int volume, int temporary);
};

class Class_004cdb40 {
public:
    char unknown_0[0x20];
    int field_20;                      // +0x20
    char unknown_24[0x1fc - 0x24];
    int field_1fc;                     // +0x1fc
    int field_200;                     // +0x200 tracks on the disc
    int field_204;                     // +0x204
    int field_208;                     // +0x208 current track
    int field_20c;                     // +0x20c
    int field_210;                     // +0x210
    unsigned char arr_214[100];        // +0x214
    int field_278;                     // +0x278 mode
    int field_27c;                     // +0x27c
    int field_280;                     // +0x280
    int field_284;                     // +0x284
    char unknown_288[4];
    int field_28c;                     // +0x28c

    void FUN_004cdb40();
};

// FUNCTION: 0x4cdb40
void Class_004cdb40::FUN_004cdb40()
{
    char buf[64];
    int playing;
    int r;
    int count;
    int i;
    int j;
    int zero = 0;

    if (field_200 == zero)
        return;
    if (field_278 == 4) {
        mciSendStringA("stop cdaudio", (LPSTR)zero, 0, (HWND)zero);
        if (field_200 != zero)
            field_208 = 1;
        else
            field_208 = zero;
        field_20c = zero;
        field_284 = zero;
        FUN_004b64d0(DAT_0050b540);
        FUN_004b64d0(DAT_0050b544);
        DAT_0050b544 = DAT_0050b540 = -1;
        return;
    }
    if (field_20c == 2)
        return;
    if (field_278 != 2 && field_278 != 3) {
        int one = 1;
        switch (field_1fc) {
        case 0:
            {
                int none = 0;
                if (field_20c == zero)
                    return;
                field_20c = zero;
                playing = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero) == none
                        ? strcmp(buf, "playing") == none : none;
                if (playing == none)
                    return;
                mciSendStringA("stop cdaudio", (LPSTR)none, 0, (HWND)none);
                if (field_200 != none)
                    field_208 = 1;
                else
                    field_208 = none;
                field_20c = none;
                field_284 = none;
                FUN_004b64d0(DAT_0050b540);
                FUN_004b64d0(DAT_0050b544);
                DAT_0050b544 = DAT_0050b540 = -1;
                return;
            }
        case 1:
            playing = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero) == zero
                    ? strcmp(buf, "playing") == zero : zero;
            if (playing == zero)
                goto done;
            if (field_208 < one)
                field_208 = one;
            else
                field_208++;
            ((Class_004ceb60*)this)->FUN_004ceb60(field_208, field_200 - field_208 + 1);
            if (field_208 > field_200)
                field_208 = one;
            goto done;
        case 2:
            playing = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero) == zero
                    ? strcmp(buf, "playing") == zero : zero;
            if (playing == zero)
                goto done;
            ((Class_004ceb60*)this)->FUN_004ceb60(rand() % field_200 + 1, one);
            goto done;
        case 3:
            playing = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero) == zero
                    ? strcmp(buf, "playing") == zero : zero;
            if (playing == zero || field_208 != field_204) {
                if (field_204 == zero)
                    field_204 = one;
                ((Class_004ceb60*)this)->FUN_004ceb60(field_204, one);
            }
            goto done;
        case 4:
            break;
        default:
            goto done;
        }
    }
    r = rand() & 0xf;
    playing = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero) == zero
            ? strcmp(buf, "playing") == 0 : 0;
    if (playing != 0 && arr_214[field_208] == field_278)
        goto done;
    count = (r + 1) * field_200;
    i = field_208;
    while (count > 0) {
            i++;
            if (i > field_200)
                i = 1;
            if (arr_214[i] == field_278) {
                if (--r <= 0) {
                    j = i;
                    while (j <= field_200 && arr_214[j] == field_278)
                        j++;
                    ((Class_004ceb60*)this)->FUN_004ceb60(i, j - i);
                    break;
                }
            }
            count--;
    }
    if (count > 0)
        goto done;
stop:
    mciSendStringA("stop cdaudio", (LPSTR)zero, 0, (HWND)zero);
    field_20c = 0;
    field_208 = (field_200 != 0);
    field_284 = 0;
    FUN_004b64d0(DAT_0050b540);
    FUN_004b64d0(DAT_0050b544);
    DAT_0050b544 = DAT_0050b540 = -1;
done:
    ((Class_004d00d0*)this)->FUN_004d00d0(field_20, 1);
    field_20c = 1;
    return;
}
