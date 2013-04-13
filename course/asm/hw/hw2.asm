; vim:ft=asm:syntax=ia64:foldmethod=marker
; {{{ include file
.486
.model flat, stdcall
option casemap :none   ; case sensitive

include \masm32\include\windows.inc
include \masm32\include\masm32.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\macros\macros.asm

includelib \masm32\lib\masm32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
; }}} magic lines

; {{{ .data
.data

debugformat	db	"sign: %d, length: %d", 10, "num: ", 0
intformat	db	"%d", 0
signformat	db	"-", 0
endl		db	10,0

; }}} .data
; {{{ .data?
.data?

num1	dd	222 dup(?)
num2	dd	222 dup(?)
num3	dd	222 dup(?)

tmp	db	222 dup(?)

; }}} .data?
.code

; {{{ main:
;  -4: num1[]   -8: len1  -12: sign1
; -16: num2[]  -20: len2  -24: sign2
; -28: num3[]  -32: len3  -36: sign3
main:
; {{{ initialize
enter 80, 0
and esp, 0fffffff0h
mov dword ptr [ebp-4], offset num1
mov dword ptr [ebp-16], offset num2
mov dword ptr [ebp-28], offset num3
; }}}
; {{{ read num1
mov esi, input("Operand 1: ")
mov eax, dword ptr [ebp-4]
mov dword ptr [esp], eax
lea eax, dword ptr [ebp-8]
mov dword ptr [esp+4], eax
lea eax, dword ptr [ebp-12]
mov dword ptr [esp+8], eax
call parseNum ; }}}
; {{{ read num2
mov esi, input("Operand 2: ")
mov eax, dword ptr [ebp-16]
mov dword ptr [esp], eax
lea eax, dword ptr [ebp-20]
mov dword ptr [esp+4], eax
lea eax, dword ptr [ebp-24]
mov dword ptr [esp+8], eax
call parseNum ; }}}
; {{{ read operator & operate
; {{{ push num2
mov eax, dword ptr [ebp-16]
mov dword ptr [esp+12], eax
mov eax, dword ptr [ebp-20]
mov dword ptr [esp+16], eax
mov eax, dword ptr [ebp-24]
mov dword ptr [esp+20], eax ; }}}
mov esi, input("Operation: ")
cmp byte ptr [esi], '-'
jne _elsif_ma_1_1
	; {{{ neg sign2
	neg dword ptr [ebp-24]
	mov eax, dword ptr [ebp-24]
	mov dword ptr [esp+20], eax ; }}}
	jmp _elsif_ma_1_1_1
_elsif_ma_1_1:
cmp byte ptr [esi], '+'
jne _elsif_ma_1_2
_elsif_ma_1_1_1:
	; {{{ push *num1
	mov eax, dword ptr [ebp-4]
	mov dword ptr [esp], eax
	lea eax, dword ptr [ebp-8]
	mov dword ptr [esp+4], eax
	lea eax, dword ptr [ebp-12]
	mov dword ptr [esp+8], eax ; }}}
	call addNum
	; {{{ replace num3 with num1
	mov eax, dword ptr [ebp-4]
	mov dword ptr [ebp-28], eax
	mov eax, dword ptr [ebp-8]
	mov dword ptr [ebp-32], eax
	mov eax, dword ptr [ebp-12]
	mov dword ptr [ebp-36], eax ; }}}
	jmp _endif_ma_1
_elsif_ma_1_2:
; {{{ push num1, *num3
mov eax, dword ptr [ebp-4]
mov dword ptr [esp], eax
mov eax, dword ptr [ebp-8]
mov dword ptr [esp+4], eax
mov eax, dword ptr [ebp-12]
mov dword ptr [esp+8], eax
mov eax, dword ptr [ebp-28]
mov dword ptr [esp+24], eax
lea eax, dword ptr [ebp-32]
mov dword ptr [esp+28], eax
lea eax, dword ptr [ebp-36]
mov dword ptr [esp+32], eax ; }}}
cmp byte ptr [esi], '*'
jne _elsif_ma_1_3
	call mulNum
	jmp _endif_ma_1
_elsif_ma_1_3:
cmp byte ptr [esi], '/'
jne _endif_ma_1
	call divNum
_endif_ma_1: ; }}}
; {{{ print num3
mov eax, dword ptr [ebp-36]
cmp dword ptr [ebp-36], 1
je _endif_ma_4
	print offset signformat
_endif_ma_4:
mov ecx, dword ptr [ebp-32]
mov esi, dword ptr [ebp-28]
lea edi, [esi+ecx*4-4]
_while_ma_3:
cmp edi, esi
jl _endwhile_ma_3
	pushad
	invoke wsprintf, offset tmp, offset intformat, dword ptr [edi]
	print offset tmp
	popad
	sub edi, 4
jmp _while_ma_3
_endwhile_ma_3:
print offset endl ; }}}
mov eax, 0
leave
ret 0
; }}} main:

; {{{ parseNum(int num[], int *len, int *sign): parse number string from esi	(change all register)
; +8: num[]  +12: *len  +16: *sign
parseNum:
; {{{ initialize
push ebp
mov ebp, esp
sub esp, 80
and esp, 0fffffff0h ; }}}
; {{{ judge sign
mov ebx, dword ptr [ebp+16]
cmp byte ptr [esi], '-'
jne _elsif_pn_1
	mov dword ptr [ebx], -1
	inc esi
	jmp _endif_pn_1
_elsif_pn_1:
	mov dword ptr [ebx], 1
_endif_pn_1: ; }}}
; {{{ judge length
mov edi, esi
_while_pn_1:
cmp byte ptr [edi], 0
je _endwhile_pn_1
	inc edi
jmp _while_pn_1
_endwhile_pn_1:
mov eax, edi
sub eax, esi
mov ebx, dword ptr [ebp+12]
mov dword ptr [ebx], eax ; }}}
; {{{ store number into num
mov esi, dword ptr [ebp+8]
dec edi
mov ebx, dword ptr [ebp+12]
mov ecx, dword ptr [ebx]
_while_pn_2:
cmp ecx, 0
je _endwhile_pn_2
	movzx eax, byte ptr [edi]
	sub eax, '0'
	mov dword ptr [esi], eax
	add esi, 4
	dec edi
dec ecx
jmp _while_pn_2
_endwhile_pn_2: ; }}}
leave
ret ; }}}

