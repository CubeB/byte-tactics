// Decompiled by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// Blits one textured span with a depth (Z) buffer. For each pixel it compares
// the interpolated Z against the depth buffer and, when it passes, writes the
// palette-mapped texel and the new depth value. Source coordinates are 16.16
// fixed-point; the u/v/z/light steps are the span deltas divided by the span
// width. The span is clipped to the left edge and to the target surface's
// width. The texture width selects the texel layout: 8/16/32/64/128 pick the
// sub-texel mask, anything else uses the width itself as a row stride. With no
// depth buffer the four power-of-two formats dispatch to the FUN_004cd8xx span
// helpers and the remaining formats run inline loops. value is one
// function-scope temporary shared by every body; giving it a single slot keeps
// the per-body loop counters in the slots the original used.
#include <ddraw.h>
struct Surface_004c8020 {
    unsigned short width;
    char pad[14];
    unsigned char* data;
    unsigned char* depth;
};
struct Display_004c8020 { char pad[0xc4]; unsigned char* palette; };
Display_004c8020* GetDisplay();
void __cdecl FUN_004cd896(unsigned char*, unsigned char*, int, int, int, int, int);
void __cdecl FUN_004cd8da(unsigned char*, unsigned char*, int, int, int, int, int);
void __cdecl FUN_004cd91e(unsigned char*, unsigned char*, int, int, int, int, int);
void __cdecl FUN_004cd962(unsigned char*, unsigned char*, int, int, int, int, int);

// FUNCTION: 0x4c8020
void __stdcall DrawLitTexturedSpan(int row, int* span, Surface_004c8020* target, Surface_004c8020* texture)
{
    unsigned char* dest = target->data;
    unsigned char* depth = target->depth;
    unsigned char* src = texture->data;
    Display_004c8020* display = GetDisplay();
    int width = span[1] - span[0];
    int du = (span[4] - span[2]) / width;
    int dv = (span[5] - span[3]) / width;
    int dz = (span[7] - span[6]) / width;
    int dl = (span[9] - span[8]) / width;
    if (span[0] < 0) {
        span[2] -= du * span[0];
        span[3] -= dv * span[0];
        span[6] -= dz * span[0];
        span[8] -= dl * span[0];
        span[0] = 0;
    }
    if (span[1] > target->width - 1) span[1] = target->width - 1;
    width = span[1] - span[0];
    if (width > 0) {
        int u = span[2];
        int v = span[3];
        int z = span[6];
        int light = span[8];
        int value;
        dest += target->width * row + span[0];
        if (depth) {
            depth += target->width * row + span[0];
            switch(texture->width) {
            case 128: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 9) & ~127)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 64: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 10) & ~63)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 32: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 11) & ~31)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 16: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 12) & ~15)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 8: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 13) & ~7)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            default: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + (v >> 16) * texture->width];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            }
        }
        switch(texture->width) {
        case 128:
            FUN_004cd896(dest, src, width, u, v, du, dv);
        case 64:
            FUN_004cd8da(dest, src, width, u, v, du, dv);
            return;
        case 32:
            FUN_004cd91e(dest, src, width, u, v, du, dv);
            return;
        case 16:
            FUN_004cd962(dest, src, width, u, v, du, dv);
            return;
        case 8: {
            int n=width;
            do {
                *dest++ = src[(u >> 16) + ((v >> 13) & ~7)];
                u += du; v += dv;
            } while (--n);
            return;
        }
        default: {
            int n=width;
            do {
                *dest++ = src[(u >> 16) + (v >> 16) * texture->width];
                u += du; v += dv;
            } while (--n);
            return;
        }
        }
    }
}
