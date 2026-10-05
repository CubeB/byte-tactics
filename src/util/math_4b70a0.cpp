// Decompiled by Claude Opus 5.5. Names are provisional.
//
// Fixed-point trigonometry, hand-written in assembly: every routine has a
// MASM-style frame (`push ebp / mov ebp, esp ... leave / ret`), saves the
// registers it uses, and the routines follow one another with no alignment,
// so they are one naked function here with a public label for each entry
// point. Angles are 16-bit (0x10000 is a full turn); the sine table holds
// 1024 entries of sin * 0x2000 (the cosine table is the same, a quarter turn
// on), so a lookup is the angle's top 10 bits.
//
// int  FUN_004b70a0(unsigned short angle)        sine table entry (sin * 0x2000)
// int  FUN_004b70c0(unsigned short angle)        cosine table entry
// int  FUN_004b70e0(int a, int b)                (a * b) >> 32
// int  FUN_004b70ef(short angle, int scale)      scale * sin(angle), rounded
// int  FUN_004b7123(short angle, int scale)      scale * cos(angle), rounded
// int  FUN_004b715a(int x, int z)                atan2(z, x) as an angle
// void FUN_004b7173(short angle, int* xz)        rotate the point xz by angle
// void FUN_004b71a7(short* angles)               build the rotation matrix DAT_0050a400
//                                                from the angles at +0xc, +0xe, +0x10
// void FUN_004b72e2(int* in, int* out)           out = DAT_0050a400 applied to in
// int  FUN_004b7381(int a, int b, int c)         a * b / c, or 0 when c is 0

extern "C" short DAT_00509f00[];        // sine table
extern "C" short DAT_0050a000[];        // the sine table from a quarter turn on (cosine)
extern "C" double DAT_00509ef0;         // 0x8000 / pi: radians to angle units
extern "C" double DAT_00509ef8;         // pi / 0x8000: angle units to radians
extern "C" int DAT_0050a400[9];         // the rotation matrix FUN_004b71a7 builds

