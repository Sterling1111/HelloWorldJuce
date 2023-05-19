PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003

E  = %10000000
RW = %01000000
RS = %00100000

  .org $8000

reset:
   ldx #$ff       ; initialise stack pointer
   txs

   lda #%11111111 ; Set all pins on VIA port B to output
   sta DDRB
   lda #%11100000 ; Set top 3 pins on VIA port A to output
   sta DDRA

   lda #%00111000 ; Set 8-bit mode; 2-line display; 5x8 font
   jsr lcd_instruction
   lda #%00001110 ; Display on; cursor on; blink off
   jsr lcd_instruction
   lda #%00000111 ; Increment and shift cursor; shift display
   jsr lcd_instruction
   lda #%00000001 ; Clear display
   jsr lcd_instruction
   lda #%00000010 ; Home
   jsr lcd_instruction

   lda #%01001000 ; Set Cursor at CGRAM 0x8
   jsr lcd_instruction

   ldx #0
   ldy #16
cgram:
   lda heart,x
   jsr print_char
   inx
   dey
   bne cgram

   lda #%10010000 ; Set Cursor at position 16
   jsr lcd_instruction

   ldx #0
print1:
   lda line1,x
   beq done1
   jsr print_char
   inx
   jmp print1

done1:
   lda #%00000110 ; Increment and shift cursor; no shift display
   jsr lcd_instruction

   lda #%11010000 ; Set Cursor at position 0x50
   jsr lcd_instruction

   ldx #0
print2:
   lda line2,x
   beq done2
   jsr print_char
   inx
   jmp print2
done2:

   ldy #60
shift40:
   lda #%00011000 ; Shift display left
   jsr lcd_instruction
   dey
   bne shift40

loop:
   jmp loop

;                 0123456789012345678901234567890123456789
line1: .asciiz   "cut off my legs not im an amputee"
line2: .byte   2," im not well! ",1,0

heart: .byte %00000,%01010,%11111,%11111,%01110,%00100,%00000,%00000
rr: .byte %11011,%10101,%11111,%11111,%01110,%01010,%11011,%00000

lcd_wait:
   pha
   lda #%00000000 ; Set all pins on VIA port B to input
   sta DDRB
lcdbusy:
   lda #RW
   sta PORTA
   lda #(RW | E)
   sta PORTA
   lda PORTB
   and #%10000000 ; filter out the address bits, leaving the busy flag
   bne lcdbusy    ; branch if not zero
   lda #RW
   sta PORTA
   lda #%11111111 ; Set all pins on VIA port B to output
   sta DDRB
   pla
   rts

lcd_instruction:
   jsr lcd_wait
   sta PORTB
   lda #0         ; Clear RS/RW/E bits - RS=0=instruction register
   sta PORTA
   lda #E         ; Set E bit to send instruction
   sta PORTA
   lda #0         ; Clear RS/RW/E bits
   sta PORTA
   rts

print_char:
   jsr lcd_wait
   sta PORTB
   lda #RS        ; Set RS bit; Clear RW/E bits - RS=1=data register
   sta PORTA
   lda #(RS | E)  ; Set RS + E bit to send instruction
   sta PORTA
   lda #RS        ; Clear E bit
   sta PORTA
   rts

  .org $fffc
  .word reset
  .word $0000