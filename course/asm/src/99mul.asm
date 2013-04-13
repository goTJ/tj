global main
extern printf, scanf, puts

section .data

format	db	"%2d ", 0
n	dd	10

section .bss

section .text

main:

mov ecx, 9
.for1:
push ecx
mov ecx, 9
	.for2:
	mov eax, 10
	sub eax, [esp]
	mov ebx, 10
	sub ebx, ecx
	mul ebx
	push ecx
	push eax
	push format
	call printf
	add esp, 8
	pop ecx
	loop .for2
;output newline
pop ecx
push format+4
call puts
add esp, 4
loop .for1

ret 0
