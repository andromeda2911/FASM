use16

org 0x0000


ROM_SIZE_IN_BLOCK = 16; 1 means ROM size is 1 block (512 bytes)
ROM_SIZE_IN_BYTE = ROM_SIZE_IN_BLOCK * 512

    DB      55h, 0AAh          ; Boot signature    
    DB      16            ; Block size in sectors (200h each)     
    jmp rom_entry


rom_entry:    
    pusha  
        
    cli                 
    mov   ax,cs       
    mov   ds,ax
    mov   es,ax
    sti
 
    call resetkb1       
    call  cls   
	
	mov si, text_string     ; Put string position into SI
    call print_string       ; Call our string-printing routine 
;	call newline
		
	call readkey		; wait key to continue
	cmp ah, 0x01
	je .exit_term
		
    call init_serial
	mov si, msg
    call print_msg_serial
	
	call kbd_flush			; flush keyboard
	call serial_flush		; flush serial
	
.read_term:	
	call kbdhit
	cmp al, 1
	je .send_to_serial
	
    ;jmp .read_term
	
.read_from_serial:
	call serial_byte_available
	cmp al, 0
	je .read_term
	call serial_read_char	
	cmp al, 13
	jne .cont_read
	
	call crlf_to_serial		; print crlf
	call serial_flush		; flush serial
	jmp .read_from_serial
	
.cont_read:	       
	call printchar	
    call serial_write_char
	jmp .read_from_serial

.send_to_serial:
	call readkey			; read key from keyboard
	call printchar			; print char to screen
	cmp al, 13
	jne .cont
	
	call crlf_to_serial		; print crlf
	jmp .read_term

.cont:	
	call serial_write_char	; send char to serial
	jmp .read_term
		
.exit_term:	
	call cls
	popa
    retf

   ;.bounce:
    ;jmp .bounce                   ; Jump here - infinite loop!

include '..\mylib\serial.inc'
include '..\mylib\kbd.inc'
include '..\mylib\myfunc.inc'

resetkb1:
    push ax     
    in al, 0x61
    and al, 0x7F
    out 0x61, al       
    pop ax
    ret
    
resetkb2:
    push ax     
    in al, 0x61
    or al, 0x80
    out 0x61, al       
    pop ax
    ret
      

;waitkey:
;    push ax
    
;.wait_key:
;    in al, 0x64         ; Read status port
;    test al, 1          ; Bit 0 set if output buffer full
;    jz .wait_key        ; Wait until a key is available
        
;.key_loop:   
;    xor ax, ax
;    in  al, 0x60
    
;    cmp  al, 0x01
;    je .exit
              ; Affiche AL
;    jmp .wait_key
    
;.exit:  
      
;    pop ax          
;    ret

       
text_string db "ESC for exit or another key for continue", 0
msg_serial	db "CRLF", 13, 10, 0
;msg db "Terminal ready. Type a key: ", 13, 10, 0

 
times ROM_SIZE_IN_BYTE-($-$$) db 0x00 
;times (ROM_SIZE_IN_BYTE - ($ - rom_start)) db 0

 ;patch_byte is calculated and automagically inserted below
PREV_CHKSUM = 0
repeat $
   load CHKSUM byte from %-1
   CHKSUM = (PREV_CHKSUM + CHKSUM) mod 0x100
   PREV_CHKSUM = CHKSUM
end repeat
store byte (0x100 - CHKSUM) at ($-1)  ; store the patch_byte         


