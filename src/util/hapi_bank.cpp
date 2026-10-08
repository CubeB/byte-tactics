// Decompiled by Haiku, deepseek-v4.1-flash, GPT-5.6-Terra, GPT-6, GPT-6.1-sol, deepseek-v4.1, space-bunny-free, Claude Opus 5.5, mimo-v2.6-pro, longcat-2.5-preview-free, muse-spark-1.3-free, Sonnet 5.5, Claude Fable 5.1, Opus and Sonnet. Names are provisional.
// HapiBank: a bank of named accounts, each holding named items (integers,
// doubles and strings) and safe-deposit boxes (blocks of bytes under a name
// or a number), saved to and loaded from a HAPIBANK file.

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <io.h>
// Nothing here uses <stdlib.h>: its symbols put SaveAccount, LoadAccount and
// OpenAccount in the symbol-id windows they match in (docs/c2-regalloc.md).
#include <stdlib.h>

// A block of bytes an account keeps under a name or a number.
struct SafeDepositBox {              // 0x14 bytes
    int named;                       // +0x00
    union {
        char* name;                  // +0x04 when named
        int number;                  // +0x04 when not
    };
    int size;                        // +0x08
    int pos;                         // +0x0c, the read and write position
    char* data;                      // +0x10
};

// A named value of an account.
struct BankItem {                    // 0x10 bytes
    char* name;                      // +0x00
    int type;                        // +0x04, 1 = integer, 2 = double, 3 = string
    union {
        int value;                   // +0x08
        double real;                 // +0x08
        char* string;                // +0x08
    };
};

struct BankAccount {                 // 0x18 bytes
    char* name;                      // +0x00
    int itemCount;                   // +0x04
    int boxCount;                    // +0x08
    int currentBox;                  // +0x0c
    BankItem* items;                 // +0x10
    SafeDepositBox* boxes;           // +0x14
};

struct AccountList {                 // 0x0c bytes
    int count;                       // +0x00
    BankAccount* accounts;           // +0x04
    int current;                     // +0x08
};

// An open HAPI file.
struct FileHandle {
    void* fp;                        // +0x00
    void* shared;                    // +0x04
    void* info;                      // +0x08
    int pos;                         // +0x0c
    void* buffer;                    // +0x10, the block sizes
    void* buffer2;                   // +0x14, the current block
    char name[0x100];                // +0x18
};

FileHandle* __stdcall HAPI_OpenFileRead(char* name);
int __stdcall HAPI_CloseFile(FileHandle* file);
long __stdcall HAPI_SeekFile(FileHandle* file, long pos);
long __stdcall HAPI_TellFile(FileHandle* file);
void __stdcall HAPI_readfromfile(FileHandle* file, void* buf, int size);
long __stdcall HAPI_FileLength(FileHandle* file);

void* __cdecl GameAllocShared(unsigned int size);
void* __cdecl GameCalloc(unsigned int count, unsigned int size);
void* __cdecl GameReallocIgnoreTag(void* ptr, unsigned int size);
void __cdecl GameFreeThunk(void* p);
char* __cdecl GameStrdup(char* s);
int __cdecl SetOutOfMemoryHandler(int handle);

int __stdcall SquashUnpackedSize(unsigned char* src);
int __stdcall SquashUnpack(void* dest, void* src);
char* __stdcall SquashErrorString(int code);
int __stdcall SquashMaxPackedSize(int size, int level);
int __stdcall SquashPack(void* dest, int* destSize, void* src, int srcSize, int param_5, int param_6);
void __stdcall FatalError(char* message);
char* __stdcall StripExtension(char* name);

// The file header. 0x22 bytes of it are read and written.
#pragma pack(push, 1)
struct BankFileHeader {              // 0x22 bytes
    char magic[8];                   // +0x00 "HAPIBANK"
    int nameOffset;                  // +0x08, of the bank's name in the string pool
    int poolOffset;                  // +0x0c, of the string pool in the file
    int headerSize;                  // +0x10, where the first account starts
    int version;                     // +0x14
    bool compressed;                 // +0x18, the string pool is squashed
    char unused_19[9];
};
#pragma pack(pop)

