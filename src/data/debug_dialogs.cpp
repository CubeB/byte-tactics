// The dialog templates of Cavedog's debug library, compiled into the game as
// data: 0x4da480 copies one into global memory by its resource id (0x81,
// 0x66, 0x67) for DialogBoxIndirectParam. Each is a DLGTEMPLATE followed by
// its title, its font and one DLGITEMTEMPLATE per control, every control on
// a four-byte boundary, as a resource compiler lays them out.

#include <windows.h>

#pragma pack(push, 2)

struct AssertDialog {
    DLGTEMPLATE dialog;
    WORD menu;                         // none
    WORD windowClass;                  // the standard dialog class
    WCHAR title[37];
    WORD pointSize;
    WCHAR font[14];
    WORD align0;
    DLGITEMTEMPLATE item0;          // static 0xffff
    WORD class0[2];
    WCHAR text0[24];
    WORD extra0;
    DLGITEMTEMPLATE item1;          // edit 0x3ef
    WORD class1[2];
    WCHAR text1[1];
    WORD extra1;
    WORD align2;
    DLGITEMTEMPLATE item2;          // static 0x3f4
    WORD class2[2];
    WCHAR text2[56];
    WORD extra2;
    DLGITEMTEMPLATE item3;          // edit 0x3f0
    WORD class3[2];
    WCHAR text3[1];
    WORD extra3;
    WORD align4;
    DLGITEMTEMPLATE item4;          // button 0x3ec
    WORD class4[2];
    WCHAR text4[6];
    WORD extra4;
    DLGITEMTEMPLATE item5;          // button 0x3
    WORD class5[2];
    WCHAR text5[7];
    WORD extra5;
    WORD align6;
    DLGITEMTEMPLATE item6;          // button 0x3f1
    WORD class6[2];
    WCHAR text6[6];
    WORD extra6;
    DLGITEMTEMPLATE item7;          // button 0x3ed
    WORD class7[2];
    WCHAR text7[7];
    WORD extra7;
    WORD align8;
    DLGITEMTEMPLATE item8;          // button 0x3f2
    WORD class8[2];
    WCHAR text8[14];
    WORD extra8;
    DLGITEMTEMPLATE item9;          // button 0x3ee
    WORD class9[2];
    WCHAR text9[15];
    WORD extra9;
    WORD align10;
    DLGITEMTEMPLATE item10;          // button 0x3f3
    WORD class10[2];
    WCHAR text10[13];
    WORD extra10;
    WORD align11;
    DLGITEMTEMPLATE item11;          // button 0x1
    WORD class11[2];
    WCHAR text11[13];
    WORD extra11;
    WORD align12;
    DLGITEMTEMPLATE item12;          // button 0x3fc
    WORD class12[2];
    WCHAR text12[9];
    WORD extra12;
    WORD align13;
    DLGITEMTEMPLATE item13;          // button 0x3fd
    WORD class13[2];
    WCHAR text13[9];
    WORD extra13;
    WORD align14;
    DLGITEMTEMPLATE item14;          // button 0x3fe
    WORD class14[2];
    WCHAR text14[9];
    WORD extra14;
    WORD align15;
    DLGITEMTEMPLATE item15;          // button 0x3ff
    WORD class15[2];
    WCHAR text15[9];
    WORD extra15;
    WORD align16;
    DLGITEMTEMPLATE item16;          // button 0x3fa
    WORD class16[2];
    WCHAR text16[6];
    WORD extra16;
    BYTE following[24];       // past the template: the next resource's first bytes
};