; {{{ addNum(int num1[], int *len1, int *sign1, int num2[], int len2, int sign2)
; add num1 and num2 into num1
;  +8: num1[]  +12: *len1  +16: *sign1
; +20: num2[]  +24: len2  +28: sign2
addNum:
push ebp
mov ebp, esp

; {{{ neg every digits of num1 if sign1 is equal to -1
mov eax, dword ptr [ebp+16]
cmp dword ptr [eax], -1
jne _if_an_1
	mov eax, dword ptr [ebp+12]
	mov ecx, dword ptr [eax]
	mov esi, dword ptr [ebp+8]
	lea edi, [esi+ecx*4]
	_while_an_1:
	cmp esi, edi
	je _endwhile_an_1
		neg dword ptr [esi]
		add esi, 4
	jmp _while_an_1
	_endwhile_an_1:
_if_an_1: ; }}}
; {{{ neg every digits of num2 if sign2 is equal to -1
mov eax, dword ptr [ebp+28]
cmp dword ptr [ebp+28], -1
jne _endif_an_2
	mov ecx, dword ptr [ebp+24]
	mov esi, dword ptr [ebp+20]
	lea edi, [esi+ecx*4]
	_while_an_2:
	cmp esi, edi
	je _endwhile_an_2
		neg dword ptr [esi]
		mov eax, dword ptr [esi]
		add esi, 4
	jmp _while_an_2
	_endwhile_an_2:
_endif_an_2: ; }}}
; {{{ ebx=max(len1,len2), and add num1 and num2 into num1
mov eax, dword ptr [ebp+12]
mov ebx, dword ptr [eax]
cmp ebx, dword ptr [ebp+24]
jge _endif_ad_1
	mov ebx, dword ptr [ebp+24]
_endif_ad_1:
mov esi, dword ptr [ebp+8]
mov edx, dword ptr [ebp+20]
lea edi, [esi+ebx*4]
_while_ad_3:
cmp esi, edi
je _endwhile_ad_3
	mov eax, dword ptr [edx]
	add dword ptr [esi], eax
	add edx, 4
	add esi, 4
jmp _while_ad_3
_endwhile_ad_3: ; }}}
; {{{ find the highest bits unequal to 0
sub esi, 4
_while_ad_4:
cmp ebx, 1
je _endwhile_ad_4
	cmp dword ptr [esi], 0
	jne _endwhile_ad_4
	sub esi, 4
	dec ebx
jmp _while_ad_4
_endwhile_ad_4: ; }}}
; {{{ judge *sign1 and neg *num1 if *sign1 == -1
lea edi, [esi+4]
cmp dword ptr [esi], 0
jl _else_ad_2
	mov eax, dword ptr [ebp+16]
	mov dword ptr [eax], 1
	mov esi, dword ptr [ebp+8]
	jmp _endif_ad_2
_else_ad_2:
	mov eax, dword ptr [ebp+16]
	mov dword ptr [eax], -1
	mov eax, dword ptr [ebp+8]
	_while_ad_5:
	cmp esi, eax
	jl _endwhile_ad_5
		neg dword ptr [esi]
		sub esi, 4
	jmp _while_ad_5
	_endwhile_ad_5:
	add esi, 4
_endif_ad_2: ; }}}
; {{{ arrange num1
mov ecx, 10
_while_ad_6:
cmp esi, edi
je _endwhile_ad_6
	mov eax, dword ptr [esi]
	cdq
	idiv ecx
	mov dword ptr [esi], edx
	add dword ptr [esi+4], eax
	cmp dword ptr [esi], 0
	jge _endif_ad_3
		add dword ptr [esi], 10
		dec dword ptr [esi+4]
	_endif_ad_3:
	mov eax, dword ptr [esi]
	add esi, 4
jmp _while_ad_6
_endwhile_ad_6:
inc ebx ; }}}
; {{{ find the highest bits unequal to 0, and store into num3
_while_ad_7:
cmp ebx, 1
je _endwhile_ad_7
	cmp dword ptr [esi], 0
	jne _endwhile_ad_7
	sub esi, 4
	dec ebx
jmp _while_ad_7
_endwhile_ad_7:
mov eax, dword ptr [ebp+12]
mov dword ptr [eax], ebx ; }}}
; {{{ neg every digits of num2 if sign2 is equal to -1
mov eax, dword ptr [ebp+28]
cmp dword ptr [ebp+28], -1
jne _endif_an_22
	mov ecx, dword ptr [ebp+24]
	mov esi, dword ptr [ebp+20]
	lea edi, [esi+ecx*4]
	_while_an_22:
	cmp esi, edi
	je _endwhile_an_22
		neg dword ptr [esi]
		mov eax, dword ptr [esi]
		add esi, 4
	jmp _while_an_22
	_endwhile_an_22:
_endif_an_22: ; }}}
leave
ret ; }}}