// The string pool as loaded: the original keeps it at the bottom of
// OpenBank's frame and hands it by address to LoadAccount.
struct PoolImage {                   // 0x08 bytes
    void* buf;                       // +0x00
    int size;                        // +0x04
};

// The string pool as SaveBank builds it.
struct StringPool {
    char* data;                      // +0x00
    int len;                         // +0x04
    int csize;                       // +0x08
};

struct AccountHeader {               // 0x20 bytes
    int size;                        // +0x00, of the whole account
    char* strOffset;                 // +0x04, of its name in the string pool
    int nInts;                       // +0x08
    int nDoubles;                    // +0x0c
    int nStrings;                    // +0x10
    int nBlobs;                      // +0x14
    int compressed;                  // +0x18
    int unknown_1c;                  // +0x1c
};

// The records of an account's body. The offsets are pointer-typed.
struct IntRecord {                   // 8 bytes
    char* name;
    int value;
};

#pragma pack(push, 4)
struct DoubleRecord {                // 12 bytes
    char* name;
    double value;
};
#pragma pack(pop)

struct BoxRecord {                   // 16 bytes
    char* name;                      // negative for a numbered box
    int number;
    int offset;                      // of the bytes in the file
    int len;
};

#include "hapi_bank.h"

// FUNCTION: 0x4b3620
HapiBank* HapiBank::InitBank()
{
    bank = 0;
    return this;
}

// The out-of-line destructor of HapiBank: frees the account list and
// everything hanging off it.
// FUNCTION: 0x4b3630
void HapiBank::CloseBank()
{
    if (bank != 0) {
        for (int i = 0; i < bank->count; i++) {
            BankAccount* account = &bank->accounts[i];
            GameFreeThunk(account->name);
            if (account->items != 0) {
                for (int j = 0; j < account->itemCount; j++) {
                    GameFreeThunk(account->items[j].name);
                    if (account->items[j].type == 3) {
                        GameFreeThunk(account->items[j].string);
                    }
                }
                GameFreeThunk(account->items);
            }
            if (account->boxes != 0) {
                for (int k = 0; k < account->boxCount; k++) {
                    if (account->boxes[k].named != 0) {
                        GameFreeThunk(account->boxes[k].name);
                    }
                    if (account->boxes[k].data != 0) {
                        GameFreeThunk(account->boxes[k].data);
                    }
                }
                GameFreeThunk(account->boxes);
            }
        }
        if (bank->accounts != 0) {
            GameFreeThunk(bank->accounts);
        }
        GameFreeThunk(bank);
    }
}

// FUNCTION: 0x4b3750
void HapiBank::NewBank()
{
    CloseBank();
    bank = (AccountList*)GameCalloc(1, 0xc);
    bank->current = -1;
}

// FUNCTION: 0x4b3770
int HapiBank::OpenBank(char* filename, char* name, char* account)
{
    void* raw;
    BankFileHeader h;
    // Declared after raw and h: gives the locals the original's stack slots.
    PoolImage img;
    char errmsg[0x80];
    // Separate from filename and name: reusing one variable changes the register split.
    FileHandle* file;
    long remaining;
    long pos;

    file = HAPI_OpenFileRead(filename);
    if (file == 0) {
        return 0;
    }

    HAPI_readfromfile(file, &h, 0x22);
    if (strncmp(h.magic, "HAPIBANK", 8) != 0) {
        HAPI_CloseFile(file);
        return 0;
    }

    if (h.version != 1) {
        HAPI_CloseFile(file);
        return 0;
    }

    img.buf = 0;
    img.size = 0;
    remaining = HAPI_FileLength(file) - h.poolOffset;
    HAPI_SeekFile(file, h.poolOffset);

    if (h.compressed != 0) {
        void* src = GameAllocShared(remaining);
        int dsize;
        int err;

        HAPI_readfromfile(file, src, remaining);
        dsize = SquashUnpackedSize((unsigned char*)src);
        raw = GameAllocShared(dsize);
        err = SquashUnpack(raw, src);
        if (err != 0) {
            sprintf(errmsg, "[HapiBank::OpenBank] Decompression Error: %s",
                    SquashErrorString(err));
            FatalError(errmsg);
        }
        img.buf = GameReallocIgnoreTag(img.buf, dsize);
        // Stored before the memcpy: afterwards the frame grows by 4 bytes.
        img.size = dsize;
        memcpy(img.buf, raw, dsize);
        GameFreeThunk(raw);
        GameFreeThunk(src);
    } else {
        if (img.buf != 0) {
            GameFreeThunk(img.buf);
        }
        img.buf = GameAllocShared(remaining);
        img.size = remaining;
        HAPI_readfromfile(file, img.buf, remaining);
    }

    if (name != 0) {
        if (_strcmpi(name, (char*)img.buf + h.nameOffset) != 0) {
            HAPI_CloseFile(file);
            if (img.buf != 0) {
                GameFreeThunk(img.buf);
            }
            return 0;
        }
    }

    CloseBank();
    bank = (AccountList*)GameCalloc(1, 0xc);
    bank->current = -1;

    HAPI_SeekFile(file, h.headerSize);
    pos = HAPI_TellFile(file);
    if (pos < h.poolOffset) {
        do {
            LoadAccount(file, (int*)&img.buf, account);
            pos = HAPI_TellFile(file);
        } while (pos < h.poolOffset);
    }

    HAPI_CloseFile(file);
    if (img.buf != 0) {
        GameFreeThunk(img.buf);
    }
    return 1;
}

