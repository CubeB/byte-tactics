// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// MATCH (1544 bytes). Three levers got here:
// (1) 95.7% to 98.6%: in the Item2 record loop the dataOffset update must execute
// BEFORE the fwrite call (`int len = ...; rec[2] = dataOffset; rec[3] = len;
// dataOffset += len; fwrite(...)`), otherwise dataOffset stays live across the
// call in a callee-saved register and the whole function re-registers; with the
// increment first, dataOffset lives in [esp+0x64] as in the original and edi
// becomes the loop zero/index register. Also, in the type==3 Item1 case, declare
// `char* second = slot->items1[i].value;` BEFORE `int oldlen2 = buf->len;`.
// (2) 98.6% to MATCH: the compression tail at 0x4b4125 was the last 6 bytes. The
// original keeps `off` in ebp from 0x4b412b (and reloads it at 0x4b421f) while
// spilling `raw` to [esp+0x5c]; the plain source keeps `off` in ebx and
// materialises `off + 0x20` into [esp+0x68]. That is a pure allocator tie between
// two callee-saved registers, and one extra reference to `off` inside the
// compress block splits it in the original's favour.
// Every dead spelling tried on 2026-09-30 is folded before allocation and does
// nothing: `(void)off;`, `off = off;`, a bare `off;`, an empty `if (off) { }`, an
// empty `switch (off) { }`, a comma use (`int d = (off, 0);`,
// `FUN_004d8450((off, len))`, `(off, h.size - 0x20)`), `len + (off & 0)`, a named
// `int start = off + 0x20;` local used by both fseeks, declaring `char* raw;`
// before `int len`, `unsigned off`, `0x20 + off` as the second fseek offset, a
// `char* raw = 0;` two-statement definition, an empty `for` loop bounded by
// `(off & 0)`, `off - off`, and a `sizeof`-style trick.
// The nudge that works is a redundant conditional re-assignment of `len`:
//     if (off < 0) len = h.size - 0x20;
// Both arms store the same value, so MSVC folds the assignment to nothing and
// the statement costs 0 bytes, but the extra `off` reference survives to the
// allocator and reproduces the original's ebp/ebx split exactly. This is the
// redundant self-correction lever of guide technique 5 (`if (v) v = 1; else
// v = 0;`), the same family as the original `if (off < 0) { len = 0; }` which
// also flips the tie but emits the guard (test/jge/xor, 6 bytes) for 98.6%.
// `if (off <= -1) len = h.size - 0x20;`, the braced body, and a version that
// stores through a throwaway `int z` all match too.
#include <vector>
#include <io.h>

struct AccountList {
    int count;
    void* slots;
};

struct BankItem {                    // 0x10 bytes
    char* name;                      // +0x00
    int type;                        // +0x04
    int value;                       // +0x08 (int, double or char* depending on type)
    int unknown_c;                   // +0x0c
};

struct SafeDepositBox {              // 0x14 bytes
    int flag;                        // +0x00
    char* name;                      // +0x04 (or the id as an int when flag == 0)
    int len;                         // +0x08
    int unknown_c;                   // +0x0c
    char* buffer;                    // +0x10
};

struct BankAccount {                 // 0x18 bytes
    char* name;                      // +0x00
    int count1;                      // +0x04
    int count2;                      // +0x08
    int unknown_c;                   // +0x0c
    BankItem* items1;                // +0x10
    SafeDepositBox* items2;          // +0x14
};

struct Buffer_004b3c60 {             // the shared string pool
    char* data;                      // +0x00
    int len;                         // +0x04
    int csize;                       // +0x08
};

struct AccountHeader {               // 0x20 bytes
    int size;                        // +0x00
    int strOffset;                   // +0x04
    int nInts;                       // +0x08
    int nDoubles;                    // +0x0c
    int nStrings;                    // +0x10
    int nBlobs;                      // +0x14
    int compressed;                  // +0x18
    int unknown_1c;                  // +0x1c
};

class Class_004b3750 {
public:
    AccountList* field_0;
    char unknown_4[4];
    int field_8;

