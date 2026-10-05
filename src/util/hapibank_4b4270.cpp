// Decompiled by space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Claude Fable 5.1, retried by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Reads one 0x20-byte section header out of a HapiBank archive (called in a loop
// by 0x4b3770) and, when the caller's name matches the section name, unpacks the
// section body and files its records away in the current section of the parsed
// bank: integers, doubles, strings and raw blobs, in that order.
//
// Claude Fable 5.1 (79.2% -> 93.0%): the image base is an integer and the
// offsets in the header and the records are pointer-typed (`char* name`), so
// every `offset + *image` names the base as the add's destination; the int and
// double loops copy each record into a struct local; `<memory.h>` instead of
// `<string.h>` fixed two operand orders.
//
// Claude Opus 5.5 (93.0% -> MATCH): the blob loop is four calls to small
// methods of the same class that /Ob2 inlined, all of them matched on their
// own: 0x4b4b50 / 0x4b4ba0 open a box by id or by name (their identical
// "set the current box" stores are tail-merged into the one store after the
// branch, and their `buffer != 0` results are dropped), 0x4b4cf0 appends
// bytes to the current box and 0x4b4c10 seeks it (the `xor edx,edx` before
// the `rep movsb` is its constant 0). The record is a 16-byte struct copy
// (the original's 16 bytes between the header and the message buffer), and
// the source pointer is a named local computed before the append: passed
// straight as the argument, MSVC forwards it into the inlined memcpy and
// evaluates it late. The helpers are defined after this function, in address
// order, and still inline; they have no FUNCTION lines because each one has
// its own file.
#include <stdio.h>
#include <memory.h>

extern int __cdecl _strcmpi(const char* s1, const char* s2);

struct File_004b4270 {            // the open archive
    void* field_0;                // +0x00
    void* field_4;                // +0x04
    void* field_8;                // +0x08
    int field_c;                  // +0x0c
    void* field_10;               // +0x10
    void* field_14;               // +0x14
    char name[0x100];             // +0x18
};