// IDD 0x81: the assertion report, with Copy, Abort, Save, Debug, Save && Mail, Ignore buttons.
// GLOBAL: 0x50c958
AssertDialog g_assertDialog = {
    {0x80c000c0, 0x0, 17, 0, 0, 373, 212},
    0,
    0,
    L"Cavedog Entertainment Assert Display",
    8,
    L"MS Sans Serif",
    0,
    {0x50020000, 0x0, 7, 5, 76, 8, 0xffff},
    {0xffff, 0x82},
    L"Debug Assertion Failed!",
    0,
    {0x50a00804, 0x0, 7, 17, 359, 92, 0x3ef},
    {0xffff, 0x81},
    L"",
    0,
    0,
    {0x50020000, 0x0, 7, 114, 176, 8, 0x3f4},
    {0xffff, 0x82},
    L"Describe &under what circumstances this assert appears:",
    0,
    {0x50a11004, 0x0, 7, 127, 359, 38, 0x3f0},
    {0xffff, 0x81},
    L"",
    0,
    0,
    {0x50010000, 0x0, 7, 171, 50, 14, 0x3ec},
    {0xffff, 0x80},
    L"&Copy",
    0,
    {0x50010000, 0x0, 7, 191, 50, 14, 0x3},
    {0xffff, 0x80},
    L"&Abort",
    0,
    0,
    {0x50010000, 0x0, 77, 171, 50, 14, 0x3f1},
    {0xffff, 0x80},
    L"&Save",
    0,
    {0x50010000, 0x0, 77, 191, 50, 14, 0x3ed},
    {0xffff, 0x80},
    L"&Debug",
    0,
    0,
    {0x50010000, 0x0, 147, 171, 50, 14, 0x3f2},
    {0xffff, 0x80},
    L"Save && &Mail",
    0,
    {0x50010000, 0x0, 147, 191, 50, 14, 0x3ee},
    {0xffff, 0x80},
    L"Ignore Al&ways",
    0,
    0,
    {0x50010000, 0x0, 217, 171, 50, 14, 0x3f3},
    {0xffff, 0x80},
    L"Disable A&ll",
    0,
    0,
    {0x50010001, 0x0, 217, 191, 50, 14, 0x1},
    {0xffff, 0x80},
    L"&Ignore Once",
    0,
    0,
    {0x50010000, 0x0, 273, 167, 28, 14, 0x3fc},
    {0xffff, 0x80},
    L"Abort &2",
    0,
    0,
    {0x50010000, 0x0, 273, 184, 28, 14, 0x3fd},
    {0xffff, 0x80},
    L"Abort &3",
    0,
    0,
    {0x50010000, 0x0, 306, 167, 28, 14, 0x3fe},
    {0xffff, 0x80},
    L"Abort &4",
    0,
    0,
    {0x50010000, 0x0, 338, 167, 28, 14, 0x3ff},
    {0xffff, 0x80},
    L"Abort &5",
    0,
    0,
    {0x50010000, 0x0, 316, 191, 50, 14, 0x3fa},
    {0xffff, 0x80},
    L"&Help",
    0,
    {0xc0, 0x00, 0xc8, 0x88, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xde, 0x00, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00},
};

struct MemoryDialog {
    DLGTEMPLATE dialog;
    WORD menu;                         // none
    WORD windowClass;                  // the standard dialog class
    WCHAR title[22];
    WORD pointSize;
    WCHAR font[8];
    DLGITEMTEMPLATE item0;          // button 0x1
    WORD class0[2];
    WCHAR text0[6];
    WORD extra0;
    DLGITEMTEMPLATE item1;          // edit 0x3e8
    WORD class1[2];
    WCHAR text1[1];
    WORD extra1;
    WORD align2;
    DLGITEMTEMPLATE item2;          // button 0x3e9
    WORD class2[2];
    WCHAR text2[14];
    WORD extra2;
    DLGITEMTEMPLATE item3;          // button 0x3ea
    WORD class3[2];
    WCHAR text3[15];
    WORD extra3;
    WORD align4;
    DLGITEMTEMPLATE item4;          // button 0x3eb
    WORD class4[2];
    WCHAR text4[10];
    WORD extra4;
    DLGITEMTEMPLATE item5;          // button 0x3f9
    WORD class5[2];
    WCHAR text5[13];
    WORD extra5;
    WORD align6;
    DLGITEMTEMPLATE item6;          // button 0x3fa
    WORD class6[2];
    WCHAR text6[6];
    WORD extra6;
    BYTE following[28];       // past the template: the next resource's first bytes
};

// IDD 0x66: the memory status window.
// GLOBAL: 0x50cd38
MemoryDialog g_memoryDialog = {
    {0x88c800c0, 0x0, 7, 0, 0, 222, 169},
    0,
    0,
    L"Cavedog Memory Status",
    10,
    L"Courier",
    {0x50010001, 0x0, 158, 7, 57, 14, 0x1},
    {0xffff, 0x80},
    L"Close",
    0,
    {0x50810804, 0x0, 7, 7, 146, 155, 0x3e8},
    {0xffff, 0x81},
    L"",
    0,
    0,
    {0x50010000, 0x0, 158, 78, 57, 14, 0x3e9},
    {0xffff, 0x80},
    L"&Dump details",
    0,
    {0x50010000, 0x0, 158, 60, 57, 14, 0x3ea},
    {0xffff, 0x80},
    L"&Reset details",
    0,
    0,
    {0x50010000, 0x0, 158, 34, 57, 14, 0x3eb},
    {0xffff, 0x80},
    L"Reset all",
    0,
    {0x50010003, 0x0, 158, 98, 60, 10, 0x3f9},
    {0xffff, 0x80},
    L"&Working set",
    0,
    0,
    {0x50010000, 0x0, 165, 148, 50, 14, 0x3fa},
    {0xffff, 0x80},
    L"&Help",
    0,
    {0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xc8, 0x88, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x01, 0x00, 0x02, 0x00, 0x78, 0x01, 0x9d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00},
};

