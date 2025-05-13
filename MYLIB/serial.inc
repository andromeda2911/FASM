CR       EQU      13
LF       EQU      10

; ----------------------------
; Initialize COM1 (0x3F8)
; ----------------------------
init_serial:
    mov dx, 0x3FB        ; Line Control Register
    mov al, 0x80         ; Enable DLAB
    out dx, al

    mov dx, 0x3F8        ; Set baud rate divisor low byte (115200)
    mov al, 0x0C		 ; 9600 bauds
    out dx, al

    mov dx, 0x3F9        ; Set baud rate divisor high byte
    mov al, 0x00
    out dx, al

    mov dx, 0x3FB        ; Disable DLAB, 8N1 config
    mov al, 0x03
    out dx, al

; === Step 4: Set MCR for no flow control ===
; OUT2=1, RTS=1, DTR=1 (standard modem lines enabled, no handshaking)
;    mov dx, 0x3FC       ; MCR (Modem Control Register)
;   mov al, 0x0B        ; 0000 1011
;    out dx, al
	
    mov dx, 0x3F9        ; Disable interrupts
    mov al, 0x00
    out dx, al
    ret

; ----------------------------
; Send character in AL to serial
; ----------------------------
serial_write_char:
	push cx
	mov cl, al
.wait:	
    mov dx, 0x3FD        ; Line Status Register
    in al, dx
    test al, 0x20        ; Bit 5 = Transmitter ready
    jz .wait
	
    mov dx, 0x3F8	
	mov al, cl
    out dx, al
	pop cx
    ret

; ----------------------------
; Read character into AL from serial
; ----------------------------
serial_read_char:
.wait:
    mov dx, 0x3FD
    in al, dx
    test al, 0x01        ; Bit 0 = Data ready
    jz .wait

    mov dx, 0x3F8
    in al, dx
    ret

; ----------------------------
; Flush serial inpuit
; ----------------------------
serial_flush:
    push ax
    push dx

.flush_loop:
    mov dx, 0x3FD        ; Line Status Register (COM1)
    in al, dx
    test al, 1           ; Bit 0 = Data Ready?
    jz .done             ; If not, buffer is empty

    mov dx, 0x3F8        ; Read and discard the byte
    in al, dx
    jmp .flush_loop

.done:
    pop dx
    pop ax
    ret
	
; ----------------------------
; Print string to screen and serial
; ----------------------------
print_msg_serial:
    ;mov si, msg
.next:
	cld
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    push ax
    call serial_write_char
    pop ax
    jmp .next
.done:
    ret

; ----------------------------
; Serial byte available
; ----------------------------
serial_byte_available:
    mov dx, 0x3FD        ; COM1 Line Status Register
    in al, dx
    test al, 0x01        ; Bit 0 = Data Ready
    jz .no_data
    mov al, 1            ; Return 1 = data available
    ret

.no_data:
    xor al, al           ; Return 0 = no data
    ret
	
msg db CR, LF, "Terminal ready. Type a key: ", CR, LF, 0
	
;call init_serial

;mov al, 'H'
;call serial_write_char

;call serial_read_char    ; Waits until a character is received
;call serial_write_char   ; Echoes it back

	