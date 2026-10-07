// Decompiled by Claude Opus 5.5. Names are provisional.
//
// Surface drawing, hand-written in assembly: blitters, run-length sprite
// blitters, lines, rectangle fills and a glyph blitter, all __cdecl with
// MASM-style frames (`push ebp / mov ebp, esp / add esp, -N ... leave / ret`)
// that save every register they use.
//
// 0x4cbbe0  BlitSurface(dst, src, x, y)                   blit a bitmap at (x, y), clipped
// 0x4cbcd5  FUN_004cbcd5(dst, src, x, y, key)              the same, skipping the key colour
// 0x4cbdd1  FUN_004cbdd1(dst, src, rect, pos)              copy a rectangle
// 0x4cbe70  FUN_004cbe70(dst, src, rect, pos, key)         copy a rectangle, skipping the key colour
// 0x4cbef1  FUN_004cbef1(dst, x, y, tile)                  copy a 32 x 32 tile to (x, y)
// 0x4cbf2c  FUN_004cbf2c(dst, src, rect, pos, key, table)  dst = table[src][dst], skipping the key
// 0x4cbfc4  FUN_004cbfc4(dst, src, rect, pos, key, table)  dst = table[dst] under each non-key pixel
// 0x4cc057, 0x4cc1bf, 0x4cc3d0, 0x4cc51d                    run-length sprite blitters
//           (pixels, pitch, drect, runs, srect[, table]): each row of runs follows
//           its byte length (a word); a run's count byte says skip, copy or repeat
// 0x4cc332  FUN_004cc332(dst, src, rect, pos, key, table)  dst = table[src - 0x4f][dst],
//                                                           skipping the key
// 0x4cc650  IsLineVisible(surface, x0, y0, x1, y1)          0 when the segment misses the surface
// 0x4cc7ab  FUN_004cc7ab(surface, x0, y0, x1, y1, color)   draw a line
// 0x4cc8df  FUN_004cc8df(surface, x0, y0, x1, y1, ..., table)   draw a line through a table
// 0x4cca33  (x0, y0, x1, y1, color)                         16-bit line; no callers, and it
//                                                           hands IsLineVisible no surface
// 0x4ccb65  (surface in esi)                                 no callers
// 0x4ccd1c  FUN_004ccd1c(surface, rect, color)             rectangle outline, four FUN_004cc7ab
// 0x4ccd85  (rect, color)                                   the same through 0x4cca33; no callers
// 0x4ccdea  FUN_004ccdea(surface, rect, color)             fill a rectangle
// 0x4cce87  FUN_004cce87(surface, rect, value)             xor a rectangle with a byte
// 0x4cced5  FUN_004cced5(pixels, pitch, w, h, table)       translate pixels through a table
// 0x4ccf60  BlitText(pixels, pitch, font, text, x, y, c1, c2, c3)   glyph blitter
// 0x4cd010  FUN_004cd010(dst, src, x0, y0, x1, y1, step)   no callers
// 0x4cd896..0x4cd962  (dest, src, width, y, x, rowstep, colstep)  sample a span of src into dest

