// Decompiled by deepseek-v4.1-flash, finished by GPT-6, continued by GPT-6.1-sol. Names are provisional.
// Partial: best 69.9%, same score as the #1469 retry. Mode 4 and case 0
// still duplicate the release tail; explicit shared-label variants scored
// lower. Splitting the random-counter decrement did not change the score.
// Main remaining differences are switch layout, strcmp lowering, and loop
// register allocation. FUN_004b64d0 returns int, as in its matched implementation.
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

    if (field_200 == 0)
        return;
    if (field_278 == 4) {
        mciSendStringA("stop cdaudio", 0, 0, 0);
        if (field_200 != 0)
            field_208 = 1;
        else
            field_208 = 0;
        field_20c = 0;
        field_284 = 0;
        FUN_004b64d0(DAT_0050b540);
        FUN_004b64d0(DAT_0050b544);
        DAT_0050b544 = DAT_0050b540 = -1;
        return;
    }
    if (field_20c == 2)
        return;
    if (field_278 != 2 && field_278 != 3) {
        switch (field_1fc) {
        case 0:
            if (field_20c == 0)
                return;
            field_20c = 0;
            playing = mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0
                    ? strcmp(buf, "playing") == 0 : 0;
            if (playing == 0)
                return;
            mciSendStringA("stop cdaudio", 0, 0, 0);
            if (field_200 != 0)
                field_208 = 1;
            else
                field_208 = 0;
            field_20c = 0;
            field_284 = 0;
            FUN_004b64d0(DAT_0050b540);
            FUN_004b64d0(DAT_0050b544);
            DAT_0050b544 = DAT_0050b540 = -1;
            return;
        case 1:
            playing = mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0
                    ? strcmp(buf, "playing") == 0 : 0;
            if (playing != 0)
                goto done;
            if (field_208 < 1)
                field_208 = 1;
            else
                field_208++;
            ((Class_004ceb60*)this)->FUN_004ceb60(field_208, field_200 - field_208 + 1);
            if (field_208 > field_200)
                field_208 = 1;
            goto done;
        case 2:
            playing = mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0
                    ? strcmp(buf, "playing") == 0 : 0;
            if (playing != 0)
                goto done;
            ((Class_004ceb60*)this)->FUN_004ceb60(rand() % field_200 + 1, 1);
            goto done;
        case 3:
            playing = mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0
                    ? strcmp(buf, "playing") == 0 : 0;
            if (playing == 0 || field_208 != field_204) {
                if (field_204 == 0)
                    field_204 = 1;
                ((Class_004ceb60*)this)->FUN_004ceb60(field_204, 1);
            }
            goto done;
        case 4:
            break;
        default:
            goto done;
        }
    }
    r = rand() & 0xf;
    playing = mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0
            ? strcmp(buf, "playing") == 0 : 0;
    if (playing != 0 && arr_214[field_208] == field_278)
        goto done;
    count = (r + 1) * field_200;
    i = field_208;
    if (count > 0) {
        do {
            i++;
            if (i > field_200)
                i = 1;
            if (arr_214[i] == field_278) {
                if (--r < 1) {
                    j = i;
                    goto found;
                }
            }
            count--;
        } while (count > 0);
        if (count > 0)
            goto done;
    }
stop:
    mciSendStringA("stop cdaudio", 0, 0, 0);
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
found:
    while (j <= field_200 && arr_214[j] == field_278)
        j++;
    ((Class_004ceb60*)this)->FUN_004ceb60(i, j - i);
    if (count > 0)
        goto done;
    goto stop;
}
