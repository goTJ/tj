; vim:ft=asm:foldmethod=marker
;======================================================================
; This program implements quick sort by nasm
; How to assemble:
;       $ nasm -f elf ${filename}.asm
;       $ gcc ${filename}.o -o ${filename}
; How to excute:
;       $ ./${filenam}
; Input format:
;	No input needs.
; Output format:
;	sorting result
;======================================================================
global main
extern printf, scanf, puts

; {{{ section .data
section .data

outstr	db	"%d ", 0
data	dd	5, 4, 3, 2, 1, 00h
n	dd	($-data)/4-1

; }}} section .data
; {{{ section .bss
section .bss


; }}} section .bss

section .text

; {{{ main:
main:
pushad

; {{{ qsort
; {{{ push into stack
push data
mov edi, data
mov eax, 4
mul dword [n]
sub eax, 4
add edi, eax
push edi
; }}} push into stack
; {{{ simulate recursion by stack
mov ecx, 1
.dowhile1:
	dec ecx
	; {{{ initialize
	mov edi, [esp]		; edi = head;
	mov esi, [esp+4]	; esi = tail;
	mov ebx, [edi]		; ebx = data[tail];	// radix
	sub edi, 4		; edi = tail-1;
	; }}} initialize
	; {{{ partition
	.while1:
	cmp esi, edi
	jge .endwhile1
		cmp [esi], ebx
		jle .else1
			mov eax, [esi]
			xchg eax, [edi]
			mov [esi], eax
			sub edi, 4
			jmp .endif1
		.else1:
			add esi, 4
		.endif1:
	jmp .while1
	.endwhile1:
	; }}} partition
	; {{{ swap(data[tail], data[esi])
	pop ebx			; ebx = tail;
	mov eax, [ebx]		
	xchg eax, [esi]
	mov [ebx], eax
	; }}} swap(data[tail], data[esi])
	; {{{ try if need to push data [head~esi) into stack
	sub esi, 4
	pop eax			; ebx = head;
	cmp eax, esi
	jge .endif2
		inc ecx
		push eax
		push esi
	.endif2:
	; }}} try if need to push data [head~esi) into stack
	; {{{ try if need to push data (esi~tail) into stack
	add edi, 4
	cmp edi, ebx
	jge .endif3
		inc ecx
		push edi
		push ebx
	.endif3:
	; }}} try if need to push data (esi~tail) into stack
cmp ecx, 0
jg .dowhile1
; }}} recursion
; }}} qsort
call display

popad
ret 0
; }}} main:

; {{{ display: display data
display:
pushad

mov esi, data
mov edi, data
mov eax, 4
mul dword [n]
add edi, eax

.while2:
cmp esi, edi
jge .endwhile2
	push dword [esi]
	push outstr
	call printf
	add esp, 8
	add esi, 4
	jmp .while2
.endwhile2:
push outstr+3
call puts
add esp, 4

popad
ret
; }}} display:
