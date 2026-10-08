// Decompiled by space-bunny-free, deepseek-v4.1, deepseek-v4.1-flash, GPT-6.1-sol, Space Bunny Free, DeepSeek V4.1 Flash, Sonnet 5.5, muse-spark-1.3-free, LongCat 2.5 Preview Free, Opus and Haiku. Names are provisional.
#include <stdlib.h>
#include <string.h>
#include <io.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
char* __stdcall SkipTextLines(char* list, int index);
int __cdecl _strcmpi(const char* a, const char* b);
int __stdcall HAPI_FindFirst(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall HAPI_FindNext(int handle, struct _finddata_t* fd);
void __stdcall HAPI_FindClose(int handle);
char* __stdcall StripExtension(char* name);
long __stdcall HAPI_FileLengthByName(char* name);
void __stdcall GetCurrentDriveLetter(char* buf);
char* __stdcall GetDriveDirectory(char* drive, char* buf, int size);
int __stdcall ScanDirectory(char* path, char* list, char* sizes, int mode, int flag, int what);
void __stdcall FUN_004a32a0(void* gui, const char* name, int x, int y, int z);
void __stdcall StripFileName(char* path);
void __stdcall StripPath(char* path);
void __stdcall ChangeDrive(char* drive);
void __stdcall ChangeDirectory(const char* path);
int GetTickRate();
unsigned int GetTicks();
int GetTextKeyColor();

#pragma pack(push, 1)

// The 0xa4-byte element shared by every blink-word view: the drawing views
// split the first 0xa0 bytes into text and fields, the allocation view sees
// the whole 0xa0 as text, and both put the value at +0xa0.
struct BlinkWord {
    char text[0x80];                 // +0x0
    int field_80;                    // +0x80
    int field_84;                    // +0x84
    int field_88;                    // +0x88
    float field_8c;                  // +0x8c
    int field_90;                    // +0x90
    float field_94;                  // +0x94
    int field_98;                    // +0x98
    float field_9c;                  // +0x9c
    int value;                       // +0xa0
};

// A gadget entry, 0x15b bytes, shared by the requester's callers and its
// click handler. The name is a plain string; value (0xb6) holds the
// highlighted file name, field_ba the selection index.
struct Entry {
    char unknown_0[0x13];
    short field_13;                  // +0x13
    short field_15;                  // +0x15
    char unknown_17[0xb6 - 0x17];
    char value[4];                   // +0xb6
    short field_ba;                  // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Obj18 {
    char unknown_0[4];
    char* field_4;                   // +0x4
};

// The object behind FileRequester::field_4.
struct ReqSub {
    char unknown_0[0x140];
    short field_140;                 // +0x140
};

struct FileRequester {
    void* gui;                       // +0x00
    ReqSub* field_4;                 // +0x04
    char* field_8;                   // +0x08
    char* field_c;                   // +0x0c
    char* field_10;                  // +0x10
    char drive[0x10];                // +0x14
    char cwd[0x100];                 // +0x24
    char save_drive[0x10];           // +0x124
    char save_cwd[0x100];            // +0x134
    char* names;                     // +0x234
    char* sizes;                     // +0x238
    char* selected;                  // +0x23c
    int field_240;                   // +0x240
    void (__stdcall* callback)(void*);   // +0x244
};

struct Layer {
    char unknown_0[4];
    Entry* entries;                  // +0x04
    char unknown_8[4];
    FileRequester* req;              // +0x0c
};

struct Gadget {
    char unknown_0[0x18];
    Layer* layer;                    // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};

struct Dialog {
    char path[0x13];                 // +0x00
    short field_13;                  // +0x13
    char unknown_15[0x18 - 0x15];    // +0x15
    Obj18* field_18;                 // +0x18
    char unknown_1c[0xa6 - 0x1c];
    BlinkWord* words;                // +0xa6
    int count;                       // +0xaa
    int active;                      // +0xae
    char unknown_b2[0xcca - 0xb2];
    int field_cca;                   // +0xcca

    // self is really the first stack argument, not `this`.
    FileRequester* OpenFileRequester(Dialog* self, char* arg2, char* arg3, char* arg4);
};

#pragma pack(pop)

struct Entry_004a1810;

Entry* __stdcall FUN_004a0010(Entry* entries, char* name);
Entry* __stdcall FUN_004a0180(Entry* entries, char* name);
Entry* __stdcall FUN_004a0200(Entry* entries, char* name);
Entry* __stdcall FindGadgetOrNull(Entry* entries, char* name);
int __stdcall IsGadgetNamed(Entry* entries, int i, char* name);
int __stdcall FindGadgetIndex(Entry* entries, char* name, int type);
Entry* __stdcall FindGadgetChecked(Entry* entries, char* name);
void __stdcall FUN_0049fa90(void* obj);
void __stdcall ClearSelectedGadget(void* obj);
void __stdcall RefreshFileList(FileRequester* obj);
void* __stdcall LoadGuiLayer(void* param_1, char* param_2, int param_3);
void __stdcall SelectFontForEntry(Entry_004a1810* entries, int index);
void __stdcall SetTextColors(int param_1, int param_2);
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);

// Only the first pass (over 0..n) sits inside `if (n != -1)`; start = n + 1,
// end = count - 1 and the second pass always run, which sorts the whole list
// when no line starts with a backslash.
// The second pass compares keys the other way round, keys[i+1] - keys[i], so
// with keys the part after the split sorts descending while the part before
// it sorts ascending; both _strcmpi calls compare ptr1[i] with ptr1[i+1]. n
// records the LAST line starting with a backslash.
// FUNCTION: 0x4aefa0
void __stdcall SortFileList(char* list1, char* list2, int* keys, int count)
{
    char** ptr2;
    char** ptr1;
    int swapped;
    int n;
    char* buf2;
    char* buf1;
    int end;
    int start;
    int i;

    buf1 = (char*)FUN_004d83b0("SORTED LIST1", 0x17700);
    buf2 = (char*)FUN_004d83b0("SORTED LIST2", 0x17700);
    ptr1 = (char**)FUN_004d83b0("PTR LIST1", 0x2ee0);
    ptr2 = (char**)FUN_004d83b0("PTR LIST2", 0x2ee0);
    n = -1;
    for (i = 0; i < count; i++) {
        ptr1[i] = SkipTextLines(list1, i);
        if (list2)
            ptr2[i] = SkipTextLines(list2, i);
        if (ptr1[i][0] == '\\')
            n = i;
    }
    if (n != -1) {
        do {
            swapped = 0;
            for (i = 0; i < n; i++) {
                int diff;
                if (keys)
                    diff = keys[i] - keys[i+1];
                else
                    diff = _strcmpi(ptr1[i], ptr1[i+1]);
                if (diff > 0) {
                    char* t = ptr1[i];
                    ptr1[i] = ptr1[i+1];
                    ptr1[i+1] = t;
                    if (list2) {
                        t = ptr2[i];
                        ptr2[i] = ptr2[i+1];
                        ptr2[i+1] = t;
                    }
                    if (keys) {
                        int u = keys[i];
                        keys[i] = keys[i+1];
                        keys[i+1] = u;
                    }
                    swapped = 1;
                }
            }
        } while (swapped);
    }
    start = n + 1;
    end = count - 1;
    do {
        swapped = 0;
        for (i = start; i < end; i++) {
            int diff;
            if (keys)
                diff = keys[i+1] - keys[i];
            else
                diff = _strcmpi(ptr1[i], ptr1[i+1]);
            if (diff > 0) {
                char* t = ptr1[i];
                ptr1[i] = ptr1[i+1];
                ptr1[i+1] = t;
                if (list2) {
                    t = ptr2[i];
                    ptr2[i] = ptr2[i+1];
                    ptr2[i+1] = t;
                }
                if (keys) {
                    int u = keys[i];
                    keys[i] = keys[i+1];
                    keys[i+1] = u;
                }
                swapped = 1;
            }
        }
    } while (swapped);
    {
        char* p = buf1;
        char* q = buf2;
        for (i = 0; i < count; i++) {
            strcpy(p, ptr1[i]);
            p += strlen(ptr1[i]) + 1;
            if (list2) {
                strcpy(q, ptr2[i]);
                q += strlen(ptr2[i]) + 1;
            }
        }
        memcpy(list1, buf1, p - buf1);
        if (list2)
            memcpy(list2, buf2, q - buf2);
    }
    FUN_004d85a0(ptr2);
    FUN_004d85a0(ptr1);
    FUN_004d85a0(buf2);
    FUN_004d85a0(buf1);
}

// The file walk runs in every case and continues the sizes list of the
// directory walk.
// FUNCTION: 0x4af320
int __stdcall ScanDirectory(char* path, char* list, char* sizes, int mode, int flag, int what)
{
    char* first = list;
    int num = 0;
    char* times = (char*)FUN_004d83b0("FILETIMES", 0x2ee0);
    long* tp = (long*)times;
    struct _finddata_t fd;
    char text[256];
    int find;

    if (mode == 1) {
        find = HAPI_FindFirst("*.", &fd, -1, 1);
        if (find != -1) {
            do {
                if (fd.name[0] != '.' && fd.attrib == 0x10) {
                    strncpy(list, "\\", 1);
                    list++;
                    strcpy(list, fd.name);
                    list += strlen(fd.name);
                    *list++ = 0;
                    if (sizes) {
                        strcpy(sizes, "<DIR>");
                        sizes += 6;
                    }
                    num++;
                }
            } while (HAPI_FindNext(find, &fd) != -1);
            HAPI_FindClose(find);
        }
    }
    {
        find = HAPI_FindFirst(path, &fd, -1, 1);
        if (find != -1) {
            do {
                if (fd.name[0] != '.' && fd.attrib != 0x10) {
                    strcpy(list, fd.name);
                    if (flag == 1) {
                        StripExtension(list);
                    }
                    list += strlen(list) + 1;
                    *tp++ = fd.time_write;
                    if (sizes) {
                        _itoa(HAPI_FileLengthByName(fd.name), text, 10);
                        strcpy(sizes, text);
                        sizes += strlen(text) + 1;
                    }
                    num++;
                }
            } while (HAPI_FindNext(find, &fd) != -1);
            HAPI_FindClose(find);
        }
    }
    if (what) {
        if (what == 2) {
            SortFileList(first, 0, 0, num);
        } else {
            SortFileList(first, 0, (int*)times, num);
        }
    }
    FUN_004d85a0(times);
    *list = 0;
    return num;
}

// FUNCTION: 0x4af5b0
void __stdcall RefreshFileList(FileRequester* obj)
{
    GetCurrentDriveLetter(obj->drive);
    GetDriveDirectory(obj->drive, obj->cwd, 0x100);
    int n = ScanDirectory(obj->field_c + 0xb6, obj->names, obj->sizes, 1, 0, 0);
    SortFileList(obj->names, obj->sizes, 0, n);
    FUN_004a32a0(obj->gui, "SWIN", (int)obj->names, n, 0);
    FUN_004a32a0(obj->gui, "SIZE", (int)obj->sizes, n, 0);
    obj->field_4->field_140 = 0;
    strcpy(obj->field_10 + 0xb6, obj->cwd);
}

// Click handler of the file requester (FILEREQ.GUI, opened by 0x4afa30).
// On close (field_60 == -1) it restores the saved drive and directory and
// frees the request data. Otherwise it acts on the entry the user clicked:
// LOAD/SWIN enter a directory, CANC accepts, NAME takes the highlighted file,
// PATH walks one level up and the *DRV entries pick a drive letter.
// FUNCTION: 0x4af670
void __stdcall FileRequesterHandler(Gadget* gadget)
{
    Entry* entries;
    char drive[2];
    int result;
    int i;
    int n;
    // Declared after n: the operand order of the cwd[] accesses follows it.
    FileRequester* req;
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
        RefreshFileList(req);
        FUN_0049fa90(gadget);
        ClearSelectedGadget(gadget);
    }
}

