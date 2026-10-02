	TITLE	src/unsorted/0x483fa0.cpp
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??8@YAHABU_GUID@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FUN_00483fa0@@YGXPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?FUN_00483fa0@@YGXPAX@Z				; FUN_00483fa0
EXTRN	?g_game@@3PAUGame_00483fa0@@A:DWORD		; g_game
EXTRN	?FUN_004b8150@@YGXPAXPAUBitmap_00483fa0@@HH@Z:NEAR ; FUN_004b8150
EXTRN	?FUN_004c6e70@@YGXPAXHHPAE@Z:NEAR		; FUN_004c6e70
;	COMDAT ?FUN_00483fa0@@YGXPAX@Z
_TEXT	SEGMENT
_surface$ = 8
_bmp$ = -24
_ax$ = -56
_ay$ = -64
_px$ = -60
_py$ = -52
_w1$ = -72
_w2$ = -68
_rem1$ = -40
_rem2$ = -36
_stride$ = -48
_p1$29017 = 8
_p2$29018 = -44
_n$29020 = -32
_p1$29029 = 8
_p2$29030 = -44
_n$29031 = -32
_n$29044 = -36
_y$29045 = 8
_stride2$29047 = -28
_offset$29048 = -32
?FUN_00483fa0@@YGXPAX@Z PROC NEAR			; FUN_00483fa0, COMDAT
; File src/unsorted/0x483fa0.cpp
; Line 107
	sub	esp, 72					; 00000048H
; Line 109
	mov	ecx, DWORD PTR ?g_game@@3PAUGame_00483fa0@@A ; g_game
	push	ebx
	push	ebp
	push	esi
	mov	eax, DWORD PTR [ecx+228903]
; Line 110
	mov	esi, DWORD PTR [ecx+82719]
; Line 111
	mov	edx, DWORD PTR [ecx+228907]
	mov	DWORD PTR _ax$[esp+84], eax
; Line 113
	mov	eax, esi
	mov	DWORD PTR _ay$[esp+84], edx
	cdq
	and	edx, 31					; 0000001fH
	push	edi
	mov	edi, DWORD PTR [ecx+82723]
	add	eax, edx
	sar	eax, 5
	mov	DWORD PTR _px$[esp+88], eax
; Line 117
	mov	ebx, DWORD PTR [ecx+228919]
	shl	eax, 5
	sub	esi, eax
	mov	eax, edi
	cdq
	and	edx, 31					; 0000001fH
; Line 119
	mov	ebp, DWORD PTR [ecx+228923]
	add	eax, edx
	sar	eax, 5
	mov	DWORD PTR _py$[esp+88], eax
	shl	eax, 5
	sub	edi, eax
	lea	eax, DWORD PTR [ebx+esi]
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	mov	DWORD PTR _w1$[esp+88], eax
; Line 120
	lea	eax, DWORD PTR [ebp+edi]
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
; Line 121
	mov	edx, DWORD PTR _w1$[esp+88]
	shl	edx, 5
	sar	eax, 5
	sub	ebx, edx
; Line 122
	mov	edx, eax
	shl	edx, 5
	sub	ebp, edx
	add	ebx, esi
; Line 123
	xor	edx, edx
	add	ebp, edi
; Line 125
	cmp	ebx, edx
	mov	DWORD PTR _w2$[esp+88], eax
	mov	DWORD PTR _rem1$[esp+88], ebx
	mov	DWORD PTR _rem2$[esp+88], ebp
	mov	WORD PTR _bmp$[esp+92], dx
	mov	WORD PTR _bmp$[esp+94], dx
	je	SHORT $L29011
; Line 126
	inc	DWORD PTR _w1$[esp+88]
$L29011:
; Line 127
	cmp	ebp, edx
	je	SHORT $L29012
; Line 128
	inc	eax
	mov	DWORD PTR _w2$[esp+88], eax
$L29012:
; Line 129
	mov	eax, DWORD PTR [ecx+82483]
; Line 136
	mov	ebx, DWORD PTR _surface$[esp+84]
	cdq
	sub	eax, edx
	mov	edx, 32					; 00000020H
	sar	eax, 1
	test	esi, esi
	mov	DWORD PTR _stride$[esp+88], eax
	mov	WORD PTR _bmp$[esp+88], dx
	mov	WORD PTR _bmp$[esp+90], dx
	mov	BYTE PTR _bmp$[esp+97], 0
	mov	BYTE PTR _bmp$[esp+98], 0
	jne	SHORT $L29015
	mov	edx, DWORD PTR _rem1$[esp+88]
	test	edx, edx
	je	$L29024
