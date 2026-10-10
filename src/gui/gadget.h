// Gadget: one 0x15b-byte GUI entry (Thaldren's GuiGadget), the control record
// of a dialog's entry table: its type, geometry and flags in the first 0x1b
// bytes and, from +0xb6 on, the entry-0 fields (the count, the dialog's assets
// and the names) plus the readings the union keeps for the other entry kinds
// (a list entry, a hotspot, a progress bar, a slider and a text input). The one
// declaration of the type, for the files that define or read it; the types
// behind the pointers stay private to their own files.
#ifndef GADGET_H
#define GADGET_H

#pragma pack(push, 1)

struct GafEntry;
struct GafFrame;
struct Gui;

struct Gadget {                         // 0x15b bytes, one GUI list entry
    unsigned char type;                 // +0x00
    unsigned char team;                 // +0x01
    char name[0x11];                    // +0x02 (strncpy 0x10)
    short x;                            // +0x13
    short y;                            // +0x15
    short width;                        // +0x17
    short height;                       // +0x19
    int attribs;                        // +0x1b
    int colours;                        // +0x1f
    int image;                          // +0x23
    char textureNumber;                 // +0x27, the GUI file texturenumber
    signed char tab;                    // +0x28
    unsigned char active;               // +0x29
    char commonAttribs;                 // +0x2a, bit 2: a unit entry, bit 3: the stockpile entry
    void* archive;                      // +0x2b
    GafEntry* gaf;                      // +0x2f
    char helpKey[0x60 - 0x33];          // +0x33
    int index;                          // +0x60
    char unknown_64[0xb4 - 0x64];
    unsigned short shown : 1;           // +0xb4 bit 0
    unsigned short bits_b4 : 15;
    union {
        short count;                    // +0xb6 (entry 0 only)
        char text[0x80];                // +0xb6
        struct {                        // entry 0
            char unknown_b6[2];
            void* saveUnder;            // +0xb8, the SAVE UNDER bitmap
            void* surface;              // +0xbc, the GUI SURFACE bitmap
            void* archive;              // +0xc0
            GafEntry* background;       // +0xc4
        } assets;
        struct {                        // type 12
            char unknown_b6[2];
            GafFrame* glyph;                    // +0xb8
            unsigned short flag;        // +0xbc, bit 0 set by type 12
        } frame;
        struct {                        // type 2
            int sortKey;                // +0xb6
            short selected;             // +0xba, the selected line
            short firstRow;             // +0xbc, the first line shown
            short maxFirstRow;          // +0xbe, the largest value of firstRow
            short rowCount;             // +0xc0, the number of lines or items
            char* field_c2;             // +0xc2, the text lines
            void* rows;                 // +0xc6, the cell or item array
            GafEntry* gaf;              // +0xca
            void (__stdcall* callback)(Gui*, Gadget*);  // +0xce
            void* records;              // +0xd2, the record the entry is bound to
            union {
                int language;           // +0xd6
                void* filebuf;          // +0xd6, buffer type 7/8 loads
            };
            short scroll;               // +0xda
        } list;
        struct {                        // entry 0
            char unknown_b6[0x16];
            char choice[0x10];          // +0xcc
            char choice2[0x10];         // +0xdc
            char focusName[0x10];       // +0xec
        } names;
        struct {                        // type 6, a hotspot
            void (__stdcall* callback)(Gui*, Gadget*);  // +0xb6
            char unknown_ba[4];
            GafEntry* gaf;              // +0xbe
            GafFrame* frame;            // +0xc2
            short index;                // +0xc6
            unsigned char flags;        // +0xc8
        } hotspot;
        struct {                        // type 13, a progress bar
            int total;                  // +0xb6, the divisor of the fill
            int value;                  // +0xba
            int max;                    // +0xbe
            int interval;               // +0xc2
            int next;                   // +0xc6
            float step;                 // +0xca
            int active;                 // +0xce
            int showText;               // +0xd2
        } anim;
    } u;
    union {                             // +0x136
        short range;
        struct {
            unsigned char stages;       // +0x136
            unsigned char stageIndex;   // +0x137
        };
    };
    short status;                       // +0x138 (a text input's length limit, the GUI file's maxchars)
    union {                             // +0x13a
        struct {
            unsigned char quickKey;     // +0x13a
            unsigned char frameBase;    // +0x13b, the frame its images start at
            union {                     // +0x13c
                unsigned char grayedout;
                unsigned short flag;    // bit 0 set by type 1
                struct {
                    unsigned short flag_bit : 1;
                    unsigned short unknown_13c_1 : 15;
                };
                int max;
            };
        };
        GafEntry* inputGaf;
        struct {
            char unknown_13a_b[0x14e - 0x13a];
            GafEntry* sliderGaf;        // +0x14e
            unsigned char sliderStyle;  // +0x152
        };
        struct {
            char unknown_13a_c[0x140 - 0x13a];
            short knobPos;              // +0x140
            short knobSize;             // +0x142
            void (__stdcall* sliderCallback)(Gui*, int);      // +0x144
            short unknown_148;
            int sliderUser;             // +0x14a
        };
        struct {
            char unknown_13a_d[0x147 - 0x13a];
            unsigned char hotkey;       // +0x147, the quick key of a type 5 label
            unsigned int flag_148;      // +0x148, bit 0 set by type 5
        };
    };
    char unknown_153[0x157 - 0x153];
    int sliderLocked;                   // +0x157, set by SetGadgetGrayedOut for type 4
};

#pragma pack(pop)

#endif
