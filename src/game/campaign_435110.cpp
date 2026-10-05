// Decompiled by space-bunny-free. Names are provisional.
// Loads a campaign (the file name of a .cpf campaign script) into the
// campaign object: clears the script list, copies the file name into
// `campaign`, empties all nine name slots (0x4353b0, inlined), builds the
// path of the campaign file itself into name slot 0 (0x435430), loads the
// script into the list at +0xa08, and on failure reports it in the message
// box and retries with the blank name. Then it clears the mission index and
// loads mission 0, but only when a campaign name was given at all: an empty
// `file` returns here without touching the state.
//
// Two things look like Cavedog's own slips, kept as the original has them:
// - the second `strlen(file) != 0` test is dead: 0x435430 cannot change the
//   caller's pointer, and the first test already passed;
// - the message buffer is 0x80 bytes at the top of a 0x80 frame, while the
//   name printed into it can be 0xff bytes, so a long campaign path overruns
//   the frame. 0x435da0.cpp uses 0x100 for the same message.
#include <windows.h>
#include <stdio.h>
#include <string.h>

extern char DAT_005119b8[];
extern char* g_game;

int __stdcall FUN_004bbc40(char* path);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                      // +4
    int field_8;
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_00435c00 {
public:
    int type;                          // +0x0
    char campaign[0x100];              // +0x4
    char names[9][0x100];              // +0x104
    int exists;                        // +0xa04
    Class_004c2ea0 list;               // +0xa08
    char unknown_a14[0xc18 - 0xa14];
    int missionIndex;                  // +0xc18
    int field_c1c;                     // +0xc1c

    // 0x4353b0, inlined
    void SetName(int index, char* text)
    {
        strcpy(names[index], text);
        if (index == 1) {
            if (strlen(text) != 0)
                exists = FUN_004bbc40(text);
            else
                exists = 0;
        }
    }

    // 0x4356c0, inlined
    char* GetName(int index)
    {
        char* ptr = (char*)this + index * 0x100 + 0x104;
        if (strlen(ptr) > 0)
            return ptr;
        return 0;
    }

    void BuildCampaignFilePath(int index, char* dir, char* name, char* ext);
    int FUN_00435da0(char* map);
};

class Class_00435110 : public Class_00435c00 {
public:
    void LoadCampaign(char* file);
};

// FUNCTION: 0x435110
void Class_00435110::LoadCampaign(char* file)
{
    char msg[0x80];

    ((Class_004c3240*)&list)->FUN_004c3240();
    strcpy(campaign, file);
    for (int i = 0; i < 9; i++)
        SetName(i, DAT_005119b8);
    if (strlen(file) != 0) {
        BuildCampaignFilePath(0, "camps", campaign, "TDF");
        if (strlen(file) != 0) {
            if (!((Class_004c2f60*)&list)->FUN_004c2f60(GetName(0))) {
                wsprintfA(msg, "The requested campaign file, %s, does not exist.", GetName(0));
                OpenMessageBox(g_game + 0x519, msg, 0x1e0, 1, 1);
                LoadCampaign(DAT_005119b8);
                return;
            }
        }
        field_c1c = 0;
        missionIndex = 0;
        FUN_00435da0(0);
    }
}