// FUNCTION: 0x4b39c0
int HapiBank::SaveBank(char* filename, char* name, int compress, int audit)
{
    char path[256];
    BankFileHeader header;
    // noff stays an unassigned local: the cbuf == 0 arm reads it (as in the original).
    int noff;
    int err;
    StringPool pool;
    FILE* file;
    int i;
    if (bank == 0 || bank->count == 0) { return 0; }
    if (audit) {
        strcpy(path, filename);
        StripExtension(path);
        strcat(path, ".cpa");
        WriteAuditFile(path);
    }
    file = fopen(filename, "w+b");
    if (file == 0) { return 0; }
    pool.data = 0;
    pool.len = 0;
    memset(&header, 0, sizeof(header));
    strncpy(header.magic, "HAPIBANK", 8);
    header.version = 1;
    int oldlen = pool.len;
    pool.len = oldlen + strlen(name) + 1;
    pool.data = (char*)GameReallocIgnoreTag(pool.data, pool.len);
    strcpy(pool.data + oldlen, name);
    header.nameOffset = oldlen;
    header.headerSize = sizeof(header);
    fwrite(&header, sizeof(header), 1, file);
    for (i = 0; i < bank->count; i++) { SaveAccount(i, file, &pool, compress); }
    fseek(file, 0, 2);
    header.poolOffset = ftell(file);
    int dsize = pool.len;
    pool.csize = SquashMaxPackedSize(dsize, 2);
    int handle = SetOutOfMemoryHandler(0);
    char* cbuf = (char*)GameAllocShared(pool.csize);
    SetOutOfMemoryHandler(handle);
    if (cbuf != 0) { err = SquashPack(cbuf, &pool.csize, pool.data, dsize, 1, 0); }
    else { err = noff; }   // noff is never assigned
    if (cbuf != 0 && err == 0 && pool.csize < dsize) {
        fwrite(cbuf, pool.csize, 1, file);
        header.compressed = true;
    } else {
        fwrite(pool.data, pool.len, 1, file);
    }
    fseek(file, 0, 0);
    fwrite(&header, sizeof(header), 1, file);
    fclose(file);
    if (cbuf != 0) { GameFreeThunk(cbuf); }
    if (pool.data != 0) { GameFreeThunk(pool.data); }
    return 1;
}

// FUNCTION: 0x4b3c60
void HapiBank::SaveAccount(int index, FILE* file, StringPool* buf, int compress)
{
    BankAccount* slot = &bank->accounts[index];
    if (slot->itemCount <= 0 && slot->boxCount <= 0) {
        return;
    }

    int off = ftell(file);
    AccountHeader h;
    memset(&h, 0, sizeof(h));
    fwrite(&h, sizeof(h), 1, file);

    {
        char* name = slot->name;
        int oldlen = buf->len;
        buf->len += strlen(name) + 1;
        buf->data = (char*)GameReallocIgnoreTag(buf->data, buf->len);
        strcpy(buf->data + oldlen, name);
        h.strOffset = (char*)oldlen;
    }

    int i;
    for (i = 0; i < slot->itemCount; i++) {
        if (slot->items[i].type == 1) {
            char* name = slot->items[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)GameReallocIgnoreTag(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[2];
            rec[0] = oldlen;
            rec[1] = slot->items[i].value;
            fwrite(rec, 8, 1, file);
            h.nInts++;
        }
    }

    for (i = 0; i < slot->itemCount; i++) {
        if (slot->items[i].type == 2) {
            char* name = slot->items[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)GameReallocIgnoreTag(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[3];
            rec[0] = oldlen;
            *(double*)&rec[1] = slot->items[i].real;
            fwrite(rec, 0xc, 1, file);
            h.nDoubles++;
        }
    }

    for (i = 0; i < slot->itemCount; i++) {
        if (slot->items[i].type == 3) {
            char* name = slot->items[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)GameReallocIgnoreTag(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[2];
            rec[0] = oldlen;
            // Declared before oldlen2.
            char* second = slot->items[i].string;
            int oldlen2 = buf->len;
            buf->len += strlen(second) + 1;
            buf->data = (char*)GameReallocIgnoreTag(buf->data, buf->len);
            strcpy(buf->data + oldlen2, second);
            rec[1] = oldlen2;
            fwrite(rec, 8, 1, file);
            h.nStrings++;
        }
    }

    for (i = 0; i < slot->boxCount; i++) {
        if (slot->boxes[i].size > 0) {
            h.nBlobs++;
        }
    }

    int dataOffset = ftell(file) + h.nBlobs * 0x10;
    for (i = 0; i < slot->boxCount; i++) {
        if (slot->boxes[i].size > 0) {
            int rec[4];
            if (slot->boxes[i].named != 0) {
                char* nm = slot->boxes[i].name;
                int oldlen = buf->len;
                buf->len += strlen(nm) + 1;
                buf->data = (char*)GameReallocIgnoreTag(buf->data, buf->len);
                strcpy(buf->data + oldlen, nm);
                rec[0] = oldlen;
            } else {
                rec[0] = -1;
                rec[1] = slot->boxes[i].number;
            }
            // dataOffset is updated before the fwrite so it is not live across the call.
            rec[2] = dataOffset;
            int len = slot->boxes[i].size;
            rec[3] = len;
            dataOffset += len;
            fwrite(rec, 0x10, 1, file);
        }
    }

    for (i = 0; i < slot->boxCount; i++) {
        if (slot->boxes[i].size > 0) {
            fwrite(slot->boxes[i].data, slot->boxes[i].size, 1, file);
        }
    }

    h.size = ftell(file) - off;

    if (compress != 0) {
        int handle = SetOutOfMemoryHandler(0);
        int len = h.size - 0x20;
        // A redundant conditional re-assignment of `len`: it mentions `off` once
        // more and compiles to nothing (both arms store the same value), which
        // flips the off/raw register tie to the original's. See the note at the top.
        if (off < 0) len = h.size - 0x20;
        char* raw = (char*)GameAllocShared(len);
        if (raw != 0) {
            fseek(file, off + 0x20, 0);
            fread(raw, len, 1, file);
            int csize = SquashMaxPackedSize(len, 1);
            char* cbuf = (char*)GameAllocShared(csize);
            if (cbuf != 0) {
                int err = SquashPack(cbuf, &csize, raw, len, 1, 0);
                if (err == 0 && csize < len) {
                    fseek(file, off + 0x20, 0);
                    fwrite(cbuf, csize, 1, file);
                    h.size = csize + 0x20;
                    h.compressed = 1;
                    _chsize(file->_file, ftell(file));
                }
                GameFreeThunk(cbuf);
            }
            GameFreeThunk(raw);
        }
        SetOutOfMemoryHandler(handle);
    }

    fseek(file, off, 0);
    fwrite(&h, sizeof(h), 1, file);
    fseek(file, 0, 2);
}

// Reads one 0x20-byte section header out of a HapiBank archive (called in a loop
// by 0x4b3770) and, when the caller's name matches the section name, unpacks the
// section body and files its records away in the current section of the parsed
// bank: integers, doubles, strings and raw blobs, in that order.
// FUNCTION: 0x4b4270
void HapiBank::LoadAccount(FileHandle* fh, int* image, char* name)
{
    // The image base is an int and the record offsets are pointer-typed.
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
            char* tmp = (char*)GameAllocShared(len);
            HAPI_readfromfile(fh, tmp, len);
            buf = (int)GameAllocShared(SquashUnpackedSize((unsigned char*)tmp));
            int err = SquashUnpack((void*)buf, tmp);
            if (err != 0) {
                sprintf(message, "[HapiBank::LoadAccount] Decompression Error: %s\nFile: %s",
                        SquashErrorString(err), fh->name);
                FatalError(message);
            }
            GameFreeThunk(tmp);
        } else {
            buf = (int)GameAllocShared(len);
            HAPI_readfromfile(fh, (void*)buf, len);
        }
        OpenAccount(h.strOffset + *image);
        p = (int*)buf;
        {
            for (int i = 0; i < h.nInts; i++) {
                IntRecord rec = *(IntRecord*)p;
                p += 2;
                SetIntegerItem(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nDoubles; i++) {
                DoubleRecord rec = *(DoubleRecord*)p;
                p += 3;
                SetDoubleItem(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nStrings; i++) {
                int a = p[0];
                int b = p[1];
                p += 2;
                SetStringItem((char*)a + *image, (char*)b + *image);
            }
        }
        {
            for (int i = 0; i < h.nBlobs; i++) {
                BoxRecord rec = *(BoxRecord*)p;
                p += 4;
                if ((int)rec.name < 0)
                    OpenNumberedBox(rec.number);
                else
                    OpenNamedBox(rec.name + *image);
                // Named local computed before the append: as a direct argument it is evaluated late.
                char* src = (char*)(buf + (rec.offset - base) - 0x20);
                WriteBox(src, rec.len);
                SeekBox(0);
            }
        }
        HAPI_SeekFile(fh, end);
        GameFreeThunk((void*)buf);
    }
}

// Finds the account named by the argument, or appends a new empty account
// when there is none, and makes it the current one; returns 1 when it
// already existed.
// FUNCTION: 0x4b4560
int HapiBank::OpenAccount(char* name)
{
    for (int i = 0; i < bank->count; i++) {
        if (_strcmpi(bank->accounts[i].name, name) == 0) {
            bank->current = i;
            bank->accounts[i].currentBox = -1;
            return 1;
        }
    }
    bank->current = bank->count;
    bank->count++;
    bank->accounts = (BankAccount*)GameReallocIgnoreTag(
        bank->accounts, bank->count * sizeof(BankAccount));
    memset(&bank->accounts[bank->current], 0, sizeof(BankAccount));
    bank->accounts[bank->current].name = GameStrdup(name);
    bank->accounts[bank->current].currentBox = -1;
    return 0;
}

// Stores an integer under a name in the current account (FindItem with 1
// finds or adds the item), freeing the old value first when it was a string;
// returns 0 when there is no current account.
// FUNCTION: 0x4b4630
int HapiBank::SetIntegerItem(const char* name, int value)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 1);
        if (bank->accounts[bank->current].items[i].type == 3)
            GameFreeThunk(bank->accounts[bank->current].items[i].string);
        bank->accounts[bank->current].items[i].value = value;
        bank->accounts[bank->current].items[i].type = 1;
        return 1;
    }
    return 0;
}