; {{{ mulNum(int num1[], int len1, int sign1, int num2[], int len2, int sign2, int num3[], int *len3, int *sign3)
; multiply num1 and num2 into num3
;  +8: num1[]  +12: len1  +16: sign1
; +20: num2[]  +24: len2  +28: sign2
; +32: num3[]  +36: *len3  +40: *sign3
mulNum:
push ebp
mov ebp, esp
; {{{ judge sign3
mov esi, dword ptr [ebp+40]
mov eax, dword ptr [ebp+28]
cmp dword ptr [ebp+16], eax
je _elsif_mn_1
	mov dword ptr [esi], -1
jmp _endif_mn_1
_elsif_mn_1:
	mov dword ptr [esi], 1
_endif_mn_1: ; }}}
; {{{ multiply num1 & num2 into num3
mov ecx, 0
mov ebx, 0		; maintain i+j of num3[i+j] = num1[i]*num2[j]
_while_mn_1:
cmp ecx, dword ptr [ebp+12]		; ecx < len1
je _endwhile_mn_1
	mov eax, dword ptr [ebp+8]
	mov esi, dword ptr [eax]	; esi = num1[ecx]
	push ecx
	mov edi, dword ptr [ebp+20]	; edi = the head of num2
	mov eax, dword ptr [ebp+24]
	lea ecx, dword ptr [edi+eax*4]	; ecx = the tail of num1
	_while_mn_2:
	cmp edi, ecx			; num2.head < num2.tail
	je _endwhile_mn_2
		mov eax, esi
		cdq
		imul dword ptr [edi]
		mov edx, dword ptr [ebp+32]
		add dword ptr [edx+ebx*4], eax
		mov eax, dword ptr [edx+ebx*4]
		add edi, 4
		inc ebx
	jmp _while_mn_2
	_endwhile_mn_2:
	pop ecx
	sub ebx, dword ptr [ebp+24]
	inc ebx
	add dword ptr [ebp+8], 4
	inc ecx
jmp _while_mn_1
_endwhile_mn_1: ; }}}
; {{{ arrange num3
mov esi, dword ptr [ebp+32]
mov ebx, dword ptr [ebp+12]
add ebx, dword ptr [ebp+24]
lea edi, dword ptr [esi+ebx*4]
mov ecx, 10
_while_mn_3:
cmp esi, edi
je _endwhile_mn_3
	mov eax, dword ptr [esi]
	cdq
	idiv ecx
	add dword ptr [esi+4], eax
	mov dword ptr [esi], edx
	add esi, 4
jmp _while_mn_3
_endwhile_mn_3: ; }}}
; {{{ find the highest bits unequal to 0, and set len3
mov esi, dword ptr [ebp+32]
sub edi, 4
_while_mn_4:
cmp edi, esi
je _endwhile_mn_4
	mov eax, dword ptr [edi]
	cmp dword ptr [edi], 0
	jne _endwhile_mn_4
		sub edi, 4
		dec ebx
jmp _while_mn_4
_endwhile_mn_4:
mov eax, dword ptr [ebp+36]
mov dword ptr [eax], ebx ; }}}
leave
ret ; }}}

; {{{ int compare(int num1[], int len1, int sign1, int num2[], int len2, int sign2)
; compare num1 to num2, assume the two number have the same sign
;  +8: num1[]  +12: len1  +16: sign1
; +20: num2[]  +24: len2  +28: sign2
compare:
push ebp
mov ebp, esp

mov ebx, dword ptr [ebp+12]
cmp ebx, dword ptr [ebp+24]
jge _elsif_cp_2_1
	mov eax, -1
	jmp _end_func_cp
_elsif_cp_2_1:
je _endif_cp_2
	mov eax, 1
	jmp _end_func_cp
_endif_cp_2:
mov eax, dword ptr [ebp+8]
lea esi, dword ptr [eax+ebx*4-4]
mov eax, dword ptr [ebp+20]
lea edi, dword ptr [eax+ebx*4-4]
mov eax, dword ptr [esi]
_while_cp_1:
cmp ebx, 0
je _endwhile_cp_1
	mov eax, dword ptr [esi]
	cmp eax, dword ptr [edi]
	jge _elsif_cp_1_1
		mov eax, -1
		jmp _end_func_cp
	_elsif_cp_1_1:
	je _endif_cp_1
		mov eax, 1
		jmp _end_func_cp
	_endif_cp_1:
	sub esi, 4
	sub edi, 4
	dec ebx
jmp _while_cp_1
_endwhile_cp_1:

_end_func_cp:
leave
ret ; }}}