struct ProfilerDialog {
    DLGTEMPLATE dialog;
    WORD menu;                         // none
    WORD windowClass;                  // the standard dialog class
    WCHAR title[31];
    WORD pointSize;
    WCHAR font[14];
    WORD align0;
    DLGITEMTEMPLATE item0;          // static 0x3f5
    WORD class0[2];
    WCHAR text0[10];
    WORD extra0;
    DLGITEMTEMPLATE item1;          // combo box 0x3ed
    WORD class1[2];
    WCHAR text1[1];
    WORD extra1;
    WORD align2;
    DLGITEMTEMPLATE item2;          // static 0x3f3
    WORD class2[2];
    WCHAR text2[10];
    WORD extra2;
    DLGITEMTEMPLATE item3;          // combo box 0x3ee
    WORD class3[2];
    WCHAR text3[1];
    WORD extra3;
    WORD align4;
    DLGITEMTEMPLATE item4;          // button 0x3ef
    WORD class4[2];
    WCHAR text4[18];
    WORD extra4;
    DLGITEMTEMPLATE item5;          // button 0x3f1
    WORD class5[2];
    WCHAR text5[16];
    WORD extra5;
    DLGITEMTEMPLATE item6;          // button 0x3f6
    WORD class6[2];
    WCHAR text6[21];
    WORD extra6;
    WORD align7;
    DLGITEMTEMPLATE item7;          // button 0x3f7
    WORD class7[2];
    WCHAR text7[14];
    WORD extra7;
    DLGITEMTEMPLATE item8;          // button 0x3f8
    WORD class8[2];
    WCHAR text8[14];
    WORD extra8;
    DLGITEMTEMPLATE item9;          // button 0x1
    WORD class9[2];
    WCHAR text9[6];
    WORD extra9;
    DLGITEMTEMPLATE item10;          // list box 0x3f4
    WORD class10[2];
    WCHAR text10[1];
    WORD extra10;
    WORD align11;
    DLGITEMTEMPLATE item11;          // edit 0x3f0
    WORD class11[2];
    WCHAR text11[1];
    WORD extra11;
    WORD align12;
    DLGITEMTEMPLATE item12;          // button 0x3fa
    WORD class12[2];
    WCHAR text12[6];
    WORD extra12;
    BYTE following[28];       // past the template: the next resource's first bytes
};

// IDD 0x67: the performance counter window (see perf_counters.cpp).
// GLOBAL: 0x50ced8
ProfilerDialog g_profilerDialog = {
    {0x88c800c0, 0x0, 13, 1, 2, 376, 157},
    0,
    0,
    L"Cavedog Performance Monitoring",
    8,
    L"MS Sans Serif",
    0,
    {0x50020000, 0x0, 7, 9, 28, 8, 0x3f5},
    {0xffff, 0x82},
    L"Event &1:",
    0,
    {0x50210002, 0x0, 35, 7, 131, 166, 0x3ed},
    {0xffff, 0x85},
    L"",
    0,
    0,
    {0x50020000, 0x0, 7, 25, 28, 8, 0x3f3},
    {0xffff, 0x82},
    L"Event &2:",
    0,
    {0x50210002, 0x0, 35, 24, 132, 165, 0x3ee},
    {0xffff, 0x85},
    L"",
    0,
    0,
    {0x50010003, 0x0, 170, 8, 65, 10, 0x3ef},
    {0xffff, 0x80},
    L"&Enable profiling",
    0,
    {0x50010003, 0x0, 170, 25, 56, 10, 0x3f1},
    {0xffff, 0x80},
    L"&Raise priority",
    0,
    {0x50010003, 0x0, 236, 8, 78, 10, 0x3f6},
    {0xffff, 0x80},
    L"Display in &debugger",
    0,
    0,
    {0x50010003, 0x0, 236, 25, 55, 10, 0x3f7},
    {0xffff, 0x80},
    L"Display h&ere",
    0,
    {0x50010003, 0x0, 316, 8, 53, 10, 0x3f8},
    {0xffff, 0x80},
    L"&Auto pairing",
    0,
    {0x50010001, 0x0, 319, 40, 50, 14, 0x1},
    {0xffff, 0x80},
    L"Close",
    0,
    {0x50a10101, 0x0, 7, 57, 105, 93, 0x3f4},
    {0xffff, 0x83},
    L"",
    0,
    0,
    {0x50810884, 0x0, 118, 57, 251, 93, 0x3f0},
    {0xffff, 0x81},
    L"",
    0,
    0,
    {0x50010000, 0x0, 261, 40, 50, 14, 0x3fa},
    {0xffff, 0x80},
    L"&Help",
    0,
    {0x00, 0x00, 0x00, 0x00, 0xc8, 0x02, 0x34, 0x00, 0x00, 0x00, 0x56, 0x00, 0x53, 0x00, 0x5f, 0x00, 0x56, 0x00, 0x45, 0x00, 0x52, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4f, 0x00},
};

#pragma pack(pop)
