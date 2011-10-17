; vim:ft=asm:syntax=ia64:foldmethod=marker
include Irvine16.inc
; {{{ .data
.data

prompt1		db "Bitmap image1: ",0
prompt2		db 10,"Bitmap image2: ",0
prompt3		db 10,"Music format: ",0
;bmpfile1	db "a.bmp",0
;bmpfile2	db "b.bmp",0
bmpfile1	db 20 dup(?)
bmpfile2	db 20 dup(?)
bmpfileptr	dw ?
bmpHD		dw ?
buf		db 2000 dup(?)
;musicbuf	db 48h, 45h, 45h, 40h, 46h, 43h, 43h, 40h, 41h, 43h, 45h, 46h, 48h, 48h, 48h, 48h, 0C0h, 0
musicbuf	db 1000 dup(?)
musicptr	dw ?
musicstatus	db 1
_noteVal	dw 0, 9121, 8609, 8126, 7670, 7239, 6833, 6449, 6087, 5746, 5423, 5119, 4831
;                        C,   C#,    D,   D#,    E,    F,   F#,    G,   G#,    A,   A#,    B
;                        1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12
		dw 4560, 4304, 4063, 3835, 3619, 3416, 3224, 3043, 2873, 2711, 2559, 2415
;                     C,   C#,    D,   D#,    E,    F,   F#,    G,   G#,    A,   A#,    B
;                    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24

saveMode db	?

; }}}

.code

; {{{ main PROC
main PROC
mov ax, @data
mov ds,ax

; {{{ input filename and store music file into musicbuf
mov dx, offset prompt1
call WriteString
mov dx, offset bmpfile1
mov cx, (sizeof bmpfile1)-1
call ReadString
mov dx, offset prompt2
call WriteString
mov dx, offset bmpfile2
mov cx, (sizeof bmpfile2)-1
call ReadString
mov dx, offset prompt3
call WriteString
mov dx, offset buf
mov cx, (sizeof buf)-1
call ReadString
; {{{ open music file
mov ax,716Ch   	; extended create or open
mov bx,0      	; mode = read-only
mov cx,0	; normal attribute
mov dx,1	; action: open
mov si, offset buf
int 21h       	; call MS-DOS
jc  quit	; quit if error
mov bmpHD,ax ; }}}
; {{{ read music file
mov ah,3Fh	; read file or device
mov bx,bmpHD	; file handle
mov cx,1000	; max bytes to read, 1078=54+1024
mov dx,offset musicbuf	; buffer pointer
int 21h
jc  quit	; quit if error ; }}}
; {{{ close music file
mov  ah,3Eh    	; function: close file
mov  bx,bmpHD	; input file handle
int  21h       	; call MS-DOS
jc  quit	; quit if error
; }}}
; }}}
; {{{ set video mode
mov  ah,0Fh	; get current video mode
int  10h
mov  saveMode,al	; save it

mov ah,0	; set new video mode
mov al,13h	; to mode 13h
int 10h

push 0A000h	; video segment address
pop es              	; ES = A000h (video segment).  ; }}}
; {{{ control bmp display and music play
mov si, offset bmpfile1
mov bmpfileptr, si
mov ax, offset musicbuf
mov musicptr, ax
mov ax, 0
_while4:
cmp ax, 11Bh	; 11Bh is <esc>
je _endwhile4
	call showBMP

	test musicstatus, 1
	jz readKey

	; {{{ wait key with music playing
	mov si, musicptr
	noKey:
		; {{{ play note
		mov al, byte ptr [si]
		call playNote
		mov al, byte ptr [si]; }}}
		; {{{ change note
		inc si
		cmp byte ptr [si], 0
		jnz _endif7
			mov si, offset musicbuf
		_endif7:
		mov musicptr, si ; }}}
		; {{{ check key
		mov ah,11h
		int 16h ; }}}
	jz noKey
	mov musicptr, si
	jmp readKey
	; }}}

	; {{{ read key and judge
	readKey:
	mov ah,10h	; wait for keystroke
	int 16h
	cmp ax, 3920h	; 3920h is <space>
	jne _elsif5
		mov si, bmpfileptr
		cmp si, offset bmpfile1
		je _else6
			mov si, offset bmpfile1
			jmp _endif6
		_else6:
			mov si, offset bmpfile2
		_endif6:	
		mov bmpfileptr, si
		jmp _endif5
	_elsif5:
	cmp ax, 1C0Dh	; 1C0Dh is <enter>
	jne _endif5
		xor musicstatus, 1
		test musicstatus, 1
		jnz _endif5
		jmp readKey
	_endif5:
	; }}}
jmp _while4
_endwhile4: ; }}}
; {{{ restore video mode
mov ah,0   	; reset video mode
mov al,saveMode   	; to saved mode
int 10h ; }}}

quit:
mov ax, 0
exit
main ENDP
; }}}

