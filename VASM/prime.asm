PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003

value = $0200 ; 2 bytes
divider = $0202 ; 2 bytes
message = $0204 ; 6 bytes

E  = %10000000
RW = %01000000
RS = %00100000

    .org $8000

reset:
  ldx #$ff
  txs
  lda #%11111111 ; Set all pins on port B to output
  sta DDRB
  lda #%11100000 ; Set top 3 pins on port A to output
  sta DDRA

  lda #%00111000 ; Set 8-bit mode; 2-line display; 5x8 font
  jsr lcd_instruction
  lda #%00001100 ; Display on; cursor off; blink off
  jsr lcd_instruction
  lda #%00000110 ; Increment and shift cursor; don't shift display
  jsr lcd_instruction
  lda #%00000001 ; Clear display
  jsr lcd_instruction

  lda #2 ; Start with the first prime
  sta value
  jmp check_prime

increment_num:
  inc value
  bne no_carry
  inc value+1
no_carry:
  rts

check_prime:
  lda #2
  sta divider
  lda #0
  sta divider+1
  jmp divide

divide:
  lda value
  sec
  sbc divider
  bcc not_prime
  inc divider
  cmp value
  bne divide
  jmp display_prime

not_prime:
  jsr increment_num
  jmp check_prime

display_prime:
  lda value
  jsr push_char
  lda value+1
  jsr push_char
  rts

push_char:
  ; Convert number in A register to ASCII and display it on LCD
  and #0x0F
  adc #'0'
  jsr print_char
  rts

print_char:
  jsr lcd_wait
  sta PORTB
  lda #RS
  sta PORTA
  lda #(RS | E)
  sta PORTA
  lda #RS
  sta PORTA
  rts

lcd_instruction:
  jsr lcd_wait
  sta PORTB
  lda #0
  sta PORTA
  lda #E
  sta PORTA
  lda #0
  sta PORTA
  rts

lcd_wait:
  ; Wait until the LCD is ready to receive a command
  lda PORTA
  and #RW
  bne lcd_wait ; Loop until RW is 0 (LCD is not busy)
  rts

delay:
  ; Simple delay loop
  ldx #$FF
delay_loop:
  dex
  bne delay_loop
  rts

nmi:
  rti

irq:
  rti

    .org $fffa
    .word nmi
    .word reset
    .word irq