// The result of LoadGuiLayer is the object that gets the vtable slot at +8, not
// the argument.
// FUNCTION: 0x4afa30
FileRequester* Dialog::OpenFileRequester(Dialog* self, char* arg2, char* arg3, char* arg4)
{
    void* gui = LoadGuiLayer(self, "FILEREQ.GUI", 0);
    if (gui == NULL) {
        return NULL;
    }

    FileRequester* obj = (FileRequester*)FUN_004d83b0("FILE REQUESTER DATA", 0x24c);
    obj->gui = self;
    strcpy(obj->cwd, arg2);
    StripFileName(obj->cwd);

    // Must stay strlen(...) == 0: other spellings change the compare.
    if (strlen(obj->cwd) == 0) {
        strcpy(obj->cwd, "NO PATH");
    }

    ((void**)gui)[2] = (void*)FileRequesterHandler;
    ((void**)gui)[3] = obj;
    obj->callback = 0;

    Entry* entries = (Entry*)self->field_18->field_4;
    obj->field_8 = (char*)FUN_004a0010(entries, "NAME");
    obj->field_c = (char*)FUN_004a0010(entries, "MASK");
    obj->field_10 = (char*)FindGadgetOrNull(entries, "PATH");
    Entry* titl = FUN_004a0180(entries, "TITL");
    obj->field_4 = (ReqSub*)FUN_004a0200(entries, "SLID");

    short none = -1;
    entries->field_13 = none;
    entries->field_15 = none;
    strcpy(titl->value, arg4);
    titl->field_13 = none;

    obj->names = (char*)FUN_004d83b0("FILE NAMES", 0x17700);
    memset(obj->names, -1, 0x17700);

    obj->sizes = (char*)FUN_004d83b0("FILE SIZES", 0xea60);
    memset(obj->sizes, -1, 0xea60);

    obj->field_240 = (int)arg3;
    obj->selected = arg2;

    StripPath(arg2);

    strcpy(obj->field_8 + 0xb6, arg2);
    strcpy(obj->field_c + 0xb6, arg3);

    GetCurrentDriveLetter(obj->save_drive);
    GetDriveDirectory(obj->save_drive, obj->save_cwd, 0x100);
    RefreshFileList(obj);

    FUN_0049fa90(self);

    return obj;
}

