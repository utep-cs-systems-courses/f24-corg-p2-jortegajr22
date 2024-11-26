	.arch msp430g2553
	.p2align 1,0
	.text

	.global red_on
	.global green_on
	.global led_changed
	.global redVal
	.global greenVal

	.global led_init
	.global led_update
	.extern PIDIR
	.extern PIOUT

	;; defind globals

red_on:		.byte 0
green_on:	.byte 0
led_changed:	.byte 0

redVal:
	.byte 0			; red OFF
	.byte 0x01 		; red ON (BIT0)

greenVal:
	.byte 0			;green OFF
	.byte 0x40		;green ON (BIT6)

	;; led_init
led_init:
	bis #0x41, &P1DIR	;set Led bits as output
	mov #1, &led_changed
	call #led_update
	ret

	;; led_update
led_update:
	cmp #0, &led_changed 	; Check if 1
	jz update_done		;if not, skip update leds

	mov &red_on, r12
	mov.b redVal(r12), r13 	;get red led value from array

	mov &green_on, r14
	mov.b greenVal(14), r15	;get green led val from array

	bis r13, r15 		;combine red and green vals in 15

	mov #0xFF, r12 		; load to clear bits
	bic #0x41, r12		;get mask for LEDS
	and r12, &P1OUT		; clear led bits
	mov #0x20, r13
	bis r15, &P1OUT		;set new bits

	mov #0, &led_changed	;reset to 0

update_done:
	pop r0
	ret
