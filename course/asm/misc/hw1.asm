global main
extern printf, scanf

section .data

format1        db "%d %d", 0           ; input format
format2        db "%u", 10, 0          ; output format
table          times 100000 dd 1       ; table[j] stores C(i+j,j) for some i

section .bss

n              resd 1
m              resd 1

section .text

main:
pushad

; call scanf to get n & m
push dword m
push dword n
push dword format1
call scanf
add esp, 12

; count C(n,m)
mov ecx, [m]
jecxz .case1                           ; if m = 0 then jmp to case1
mov ecx, [n]
sub ecx, [m]
jecxz .case1                           ; if n = m then jmp to case1
jl .case2                              ; if n < m then jmp to case2

.for1:                                  ; for i := 1 to n-m
push ecx
mov esi, table
mov ecx, [m]
.for1.1:                            ; for j := 1 to m
mov eax, [esi]
add esi, 4                     ; C(i+j,j)  = C(i+j-1,j-1) + C(i+j-1,j)
add [esi], eax                 ; table[j] := table[j-1]   + table[j]
loop .for1.1
pop ecx
loop .for1

; call printf to print C(n,m)
.case1:                                 ; case1: 0 <= m <= n
mov esi, [m]
push dword [table + esi * 4]           ;        C(n,m) = table[m]
jmp .output
.case2:                                 ; case2: n < m
push dword 0                           ;        C(n, m) should be 0
.output:
push dword format2
call printf
add esp, 8

popad
ret
