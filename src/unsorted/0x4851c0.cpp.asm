	TITLE	src/unsorted/0x4851c0.cpp
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
;	COMDAT ?FUN_004851c0@@YGFHHHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?FUN_004851c0@@YGFHHHHHH@Z			; FUN_004851c0
EXTRN	?g_game@@3PAUGame_004851c0@@A:DWORD		; g_game
;	COMDAT ?FUN_004851c0@@YGFHHHHHH@Z
_TEXT	SEGMENT
_x1$ = 8
_y1$ = 16
_x2$ = 20
_y2$ = 28
_dx$ = 8
_dy$ = 28
?FUN_004851c0@@YGFHHHHHH@Z PROC NEAR			; FUN_004851c0, COMDAT
; File src/unsorted/0x4851c0.cpp
; Line 47
	mov	eax, DWORD PTR _y1$[esp-4]
	push	ebx
	push	ebp
	mov	ebp, DWORD PTR _x1$[esp+4]
	push	esi
	mov	esi, DWORD PTR _x2$[esp+8]
	push	edi
	mov	edi, DWORD PTR _y2$[esp+12]
	sub	esi, ebp
	sub	edi, eax
; Line 48
	mov	eax, esi
	cdq
	mov	ecx, eax
	mov	eax, edi
	xor	ecx, edx
	sub	ecx, edx
	cdq
	xor	eax, edx
	sub	eax, edx
	cmp	ecx, eax
	jl	SHORT $L542
	mov	eax, ecx
$L542:
	cdq
	and	edx, 1048575				; 000fffffH
; Line 51
	xor	ebx, ebx
	add	eax, edx
	mov	ecx, eax
	mov	eax, esi
	sar	ecx, 20					; 00000014H
	cdq
	inc	ecx
	idiv	ecx
	mov	DWORD PTR _dx$[esp+12], eax
	mov	eax, edi
	cdq
	idiv	ecx
; Line 52
	test	ecx, ecx
	mov	DWORD PTR _dy$[esp+12], eax
	jl	$L527
	mov	esi, DWORD PTR ?g_game@@3PAUGame_004851c0@@A ; g_game
	inc	ecx
	mov	DWORD PTR 20+[esp+12], ecx
$L525:
; Line 53
	mov	eax, ebp
	cdq
	and	edx, 1048575				; 000fffffH
	add	eax, edx
	mov	ecx, eax
; Line 54
	mov	eax, DWORD PTR _y1$[esp+12]
	cdq
	and	edx, 1048575				; 000fffffH
	add	eax, edx
; Line 55
	xor	edx, edx
	sar	ecx, 20					; 00000014H
	sar	eax, 20					; 00000014H
; Line 56
	test	ecx, ecx
	jl	SHORT $L531
	mov	edi, DWORD PTR [esi+82483]
	cmp	ecx, edi
	jge	SHORT $L531
	test	eax, eax
	jl	SHORT $L531
	cmp	eax, DWORD PTR [esi+82487]
	jge	SHORT $L531
; Line 57
	imul	eax, edi
	add	eax, ecx
	lea	ecx, DWORD PTR [eax+eax*2]
	lea	edx, DWORD PTR [eax+ecx*4]
	mov	eax, DWORD PTR [esi+82567]
	add	edx, eax
$L531:
; Line 58
	test	edx, edx
	je	SHORT $L537
; Line 59
	mov	ecx, DWORD PTR [esi+82543]
	xor	eax, eax
	mov	ax, WORD PTR [edx+8]
	shl	eax, 8
	movzx	ax, BYTE PTR [ecx+eax+250]
	movzx	cx, BYTE PTR [edx+4]
	add	eax, ecx
; Line 60
	cmp	bx, ax
	jge	SHORT $L534
	mov	ebx, eax
$L534:
; Line 61
	mov	dx, WORD PTR [edx]
	test	dx, dx
	je	SHORT $L537
; Line 62
	and	edx, 65535				; 0000ffffH
	mov	eax, edx
	shl	eax, 3
	sub	eax, edx
	lea	edx, DWORD PTR [eax+eax*4]
	mov	eax, DWORD PTR [esi+82775]
	mov	ecx, DWORD PTR [eax+edx*8+146]
	lea	eax, DWORD PTR [eax+edx*8]
	mov	eax, DWORD PTR [eax+110]
	mov	edi, DWORD PTR [ecx+366]
	add	eax, edi
	sar	eax, 16					; 00000010H
; Line 63
	cmp	bx, ax
	jge	SHORT $L537
	mov	ebx, eax
$L537:
; Line 66
	mov	edx, DWORD PTR _dx$[esp+12]
; Line 67
	mov	ecx, DWORD PTR _y1$[esp+12]
	mov	eax, DWORD PTR 20+[esp+12]
	add	ebp, edx
	mov	edx, DWORD PTR _dy$[esp+12]
	add	ecx, edx
	dec	eax
	mov	DWORD PTR _y1$[esp+12], ecx
	mov	DWORD PTR 20+[esp+12], eax
	jne	$L525
$L527:
; Line 71
	pop	edi
	pop	esi
	mov	ax, bx
	pop	ebp
	pop	ebx
	ret	24					; 00000018H
?FUN_004851c0@@YGFHHHHHH@Z ENDP				; FUN_004851c0
_TEXT	ENDS
END
