; vim:ft=asm:foldmethod=marker
;======================================================================
; This program implements bubble sort by nasm
; How to assemble:
;	$ nasm -f elf ${filename}.asm
;	$ gcc ${filename}.o -o ${filename}
; How to excute:
;	$ ./${filename}
; Input format:
;	No input needs.
; Output format:
;	Sorting result.
;======================================================================
global main
extern printf, scanf, puts

; {{{ section .data
section .data

format	db	"%d ", 0
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

; {{{ bubble sorting
mov ecx, 0	; for ecx from 0 to n-1
.for1:
cmp ecx, [n]
jge .endfor1
push ecx
	mov ecx, 1	; for ebx from 1 to n-1
	.for2:
	cmp ecx, [n]
	jge .endfor2
		; {{{ swap if data[ecx-1] > data[ecx]
		mov eax, [data+4*ecx-4]
		cmp eax, [data+4*ecx]
		jle .endif1
			xchg eax, [data+4*ecx]
			mov [data+4*ecx-4], eax
		.endif1:
		; }}} swap if data[ecx-1] > data[ecx]
	inc ecx
	jmp .for2
	.endfor2:
pop ecx
inc ecx
jmp .for1
.endfor1:
; }}} bubble sorting
call display

popad
ret 0
; }}} main:

; {{{ display: display data
display:
pushad

mov ecx, 0	; for ecx from 0 to n-1
.for3:
cmp ecx, [n]
jge .endfor3
	mov eax, [data+4*ecx]
	push ecx
	push eax
	push format
	call printf
	add esp, 8
	pop ecx
inc ecx
jmp .for3
.endfor3:
; {{{ output newline
push format+3
call puts
add esp, 4
; }}} output newline

popad
ret
; }}} display:
