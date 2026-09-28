// Click handler of the file requester (FILEREQ.GUI, opened by 0x4afa30).
// On close (field_60 == -1) it restores the saved drive and directory and
// frees the request data. Otherwise it acts on the entry the user clicked:
// LOAD/SWIN enter a directory, CANC accepts, NAME takes the highlighted file,
// PATH walks one level up and the *DRV entries pick a drive letter.
//
// 90% (check.py). Everything matches up to the PATH branch except the SIB
// base/index of its two stores and the compare, and the LOAD/SWIN block
// differs only in register allocation:
//   - original parks the tail pointer in edi, mine in esi (both reuse esi,
//     which held the entries pointer, inside this block);
//   - because of that, mine hoists the destination expression
//     `cwd + strlen(cwd) - 1` above the source's strlen and spills it to
//     [esp+0x20], where the original evaluates it after the source strlen,
//     parks the length in ebx (clobbering the saved parameter, restored at
//     0x4af9e3) and reaches `dec edi`;
//   - the one padding byte MSVC puts before the `repne scasb` in the else
//     block and the slot of the ebx restore inside it follow from those.
// Twenty source shapes (declaration order and scope of `name`, `i`, `tail`,
// register, a ternary instead of tail++, the two strlen calls hoisted into
// temporaries, `&cwd[i]`, `strcat`, a local `char*` for cwd, the branches
// swapped) all compile to the same 115 differing instructions, so this looks
// like a register-priority tie rather than a wrong type or argument.
// Decompiled by space-bunny-free. Names are provisional.
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

int __stdcall FUN_004a0300(Entry_004af670* entries, int i, char* name);
int __stdcall FUN_0049fdf0(Entry_004af670* entries, char* name, int type);
Entry_004af670* __stdcall FUN_0049ff90(Entry_004af670* entries, char* name);
void __stdcall FUN_0049fa90(Gadget_004af670* gadget);
void __stdcall FUN_004ab0a0(Gadget_004af670* gadget);
void __stdcall FUN_004af5b0(Req_004af670* req);
char* __stdcall FUN_004b6af0(char* text, int n);
int __stdcall FUN_004bc300(char* drive);
void __stdcall FUN_004bc360(const char* path);
void __cdecl FUN_004d85a0(void* data);

// FUNCTION: 0x4af670
void __stdcall FUN_004af670(Gadget_004af670* gadget)
{
    Req_004af670* req = gadget->layer->req;
    if (gadget->field_60 == -1) {
        FUN_004bc300(req->save_drive);
        FUN_004bc360(req->save_cwd);
        FUN_004d85a0(req);
        return;
    }

    Entry_004af670* entries = gadget->layer->entries;
    char drive[2];
    drive[1] = 0;
    int result = 0;

    if (FUN_004a0300(entries, gadget->field_60, "LOAD")
        || FUN_004a0300(entries, gadget->field_60, "SWIN")) {
        char* name = FUN_004b6af0(req->names,
                                  FUN_0049ff90(entries, "SWIN")->field_ba);
        if (name[0] == '\\') {
            strcpy(req->selected, name);
            int i;
            for (i = 0; i < 10; i++) {
                if (req->selected[i] == ' ') {
                    req->selected[i] = 0;
                    break;
                }
            }
            char* tail = req->selected;
            if ((int)strlen(req->cwd) == 3) {
                tail++;
            }
            strcpy(req->cwd + (int)strlen(req->cwd) - 1, tail);
            FUN_004bc360(req->cwd);
        } else {
            result = 1;
            strcpy(req->selected, req->cwd);
            strcat(req->selected, "\\");
            strcat(req->selected, name);
        }
    } else if (FUN_004a0300(entries, gadget->field_60, "CANC")) {
        result = 1;
    } else if (FUN_004a0300(entries, gadget->field_60, "PATH")) {
        int n = (int)strlen(req->cwd);
        if (n > 0) {
            while (n > 0) {
                if (req->cwd[n] == '\\') {
                    if (req->cwd[n - 1] == ':') {
                        req->cwd[n + 1] = 0;
                        FUN_004bc360(req->cwd);
                    } else {
                        req->cwd[n] = 0;
                        FUN_004bc360(req->cwd);
                    }
                    break;
                }
                n--;
            }
        }
    } else if (FUN_004a0300(entries, gadget->field_60, "NAME")) {
        gadget->field_60 = FUN_0049fdf0(entries, "LOAD", 14);
        int n = FUN_0049fdf0(entries, "NAME", 3);
        result = 1;
        strcpy(req->selected, entries[n].value);
    } else if (FUN_004a0300(entries, gadget->field_60, "ADRV")) {
        drive[0] = 'A';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "BDRV")) {
        drive[0] = 'B';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "CDRV")) {
        drive[0] = 'C';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "DDRV")) {
        drive[0] = 'D';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "VDRV")) {
        drive[0] = 'R';
        FUN_004bc300(drive);
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