    void SaveAccount(int index, FILE* file, Buffer_004b3c60* buf, int compress);
};

void* __cdecl FUN_004d8450(int size);
void* __cdecl FUN_004d8580(void* ptr, int size);
void __cdecl FUN_004d85a0(void* ptr);
int __cdecl SetOutOfMemoryHandler(int handle);
int __stdcall SquashMaxPackedSize(int size, int level);
int __stdcall SquashPack(void* dest, int* destSize, void* src, int srcSize, int param_5, int param_6);

// FUNCTION: 0x4b3c60
void Class_004b3750::SaveAccount(int index, FILE* file, Buffer_004b3c60* buf, int compress)
{
    BankAccount* slot = &((BankAccount*)field_0->slots)[index];
    if (slot->count1 <= 0 && slot->count2 <= 0) {
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
        buf->data = (char*)FUN_004d8580(buf->data, buf->len);
        strcpy(buf->data + oldlen, name);
        h.strOffset = oldlen;
    }

    int i;
    for (i = 0; i < slot->count1; i++) {
        if (slot->items1[i].type == 1) {
            char* name = slot->items1[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[2];
            rec[0] = oldlen;
            rec[1] = slot->items1[i].value;
            fwrite(rec, 8, 1, file);
            h.nInts++;
        }
    }

    for (i = 0; i < slot->count1; i++) {
        if (slot->items1[i].type == 2) {
            char* name = slot->items1[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[3];
            rec[0] = oldlen;
            *(double*)&rec[1] = *(double*)&slot->items1[i].value;
            fwrite(rec, 0xc, 1, file);
            h.nDoubles++;
        }
    }

    for (i = 0; i < slot->count1; i++) {
        if (slot->items1[i].type == 3) {
            char* name = slot->items1[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[2];
            rec[0] = oldlen;
            char* second = (char*)slot->items1[i].value;
            int oldlen2 = buf->len;
            buf->len += strlen(second) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen2, second);
            rec[1] = oldlen2;
            fwrite(rec, 8, 1, file);
            h.nStrings++;
        }
    }

    for (i = 0; i < slot->count2; i++) {
        if (slot->items2[i].len > 0) {
            h.nBlobs++;
        }
    }

    int dataOffset = ftell(file) + h.nBlobs * 0x10;
    for (i = 0; i < slot->count2; i++) {
        if (slot->items2[i].len > 0) {
            int rec[4];
            if (slot->items2[i].flag != 0) {
                char* nm = slot->items2[i].name;
                int oldlen = buf->len;
                buf->len += strlen(nm) + 1;
                buf->data = (char*)FUN_004d8580(buf->data, buf->len);
                strcpy(buf->data + oldlen, nm);
                rec[0] = oldlen;
            } else {
                rec[0] = -1;
                rec[1] = (int)slot->items2[i].name;
            }
            rec[2] = dataOffset;
            int len = slot->items2[i].len;
            rec[3] = len;
            dataOffset += len;
            fwrite(rec, 0x10, 1, file);
        }
    }

    for (i = 0; i < slot->count2; i++) {
        if (slot->items2[i].len > 0) {
            fwrite(slot->items2[i].buffer, slot->items2[i].len, 1, file);
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
        char* raw = (char*)FUN_004d8450(len);
        if (raw != 0) {
            fseek(file, off + 0x20, 0);
            fread(raw, len, 1, file);
            int csize = SquashMaxPackedSize(len, 1);
            char* cbuf = (char*)FUN_004d8450(csize);
            if (cbuf != 0) {
                int err = SquashPack(cbuf, &csize, raw, len, 1, 0);
                if (err == 0 && csize < len) {
                    fseek(file, off + 0x20, 0);
                    fwrite(cbuf, csize, 1, file);
                    h.size = csize + 0x20;
                    h.compressed = 1;
                    _chsize(file->_file, ftell(file));
                }
                FUN_004d85a0(cbuf);
            }
            FUN_004d85a0(raw);
        }
        SetOutOfMemoryHandler(handle);
    }

    fseek(file, off, 0);
    fwrite(&h, sizeof(h), 1, file);
    fseek(file, 0, 2);
}