// FUNCTION: 0x4cbbe0
extern "C" __declspec(naked) void __cdecl BlitSurface(void* dst, void* src, int x, int y)
{
    __asm {
        push ebp
        mov ebp, esp
        add esp, -0x1c
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        mov edi, dword ptr [ebp + 8]
        mov eax, dword ptr [edi + 8]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [edi]
        mov dword ptr [ebp - 0x1c], eax
        cmp eax, dword ptr [ebp + 0x10]
        jle L004cbccd
        mov eax, dword ptr [edi + 4]
        mov dword ptr [ebp - 8], eax
        cmp eax, dword ptr [ebp + 0x14]
        jle L004cbccd
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 0xc], ecx
        mov ecx, dword ptr [esi]
        mov dword ptr [ebp - 0x18], ecx
        mov eax, dword ptr [ebp + 0x10]
        neg eax
        cmp eax, ecx
        jge L004cbccd
        mov ecx, dword ptr [esi + 4]
        mov dword ptr [ebp - 0x10], ecx
        mov eax, dword ptr [ebp + 0x14]
        neg eax
        cmp eax, ecx
        jge L004cbccd
        mov esi, dword ptr [esi + 0xc]
        mov edi, dword ptr [edi + 0xc]
        mov ebx, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp - 0x18]
        mov eax, dword ptr [ebp + 0x10]
        cmp eax, 0
        jl L004cbc5b
        sub ebx, eax
        add edi, eax
        jmp L004cbc61
    L004cbc5b:
        add ecx, eax
        neg eax
        add esi, eax
    L004cbc61:
        cmp ecx, ebx
        jle L004cbc67
        mov ecx, ebx
    L004cbc67:
        mov dword ptr [ebp - 0x14], ecx
        mov ebx, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 0x10]
        mov eax, dword ptr [ebp + 0x14]
        cmp eax, 0
        jl L004cbc81
        sub ebx, eax
        mul dword ptr [ebp - 4]
        add edi, eax
        jmp L004cbc8a
    L004cbc81:
        add ecx, eax
        neg eax
        mul dword ptr [ebp - 0xc]
        add esi, eax
    L004cbc8a:
        cmp ecx, ebx
        jle L004cbc90
        mov ecx, ebx
    L004cbc90:
        mov eax, ecx
        mov edx, dword ptr [ebp - 0xc]
        sub edx, dword ptr [ebp - 0x14]
        mov ebx, dword ptr [ebp - 4]
        sub ebx, dword ptr [ebp - 0x14]
        mov ecx, edi
        and ecx, 3
        jne L004cbcb3
        mov ecx, dword ptr [ebp - 0x14]
        and ecx, 3
        jne L004cbcb3
        shr dword ptr [ebp - 0x14], 2
        jmp L004cbcc1
    L004cbcb3:
        mov ecx, dword ptr [ebp - 0x14]
        rep movsb
        add esi, edx
        add edi, ebx
        dec eax
        jne L004cbcb3
        jmp L004cbccd
    L004cbcc1:
        mov ecx, dword ptr [ebp - 0x14]
        rep movsd
        add esi, edx
        add edi, ebx
        dec eax
        jne L004cbcc1
    L004cbccd:
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cbcd5
    L004cbcd5:
        push ebp
        mov ebp, esp
        add esp, -0x24
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        mov edi, dword ptr [ebp + 8]
        mov eax, dword ptr [edi + 8]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [edi]
        mov dword ptr [ebp - 0x20], eax
        cmp eax, dword ptr [ebp + 0x10]
        jle L004cbdc9
        mov eax, dword ptr [edi + 4]
        mov dword ptr [ebp - 8], eax
        cmp eax, dword ptr [ebp + 0x14]
        jle L004cbdc9
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 0x10], ecx
        mov ecx, dword ptr [esi]
        mov dword ptr [ebp - 0x1c], ecx
        mov eax, dword ptr [ebp + 0x10]
        neg eax
        cmp eax, ecx
        jge L004cbdc9
        mov ecx, dword ptr [esi + 4]
        mov dword ptr [ebp - 0x14], ecx
        mov eax, dword ptr [ebp + 0x14]
        neg eax
        cmp eax, ecx
        jge L004cbdc9
        mov esi, dword ptr [esi + 0xc]
        mov edi, dword ptr [edi + 0xc]
        mov ebx, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp - 0x1c]
        mov eax, dword ptr [ebp + 0x10]
        cmp eax, 0
        jl L004cbd50
        sub ebx, eax
        add edi, eax
        jmp L004cbd56
    L004cbd50:
        add ecx, eax
        neg eax
        add esi, eax
    L004cbd56:
        cmp ecx, ebx
        jle L004cbd5c
        mov ecx, ebx
    L004cbd5c:
        mov dword ptr [ebp - 0x24], ecx
        mov ebx, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 0x14]
        mov eax, dword ptr [ebp + 0x14]
        cmp eax, 0
        jl L004cbd76
        sub ebx, eax
        mul dword ptr [ebp - 4]
        add edi, eax
        jmp L004cbd7f
    L004cbd76:
        add ecx, eax
        neg eax
        mul dword ptr [ebp - 0x10]
        add esi, eax
    L004cbd7f:
        cmp ecx, ebx
        jle L004cbd85
        mov ecx, ebx
    L004cbd85:
        mov dword ptr [ebp - 0xc], ecx
        mov ebx, dword ptr [ebp - 4]
        sub ebx, dword ptr [ebp - 0x24]
        mov eax, dword ptr [ebp - 0x10]
        sub eax, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0x18], eax
        mov al, byte ptr [ebp + 0x18]
        mov dl, al
        shl dx, 8
        mov dl, al
        shl edx, 0x10
        mov dl, al
        shl dx, 8
        mov dl, al
    L004cbdad:
        mov ecx, dword ptr [ebp - 0x24]
    L004cbdb0:
        mov al, byte ptr [esi]
        _emit 0x38      // cmp al, dl, as r/m8, r8 (the inline assembler only encodes r8, r/m8)
        _emit 0xd0
        je L004cbdb8
        mov byte ptr [edi], al
    L004cbdb8:
        inc esi
        inc edi
        dec ecx
        jne L004cbdb0
        add esi, dword ptr [ebp - 0x18]
        add edi, ebx
        dec dword ptr [ebp - 0xc]
        jne L004cbdad
        jmp L004cbdc9
    L004cbdc9:
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cbdd1
    L004cbdd1:
        push ebp
        mov ebp, esp
        add esp, -0x10
        push esi
        push edi
        push ebx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [esi + 8]
        sub ecx, dword ptr [esi]
        inc ecx
        mov dword ptr [ebp - 0xc], ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        inc eax
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [esi + 4]
        mov ebx, dword ptr [esi]
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 4], ecx
        mov esi, dword ptr [esi + 0xc]
        mul ecx
        add eax, ebx
        add esi, eax
        mov eax, dword ptr [edi + 4]
        mov ebx, dword ptr [edi]
        mov edi, dword ptr [ebp + 8]
        mov ecx, dword ptr [edi + 8]
        mov dword ptr [ebp - 8], ecx
        mov edi, dword ptr [edi + 0xc]
        mul ecx
        add eax, ebx
        add edi, eax
        mov ecx, dword ptr [ebp - 0xc]
        mov eax, dword ptr [ebp - 4]
        sub eax, ecx
        mov ebx, dword ptr [ebp - 8]
        sub ebx, ecx
        mov edx, dword ptr [ebp - 0x10]
        shr ecx, 1
        jb L004cbe5e
        mov dword ptr [ebp - 0xc], ecx
        shr ecx, 1
        jb L004cbe4f
        mov dword ptr [ebp - 0xc], ecx
    L004cbe41:
        mov ecx, dword ptr [ebp - 0xc]
        rep movsd
        add esi, eax
        add edi, ebx
        dec edx
        jne L004cbe41
        jmp L004cbe6a
    L004cbe4f:
        mov ecx, dword ptr [ebp - 0xc]
        rep movsw
        add esi, eax
        add edi, ebx
        dec edx
        jne L004cbe4f
        jmp L004cbe6a
    L004cbe5e:
        mov ecx, dword ptr [ebp - 0xc]
        rep movsb
        add esi, eax
        add edi, ebx
        dec edx
        jne L004cbe5e
    L004cbe6a:
        popfd
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cbe70
    L004cbe70:
        push ebp
        mov ebp, esp
        add esp, -0x10
        push esi
        push edi
        push ebx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [esi + 8]
        sub ecx, dword ptr [esi]
        inc ecx
        mov dword ptr [ebp - 0xc], ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        inc eax
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [esi + 4]
        mov ebx, dword ptr [esi]
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 4], ecx
        mov esi, dword ptr [esi + 0xc]
        mul ecx
        add eax, ebx
        add esi, eax
        mov eax, dword ptr [edi + 4]
        mov ebx, dword ptr [edi]
        mov edi, dword ptr [ebp + 8]
        mov ecx, dword ptr [edi + 8]
        mov dword ptr [ebp - 8], ecx
        mov edi, dword ptr [edi + 0xc]
        mul ecx
        add eax, ebx
        add edi, eax
        mov ecx, dword ptr [ebp - 0xc]
        mov edx, dword ptr [ebp - 4]
        sub edx, ecx
        mov ebx, dword ptr [ebp - 8]
        sub ebx, ecx
        mov ah, byte ptr [ebp + 0x18]
    L004cbed2:
        mov ecx, dword ptr [ebp - 0xc]
    L004cbed5:
        mov al, byte ptr [esi]
        _emit 0x38      // cmp al, ah, as r/m8, r8 (the inline assembler only encodes r8, r/m8)
        _emit 0xe0
        je L004cbedd
        mov byte ptr [edi], al
    L004cbedd:
        inc esi
        inc edi
        dec ecx
        jne L004cbed5
        add esi, edx
        add edi, ebx
        dec dword ptr [ebp - 0x10]
        jne L004cbed2
        popfd
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cbef1
    L004cbef1:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0x14]
        mov edi, dword ptr [ebp + 8]
        mov eax, dword ptr [edi + 8]
        mov ebx, eax
        mul dword ptr [ebp + 0x10]
        add eax, dword ptr [ebp + 0xc]
        mov edi, dword ptr [edi + 0xc]
        add edi, eax
        mov edx, 0x20
        sub ebx, edx
    L004cbf18:
        mov ecx, 8
        rep movsd
        add edi, ebx
        dec edx
        jne L004cbf18
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cbf2c
    L004cbf2c:
        push ebp
        mov ebp, esp
        add esp, -0x18
        push esi
        push edi
        push ebx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [esi + 8]
        sub ecx, dword ptr [esi]
        inc ecx
        mov dword ptr [ebp - 0xc], ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        inc eax
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [esi + 4]
        mov ebx, dword ptr [esi]
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 4], ecx
        mov esi, dword ptr [esi + 0xc]
        mul ecx
        add eax, ebx
        add esi, eax
        mov eax, dword ptr [edi + 4]
        mov ebx, dword ptr [edi]
        mov edi, dword ptr [ebp + 8]
        mov ecx, dword ptr [edi + 8]
        mov dword ptr [ebp - 8], ecx
        mov edi, dword ptr [edi + 0xc]
        mul ecx
        add eax, ebx
        add edi, eax
        mov ecx, dword ptr [ebp - 0xc]
        mov edx, dword ptr [ebp - 4]
        sub edx, ecx
        mov dword ptr [ebp - 0x14], edx
        mov ebx, dword ptr [ebp - 8]
        sub ebx, ecx
        mov dword ptr [ebp - 0x18], ebx
        mov edx, dword ptr [ebp + 0x1c]
        xor eax, eax
    L004cbf96:
        mov ecx, dword ptr [ebp - 0xc]
    L004cbf99:
        mov al, byte ptr [esi]
        cmp al, byte ptr [ebp + 0x18]
        je L004cbfae
        mov ebx, eax
        shl ebx, 8
        mov al, byte ptr [edi]
        add ebx, edx
        mov al, byte ptr [eax + ebx]
        mov byte ptr [edi], al
    L004cbfae:
        inc esi
        inc edi
        dec ecx
        jne L004cbf99
        add esi, dword ptr [ebp - 0x14]
        add edi, dword ptr [ebp - 0x18]
        dec dword ptr [ebp - 0x10]
        jne L004cbf96
        popfd
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cbfc4
    L004cbfc4:
        push ebp
        mov ebp, esp
        add esp, -0x18
        push esi
        push edi
        push ebx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [esi + 8]
        sub ecx, dword ptr [esi]
        inc ecx
        mov dword ptr [ebp - 0xc], ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        inc eax
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [esi + 4]
        mov ebx, dword ptr [esi]
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 4], ecx
        mov esi, dword ptr [esi + 0xc]
        mul ecx
        add eax, ebx
        add esi, eax
        mov eax, dword ptr [edi + 4]
        mov ebx, dword ptr [edi]
        mov edi, dword ptr [ebp + 8]
        mov ecx, dword ptr [edi + 8]
        mov dword ptr [ebp - 8], ecx
        mov edi, dword ptr [edi + 0xc]
        mul ecx
        add eax, ebx
        add edi, eax
        mov ecx, dword ptr [ebp - 0xc]
        mov edx, dword ptr [ebp - 4]
        sub edx, ecx
        mov dword ptr [ebp - 0x14], edx
        mov ebx, dword ptr [ebp - 8]
        sub ebx, ecx
        mov dword ptr [ebp - 0x18], ebx
        mov edx, dword ptr [ebp + 0x1c]
        xor eax, eax
        mov bh, byte ptr [ebp + 0x18]
    L004cc031:
        mov ecx, dword ptr [ebp - 0xc]
    L004cc034:
        mov al, byte ptr [esi]
        _emit 0x38      // cmp al, bh, as r/m8, r8 (the inline assembler only encodes r8, r/m8)
        _emit 0xf8
        je L004cc041
        mov al, byte ptr [edi]
        mov bl, byte ptr [eax + edx]
        mov byte ptr [edi], bl
    L004cc041:
        inc esi
        inc edi
        dec ecx
        jne L004cc034
        add esi, dword ptr [ebp - 0x14]
        add edi, dword ptr [ebp - 0x18]
        dec dword ptr [ebp - 0x10]
        jne L004cc031
        popfd
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cc057
    L004cc057:
        push ebp
        mov ebp, esp
        add esp, -4
        push ebx
        push ecx
        push esi
        push edi
        push ebp
        mov edi, dword ptr [ebp + 8]
        mov esi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebx + 4]
        imul ecx
        add edi, dword ptr [ebx]
        add edi, eax
        mov edx, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [edx + 4]
        or ebx, ebx
    L004cc07f:
        je L004cc08c
        movzx eax, word ptr [esi]
        lea esi, [eax + esi + 2]
        dec ebx
        jmp L004cc07f
        ALIGN 4
    L004cc08c:
        mov ebx, dword ptr [edx + 0xc]
        sub ebx, dword ptr [edx + 4]
        mov dword ptr [ebp - 4], ebx
        mov ebx, dword ptr [edx + 8]
        jl L004cc0a9
        mov edx, dword ptr [edx]
        sub ebx, edx
        inc ebx
        jle L004cc0a9
        jmp L004cc0b5
        ALIGN 4
    L004cc0a4:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc0a9:
        pop ebp
        pop edi
        pop esi
        pop ecx
        pop ebx
        leave
        ret
    L004cc0b0:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc0b5:
        movzx eax, word ptr [esi]
        push edx
        push ecx
        push ebx
        add ecx, edi
        push ecx
        lea ecx, [eax + esi + 2]
        push ecx
        mov ecx, dword ptr [ebp - 4]
        or ecx, ecx
        js L004cc0a4
        dec dword ptr [ebp - 4]
        add esi, 2
        or eax, eax
        je L004cc0b0
        or edx, edx
        jle L004cc114
    L004cc0d8:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc0ec
        sub edx, eax
        je L004cc114
        jg L004cc0d8
        mov eax, edx
        neg eax
        jmp L004cc120
    L004cc0ec:
        shr eax, 1
        jae L004cc100
        inc eax
        inc esi
        sub edx, eax
        je L004cc114
        jg L004cc0d8
        mov eax, edx
        dec esi
        neg eax
        jmp L004cc12d
        ALIGN 4
    L004cc100:
        inc eax
        add esi, eax
        sub edx, eax
        je L004cc114
        jg L004cc0d8
        add esi, edx
        mov eax, edx
        neg eax
        jmp L004cc175
        ALIGN 4
    L004cc114:
        or ebx, ebx
        jle L004cc0b0
    L004cc118:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc128
    L004cc120:
        add edi, eax
        sub ebx, eax
        jle L004cc0b0
        jmp L004cc118
    L004cc128:
        shr eax, 1
        jae L004cc174
        inc eax
    L004cc12d:
        sub ebx, eax
        jl L004cc150
        mov ecx, eax
        xor edx, edx
        mov dl, byte ptr [esi]
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        inc esi
    L004cc13e:
        xor eax, eax
        mov al, byte ptr [edi]
        add eax, edx
        mov al, byte ptr [eax]
        mov byte ptr [edi], al
        inc edi
        sub ecx, 1
        jne L004cc13e
        jmp L004cc114
    L004cc150:
        add eax, ebx
        mov ecx, eax
        xor edx, edx
        mov dl, byte ptr [esi]
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        inc esi
    L004cc15f:
        xor eax, eax
        mov al, byte ptr [edi]
        add eax, edx
        mov al, byte ptr [eax]
        mov byte ptr [edi], al
        inc edi
        sub ecx, 1
        jne L004cc15f
        jmp L004cc114
        ALIGN 4
    L004cc174:
        inc eax
    L004cc175:
        sub ebx, eax
        jl L004cc19c
        mov ecx, eax
    L004cc17b:
        xor edx, edx
        xor eax, eax
        mov dl, byte ptr [esi]
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        mov al, byte ptr [edi]
        mov al, byte ptr [edx + eax]
        mov byte ptr [edi], al
        inc esi
        inc edi
        sub ecx, 1
        jne L004cc17b
        _emit 0xe9      // jmp L004cc114: near jump (the inline assembler leaves the ALIGN padding out when it sizes it)
        _emit 0x7a
        _emit 0xff
        _emit 0xff
        _emit 0xff
        ALIGN 4
    L004cc19c:
        add eax, ebx
        mov ecx, eax
    L004cc1a0:
        xor edx, edx
        xor eax, eax
        mov dl, byte ptr [esi]
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        mov al, byte ptr [edi]
        mov al, byte ptr [edx + eax]
        mov byte ptr [edi], al
        inc esi
        inc edi
        sub ecx, 1
        jne L004cc1a0
        jmp L004cc114

        // ENTRY: 0x4cc1bf
    L004cc1bf:
        push ebp
        mov ebp, esp
        add esp, -4
        push ebx
        push ecx
        push esi
        push edi
        push ebp
        mov edi, dword ptr [ebp + 8]
        mov esi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebx + 4]
        imul ecx
        add edi, dword ptr [ebx]
        add edi, eax
        mov edx, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [edx + 4]
        or ebx, ebx
    L004cc1e7:
        je L004cc1f4
        movzx eax, word ptr [esi]
        lea esi, [eax + esi + 2]
        dec ebx
        jmp L004cc1e7
        ALIGN 4
    L004cc1f4:
        mov ebx, dword ptr [edx + 0xc]
        sub ebx, dword ptr [edx + 4]
        mov dword ptr [ebp - 4], ebx
        mov ebx, dword ptr [edx + 8]
        jl L004cc211
        mov edx, dword ptr [edx]
        sub ebx, edx
        inc ebx
        jle L004cc211
        jmp L004cc21d
        ALIGN 4
    L004cc20c:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc211:
        pop ebp
        pop edi
        pop esi
        pop ecx
        pop ebx
        leave
        ret
    L004cc218:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc21d:
        movzx eax, word ptr [esi]
        push edx
        push ecx
        push ebx
        add ecx, edi
        push ecx
        lea ecx, [eax + esi + 2]
        push ecx
        mov ecx, dword ptr [ebp - 4]
        or ecx, ecx
        js L004cc20c
        dec dword ptr [ebp - 4]
        add esi, 2
        or eax, eax
        je L004cc218
        or edx, edx
        jle L004cc27c
    L004cc240:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc254
        sub edx, eax
        je L004cc27c
        jg L004cc240
        mov eax, edx
        neg eax
        jmp L004cc288
    L004cc254:
        shr eax, 1
        jae L004cc268
        inc eax
        inc esi
        sub edx, eax
        je L004cc27c
        jg L004cc240
        mov eax, edx
        dec esi
        neg eax
        jmp L004cc295
        ALIGN 4
    L004cc268:
        inc eax
        add esi, eax
        sub edx, eax
        je L004cc27c
        jg L004cc240
        add esi, edx
        mov eax, edx
        neg eax
        jmp L004cc2e1
        ALIGN 4
    L004cc27c:
        or ebx, ebx
        jle L004cc218
    L004cc280:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc290
    L004cc288:
        add edi, eax
        sub ebx, eax
        jle L004cc218
        jmp L004cc280
    L004cc290:
        shr eax, 1
        jae L004cc2e0
        inc eax
    L004cc295:
        sub ebx, eax
        jl L004cc2bc
        mov ecx, eax
        xor edx, edx
        mov dl, byte ptr [esi]
        sub edx, 0x4f
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        inc esi
    L004cc2a9:
        xor eax, eax
        mov al, byte ptr [edi]
        add eax, edx
        mov al, byte ptr [eax]
        mov byte ptr [edi], al
        inc edi
        sub ecx, 1
        jne L004cc2a9
        jmp L004cc27c
        ALIGN 4
    L004cc2bc:
        add eax, ebx
        mov ecx, eax
        xor edx, edx
        mov dl, byte ptr [esi]
        sub edx, 0x4f
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        inc esi
    L004cc2ce:
        xor eax, eax
        mov al, byte ptr [edi]
        add eax, edx
        mov al, byte ptr [eax]
        mov byte ptr [edi], al
        inc edi
        sub ecx, 1
        jne L004cc2ce
        jmp L004cc27c
    L004cc2e0:
        inc eax
    L004cc2e1:
        sub ebx, eax
        jl L004cc30c
        mov ecx, eax
    L004cc2e7:
        xor edx, edx
        xor eax, eax
        mov dl, byte ptr [esi]
        sub edx, 0x4f
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        mov al, byte ptr [edi]
        mov al, byte ptr [edx + eax]
        mov byte ptr [edi], al
        inc esi
        inc edi
        sub ecx, 1
        jne L004cc2e7
        jmp L004cc27c
        ALIGN 4
    L004cc30c:
        add eax, ebx
        mov ecx, eax
    L004cc310:
        xor edx, edx
        xor eax, eax
        mov dl, byte ptr [esi]
        sub edx, 0x4f
        shl edx, 8
        add edx, dword ptr [ebp + 0x1c]
        mov al, byte ptr [edi]
        mov al, byte ptr [edx + eax]
        mov byte ptr [edi], al
        inc esi
        inc edi
        sub ecx, 1
        jne L004cc310
        jmp L004cc27c

        // ENTRY: 0x4cc332
    L004cc332:
        push ebp
        mov ebp, esp
        add esp, -0x18
        push esi
        push edi
        push ebx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [esi + 8]
        sub ecx, dword ptr [esi]
        inc ecx
        mov dword ptr [ebp - 0xc], ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        inc eax
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [esi + 4]
        mov ebx, dword ptr [esi]
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 4], ecx
        mov esi, dword ptr [esi + 0xc]
        mul ecx
        add eax, ebx
        add esi, eax
        mov eax, dword ptr [edi + 4]
        mov ebx, dword ptr [edi]
        mov edi, dword ptr [ebp + 8]
        mov ecx, dword ptr [edi + 8]
        mov dword ptr [ebp - 8], ecx
        mov edi, dword ptr [edi + 0xc]
        mul ecx
        add eax, ebx
        add edi, eax
        mov ecx, dword ptr [ebp - 0xc]
        mov edx, dword ptr [ebp - 4]
        sub edx, ecx
        mov dword ptr [ebp - 0x14], edx
        mov ebx, dword ptr [ebp - 8]
        sub ebx, ecx
        mov dword ptr [ebp - 0x18], ebx
        mov edx, dword ptr [ebp + 0x1c]
        xor eax, eax
    L004cc39c:
        mov ecx, dword ptr [ebp - 0xc]
    L004cc39f:
        mov al, byte ptr [esi]
        cmp al, byte ptr [ebp + 0x18]
        je L004cc3b7
        mov ebx, eax
        sub ebx, 0x4f
        shl ebx, 8
        add ebx, edx
        mov al, byte ptr [edi]
        mov al, byte ptr [eax + ebx]
        mov byte ptr [edi], al
    L004cc3b7:
        inc esi
        inc edi
        dec ecx
        jne L004cc39f
        add esi, dword ptr [ebp - 0x14]
        add edi, dword ptr [ebp - 0x18]
        dec dword ptr [ebp - 0x10]
        jne L004cc39c
        popfd
        pop ebx
        pop edi
        pop esi
        leave
        ret
    }
}