// The double counterpart of SetIntegerItem.
// FUNCTION: 0x4b46c0
int HapiBank::SetDoubleItem(const char* name, double value)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 1);
        if (bank->accounts[bank->current].items[i].type == 3)
            GameFreeThunk(bank->accounts[bank->current].items[i].string);
        bank->accounts[bank->current].items[i].real = value;
        bank->accounts[bank->current].items[i].type = 2;
        return 1;
    }
    return 0;
}

// The string counterpart of SetIntegerItem: stores a copy of the string, and
// does nothing when it is null.
// FUNCTION: 0x4b4750
int HapiBank::SetStringItem(const char* name, char* value)
{
    if (bank && bank->current >= 0 && value) {
        int i = FindItem(name, 1);
        if (bank->accounts[bank->current].items[i].type == 3)
            GameFreeThunk(bank->accounts[bank->current].items[i].string);
        bank->accounts[bank->current].items[i].string = GameStrdup(value);
        bank->accounts[bank->current].items[i].type = 3;
        return 1;
    }
    return 0;
}

// Reads an integer item of the current account; returns `def` when there is
// no current account, the item is missing, or it is not an integer.
// FUNCTION: 0x4b4800
int HapiBank::GetIntegerItem(char* name, int def)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 0);
        if (i >= 0 && bank->accounts[bank->current].items[i].type == 1)
            return bank->accounts[bank->current].items[i].value;
    }
    return def;
}

