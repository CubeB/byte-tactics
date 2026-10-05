// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by Sonnet 5.5. Names are provisional.
// Click handler of the file requester (FILEREQ.GUI, opened by 0x4afa30).
// On close (field_60 == -1) it restores the saved drive and directory and
// frees the request data. Otherwise it acts on the entry the user clicked:
// LOAD/SWIN enter a directory, CANC accepts, NAME takes the highlighted file,
// PATH walks one level up and the *DRV entries pick a drive letter.
//
// MATCH. The last three instructions (the SIB base/index order of the
// cwd[n-1], cwd[n+1], cwd[n] accesses in the PATH branch) were decided by the
// order of the function-scope declarations: all locals are declared
// uninitialised at the top, with `req` LAST (after `n`), and assigned later.
// With req declared first (or initialised in its declaration) MSVC 5 puts the
// counter in the SIB base slot; with req numbered after n it puts req there,
// as the original does. Half of the 720 orders of {entries, drive, result, i,
// n, req} match, so any order with n before req works.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004af670 {              // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    char value[0xba - 0xb6];         // +0xb6
    short field_ba;                  // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Req_004af670 {
    char unknown_0[0x24];
    char cwd[0x100];                 // +0x24
    char save_drive[0x10];           // +0x124
    char save_cwd[0x100];            // +0x134
    char* names;                     // +0x234
    char* sizes;                     // +0x238
    char* selected;                  // +0x23c
    int field_240;                   // +0x240
    void (__stdcall* callback)(void*);   // +0x244
};

struct Layer_004af670 {
    char unknown_0[4];
    Entry_004af670* entries;         // +0x04
    char unknown_8[4];
    Req_004af670* req;               // +0x0c
};

struct Gadget_004af670 {
    char unknown_0[0x18];
    Layer_004af670* layer;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};
#pragma pack(pop)

int __stdcall IsGadgetNamed(Entry_004af670* entries, int i, char* name);
int __stdcall FindGadgetIndex(Entry_004af670* entries, char* name, int type);
Entry_004af670* __stdcall FindGadgetChecked(Entry_004af670* entries, char* name);
void __stdcall FUN_0049fa90(Gadget_004af670* gadget);
void __stdcall FUN_004ab0a0(Gadget_004af670* gadget);
void __stdcall FUN_004af5b0(Req_004af670* req);
char* __stdcall SkipTextLines(char* text, int n);
int __stdcall ChangeDrive(char* drive);
void __stdcall ChangeDirectory(const char* path);
void __cdecl FUN_004d85a0(void* data);

// FUNCTION: 0x4af670
void __stdcall FileRequesterHandler(Gadget_004af670* gadget)
{
    Entry_004af670* entries;
    char drive[2];
    int result;
    int i;
    int n;
    Req_004af670* req;
    req = gadget->layer->req;
    if (gadget->field_60 == -1) {
        ChangeDrive(req->save_drive);
        ChangeDirectory(req->save_cwd);
        FUN_004d85a0(req);
        return;
    }

    entries = gadget->layer->entries;
    drive[1] = 0;
    result = 0;

    if (IsGadgetNamed(entries, gadget->field_60, "LOAD")
        || IsGadgetNamed(entries, gadget->field_60, "SWIN")) {
        char* name = SkipTextLines(req->names,
                                  FindGadgetChecked(entries, "SWIN")->field_ba);
        if (name[0] == '\\') {
            strcpy(req->selected, name);
            for (i = 0; i < 10; i++) {
                if (req->selected[i] == ' ') {
                    req->selected[i] = 0;
                    break;
                }
            }
            n = (int)strlen(req->cwd);
            char* tail = req->selected;
            if (n == 3) {
                tail++;
            }
            strcat(req->cwd, tail);
            ChangeDirectory(req->cwd);
        } else {
            result = 1;
            strcpy(req->selected, req->cwd);
            strcat(req->selected, "\\");
            strcat(req->selected, name);
        }
    } else if (IsGadgetNamed(entries, gadget->field_60, "CANC")) {
        result = 1;
    } else if (IsGadgetNamed(entries, gadget->field_60, "PATH")) {
        n = (int)strlen(req->cwd);
        if (n > 0) {
            while (n > 0) {
                if (req->cwd[n] == '\\') {
                    if (req->cwd[n - 1] == ':') {
                        req->cwd[n + 1] = 0;
                        ChangeDirectory(req->cwd);
                    } else {
                        req->cwd[n] = 0;
                        ChangeDirectory(req->cwd);
                    }
                    break;
                }
                n--;
            }
        }
    } else if (IsGadgetNamed(entries, gadget->field_60, "NAME")) {
        gadget->field_60 = FindGadgetIndex(entries, "LOAD", 14);
        n = FindGadgetIndex(entries, "NAME", 3);
        result = 1;
        strcpy(req->selected, entries[n].value);
    } else if (IsGadgetNamed(entries, gadget->field_60, "ADRV")) {
        drive[0] = 'A';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->field_60, "BDRV")) {
        drive[0] = 'B';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->field_60, "CDRV")) {
        drive[0] = 'C';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->field_60, "DDRV")) {
        drive[0] = 'D';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->field_60, "VDRV")) {
        drive[0] = 'R';
        ChangeDrive(drive);
    }

    if (result == 1) {
        if (req->callback) {
            req->callback(req);
        }
    } else {
        FUN_004af5b0(req);
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
    }
}