// FUNCTION: 0x4afc60
void __stdcall AllocBlinkWords(Dialog* obj, int count)
{
    if (obj->words) {
        FUN_004d85a0(obj->words);
        obj->words = 0;
    }
    obj->words = (BlinkWord*)FUN_004d83b0("BlinkWords", count * sizeof(BlinkWord));
    for (int i = 0; i < count; i++)
        obj->words[i].text[0] = 0;
    obj->count = count;
    obj->active = 1;
    obj->words[0].value = -1;
}

// FUNCTION: 0x4afcf0
void __stdcall FreeBlinkWords(int param_1)
{
    int ptr = *(int*)(param_1 + 0xa6);
    FUN_004d85a0((void*)ptr);
    *(int*)(param_1 + 0xa6) = 0;
    *(int*)(param_1 + 0xae) = 0;
}

// FUNCTION: 0x4afd20
void __stdcall SetBlinkGadget(Dialog* obj, int value)
{
    obj->words->value = value;
}

// FUNCTION: 0x4afd40
void __stdcall EnableBlinkWords(int param_1)
{
    int ecx = *(int*)(param_1 + 0xa6);
    if (ecx != 0) {
        *(int*)(param_1 + 0xae) = 1;
    }
}

// FUNCTION: 0x4afd60
void __stdcall DisableBlinkWords(int param_1)
{
    if (*(int*)(param_1 + 0xa6) != 0) {
        *(int*)(param_1 + 0xae) = 0;
    }
}