// FUNCTION: 0x4cc3d0
extern "C" __declspec(naked) void __cdecl BlitCompressedLit(unsigned char* pixels, int pitch, void* drect, void* runs,
                                             void* srect, unsigned char* table)
{
    __asm {
        push ebp
        mov ebp, esp
        add esp, -4
        push ebx
        push ecx
        push esi
        push edi
        push ebp
        mov edi, dword ptr [ebp + 8]
        mov esi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebx + 4]
        imul ecx
        add edi, dword ptr [ebx]
        add edi, eax
        mov edx, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [edx + 4]
        or ebx, ebx
    L004cc3f8:
        je L004cc404
        movzx eax, word ptr [esi]
        lea esi, [eax + esi + 2]
        dec ebx
        jmp L004cc3f8
    L004cc404:
        mov ebx, dword ptr [edx + 0xc]
        sub ebx, dword ptr [edx + 4]
        mov dword ptr [ebp - 4], ebx
        mov ebx, dword ptr [edx + 8]
        jl L004cc421
        mov edx, dword ptr [edx]
        sub ebx, edx
        inc ebx
        jle L004cc421
        jmp L004cc42d
        ALIGN 4
    L004cc41c:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc421:
        pop ebp
        pop edi
        pop esi
        pop ecx
        pop ebx
        leave
        ret
    L004cc428:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc42d:
        movzx eax, word ptr [esi]
        push edx
        push ecx
        push ebx
        add ecx, edi
        push ecx
        lea ecx, [eax + esi + 2]
        push ecx
        mov ecx, dword ptr [ebp - 4]
        or ecx, ecx
        js L004cc41c
        dec dword ptr [ebp - 4]
        add esi, 2
        or eax, eax
        je L004cc428
        or edx, edx
        jle L004cc48c
    L004cc450:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc464
        sub edx, eax
        je L004cc48c
        jg L004cc450
        mov eax, edx
        neg eax
        jmp L004cc498
    L004cc464:
        shr eax, 1
        jae L004cc478
        inc eax
        inc esi
        sub edx, eax
        je L004cc48c
        jg L004cc450
        mov eax, edx
        dec esi
        neg eax
        jmp L004cc4a5
        ALIGN 4
    L004cc478:
        inc eax
        add esi, eax
        sub edx, eax
        je L004cc48c
        jg L004cc450
        add esi, edx
        mov eax, edx
        neg eax
        jmp L004cc4e1
        ALIGN 4
    L004cc48c:
        or ebx, ebx
        jle L004cc428
    L004cc490:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc4a0
    L004cc498:
        add edi, eax
        sub ebx, eax
        jle L004cc428
        jmp L004cc490
    L004cc4a0:
        shr eax, 1
        jae L004cc4e0
        inc eax
    L004cc4a5:
        sub ebx, eax
        jl L004cc4c4
        mov ecx, eax
        xor edx, edx
        mov dl, byte ptr [esi]
        add edx, dword ptr [ebp + 0x1c]
        xor eax, eax
        mov al, byte ptr [edx]
        inc esi
    L004cc4b7:
        mov byte ptr [edi], al
        inc edi
        sub ecx, 1
        jne L004cc4b7
        jmp L004cc48c
        ALIGN 4
    L004cc4c4:
        add eax, ebx
        mov ecx, eax
        xor edx, edx
        mov dl, byte ptr [esi]
        add edx, dword ptr [ebp + 0x1c]
        xor eax, eax
        mov al, byte ptr [edx]
        inc esi
    L004cc4d4:
        mov byte ptr [edi], al
        inc edi
        sub ecx, 1
        jne L004cc4d4
        jmp L004cc48c
        ALIGN 4
    L004cc4e0:
        inc eax
    L004cc4e1:
        sub ebx, eax
        jl L004cc500
        mov ecx, eax
    L004cc4e7:
        xor edx, edx
        xor eax, eax
        mov dl, byte ptr [esi]
        add edx, dword ptr [ebp + 0x1c]
        mov al, byte ptr [edx]
        mov byte ptr [edi], al
        inc esi
        inc edi
        sub ecx, 1
        jne L004cc4e7
        jmp L004cc48c
        ALIGN 4
    L004cc500:
        add eax, ebx
        mov ecx, eax
    L004cc504:
        xor edx, edx
        xor eax, eax
        mov dl, byte ptr [esi]
        add edx, dword ptr [ebp + 0x1c]
        mov al, byte ptr [edx]
        mov byte ptr [edi], al
        inc esi
        inc edi
        sub ecx, 1
        jne L004cc504
        jmp L004cc48c

        // ENTRY: 0x4cc51d
    L004cc51d:
        push ebp
        mov ebp, esp
        push ebx
        push ecx
        push esi
        push edi
        push ebp
        mov edi, dword ptr [ebp + 8]
        mov esi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebx + 4]
        imul ecx
        add edi, dword ptr [ebx]
        add edi, eax
        mov edx, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [edx + 4]
        or ebx, ebx
    L004cc542:
        je L004cc550
        movzx eax, word ptr [esi]
        lea esi, [eax + esi + 2]
        dec ebx
        jmp L004cc542
        ALIGN 4
    L004cc550:
        mov ebx, dword ptr [edx + 8]
        mov ebp, dword ptr [edx + 0xc]
        sub ebp, dword ptr [edx + 4]
        jl L004cc569
        mov edx, dword ptr [edx]
        sub ebx, edx
        inc ebx
        jle L004cc569
        jmp L004cc575
    L004cc564:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc569:
        pop ebp
        pop edi
        pop esi
        pop ecx
        pop ebx
        leave
        ret
    L004cc570:
        pop esi
        pop edi
        pop ebx
        pop ecx
        pop edx
    L004cc575:
        movzx eax, word ptr [esi]
        push edx
        push ecx
        push ebx
        add ecx, edi
        push ecx
        lea ecx, [eax + esi + 2]
        push ecx
        or ebp, ebp
        js L004cc564
        dec ebp
        add esi, 2
        or eax, eax
        je L004cc570
        or edx, edx
        jle L004cc5d0
        ALIGN 4
    L004cc594:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc5a8
        sub edx, eax
        je L004cc5d0
        jg L004cc594
        mov eax, edx
        neg eax
        jmp L004cc5dc
    L004cc5a8:
        shr eax, 1
        jae L004cc5bc
        inc eax
        inc esi
        sub edx, eax
        je L004cc5d0
        jg L004cc594
        mov eax, edx
        dec esi
        neg eax
        jmp L004cc5e9
        ALIGN 4
    L004cc5bc:
        inc eax
        add esi, eax
        sub edx, eax
        je L004cc5d0
        jg L004cc594
        add esi, edx
        mov eax, edx
        neg eax
        jmp L004cc619
        ALIGN 4
    L004cc5d0:
        or ebx, ebx
        jle L004cc570
    L004cc5d4:
        movzx eax, byte ptr [esi]
        inc esi
        shr eax, 1
        jae L004cc5e4
    L004cc5dc:
        add edi, eax
        sub ebx, eax
        jle L004cc570
        jmp L004cc5d4
    L004cc5e4:
        shr eax, 1
        jae L004cc618
        inc eax
    L004cc5e9:
        sub ebx, eax
        jl L004cc600
        mov ecx, eax
        mov al, byte ptr [esi]
        inc esi
        shr ecx, 1
        mov ah, al
        rep stosw
        jae L004cc5d0
        mov byte ptr [edi], al
        inc edi
        jmp L004cc5d0
    L004cc600:
        add eax, ebx
        mov ecx, eax
        mov al, byte ptr [esi]
        inc esi
        mov ah, al
        shr ecx, 1
        rep stosw
        jae L004cc5d0
        mov byte ptr [edi], al
        inc edi
        jmp L004cc5d0
        ALIGN 4
    L004cc618:
        inc eax
    L004cc619:
        sub ebx, eax
        jl L004cc630
        mov ecx, eax
        shr ecx, 1
        rep movsw
        jae L004cc5d0
        mov al, byte ptr [esi]
        inc esi
        mov byte ptr [edi], al
        inc edi
        jmp L004cc5d0
        ALIGN 4
    L004cc630:
        add eax, ebx
        mov ecx, eax
        shr ecx, 1
        rep movsw
        jae L004cc5d0
        mov al, byte ptr [esi]
        inc esi
        mov byte ptr [edi], al
        inc edi
        jmp L004cc5d0
    }
}

