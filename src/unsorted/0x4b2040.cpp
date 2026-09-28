// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: Complex function with virtual calls and stream reading. Could not match.
#include <string.h>

struct ArrayB {
    char unknown_0[8];
    int value;                          // +0x8
    char unknown_c[8];
};

struct ArrayA {
    char unknown_0[0xc];
    int subIndex;                       // +0xc
    char unknown_10[4];
    ArrayB* ptr2;                       // +0x14
};

struct Table_004b4bf0 {
    char unknown_0[4];
    ArrayA* base;                       // +0x4
    int index;                          // +0x8
};

class Class_004b4bf0 {
public:
    Table_004b4bf0* table;              // +0x00
    int FUN_004b4bf0();
};

struct Chunk_004b4c10 {
    char unknown_0[8];
    int size;                           // +0x08
    int pos;                            // +0x0c
    char* data;                         // +0x10
};

struct Slot_004b4c10 {
    char unknown_0[0xc];
    int current;                        // +0x0c
    char unknown_10[4];
    Chunk_004b4c10* chunks;             // +0x14
};

struct Table_004b4c10 {
    char unknown_0[4];
    Slot_004b4c10* slots;               // +0x04
    int index;                          // +0x08
};

class Class_004b4c10 {
public:
    Table_004b4c10* table;              // +0x00
    void FUN_004b4c10(int pos);
};

struct Chunk_004b4c80 {
    char unknown_0[8];
    int size;                           // +0x08
    int pos;                            // +0x0c
    char* data;                         // +0x10
};

struct Slot_004b4c80 {
    char unknown_0[0xc];
    int current;                        // +0x0c
    char unknown_10[4];
    Chunk_004b4c80* chunks;             // +0x14
};

struct Table_004b4c80 {
    char unknown_0[4];
    Slot_004b4c80* slots;               // +0x04
    int index;                          // +0x08
};

class Class_004b4c80 {
public:
    Table_004b4c80* table;              // +0x00
    int FUN_004b4c80(void* dst, int len);
};

int* FUN_004d83b0(unsigned int param_1, unsigned int param_2);
void FUN_004d85a0(int* param_1);

class Class_004b2040 {
public:
    void* vtbl;                         // +0x00
    char unknown_4[4];                  // +0x04
    int field_8;                        // +0x08
    int field_c;                        // +0x0c
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    int field_18;                       // +0x18
    int field_1c[0x520 / 4];            // +0x1c..0x53c
    int field_53c;                      // +0x53c

    int FUN_004b2040(void* src);
};

typedef void (*VirtFn0)(Class_004b2040*);
typedef void (*VirtFn1)(Class_004b2040*, int, int);
typedef void (*VirtFn2)(Class_004b2040*, int, int);
typedef void (*VirtFn3)(Class_004b2040*, int, int);
typedef void (*VirtFn4)(Class_004b2040*, int, int, int);
typedef void (*VirtFn5)(Class_004b2040*, int, int, int);

// FUNCTION: 0x4b2040
int Class_004b2040::FUN_004b2040(void* src)
{
    char local_buf[0x528];              // +0x20..0x548

    int* obj = (int*)this->field_8;
    int n1 = obj[4];                    // +0x10
    int n2 = obj[2];                    // +0x8
    int size1 = n1 * 4;
    int size2 = n2 * 108;
    int total = size1 + size2 + 0x528;

    if (((Class_004b4bf0*)src)->FUN_004b4bf0() != total) {
        return 0;
    }

    ((Class_004b4c10*)src)->FUN_004b4c10(0);

    if (((Class_004b4c80*)src)->FUN_004b4c80(local_buf, 0x528) != 0x528) {
        return 0;
    }

    if (this->field_c != (int)local_buf[0]) {
        return 0;
    }

    int* dst = &this->field_1c[0];
    int* srcp = (int*)&local_buf[4];
    for (int i = 0; i < 8; i++) {
        memcpy(dst, srcp, 0xa4);
        dst[0x20 / 4] = 0;
        dst += 0xa4 / 4;
        srcp += 0xa4 / 4;
    }

    this->field_53c = (int)this;

    int result = ((Class_004b4c80*)src)->FUN_004b4c80((void*)this->field_10, size1);
    if (result != size1) {
        return 0;
    }

    int* pieceStates = FUN_004d83b0(size2, (unsigned int)"Piece States");

    result = ((Class_004b4c80*)src)->FUN_004b4c80((void*)pieceStates, size2);
    if (result != size2) {
        return 0;
    }

    int count = obj[2];                 // +0x8
    int* ptr = (int*)pieceStates;
    int* data = (int*)this->field_14;
    int idx = 0;
    int i;
    for (i = 0; i < count; i++) {
        data[idx] = 1;
        ((VirtFn1*)vtbl)[2](this, ptr[-1], i);
        ((VirtFn2*)vtbl)[3](this, ptr[0], i);
        ((VirtFn3*)vtbl)[4](this, ptr[1], i);
        for (int k = 0; k < 2; k++) {
            data[idx + 0xc] = ptr[-3];
            data[idx] = ptr[0];
            data[idx + 0xc] = ptr[3];
            data[idx + 0x18] = ptr[6];
            data[idx + 0x24] = ptr[9];
            data[idx + 0x30] = ptr[12];
            ((VirtFn4*)vtbl)[0](this, ptr[15], k, i);
            ((VirtFn5*)vtbl)[1](this, ptr[18], k, i);
        }
        idx += 0x4c;
        ptr += 0x6c;
    }

    this->field_18 = 1;
    FUN_004d85a0((int*)pieceStates);
    return 1;
}
