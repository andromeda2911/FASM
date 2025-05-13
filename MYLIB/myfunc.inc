;include 'serial.inc'

; ----------------------------
; Print string
; ----------------------------
print_string:
    mov ah, 0x0E
.next_char:
    lodsb
    cmp al, 0
    je .done1
    int 0x10
    jmp .next_char
.done1:
    ret
	
;print_string:                   ; Routine: output string in SI to screen
	;push ax
	;push bx
	;push cx
	;push dx
;.repeat:
;    mov ah, 09h             ; int 10h 'print char' function
;    mov bh, 0x00
;    mov bl, 0x03
;    mov cx, 01h
        
;    cld
;    lodsb                   ; Get character from string
;    cmp al, 0
;    je .done1

    ; If char is zero, end of string
;    int 10h                 ; Otherwise, print it
;    mov bh, 00h
;    mov ah, 03h
;    int 10h
;    mov ah, 02h
;    mov bh, 00h
;    inc dl
;    int 10h
;    jmp .repeat

;.done1:
	;pop dx
	;pop cx
	;pop bx
	;pop ax
;    ret


; ----------------------------
; Clear screen
; ----------------------------        
clrscr:
   mov dh, 0
   mov dl, 0
   call set_cursor
   mov ah, 0x0a
   mov al, ' '
   mov bh, 0
   mov cx, 2000
   int 0x10
   ret

; ----------------------------
; Clear screen
; ----------------------------
cls:
  push ax
  mov ah, 0x00
  mov al, 0x03  ; text mode 80x25 16 colours
  int 0x10
  pop ax
  ret

; ----------------------------
; Set cursor
; ----------------------------
set_cursor:
    push ax
    mov ah, 0x02
    int 0x10
    pop ax
    ret

; ----------------------------
; Send crlf to serial and to screen
; ----------------------------
crlf_to_serial:
	push ax
	mov al, 13
	call printchar
	call serial_write_char
	mov al, 10
	call printchar
	call serial_write_char
	pop ax
	ret
	
	
; ----------------------
; Print character in AL
; ----------------------
printchar:
    mov ah, 0x0E
    int 0x10
    ret

; ----------------------------
; Print new line
; ----------------------------    
newline:
    push ax
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10  
    mov al, 0x0A
    int 0x10
    pop ax
    ret	