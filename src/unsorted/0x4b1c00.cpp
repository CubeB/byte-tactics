// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: Register allocation mismatch. Original uses edi for 'this', compiler uses ebx.
// Stack frame 0x1c vs 0x18. 12.3% match.
#include <stdlib.h>

class Class_004b1c00 {
public:
    char unknown_0[8];
    void* field_8;                       // +0x8: pointer to object with count at +8
    char unknown_c[0x14 - 0xc];
    int* field_14;                       // +0x14: array of ints
    int field_18;                        // +0x18: flag

    virtual void vtbl_0(int outer, int inner, int value);
    virtual void vtbl_1(int outer, int inner, int value);
    virtual int vtbl_5(int outer, int inner);
    virtual int vtbl_6(int outer, int inner);

    void FUN_004b1c00(int param_1);
};

// FUNCTION: 0x4b1c00
void Class_004b1c00::FUN_004b1c00(int param_1)
{
    if (param_1 == 0)
        return;
    if (this->field_18 == 0)
        return;
    this->field_18 = 0;

    int count = ((int*)this->field_8)[2];
    if (count <= 0)
        return;

    int local_14 = 0;
    int local_10 = 0xd;

    for (int ebp = 0; ebp < count; ebp++) {
        char* array = (char*)this->field_14;

        if (*(int*)(array + local_14) != 0) {
            *(int*)(array + local_14) = 0;

            for (int ebx = 0; ebx <= 2; ebx++) {
                int esi = local_14 + 0x28 + ebx * 4;

                if (*(int*)(array + esi - 0x18) != 0) {
                    int eax = this->vtbl_5(ebp, ebx) + param_1 * *(int*)(array + esi - 0x18);
                    int edx = *(int*)(array + (local_10 + ebx - 0xc) * 4);
                    if (*(int*)(array + esi - 0x18) > 0) {
                        if (eax < edx) {
                            *(int*)(array + local_14) = 1;
                        } else {
                            eax = edx;
                            *(int*)(array + esi - 0x18) = 0;
                        }
                    } else {
                        if (eax > edx) {
                            *(int*)(array + local_14) = 1;
                        } else {
                            eax = edx;
                            *(int*)(array + esi - 0x18) = 0;
                        }
                    }
                    this->vtbl_0(ebp, ebx, eax);
                }

                if (*(int*)(array + esi + 0x18) != 0) {
                    *(int*)(array + esi) += *(int*)(array + esi + 0x18);
                    int ecx = *(int*)(array + (local_10 + ebx) * 4);
                    if (*(int*)(array + esi + 0x18) > 0) {
                        if (*(int*)(array + esi) >= ecx) {
                            *(int*)(array + esi) = ecx;
                            *(int*)(array + esi + 0x18) = 0;
                        }
                    } else {
                        if (*(int*)(array + esi) <= ecx) {
                            *(int*)(array + esi) = ecx;
                            *(int*)(array + esi + 0x18) = 0;
                        }
                    }
                }

                if (*(int*)(array + esi) != 0) {
                    int ebx_result = this->vtbl_6(ebp, ebx);
                    int eax = *(int*)(array + esi);
                    int ecx = param_1 * eax;
                    ebx_result += ecx;
                    int edx = *(int*)(array + (ebx + local_10 - 6) * 4);
                    if (edx != -1) {
                        if (eax > 0) {
                            int diff = abs(edx - ebx_result + 0x10000) & 0xffff;
                            if (diff > ecx) {
                                *(int*)(array + local_14) = 1;
                            } else {
                                *(int*)(array + esi) = 0;
                                ebx_result = edx;
                            }
                        } else {
                            int diff = abs(ebx_result - edx + 0x10000) & 0xffff;
                            if (diff > -ecx) {
                                *(int*)(array + local_14) = 1;
                            } else {
                                *(int*)(array + esi) = 0;
                                ebx_result = edx;
                            }
                        }
                    } else {
                        *(int*)(array + local_14) = 1;
                    }
                    this->vtbl_1(ebp, ebx, ebx_result & 0xffff);
                }
            }
        }

        if (*(int*)(array + local_14) != 0) {
            this->field_18 = 1;
        }
        local_14 += 0x4c;
        local_10 += 0x13;
    }
}