$L29015:
; Line 137
	imul	eax, DWORD PTR _py$[esp+88]
	mov	ebp, DWORD PTR _px$[esp+88]
; Line 138
	mov	edx, DWORD PTR [ecx+82571]
	add	eax, ebp
	lea	ebp, DWORD PTR [edx+eax*2]
	mov	DWORD PTR _p1$29017[esp+84], ebp
; Line 139
	mov	ebp, DWORD PTR _w1$[esp+88]
	add	eax, ebp
; Line 140
	mov	ebp, DWORD PTR _ay$[esp+88]
	lea	eax, DWORD PTR [edx+eax*2-2]
	mov	DWORD PTR _p2$29018[esp+88], eax
; Line 141
	mov	eax, DWORD PTR _w2$[esp+88]
; Line 142
	test	eax, eax
	mov	DWORD PTR _n$29020[esp+88], eax
	jle	$L29024
; Line 152
	mov	edx, DWORD PTR _stride$[esp+88]
	lea	eax, DWORD PTR [edx+edx]
	mov	DWORD PTR -28+[esp+88], eax
$L29022:
; Line 143
	test	esi, esi
	je	SHORT $L29025
; Line 144
	mov	eax, DWORD PTR _p1$29017[esp+84]
	mov	ecx, DWORD PTR [ecx+82563]
	xor	edx, edx
	mov	dx, WORD PTR [eax]
	mov	eax, DWORD PTR [ecx+4]
	shl	edx, 10					; 0000000aH
	add	edx, eax
; Line 145
	mov	eax, DWORD PTR _ax$[esp+88]
	mov	DWORD PTR _bmp$[esp+104], edx
	mov	edx, ebp
	sub	edx, edi
	sub	eax, esi
	push	edx
	lea	ecx, DWORD PTR _bmp$[esp+92]
	push	eax
	push	ecx
	push	ebx
	call	?FUN_004b8150@@YGXPAXPAUBitmap_00483fa0@@HH@Z ; FUN_004b8150
	mov	ecx, DWORD PTR ?g_game@@3PAUGame_00483fa0@@A ; g_game
$L29025:
; Line 147
	mov	eax, DWORD PTR _rem1$[esp+88]
	test	eax, eax
	je	SHORT $L29026
; Line 148
	mov	eax, DWORD PTR _p2$29018[esp+88]
	mov	ecx, DWORD PTR [ecx+82563]
	xor	edx, edx
	mov	dx, WORD PTR [eax]
	mov	eax, DWORD PTR [ecx+4]
	shl	edx, 10					; 0000000aH
	add	edx, eax
; Line 149
	mov	eax, DWORD PTR _w1$[esp+88]
	mov	DWORD PTR _bmp$[esp+104], edx
	mov	edx, ebp
	sub	edx, edi
	lea	ecx, DWORD PTR _bmp$[esp+88]
	push	edx
	mov	edx, DWORD PTR _ax$[esp+92]
	shl	eax, 5
	add	eax, edx
	sub	eax, esi
	sub	eax, 32					; 00000020H
	push	eax
	push	ecx
	push	ebx
	call	?FUN_004b8150@@YGXPAXPAUBitmap_00483fa0@@HH@Z ; FUN_004b8150
	mov	ecx, DWORD PTR ?g_game@@3PAUGame_00483fa0@@A ; g_game
$L29026:
; Line 151
	mov	eax, DWORD PTR -28+[esp+88]
	mov	edx, DWORD PTR _p1$29017[esp+84]
	add	edx, eax
; Line 153
	add	ebp, 32					; 00000020H
	mov	DWORD PTR _p1$29017[esp+84], edx
	mov	edx, DWORD PTR _p2$29018[esp+88]
	add	edx, eax
; Line 154
	mov	eax, DWORD PTR _n$29020[esp+88]
	dec	eax
	mov	DWORD PTR _p2$29018[esp+88], edx
	mov	DWORD PTR _n$29020[esp+88], eax
	jne	$L29022
