// MovementClass: one of the 32 entries of the movement class table at
// 0x512358: the name, the footprint, the water depths and slopes read from
// moveinfo.tdf (the TDF keys FootPrintX, FootPrintZ, maxwaterdepth,
// minwaterdepth, maxslope, badslope, maxwaterslope and badwaterslope), and the
// 2-bit-per-cell passability map it owns. The one declaration of the class,
// for movement_class.cpp, which loads it and defines the methods, and every
// file that reads a field or refreshes its pass map. The types behind the
// pointers stay private to their own files.
#ifndef MOVEMENT_CLASS_H
#define MOVEMENT_CLASS_H

struct Point16;
struct Source_00440340;
struct Object_00440af0;

class MovementClass {
public:
    int* name;                         // +0x00
    short footprintX;                  // +0x04
    short footprintZ;                  // +0x06
    short maxWaterDepth;               // +0x08
    short minWaterDepth;               // +0x0a
    unsigned char maxSlope;            // +0x0c
    unsigned char badSlope;            // +0x0d
    unsigned char maxWaterSlope;       // +0x0e
    unsigned char badWaterSlope;       // +0x0f
    unsigned int width;                // +0x10, of the pass map
    unsigned int height;               // +0x14
    unsigned int* cells;               // +0x18
    unsigned int lastTick;             // +0x1c

    MovementClass();
    ~MovementClass();
    void ReadMoveInfo(Source_00440340* src);
    void ResizePassMap(unsigned int w, unsigned int h);
    void SetPassMapCell(int param_1, int param_2, int param_3);
    void RefreshPassMap(Point16 a, Point16 b);
    void RefreshMovedUnits(Object_00440af0* p);
    void RefreshUnitIfStale(Object_00440af0* p);
    int InBounds(unsigned int x, unsigned int y)
    {
        return x < width && y < height;
    }
    int Get(int x, int y)
    {
        return (cells[width * (y >> 4) + x] >> ((y & 0xf) << 1)) & 3;
    }
};

#endif
