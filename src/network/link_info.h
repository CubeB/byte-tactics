// LinkInfo: one online service entry, as OnlineGetLinkInfo (online.dll) fills
// the array in: the service id (-1 when the slot is unused) and its name.
#ifndef LINK_INFO_H
#define LINK_INFO_H

struct LinkInfo {
    int id;                            // +0x00, -1: unused
    char name[32];                     // +0x04
};

#endif