$L29024:
; Line 157
	test	edi, edi
	jne	SHORT $L29028
	mov	eax, DWORD PTR _rem2$[esp+88]
	test	eax, eax
	je	$L29036
$L29028:
; Line 158
	mov	edx, DWORD PTR _stride$[esp+88]
	mov	ebp, DWORD PTR _px$[esp+88]
	imul	edx, DWORD PTR _py$[esp+88]
	mov	eax, DWORD PTR [ecx+82571]
	add	edx, ebp
; Line 159
	mov	ebp, DWORD PTR _py$[esp+88]
	lea	edx, DWORD PTR [eax+edx*2]
	mov	DWORD PTR _p1$29029[esp+84], edx
	mov	edx, DWORD PTR _w2$[esp+88]
	lea	edx, DWORD PTR [edx+ebp-1]
	mov	ebp, DWORD PTR _px$[esp+88]
	imul	edx, DWORD PTR _stride$[esp+88]
	add	edx, ebp
; Line 161
	mov	ebp, DWORD PTR _ax$[esp+88]
	lea	eax, DWORD PTR [eax+edx*2]
	mov	DWORD PTR _p2$29030[esp+88], eax
	mov	eax, DWORD PTR _w1$[esp+88]
; Line 162
	test	eax, eax
	mov	DWORD PTR _n$29031[esp+88], eax
	jle	$L29036
$L29034:
; Line 163
	test	edi, edi
	je	SHORT $L29037
; Line 164
	mov	eax, DWORD PTR _p1$29029[esp+84]
	mov	ecx, DWORD PTR [ecx+82563]
	xor	edx, edx
	mov	dx, WORD PTR [eax]
	mov	eax, DWORD PTR [ecx+4]
	shl	edx, 10					; 0000000aH
	add	edx, eax
; Line 165
	mov	eax, ebp
	mov	DWORD PTR _bmp$[esp+104], edx
	mov	edx, DWORD PTR _ay$[esp+88]
	sub	edx, edi
	sub	eax, esi
	push	edx
	lea	ecx, DWORD PTR _bmp$[esp+92]
	push	eax
	push	ecx
	push	ebx
	call	?FUN_004b8150@@YGXPAXPAUBitmap_00483fa0@@HH@Z ; FUN_004b8150
	mov	ecx, DWORD PTR ?g_game@@3PAUGame_00483fa0@@A ; g_game
$L29037:
; Line 167
	mov	eax, DWORD PTR _rem2$[esp+88]
	test	eax, eax
	je	SHORT $L29038
; Line 168
	mov	eax, DWORD PTR _p2$29030[esp+88]
	mov	ecx, DWORD PTR [ecx+82563]
	xor	edx, edx
	mov	dx, WORD PTR [eax]
	mov	eax, DWORD PTR [ecx+4]
; Line 169
	mov	ecx, DWORD PTR _ay$[esp+88]
	shl	edx, 10					; 0000000aH
	add	edx, eax
	mov	eax, ebp
	mov	DWORD PTR _bmp$[esp+104], edx
	mov	edx, DWORD PTR _w2$[esp+88]
	shl	edx, 5
	add	edx, ecx
	sub	eax, esi
	sub	edx, edi
	lea	ecx, DWORD PTR _bmp$[esp+88]
	sub	edx, 32					; 00000020H
	push	edx
	push	eax
	push	ecx
	push	ebx
	call	?FUN_004b8150@@YGXPAXPAUBitmap_00483fa0@@HH@Z ; FUN_004b8150
	mov	ecx, DWORD PTR ?g_game@@3PAUGame_00483fa0@@A ; g_game
$L29038:
; Line 171
	mov	edx, DWORD PTR _p1$29029[esp+84]
	mov	eax, 2
	add	edx, eax
; Line 173
	add	ebp, 32					; 00000020H
	mov	DWORD PTR _p1$29029[esp+84], edx
	mov	edx, DWORD PTR _p2$29030[esp+88]
	add	edx, eax
; Line 174
	mov	eax, DWORD PTR _n$29031[esp+88]
	dec	eax
	mov	DWORD PTR _p2$29030[esp+88], edx
	mov	DWORD PTR _n$29031[esp+88], eax
	jne	$L29034
$L29036:
; Line 178
	mov	ebp, DWORD PTR _w1$[esp+88]
	test	esi, esi
	je	SHORT $L29079
