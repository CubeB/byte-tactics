// ArchiveEntry: one entry of an HPI archive directory, packed at 9 bytes. While
// an archive is being built the first two fields are offsets into the package;
// once it is loaded they are pointers, which the union members name. The
// directory list and the file info stay private to the files that use them.
#ifndef ARCHIVE_ENTRY_H
#define ARCHIVE_ENTRY_H

struct Info;
struct ArchiveDirectory;

#pragma pack(push, 1)
struct ArchiveEntry {
    union {                            // +0x0
        int name;
        char* text;
    };
    union {                            // +0x4
        int data;
        Info* info;
        ArchiveDirectory* dir;
    };
    unsigned char flags;               // +0x8, bit 0: a directory, bit 1: shadowed
};
#pragma pack(pop)

#endif
