// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 61.4% (original 1544 bytes, ours 1553). Writer counterpart of
// 0x4b4270 (the HapiBank section reader, whose file pins every layout used
// here). Writes one 0x20-byte section header, the int/double/string record
// arrays, the blob records and the blob data into the open archive, optionally
// replaces the uncompressed body with a compressed one, then rewrites the
// header in place. Runtime call sequence, all callees, the record sizes and the
// header field order already line up; what is left is allocation:
//  * prologue: original does `mov edx,[ecx+4]` (slots) first then
//    `lea eax,[eax+eax*2]` (index*3), giving `lea ebp,[edx+eax*8]`; ours is the
//    reverse (`lea edx,[eax+eax*2]` / `mov eax,[ecx+4]` / `lea ebp,[eax+edx*8]`).
//  * `off` (ftell) lands at esp+0x1c in ours, esp+0x14 in the original, so our
//    frame is 8 bytes wider in the low locals.
//  * the append idiom: original spills oldlen (`mov [esp+0x10],ecx`), reloads
//    buf->len after the strlen and keeps the NAME in esi across FUN_004d8580;
//    ours keeps oldlen in esi and reloads the name from slot->items1[i] after
//    the call. Naming the string local or moving the append into a static
//    helper both made this worse (46.9% / 45.7%).
//  * the ints loop: original keeps slot in ebp and the item in eax; ours moves
//    slot to a spill and uses ebp for the item.
// What fixed the biggest step (43.3 -> 61.4): write `slot->items1[i]` member
// accesses directly instead of binding an `Item1* it` local; MSVC then reloads
// the item instead of pinning it in a callee-saved register. The same change in
// the two blob loops is much worse (27.2%), so those keep their `Item2* it`.
// The blob record leaves rec[1] unassigned in the flag != 0 arm exactly as the
// original does (the reader ignores it on that path); keep `int rec[4]`.
// See the notes at the end of the file too.
#include <stdio.h>
#include <string.h>
#include <io.h>

struct Table_004b3630 {
    int count;
    void* slots;
};

struct Item1_004b3c60 {              // 0x10 bytes
    char* name;                      // +0x00
    int type;                        // +0x04
    int value;                       // +0x08 (int, double or char* depending on type)
    int unknown_c;                   // +0x0c
};

struct Item2_004b3c60 {              // 0x14 bytes
    int flag;                        // +0x00
    char* name;                      // +0x04 (or the id as an int when flag == 0)
    int len;                         // +0x08
    int unknown_c;                   // +0x0c
    char* buffer;                    // +0x10
};

struct Slot_004b3c60 {               // 0x18 bytes
    char* name;                      // +0x00
    int count1;                      // +0x04
    int count2;                      // +0x08
    int unknown_c;                   // +0x0c
    Item1_004b3c60* items1;          // +0x10
    Item2_004b3c60* items2;          // +0x14
};

struct Buffer_004b3c60 {             // the shared string pool
    char* data;                      // +0x00
    int len;                         // +0x04
    int csize;                       // +0x08
};

struct Header_004b3c60 {             // 0x20 bytes
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
    Table_004b3630* field_0;
    char unknown_4[4];
    int field_8;

    void FUN_004b3c60(int index, FILE* file, Buffer_004b3c60* buf, int compress);
};

void* __cdecl FUN_004d8450(int size);
void* __cdecl FUN_004d8580(void* ptr, int size);
void __cdecl FUN_004d85a0(void* ptr);
int __cdecl FUN_004d8e50(int handle);
int __stdcall FUN_004d1aa0(int size, int level);
int __stdcall FUN_004d1820(void* dest, int* destSize, void* src, int srcSize, int param_5, int param_6);