// FUNCTION: 0x4b70a0
extern "C" __declspec(naked) int __cdecl FUN_004b70a0(unsigned short angle)
{
    __asm {
        push ebp
        mov ebp, esp
        push esi
        push ebx
        lea esi, DAT_00509f00
        movzx ebx, word ptr [ebp + 8]
        shr ebx, 6
        and ebx, 0xfffe
        movsx eax, word ptr [ebx + esi]
        pop ebx
        pop esi
        leave
        ret

        // ENTRY: 0x4b70c0
    L004b70c0:
        push ebp
        mov ebp, esp
        push esi
        push ebx
        lea esi, DAT_0050a000
        movzx ebx, word ptr [ebp + 8]
        shr ebx, 6
        and ebx, 0xfffe
        movsx eax, word ptr [ebx + esi]
        pop ebx
        pop esi
        leave
        ret

        // ENTRY: 0x4b70e0
        push ebp
        mov ebp, esp
        push edx
        mov eax, [ebp + 8]
        imul dword ptr [ebp + 0xc]
        mov eax, edx
        pop edx
        leave
        ret

        // ENTRY: 0x4b70ef
        push ebp
        mov ebp, esp
        push esi
        push ebx
        push edx
        lea esi, DAT_00509f00
        movsx ebx, word ptr [ebp + 8]
        add ebx, 0x20
        shr ebx, 6
        and ebx, 0x3fe
        movsx eax, word ptr [ebx + esi]
        imul dword ptr [ebp + 0xc]
        add eax, 0x1000
        adc edx, 0
        shrd eax, edx, 13
        pop edx
        pop ebx
        pop esi
        leave
        ret

        // ENTRY: 0x4b7123
        push ebp
        mov ebp, esp
        push esi
        push ebx
        push edx
        lea esi, DAT_00509f00
        movzx ebx, word ptr [ebp + 8]
        add ebx, 0x4020
        shr ebx, 6
        and ebx, 0x3fe
        movsx eax, word ptr [ebx + esi]
        imul dword ptr [ebp + 0xc]
        add eax, 0x1000
        adc edx, 0
        shrd eax, edx, 13
        pop edx
        pop ebx
        pop esi
        leave
        ret

        // ENTRY: 0x4b715a
        push ebp
        mov ebp, esp
        fild dword ptr [ebp + 8]
        fild dword ptr [ebp + 0xc]
        fpatan
        fmul DAT_00509ef0
        fistp dword ptr [ebp + 8]
        mov eax, [ebp + 8]
        leave
        ret

        // ENTRY: 0x4b7173
        push ebp
        mov ebp, esp
        cmp word ptr [ebp + 8], 0
        je L004b71a5
        mov eax, [ebp + 0xc]
        fild dword ptr [eax + 4]
        fild dword ptr [eax]
        fld st(1)
        fld st(1)
        fild word ptr [ebp + 8]
        fmul DAT_00509ef8
        fsincos
        fmul st(5), st
        fmulp st(4), st
        fmul st(2), st
        fmulp st(1), st
        faddp st(3), st
        fsubp st(1), st
        fistp dword ptr [eax]
        fistp dword ptr [eax + 4]
    L004b71a5:
        leave
        ret

        // ENTRY: 0x4b71a7
        push ebp
        mov ebp, esp
        add esp, -0x18
        push esi
        push edi
        push ebx
        push edx
        mov esi, [ebp + 8]
        push word ptr [esi + 0xe]
        call L004b70c0
        mov [ebp - 8], eax
        mov edi, eax
        push word ptr [esi + 0x10]
        call L004b70c0
        add sp, 4
        mov [ebp - 0xc], eax
        imul edi
        shrd eax, edx, 13
        mov DAT_0050a400, eax
        push word ptr [esi + 0xc]
        call L004b70c0
        mov [ebp - 4], eax
        mov edi, eax
        push word ptr [esi + 0x10]
        call FUN_004b70a0
        add sp, 4
        mov [ebp - 0x18], eax
        imul edi
        shrd eax, edx, 13
        push eax
        push word ptr [esi + 0xc]
        call FUN_004b70a0
        mov [ebp - 0x10], eax
        mov edi, eax
        push word ptr [esi + 0xe]
        call FUN_004b70a0
        add sp, 4
        mov [ebp - 0x14], eax
        imul edi
        shrd eax, edx, 13
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 13
        pop ebx
        add eax, ebx
        mov DAT_0050a400[4], eax
        mov eax, [ebp - 0x10]
        imul dword ptr [ebp - 0x18]
        shrd eax, edx, 13
        mov edi, eax
        mov eax, [ebp - 4]
        imul dword ptr [ebp - 0x14]
        shrd eax, edx, 13
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 13
        sub edi, eax
        mov DAT_0050a400[8], edi
        mov eax, [ebp - 8]
        imul dword ptr [ebp - 0x18]
        shrd eax, edx, 13
        neg eax
        mov DAT_0050a400[12], eax
        mov eax, [ebp - 4]
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 13
        mov edi, eax
        mov eax, [ebp - 0x10]
        imul dword ptr [ebp - 0x14]
        shrd eax, edx, 13
        imul dword ptr [ebp - 0x18]
        shrd eax, edx, 13
        sub edi, eax
        mov DAT_0050a400[16], edi
        mov eax, [ebp - 0x10]
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 13
        mov edi, eax
        mov eax, [ebp - 4]
        imul dword ptr [ebp - 0x14]
        shrd eax, edx, 13
        imul dword ptr [ebp - 0x18]
        shrd eax, edx, 13
        add eax, edi
        mov DAT_0050a400[20], eax
        mov eax, [ebp - 0x14]
        mov DAT_0050a400[24], eax
        mov eax, [ebp - 0x10]
        imul dword ptr [ebp - 8]
        shrd eax, edx, 13
        neg eax
        mov DAT_0050a400[28], eax
        mov eax, [ebp - 4]
        imul dword ptr [ebp - 8]
        shrd eax, edx, 13
        mov DAT_0050a400[32], eax
        pop edx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4b72e2
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ecx
        push edx
        mov esi, [ebp + 8]
        mov edi, [ebp + 0xc]
        mov eax, [esi]
        imul DAT_0050a400
        shrd eax, edx, 13
        mov ecx, eax
        mov eax, [esi + 4]
        imul DAT_0050a400[12]
        shrd eax, edx, 13
        add ecx, eax
        mov eax, [esi + 8]
        imul DAT_0050a400[24]
        shrd eax, edx, 13
        add eax, ecx
        mov [edi], eax
        mov eax, [esi]
        imul DAT_0050a400[4]
        shrd eax, edx, 13
        mov ecx, eax
        mov eax, [esi + 4]
        imul DAT_0050a400[16]
        shrd eax, edx, 13
        add ecx, eax
        mov eax, [esi + 8]
        imul DAT_0050a400[28]
        shrd eax, edx, 13
        add eax, ecx
        mov [edi + 4], eax
        mov eax, [esi]
        imul DAT_0050a400[8]
        shrd eax, edx, 13
        mov ecx, eax
        mov eax, [esi + 4]
        imul DAT_0050a400[20]
        shrd eax, edx, 13
        add ecx, eax
        mov eax, [esi + 8]
        imul DAT_0050a400[32]
        shrd eax, edx, 13
        add eax, ecx
        mov [edi + 8], eax
        pop edx
        pop ecx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4b7381
        push ebp
        mov ebp, esp
        push ebx
        push ecx
        push edx
        mov ecx, [ebp + 0x10]
        or ecx, ecx
        je L004b739d
        mov ebx, [ebp + 0xc]
        mov eax, [ebp + 8]
        imul ebx
        idiv ecx
        pop edx
        pop ecx
        pop ebx
        leave
        ret
    L004b739d:
        xor eax, eax
        pop edx
        pop ecx
        pop ebx
        leave
        ret
    }
}
