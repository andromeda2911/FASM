; ----------------------------
; Keyboard hit
; ----------------------------
kbdhit:
    mov ah, 01h        ; BIOS: check for keystroke
    int 16h
    jz .no_key         ; Zero flag set? No key
    mov al, 1
    ret

.no_key:
    xor al, al         ; Return 0 = no key
    ret
	
; ----------------------------
; Flush buffer keyboard
; ----------------------------	
kbd_flush:
    push ax
    push bx

.flush_loop:
    mov ah, 01h        ; Check for key
    int 16h
    jz .done2           ; If no key, we're done

    ; Discard the key
    mov ah, 00h
    int 16h
    jmp .flush_loop

.done2:
    pop bx
    pop ax
    ret

; ----------------------------
; Wait for read key
; ----------------------------
readkey:           
    xor ax, ax
    mov ah, 0x00
    int 16h          
    ret
	
; ----------------------------
; Wait for key press
; ----------------------------
waitkey_press:         
    xor ax, ax
    mov ah, 0x00
    int 16h     
    ret
	
checkkey:
    push ax
   
.loop2:
    ; Check if a key is available
    mov ah, 0x01           ; INT 16h, function 01h = check key
    int 0x16
    jz .loop2               ; No key? Keep checking

    ; Read the key
    mov ah, 0x00          ; INT 16h, function 00h = read key
    int 0x16               ; AL = ASCII, AH = scan code

    ; Check if it's ESC (ASCII 27)
    cmp ah, 0x01    
    je .done3
    call printchar
    jmp .loop2

.done3: 
    call cls
    pop ax
    ret
