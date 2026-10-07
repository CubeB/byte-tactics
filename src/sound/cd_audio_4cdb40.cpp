// Decompiled by deepseek-v4.1-flash, finished by GPT-6, continued by GPT-6.1-sol, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>

extern int g_cdNextTrackTimer;
extern int g_cdFadeTimer;

extern int __stdcall RemoveTimer(int);

class Sound {
public:
    int PlayCdTrack(int index, int flag);
};

class Class_004d00d0 {
public:
    int SetAuxVolume(int volume, int temporary);
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

    void PlayNextTrack();
};

// FUNCTION: 0x4cdb40
void Class_004cdb40::PlayNextTrack()
{
    char buf[64];
    int playing;
    int r;
    int count;
    int i;
    int j;
    int zero = 0;
    int res;

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
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        // Chained store, as in the original: g_cdNextTrackTimer is written first.
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
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
                // Result kept in a local so the call is not folded into test eax,eax.
                res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
                playing = res == none ? strcmp(buf, "playing") == none : none;
                if (playing == none)
                    return;
                mciSendStringA("stop cdaudio", (LPSTR)none, 0, (HWND)none);
                if (field_200 != none)
                    field_208 = 1;
                else
                    field_208 = none;
                field_20c = none;
                field_284 = none;
                RemoveTimer(g_cdNextTrackTimer);
                RemoveTimer(g_cdFadeTimer);
                g_cdNextTrackTimer = g_cdFadeTimer = -1;
                return;
            }
        case 1:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;
            if (playing != zero)
                goto done;
            if (field_208 < one)
                field_208 = one;
            else
                field_208++;
            ((Sound*)this)->PlayCdTrack(field_208, field_200 - field_208 + 1);
            if (field_208 > field_200)
                field_208 = one;
            goto done;
            }
        case 2:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;
            if (playing != zero)
                goto done;
            ((Sound*)this)->PlayCdTrack(rand() % field_200 + 1, one);
            goto done;
            }
        case 3:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;            if (playing == zero || field_208 != field_204) {
                if (field_204 == zero)
                    field_204 = one;
                ((Sound*)this)->PlayCdTrack(field_204, one);
            }
            goto done;
            }
        case 4:
            break;
        default:
            goto done;
        }
    }
    r = rand() & 0xf;
    res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
    playing = res == zero ? strcmp(buf, "playing") == 0 : 0;
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
                    ((Sound*)this)->PlayCdTrack(i, j - i);
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
    RemoveTimer(g_cdNextTrackTimer);
    RemoveTimer(g_cdFadeTimer);
    g_cdNextTrackTimer = g_cdFadeTimer = -1;
done:
    ((Class_004d00d0*)this)->SetAuxVolume(field_20, 1);
    field_20c = 1;
    return;
}