long __stdcall HAPI_TellFile(File_004b4270* file);
void __stdcall HAPI_readfromfile(File_004b4270* file, void* buf, int size);
void __stdcall HAPI_SeekFile(File_004b4270* file, long pos);
void* __cdecl FUN_004d8450(unsigned int size);
int __stdcall SquashUnpackedSize(unsigned char* src);
int __stdcall SquashUnpack(void* dest, void* src);
char* __stdcall SquashErrorString(int code);
void* __cdecl FUN_004d8580(void* ptr, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FatalError(char* message);

struct SafeDepositBox {           // 0x14 bytes, one box of a section
    int used;                     // +0x00
    int value;                    // +0x04, string offset or integer
    int size;                     // +0x08
    int pos;                      // +0x0c, write/read position
    char* buffer;                 // +0x10
};

struct BankAccount {              // 0x18 bytes, one section
    char unknown_0[8];
    int count;                    // +0x08
    int current;                  // +0x0c
    char unknown_10[4];
    SafeDepositBox* entries;      // +0x14
};

struct AccountList {
    char unknown_0[4];
    BankAccount* slots;           // +0x04
    int index;                    // +0x08
};

struct AccountHeader {            // 0x20 bytes, read from the file
    int size;                     // +0x00, of the whole section
    char* strOffset;              // +0x04, of the section name (an offset stored as a pointer)
    int nInts;                    // +0x08
    int nDoubles;                 // +0x0c
    int nStrings;                 // +0x10
    int nBlobs;                   // +0x14
    int compressed;               // +0x18
    int unknown_1c;               // +0x1c
};

struct IntRec_004b4270 {          // 8 bytes, one int record of the body
    char* name;
    int value;
};

#pragma pack(push, 4)
struct DblRec_004b4270 {          // 12 bytes, one double record of the body
    char* name;
    double value;
};
#pragma pack(pop)

struct BlobRec_004b4270 {         // 16 bytes, one blob record of the body
    char* name;                   // offset of the box name, negative for a numbered box
    int id;
    int offset;                   // file offset of the bytes
    int len;
};

class HapiBank {
public:
    AccountList* table;

    void LoadAccount(File_004b4270* fh, int* image, char* name);
    int OpenAccount(char* name);
    int SetIntegerItem(const char* name, int value);
    int SetDoubleItem(const char* name, double value);
    int SetStringItem(const char* name, char* value);
    int FindNumberedBox(int value, int flag);
    int FindNamedBox(char* name, int flag);
    int OpenNumberedBox(int id);
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int WriteBox(void* src, int len);
};

// FUNCTION: 0x4b4270
void HapiBank::LoadAccount(File_004b4270* fh, int* image, char* name)
{
    int buf;
    int len;
    int base;
    int end;
    int* p;
    AccountHeader h;
    char message[1000];

    base = (int)HAPI_TellFile(fh);
    HAPI_readfromfile(fh, &h, 0x20);
    end = base + h.size;
    if (name != 0 && _strcmpi(name, h.strOffset + *image) != 0) {
        HAPI_SeekFile(fh, end);
        return;
    }
    len = h.size - 0x20;
    if (len > 0) {
        if (h.compressed == 1) {
            char* tmp = (char*)FUN_004d8450(len);
            HAPI_readfromfile(fh, tmp, len);
            buf = (int)FUN_004d8450(SquashUnpackedSize((unsigned char*)tmp));
            int err = SquashUnpack((void*)buf, tmp);
            if (err != 0) {
                sprintf(message, "[HapiBank::LoadAccount] Decompression Error: %s\nFile: %s",
                        SquashErrorString(err), fh->name);
                FatalError(message);
            }
            FUN_004d85a0(tmp);
        } else {
            buf = (int)FUN_004d8450(len);
            HAPI_readfromfile(fh, (void*)buf, len);
        }
        ((HapiBank*)this)->OpenAccount(h.strOffset + *image);
        p = (int*)buf;
        {
            for (int i = 0; i < h.nInts; i++) {
                IntRec_004b4270 rec = *(IntRec_004b4270*)p;
                p += 2;
                ((HapiBank*)this)->SetIntegerItem(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nDoubles; i++) {
                DblRec_004b4270 rec = *(DblRec_004b4270*)p;
                p += 3;
                ((HapiBank*)this)->SetDoubleItem(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nStrings; i++) {
                int a = p[0];
                int b = p[1];
                p += 2;
                ((HapiBank*)this)->SetStringItem((char*)a + *image, (char*)b + *image);
            }
        }
        {
            for (int i = 0; i < h.nBlobs; i++) {
                BlobRec_004b4270 rec = *(BlobRec_004b4270*)p;
                p += 4;
                if ((int)rec.name < 0)
                    ((HapiBank*)this)->OpenNumberedBox(rec.id);
                else
                    ((HapiBank*)this)->OpenNamedBox(rec.name + *image);
                char* src = (char*)(buf + (rec.offset - base) - 0x20);
                ((HapiBank*)this)->WriteBox(src, rec.len);
                ((HapiBank*)this)->SeekBox(0);
            }
        }
        HAPI_SeekFile(fh, end);
        FUN_004d85a0((void*)buf);
    }
}

// The four helpers the blob loop inlines (each matched in its own file).

int HapiBank::OpenNumberedBox(int id)
{
    int r = ((HapiBank*)this)->FindNumberedBox(id, 1);
    table->slots[table->index].current = r;
    return table->slots[table->index].entries[r].buffer != 0;
}

int HapiBank::OpenNamedBox(char* name)
{
    int i = ((HapiBank*)this)->FindNamedBox(name, 1);
    table->slots[table->index].current = i;
    return table->slots[table->index].entries[i].buffer != 0;
}

void HapiBank::SeekBox(int pos)
{
    BankAccount* s = &table->slots[table->index];
    SafeDepositBox* c = &s->entries[s->current];
    if (pos < 0) {
        pos = 0;
    }
    if (pos > c->size) {
        pos = c->size;
    }
    c->pos = pos;
}

int HapiBank::WriteBox(void* src, int len)
{
    SafeDepositBox* c = &table->slots[table->index].entries[table->slots[table->index].current];
    int need = len + c->pos;
    if (need > c->size) {
        c->buffer = (char*)FUN_004d8580(c->buffer, need);
        c->size = need;
    }
    memcpy(c->buffer + c->pos, src, len);
    c->pos += len;
    return len;
}