; Line 180
	mov	eax, DWORD PTR _px$[esp+88]
	mov	edx, 32					; 00000020H
	sub	edx, esi
	mov	esi, DWORD PTR _ax$[esp+88]
	dec	ebp
	add	esi, edx
	inc	eax
	mov	DWORD PTR _w1$[esp+88], ebp
	mov	DWORD PTR _ax$[esp+88], esi
	mov	DWORD PTR _px$[esp+88], eax
$L29079:
; Line 183
	mov	eax, DWORD PTR _w2$[esp+88]
	test	edi, edi
	je	SHORT $L29080
; Line 184
	mov	edx, 32					; 00000020H
	dec	eax
	sub	edx, edi
	mov	edi, DWORD PTR _ay$[esp+88]
	add	edi, edx
; Line 185
	mov	edx, DWORD PTR _py$[esp+88]
	mov	DWORD PTR _ay$[esp+88], edi
	inc	edx
	jmp	SHORT $L29040
$L29080:
	mov	edx, DWORD PTR _py$[esp+88]
$L29040:
; Line 187
	mov	esi, DWORD PTR _rem1$[esp+88]
	test	esi, esi
	je	SHORT $L29041
; Line 188
	dec	ebp
	mov	DWORD PTR _w1$[esp+88], ebp
$L29041:
; Line 189
	mov	esi, DWORD PTR _rem2$[esp+88]
	test	esi, esi
	je	SHORT $L29042
; Line 190
	dec	eax
$L29042:
; Line 192
	test	eax, eax
	jle	$L29051
; Line 193
	mov	DWORD PTR _n$29044[esp+88], eax
; Line 194
	mov	eax, DWORD PTR _ay$[esp+88]
	mov	DWORD PTR _y$29045[esp+84], eax
; Line 196
	mov	eax, DWORD PTR _stride$[esp+88]
	and	eax, 65535				; 0000ffffH
; Line 197
	imul	edx, eax
	lea	esi, DWORD PTR [eax+eax]
	mov	DWORD PTR _stride2$29047[esp+88], esi
	mov	esi, DWORD PTR _px$[esp+88]
	add	edx, esi
	shl	edx, 1
	mov	eax, edx
	mov	DWORD PTR _offset$29048[esp+88], eax
	jmp	SHORT $L29049
$L29081:
	mov	ebp, DWORD PTR _w1$[esp+88]
$L29049:
; Line 199
	mov	esi, DWORD PTR [ecx+82571]
	add	esi, eax
; Line 201
	test	ebp, ebp
	jle	SHORT $L29060
; Line 202
	mov	edi, DWORD PTR _ax$[esp+88]
$L29069:
; Line 204
	mov	eax, DWORD PTR [ecx+82563]
	xor	edx, edx
	mov	dx, WORD PTR [esi]
	mov	ecx, DWORD PTR [eax+4]
	shl	edx, 10					; 0000000aH
	add	edx, ecx
	mov	ecx, DWORD PTR _y$29045[esp+84]
	push	edx
	push	ecx
	push	edi
	push	ebx
	call	?FUN_004c6e70@@YGXPAXHHPAE@Z		; FUN_004c6e70
; Line 207
	mov	ecx, DWORD PTR ?g_game@@3PAUGame_00483fa0@@A ; g_game
	add	edi, 32					; 00000020H
	add	esi, 2
	dec	ebp
	jne	SHORT $L29069
$L29060:
; Line 209
	mov	eax, DWORD PTR _offset$29048[esp+88]
	mov	edi, DWORD PTR _stride2$29047[esp+88]
; Line 210
	mov	esi, DWORD PTR _y$29045[esp+84]
; Line 211
	mov	edx, DWORD PTR _n$29044[esp+88]
	add	eax, edi
	add	esi, 32					; 00000020H
	dec	edx
	mov	DWORD PTR _offset$29048[esp+88], eax
	mov	DWORD PTR _y$29045[esp+84], esi
	mov	DWORD PTR _n$29044[esp+88], edx
	jne	SHORT $L29081
$L29051:
; Line 213
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx
	add	esp, 72					; 00000048H
	ret	4
?FUN_00483fa0@@YGXPAX@Z ENDP				; FUN_00483fa0
_TEXT	ENDS
END
