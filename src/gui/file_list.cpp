// Decompiled by space-bunny-free, deepseek-v4.1, deepseek-v4.1-flash, GPT-6.1-sol, Space Bunny Free, DeepSeek V4.1 Flash, Sonnet 5.5, muse-spark-1.3-free, LongCat 2.5 Preview Free, Opus and Haiku. Names are provisional.
#include <stdlib.h>
#include <string.h>
#include <io.h>

void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);
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
void __stdcall ConfigureListBoxByName(void* gui, const char* name, int x, int y, int z);
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
    int x;                           // +0x80
    int y;                           // +0x84
    int colourA;                     // +0x88
    float periodA;                   // +0x8c
    int colourB;                     // +0x90
    float periodB;                   // +0x94
    int phase;                       // +0x98
    float nextToggle;                // +0x9c
    int value;                       // +0xa0
};

// A gadget entry, 0x15b bytes, shared by the requester's callers and its
// click handler. The name is a plain string; value (0xb6) holds the
// highlighted file name, selected the selection index.
struct Gadget {
    char unknown_0[0x13];
    short x;                         // +0x13
    short y;                         // +0x15
    short width;                     // +0x17
    short height;                    // +0x19
    int attribs;                     // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    char value[4];                   // +0xb6
    short selected;                  // +0xba
    char unknown_bc[0x136 - 0xbc];
    unsigned char stages;            // +0x136
    unsigned char stageIndex;        // +0x137
    char unknown_138[0x140 - 0x138];
    short knobPos;                   // +0x140
    short knobSize;                  // +0x142
    void* sliderCallback;            // +0x144 (a typed function pointer here takes symbol ids)
    char unknown_148[0x15b - 0x148];
};

struct Obj18 {
    char unknown_0[4];
    char* field_4;                   // +0x4
};

// The slider gadget behind FileRequester::slider.
struct ReqSub {
    char unknown_0[0x140];
    short knobPos;                   // +0x140
};

struct FileRequester {
    void* gui;                       // +0x00
    ReqSub* slider;                  // +0x04 (the SLID gadget)
    char* nameGadget;                // +0x08 (NAME)
    char* maskGadget;                // +0x0c (MASK)
    char* pathGadget;                // +0x10 (PATH)
    char drive[0x10];                // +0x14
    char cwd[0x100];                 // +0x24
    char save_drive[0x10];           // +0x124
    char save_cwd[0x100];            // +0x134
    char* names;                     // +0x234
    char* sizes;                     // +0x238
    char* selected;                  // +0x23c
    int mask;                        // +0x240 (the mask text passed to OpenFileRequester)
    void (__stdcall* callback)(void*);   // +0x244
};

struct Layer {
    char unknown_0[4];
    Gadget* entries;                 // +0x04
    char unknown_8[4];
    FileRequester* req;              // +0x0c
};

struct Gui {
    char unknown_0[0x18];
    Layer* layer;                    // +0x18
    char unknown_1c[0x60 - 0x1c];
    int selected;                    // +0x60
};

struct Dialog {
    char path[0x13];                 // +0x00
    short field_13;                  // +0x13
    char unknown_15[0x18 - 0x15];    // +0x15
    Obj18* holder;                   // +0x18
    char unknown_1c[0xa6 - 0x1c];
    BlinkWord* words;                // +0xa6
    int count;                       // +0xaa
    int active;                      // +0xae
    char unknown_b2[0xcca - 0xb2];
    int changed;                     // +0xcca

    // self is really the first stack argument, not `this`.
    FileRequester* OpenFileRequester(Dialog* self, char* arg2, char* arg3, char* arg4);
};

#pragma pack(pop)

struct Entry_004a1810;