// FUNCTION: 0x4cc650
extern "C" __declspec(naked) int __cdecl IsLineVisible(void* surface, int x0, int y0, int x1, int y1)
{
    __asm {
    L004cc650:
        push ebp
        mov ebp, esp
        add esp, -0xc
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov esi, dword ptr [ebp + 8]
        mov edi, dword ptr [esi]
        mov esi, dword ptr [esi + 4]
        mov dword ptr [ebp - 0xc], 1
    L004cc66a:
        mov eax, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [ebp + 0x18]
        cmp eax, edi
        jge L004cc698
        jae L004cc698
        cmp ebx, edi
        jge L004cc698
        jae L004cc698
        cmp ecx, esi
        jge L004cc698
        jae L004cc698
        cmp edx, esi
        jge L004cc698
        jae L004cc698
        jmp L004cc79d
    L004cc693:
        jmp L004cc7a7
    L004cc698:
        mov dword ptr [ebp - 4], ebx
        sub dword ptr [ebp - 4], eax
        jns L004cc6ab
        cmp eax, 0
        jl L004cc693
        cmp ebx, edi
        jge L004cc693
        jmp L004cc6b4
    L004cc6ab:
        cmp eax, edi
        jge L004cc693
        cmp ebx, 0
        jl L004cc693
    L004cc6b4:
        mov dword ptr [ebp - 8], edx
        sub dword ptr [ebp - 8], ecx
        jns L004cc6c7
        cmp ecx, 0
        jl L004cc693
        cmp edx, esi
        jge L004cc693
        jmp L004cc6d0
    L004cc6c7:
        cmp ecx, esi
        jge L004cc693
        cmp edx, 0
        jl L004cc693
    L004cc6d0:
        cmp eax, 0
        jge L004cc6e9
        neg eax
        imul dword ptr [ebp - 8]
        idiv dword ptr [ebp - 4]
        add dword ptr [ebp + 0x10], eax
        mov dword ptr [ebp + 0xc], 0
        jmp L004cc6ff
    L004cc6e9:
        sub eax, edi
        jl L004cc6ff
        inc eax
        neg eax
        imul dword ptr [ebp - 8]
        idiv dword ptr [ebp - 4]
        add dword ptr [ebp + 0x10], eax
        mov dword ptr [ebp + 0xc], edi
        dec dword ptr [ebp + 0xc]
    L004cc6ff:
        mov eax, dword ptr [ebp + 0x10]
        cmp eax, 0
        jge L004cc71b
        neg eax
        imul dword ptr [ebp - 4]
        idiv dword ptr [ebp - 8]
        add dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 0x10], 0
        jmp L004cc731
    L004cc71b:
        sub eax, esi
        jl L004cc731
        inc eax
        neg eax
        imul dword ptr [ebp - 4]
        idiv dword ptr [ebp - 8]
        add dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 0x10], esi
        dec dword ptr [ebp + 0x10]
    L004cc731:
        mov eax, dword ptr [ebp + 0x14]
        cmp eax, 0
        jge L004cc74d
        neg eax
        imul dword ptr [ebp - 8]
        idiv dword ptr [ebp - 4]
        add dword ptr [ebp + 0x18], eax
        mov dword ptr [ebp + 0x14], 0
        jmp L004cc763
    L004cc74d:
        sub eax, edi
        jl L004cc763
        inc eax
        neg eax
        imul dword ptr [ebp - 8]
        idiv dword ptr [ebp - 4]
        add dword ptr [ebp + 0x18], eax
        mov dword ptr [ebp + 0x14], edi
        dec dword ptr [ebp + 0x14]
    L004cc763:
        mov eax, dword ptr [ebp + 0x18]
        cmp eax, 0
        jge L004cc77f
        neg eax
        imul dword ptr [ebp - 4]
        idiv dword ptr [ebp - 8]
        add dword ptr [ebp + 0x14], eax
        mov dword ptr [ebp + 0x18], 0
        jmp L004cc798
    L004cc77f:
        sub eax, esi
        jl L004cc798
        inc eax
        neg eax
        imul dword ptr [ebp - 4]
        idiv dword ptr [ebp - 8]
        add dword ptr [ebp + 0x14], eax
        mov dword ptr [ebp + 0x18], esi
        dec dword ptr [ebp + 0x18]
        inc dword ptr [ebp - 0xc]
    L004cc798:
        jmp L004cc66a
    L004cc79d:
        mov eax, dword ptr [ebp - 0xc]
    L004cc7a0:
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret
    L004cc7a7:
        xor eax, eax
        jmp L004cc7a0

        // ENTRY: 0x4cc7ab
    L004cc7ab:
        push ebp
        mov ebp, esp
        add esp, -0xc
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        push dword ptr [ebp + 0x18]
        push dword ptr [ebp + 0x14]
        push dword ptr [ebp + 0x10]
        push dword ptr [ebp + 0xc]
        push dword ptr [ebp + 8]
        call L004cc650
        pop dword ptr [ebp + 8]
        pop dword ptr [ebp + 0xc]
        pop dword ptr [ebp + 0x10]
        pop dword ptr [ebp + 0x14]
        pop dword ptr [ebp + 0x18]
        or eax, eax
        je L004cc8d7
        mov edi, dword ptr [ebp + 8]
        mov esi, dword ptr [edi + 8]
        mov ecx, dword ptr [ebp + 0x14]
        sub ecx, dword ptr [ebp + 0xc]
        je L004cc83b
        jns L004cc807
        neg ecx
        mov ebx, dword ptr [ebp + 0x14]
        xchg dword ptr [ebp + 0xc], ebx
        mov dword ptr [ebp + 0x14], ebx
        mov ebx, dword ptr [ebp + 0x18]
        xchg dword ptr [ebp + 0x10], ebx
        mov dword ptr [ebp + 0x18], ebx
    L004cc807:
        mov ebx, dword ptr [ebp + 0x18]
        sub ebx, dword ptr [ebp + 0x10]
        je L004cc866
        jns L004cc815
        neg ebx
        neg esi
    L004cc815:
        push esi
        mov dword ptr [ebp - 0xc], offset L004cc880
        cmp ebx, ecx
        jle L004cc82a
        mov dword ptr [ebp - 0xc], offset L004cc8ac
        xchg ebx, ecx
    L004cc82a:
        shl ebx, 1
        mov dword ptr [ebp - 4], ebx
        sub ebx, ecx
        mov esi, ebx
        sub ebx, ecx
        mov dword ptr [ebp - 8], ebx
        jmp dword ptr [ebp - 0xc]
    L004cc83b:
        mov eax, dword ptr [ebp + 0x10]
        mov ebx, dword ptr [ebp + 0x18]
        mov ecx, ebx
        sub ecx, eax
        jge L004cc84b
        neg ecx
        mov eax, ebx
    L004cc84b:
        inc ecx
        mov ebx, dword ptr [edi + 0xc]
        mov edx, dword ptr [edi + 8]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        dec esi
        mov eax, dword ptr [ebp + 0x1c]
    L004cc85f:
        stosb
        add edi, esi
        loop L004cc85f
        jmp L004cc8d7
    L004cc866:
        mov ebx, dword ptr [edi + 0xc]
        mov eax, dword ptr [ebp + 0x10]
        mov edx, dword ptr [edi + 8]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        inc ecx
        mov eax, dword ptr [ebp + 0x1c]
        rep stosb
        jmp L004cc8d7
    L004cc880:
        mov ebx, dword ptr [edi + 0xc]
        mov eax, dword ptr [ebp + 0x10]
        mov edx, dword ptr [edi + 8]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        inc ecx
        pop ebx
        mov eax, dword ptr [ebp + 0x1c]
    L004cc897:
        stosb
        or esi, esi
        jns L004cc8a3
        add esi, dword ptr [ebp - 4]
        loop L004cc897
        jmp L004cc8d7
    L004cc8a3:
        add esi, dword ptr [ebp - 8]
        add edi, ebx
        loop L004cc897
        jmp L004cc8d7
    L004cc8ac:
        mov ebx, dword ptr [edi + 0xc]
        mov eax, dword ptr [ebp + 0x10]
        mov edx, dword ptr [edi + 8]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        inc ecx
        pop ebx
        mov eax, dword ptr [ebp + 0x1c]
    L004cc8c3:
        stosb
        add edi, ebx
        or esi, esi
        jns L004cc8d2
        add esi, dword ptr [ebp - 4]
        dec edi
        loop L004cc8c3
        jmp L004cc8d7
    L004cc8d2:
        add esi, dword ptr [ebp - 8]
        loop L004cc8c3
    L004cc8d7:
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cc8df
    L004cc8df:
        push ebp
        mov ebp, esp
        add esp, -0xc
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        mov edi, dword ptr [ebp + 8]
        mov esi, dword ptr [edi + 8]
        mov ecx, dword ptr [ebp + 0x14]
        sub ecx, dword ptr [ebp + 0xc]
        je L004cc944
        jns L004cc910
        neg ecx
        mov ebx, dword ptr [ebp + 0x14]
        xchg dword ptr [ebp + 0xc], ebx
        mov dword ptr [ebp + 0x14], ebx
        mov ebx, dword ptr [ebp + 0x18]
        xchg dword ptr [ebp + 0x10], ebx
        mov dword ptr [ebp + 0x18], ebx
    L004cc910:
        mov ebx, dword ptr [ebp + 0x18]
        sub ebx, dword ptr [ebp + 0x10]
        je L004cc983
        jns L004cc91e
        neg ebx
        neg esi
    L004cc91e:
        push esi
        mov dword ptr [ebp - 0xc], offset L004cc9b0
        cmp ebx, ecx
        jle L004cc933
        mov dword ptr [ebp - 0xc], offset L004cc9ee
        xchg ebx, ecx
    L004cc933:
        shl ebx, 1
        mov dword ptr [ebp - 4], ebx
        sub ebx, ecx
        mov esi, ebx
        sub ebx, ecx
        mov dword ptr [ebp - 8], ebx
        jmp dword ptr [ebp - 0xc]
    L004cc944:
        mov eax, dword ptr [ebp + 0x10]
        mov ebx, dword ptr [ebp + 0x18]
        mov ecx, ebx
        sub ecx, eax
        jge L004cc954
        neg ecx
        mov eax, ebx
    L004cc954:
        inc ecx
        mov ebx, dword ptr [edi + 0xc]
        mov edx, dword ptr [edi]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        mov edx, esi
        mov esi, dword ptr [ebp + 0x20]
        mov eax, dword ptr [ebp + 0x1c]
        shl eax, 8
        add esi, eax
        xor eax, eax
    L004cc972:
        mov al, byte ptr [edi]
        mov al, byte ptr [eax + esi]
        mov byte ptr [edi], al
        add edi, edx
        dec ecx
        jne L004cc972
        jmp L004cca2b
    L004cc983:
        mov ebx, dword ptr [edi + 0xc]
        mov eax, dword ptr [ebp + 0x10]
        mov edx, dword ptr [edi + 8]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        inc ecx
        mov eax, dword ptr [ebp + 0x1c]
        mov esi, dword ptr [ebp + 0x20]
        shl eax, 8
        add esi, eax
        xor eax, eax
    L004cc9a3:
        mov al, byte ptr [edi]
        mov al, byte ptr [eax + esi]
        mov byte ptr [edi], al
        inc edi
        dec ecx
        jne L004cc9a3
        jmp L004cca2b
    L004cc9b0:
        mov ebx, dword ptr [edi + 0xc]
        mov eax, dword ptr [ebp + 0x10]
        mov edx, dword ptr [edi + 8]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        inc ecx
        pop ebx
        mov edx, esi
        mov esi, dword ptr [ebp + 0x20]
        mov eax, dword ptr [ebp + 0x1c]
        shl eax, 8
        add esi, eax
        xor eax, eax
    L004cc9d3:
        mov al, byte ptr [edi]
        mov al, byte ptr [eax + esi]
        mov byte ptr [edi], al
        or edx, edx
        jns L004cc9e5
        add edx, dword ptr [ebp - 4]
        loop L004cc9d3
        jmp L004cca2b
    L004cc9e5:
        add edx, dword ptr [ebp - 8]
        add edi, ebx
        loop L004cc9d3
        jmp L004cca2b
    L004cc9ee:
        mov ebx, dword ptr [edi + 0xc]
        mov eax, dword ptr [ebp + 0x10]
        mov edx, dword ptr [edi + 8]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 0xc]
        mov edi, eax
        inc ecx
        pop ebx
        mov edx, esi
        mov esi, dword ptr [ebp + 0x20]
        mov eax, dword ptr [ebp + 0x1c]
        shl eax, 8
        add esi, eax
        xor eax, eax
    L004cca11:
        mov al, byte ptr [edi]
        mov al, byte ptr [eax + esi]
        mov byte ptr [edi], al
        add edi, ebx
        or edx, edx
        jns L004cca26
        add edx, dword ptr [ebp - 4]
        dec edi
        loop L004cca11
        jmp L004cca2b
    L004cca26:
        add edx, dword ptr [ebp - 8]
        loop L004cca11
    L004cca2b:
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cca33
    L004cca33:
        push ebp
        mov ebp, esp
        add esp, -0xc
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        push dword ptr [ebp + 0x14]
        push dword ptr [ebp + 0x10]
        push dword ptr [ebp + 0xc]
        push dword ptr [ebp + 8]
        call L004cc650
        pop dword ptr [ebp + 8]
        pop dword ptr [ebp + 0xc]
        pop dword ptr [ebp + 0x10]
        pop dword ptr [ebp + 0x14]
        or eax, eax
        je L004ccb5d
        movzx esi, word ptr [edi]
        mov ecx, dword ptr [ebp + 0x10]
        sub ecx, dword ptr [ebp + 8]
        je L004ccaba
        jns L004cca86
        neg ecx
        mov ebx, dword ptr [ebp + 0x10]
        xchg dword ptr [ebp + 8], ebx
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [ebp + 0x14]
        xchg dword ptr [ebp + 0xc], ebx
        mov dword ptr [ebp + 0x14], ebx
    L004cca86:
        mov ebx, dword ptr [ebp + 0x14]
        sub ebx, dword ptr [ebp + 0xc]
        je L004ccae5
        jns L004cca94
        neg ebx
        neg esi
    L004cca94:
        push esi
        mov dword ptr [ebp - 0xc], offset L004ccb02
        cmp ebx, ecx
        jle L004ccaa9
        mov dword ptr [ebp - 0xc], offset L004ccb30
        xchg ebx, ecx
    L004ccaa9:
        shl ebx, 1
        mov dword ptr [ebp - 4], ebx
        sub ebx, ecx
        mov esi, ebx
        sub ebx, ecx
        mov dword ptr [ebp - 8], ebx
        jmp dword ptr [ebp - 0xc]
    L004ccaba:
        mov eax, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [ebp + 0x14]
        mov ecx, ebx
        sub ecx, eax
        jge L004ccaca
        neg ecx
        mov eax, ebx
    L004ccaca:
        inc ecx
        mov ebx, dword ptr [edi + 4]
        movzx edx, word ptr [edi]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 8]
        mov edi, eax
        mov eax, dword ptr [ebp + 0x18]
    L004ccadd:
        xor byte ptr [edi], al
        add edi, esi
        loop L004ccadd
        jmp L004ccb5d
    L004ccae5:
        mov ebx, dword ptr [edi + 4]
        mov eax, dword ptr [ebp + 0xc]
        movzx edx, word ptr [edi]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 8]
        mov edi, eax
        inc ecx
        mov eax, dword ptr [ebp + 0x18]
    L004ccafb:
        xor byte ptr [edi], al
        inc edi
        loop L004ccafb
        jmp L004ccb5d
    L004ccb02:
        mov ebx, dword ptr [edi + 4]
        mov eax, dword ptr [ebp + 0xc]
        movzx edx, word ptr [edi]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 8]
        mov edi, eax
        inc ecx
        pop ebx
        mov eax, dword ptr [ebp + 0x18]
    L004ccb19:
        xor byte ptr [edi], al
        inc edi
        or esi, esi
        jns L004ccb27
        add esi, dword ptr [ebp - 4]
        loop L004ccb19
        jmp L004ccb5d
    L004ccb27:
        add esi, dword ptr [ebp - 8]
        add edi, ebx
        loop L004ccb19
        jmp L004ccb5d
    L004ccb30:
        mov ebx, dword ptr [edi + 4]
        mov eax, dword ptr [ebp + 0xc]
        movzx edx, word ptr [edi]
        mul edx
        add eax, ebx
        add eax, dword ptr [ebp + 8]
        mov edi, eax
        inc ecx
        pop ebx
        inc ebx
        mov eax, dword ptr [ebp + 0x18]
    L004ccb48:
        xor byte ptr [edi], al
        add edi, ebx
        or esi, esi
        jns L004ccb58
        add esi, dword ptr [ebp - 4]
        dec edi
        loop L004ccb48
        jmp L004ccb5d
    L004ccb58:
        add esi, dword ptr [ebp - 8]
        loop L004ccb48
    L004ccb5d:
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4ccb65
    L004ccb65:
        push ebp
        mov ebp, esp
        add esp, -0xc
        push esi
        push edi
        push ebx
        push ecx
        push edx
        movzx edi, word ptr [esi]
        movzx esi, word ptr [esi + 2]
        mov dword ptr [ebp - 0xc], 1
    L004ccb7e:
        mov ebx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebx]
        cmp eax, edi
        jge L004ccbae
        jae L004ccbae
        mov eax, dword ptr [ebx + 8]
        cmp eax, edi
        jge L004ccbae
        jae L004ccbae
        mov eax, dword ptr [ebx + 4]
        cmp eax, esi
        jge L004ccbae
        jae L004ccbae
        mov eax, dword ptr [ebx + 0xc]
        cmp eax, esi
        jge L004ccbae
        jae L004ccbae
        jmp L004ccd0e
    L004ccba9:
        jmp L004ccd18
    L004ccbae:
        mov eax, dword ptr [ebx + 8]
        sub eax, dword ptr [ebx]
        mov dword ptr [ebp - 4], eax
        jns L004ccbc6
        mov eax, dword ptr [ebx]
        cmp eax, 0
        jl L004ccba9
        cmp dword ptr [ebx + 8], edi
        jge L004ccba9
        jmp L004ccbd0
    L004ccbc6:
        cmp dword ptr [ebx], edi
        jge L004ccba9
        cmp dword ptr [ebx + 8], 0
        jl L004ccba9
    L004ccbd0:
        mov eax, dword ptr [ebx + 0xc]
        sub eax, dword ptr [ebx + 4]
        mov dword ptr [ebp - 8], eax
        jns L004ccbe8
        cmp dword ptr [ebx + 4], 0
        jl L004ccba9
        cmp dword ptr [ebx + 0xc], esi
        jge L004ccba9
        jmp L004ccbf4
    L004ccbe8:
        cmp dword ptr [ebx + 4], esi
        jge L004ccba9
        cmp word ptr [ebx + 0xc], 0
        jl L004ccba9
    L004ccbf4:
        mov eax, dword ptr [ebx]
        cmp eax, 0
        jge L004ccc18
        neg eax
        imul dword ptr [ebp - 8]
        cmp dword ptr [ebp - 4], 0
        jne L004ccc0a
        xor eax, eax
        jmp L004ccc0d
    L004ccc0a:
        idiv dword ptr [ebp - 4]
    L004ccc0d:
        add dword ptr [ebx + 4], eax
        mov dword ptr [ebx], 0
        jmp L004ccc36
    L004ccc18:
        sub eax, edi
        jl L004ccc36
        inc eax
        neg eax
        imul dword ptr [ebp - 8]
        cmp dword ptr [ebp - 4], 0
        jne L004ccc2c
        xor eax, eax
        jmp L004ccc2f
    L004ccc2c:
        idiv dword ptr [ebp - 4]
    L004ccc2f:
        add dword ptr [ebx + 4], eax
        mov dword ptr [ebx], edi
        dec dword ptr [ebx]
    L004ccc36:
        mov eax, dword ptr [ebx + 4]
        cmp eax, 0
        jge L004ccc5b
        neg eax
        imul dword ptr [ebp - 4]
        cmp dword ptr [ebp - 8], 0
        jne L004ccc4d
        xor eax, eax
        jmp L004ccc50
    L004ccc4d:
        idiv dword ptr [ebp - 8]
    L004ccc50:
        add dword ptr [ebx], eax
        mov dword ptr [ebx + 4], 0
        jmp L004ccc7a
    L004ccc5b:
        sub eax, esi
        jl L004ccc7a
        inc eax
        neg eax
        imul dword ptr [ebp - 4]
        cmp dword ptr [ebp - 8], 0
        jne L004ccc6f
        xor eax, eax
        jmp L004ccc72
    L004ccc6f:
        idiv dword ptr [ebp - 8]
    L004ccc72:
        add dword ptr [ebx], eax
        mov dword ptr [ebx + 4], esi
        dec dword ptr [ebx + 4]
    L004ccc7a:
        mov eax, dword ptr [ebx + 8]
        cmp eax, 0
        jge L004ccca0
        neg eax
        imul dword ptr [ebp - 8]
        cmp dword ptr [ebp - 4], 0
        jne L004ccc91
        xor eax, eax
        jmp L004ccc94
    L004ccc91:
        idiv dword ptr [ebp - 4]
    L004ccc94:
        add dword ptr [ebx + 0xc], eax
        mov dword ptr [ebx + 8], 0
        jmp L004cccc0
    L004ccca0:
        sub eax, edi
        jl L004cccc0
        inc eax
        neg eax
        imul dword ptr [ebp - 8]
        cmp dword ptr [ebp - 4], 0
        jne L004cccb4
        xor eax, eax
        jmp L004cccb7
    L004cccb4:
        idiv dword ptr [ebp - 4]
    L004cccb7:
        add dword ptr [ebx + 0xc], eax
        mov dword ptr [ebx + 8], edi
        dec dword ptr [ebx + 8]
    L004cccc0:
        mov eax, dword ptr [ebx + 0xc]
        cmp eax, 0
        jge L004ccce6
        neg eax
        imul dword ptr [ebp - 4]
        cmp dword ptr [ebp - 8], 0
        jne L004cccd7
        xor eax, eax
        jmp L004cccda
    L004cccd7:
        idiv dword ptr [ebp - 8]
    L004cccda:
        add dword ptr [ebx + 8], eax
        mov dword ptr [ebx + 0xc], 0
        jmp L004ccd09
    L004ccce6:
        sub eax, esi
        jl L004ccd09
        inc eax
        neg eax
        imul dword ptr [ebp - 4]
        cmp dword ptr [ebp - 8], 0
        jne L004cccfa
        xor eax, eax
        jmp L004cccfd
    L004cccfa:
        idiv dword ptr [ebp - 8]
    L004cccfd:
        add dword ptr [ebx + 8], eax
        mov dword ptr [ebx + 0xc], esi
        dec dword ptr [ebx + 0xc]
        inc dword ptr [ebp - 0xc]
    L004ccd09:
        jmp L004ccb7e
    L004ccd0e:
        mov eax, dword ptr [ebp - 0xc]
    L004ccd11:
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret
    L004ccd18:
        xor eax, eax
        jmp L004ccd11

        // ENTRY: 0x4ccd1c
    L004ccd1c:
        push ebp
        mov ebp, esp
        push esi
        mov esi, dword ptr [ebp + 0xc]
        push dword ptr [ebp + 0x10]
        push dword ptr [esi + 4]
        push dword ptr [esi + 8]
        push dword ptr [esi + 4]
        push dword ptr [esi]
        push dword ptr [ebp + 8]
        call L004cc7ab
        add sp, 0x14
        push dword ptr [esi + 0xc]
        push dword ptr [esi + 8]
        push dword ptr [esi + 0xc]
        push dword ptr [esi]
        push dword ptr [ebp + 8]
        call L004cc7ab
        add sp, 0x14
        push dword ptr [esi + 0xc]
        push dword ptr [esi]
        push dword ptr [esi + 4]
        push dword ptr [esi]
        push dword ptr [ebp + 8]
        call L004cc7ab
        add sp, 0x14
        push dword ptr [esi + 0xc]
        push dword ptr [esi + 8]
        push dword ptr [esi + 4]
        push dword ptr [esi + 8]
        push dword ptr [ebp + 8]
        call L004cc7ab
        add sp, 0x18
        pop esi
        leave
        ret

        // ENTRY: 0x4ccd85
    L004ccd85:
        push ebp
        mov ebp, esp
        push esi
        mov esi, dword ptr [ebp + 8]
        push dword ptr [ebp + 0xc]
        push dword ptr [esi + 4]
        push dword ptr [esi + 8]
        push dword ptr [esi + 4]
        push dword ptr [esi]
        call L004cca33
        add sp, 0x10
        push dword ptr [esi + 0xc]
        push dword ptr [esi + 8]
        push dword ptr [esi + 0xc]
        push dword ptr [esi]
        call L004cca33
        add sp, 0x10
        mov eax, dword ptr [esi + 0xc]
        dec eax
        push eax
        push dword ptr [esi]
        mov eax, dword ptr [esi + 4]
        inc eax
        push eax
        push dword ptr [esi]
        call L004cca33
        add sp, 0x10
        mov eax, dword ptr [esi + 0xc]
        dec eax
        push eax
        push dword ptr [esi + 8]
        mov eax, dword ptr [esi + 4]
        inc eax
        push eax
        push dword ptr [esi + 8]
        call L004cca33
        add sp, 0x14
        pop esi
        leave
        ret

        // ENTRY: 0x4ccdea
    L004ccdea:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov esi, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp + 8]
        mov ebx, dword ptr [esi + 8]
        mov eax, dword ptr [esi]
        inc ebx
        and eax, 3
        and ebx, 3
        add eax, ebx
        jne L004cce4c
        mov ecx, dword ptr [edi + 8]
        mov eax, dword ptr [esi + 4]
        mov ebx, dword ptr [edi + 0xc]
        mul ecx
        add eax, ebx
        mov ebx, dword ptr [esi]
        add eax, ebx
        mov edi, eax
        mov edx, dword ptr [esi + 8]
        sub edx, ebx
        inc edx
        sub ecx, edx
        mov ebx, ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        mov esi, eax
        mov al, byte ptr [ebp + 0x10]
        mov ah, al
        bswap eax
        mov al, byte ptr [ebp + 0x10]
        mov ah, al
    L004cce39:
        mov ecx, edx
        shr ecx, 2
        rep stosd
        add edi, ebx
        dec esi
        jns L004cce39
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret
    L004cce4c:
        pushfd
        cld
        mov ecx, dword ptr [edi + 8]
        mov eax, dword ptr [esi + 4]
        mov ebx, dword ptr [edi + 0xc]
        mul ecx
        add eax, ebx
        mov ebx, dword ptr [esi]
        add eax, ebx
        mov edi, eax
        mov edx, dword ptr [esi + 8]
        sub edx, ebx
        inc edx
        sub ecx, edx
        mov ebx, ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        mov esi, eax
        mov eax, dword ptr [ebp + 0x10]
    L004cce76:
        mov ecx, edx
        rep stosb
        add edi, ebx
        dec esi
        jns L004cce76
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cce87
    L004cce87:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp + 8]
        movzx ecx, word ptr [edi]
        mov eax, dword ptr [esi + 4]
        mul cx
        mov ebx, dword ptr [edi + 4]
        add eax, ebx
        mov ebx, dword ptr [esi]
        add eax, ebx
        mov edi, eax
        mov edx, dword ptr [esi + 8]
        sub edx, ebx
        inc edx
        sub ecx, edx
        mov ebx, ecx
        mov eax, dword ptr [esi + 0xc]
        sub eax, dword ptr [esi + 4]
        mov esi, eax
        mov eax, dword ptr [ebp + 0x10]
    L004ccec0:
        mov ecx, edx
    L004ccec2:
        xor byte ptr [edi], al
        inc edi
        dec ecx
        jne L004ccec2
        add edi, ebx
        dec esi
        jns L004ccec0
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cced5
    L004cced5:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp + 0x10]
        and eax, 3
        and ebx, 3
        add eax, ebx
        jne L004ccf32
        mov eax, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [ebp + 8]
        mov esi, eax
        mov edi, ebx
        xor eax, eax
    L004ccef9:
        mov ecx, dword ptr [ebp + 0x10]
    L004ccefc:
        mov edx, dword ptr [ecx + edi - 4]
        mov al, dl
        mov bl, byte ptr [eax + esi]
        mov al, dh
        bswap edx
        mov bh, byte ptr [eax + esi]
        mov al, dl
        bswap ebx
        mov bl, byte ptr [eax + esi]
        mov al, dh
        mov bh, byte ptr [eax + esi]
        bswap ebx
        mov dword ptr [ecx + edi - 4], ebx
        sub ecx, 4
        jne L004ccefc
        add edi, dword ptr [ebp + 0xc]
        dec dword ptr [ebp + 0x14]
        jne L004ccef9
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret
    L004ccf32:
        mov eax, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [ebp + 8]
        mov esi, eax
        mov edi, ebx
        xor eax, eax
        mov edx, dword ptr [ebp + 0x14]
    L004ccf41:
        mov ecx, dword ptr [ebp + 0x10]
    L004ccf44:
        mov al, byte ptr [ecx + edi - 1]
        mov bl, byte ptr [eax + esi]
        mov byte ptr [ecx + edi - 1], bl
        dec ecx
        jne L004ccf44
        add edi, dword ptr [ebp + 0xc]
        dec edx
        jne L004ccf41
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret
    }
}

