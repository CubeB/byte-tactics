// FileHandle: a file the game has open through the HAPI calls (HAPI_OpenFile,
// HAPI_CreateFile and the readers), loose on disk or inside an archive. The one
// declaration of the struct, for hpi.cpp and every file that reads or writes
// through it; the types behind the pointers stay private to the archive code.
#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

struct _iobuf;
typedef struct _iobuf FILE;
struct OPENHAPIFILE;
struct Info;

struct FileHandle {
    FILE* fp;                          // +0x0, the loose file or the archive's
    OPENHAPIFILE* shared;              // +0x4, the open archive (0 for a loose file)
    Info* info;                        // +0x8, the entry's offset, size and compression
    unsigned int pos;                  // +0xc, read position
    int* buffer;                       // +0x10, the block sizes
    unsigned char* buffer2;            // +0x14, the current block
    char name[0x100];                  // +0x18

    void SetFileName(const char* text);
};

#endif
