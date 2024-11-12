	.arch msp430g2553
	.p2align 1,0
	.text


	.global blinkUpdate
	.extern greenControl
	.data
blinkCount:
	.word 0			;initialize blinkCount to 0

	
	.text
blinkUpdate:
	mov &blinkCount, r12  	;Load blinkCount into r12
	add #1, r12		;increment
	mov r12, &blinkCount

	cmp r12, r13
	jc limit		; blinkCount < blinkLimit

	mov #0, &blinkCount 	;Reset blinkCount
	mov #1, r12		;Set to on (1)
	call #greenControl
	jmp done

limit:
	mov #0, r12		;Set to off (0)
	call #greenControl

done:
	ret
