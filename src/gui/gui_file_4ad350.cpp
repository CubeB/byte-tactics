// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

struct Tree_004ad350 {
    char unknown_0[4];
    Class_004c46c0* current;           // +0x4
};

#pragma pack(push, 1)
struct Common_004ad350 {
    unsigned char id;                  // +0x00
    unsigned char assoc;               // +0x01
    char name[0x11];                   // +0x02
    short xpos;                        // +0x13
    short ypos;                        // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int attribs;                       // +0x1b
    int colorf;                        // +0x1f
    int colorb;                        // +0x23
    unsigned char texturenumber;       // +0x27
    unsigned char fontnumber;          // +0x28
    unsigned char active;              // +0x29
    unsigned char commonattribs;       // +0x2a
    char unknown_2b[8];                // +0x2b
    char help[0x81];                   // +0x33
    unsigned short gaffile;            // +0xb4
};
#pragma pack(pop)

extern char DAT_005119b8[];

char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x4ad350
void __stdcall FUN_004ad350(Common_004ad350* obj, Tree_004ad350* tree)
{
    if (((Class_004c3410*)tree)->FUN_004c3410("COMMON") == 1) {
        obj->id = (unsigned char)tree->current->FUN_004c46c0("id", 0);
        obj->assoc = (unsigned char)tree->current->FUN_004c46c0("assoc", 0);
        ((Class_004c48c0*)tree->current)->FUN_004c48c0(obj->name, "name", 0x10, DAT_005119b8);
        obj->xpos = (short)tree->current->FUN_004c46c0("xpos", 0);
        obj->ypos = (short)tree->current->FUN_004c46c0("ypos", 0);
        obj->width = (short)tree->current->FUN_004c46c0("width", 0);
        obj->height = (short)tree->current->FUN_004c46c0("height", 0);
        obj->attribs = tree->current->FUN_004c46c0("attribs", 0);
        obj->colorf = (unsigned short)tree->current->FUN_004c46c0("colorf", 0);
        obj->colorb = (unsigned short)tree->current->FUN_004c46c0("colorb", 0);
        obj->texturenumber = (unsigned char)tree->current->FUN_004c46c0("texturenumber", 0);
        obj->fontnumber = (unsigned char)tree->current->FUN_004c46c0("fontnumber", 0);
        obj->active = (unsigned char)tree->current->FUN_004c46c0("active", 0);
        obj->commonattribs = (unsigned char)tree->current->FUN_004c46c0("commonattribs", 0);
        ((Class_004c48c0*)tree->current)->FUN_004c48c0(obj->help, "help", 0x80, DAT_005119b8);
        memset(obj->help, 0, 0x81);
        strncpy(obj->help, FUN_004c5740(obj->help), 0x80);
        int gf = tree->current->FUN_004c46c0("gaffile", 0);
        obj->gaffile = (gf ^ obj->gaffile) & 1 ^ obj->gaffile;
    }
}