Gadget* __stdcall FindGadgetChecked_B(Gadget* entries, char* name);
Gadget* __stdcall FindGadgetChecked_C(Gadget* entries, char* name);
Gadget* __stdcall FindGadgetChecked_D(Gadget* entries, char* name);
Gadget* __stdcall FindGadgetOrNull(Gadget* entries, char* name);
int __stdcall IsGadgetNamed(Gadget* entries, int i, char* name);
int __stdcall FindGadgetIndex(Gadget* entries, char* name, int type);
Gadget* __stdcall FindGadgetChecked(Gadget* entries, char* name);
void __stdcall MarkChanged(void* obj);
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

    buf1 = (char*)GameAllocIgnoreTag("SORTED LIST1", 0x17700);
    buf2 = (char*)GameAllocIgnoreTag("SORTED LIST2", 0x17700);
    ptr1 = (char**)GameAllocIgnoreTag("PTR LIST1", 0x2ee0);
    ptr2 = (char**)GameAllocIgnoreTag("PTR LIST2", 0x2ee0);
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
    GameFreeThunk(ptr2);
    GameFreeThunk(ptr1);
    GameFreeThunk(buf2);
    GameFreeThunk(buf1);
}

// The file walk runs in every case and continues the sizes list of the
// directory walk.
// FUNCTION: 0x4af320
int __stdcall ScanDirectory(char* path, char* list, char* sizes, int mode, int flag, int what)
{
    char* first = list;
    int num = 0;
    char* times = (char*)GameAllocIgnoreTag("FILETIMES", 0x2ee0);
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
    GameFreeThunk(times);
    *list = 0;
    return num;
}

// FUNCTION: 0x4af5b0
void __stdcall RefreshFileList(FileRequester* obj)
{
    GetCurrentDriveLetter(obj->drive);
    GetDriveDirectory(obj->drive, obj->cwd, 0x100);
    int n = ScanDirectory(obj->maskGadget + 0xb6, obj->names, obj->sizes, 1, 0, 0);
    SortFileList(obj->names, obj->sizes, 0, n);
    ConfigureListBoxByName(obj->gui, "SWIN", (int)obj->names, n, 0);
    ConfigureListBoxByName(obj->gui, "SIZE", (int)obj->sizes, n, 0);
    obj->slider->knobPos = 0;
    strcpy(obj->pathGadget + 0xb6, obj->cwd);
}

