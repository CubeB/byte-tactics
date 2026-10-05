// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Writes one radar surface into the chunked file writer: an 8-byte header of
// width and height, then one row per scan line. The read counterpart is
// 0x4c6f80, which allocates a surface of width*height and reads the rows back.
// The surface layout matches the one built by 0x4c69f0.
// <ddraw.h> decides the multiply operand order (tools/headers.py stops at the
// C runtime headers and never tries it).
#include <ddraw.h>

struct Surface_004c6f10 {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
    char* data;                        // +0xc
};

class HapiBank {
public:
    void SeekBox(int pos);
    int WriteBox(void* src, int len);
};

// FUNCTION: 0x4c6f10
void __stdcall SaveSurface(Surface_004c6f10* surface, HapiBank* file)
{
    ((HapiBank*)file)->SeekBox(0);
    int header[2];
    header[0] = surface->width;
    header[1] = surface->height;
    file->WriteBox(header, 8);
    for (int i = 0; i < header[1]; i++) {
        file->WriteBox(surface->data + i * surface->pitch, header[0]);
    }
}
