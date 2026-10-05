// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Refreshes the CD-list table at DAT_0051e828 from the CD object: it saves and
// restores the object's track table across an MCI close/open, looks the current
// disc id up in the 20-entry table and, when found, moves that entry to the
// front and re-reads its track bytes. A disc not in the table is inserted at the
// front after shifting the others up, provided the drive reports 16 audio
// tracks. The final cleanup call depends on the game mode.
//
// Both table walks are load bearing. The search loop must test `*slot != id`
// first and keep the `break` as its own block, otherwise MSVC rotates it. The
// shift-up loop is a downward pointer walk against the addresses.
#include <string.h>
#include <windows.h>

class Class_004ce3e0 {
public:
    void FUN_004ce3e0(const void* src);
};

class Class_004ce450 {
public:
    int GetTrackCount();
};

class Class_004ce460 {
public:
    int IsFirstTrackData();
};

class Class_004ce680 {
public:
    char unknown_0[0x200];
    int size;                          // +0x200
    char unknown_204[0x278 - 0x204];
    int field_278;                     // +0x278

    int GetTrackCategory();
};

class Sound {
public:
    int SetTrackCategory(int param_1);
    int FUN_004cd9c0();
};

class Class_004ce7a0 {
public:
    int SetPlaybackOrder(int param_1);
};

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

class Class_004ced40 {
public:
    int StopCdAudio();
};

class Class_004cedc0 {
public:
    void EnableCdAudio(int on);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    Class_004ce680* cd;                // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x37f14 - 0x2a45];
    unsigned char field_37f14;         // +0x37f14
    unsigned char unknown_37f15;
    unsigned char field_37f16;         // +0x37f16
    char unknown_37f17[0x391f1 - 0x37f17];
    int mode;                          // +0x391f1
};
#pragma pack(pop)

extern Game* g_game;

extern int DAT_0051e828[0x2a8];
extern int DAT_0051e848;
extern int DAT_0051e84c;
extern int DAT_0051e850;
extern int DAT_0051e854;
extern int DAT_0051e858;
extern int DAT_0051f2e8;

// FUNCTION: 0x490fe0
void FUN_00490fe0()
{
    char tracks[16] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char buf[0x88];

    int saved = g_game->cd->GetTrackCategory();
    mciSendStringA("stop cdaudio", 0, 0, 0);
    mciSendStringA("close cdaudio", 0, 0, 0);
    mciSendStringA("open cdaudio", 0, 0, 0);
    ((Class_004cedc0*)g_game->cd)->EnableCdAudio(g_game->field_37f14 & 1);
    ((Class_004ce7a0*)g_game->cd)->SetPlaybackOrder(g_game->field_37f16);
    ((Sound*)g_game->cd)->SetTrackCategory(saved);

    int id = ((Sound*)g_game->cd)->FUN_004cd9c0();
    int index = 0;
    int* slot = &DAT_0051e848;
    while (1) {
        if (*slot != id) {
            slot = (int*)((char*)slot + 0x88);
            index++;
            if ((int)slot < (int)&DAT_0051f2e8)
                continue;
            goto newdisc;
        }
        break;
    }
    {
        memcpy(buf, (char*)DAT_0051e828 + index * 0x88, 0x88);
        for (int j = index; j > 0; j--)
            memcpy((char*)DAT_0051e828 + j * 0x88,
                   (char*)DAT_0051e828 + (j - 1) * 0x88, 0x88);
        memcpy(DAT_0051e828, buf, 0x88);
        ((Class_004ce3e0*)g_game->cd)->FUN_004ce3e0(&DAT_0051e84c);
    }
newdisc:
    if (index == 0x14) {
        if (((Class_004ce450*)g_game->cd)->GetTrackCount() == 0x10) {
            if (((Class_004ce460*)g_game->cd)->IsFirstTrackData() != 0) {
                ((Class_004ce3e0*)g_game->cd)->FUN_004ce3e0(tracks);
            }
        }
        int p = (int)DAT_0051e828 + 0xa18;
        for (; p > (int)DAT_0051e828; p -= 0x88)
            memcpy((void*)p, (void*)(p - 0x88), 0x88);
        DAT_0051e84c = *(int*)&tracks[0];
        DAT_0051e850 = *(int*)&tracks[4];
        DAT_0051e854 = *(int*)&tracks[8];
        DAT_0051e858 = *(int*)&tracks[12];
        DAT_0051e848 = id;
    }
    if ((g_game->flags_2a44 & 4) != 0 && g_game->mode == 6)
        ((Class_004cdb40*)g_game->cd)->PlayNextTrack();
    else
        ((Class_004ced40*)g_game->cd)->StopCdAudio();
}