// FUNCTION: 0x4b3c60
void Class_004b3750::FUN_004b3c60(int index, FILE* file, Buffer_004b3c60* buf, int compress)
{
    Slot_004b3c60* slot = &((Slot_004b3c60*)field_0->slots)[index];
    if (slot->count1 <= 0 && slot->count2 <= 0) {
        return;
    }

    int off = ftell(file);
    Header_004b3c60 h;
    memset(&h, 0, sizeof(h));
    fwrite(&h, sizeof(h), 1, file);

    {
        int oldlen = buf->len;
        buf->len = oldlen + strlen(slot->name) + 1;
        buf->data = (char*)FUN_004d8580(buf->data, buf->len);
        strcpy(buf->data + oldlen, slot->name);
        h.strOffset = oldlen;
    }

    int i;
    for (i = 0; i < slot->count1; i++) {
        if (slot->items1[i].type == 1) {
            int oldlen = buf->len;
            buf->len = oldlen + strlen(slot->items1[i].name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, slot->items1[i].name);
            int rec[2];
            rec[0] = oldlen;
            rec[1] = slot->items1[i].value;
            fwrite(rec, 8, 1, file);
            h.nInts++;
        }
    }

    for (i = 0; i < slot->count1; i++) {
        if (slot->items1[i].type == 2) {
            int oldlen = buf->len;
            buf->len = oldlen + strlen(slot->items1[i].name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, slot->items1[i].name);
            int rec[2];
            rec[0] = oldlen;
            *(double*)&rec[1] = *(double*)&slot->items1[i].value;
            fwrite(rec, 0xc, 1, file);
            h.nDoubles++;
        }
    }

    for (i = 0; i < slot->count1; i++) {
        if (slot->items1[i].type == 3) {
            int oldlen = buf->len;
            buf->len = oldlen + strlen(slot->items1[i].name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, slot->items1[i].name);
            int rec[2];
            rec[0] = oldlen;
            int oldlen2 = buf->len;
            char* second = (char*)slot->items1[i].value;
            buf->len = oldlen2 + strlen(second) + 1;
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
        Item2_004b3c60* it = &slot->items2[i];
        if (it->len > 0) {
            int rec[4];
            if (it->flag != 0) {
                int oldlen = buf->len;
                buf->len = oldlen + strlen(it->name) + 1;
                buf->data = (char*)FUN_004d8580(buf->data, buf->len);
                strcpy(buf->data + oldlen, it->name);
                rec[0] = oldlen;
            } else {
                rec[0] = -1;
                rec[1] = (int)it->name;
            }
            rec[2] = dataOffset;
            rec[3] = it->len;
            fwrite(rec, 0x10, 1, file);
            dataOffset += it->len;
        }
    }

    for (i = 0; i < slot->count2; i++) {
        Item2_004b3c60* it = &slot->items2[i];
        if (it->len > 0) {
            fwrite(it->buffer, it->len, 1, file);
        }
    }

    h.size = ftell(file) - off;

    if (compress != 0) {
        int handle = FUN_004d8e50(0);
        int len = h.size - 0x20;
        char* raw = (char*)FUN_004d8450(len);
        if (raw != 0) {
            fseek(file, off + 0x20, 0);
            fread(raw, len, 1, file);
            int csize = FUN_004d1aa0(len, 1);
            char* cbuf = (char*)FUN_004d8450(csize);
            if (cbuf != 0) {
                int err = FUN_004d1820(cbuf, &csize, raw, len, 1, 0);
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
        FUN_004d8e50(handle);
    }

    fseek(file, off, 0);
    fwrite(&h, sizeof(h), 1, file);
    fseek(file, 0, 2);
}

// STILL DIFFERS: register allocation only (see the header comment). The section
// header field order is pinned against the reader 0x4b4270 (size +0x00,
// strOffset +0x04, nInts +0x08, nDoubles +0x0c, nStrings +0x10, nBlobs +0x14,
// compressed +0x18), and the call sequence matches forward. The argument homes
// esp+0x5c..esp+0x68 are reused by MSVC as the loop counters, the running data
// offset and the compression handle; ours reuses different slots, which is the
// main residual.