; {{{ divNum(int num1[], int len1, int sign1, int num2[], int len2, int sign2, int num3[], int *len3, int *sign3)
; divide num1 and num2 into num3
;  +8: num1[]  +12: len1  +16: sign1
; +20: num2[]  +24: len2  +28: sign2
; +32: num3[]  +36: *len3  +40: *sign3
; -4: i  -8: n -12: l3
divNum:
; {{{ initialize
push ebp
mov ebp, esp
sub esp, 80
and esp, 0fffffff0h
; }}}
; {{{ judge sign3
mov esi, dword ptr [ebp+40]
mov eax, dword ptr [ebp+28]
cmp dword ptr [ebp+16], eax
je _elsif_dn_1
	mov dword ptr [esi], -1
jmp _endif_dn_1
_elsif_dn_1:
	mov dword ptr [esi], 1
_endif_dn_1: ; }}}
; {{{ divide by subtract num1 by num2
; {{{ set something
mov dword ptr [ebp+16], 1	; sign1 = 1
mov dword ptr [ebp+28], -1	; sign2 = -1	for call subNum
mov ebx, dword ptr [ebp+12]
sub ebx, dword ptr [ebp+24]	; ebx = len2-len1
mov dword ptr [ebp-12], ebx	; l3 = ebx
lea eax, dword ptr [ebx*4]
add dword ptr [ebp+8], eax	; num1 = &num1[ebx]
sub dword ptr [ebp+12], ebx	; len1 -= ebx
add dword ptr [ebp+32], eax	; num3 = &num3[ebx]
mov dword ptr [ebp-4], ebx	; i = l3	; }}}
; {{{ push num2
mov eax, dword ptr [ebp+20]
mov dword ptr [esp+12], eax
mov eax, dword ptr [ebp+24]
mov dword ptr [esp+16], eax
mov eax, dword ptr [ebp+28]
mov dword ptr [esp+20], eax ; }}}
_while_dn_1:
cmp dword ptr [ebp-4], 0
jl _endwhile_dn_1
	mov dword ptr [ebp-8], 0	; n = 0
	; {{{ call compare
	mov eax, dword ptr [ebp+12]
	mov dword ptr [esp+4], eax
	mov eax, dword ptr [ebp+16]
	mov dword ptr [esp+8], eax
	mov eax, dword ptr [ebp+8]
	mov dword ptr [esp], eax
	call compare ; }}}
	_while_dn_2:
	cmp eax, 0
	jle _endwhile_dn_2
		; {{{ call addNum (subtract num1 by num2)
		lea eax, dword ptr [ebp+12]
		mov dword ptr [esp+4], eax
		lea eax, dword ptr [ebp+16]
		mov dword ptr [esp+8], eax
		call addNum ; }}}
		inc dword ptr [ebp-8]		; n++
		; {{{ call compare
		mov eax, dword ptr [ebp+12]
		mov dword ptr [esp+4], eax
		mov eax, dword ptr [ebp+16]
		mov dword ptr [esp+8], eax
		call compare ; }}}
	jmp _while_dn_2
	_endwhile_dn_2:
	; {{{ *num3 = n
	mov eax, dword ptr [ebp-8]
	mov ebx, dword ptr [ebp+32]
	mov dword ptr [ebx], eax ; }}}
	inc dword ptr [ebp+12]	; len1++
	sub dword ptr [ebp+32], 4	; num3--
	sub dword ptr [ebp+8], 4	; num1--
	dec dword ptr [ebp-4]	; i--
jmp _while_dn_1
_endwhile_dn_1:
add dword ptr [ebp+32], 4
mov ebx, dword ptr [ebp-12]
inc ebx
cmp ebx, 1
jge _endif_dn_2
	mov dword ptr ebx, 1
_endif_dn_2: ; }}}
; {{{ find the highest bits unequal to 0, and set len3
mov esi, dword ptr [ebp+32]
lea edi, dword ptr [esi+ebx*4-4]
_while_dn_4:
cmp edi, esi
je _endwhile_dn_4
	cmp dword ptr [edi], 0
	jne _endwhile_dn_4
		sub edi, 4
		dec ebx
jmp _while_dn_4
_endwhile_dn_4:
mov eax, dword ptr [ebp+36]
mov dword ptr [eax], ebx ; }}}
leave
ret ; }}}

end main