// FUNCTION: 0x4b4850
double HapiBank::GetDoubleItem(char* name, double def)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 0);
        if (i >= 0 && bank->accounts[bank->current].items[i].type == 2)
            return bank->accounts[bank->current].items[i].real;
    }
    return def;
}

// FUNCTION: 0x4b48a0
char* HapiBank::GetStringItem(char* name, char* def)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 0);
        if (i >= 0 && bank->accounts[bank->current].items[i].type == 3)
            return bank->accounts[bank->current].items[i].string;
    }
    return def;
}

// FUNCTION: 0x4b48f0
int HapiBank::HasItem(const char* name)
{
    int i = FindItem(name, 0);
    return i >= 0 ? 1 : 0;
}

// Returns the index of the named item of the current account; when it is
// missing, adds it if `create` is set and returns -1 otherwise.
// FUNCTION: 0x4b4910
int HapiBank::FindItem(const char* name, int create)
{
    AccountList* f = bank;
    if (!f || f->current < 0)
        return -1;
    BankAccount* s = &f->accounts[f->current];
    for (int i = 0; i < s->itemCount; i++) {
        if (_strcmpi(s->items[i].name, name) == 0)
            return i;
    }
    if (!create)
        return -1;
    int n = s->itemCount;
    s->itemCount = n + 1;
    s->items = (BankItem*)GameReallocIgnoreTag(s->items, (n + 1) * sizeof(BankItem));
    memset(&s->items[n], 0, sizeof(BankItem));
    s->items[n].name = GameStrdup((char*)name);
    return n;
}

// FUNCTION: 0x4b49d0
int HapiBank::FindNumberedBox(int number, int create)
{
    AccountList* t = bank;
    if (!t || t->current < 0)
        return -1;
    BankAccount* s = &t->accounts[t->current];
    for (int i = 0; i < s->boxCount; i++) {
        if (s->boxes[i].named == 0 && s->boxes[i].number == number)
            return i;
    }
    if (!create)
        return -1;
    int n = s->boxCount;
    s->boxCount = n + 1;
    s->boxes = (SafeDepositBox*)GameReallocIgnoreTag(s->boxes, (n + 1) * sizeof(SafeDepositBox));
    memset(&s->boxes[n], 0, sizeof(SafeDepositBox));
    s->boxes[n].number = number;
    s->boxes[n].named = 0;
    return n;
}

// FUNCTION: 0x4b4a80
int HapiBank::FindNamedBox(char* name, int create)
{
    AccountList* t = bank;
    if (!t || t->current < 0)
        return -1;
    BankAccount* s = &t->accounts[t->current];
    for (int i = 0; i < s->boxCount; i++) {
        if (s->boxes[i].named && _strcmpi(s->boxes[i].name, name) == 0)
            return i;
    }
    if (!create)
        return -1;
    int n = s->boxCount;
    s->boxCount = n + 1;
    s->boxes = (SafeDepositBox*)GameReallocIgnoreTag(s->boxes, (n + 1) * sizeof(SafeDepositBox));
    memset(&s->boxes[n], 0, sizeof(SafeDepositBox));
    s->boxes[n].name = GameStrdup(name);
    s->boxes[n].named = 1;
    return n;
}

// Makes the numbered box (added when missing) the current one; returns
// whether it holds any bytes.
// FUNCTION: 0x4b4b50
int HapiBank::OpenNumberedBox(int number)
{
    int r = FindNumberedBox(number, 1);
    bank->accounts[bank->current].currentBox = r;
    return bank->accounts[bank->current].boxes[r].data != 0;
}

