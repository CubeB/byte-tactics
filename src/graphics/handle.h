// Handle: a GAF sequence reference (Thaldren's Ref_004b8b30), 0xc bytes: the
// frame index in the sequence, the current entry's countdown in value, kind
// set while the sequence loops, and the entry table src. It is the +0xcc of a
// feature and the +0x4 of a feature spot, read by GetGafSequenceFrame and
// advanced by InitGafSequence and AdvanceGafSequence. The one declaration of
// the type, for the files that call those; the entry table behind src stays
// private to gaf.cpp.
#ifndef HANDLE_H
#define HANDLE_H

struct GafEntry;

struct Handle {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    unsigned char kind;                // +0x4, loops while set
    char unknown_5[3];
    GafEntry* src;                     // +0x8
};

#endif
