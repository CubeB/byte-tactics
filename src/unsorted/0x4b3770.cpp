// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: 64.2% match. Register allocation and stack layout differences.
#include <string.h>
#include <stdio.h>

struct Table_004b3630 {
    int count;
    void* slots;
};

class Class_004b3630 {
public:
    Table_004b3630* table;
    void FUN_004b3630();
};

struct File_004bb5d0;

void* __stdcall FUN_004bb5b0(void* param1);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);
long __stdcall FUN_004bb710(File_004bb5d0* file, long pos);
long __stdcall FUN_004bb7a0(File_004bb5d0* file);
void __stdcall FUN_004bb7c0(File_004bb5d0* file, void* buf, int size);
long __stdcall FUN_004bbd00(File_004bb5d0* file);
void* __cdecl FUN_004d8450(unsigned int size);
void* __cdecl FUN_004d8460(unsigned int count, unsigned int size);
void* __cdecl FUN_004d8580(void* ptr, unsigned int size);
void FUN_004d85a0(void* p);
int __stdcall FUN_004d1b40(unsigned char* header);
int __stdcall FUN_004d1970(void* dest, void* source);
void* __stdcall FUN_004d1c60(int code);
void __stdcall FUN_004b6290(char* message);
int __stdcall FUN_004b4270(void* self, File_004bb5d0* file, void** buf, void* arg3);
int __cdecl _strcmpi(const char* s1, const char* s2);
int __cdecl sprintf(char* buf, const char* fmt, ...);

class Class_004b3770 : public Class_004b3630 {
public:
    char unknown_4[4];
    int field_8;

    int FUN_004b3770(char* filename, char* name, void* arg3);
};

// FUNCTION: 0x4b3770
int Class_004b3770::FUN_004b3770(char* filename, char* name, void* arg3)
{
    char header[0x22];
    char errmsg[0x100];
    void* buf = 0;
    int bufsize = 0;
    long remaining;
    long pos;
    File_004bb5d0* file;

    file = (File_004bb5d0*)FUN_004bb5b0(filename);
    if (file == 0) {
        return 0;
    }

    FUN_004bb7c0(file, header, 0x22);
    if (strncmp(header, "HAPIBANK", 8) != 0) {
        FUN_004bb5d0(file);
        return 0;
    }

    if (*(int*)(header + 0x14) != 1) {
        FUN_004bb5d0(file);
        return 0;
    }

    pos = FUN_004bbd00(file);
    FUN_004bb710(file, *(int*)(header + 0xc));
    remaining = pos - *(int*)(header + 0xc);

    if (*(unsigned char*)(header + 0x18) != 0) {
        void* src = FUN_004d8450(remaining);
        FUN_004bb7c0(file, src, remaining);
        int dsize = FUN_004d1b40((unsigned char*)src);
        void* dest = FUN_004d8450(dsize);
        int errcode = FUN_004d1970(dest, src);
        if (errcode != 0) {
            char* errstr = (char*)FUN_004d1c60(errcode);
            sprintf(errmsg, "[HapiBank::OpenBank] Decompression Error: %s", errstr);
            FUN_004b6290(errmsg);
        }
        buf = FUN_004d8580(0, dsize);
        memcpy(buf, dest, dsize);
        FUN_004d85a0(dest);
        FUN_004d85a0(src);
        bufsize = dsize;
    } else {
        if (buf != 0) {
            FUN_004d85a0(buf);
        }
        buf = FUN_004d8450(remaining);
        bufsize = remaining;
        FUN_004bb7c0(file, buf, remaining);
    }

    if (name != 0) {
        if (_strcmpi((char*)buf + *(int*)(header + 0x8), name) != 0) {
            FUN_004bb5d0(file);
            if (buf != 0) {
                FUN_004d85a0(buf);
            }
            return 0;
        }
    }

    FUN_004b3630();
    table = (Table_004b3630*)FUN_004d8460(1, 0xc);
    ((int*)table)[2] = -1;

    FUN_004bb710(file, *(int*)(header + 0x10));
    pos = FUN_004bb7a0(file);
    if (pos < *(int*)(header + 0xc)) {
        do {
            FUN_004b4270(this, file, &buf, arg3);
            pos = FUN_004bb7a0(file);
        } while (pos < *(int*)(header + 0xc));
    }

    FUN_004bb5d0(file);
    if (buf != 0) {
        FUN_004d85a0(buf);
    }
    return 1;
}