// Click handler of the file requester (FILEREQ.GUI, opened by 0x4afa30).
// On close (selected == -1) it restores the saved drive and directory and
// frees the request data. Otherwise it acts on the entry the user clicked:
// LOAD/SWIN enter a directory, CANC accepts, NAME takes the highlighted file,
// PATH walks one level up and the *DRV entries pick a drive letter.
// FUNCTION: 0x4af670
void __stdcall FileRequesterHandler(Gui* gadget)
{
    Gadget* entries;
    char drive[2];
    int result;
    int i;
    int n;
    // Declared after n: the operand order of the cwd[] accesses follows it.
    FileRequester* req;
    req = gadget->layer->req;
    if (gadget->selected == -1) {
        ChangeDrive(req->save_drive);
        ChangeDirectory(req->save_cwd);
        GameFreeThunk(req);
        return;
    }

    entries = gadget->layer->entries;
    drive[1] = 0;
    result = 0;

    if (IsGadgetNamed(entries, gadget->selected, "LOAD")
        || IsGadgetNamed(entries, gadget->selected, "SWIN")) {
        char* name = SkipTextLines(req->names,
                                  FindGadgetChecked(entries, "SWIN")->selected);
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
    } else if (IsGadgetNamed(entries, gadget->selected, "CANC")) {
        result = 1;
    } else if (IsGadgetNamed(entries, gadget->selected, "PATH")) {
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
    } else if (IsGadgetNamed(entries, gadget->selected, "NAME")) {
        gadget->selected = FindGadgetIndex(entries, "LOAD", 14);
        n = FindGadgetIndex(entries, "NAME", 3);
        result = 1;
        strcpy(req->selected, entries[n].value);
    } else if (IsGadgetNamed(entries, gadget->selected, "ADRV")) {
        drive[0] = 'A';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->selected, "BDRV")) {
        drive[0] = 'B';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->selected, "CDRV")) {
        drive[0] = 'C';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->selected, "DDRV")) {
        drive[0] = 'D';
        ChangeDrive(drive);
    } else if (IsGadgetNamed(entries, gadget->selected, "VDRV")) {
        drive[0] = 'R';
        ChangeDrive(drive);
    }

    if (result == 1) {
        if (req->callback) {
            req->callback(req);
        }
    } else {
        RefreshFileList(req);
        MarkChanged(gadget);
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

    FileRequester* obj = (FileRequester*)GameAllocIgnoreTag("FILE REQUESTER DATA", 0x24c);
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

    Gadget* entries = (Gadget*)self->holder->field_4;
    obj->nameGadget = (char*)FindGadgetChecked_B(entries, "NAME");
    obj->maskGadget = (char*)FindGadgetChecked_B(entries, "MASK");
    obj->pathGadget = (char*)FindGadgetOrNull(entries, "PATH");
    Gadget* titl = FindGadgetChecked_C(entries, "TITL");
    obj->slider = (ReqSub*)FindGadgetChecked_D(entries, "SLID");

    short none = -1;
    entries->x = none;
    entries->y = none;
    strcpy(titl->value, arg4);
    titl->x = none;

    obj->names = (char*)GameAllocIgnoreTag("FILE NAMES", 0x17700);
    memset(obj->names, -1, 0x17700);

    obj->sizes = (char*)GameAllocIgnoreTag("FILE SIZES", 0xea60);
    memset(obj->sizes, -1, 0xea60);

    obj->mask = (int)arg3;
    obj->selected = arg2;

    StripPath(arg2);

    strcpy(obj->nameGadget + 0xb6, arg2);
    strcpy(obj->maskGadget + 0xb6, arg3);

    GetCurrentDriveLetter(obj->save_drive);
    GetDriveDirectory(obj->save_drive, obj->save_cwd, 0x100);
    RefreshFileList(obj);

    MarkChanged(self);

    return obj;
}

// FUNCTION: 0x4afc60
void __stdcall AllocBlinkWords(Dialog* obj, int count)
{
    if (obj->words) {
        GameFreeThunk(obj->words);
        obj->words = 0;
    }
    obj->words = (BlinkWord*)GameAllocIgnoreTag("BlinkWords", count * sizeof(BlinkWord));
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
    GameFreeThunk((void*)ptr);
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
    obj->words[i].x = p2;
    obj->words[i].y = p3;
    obj->words[i].colourA = p4;
    obj->words[i].colourB = p5;
    obj->words[i].periodA = f6;
    obj->words[i].periodB = f7;

    int rate = GetTickRate();
    obj->words[i].nextToggle = (float)rate * f6 + (float)time;
    obj->words[i].phase = 0;
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
        SelectFontForEntry((Entry_004a1810*)obj->holder->field_4,
                     obj->words->value);

    for (int i = 0; i < obj->count; i++) {
        if (obj->words[i].text[0] == 0)
            continue;

        float ft = (float)time;
        if (obj->words[i].nextToggle < ft) {
            if (obj->words[i].phase != 0) {
                obj->words[i].nextToggle =
                    (float)GetTickRate() * obj->words[i].periodA + ft;
                obj->words[i].phase = 0;
            } else {
                obj->words[i].nextToggle =
                    (float)GetTickRate() * obj->words[i].periodB + ft;
                obj->words[i].phase = 1;
            }
        }

        if (obj->words[i].phase != 0)
            SetTextColors(obj->words[i].colourB, GetTextKeyColor());
        else
            SetTextColors(obj->words[i].colourA, GetTextKeyColor());

        DrawString((void*)*(int*)(obj->holder->field_4 + 0xbc),
                     obj->words[i].text, obj->words[i].x,
                     obj->words[i].y, -1);
    }
}