; {{{ showBMP PROC -- si: bmpfile1name
showBMP PROC

; {{{ open bmp file
mov ax,716Ch   	; extended create or open
mov bx,0      	; mode = read-only
mov cx,0	; normal attribute
mov dx,1	; action: open
int 21h       	; call MS-DOS
jc  quit	; quit if error
mov bmpHD,ax ; }}}
; {{{ read bmp file
mov ah,3Fh	; read file or device
mov bx,bmpHD	; file handle
mov cx,1078	; max bytes to read, 1078=54+1024
mov dx,offset buf	; buffer pointer
int 21h
jc  quit	; quit if error ; }}}
; {{{ set palette color
mov si, offset buf
add si, 54
mov cx, 0
_while1:
cmp cx, 256
je _endwhile1
	mov dx,3c8h	; video palette port (3C8h)
	mov ax,cx	; set palette index
	out dx,al

	mov dx,3c9h	; colors go to port 3C9h
	mov al,byte ptr [si+2]	; red
	shr al, 2
	out dx,al
	mov al,byte ptr [si+1]	; green
	shr al, 2
	out dx,al
	mov al,byte ptr [si]	; blue
	shr al, 2
	out dx,al
	add si, 4
	inc ecx
jmp _while1
_endwhile1: ; }}}
; {{{ set bitmap
mov cx, 199
mov di, 0
_while2:
cmp cx, 0
jl _endwhile2
	push cx
	; {{{ read bmp file
	mov ah,3Fh	; read file or device
	mov bx,bmpHD	; file handle
	mov cx,320	; max bytes to read
	mov dx,offset buf	; buffer pointer
	int 21h
	jc  quit	; quit if error ; }}}
	; {{{ set color
	mov si, offset buf
	mov ax, word ptr [esp]
	mov bx, 320
	mul bx
	mov di, ax
	mov cx, 0
	_while3:
	cmp cx, 320
	je _endwhile3
		mov al, byte ptr [si]
		mov byte ptr es:[di], al
		inc di
		inc si
		inc cx
	jmp _while3
	_endwhile3: ; }}}
	pop cx
	dec cx
jmp _while2
_endwhile2:
; }}}
; {{{ close bmp file
mov  ah,3Eh    	; function: close file
mov  bx,bmpHD	; input file handle
int  21h       	; call MS-DOS
jc  quit	; quit if error
; }}}
quit:
ret
showBMP ENDP
; }}}

; {{{ playNote PROC -- al: note
playNote PROC

; {{{ play tone
movzx cx, al
test al, 00111111b
jz mute
	; {{{ turn speaker on
	mov al,10110110b    ;33 準備把 BX 寫入埠 42H 當作計數暫存器
	out 43h,al
	in al,61h		; get speaker status
	or al,00000011b   	; set lowest 2 bits
	out 61h,al     		; turn speaker on ; }}}
	mov ax, cx
	and al, 00111111b
	shl al, 1
	movzx bx, al
	mov ax, _noteVal[bx]
	out 42h, al
	mov al, ah
	out 42h, al
	jmp end_play
mute:
	in  ax,61h            	; get original status
	and  al,11111100b    	; clear lowest 2 bits
	out  61h,al		; turn speaker off
end_play:
; }}}
; {{{ delay
shr cx, 6
mov bx, 1
cmp cx, 1
jne _elsif_1_8
	shl bx, 1
	jmp _endif8
_elsif_1_8:
cmp cx, 2
jne _elsif_2_8
	shl bx, 2
	jmp _endif8
_elsif_2_8:
cmp cx, 3
jne _endif8
	shl bx, 3
	jmp _endif8
_endif8:
mov ax, 1000
mul bx
call Delay ; }}}
; {{{ mute for a short time
in  ax,61h            	; get original status
and  al,11111100b    	; clear lowest 2 bits
out  61h,al		; turn speaker off
mov ax, 200
call Delay ; }}}

ret
playNote ENDP ; }}}

end main