// FUNCTION: 0x4b4ba0
int HapiBank::OpenNamedBox(char* name)
{
    int i = FindNamedBox(name, 1);
    bank->accounts[bank->current].currentBox = i;
    return bank->accounts[bank->current].boxes[i].data != 0;
}

// FUNCTION: 0x4b4bf0
int HapiBank::GetBoxSize()
{
    AccountList* p = bank;
    BankAccount* a = &p->accounts[p->current];
    int box = p->accounts[p->current].currentBox;
    return a->boxes[box].size;
}

// Sets the current box's position, clamped to 0..size.
// FUNCTION: 0x4b4c10
void HapiBank::SeekBox(int pos)
{
    BankAccount* s = &bank->accounts[bank->current];
    SafeDepositBox* c = &s->boxes[s->currentBox];
    if (pos < 0) {
        pos = 0;
    }
    if (pos > c->size) {
        pos = c->size;
    }
    c->pos = pos;
}

// Moves the current box's position to its end and returns it (the returned
// value is what keeps it in eax).
// FUNCTION: 0x4b4c50
int HapiBank::SeekBoxEnd()
{
    BankAccount* e = &bank->accounts[bank->current];
    SafeDepositBox* it = &e->boxes[e->currentBox];
    it->pos = it->size;
    return it->pos;
}

// Reads up to len bytes from the current box into dst and advances its
// position.
// FUNCTION: 0x4b4c80
int HapiBank::ReadBox(void* dst, int len)
{
    SafeDepositBox* c = &bank->accounts[bank->current].boxes[bank->accounts[bank->current].currentBox];
    int avail = c->size - c->pos;
    if (avail <= 0) {
        return 0;
    }
    if (len > avail) {
        len = avail;
    }
    memcpy(dst, c->data + c->pos, len);
    c->pos += len;
    return len;
}

// Writes len bytes from src at the current box's position, growing the box
// as needed, and advances the position.
// FUNCTION: 0x4b4cf0
int HapiBank::WriteBox(void* src, int len)
{
    SafeDepositBox* c = &bank->accounts[bank->current].boxes[bank->accounts[bank->current].currentBox];
    int need = len + c->pos;
    if (need > c->size) {
        c->data = (char*)GameReallocIgnoreTag(c->data, need);
        c->size = need;
    }
    memcpy(c->data + c->pos, src, len);
    c->pos += len;
    return len;
}

// FUNCTION: 0x4b4d70
void HapiBank::WriteAuditFile(char* filename)
{
    FILE* file = fopen(filename, "w");
    int i;

    if (file == 0) {
        return;
    }

    fprintf(file, "HapiBank Audit File\n\n");
    fprintf(file, "Number of accounts: %i\n\n", bank->count);

    for (i = 0; i < bank->count; i++) {
        BankAccount* account = (BankAccount*)((char*)bank->accounts + i * 0x18);
        int j;

        fprintf(file, "Account:  \"%s\"\n{\n", account->name);
        for (j = 0; j < account->itemCount; j++) {
            BankItem* item = (BankItem*)((char*)account->items + j * 0x10);

            switch (item->type) {
            case 1:
                fprintf(file, "   Item \"%s\" Integer:  %i (0x%08x)\n", item->name, item->value, item->value);
                break;
            case 2:
                fprintf(file, "   Item \"%s\" Double:  %f\n", item->name, item->real);
                break;
            case 3:
                fprintf(file, "   Item \"%s\" String:  \"%s\"\n", item->name, item->string);
                break;
            default:
                fprintf(file, "   Item \"%s\" Uninitialized or Unknown\n", item->name);
                break;
            }
        }

        fprintf(file, "\n");
        for (j = 0; j < account->boxCount; j++) {
            SafeDepositBox* box = (SafeDepositBox*)((char*)account->boxes + j * 0x14);

            if (box->named != 0) {
                fprintf(file, "   Safe-Deposit Box \"%s\" contains %i bytes\n", box->name, box->size);
            } else {
                fprintf(file, "   Safe-Deposit Box #%i contains %i bytes\n", box->number, box->size);
            }
        }
        fprintf(file, "}\n\n");
    }

    fprintf(file, "----------EOF----------\n");
    fclose(file);
}