// FUNCTION: 0x4ccf60
extern "C" __declspec(naked) void __cdecl BlitText(unsigned char* pixels, int pitch, void* font, unsigned char* text,
                                             int x, int y, int c1, int c2, int c3)
{
    __asm {
        push ebp
        mov ebp, esp
        add esp, -0x10
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov edi, dword ptr [ebp + 8]
        mov esi, dword ptr [ebp + 0x10]
        movzx eax, byte ptr [esi]
        mov dword ptr [ebp - 0x10], eax
        movzx eax, byte ptr [esi + 3]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp + 0x1c]
        mov cl, byte ptr [esi + 2]
        movsx ebx, cl
        sub eax, ebx
        mul dword ptr [ebp + 0xc]
        add edi, eax
        add edi, dword ptr [ebp + 0x18]
    L004ccf91:
        mov dword ptr [ebp - 0xc], edi
        mov edi, dword ptr [ebp + 0x14]
        mov al, byte ptr [edi]
        mov edi, dword ptr [ebp - 0xc]
        or al, al
        je L004cd008
        cmp al, 0xa
        je L004cd008
        movzx ebx, al
        inc dword ptr [ebp + 0x14]
        sub ebx, dword ptr [ebp - 8]
        jb L004ccf91
        shl ebx, 1
        add ebx, 4
        add ebx, esi
        movzx ebx, word ptr [ebx]
        or ebx, ebx
        je L004ccf91
        add ebx, esi
        mov cl, byte ptr [ebx]
        inc ebx
        mov eax, dword ptr [ebp - 0x10]
        mov dword ptr [ebp - 4], eax
        mov dl, 1
    L004ccfca:
        mov ch, cl
    L004ccfcc:
        dec dl
        jne L004ccfd5
        mov dh, byte ptr [ebx]
        inc ebx
        mov dl, 8
    L004ccfd5:
        mov al, byte ptr [ebp + 0x20]
        mov ah, byte ptr [ebp + 0x28]
        shl dh, 1
        jb L004ccfe2
        mov al, byte ptr [ebp + 0x24]
    L004ccfe2:
        _emit 0x38      // cmp al, ah, as r/m8, r8 (the inline assembler only encodes r8, r/m8)
        _emit 0xe0
        je L004ccfe8
        mov byte ptr [edi], al
    L004ccfe8:
        inc edi
        dec ch
        je L004ccfef
        jmp L004ccfcc
    L004ccfef:
        add edi, dword ptr [ebp + 0xc]
        dec dword ptr [ebp - 4]
        jne L004cd001
        mov edi, dword ptr [ebp - 0xc]
        movzx eax, cl
        add edi, eax
        jmp L004ccf91
    L004cd001:
        movzx eax, cl
        sub edi, eax
        jmp L004ccfca
    L004cd008:
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret
    }
}