// FUNCTION: 0x4afd80
int __stdcall AddBlinkWord(Dialog* obj, const char* text, int p2, int p3,
                           int p4, int p5, float f6, float f7)
{
    BlinkWord* p = obj->words;
    if (p == 0)
        return 0;

    int count = obj->count;
    int i = 0;
    for (i = 0; i < count; i++, p++) {
        if (p->text[0] == 0)
            break;
    }
    if (i == count)
        return 0;

    int time = GetTicks();

    strncpy(obj->words[i].text, text, 0x80);
    obj->words[i].field_80 = p2;
    obj->words[i].field_84 = p3;
    obj->words[i].field_88 = p4;
    obj->words[i].field_90 = p5;
    obj->words[i].field_8c = f6;
    obj->words[i].field_94 = f7;

    int rate = GetTickRate();
    obj->words[i].field_9c = (float)rate * f6 + (float)time;
    obj->words[i].field_98 = 0;
    return 1;
}

// FUNCTION: 0x4afe90
void __stdcall ClearBlinkWord(void* param1, int param2)
{
    int max = *(int*)((char*)param1 + 0xaa);
    if (param2 <= max) {
        void* ptr = *(void**)((char*)param1 + 0xa6);
        int calc = param2 + param2 * 4;  // param2 * 5
        int offset = (param2 + calc * 8) * 4;  // (param2 * 41) * 4 = param2 * 0xa4
        *(unsigned char*)((char*)ptr + offset) = 0;
    }
}

// FUNCTION: 0x4afec0
void __stdcall ClearBlinkWords(Dialog* obj)
{
    if (obj->words != 0) {
        for (int i = 0; i < obj->count; i++) {
            obj->words[i].text[0] = 0;
        }
    }
}

// The object at obj+0x18; +4 points at the entry array.
// FUNCTION: 0x4aff00
void __stdcall DrawBlinkWords(Dialog* obj)
{
    if (obj->active == 0)
        return;

    int time = GetTicks();

    if (obj->words->value != -1)
        SelectFontForEntry((Entry_004a1810*)obj->field_18->field_4,
                     obj->words->value);

    for (int i = 0; i < obj->count; i++) {
        if (obj->words[i].text[0] == 0)
            continue;

        float ft = (float)time;
        if (obj->words[i].field_9c < ft) {
            if (obj->words[i].field_98 != 0) {
                obj->words[i].field_9c =
                    (float)GetTickRate() * obj->words[i].field_8c + ft;
                obj->words[i].field_98 = 0;
            } else {
                obj->words[i].field_9c =
                    (float)GetTickRate() * obj->words[i].field_94 + ft;
                obj->words[i].field_98 = 1;
            }
        }

        if (obj->words[i].field_98 != 0)
            SetTextColors(obj->words[i].field_90, GetTextKeyColor());
        else
            SetTextColors(obj->words[i].field_88, GetTextKeyColor());

        DrawString((void*)*(int*)(obj->field_18->field_4 + 0xbc),
                     obj->words[i].text, obj->words[i].field_80,
                     obj->words[i].field_84, -1);
    }
}
