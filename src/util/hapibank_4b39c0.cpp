// Decompiled by longcat-2.5-preview-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// MATCH. The 99.6% version carried a second local (`noff = oldlen`) and emitted
// `mov [esp+0x1c], edx` at 0x4b3aba where the original has `mov [esp+0x1c], esi`.
// Both registers hold the same value, so only the allocator's choice differed.
// Dropping the `noff = oldlen` statement and spilling `oldlen` itself fixes the
// store, but only while `noff` stays an unassigned local that the cbuf == 0 arm
// reads: that read is what keeps the spill slot alive, so the store is written
// from the register copy rather than from the loaded value. Every assigned form
// tried (`noff = oldlen` at four different points, `int noff = 0`, `err = 0`,
// `err = oldlen`, and `noff` as a 4th Buffer field) collapses the two names into
// one and loses the `mov esi, edx` copy, which costs 2 bytes.
// Suspected original bug: the cbuf == 0 arm at 0x4b3bb2 reads a stack slot that
// nothing assigns on that path (`mov eax, [esp+0x1c]`); it happens to hold the
// old buf.len spill from 0x4b3aba. Harmless, because err is only tested when
// cbuf != 0. Kept as written so the code matches.
#include <stdio.h>
#include <string.h>
struct AccountList { int count; void* slots; };
#pragma pack(push, 1)
struct BankFileHeader { char name[8]; int nameOffset; int dataOffset; int headerSize; int version; bool compressed; char unused_19[9]; };
#pragma pack(pop)
struct Buffer_004b39c0 { char* data; int len; int csize; };
class HapiBank {
public:
    AccountList* field_0;
    char unknown_4[4];
    int field_8;
    int SaveBank(char* name, char* ext, int param_3, int param_4);
    void SaveAccount(int index, FILE* file, Buffer_004b39c0* buf, int param_3);
    void WriteAuditFile(char* name);
};
char* __stdcall StripExtension(char* name);
void* __cdecl FUN_004d8450(int size);
void* __cdecl FUN_004d8580(void* ptr, int size);
void __cdecl FUN_004d85a0(void* ptr);
int __cdecl SetOutOfMemoryHandler(int handle);
int __stdcall SquashMaxPackedSize(int size, int level);
int __stdcall SquashPack(void* dest, int* destSize, void* src, int srcSize, int param_5, int param_6);
// FUNCTION: 0x4b39c0
int HapiBank::SaveBank(char* name, char* ext, int param_3, int param_4)
{
    char path[256];
    BankFileHeader header;
    int noff;
    int err;
    Buffer_004b39c0 buf;
    FILE* file;
    int i;
    if (field_0 == 0 || field_0->count == 0) { return 0; }
    if (param_4) {
        strcpy(path, name);
        StripExtension(path);
        strcat(path, ".cpa");
        ((HapiBank*)this)->WriteAuditFile(path);
    }
    file = fopen(name, "w+b");
    if (file == 0) { return 0; }
    buf.data = 0;
    buf.len = 0;
    memset(&header, 0, sizeof(header));
    strncpy(header.name, "HAPIBANK", 8);
    header.version = 1;
    int oldlen = buf.len;
    buf.len = oldlen + strlen(ext) + 1;
    buf.data = (char*)FUN_004d8580(buf.data, buf.len);
    strcpy(buf.data + oldlen, ext);
    header.nameOffset = oldlen;
    header.headerSize = sizeof(header);
    fwrite(&header, sizeof(header), 1, file);
    for (i = 0; i < field_0->count; i++) { SaveAccount(i, file, &buf, param_3); }
    fseek(file, 0, 2);
    header.dataOffset = ftell(file);
    int dsize = buf.len;
    buf.csize = SquashMaxPackedSize(dsize, 2);
    int handle = SetOutOfMemoryHandler(0);
    char* cbuf = (char*)FUN_004d8450(buf.csize);
    SetOutOfMemoryHandler(handle);
    if (cbuf != 0) { err = SquashPack(cbuf, &buf.csize, buf.data, dsize, 1, 0); }
    else { err = noff; }   // noff is never assigned; see the note at the top
    if (cbuf != 0 && err == 0 && buf.csize < dsize) {
        fwrite(cbuf, buf.csize, 1, file);
        header.compressed = true;
    } else {
        fwrite(buf.data, buf.len, 1, file);
    }
    fseek(file, 0, 0);
    fwrite(&header, sizeof(header), 1, file);
    fclose(file);
    if (cbuf != 0) { FUN_004d85a0(cbuf); }
    if (buf.data != 0) { FUN_004d85a0(buf.data); }
    return 1;
}