// FUNCTION: 0x4cd010
extern "C" __declspec(naked) void __cdecl FUN_004cd010(void* dst, void* src, int x0, int y0, int x1, int y1, int step)
{
    __asm {
        push ebp
        mov ebp, esp
        add esp, -0x18
        push esi
        push edi
        push ebx
        push ecx
        push edx
        pushfd
        cld
        mov esi, dword ptr [ebp + 0xc]
        mov eax, dword ptr [esi + 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esi + 0xc]
        mov edi, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0x20]
        mov eax, dword ptr [ebp + 0x18]
        sub eax, dword ptr [ebp + 0x10]
        je L004cd65f
        jns L004cd06e
        neg eax
        inc eax
        mov ebx, dword ptr [ebp + 0x1c]
        sub ebx, dword ptr [ebp + 0x14]
        je L004cd797
        jns L004cd05f
        neg ebx
        inc ebx
        cmp eax, ebx
        jns L004cd05a
        jmp L004cd5a5
    L004cd05a:
        jmp L004cd4eb
    L004cd05f:
        inc ebx
        cmp eax, ebx
        jns L004cd069
        jmp L004cd377
    L004cd069:
        jmp L004cd431
    L004cd06e:
        inc eax
        mov ebx, dword ptr [ebp + 0x1c]
        sub ebx, dword ptr [ebp + 0x14]
        je L004cd81c
        jns L004cd08b
        neg ebx
        inc ebx
        cmp eax, ebx
        jns L004cd086
        jmp L004cd09a
    L004cd086:
        jmp L004cd149
    L004cd08b:
        inc ebx
        cmp eax, ebx
        jns L004cd095
        jmp L004cd2bd
    L004cd095:
        jmp L004cd203
    L004cd09a:
        cmp ecx, ebx
        jle L004cd0e6
        cmp ecx, eax
        jle L004cd0e4
        mov dword ptr [ebp - 0x14], eax
        mov dword ptr [ebp - 0x18], ebx
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov ebx, ecx
        mov edx, ecx
    L004cd0b7:
        mov al, byte ptr [esi]
    L004cd0b9:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        sub ebx, dword ptr [ebp - 0x18]
        jae L004cd0d9
        sub esi, dword ptr [ebp - 4]
        add ebx, dword ptr [ebp + 0x20]
        sub edx, dword ptr [ebp - 0x14]
        jae L004cd0b7
        inc esi
        add edx, dword ptr [ebp + 0x20]
        jmp L004cd0b7
    L004cd0d9:
        sub edx, dword ptr [ebp - 0x14]
        jae L004cd0b9
        inc esi
        add edx, dword ptr [ebp + 0x20]
        jmp L004cd0b7
    L004cd0e4:
        jmp L004cd0ec
    L004cd0e6:
        cmp ecx, eax
        jle L004cd0ec
        jmp L004cd0ec
    L004cd0ec:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        sub dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd12a:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd141
        sub esi, dword ptr [ebp - 4]
    L004cd141:
        add edx, dword ptr [ebp - 8]
        adc esi, 0
        jmp L004cd12a
    L004cd149:
        cmp ecx, ebx
        jle L004cd1a0
        cmp ecx, eax
        jle L004cd19e
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 8], eax
        mov edx, ebx
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd17a:
        mov al, byte ptr [esi]
    L004cd17c:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd196
        sub esi, dword ptr [ebp - 4]
        add edx, dword ptr [ebp - 8]
        adc esi, 0
        jmp L004cd17a
    L004cd196:
        add edx, dword ptr [ebp - 8]
        jae L004cd17c
        inc esi
        jmp L004cd17a
    L004cd19e:
        jmp L004cd1a6
    L004cd1a0:
        cmp ecx, eax
        jle L004cd1a6
        jmp L004cd1a6
    L004cd1a6:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        sub dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd1e4:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd1fb
        sub esi, dword ptr [ebp - 4]
    L004cd1fb:
        add edx, dword ptr [ebp - 8]
        adc esi, 0
        jmp L004cd1e4
    L004cd203:
        cmp ecx, ebx
        jle L004cd25a
        cmp ecx, eax
        jle L004cd258
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 8], eax
        mov edx, ebx
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd234:
        mov al, byte ptr [esi]
    L004cd236:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd250
        add esi, dword ptr [ebp - 4]
        add edx, dword ptr [ebp - 8]
        adc esi, 0
        jmp L004cd234
    L004cd250:
        add edx, dword ptr [ebp - 8]
        jae L004cd236
        inc esi
        jmp L004cd234
    L004cd258:
        jmp L004cd260
    L004cd25a:
        cmp ecx, eax
        jle L004cd260
        jmp L004cd260
    L004cd260:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        add dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd29e:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd2b5
        add esi, dword ptr [ebp - 4]
    L004cd2b5:
        add edx, dword ptr [ebp - 8]
        adc esi, 0
        jmp L004cd29e
    L004cd2bd:
        cmp ecx, ebx
        jle L004cd314
        cmp ecx, eax
        jle L004cd312
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 8], eax
        mov edx, ebx
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd2ee:
        mov al, byte ptr [esi]
    L004cd2f0:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd30a
        add esi, dword ptr [ebp - 4]
        add edx, dword ptr [ebp - 8]
        adc esi, 0
        jmp L004cd2ee
    L004cd30a:
        add edx, dword ptr [ebp - 8]
        jae L004cd2f0
        inc esi
        jmp L004cd2ee
    L004cd312:
        jmp L004cd31a
    L004cd314:
        cmp ecx, eax
        jle L004cd31a
        jmp L004cd31a
    L004cd31a:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        add dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd358:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd36f
        add esi, dword ptr [ebp - 4]
    L004cd36f:
        add edx, dword ptr [ebp - 8]
        adc esi, 0
        jmp L004cd358
    L004cd377:
        cmp ecx, ebx
        jle L004cd3ce
        cmp ecx, eax
        jle L004cd3cc
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 8], eax
        mov edx, ebx
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd3a8:
        mov al, byte ptr [esi]
    L004cd3aa:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd3c4
        add esi, dword ptr [ebp - 4]
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd3a8
    L004cd3c4:
        add edx, dword ptr [ebp - 8]
        jae L004cd3aa
        dec esi
        jmp L004cd3a8
    L004cd3cc:
        jmp L004cd3d4
    L004cd3ce:
        cmp ecx, eax
        jle L004cd3d4
        jmp L004cd3d4
    L004cd3d4:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        sub dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd412:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        sub esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd429
        add esi, dword ptr [ebp - 4]
    L004cd429:
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd412
    L004cd431:
        cmp ecx, ebx
        jle L004cd488
        cmp ecx, eax
        jle L004cd486
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 8], eax
        mov edx, ebx
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd462:
        mov al, byte ptr [esi]
    L004cd464:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd47e
        add esi, dword ptr [ebp - 4]
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd462
    L004cd47e:
        add edx, dword ptr [ebp - 8]
        jae L004cd464
        dec esi
        jmp L004cd462
    L004cd486:
        jmp L004cd48e
    L004cd488:
        cmp ecx, eax
        jle L004cd48e
        jmp L004cd48e
    L004cd48e:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        sub dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd4cc:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        sub esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd4e3
        add esi, dword ptr [ebp - 4]
    L004cd4e3:
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd4cc
    L004cd4eb:
        cmp ecx, ebx
        jle L004cd542
        cmp ecx, eax
        jle L004cd540
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 8], eax
        mov edx, ebx
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd51c:
        mov al, byte ptr [esi]
    L004cd51e:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd538
        sub esi, dword ptr [ebp - 4]
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd51c
    L004cd538:
        add edx, dword ptr [ebp - 8]
        jae L004cd51e
        dec esi
        jmp L004cd51c
    L004cd540:
        jmp L004cd548
    L004cd542:
        cmp ecx, eax
        jle L004cd548
        jmp L004cd548
    L004cd548:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        add dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd586:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        sub esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd59d
        sub esi, dword ptr [ebp - 4]
    L004cd59d:
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd586
    L004cd5a5:
        cmp ecx, ebx
        jle L004cd5fc
        cmp ecx, eax
        jle L004cd5fa
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 8], eax
        mov edx, ebx
        xor eax, eax
        div cx
        shl eax, 0x10
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd5d6:
        mov al, byte ptr [esi]
    L004cd5d8:
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd5f2
        sub esi, dword ptr [ebp - 4]
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd5d6
    L004cd5f2:
        add edx, dword ptr [ebp - 8]
        jae L004cd5d8
        dec esi
        jmp L004cd5d6
    L004cd5fa:
        jmp L004cd602
    L004cd5fc:
        cmp ecx, eax
        jle L004cd602
        jmp L004cd602
    L004cd602:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 8], edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, ebx
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov dword ptr [ebp - 0xc], edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        add dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        xor ebx, ebx
        xor edx, edx
    L004cd640:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        sub esi, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0xc]
        jae L004cd657
        sub esi, dword ptr [ebp - 4]
    L004cd657:
        add edx, dword ptr [ebp - 8]
        sbb esi, 0
        jmp L004cd640
    L004cd65f:
        mov eax, dword ptr [ebp + 0x1c]
        sub eax, dword ptr [ebp + 0x14]
        jge L004cd701
        neg eax
        inc eax
        cmp ecx, eax
        je L004cd6e4
        jle L004cd6a7
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov ecx, eax
        mov edx, dword ptr [ebp - 4]
        mov eax, dword ptr [ebp + 0x14]
        mul edx
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd692:
        mov al, byte ptr [esi]
        sub esi, dword ptr [ebp - 4]
    L004cd697:
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        add ebx, ecx
        jae L004cd697
        jmp L004cd692
    L004cd6a7:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov ecx, eax
        shl ecx, 0x10
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd6cc:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        sub esi, dword ptr [ebp - 0x10]
        add ebx, ecx
        jae L004cd6cc
        sub esi, dword ptr [ebp - 4]
        jmp L004cd6cc
    L004cd6e4:
        mov eax, dword ptr [ebp + 0x14]
        mov ebx, dword ptr [ebp - 4]
        mul ebx
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
    L004cd6f1:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        sub esi, ebx
        inc edi
        jmp L004cd6f1
    L004cd701:
        inc eax
        cmp ecx, eax
        je L004cd77a
        jle L004cd73b
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov ecx, eax
        mov edx, dword ptr [ebp - 4]
        mov eax, dword ptr [ebp + 0x14]
        mul edx
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd726:
        mov al, byte ptr [esi]
        add esi, dword ptr [ebp - 4]
    L004cd72b:
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        add ebx, ecx
        jae L004cd72b
        jmp L004cd726
    L004cd73b:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov ecx, edx
        shr eax, 0x10
        mul dword ptr [ebp - 4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd762:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        add esi, dword ptr [ebp - 0x10]
        add ebx, ecx
        jae L004cd762
        add esi, dword ptr [ebp - 4]
        jmp L004cd762
    L004cd77a:
        mov eax, dword ptr [ebp + 0x14]
        mov ebx, dword ptr [ebp - 4]
        mul ebx
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
    L004cd787:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        inc edi
        add esi, ebx
        jmp L004cd787
    L004cd797:
        cmp ecx, eax
        je L004cd806
        jle L004cd7cc
        mov edx, eax
        xor eax, eax
        div cx
        shl eax, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd7b9:
        mov al, byte ptr [esi]
        dec esi
    L004cd7bc:
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        add ebx, ecx
        jae L004cd7bc
        jmp L004cd7b9
    L004cd7cc:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov edx, eax
        shl edx, 0x10
        mov ecx, edx
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd7f0:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        sub esi, dword ptr [ebp - 0x10]
        add ebx, ecx
        jae L004cd7f0
        dec esi
        jmp L004cd7f0
    L004cd806:
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
    L004cd811:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec ecx
        je L004cd88e
        dec esi
        inc edi
        jmp L004cd811
    L004cd81c:
        cmp ecx, eax
        je L004cd881
        jle L004cd84d
        xor edx, edx
        shl eax, 0x10
        div ecx
        shl eax, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd83e:
        mov al, byte ptr [esi]
        inc esi
    L004cd841:
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        add ebx, ecx
        jae L004cd841
        jmp L004cd83e
    L004cd84d:
        xor edx, edx
        shl eax, 0x10
        div ecx
        mov ecx, eax
        shl ecx, 0x10
        shr eax, 0x10
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        mov edx, dword ptr [ebp + 0x20]
        xor ebx, ebx
    L004cd86f:
        mov al, byte ptr [esi]
        mov byte ptr [edi], al
        dec edx
        je L004cd88e
        inc edi
        add esi, dword ptr [ebp - 0x10]
        add ebx, ecx
        adc esi, 0
        jmp L004cd86f
    L004cd881:
        mov eax, dword ptr [ebp + 0x14]
        mul dword ptr [ebp - 4]
        add eax, dword ptr [ebp + 0x10]
        add esi, eax
        rep movsb
    L004cd88e:
        popfd
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cd896
    L004cd896:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp + 0xc]
        mov edi, eax
        mov esi, ebx
        mov ecx, dword ptr [ebp + 0x14]
        mov edx, dword ptr [ebp + 0x18]
        mov eax, dword ptr [ebp + 0x10]
        inc eax
    L004cd8b2:
        dec eax
        je L004cd8d3
        mov ebx, edx
        shl ebx, 7
        add edx, dword ptr [ebp + 0x20]
        and ebx, 0xff800000
        add ebx, ecx
        shr ebx, 0x10
        add ecx, dword ptr [ebp + 0x1c]
        mov bl, byte ptr [ebx + esi]
        mov byte ptr [edi], bl
        inc edi
        jmp L004cd8b2
    L004cd8d3:
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cd8da
    L004cd8da:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp + 0xc]
        mov edi, eax
        mov esi, ebx
        mov ecx, dword ptr [ebp + 0x14]
        mov edx, dword ptr [ebp + 0x18]
        mov eax, dword ptr [ebp + 0x10]
        inc eax
    L004cd8f6:
        dec eax
        je L004cd917
        mov ebx, edx
        shl ebx, 6
        add edx, dword ptr [ebp + 0x20]
        and ebx, 0xffc00000
        add ebx, ecx
        shr ebx, 0x10
        add ecx, dword ptr [ebp + 0x1c]
        mov bl, byte ptr [ebx + esi]
        mov byte ptr [edi], bl
        inc edi
        jmp L004cd8f6
    L004cd917:
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cd91e
    L004cd91e:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp + 0xc]
        mov edi, eax
        mov esi, ebx
        mov ecx, dword ptr [ebp + 0x14]
        mov edx, dword ptr [ebp + 0x18]
        mov eax, dword ptr [ebp + 0x10]
        inc eax
    L004cd93a:
        dec eax
        je L004cd95b
        mov ebx, edx
        shl ebx, 5
        add edx, dword ptr [ebp + 0x20]
        and ebx, 0xffe00000
        add ebx, ecx
        shr ebx, 0x10
        add ecx, dword ptr [ebp + 0x1c]
        mov bl, byte ptr [ebx + esi]
        mov byte ptr [edi], bl
        inc edi
        jmp L004cd93a
    L004cd95b:
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret

        // ENTRY: 0x4cd962
    L004cd962:
        push ebp
        mov ebp, esp
        push esi
        push edi
        push ebx
        push ecx
        push edx
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp + 0xc]
        mov edi, eax
        mov esi, ebx
        mov ecx, dword ptr [ebp + 0x14]
        mov edx, dword ptr [ebp + 0x18]
        mov eax, dword ptr [ebp + 0x10]
        inc eax
    L004cd97e:
        dec eax
        je L004cd99f
        mov ebx, edx
        shl ebx, 4
        add edx, dword ptr [ebp + 0x20]
        and ebx, 0xfff00000
        add ebx, ecx
        shr ebx, 0x10
        add ecx, dword ptr [ebp + 0x1c]
        mov bl, byte ptr [ebx + esi]
        mov byte ptr [edi], bl
        inc edi
        jmp L004cd97e
    L004cd99f:
        pop edx
        pop ecx
        pop ebx
        pop edi
        pop esi
        leave
        ret
    }
}

